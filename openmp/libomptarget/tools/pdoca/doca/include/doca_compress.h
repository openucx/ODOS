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
 * @file doca_compress.h
 * @page DOCA COMPRESS
 * @defgroup DOCACOMPRESS DOCA COMPRESS engine
 * DOCA COMPRESS library. For more details please refer to the user guide on DOCA devzone.
 *
 * @{
 */
#ifndef DOCA_COMPRESS_H_
#define DOCA_COMPRESS_H_

#include <inttypes.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * DOCA core opaque types
 *********************************************************************************************************************/

struct doca_buf;
struct doca_ctx;
struct doca_dev;
struct doca_devinfo;

/*********************************************************************************************************************
 * DOCA COMPRESS Context
 *********************************************************************************************************************/

/**
 * Opaque structure representing a DOCA COMPRESS instance.
 */
struct doca_compress;

/**
 * Create a DOCA COMPRESS instance.
 *
 * @param [in] dev
 * The device to attach to the compress context
 * @param [out] compress
 * Pointer to pointer to be set to point to the created doca_compress instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - one or more of the arguments is null.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_compress.
 * - DOCA_ERROR_INITIALIZATION - failed to initialize a mutex.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_create(struct doca_dev *dev, struct doca_compress **compress);

/**
 * Destroy a DOCA COMPRESS instance.
 *
 * @param [in] compress
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_IN_USE - if unable to gain exclusive access to the compress instance
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_destroy(struct doca_compress *compress);

/**
 * Adapt doca_compress instance into a generalized context for use with doca core objects.
 *
 * @param [in] compress
 * Compress instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_compress_as_ctx(struct doca_compress *compress);

/**
 * Get the maximum number of tasks
 *
 * @details This method retrieves the maximum number of tasks for a device. Sum of num tasks should not exceed
 * this number.
 *
 * @param [in] compress
 * Compress context to get max number of tasks from
 * @param [out] max_num_tasks
 * Sum of num tasks should not exceed this number (@see doca_compress_task_compress_deflate_set_conf,
 * doca_compress_task_decompress_deflate_set_conf, doca_compress_task_decompress_lz4_set_conf)
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_get_max_num_tasks(struct doca_compress *compress, uint32_t *max_num_tasks);

/************************************************************************/
/* COMPRESS DEFLATE TASK                                                */
/************************************************************************/

/**
 * @brief Compress deflate task
 */
struct doca_compress_task_compress_deflate;

/**
 * @brief Function to execute on compress deflate task completion.
 *
 * @param [in] task
 * compress deflate task.
 * The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * task user data
 * @param [in] ctx_user_data
 * doca_ctx user data
 */
typedef void (*doca_compress_task_compress_deflate_completion_cb_t)(struct doca_compress_task_compress_deflate *task,
								    union doca_data task_user_data,
								    union doca_data ctx_user_data);

/**
 * @brief Check if a compress deflate task is supported by a device
 *
 * @details This method checks if a compress deflate task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query the device for its capabilities.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_compress_deflate_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Get compress deflate max size
 *
 * @details This method retrieves a compress deflate max size for a device
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_buffer_size
 * The max buffer size for compress deflate operation in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_compress_deflate_get_max_buf_size(const struct doca_devinfo *devinfo,
								      uint64_t *max_buffer_size);

/**
 * Get the maximum supported number of elements in DOCA linked-list buffer for compress deflate task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_buf_list_len
 * The maximum supported number of elements in DOCA linked-list buffer.
 * The value 1 indicates that only a single element is supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_compress_deflate_get_max_buf_list_len(const struct doca_devinfo *devinfo,
									  uint32_t *max_buf_list_len);

/**
 * @brief This method sets the compress deflate task configuration
 *
 * @param [in] compress
 * The compress context to config
 * @param [in] task_completion_cb
 * Task completion callback
 * @param [in] task_error_cb
 * Task error callback
 * @param [in] num_tasks
 * Number of compress deflate tasks that the context can allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_task_compress_deflate_set_conf(
	struct doca_compress *compress,
	doca_compress_task_compress_deflate_completion_cb_t task_completion_cb,
	doca_compress_task_compress_deflate_completion_cb_t task_error_cb,
	uint32_t num_tasks);

/**
 * @brief Allocate compress deflate task
 *
 * @details This method allocates and initializes a compress deflate task. Task parameters can be set later on
 * by setters.
 *
 * @param [in] compress
 * The compress context to allocate the task from
 * @param [in] src_buff
 * Source buffer
 * @param [in] dst_buff
 * Destination buffer
 * @param [in] user_data
 * doca_data that can be retrieved from the task (usually when the task is completed).
 * @param [out] task
 * The allocated task
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - all compress deflate tasks are already allocated.
 * - DOCA_ERROR_INVALID_VALUE - can not initialize source HW resources.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_task_compress_deflate_alloc_init(struct doca_compress *compress,
							    struct doca_buf const *src_buff,
							    struct doca_buf *dst_buff,
							    union doca_data user_data,
							    struct doca_compress_task_compress_deflate **task);

/**
 * @brief convert compress deflate task to doca_task
 *
 * @param [in] task
 * The task to convert
 * @return doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_compress_task_compress_deflate_as_task(struct doca_compress_task_compress_deflate *task);

/**
 * @brief set compress deflate task source
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] src_buff
 * Source buffer to set
 */
DOCA_EXPERIMENTAL
void doca_compress_task_compress_deflate_set_src(struct doca_compress_task_compress_deflate *task,
						 struct doca_buf const *src_buff);

/**
 * @brief get compress deflate task source
 *
 * @param [in] task
 * Task to get the source from
 *
 * @return source buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_compress_task_compress_deflate_get_src(
	const struct doca_compress_task_compress_deflate *task);

/**
 * @brief set compress deflate task destination
 *
 * @param [in] task
 * Task to set the destination to
 * @param [in] dst_buff
 * destination buffer to set
 */
DOCA_EXPERIMENTAL
void doca_compress_task_compress_deflate_set_dst(struct doca_compress_task_compress_deflate *task,
						 struct doca_buf *dst_buff);

/**
 * @brief get compress deflate task destination
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return destination buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_compress_task_compress_deflate_get_dst(const struct doca_compress_task_compress_deflate *task);

/**
 * @brief get compress deflate task CRC checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return CRC
 */
DOCA_EXPERIMENTAL
uint32_t doca_compress_task_compress_deflate_get_crc_cs(const struct doca_compress_task_compress_deflate *task);

/**
 * @brief get compress deflate task adler checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return adler
 */
DOCA_EXPERIMENTAL
uint32_t doca_compress_task_compress_deflate_get_adler_cs(const struct doca_compress_task_compress_deflate *task);

/************************************************************************/
/* DECOMPRESS DEFLATE TASK                                              */
/************************************************************************/

/**
 * @brief Decompress deflate task
 */
struct doca_compress_task_decompress_deflate;

/**
 * @brief Function to execute on decompress deflate task completion.
 *
 * @param [in] task
 * decompress deflate task.
 * The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * task user data
 * @param [in] ctx_user_data
 * doca_ctx user data
 */
typedef void (*doca_compress_task_decompress_deflate_completion_cb_t)(struct doca_compress_task_decompress_deflate *task,
								      union doca_data task_user_data,
								      union doca_data ctx_user_data);

/**
 * @brief Check if a decompress deflate task is supported by a device
 *
 * @details This method checks if a decompress deflate task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query the device for its capabilities.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_deflate_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Get decompress deflate max size
 *
 * @details This method retrieves decompress deflate max size
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_buffer_size
 * The max buffer size for decompress deflate operation in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_deflate_get_max_buf_size(const struct doca_devinfo *devinfo,
									uint64_t *max_buffer_size);

/**
 * Get the maximum supported number of elements in DOCA linked-list buffer for decompress deflate task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_buf_list_len
 * The maximum supported number of elements in DOCA linked-list buffer.
 * The value 1 indicates that only a single element is supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_deflate_get_max_buf_list_len(const struct doca_devinfo *devinfo,
									    uint32_t *max_buf_list_len);

/**
 * @brief This method sets the decompress deflate task configuration
 *
 * @param [in] compress
 * The compress context to config
 * @param [in] task_completion_cb
 * Task completion callback
 * @param [in] task_error_cb
 * Task error callback
 * @param [in] num_tasks
 * Number of decompress deflate tasks that the context can allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_INVALID_VALUE - can not initialize source HW resources.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_task_decompress_deflate_set_conf(
	struct doca_compress *compress,
	doca_compress_task_decompress_deflate_completion_cb_t task_completion_cb,
	doca_compress_task_decompress_deflate_completion_cb_t task_error_cb,
	uint32_t num_tasks);

/**
 * @brief Allocate decompress deflate task
 *
 * @details This method allocates and initializes a decompress deflate task. Task parameters can be set later on
 * by setters.
 *
 * @param [in] compress
 * The compress context to allocate the task from
 * @param [in] src_buff
 * Source buffer
 * @param [in] dst_buff
 * Destination buffer
 * @param [in] user_data
 * doca_data that can be retrieved from the task (usually when the task is completed).
 * @param [out] task
 * The allocated task
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - all compress deflate tasks are already allocated.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_task_decompress_deflate_alloc_init(struct doca_compress *compress,
							      struct doca_buf const *src_buff,
							      struct doca_buf *dst_buff,
							      union doca_data user_data,
							      struct doca_compress_task_decompress_deflate **task);

/**
 * @brief convert decompress deflate task to doca_task
 *
 * @param [in] task
 * The task to convert
 * @return doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_compress_task_decompress_deflate_as_task(struct doca_compress_task_decompress_deflate *task);

/**
 * @brief set decompress deflate task source
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] src_buff
 * Source buffer to set
 */
DOCA_EXPERIMENTAL
void doca_compress_task_decompress_deflate_set_src(struct doca_compress_task_decompress_deflate *task,
						   struct doca_buf const *src_buff);

/**
 * @brief get decompress deflate task source
 *
 * @param [in] task
 * Task to get the source from
 *
 * @return source buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_compress_task_decompress_deflate_get_src(
	const struct doca_compress_task_decompress_deflate *task);

/**
 * @brief set decompress deflate task destination
 *
 * @param [in] task
 * Task to set the destination to
 * @param [in] dst_buff
 * destination buffer to set
 */
DOCA_EXPERIMENTAL
void doca_compress_task_decompress_deflate_set_dst(struct doca_compress_task_decompress_deflate *task,
						   struct doca_buf *dst_buff);

/**
 * @brief get decompress deflate task destination
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return destination buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_compress_task_decompress_deflate_get_dst(const struct doca_compress_task_decompress_deflate *task);

/**
 * @brief get decompress deflate task CRC checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return CRC
 */
DOCA_EXPERIMENTAL
uint32_t doca_compress_task_decompress_deflate_get_crc_cs(const struct doca_compress_task_decompress_deflate *task);

/**
 * @brief get decompress deflate task adler checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return adler
 */
DOCA_EXPERIMENTAL
uint32_t doca_compress_task_decompress_deflate_get_adler_cs(const struct doca_compress_task_decompress_deflate *task);

/************************************************************************/
/* DECOMPRESS LZ4 TASK                                                  */
/************************************************************************/

/**
 * @brief decompress LZ4 task
 */
struct doca_compress_task_decompress_lz4;

/**
 * @brief Function to execute on decompress LZ4 task completion.
 *
 * @param [in] task
 * decompress LZ4 task.
 * The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * task user data
 * @param [in] ctx_user_data
 * doca_ctx user data
 */
typedef void (*doca_compress_task_decompress_lz4_completion_cb_t)(struct doca_compress_task_decompress_lz4 *task,
								  union doca_data task_user_data,
								  union doca_data ctx_user_data);

/**
 * @brief Check if a decompress LZ4 task is supported by a device
 *
 * @details This method checks if a decompress LZ4 task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query the device for its capabilities.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_DEPRECATED
doca_error_t doca_compress_cap_task_decompress_lz4_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Get decompress LZ4 max size
 *
 * @details This method retrieves decompress LZ4 max size
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_buffer_size
 * The max buffer size for decompress LZ4 operation in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_DEPRECATED
doca_error_t doca_compress_cap_task_decompress_lz4_get_max_buf_size(const struct doca_devinfo *devinfo,
								    uint64_t *max_buffer_size);

/**
 * Get the maximum supported number of elements in DOCA linked-list buffer for decompress LZ4 task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_buf_list_len
 * The maximum supported number of elements in DOCA linked-list buffer.
 * The value 1 indicates that only a single element is supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_DEPRECATED
doca_error_t doca_compress_cap_task_decompress_lz4_get_max_buf_list_len(const struct doca_devinfo *devinfo,
									uint32_t *max_buf_list_len);

/**
 * @brief This method sets the decompress LZ4 task configuration
 *
 * @param [in] compress
 * The compress context to config
 * @param [in] task_completion_cb
 * Task completion callback
 * @param [in] task_error_cb
 * Task error callback
 * @param [in] num_tasks
 * Number of decompress LZ4 tasks that the context can allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_DEPRECATED
doca_error_t doca_compress_task_decompress_lz4_set_conf(
	struct doca_compress *compress,
	doca_compress_task_decompress_lz4_completion_cb_t task_completion_cb,
	doca_compress_task_decompress_lz4_completion_cb_t task_error_cb,
	uint32_t num_tasks);

/**
 * @brief Allocate decompress LZ4 task
 *
 * @details This method allocates and initializes a decompress LZ4 task. Task parameters can be set later on
 * by setters.
 *
 * @param [in] compress
 * The compress context to allocate the task from
 * @param [in] src_buff
 * Source buffer
 * @param [in] dst_buff
 * Destination buffer
 * @param [in] user_data
 * doca_data that can be retrieved from the task (usually when the task is completed).
 * @param [out] task
 * The allocated task
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - all compress LZ4 tasks are already allocated.
 * - DOCA_ERROR_INVALID_VALUE - can not initialize source HW resources.
 *
 */
DOCA_DEPRECATED
doca_error_t doca_compress_task_decompress_lz4_alloc_init(struct doca_compress *compress,
							  struct doca_buf const *src_buff,
							  struct doca_buf *dst_buff,
							  union doca_data user_data,
							  struct doca_compress_task_decompress_lz4 **task);

/**
 * @brief convert decompress LZ4 task to doca_task
 *
 * @param [in] task
 * The task to convert
 * @return doca_task
 */
DOCA_DEPRECATED
struct doca_task *doca_compress_task_decompress_lz4_as_task(struct doca_compress_task_decompress_lz4 *task);

/**
 * @brief set decompress LZ4 task source
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] src_buff
 * Source buffer to set
 */
DOCA_DEPRECATED
void doca_compress_task_decompress_lz4_set_src(struct doca_compress_task_decompress_lz4 *task,
					       struct doca_buf const *src_buff);

/**
 * @brief get decompress LZ4 task source
 *
 * @param [in] task
 * Task to get the source from
 *
 * @return source buffer
 */
DOCA_DEPRECATED
struct doca_buf const *doca_compress_task_decompress_lz4_get_src(const struct doca_compress_task_decompress_lz4 *task);

/**
 * @brief set decompress LZ4 task destination
 *
 * @param [in] task
 * Task to set the destination to
 * @param [in] dst_buff
 * destination buffer to set
 */
DOCA_DEPRECATED
void doca_compress_task_decompress_lz4_set_dst(struct doca_compress_task_decompress_lz4 *task,
					       struct doca_buf *dst_buff);

/**
 * @brief get decompress LZ4 task destination
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return destination buffer
 */
DOCA_DEPRECATED
struct doca_buf *doca_compress_task_decompress_lz4_get_dst(const struct doca_compress_task_decompress_lz4 *task);

/**
 * @brief get decompress LZ4 task CRC checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return CRC
 */
DOCA_DEPRECATED
uint32_t doca_compress_task_decompress_lz4_get_crc_cs(const struct doca_compress_task_decompress_lz4 *task);

/**
 * @brief get decompress LZ4 task adler checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return adler
 */
DOCA_DEPRECATED
uint32_t doca_compress_task_decompress_lz4_get_adler_cs(const struct doca_compress_task_decompress_lz4 *task);

/************************************************************************/
/* DECOMPRESS LZ4 STREAM TASK                                           */
/************************************************************************/

/**
 * @brief decompress LZ4 stream task
 * This task expects a stream of one or more blocks, without the frame (i.e., the magic number, frame descriptor, and
 * content checksum).
 */
struct doca_compress_task_decompress_lz4_stream;

/**
 * @brief Function to execute on decompress LZ4 stream task completion.
 *
 * @param [in] task
 * decompress LZ4 stream task.
 * The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * task user data
 * @param [in] ctx_user_data
 * doca_ctx user data
 */
typedef void (*doca_compress_task_decompress_lz4_stream_completion_cb_t)(
	struct doca_compress_task_decompress_lz4_stream *task,
	union doca_data task_user_data,
	union doca_data ctx_user_data);

/**
 * @brief Check if a decompress LZ4 stream task is supported by a device
 *
 * @details This method checks if a decompress LZ4 stream task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query the device for its capabilities.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_lz4_stream_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Get decompress LZ4 stream max size
 *
 * @details This method retrieves decompress LZ4 stream max size
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_buffer_size
 * The max buffer size for decompress LZ4 stream operation in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_lz4_stream_get_max_buf_size(const struct doca_devinfo *devinfo,
									   uint64_t *max_buffer_size);

/**
 * Get the maximum supported number of elements in DOCA linked-list buffer for decompress LZ4 stream task
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_buf_list_len
 * The maximum supported number of elements in DOCA linked-list buffer.
 * The value 1 indicates that only a single element is supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_lz4_stream_get_max_buf_list_len(const struct doca_devinfo *devinfo,
									       uint32_t *max_buf_list_len);

/**
 * @brief This method sets the decompress LZ4 stream task configuration
 *
 * @param [in] compress
 * The compress context to config
 * @param [in] task_completion_cb
 * Task completion callback
 * @param [in] task_error_cb
 * Task error callback
 * @param [in] num_tasks
 * Number of decompress LZ4 stream tasks that the context can allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_task_decompress_lz4_stream_set_conf(
	struct doca_compress *compress,
	doca_compress_task_decompress_lz4_stream_completion_cb_t task_completion_cb,
	doca_compress_task_decompress_lz4_stream_completion_cb_t task_error_cb,
	uint32_t num_tasks);

/**
 * @brief Allocate decompress LZ4 stream task
 *
 * @details This method allocates and initializes a decompress LZ4 stream task. Task parameters can be set later on
 * by setters.
 *
 * @param [in] compress
 * The compress context to allocate the task from
 * @param [in] has_block_checksum
 * 1 if the task should expect blocks in the stream to have a checksum, 0 otherwise
 * @param [in] are_blocks_independent
 * 1 the the task should expect blocks to be independent, 0 otherwise (dependent blocks)
 * @param [in] src_buff
 * Source buffer
 * @param [in] dst_buff
 * Destination buffer
 * @param [in] user_data
 * doca_data that can be retrieved from the task (usually when the task is completed).
 * @param [out] task
 * The allocated task
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - all decompress LZ4 stream tasks are already allocated.
 * - DOCA_ERROR_INVALID_VALUE - can not initialize source HW resources.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_task_decompress_lz4_stream_alloc_init(struct doca_compress *compress,
								 uint8_t has_block_checksum,
								 uint8_t are_blocks_independent,
								 struct doca_buf const *src_buff,
								 struct doca_buf *dst_buff,
								 union doca_data user_data,
								 struct doca_compress_task_decompress_lz4_stream **task);

/**
 * @brief convert decompress LZ4 stream task to doca_task
 *
 * @param [in] task
 * The task to convert
 * @return doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_compress_task_decompress_lz4_stream_as_task(
	struct doca_compress_task_decompress_lz4_stream *task);

/**
 * @brief set decompress LZ4 stream task has_block_checksum flag
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] has_block_checksum
 * 1 if the task should expect blocks in the stream to have a checksum, 0 otherwise
 */
DOCA_EXPERIMENTAL
void doca_compress_task_decompress_lz4_stream_set_has_block_checksum(
	struct doca_compress_task_decompress_lz4_stream *task,
	uint8_t has_block_checksum);

/**
 * @brief get decompress LZ4 stream task has_block_checksum flag
 *
 *
 * @param [in] task
 * Task to get the has_block_checksum flag from
 *
 * @return 1 if the task should expect blocks in the stream to have a checksum, 0 otherwise
 */
DOCA_EXPERIMENTAL
uint8_t doca_compress_task_decompress_lz4_stream_get_has_block_checksum(
	const struct doca_compress_task_decompress_lz4_stream *task);

/**
 * @brief set decompress LZ4 stream task are_blocks_independent flag
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] are_blocks_independent
 * 1 the the task should expect blocks to be independent, 0 otherwise (dependent blocks)
 */
DOCA_EXPERIMENTAL
void doca_compress_task_decompress_lz4_stream_set_are_blocks_independent(
	struct doca_compress_task_decompress_lz4_stream *task,
	uint8_t are_blocks_independent);

/**
 * @brief get decompress LZ4 stream task are_blocks_independent flag
 *
 * @param [in] task
 * Task to get the are_blocks_independent flag from
 *
 * @return 1 if the task should expect blocks to be independent, 0 otherwise (dependent blocks)
 */
DOCA_EXPERIMENTAL
uint8_t doca_compress_task_decompress_lz4_stream_get_are_blocks_independent(
	const struct doca_compress_task_decompress_lz4_stream *task);

/**
 * @brief set decompress LZ4 stream task source
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] src_buff
 * Source buffer to set
 */
DOCA_EXPERIMENTAL
void doca_compress_task_decompress_lz4_stream_set_src(struct doca_compress_task_decompress_lz4_stream *task,
						      struct doca_buf const *src_buff);

/**
 * @brief get decompress LZ4 stream task source
 *
 * @param [in] task
 * Task to get the source from
 *
 * @return source buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_compress_task_decompress_lz4_stream_get_src(
	const struct doca_compress_task_decompress_lz4_stream *task);

/**
 * @brief set decompress LZ4 stream task destination
 *
 * @param [in] task
 * Task to set the destination to
 * @param [in] dst_buff
 * destination buffer to set
 */
DOCA_EXPERIMENTAL
void doca_compress_task_decompress_lz4_stream_set_dst(struct doca_compress_task_decompress_lz4_stream *task,
						      struct doca_buf *dst_buff);

/**
 * @brief get decompress LZ4 stream task destination
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return destination buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_compress_task_decompress_lz4_stream_get_dst(
	const struct doca_compress_task_decompress_lz4_stream *task);

/**
 * @brief get decompress LZ4 stream task CRC checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return CRC checksum of the stream
 */
DOCA_EXPERIMENTAL
uint32_t doca_compress_task_decompress_lz4_stream_get_crc_cs(
	const struct doca_compress_task_decompress_lz4_stream *task);

/**
 * @brief get decompress LZ4 stream task xxHash-32 checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the checksums from
 *
 * @return xxHash-32 checksum of the stream
 */
DOCA_EXPERIMENTAL
uint32_t doca_compress_task_decompress_lz4_stream_get_xxh_cs(
	const struct doca_compress_task_decompress_lz4_stream *task);

/************************************************************************/
/* DECOMPRESS LZ4 BLOCK TASK                                            */
/************************************************************************/

/**
 * @brief decompress LZ4 block task
 * This task expects a single, compressed, data-only block (i.e., without block size or block checksum).
 */
struct doca_compress_task_decompress_lz4_block;

/**
 * @brief Function to execute on decompress LZ4 block task completion.
 *
 * @param [in] task
 * decompress LZ4 block task.
 * The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * task user data
 * @param [in] ctx_user_data
 * doca_ctx user data
 */
typedef void (*doca_compress_task_decompress_lz4_block_completion_cb_t)(
	struct doca_compress_task_decompress_lz4_block *task,
	union doca_data task_user_data,
	union doca_data ctx_user_data);

/**
 * @brief Check if a decompress LZ4 block task is supported by a device
 *
 * @details This method checks if a decompress LZ4 block task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query the device for its capabilities.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_lz4_block_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Get decompress LZ4 block max size
 *
 * @details This method retrieves decompress LZ4 block max size
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_buffer_size
 * The max buffer size for decompress LZ4 block operation in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_lz4_block_get_max_buf_size(const struct doca_devinfo *devinfo,
									  uint64_t *max_buffer_size);

/**
 * Get the maximum supported number of elements in DOCA linked-list buffer for decompress LZ4 block task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_buf_list_len
 * The maximum supported number of elements in DOCA linked-list buffer.
 * The value 1 indicates that only a single element is supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_cap_task_decompress_lz4_block_get_max_buf_list_len(const struct doca_devinfo *devinfo,
									      uint32_t *max_buf_list_len);

/**
 * @brief This method sets the decompress LZ4 block task configuration
 *
 * @param [in] compress
 * The compress context to config
 * @param [in] task_completion_cb
 * Task completion callback
 * @param [in] task_error_cb
 * Task error callback
 * @param [in] num_tasks
 * Number of decompress LZ4 block tasks that the context can allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_task_decompress_lz4_block_set_conf(
	struct doca_compress *compress,
	doca_compress_task_decompress_lz4_block_completion_cb_t task_completion_cb,
	doca_compress_task_decompress_lz4_block_completion_cb_t task_error_cb,
	uint32_t num_tasks);

/**
 * @brief Allocate decompress LZ4 block task
 *
 * @details This method allocates and initializes a decompress LZ4 block task. Task parameters can be set later on
 * by setters.
 *
 * @param [in] compress
 * The compress context to allocate the task from
 * @param [in] src_buff
 * Source buffer
 * @param [in] dst_buff
 * Destination buffer
 * @param [in] user_data
 * doca_data that can be retrieved from the task (usually when the task is completed).
 * @param [out] task
 * The allocated task
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - all decompress LZ4 block tasks are already allocated.
 * - DOCA_ERROR_INVALID_VALUE - can not initialize source HW resources.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_compress_task_decompress_lz4_block_alloc_init(struct doca_compress *compress,
								struct doca_buf const *src_buff,
								struct doca_buf *dst_buff,
								union doca_data user_data,
								struct doca_compress_task_decompress_lz4_block **task);

/**
 * @brief convert decompress LZ4 block task to doca_task
 *
 * @param [in] task
 * The task to convert
 * @return doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_compress_task_decompress_lz4_block_as_task(struct doca_compress_task_decompress_lz4_block *task);

/**
 * @brief set decompress LZ4 block task source
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] src_buff
 * Source buffer to set
 */
DOCA_EXPERIMENTAL
void doca_compress_task_decompress_lz4_block_set_src(struct doca_compress_task_decompress_lz4_block *task,
						     struct doca_buf const *src_buff);

/**
 * @brief get decompress LZ4 block task source
 *
 * @param [in] task
 * Task to get the source from
 *
 * @return source buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_compress_task_decompress_lz4_block_get_src(
	const struct doca_compress_task_decompress_lz4_block *task);

/**
 * @brief set decompress LZ4 block task destination
 *
 * @param [in] task
 * Task to set the destination to
 * @param [in] dst_buff
 * destination buffer to set
 */
DOCA_EXPERIMENTAL
void doca_compress_task_decompress_lz4_block_set_dst(struct doca_compress_task_decompress_lz4_block *task,
						     struct doca_buf *dst_buff);

/**
 * @brief get decompress LZ4 block task destination
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return destination buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_compress_task_decompress_lz4_block_get_dst(
	const struct doca_compress_task_decompress_lz4_block *task);

/**
 * @brief get decompress LZ4 block task CRC checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return CRC checksum of the block
 */
DOCA_EXPERIMENTAL
uint32_t doca_compress_task_decompress_lz4_block_get_crc_cs(const struct doca_compress_task_decompress_lz4_block *task);

/**
 * @brief get decompress LZ4 block task xxHash-32 checksum
 *
 * @note Valid only on successful completion of the task. Otherwise, undefined behaviour.
 *
 * @param [in] task
 * Task to get the checksums from
 *
 * @return xxHash-32 checksum of the block
 */
DOCA_EXPERIMENTAL
uint32_t doca_compress_task_decompress_lz4_block_get_xxh_cs(const struct doca_compress_task_decompress_lz4_block *task);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_COMPRESS_H_ */

/** @} */
