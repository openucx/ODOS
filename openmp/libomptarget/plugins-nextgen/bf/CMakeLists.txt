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

set(DOCA_INCPATH "/opt/mellanox/doca/include" CACHE STRING "path to doca header files")
set(DOCA_LIBPATH "/opt/mellanox/doca/lib/aarch64-linux-gnu" CACHE STRING "path to doca libraries")

message( STATUS "doca   headers path: ${DOCA_INCPATH}" )
message( STATUS "doca libraries path: ${DOCA_LIBPATH}" )

find_file(DocaInc doca_dev.h        PATHS ${DOCA_INCPATH} NO_DEFAULT_PATH)
if(NOT DocaInc)
  message( FATAL_ERROR "failed to find doca headers in ${DOCA_INCPATH}")
endif()

find_file(DocaLib libdoca_common.so PATHS ${DOCA_LIBPATH} NO_DEFAULT_PATH)
if(NOT DocaLib)
  message( FATAL_ERROR "failed to find doca libraries in ${DOCA_LIBPATH}")
endif()


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
  ${DOCA_INCPATH})

target_link_libraries(
  "omptarget.rtl.bf" PRIVATE
  "${DOCA_LIBPATH}/libdoca_common.so"
  "${DOCA_LIBPATH}/libdoca_dma.so"
  "${DOCA_LIBPATH}/libdoca_comm_channel.so")

# Report to the parent scope that we are building a plugin.
set(LIBOMPTARGET_SYSTEM_TARGETS
  "${LIBOMPTARGET_SYSTEM_TARGETS} ${tmachine_triple}" PARENT_SCOPE)


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3 -fPIC")

