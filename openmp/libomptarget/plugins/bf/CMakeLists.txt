##===----------------------------------------------------------------------===##
#
# Build a plugin for a NEC Aurora machine if available. (Can also run on host)
#
##===----------------------------------------------------------------------===##


libomptarget_say("Building BlueField offloading plugin.")

set(tmachine_name "bf")
set(tmachine_libname "bf")
set(tmachine_triple "aarch64-unknown-linux-gnu")
set(elf_machine_id 183)

#set(DOCA_PATH "/global/software/centos-8.x86_64/modules/tools/doca/1.5.1/opt/mellanox/doca")
#/global/software/rocky-9.x86_64/modules/tools/doca/2.0.2/opt/mellanox/doca/lib64
set(DOCA_PATH "/global/software/rocky-9.x86_64/modules/tools/doca/2.0.2/opt/mellanox/doca")
set(additional_libs "${DOCA_PATH}/lib64/libdoca_common.so"
                    "${DOCA_PATH}/lib64/libdoca_dma.so"
                    "${DOCA_PATH}/lib64/libdoca_comm_channel.so")

add_definitions(-w)
# Define macro to be used as prefix of the runtime messages for this target.
add_definitions("-DTARGET_NAME=${tmachine_name}")

# Define macro with the ELF ID for this target.
add_definitions("-DTARGET_ELF_ID=${elf_machine_id}")

add_llvm_library("omptarget.rtl.${tmachine_libname}" 
    SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtl.cpp

    ADDITIONAL_HEADER_DIRS
    ${LIBOMPTARGET_INCLUDE_DIR}

    LINK_LIBS
    PRIVATE
	${additional_libs}

    NO_INSTALL_RPATH
)

# Install plugin under the lib destination folder.
install(TARGETS  "omptarget.rtl.${tmachine_libname}" LIBRARY DESTINATION "${OPENMP_INSTALL_LIBDIR}")
set_target_properties("omptarget.rtl.${tmachine_libname}" PROPERTIES 
    INSTALL_RPATH "$ORIGIN" BUILD_RPATH "$ORIGIN:${CMAKE_CURRENT_BINARY_DIR}/.."
    CXX_VISIBILITY_PRESET protected)

target_include_directories("omptarget.rtl.${tmachine_libname}" PRIVATE
    ${LIBOMPTARGET_INCLUDE_DIR}
	${DOCA_PATH}/include)

target_link_libraries(
	"omptarget.rtl.${tmachine_libname}" PRIVATE
	${additional_libs})

# Report to the parent scope that we are building a plugin.
set(LIBOMPTARGET_SYSTEM_TARGETS
    "${LIBOMPTARGET_SYSTEM_TARGETS} ${tmachine_triple}" PARENT_SCOPE)


set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3 -fPIC")
