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

#libomptarget_say("Building the doca-omp-service tool")
#libomptarget_say("doca-omp-service tool using DOCA libraries from ${DOCA_PATH}/lib/")

#set(CMAKE_C_FLAGS "-ldl -fopenmp -g -target aarch64-unknown-linux")


#add_openmp_tool(doca-omp-service doca-omp-service.c)

#llvm_update_compile_flags(llvm-omp-device-info)

#target_include_directories(doca-omp-service PRIVATE
#  ${LIBOMPTARGET_INCLUDE_DIR}
#)
#target_link_libraries(doca-omp-service PRIVATE
#  elf
#  ffi
#  pthrerad
#  ${DOCA_PATH}/lib64/libdoca_common.so
#  ${DOCA_PATH}/lib64/libdoca_dma.so
#  ${DOCA_PATH}/lib64/libdoca_comm_channel.so
#)


cmake_minimum_required(VERSION 3.20.0)

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


project(docaservice)

#CC=armclang

#set(CMAKE_C_COMPILER $CC)
set(CMAKE_C_FLAGS "-O1 -ldl -fopenmp")

add_definitions(-w)

add_executable(doca-omp-service doca-omp-service.c)

target_include_directories(doca-omp-service PRIVATE ${DOCA_INCPATH})


target_link_libraries(
  doca-omp-service PRIVATE
  elf
  ffi
  pthread
  ${DOCA_LIBPATH}/libdoca_dma.so
  ${DOCA_LIBPATH}/libdoca_common.so
  ${DOCA_LIBPATH}/libdoca_comm_channel.so
)
