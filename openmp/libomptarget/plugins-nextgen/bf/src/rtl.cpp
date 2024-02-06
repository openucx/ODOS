//===----RTLs/cuda/src/rtl.cpp - Target RTLs Implementation ------- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// RTL NextGen for BlueField DPUs
//
//===----------------------------------------------------------------------===//


#include "bf.hpp"

#include "Debug.h"
//#include "Environment.h"
#include "GlobalHandler.h"
#include "OmptCallback.h"
#include "PluginInterface.h"
#include "omptarget.h"


namespace llvm {
namespace omp {
namespace target {
namespace plugin {

/// Forward declarations for all specialized data structures.
struct DOCAKernelTy;
struct DOCADeviceImageTy;
struct DOCADeviceTy;
struct DOCAPluginTy;

class DOCAGloabalHandlerTy;

/// Class implementing common functionalities of offload kernels. Each plugin
/// should define the specific kernel class, derive from this generic one, and
/// implement the necessary virtual function members.
struct DOCAKernelTy : public GenericKernelTy {
  /// Create a DOCA kernel with a name, an execution mode, and the kernel
  /// function.
  DOCAKernelTy(const char *Name, OMPTgtExecModeFlags ExecutionMode, void (*Func)(void), BlueField *BF)
    : GenericKernelTy(Name, ExecutionMode), Func(Func), BF(BF) {}

  /// Initialize the DOCA kernel
  Error initImpl(GenericDeviceTy &GenericDevice,
                 DeviceImageTy &Image) override {
    // Set the maximum number of threads to a single.
    MaxNumThreads = 1; // get it from ARM ???
    return Plugin::success();
  }

  /// Launch the kernel.
  Error launchImpl(GenericDeviceTy &GenericDevice, uint32_t NumThreads,
                   uint64_t NumBlocks, KernelArgsTy &KernelArgs, void *Args,
                   AsyncInfoWrapperTy &AsyncInfoWrapper) const override {
	unsigned int i;
	struct bf_openmp_cmd cmd;
	cmd.cmd            = Command::CMD_RUN_TARGET_REGION;
	cmd.run.entry      = (int64_t)Func;
	cmd.run.total_args = (int64_t)KernelArgs.NumArgs;

	this->BF->send_buf(&cmd, sizeof(cmd));
	for (i = 0; i < KernelArgs.NumArgs; ++i) {
		this->BF->send_buf(static_cast<void ***>(Args)[i], sizeof(void *));
	}
    this->BF->recv_buf(this->BF->conn.rbuf, __MAX_SZ__);

    return Plugin::success();
  }

  /// Get the default number of blocks and threads for the kernel.
  uint32_t getDefaultNumBlocks(GenericDeviceTy &) const override { return 1; }
  uint32_t getDefaultNumThreads(GenericDeviceTy &) const override { return 1; }


private:
  /// The kernel function to execute.
  void (*Func)(void);
  BlueField *BF;

};

/// Class wrapping a __tgt_device_image and its offload entry table on a
/// specific device. This class is responsible for storing and managing
/// the offload entries for an image on a device.
struct DOCADeviceImageTy : public DeviceImageTy {
  /// Create the DOCA image with the id and the target image pointer.
  DOCADeviceImageTy(int32_t ImageId, const __tgt_device_image *TgtImage, BlueField *BF)
      : DeviceImageTy(ImageId, TgtImage), BF(BF) {}

  BlueField *BF;

};

/// Class implementing common functionalities of offload devices. Each plugin
/// should define the specific device class, derive from this generic one, and
/// implement the necessary virtual function members.
struct DOCADeviceTy : public GenericDeviceTy {
  // Create a DOCA device with a device id and the ??? grid values.
  DOCADeviceTy(int32_t DeviceId, int32_t NumDevices)
      : GenericDeviceTy(DeviceId, NumDevices, GenDOCAGridValues) {
   }

  ~DOCADeviceTy() {}

  /// Initialize the device, its resources and get its properties.
  Error initImpl(GenericPluginTy &Plugin) override {
    int Res;
    Res = BF.init(DeviceId, BlueField::CLIENTMODE);
    if (Res == -1) {
      return Plugin::error("failed to initialize bluefied device");
    }
    Res = BF.connect();
    if (Res == -1) {
    return Plugin::error("failed to connect bluefied device");
  }
    return Plugin::success();
  }

  /// Deinitialize the device and release its resources.
  Error deinitImpl() override {
  BF.deinit();
  return Plugin::success();
  }

  /// Allocate and construct a DOCA kernel.
  Expected<GenericKernelTy *>
  constructKernelEntry(const __tgt_offload_entry &KernelEntry,
                       DeviceImageTy &Image) {
    void *Addr = BF.DynLib.getAddressOfSymbol(KernelEntry.name);

    // Allocate and create the kernel.
    DOCAKernelTy *DOCAKernel =
        Plugin::get().allocate<DOCAKernelTy>();
    new (DOCAKernel) DOCAKernelTy(
        KernelEntry.name, OMP_TGT_EXEC_MODE_GENERIC,
        (void (*)())Addr, &BF);

    return DOCAKernel;
  }

  Error setContext() override { return Plugin::success(); }

  /// Load the binary image into the device and allocate an image object.
  Expected<DeviceImageTy *> loadBinaryImpl(const __tgt_device_image *TgtImage,
                                           int32_t ImageId) override {
    int Res;
    DOCADeviceImageTy *Image =
        Plugin::get().allocate<DOCADeviceImageTy>();
    new (Image) DOCADeviceImageTy(ImageId, TgtImage, &BF);
    Res = BF.DynLib.writeImage(Image->getStart(), Image->getSize());
    if (Res == -1)
      return Plugin::error("failed to load binary");
    return Image;
  }

  /// Allocate memory.
  void *allocate(size_t Size, void *, TargetAllocTy Kind) override {
    if (Size == 0)
      return nullptr;

    void *MemAlloc = nullptr;

    struct bf_openmp_cmd cmd;
    cmd.cmd     = Command::CMD_DATA_ALLOC;
    cmd.data.sz = Size;

    switch (Kind) {
    case TARGET_ALLOC_DEFAULT:
    case TARGET_ALLOC_DEVICE:
      BF.send_buf(&cmd,      sizeof(cmd   ));
      BF.recv_buf(&MemAlloc, sizeof(size_t));
      break;
    case TARGET_ALLOC_HOST:
      MemAlloc = std::malloc(Size);
    case TARGET_ALLOC_SHARED:
      break;
    }

    //printf("data alloc: 0x%09x (%ld)\n", (size_t)MemAlloc, (size_t)MemAlloc);

    return MemAlloc;
  }

  /// Free the memory.
  int free(void *TgtPtr, TargetAllocTy Kind) override {
        struct bf_openmp_cmd cmd;
                cmd.cmd      = Command::CMD_DATA_DELETE;
                cmd.data.ptr = (int64_t)TgtPtr;

    switch (Kind) {
    case TARGET_ALLOC_DEFAULT:
    case TARGET_ALLOC_DEVICE:
      BF.send_buf(&cmd, sizeof(cmd));
      break;
    case TARGET_ALLOC_HOST:
      std::free(TgtPtr);
      break;
    case TARGET_ALLOC_SHARED:
      break;
    }
    return OFFLOAD_SUCCESS;
  }

  /// Submit data to the device (host to device transfer).
  Error dataSubmitImpl(void *TgtPtr, const void *HstPtr, int64_t Size,
                       AsyncInfoWrapperTy &AsyncInfoWrapper) override {
    int Res;

    struct bf_openmp_cmd cmd;
        cmd.cmd      = Command::CMD_DATA_SUBMIT;
        cmd.data.sz  = Size;
        cmd.data.ptr = (int64_t)TgtPtr;
    BF.send_buf(&cmd, sizeof(cmd));
    Res = BF.send_buf(HstPtr, Size);
    if (Res == -1)
      return Plugin::error("data submit failed");

    return Plugin::success();
  }

  /// Retrieve data from the device (device to host transfer).
  Error dataRetrieveImpl(void *HstPtr, const void *TgtPtr, int64_t Size,
                         AsyncInfoWrapperTy &AsyncInfoWrapper) override {
        struct bf_openmp_cmd cmd;
                cmd.cmd      = Command::CMD_DATA_RETRIEVE;
                cmd.data.sz  = Size;
                cmd.data.ptr = (int64_t)TgtPtr;
        BF.send_buf(&cmd, sizeof(cmd));
        BF.recv_buf(HstPtr, Size); // no check ???
        return Plugin::success();

  }

  /// Exchange data between two devices within the plugin. This function is not
  /// supported in this plugin.
  Error dataExchangeImpl(const void *SrcPtr, GenericDeviceTy &DstGenericDevice,
                         void *DstPtr, int64_t Size,
                         AsyncInfoWrapperTy &AsyncInfoWrapper) override {
    // This function should never be called because the function
    return Plugin::error("dataExchangeImpl not supported");
  }


  /// This plugin does nothing to lock buffers. Do not return an error, just
  /// return the same pointer as the device pointer.
  Expected<void *> dataLockImpl(void *HstPtr, int64_t Size) override {
    return HstPtr;
  }

  /// Nothing to do when unlocking the buffer.
  Error dataUnlockImpl(void *HstPtr) override { return Plugin::success(); }

  /// Indicate that the buffer is not pinned.
  Expected<bool> isPinnedPtrImpl(void *HstPtr, void *&BaseHstPtr,
                                 void *&BaseDevAccessiblePtr,
                                 size_t &BaseSize) const override {
    return false;
  }

  /// All functions are already synchronous. No need to do anything on this
  /// synchronization function.
  Error synchronizeImpl(__tgt_async_info &AsyncInfo) override {
    return Plugin::success();
  }

  /// All functions are already synchronous. No need to do anything on this
  /// query function.
  Error queryAsyncImpl(__tgt_async_info &AsyncInfo) override {
    return Plugin::success();
  }

  /// This plugin does not support interoperability
  Error initAsyncInfoImpl(AsyncInfoWrapperTy &AsyncInfoWrapper) override {
    return Plugin::error("initAsyncInfoImpl not supported");
  }

  /// This plugin does not support interoperability
  Error initDeviceInfoImpl(__tgt_device_info *DeviceInfo) override {
    return Plugin::error("initDeviceInfoImpl not supported");
  }

  /// This plugin does not support the event API. Do nothing without failing.
  Error createEventImpl(void **EventPtrStorage) override {
    *EventPtrStorage = nullptr;
    return Plugin::success();
  }
  Error destroyEventImpl(void *EventPtr) override { return Plugin::success(); }
  Error recordEventImpl(void *EventPtr,
                        AsyncInfoWrapperTy &AsyncInfoWrapper) override {
    return Plugin::success();
  }
  Error waitEventImpl(void *EventPtr,
                      AsyncInfoWrapperTy &AsyncInfoWrapper) override {
    return Plugin::success();
  }
  Error syncEventImpl(void *EventPtr) override { return Plugin::success(); }


  /// Print information about the device.
  Error obtainInfoImpl(InfoQueueTy &Info) override {
   
    unsigned long i; 
    std::vector<std::string> labels;
    std::vector<std::string> info;

    BF.getInfo(labels, info);
    for (i = 0; i < labels.size(); ++i)
      Info.add(labels[i], info[i]);

    return Plugin::success();
  }

  /// This plugin should not setup the device environment.
  virtual bool shouldSetupDeviceEnvironment() const override { return false; };

  /// Getters and setters for stack size and heap size not relevant.
  Error getDeviceStackSize(uint64_t &Value) override {
    Value = 0;
    return Plugin::success();
  }
  Error setDeviceStackSize(uint64_t Value) override {
    return Plugin::success();
  }
  Error getDeviceHeapSize(uint64_t &Value) override {
    Value = 0;
    return Plugin::success();
  }
  Error setDeviceHeapSize(uint64_t Value) override { return Plugin::success(); }

private:
  /// Grid values for Generic ELF64 plugins.
  static constexpr GV GenDOCAGridValues = {
      1, // GV_Slot_Size
      1, // GV_Warp_Size
      1, // GV_Max_Teams
      1, // GV_SimpleBufferSize
      1, // GV_Max_WG_Size
      1, // GV_Default_WG_Size
  };

  struct BlueField BF;

};

class DOCAGlobalHandlerTy final : public GenericGlobalHandlerTy {
public:
  Error getGlobalMetadataFromDevice(GenericDeviceTy &GenericDevice,
                                    DeviceImageTy &Image,
                                    GlobalTy &DeviceGlobal) override {
    const char *GlobalName = DeviceGlobal.getName().data();
    DOCADeviceImageTy &DOCAImage = reinterpret_cast<DOCADeviceImageTy &>(Image);
    void *Addr = nullptr;
    Addr = DOCAImage.BF->DynLib.getAddressOfSymbol(GlobalName);

    // Save the pointer to the symbol.
    DeviceGlobal.setPtr(Addr);
    return Plugin::success();

  }

};



/// Class implementing common functionalities of offload plugins. Each plugin
/// should define the specific plugin class, derive from this generic one, and
/// implement the necessary virtual function members.
struct DOCAPluginTy final : public GenericPluginTy {

  DOCAPluginTy() : GenericPluginTy(getTripleArch()) {}

  /// This class should not be copied.
  DOCAPluginTy(const DOCAPluginTy &) = delete;
  DOCAPluginTy(DOCAPluginTy &&) = delete;

  /// Initialize the plugin and return the number of devices.
  Expected<int32_t> initImpl() override {
    int Res = bfGetTotalDevices();
    return Res;
  }

  /// Deinitialize the plugin and release the resources.
  Error deinitImpl() override {
	  return Plugin::success();
  }

  /// Get the ELF code to recognize the binary image of this plugin.
  uint16_t getMagicElfBits() const  override {
    return ELF::EM_AARCH64;
  }

  Expected<bool> isImageCompatible(__tgt_image_info *Info) const override {
    return true;
  }
  /// Get the target triple of this plugin.
  Triple::ArchType getTripleArch() const override {
    return Triple::aarch64;
  }

  /// Indicate whether data can be exchanged directly between two devices under
  /// this same plugin. If this function returns true, it's safe to call the
  /// GenericDeviceTy::exchangeData() function on the source device.
  virtual bool isDataExchangable(int32_t SrcDeviceId, int32_t DstDeviceId) {
    return false;
  }

};

/// Create a plugin instance.
GenericPluginTy *Plugin::createPlugin() { return new DOCAPluginTy(); }


/// Create a plugin-specific device.
GenericDeviceTy *Plugin::createDevice(int32_t DeviceId, int32_t NumDevices) {
  return new DOCADeviceTy(DeviceId, NumDevices);
}

GenericGlobalHandlerTy *Plugin::createGlobalHandler() {
  return new DOCAGlobalHandlerTy;
}

/// Check the plugin-specific error code and return an error or success
/// accordingly. In case of an error, create a string error with the error
/// description. The ErrFmt should follow the format:
///     "Error in <function name>[<optional info>]: %s"
/// The last format specifier "%s" is mandatory and will be used to place the
/// error code's description. Notice this function should be only called from
/// the plugin-specific code.
template <typename... ArgsTy>
Error Plugin::check(int32_t Code, const char *ErrMsg, ArgsTy... Args) {
  return Error::success();
  if (Code == 0)
    return Error::success();

  return createStringError<ArgsTy..., const char *>(
      inconvertibleErrorCode(), ErrMsg, Args..., std::to_string(Code).data());

}



} // namespace plugin
} // namespace target
} // namespace omp
} // namespace llvm

