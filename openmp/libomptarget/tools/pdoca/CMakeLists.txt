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

libomptarget_say("Building pDOCA")

add_library(pdoca SHARED pdoca.c pdoca_iface.c)

#llvm_update_compile_flags(llvm-omp-device-info)

target_include_directories(pdoca PRIVATE
  ${CMAKE_SOURCE_DIR}/libomptarget/tools/pdoca/doca/include/
  ${LIBOMPTARGET_INCLUDE_DIR}
)

install(TARGETS pdoca LIBRARY DESTINATION lib)

