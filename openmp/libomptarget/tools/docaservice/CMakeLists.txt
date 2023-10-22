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
libomptarget_say("doca-omp-service tool using DOCA libraries from ${DOCA_PATH}/lib/")

set(CMAKE_C_FLAGS "-ldl -fopenmp -g -target aarch64-unknown-linux")


add_openmp_tool(doca-omp-service doca-omp-service.c)

llvm_update_compile_flags(llvm-omp-device-info)

target_include_directories(doca-omp-service PRIVATE
  ${LIBOMPTARGET_INCLUDE_DIR}
)
target_link_libraries(doca-omp-service PRIVATE
  elf
  ffi
  pthrerad
  ${DOCA_PATH}/lib64/libdoca_common.so
  ${DOCA_PATH}/lib64/libdoca_dma.so
  ${DOCA_PATH}/lib64/libdoca_comm_channel.so
)
