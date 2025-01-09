/*
 * Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
 *
 * This software product is a proprietary product of NVIDIA CORPORATION &
 * AFFILIATES (the "Company") and all right, title, and interest in and to the
 * software product, including all associated intellectual property rights, are
 * and shall remain exclusively with the Company.
 *
 * This software product is governed by the End User License Agreement
 * provided with the software product.
 *
 */

/**
 * @file doca_mmap.h
 * @page doca mmap
 * @defgroup MMAP DOCA Memory Map
 * @ingroup DOCACore
 * The DOCA memory map provides a centralized repository and orchestration of several memory ranges registration for
 * each device attached to the memory map.
 *
 * @{
 */

#ifndef DOCA_MMAP_H_
#define DOCA_MMAP_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_dev;
struct doca_devinfo;

/*******************************************************************************
 * DOCA Memory Map
 ******************************************************************************/

/**
 * @brief Memory range for both local & remote systems.
 *
 * Definitions:
 * Memory Range - virtually contiguous fracture of memory space defined by address and length.
 * Remote Mmap - mmap containing memory that cannot be read locally.
 * Permissions - defines who can read/write from the memory range.
 * Export - method for allowing a remote target to read/write from the memory range
 *
 * Limitations:
 * - Memory map defines 1 memory range only
 *
 * @code
 * +---------------------+
 * |Memory map           |
 * |                     |
 * |                     |          +-----------------------------------------+
 * +---------+           |--------->| memory range  +------+  +----------+    |
 * | dev1    |           |          |               | data |  | data     |    |
 * +---------+           |          |               +------+  +----------+    |
 * | dev2    |           |          +-----------------------------------------+
 * +---------+           |
 * |                     |
 * +---------------------+
 * @endcode
 *
 */
struct doca_mmap;
struct doca_dpa;

/**
 * @brief Handle on the DPA for a doca_mmap instance.
 *
 */
typedef uint32_t doca_dpa_dev_mmap_t;

/**
 * @brief Allocates zero size memory map object with default/unset attributes.
 *
 * @details The returned memory map object can be manipulated with
 * doca_mmap_property_set() API.
 *
 * Once all required mmap attributes set it should be reconfigured
 * and adjusted to meet object size setting with doca_mmap_start()
 * See doca_mmap_start for the rest of the details.
 *
 * @param [out] mmap
 * DOCA memory map structure with default/unset attributes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_mmap.
 */
DOCA_STABLE
doca_error_t doca_mmap_create(struct doca_mmap **mmap);

/**
 * @brief Destroy DOCA Memory Map structure.
 *
 * @details Before calling this function all allocated buffers should be returned back to the mmap. Destroy implicitly
 * stops the mmap.
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_PERMITTED - if there is a memory region pointed by one or more `struct doca_buf`,
 *                              or if memory deregistration failed.
 */
DOCA_STABLE
doca_error_t doca_mmap_destroy(struct doca_mmap *mmap);

/**
 * @brief Start DOCA Memory Map.
 *
 * @details Allows execution of different operations on the mmap, detailed below.
 * On start verifies & finalizes the mmap object configuration.
 *
 * The following become possible only after start:
 * - Exporting the mmap using doca_mmap_export_*.
 * - Mapping doca_buf structures to the memory ranges in the using doca_buf_inventory_buf_get_by_addr() or
 *   doca_buf_inventory_buf_dup().
 *
 * The following are NOT possible while mmap is started:
 * - Setting the properties of the mmap through doca_mmap_set_*.
 * - Adding a device to the mmap using doca_mmap_add_dev().
 * - Removing a device to the mmap using doca_mmap_rm_dev().
 *
 * @param [in] mmap
 * DOCA memory map structure.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NO_MEMORY - if memory allocation failed.
 * - DOCA_ERROR_NOT_PERMITTED - if mmap is exported or created from export.
 */
DOCA_STABLE
doca_error_t doca_mmap_start(struct doca_mmap *mmap);

/**
 * @brief Stop DOCA Memory Map.
 *
 * @details Prevents execution of different operations and allows operations that were available before start.
 * For details see doca_mmap_start().
 * Frees any export descriptor received from doca_mmap_export_*, and invalidates any mmap created from this mmap export.
 * Stop does not have to be called before destroy (that implicitly stops the mmap).
 *
 * @param [in] mmap
 * DOCA memory map structure.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_PERMITTED - if mmap was exported or created from export,
 *				or buffers that were created for this mmap, are still not destroyed.
 */
DOCA_STABLE
doca_error_t doca_mmap_stop(struct doca_mmap *mmap);

/**
 * @brief Register DOCA memory map on a given device.
 *
 * @details This operation is not permitted for:
 * - started memory map object.
 * - memory map object that have been exported or created from export.
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] dev
 * DOCA Dev instance with appropriate capability.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_PERMITTED - if memory deregistration failed or the operation is not permitted for the given mmap
 *				(see details in this function description).
 * - DOCA_ERROR_NO_MEMORY - if reached to DOCA_MMAP_MAX_NUM_DEVICES.
 * - DOCA_ERROR_ALREADY_EXIST - if doca_dev already exists in doca_mmap.
 * - DOCA_ERROR_NOT_SUPPORTED - if dev was opened using doca_rdma_bridge_open_dev_from_pd(). Only relevant to memory
 *   				map objects created using doca_mmap_create().
 */
DOCA_STABLE
doca_error_t doca_mmap_add_dev(struct doca_mmap *mmap, struct doca_dev *dev);

/**
 * @brief Deregister given device from DOCA memory map.
 *
 * @details This operation is not permitted for:
 * - started memory map object.
 * - memory map object that have been exported or created from export.
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] dev
 * DOCA Dev instance that was previously added.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_FOUND - if doca_dev doesn't exist in doca_mmap.
 * - DOCA_ERROR_NOT_PERMITTED - if memory deregistration failed or the operation is not permitted for the given mmap
 *				(see details in this function description).
 */
DOCA_STABLE
doca_error_t doca_mmap_rm_dev(struct doca_mmap *mmap, struct doca_dev *dev);

/**
 * @brief Function to be called for each populated memory range on memory map destroy.
 *
 * @param[in] addr
 *   Memory range pointer.
 * @param[in] len
 *   Memory range length.
 * @param[in] cookie
 *   An opaque pointer passed to the callback.
 */
typedef void(doca_mmap_memrange_free_cb_t)(void *addr, size_t len, void *cookie);

/**
 * @brief Compose memory map representation for later import with
 * doca_mmap_create_from_export() for one of the devices previously added to
 * the memory map.
 *
 * @details Once this function called on the object it considered as exported.
 * The same mmap can be exported using different devices.
 * Once mmap is stopped then any mmap created from export will be invalidated, and the 'export_desc' is destroyed.
 *
 * This operation is not permitted for:
 * - un-started/stopped memory map object.
 * - memory map object that have been created from export.
 * - memory map with no PCI access permission set - see doca_mmap_set_permissions()
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] dev
 * Device previously added to the memory map via doca_mmap_add_dev().
 * Device must have export capability. See doca_mmap_cap_is_export_pci_supported()
 * @param [out] export_desc
 * On successful return should have a pointer to the allocated blob containing serialized representation of the memory
 * map object for the device provided as `dev`.
 * @param [out] export_desc_len
 * Length in bytes of the export_desc.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_FOUND - if device does not exist in mmap.
 * - DOCA_ERROR_NOT_PERMITTED - the operation is not permitted for the given mmap,
 *				see details in this function description.
 * The following errors will occur if failed to produce export descriptor:
 * - DOCA_ERROR_NO_MEMORY - if failed to alloc memory for export_desc.
 * - DOCA_ERROR_NOT_SUPPORTED - device missing export capability.
 * - DOCA_ERROR_DRIVER
 *
 */
DOCA_STABLE
doca_error_t doca_mmap_export_pci(struct doca_mmap *mmap,
				  const struct doca_dev *dev,
				  const void **export_desc,
				  size_t *export_desc_len);

/**
 * @brief Compose memory map representation for later import with
 * doca_mmap_create_from_export() for one of the devices previously added to
 * the memory map.
 * The imported mmap can then be used for RDMA operations
 *
 * @details Once this function called on the object it considered as exported.
 *
 * This operation is not permitted for:
 * - un-started/stopped memory map object.
 * - memory map objects that have been created from export.
 * - memory map with no RDMA access permission set - see doca_mmap_set_permissions()
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] dev
 * Device previously added to the memory map via doca_mmap_add_dev().
 * @param [out] export_desc
 * On successful return should have a pointer to the allocated blob containing serialized representation of the memory
 * map object for the device provided as `dev`.
 * @param [out] export_desc_len
 * Length in bytes of the export_desc.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_FOUND - if device does not exist in mmap.
 * - DOCA_ERROR_NOT_PERMITTED - the operation is not permitted for the given mmap,
 *				see details in this function description.
 * The following errors will occur if failed to produce export descriptor:
 * - DOCA_ERROR_NO_MEMORY - if failed to alloc memory for export_desc.
 * - DOCA_ERROR_NOT_SUPPORTED - device missing export capability,
 *   				or was opened using doca_rdma_bridge_open_dev_from_pd().
 * - DOCA_ERROR_DRIVER
 *
 */
DOCA_STABLE
doca_error_t doca_mmap_export_rdma(struct doca_mmap *mmap,
				   const struct doca_dev *dev,
				   const void **export_desc,
				   size_t *export_desc_len);

/**
 * @brief Extract mmap handle associated with the given DOCA device, for the DPA to operate on.
 *
 * @details Export the mmap to the DPA.
 *
 * This operation is not permitted for:
 * - un-started/stopped memory map object.
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] dev
 * Device previously added to the memory map via doca_mmap_add_dev().
 * @param [out] dpa_mmap_handle
 * On successful return should have a pointer to the mmap handle for the DPA.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_FOUND - if device does not exist in mmap.
 * - DOCA_ERROR_NOT_PERMITTED - the operation is not permitted for the given mmap,
 *				see details in this function description.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_mmap_dev_get_dpa_handle(struct doca_mmap *mmap,
					  const struct doca_dev *dev,
					  doca_dpa_dev_mmap_t *dpa_mmap_handle);

/**
 * @brief Creates a memory map object representing memory ranges in remote system
 * memory space.
 *
 * @details Once this function called on the object it considered as from_export.
 *
 * The following are NOT possible for the mmap created from export:
 * - Setting the properties of the mmap using doca_mmap_set_*().
 * - Adding a device to the mmap using doca_mmap_add_dev().
 * - Removing a device to the mmap using doca_mmap_rm_dev().
 * - Exporting the mmap using doca_mmap_export_*.
 *
 * @param [in] user_data
 * Identifier provided by user for the newly created DOCA memory map.
 * If not NULL, pointed user_data will be set.
 * @param [in] export_desc
 * An export descriptor generated by doca_mmap_export_*.
 * @param [in] export_desc_len
 * Length in bytes of the export_desc.
 * @param [in] dev
 * A local device connected to the device that resides in the exported mmap.
 * In case the 'export_desc' was created using doca_mmap_export_pci(), then device must have from export PCI capability.
 * See doca_mmap_cap_is_create_from_export_pci_supported(). And must be on same PCI bus.
 * @param [out] mmap
 * DOCA memory map granting access to remote memory.
 *
 * @note: The created object not backed by local memory.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received or internal error.
 * The following errors are internal and will occur if failed to produce new mmap from export descriptor:
 * - DOCA_ERROR_NO_MEMORY - if internal memory allocation failed.
 * - DOCA_ERROR_NOT_SUPPORTED - device missing create from export capability,
 *				or was opened using doca_rdma_bridge_open_dev_from_pd().
 * - DOCA_ERROR_NOT_PERMITTED
 * - DOCA_ERROR_DRIVER
 */
DOCA_STABLE
doca_error_t doca_mmap_create_from_export(const union doca_data *user_data,
					  const void *export_desc,
					  size_t export_desc_len,
					  struct doca_dev *dev,
					  struct doca_mmap **mmap);

/*******************************************************************************
 * DOCA Memory Map properties
 ******************************************************************************/

/**
 * @brief Set the memory range of DOCA memory map.
 *
 * @details  This operation is not permitted for:
 * - started memory map object.
 * - memory map object that have been exported or created from export.
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] addr
 * Start address of the memory range to be set.
 * @param [in] len
 * The size of the memory range in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received, or addr + len overflows.
 * - DOCA_ERROR_BAD_STATE - if mmap is started.
 * - DOCA_ERROR_NOT_PERMITTED - if mmap memory range was set before
 * @note: this property is mandatory and can be done only once
 */
DOCA_STABLE
doca_error_t doca_mmap_set_memrange(struct doca_mmap *mmap, void *addr, size_t len);

/**
 * @brief Set the memory range of DOCA memory map using dmabuf.
 *
 * @details  This operation is not permitted for:
 * - started memory map object.
 * - memory map object that have been exported or created from export.
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] dmabuf_fd
 * File descriptor of the dmabuf.
 * @param [in] addr
 * Start address of the memory range to be set.
 * @param [in] dmabuf_offset
 * Start offset of the dmabuf.
 * @param [in] len
 * The size of the memory range in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NOT_SUPPORTED - if not called from linux operating system
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received, or addr + len overflows.
 * - DOCA_ERROR_BAD_STATE - if mmap is started.
 * - DOCA_ERROR_NOT_PERMITTED - if mmap memory range was set before
 * @note: this property is mandatory and can be done only once. it is only supported when used on linux operating system
 */
DOCA_STABLE
doca_error_t doca_mmap_set_dmabuf_memrange(struct doca_mmap *mmap,
					   int dmabuf_fd,
					   void *addr,
					   size_t dmabuf_offset,
					   size_t len);

/**
 * @brief Set the memory range of DOCA memory map to be a DPA heap memory.
 *
 * @details  This operation is not permitted for:
 * - started memory map object.
 * - memory map object that have been exported or created from export.
 *
 * @note When using dpa memrange, adding DOCA devices using doca_mmap_add_dev() is not allowed.
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] dpa
 * DOCA DPA context associated with the memory pointed by the given 'dpa_addr'.
 * @param [in] dpa_addr
 * Start address of the memory range to be set on the DPA address space.
 * @param [in] len
 * The size of the memory range in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received, or addr + len overflows.
 * - DOCA_ERROR_BAD_STATE - if mmap is started.
 * - DOCA_ERROR_NOT_PERMITTED - if mmap memory range was set before
 * @note: this property is mandatory and can be done only once
 */
DOCA_EXPERIMENTAL
doca_error_t doca_mmap_set_dpa_memrange(struct doca_mmap *mmap, struct doca_dpa *dpa, uint64_t dpa_addr, size_t len);

/**
 * @brief Get the memory range of DOCA memory map.
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [out] addr
 * Start address of the memory range previously set.
 * @param [out] len
 * The size of the memory range in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_BAD_STATE - memrange was never set.
 */
DOCA_STABLE
doca_error_t doca_mmap_get_memrange(const struct doca_mmap *mmap, void **addr, size_t *len);

/**
 * @brief Set callback that will free the memory range when destroying DOCA memory map.
 *
 * @note Callback is called on mmap destroy, only in case the mmap was started and destroyed without changing the
 * callback.
 *
 * @param [in] mmap
 * DOCA memory map structure.
 * @param [in] free_cb
 * Callback function to free the set memory range on memory map destroy.
 * @param [out] opaque
 * User opaque value passed to free_cb.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_BAD_STATE - if mmap is started.
 */
DOCA_STABLE
doca_error_t doca_mmap_set_free_cb(struct doca_mmap *mmap, doca_mmap_memrange_free_cb_t *free_cb, void *opaque);

/**
 * @brief Get the user_data of a DOCA Memory Map.
 *
 * @note The user_data that was provided to the mmap upon its creation.
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 * @param [out] user_data
 * The user_data of mmap if set, otherwise 0.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 */
DOCA_STABLE
doca_error_t doca_mmap_get_user_data(const struct doca_mmap *mmap, union doca_data *user_data);

/**
 * @brief Get the max number of devices to add to a DOCA Memory Map.
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 * @param [out] max_num_devices
 * The max number of devices that can be added add to mmap.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 */
DOCA_STABLE
doca_error_t doca_mmap_get_max_num_devices(const struct doca_mmap *mmap, uint32_t *max_num_devices);

/**
 * @brief Get the Total number of `struct doca_buf` objects pointing to the memory in a DOCA Memory Map.
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 * @param [out] num_bufs
 * The total number of `struct doca_buf` objects pointing to the memory in mmap.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 */
DOCA_STABLE
doca_error_t doca_mmap_get_num_bufs(const struct doca_mmap *mmap, uint32_t *num_bufs);

/**
 * @brief Get the flag indicating if a DOCA Memory Map had been exported.
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 * @param [out] exported
 * 1 if mmap had been exported, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 */
DOCA_STABLE
doca_error_t doca_mmap_get_exported(const struct doca_mmap *mmap, uint8_t *exported);

/**
 * @brief Get the flag indicating if a DOCA Memory Map had been created from an export.
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 * @param [out] from_export
 * 1 if mmap had been created from export, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 */
DOCA_STABLE
doca_error_t doca_mmap_get_from_export(const struct doca_mmap *mmap, uint8_t *from_export);

/**
 * @brief Set a new max number of devices to add to a DOCA Memory Map.
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 * @param [in] max_num_devices
 * The new max number of devices that can be added add to mmap.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_PERMITTED - if trying to set the max number of devices after first start of the mmap.
 */
DOCA_STABLE
doca_error_t doca_mmap_set_max_num_devices(struct doca_mmap *mmap, uint32_t max_num_devices);

/**
 * @brief Set access flags of the registered memory
 *
 * @details this defines what kind of access the added devices have to the memory defined in mmap
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 * @param [in] access_mask
 * bitwise combination of access flags - see enum doca_access_flag
 *
 * @return
 * DOCA_SUCCESS - in case of success
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received or trying to set an undefined access flag,
 * or invalid combination
 * - DOCA_ERROR_BAD_STATE - If mmap is started
 */
DOCA_STABLE
doca_error_t doca_mmap_set_permissions(struct doca_mmap *mmap, uint32_t access_mask);

/**
 * @brief Set user_data for a DOCA Memory Map.
 *
 * @param [in] mmap
 * The DOCA memory map structure.
 * @param [out] user_data
 * The user_data to set for mmap.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - if mmap is started.
 */
DOCA_STABLE
doca_error_t doca_mmap_set_user_data(struct doca_mmap *mmap, union doca_data user_data);

/**
 * @brief Get the mmap export to PCI capability of a device.
 *
 * @details Get uint8_t value defining if the device can be used to export an mmap over the PCI.
 * See doca_mmap_export_pci()
 * true  - device can be used with the mmap export API.
 * false - export PCI API is guaranteed to fail with DOCA_ERROR_NOT_SUPPORTED.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] mmap_export
 * 1 if the mmap export PCI capability is supported, 0 otherwise.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query capability support.
 */
DOCA_STABLE
doca_error_t doca_mmap_cap_is_export_pci_supported(const struct doca_devinfo *devinfo, uint8_t *mmap_export);

/**
 * @brief Get the mmap create from export PCI capability of a device.
 *
 * @details Get uint8_t value defining if the device can be used to create an mmap from an exported mmap where
 * the exported mmap was created using doca_mmap_export_pci().
 * See doca_mmap_create_from_export()
 * true  - device can be used with the mmap create from export PCI API.
 * false - create from export API is guaranteed to fail with DOCA_ERROR_NOT_SUPPORTED.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] from_export
 * 1 if the mmap from export PCI capability is supported, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query capability support.
 */
DOCA_STABLE
doca_error_t doca_mmap_cap_is_create_from_export_pci_supported(const struct doca_devinfo *devinfo,
							       uint8_t *from_export);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_MMAP_H_ */
