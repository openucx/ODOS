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


cmake_minimum_required(VERSION 3.0.0)

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCH )
message( STATUS "Architecture: ${ARCH}" )
if( ${ARCH} STREQUAL "x86_64" )
	set(DOCA_PATH "/global/software/rocky-9.x86_64/modules/tools/doca/2.0.2/opt/mellanox/doca")
else()
	set(DOCA_PATH "/opt/mellanox/doca")
endif()
message( STATUS "DOCA: ${DOCA_PATH}" )





project(docaservice)

#CC=armclang

#set(CMAKE_C_COMPILER $CC)
set(CMAKE_C_FLAGS "-O1 -ldl -fopenmp")

add_definitions(-w)

add_executable(
	doca-omp-service
	doca-omp-service.c
)

target_include_directories(
	doca-omp-service PRIVATE
	${DOCA_PATH}/include
)

target_link_libraries(
	doca-omp-service PRIVATE
	elf
	ffi
	pthread
	${DOCA_PATH}/lib/aarch64-linux-gnu/libdoca_dma.so
	${DOCA_PATH}/lib/aarch64-linux-gnu/libdoca_common.so
	${DOCA_PATH}/lib/aarch64-linux-gnu/libdoca_comm_channel.so
)
