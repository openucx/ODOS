##===----------------------------------------------------------------------===##
#
# Build a plugin for BlueField DPUs
#
##===----------------------------------------------------------------------===##


libomptarget_say("Building BlueField offloading plugin.")

set(tmachine_name "bf")
#set(tmachine_libname "bf")
set(tmachine_triple "aarch64-unknown-linux-gnu")
set(elf_machine_id 183)

#set(DOCA_PATH "/global/software/centos-8.x86_64/modules/tools/doca/1.5.1/opt/mellanox/doca")
#/global/software/rocky-9.x86_64/modules/tools/doca/2.0.2/opt/mellanox/doca/lib64
#set(DOCA_PATH "/global/software/rocky-9.x86_64/modules/tools/doca/2.0.2/opt/mellanox/doca")
set(DOCA_PATH "/nethome/musman37/opt/doca")



set(additional_libs "${DOCA_PATH}/lib64/libdoca_common.so"
                    "${DOCA_PATH}/lib64/libdoca_dpa.so"
                    "${DOCA_PATH}/lib64/libdoca_dma.so"
                    "${DOCA_PATH}/lib64/libdoca_comm_channel.so")

#add_definitions(-w)
# Define macro to be used as prefix of the runtime messages for this target.
add_definitions("-DTARGET_NAME=bf")

# Define macro with the ELF ID for this target.
add_definitions("-DTARGET_ELF_ID=${elf_machine_id}")
add_definitions(-DDEBUG_PREFIX="TARGET ${tmachine_name} RTL")
add_definitions("-DLIBOMPTARGET_NEXTGEN_GENERIC_PLUGIN_TRIPLE=bf")

add_llvm_library("omptarget.rtl.bf" 
    SHARED
    src/rtl.cpp

	ADDITIONAL_HEADER_DIRS

  LINK_COMPONENTS
  LINK_LIBS

  LINK_LIBS PRIVATE
  elf_common
  MemoryManager
  PluginInterface
  ${OPENMP_PTHREAD_LIB}

  NO_INSTALL_RPATH

)

if ((OMPT_TARGET_DEFAULT) AND (LIBOMPTARGET_OMPT_SUPPORT))
  target_link_libraries("omptarget.rtl.bf" PRIVATE OMPT)
endif()


if (LIBOMP_HAVE_VERSION_SCRIPT_FLAG)
  target_link_libraries(omptarget.rtl.bf PRIVATE
  "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/../exports,-z,defs")
endif()

target_include_directories(omptarget.rtl.bf PRIVATE dynamic_bf)
target_sources(omptarget.rtl.bf PRIVATE dynamic_bf/bf.cpp)


list(APPEND LIBOMPTARGET_TESTED_PLUGINS "omptarget.rtl.bf")
set(LIBOMPTARGET_TESTED_PLUGINS
    "${LIBOMPTARGET_TESTED_PLUGINS}" PARENT_SCOPE)
set(LIBOMPTARGET_SYSTEM_TARGETS 
        "${LIBOMPTARGET_SYSTEM_TARGETS} ${tmachine_triple} ${tmachine_triple}-LTO" PARENT_SCOPE)



# Install plugin under the lib destination folder.
install(TARGETS "omptarget.rtl.bf" LIBRARY DESTINATION "${OPENMP_INSTALL_LIBDIR}")
set_target_properties("omptarget.rtl.bf" PROPERTIES 
    INSTALL_RPATH "$ORIGIN" BUILD_RPATH "$ORIGIN:${CMAKE_CURRENT_BINARY_DIR}/.."
    CXX_VISIBILITY_PRESET protected)



# these should be in path LIBOMPTARGET_INCLUDE_DIR
target_include_directories("omptarget.rtl.bf" PRIVATE
   ${LIBOMPTARGET_INCLUDE_DIR}
   ${DOCA_PATH}/include)

target_link_libraries(
	"omptarget.rtl.bf" PRIVATE
	${additional_libs})

# Report to the parent scope that we are building a plugin.
set(LIBOMPTARGET_SYSTEM_TARGETS
    "${LIBOMPTARGET_SYSTEM_TARGETS} ${tmachine_triple}" PARENT_SCOPE)


set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3 -fPIC")
