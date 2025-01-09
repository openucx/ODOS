##===----------------------------------------------------------------------===##
#
# Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
##===----------------------------------------------------------------------===##
#
# Build llvm-omp-device-info tool
#
##===----------------------------------------------------------------------===##

libomptarget_say("Building the doca-omp-service tool")

#set(CMAKE_C_FLAGS "-ldl -fopenmp -g -target aarch64-unknown-linux")
set(CMAKE_C_FLAGS "-ldl -fopenmp -g")

set(DOCA_INCPATH "${CMAKE_SOURCE_DIR}/libomptarget/tools/pdoca/doca/include/" CACHE STRING "path to doca header files")
set(DOCA_LIBPATH "${CMAKE_SOURCE_DIR}/libomptarget/tools/pdoca/doca/lib/x86_64-linux-gnu" CACHE STRING "path to doca libraries")


message( STATUS "doca   headers path: ${DOCA_INCPATH}" )
message( STATUS "doca libraries path: ${DOCA_LIBPATH}" )


add_openmp_tool(doca-omp-service doca-omp-service.c)

llvm_update_compile_flags(llvm-omp-device-info)

target_include_directories(doca-omp-service PRIVATE
  ${LIBOMPTARGET_INCLUDE_DIR}
  ${DOCA_INCPATH}
)
target_link_libraries(doca-omp-service PRIVATE
  elf
  ffi
  ${DOCA_LIBPATH}/libdoca_common.so
  ${DOCA_LIBPATH}/libdoca_dma.so
  ${DOCA_LIBPATH}/libdoca_comm_channel.so
)



