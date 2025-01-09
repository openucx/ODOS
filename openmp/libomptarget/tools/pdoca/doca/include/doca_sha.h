/*
 * Copyright (c) 2022-2023 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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

#ifndef DOCA_SHA_H_
#define DOCA_SHA_H_

#include <stdint.h>

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

/**********************************************************************************************************************
 * DOCA SHA context
 *********************************************************************************************************************/

/** Opaque type representing a doca_sha instance */
struct doca_sha;

/**
 * Create an instance of doca_sha
 *
 * @param [in] dev
 * The device this doca_sha instance will operate with
 *
 * @param [out] sha
 * Pointer to a pointer which will be populated to point to the newly created doca_sha instance upon success. Caller
 * assumes ownership of the created doca_sha instance and is responsible for destroying it later when no longer
 * required
 *
 * @return
 * DOCA_SUCCESS - sha instance was created and *sha points to a valid object
 * Any other status code means the operation failed and *sha may be uninitialized. Typical error codes for this
 * function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NO_MEMORY - Failed to create a new object.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_create(struct doca_dev *dev, struct doca_sha **sha);

/**
 * Destroy an instance of doca_sha.
 *
 * @param [in] sha
 * Pointer to the doca_sha instance to destroy
 *
 * @return
 * DOCA_SUCCESS - sha instance was destroyed. sha is now a dangling pointer and should not be referenced to again.
 * Any other status code means the operation failed. Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument.
 *  - DOCA_ERROR_IN_USE - Object is in use and should be stopped before destruction.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_destroy(struct doca_sha *sha);

/**
 * Adapt a pointer to a doca_sha instance to a general doca_ctx pointer for use with core objects. This does not imply
 * any transfer of ownership. The existing owner of the doca_sha instance continues to be responsible for its' lifetime
 * and must ensure it outlives any core objects which refer to it.
 *
 * @param [in] sha
 * doca_sha instance.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_sha_as_ctx(struct doca_sha *sha);

/*********************************************************************************************************************
 * DOCA SHA Algorithms
 *********************************************************************************************************************/

/**
 * Doca sha action type enums, used to specify sha task types.
 */
enum doca_sha_algorithm {
	DOCA_SHA_ALGORITHM_SHA1,
	DOCA_SHA_ALGORITHM_SHA256,
	DOCA_SHA_ALGORITHM_SHA512,
};

/**********************************************************************************************************************
 * DOCA SHA support and capabilities
 *********************************************************************************************************************/

/**
 * Query if the device represented by the given devinfo is capable of performing doca_sha hash tasks using a given
 * algorithm.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [in] algorithm
 * The algorithm to check support for.
 *
 * @return
 * DOCA_SUCCESS - The device represented by this devinfo is capable of performing the desired operation
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NOT_SUPPORTED the desired operation is not supported by this device.
 * Any other error reported likely means the query failed to execute.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_cap_task_hash_get_supported(struct doca_devinfo const *devinfo,
						  enum doca_sha_algorithm algorithm);

/**
 * Query if the device represented by the given devinfo is capable of performing doca_sha partial hash tasks using a
 * given algorithm.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [in] algorithm
 * The algorithm to check support for.
 *
 * @return
 * DOCA_SUCCESS - The device represented by this devinfo is capable of performing the desired operation
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NOT_SUPPORTED the desired operation is not supported by this device.
 * Any other error reported likely means the query failed to execute.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_cap_task_partial_hash_get_supported(struct doca_devinfo const *devinfo,
							  enum doca_sha_algorithm algorithm);

/**
 * Query the maximum number of chained doca buffers which can be processed for any task submitted to doca_sha
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_list_num_elem
 * The maximum supported number of elements in DOCA linked-list buffer. The value 1 indicates that only a single element
 * is supported.
 *
 * @return
 * DOCA_SUCCESS - The query executed successfully, *max_list_num_elem holds a valid value
 * Upon failure *max_list_num_elem may be uninitialized. Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NOT_SUPPORTED - Device has no doca_sha support.
 * Any other error reported likely means the query failed to execute.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_cap_get_max_list_buf_num_elem(struct doca_devinfo const *devinfo, uint32_t *max_list_num_elem);

/**
 * Query the maximum length of data that can be submitted with a task (chained or un-chained)
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_buf_size
 * The maximum supported buffer size in bytes.
 *
 * @return
 * DOCA_SUCCESS - The query executed successfully, *max_buf_size holds a valid value
 * Upon failure *max_buf_size may be uninitialized. Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NOT_SUPPORTED - Device has no doca_sha support.
 * Any other error reported likely means the query failed to execute.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_cap_get_max_src_buf_size(struct doca_devinfo const *devinfo, uint64_t *max_buf_size);

/**
 * Query the minimum length of an output buffer for any doca_sha task. A task must have at least this much space in its'
 * output buffer but it may not write to all of it, the actual size of the result can be queried from the output doca
 * buffer when the task has produced an output. Providing an output buffer with less space than this is undefined
 * behavior. The user is responsible for ensuring this requirement is met, it will not be checked during task submission
 * unless task verification is requested.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [in] algorithm
 * The algorithm for which to query this capability.
 * @param [out] min_buf_size
 * The minimum required output buffer space.
 *
 * @return
 * DOCA_SUCCESS - The query executed successfully, *min_buf_size holds a valid value
 * Upon failure *min_buf_size may be uninitialized. Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NOT_SUPPORTED - Device has no doca_sha support.
 * Any other error reported likely means the query failed to execute.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_cap_get_min_dst_buf_size(struct doca_devinfo const *devinfo,
					       enum doca_sha_algorithm algorithm,
					       uint32_t *min_buf_size);

/**
 * All intermediate (non final) input buffers submitted for a partial SHA task must be a multiple of the value returned
 * form this function. The user is responsible for ensuring this requirement is met, it will not be checked during task
 * submission unless task verification is requested.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [in] algorithm
 * The algorithm for which to query this capability.
 * @param [out] partial_block_size
 * The partial input buffer block size.
 *
 * @return
 * DOCA_SUCCESS - The query executed successfully, *partial_block_size holds a valid value
 * Upon failure *partial_block_size may be uninitialized. Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NOT_SUPPORTED - Device has no doca_sha support.
 * Any other error reported likely means the query failed to execute.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_cap_get_partial_hash_block_size(struct doca_devinfo const *devinfo,
						      enum doca_sha_algorithm algorithm,
						      uint32_t *partial_block_size);

/**********************************************************************************************************************
 * DOCA SHA hash task
 *********************************************************************************************************************/

/** Opaque type representing a doca_sha_task_hash instance */
struct doca_sha_task_hash;

/**
 * Signature of a doca_sha_task_hash completion or error call-back.
 *
 * Note: A callback may choose to invoke doca_task_free to release the task during the execution of the callback.
 *
 * @param [in] task
 * Completed task. The implementation can assume this value is never NULL.
 * @param [in] task_user_data
 * User data as provided during doca_sha_task_hash_alloc_init.
 * @param [in] ctx_user_data
 * User data as provided during doca_ctx_set_user_data.
 */
typedef void (*doca_sha_task_hash_completion_cb_t)(struct doca_sha_task_hash *task,
						   union doca_data task_user_data,
						   union doca_data ctx_user_data);

/**
 * @brief This method sets the doca_sha hash task pool configuration.
 *
 * @param [in] sha
 * The SHA context to configure.
 * @param [in] task_completion_cb
 * Task completion callback.
 * @param [in] task_error_cb
 * Task error callback.
 * @param [in] log_num_tasks
 * Log2 of number of tasks.
 *
 * @return
 * DOCA_SUCCESS
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_BAD_STATE - The context is currently running (started).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_hash_set_conf(struct doca_sha *sha,
					 doca_sha_task_hash_completion_cb_t task_completion_cb,
					 doca_sha_task_hash_completion_cb_t task_error_cb,
					 uint8_t log_num_tasks);

/**
 * Allocate doca_sha hash task
 *
 * This method allocates and initializes a task. The user can choose to modify the task using the associated
 * setters while the task is not in flight. The user may choose to reconfigure and reuse a task or to release it and get
 * a new task as per their own requirements.
 *
 * @param [in] sha
 * The doca_sha context to allocate the task from.
 * @param [in] algorithm
 * SHA algorithm to use.
 * @param [in] src_buf
 * Source buffer.
 * @param [in] dst_buf
 * Destination buffer.
 * @param [in] user_data
 * doca_data that can be retrieved from the task (usually when the task is completed).
 * @param [out] task
 * The allocated task.
 *
 * @return
 * DOCA_SUCCESS - Operation succeeded, *task holds a valid value.
 * Upon failure *task may be uninitialized. Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NO_MEMORY - All tasks in the pool are in use.
 *  - DOCA_ERROR_BAD_STATE - The context is not currently running (stopped).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_hash_alloc_init(struct doca_sha *sha,
					   enum doca_sha_algorithm algorithm,
					   struct doca_buf const *src_buf,
					   struct doca_buf *dst_buf,
					   union doca_data user_data,
					   struct doca_sha_task_hash **task);

/**
 * Set a doca buffer as the source data for this task.
 *
 * @param [in] task
 * Task to configure.
 * @param [in] src_buf
 * Source buffer to use.
 *
 * @return
 * DOCA_SUCCESS - Operation succeeded
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_hash_set_src(struct doca_sha_task_hash *task, struct doca_buf const *src_buf);

/**
 * Get the current source buffer in use by this task.
 *
 * The user is responsible to ensure this function is only called for a task which has been returned by alloc_init and
 * has not yet been released. Calling this function in that circumstance is undefined behavior.
 *
 * @param [in] task
 * Task to query.
 *
 * @return
 * Pointer to the source buffer.
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_sha_task_hash_get_src(struct doca_sha_task_hash const *task);

/**
 * Set a doca buffer as the destination data for this task.
 *
 * @param [in] task
 * Task to configure.
 * @param [in] dst_buf
 * Destination buffer to use.
 *
 * @return
 * DOCA_SUCCESS - Operation succeeded
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_hash_set_dst(struct doca_sha_task_hash *task, struct doca_buf *dst_buf);

/**
 * Get the current destination buffer in use by this task.
 *
 * The user is responsible to ensure this function is only called for a task which has been returned by alloc_init and
 * has not yet been released. Calling this function in that circumstance is undefined behavior.
 *
 * @param [in] task
 * Task to query.
 *
 * @return
 * Pointer to the destination buffer.
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_sha_task_hash_get_dst(struct doca_sha_task_hash const *task);

/**
 * Set the SHA algorithm to use for this task.
 *
 * @param [in] task
 * Task to configure.
 * @param [in] algorithm
 * SHA algorithm to use.
 *
 * @return
 * DOCA_SUCCESS - Operation succeeded
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_hash_set_algorithm(struct doca_sha_task_hash *task, enum doca_sha_algorithm algorithm);

/**
 * Get the current algorithm in use by this task.
 *
 * The user is responsible to ensure this function is only called for a task which has been returned by alloc_init and
 * has not yet been released. Calling this function in that circumstance is undefined behavior.
 *
 * @param [in] task
 * Task to query.
 *
 * @return
 * Algorithm in use.
 */
DOCA_EXPERIMENTAL
enum doca_sha_algorithm doca_sha_task_hash_get_algorithm(struct doca_sha_task_hash const *task);

/**
 * Adapt a pointer to a doca_sha hash task instance to a general doca_task pointer for use with core objects. This does
 * not imply any transfer of ownership. The existing owner of the task continues to be responsible for its' lifetime and
 * must ensure it remains alive until it has completed (successfully, with an error or it was flushed).
 *
 * @param [in] task
 * Task to adapt to a doca_task.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_sha_task_hash_as_task(struct doca_sha_task_hash *task);

/**********************************************************************************************************************
 * DOCA SHA partial hash task
 *********************************************************************************************************************/

/**
 * A partial hash task allows the SHA calculation to be broken up into multiple steps. This allows for calculating the
 * SHA as more data becomes available or to calculate the SHA of very large data. This is achieved by allocating a
 * partial hash task once then submitting it multiple times. Each time the partial task is completed the user will
 * update the source buffer to point to the next section of data and then resubmit the task effectively iterating over
 * the input data until it has all be submitted to the partial hash task. To get the final calculated SHA result the
 * user must know when the data pointed to by the source buffer is the last segment of data and mark the task as final
 * by making a call to:
 *     doca_sha_task_partial_hash_set_is_final_buf.
 * Once the final buffer of the task has completed successfully the destination buffer will hold the computed SHA of
 * the data as if it has been processed in one go. From here the user may choose to either release the task or reset
 * it and start the process over once again. To reset the task to start again the user can call:
 *     doca_sha_task_partial_hash_reset
 *
 * Note: Partial hash task source buffers have additional constraints on the source buffer then are imposed for a
 * regular hash task:
 *
 * 1) All non-final source buffers must have a size which is an exact multiple of the SHA block size for the given
 *    device and algorithm. See: doca_sha_cap_get_partial_hash_block_size
 *
 *    Note: The additional constraints do NOT apply to the final source buffer and it only has to adhere to the
 *    normal constraints of a normal partial hash task.
 */

/** Opaque type representing a doca_sha_task_partial_hash instance */
struct doca_sha_task_partial_hash;

/**
 * Signature of doca_sha_task_partial_hash completion or error call-back.
 *
 * Note: A callback may choose to invoke doca_task_free to release the task during the execution of the callback.
 *
 * @param [in] task
 * Completed task. The implementation can assume this value is never NULL.
 * @param [in] task_user_data
 * User data as provided during doca_sha_task_hash_alloc_init.
 * @param [in] ctx_user_data
 * User data as provided during doca_ctx_set_user_data.
 */
typedef void (*doca_sha_task_partial_hash_completion_cb_t)(struct doca_sha_task_partial_hash *task,
							   union doca_data task_user_data,
							   union doca_data ctx_user_data);

/**
 * @brief This method sets the doca_sha partial hash task pool configuration
 *
 * @param [in] sha
 * The SHA context to configure.
 * @param [in] task_completion_cb
 * Task completion callback.
 * @param [in] task_error_cb
 * Task error callback.
 * @param [in] log_num_tasks
 * Log2 of number of tasks.
 *
 * @return
 * DOCA_SUCCESS
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_BAD_STATE - The context is currently running (started).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_partial_hash_set_conf(struct doca_sha *sha,
						 doca_sha_task_partial_hash_completion_cb_t task_completion_cb,
						 doca_sha_task_partial_hash_completion_cb_t task_error_cb,
						 uint8_t log_num_tasks);

/**
 * Allocate doca_sha hash task
 *
 * This method allocates and initializes a task. The user can choose to modify the task using the associated
 * setters while the task is not in flight. With partial tasks it is expected that a user will re-use and re-submit the
 * same task with a new source buffer each time to progress the partial SHA calculation. Once the final part of a
 * partial SHA is completed the user can then either reset or release the task.
 *
 * @param [in] sha
 * The doca_sha context to allocate the task from.
 * @param [in] algorithm
 * SHA algorithm to use.
 * @param [in] src_buf
 * Source buffer.
 * @param [in] dst_buf
 * Destination buffer.
 * @param [in] user_data
 * doca_data that can be retrieved from the task (Only set after the final task is completed)
 * @param [out] task
 * The allocated task.
 *
 * @return
 * DOCA_SUCCESS - Operation succeeded, *task holds a valid value.
 * Upon failure *task may be uninitialized. Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_NO_MEMORY - All tasks in the pool are in use.
 *  - DOCA_ERROR_BAD_STATE - The context is not currently running (stopped).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_partial_hash_alloc_init(struct doca_sha *sha,
						   enum doca_sha_algorithm algorithm,
						   struct doca_buf const *src_buf,
						   struct doca_buf *dst_buf,
						   union doca_data user_data,
						   struct doca_sha_task_partial_hash **task);

/**
 * Reset the partial state of this task so it can be re-used. This can be called at any time. All existing partial
 * session state will be lost and cannot be recovered.
 *
 * @param [in] task
 * Task to reset.
 *
 * @return
 * DOCA_SUCCESS- Operation succeeded
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_partial_hash_reset(struct doca_sha_task_partial_hash *task);

/**
 * Set a doca buffer as the source data for this task.
 *
 * NOTE: If this is the non final segment of a partial hash it must have a data length that is a multiple of the SHA
 * block size. The user is responsible to ensuring this, failure to ensure this is undefined behavior. The final source
 * buffer is not required to be a multiple of the SHA block size.
 *
 * @param [in] task
 * Task to configure.
 * @param [in] src_buf
 * Source buffer to use.
 *
 * @return
 * DOCA_SUCCESS- Operation succeeded
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_partial_hash_set_src(struct doca_sha_task_partial_hash *task,
						struct doca_buf const *src_buf);

/**
 * Get the current source buffer in use by this task.
 *
 * The user is responsible to ensure this function is only called for a task which has been returned by alloc_init and
 * has not yet been released. Calling this function in that circumstance is undefined behavior.
 *
 * @param [in] task
 * Task to query.
 *
 * @return
 * Pointer to the source buffer.
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_sha_task_partial_hash_get_src(struct doca_sha_task_partial_hash const *task);

/**
 * Set a doca buffer as the destination data for this task.
 *
 * This value can only be set before the first task is sent. (after alloc_init, or after calling reset)
 * @param [in] task
 * Task to configure
 * @param [in] dst_buf
 * Destination buffer to use.
 *
 * @return
 * DOCA_SUCCESS- Operation succeeded
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_BAD_STATE - The partial hash is in progress
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_partial_hash_set_dst(struct doca_sha_task_partial_hash *task, struct doca_buf *dst_buf);

/**
 * Get the current destination buffer in use by this task.
 *
 * The user is responsible to ensure this function is only called for a task which has been returned by alloc_init and
 * has not yet been released. Calling this function in that circumstance is undefined behavior.
 *
 * @param [in] task
 * Task to query.
 *
 * @return
 * Pointer to the destination buffer.
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_sha_task_partial_hash_get_dst(struct doca_sha_task_partial_hash const *task);

/**
 * Set the SHA algorithm to use for this task.
 *
 * This value can only be set before the first task is sent. (after alloc_init, or after clearing reset)
 * @param [in] task
 * Task to configure
 * @param [in] algorithm
 * SHA algorithm to use.
 *
 * @return
 * DOCA_SUCCESS- Operation succeeded
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_BAD_STATE - The partial hash is in progress
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_partial_hash_set_algorithm(struct doca_sha_task_partial_hash *task,
						      enum doca_sha_algorithm algorithm);

/**
 * Get the current algorithm in use by this task.
 *
 * The user is responsible to ensure this function is only called for a task which has been returned by alloc_init and
 * has not yet been released. Calling this function in that circumstance is undefined behavior.
 *
 * @param [in] task
 * Task to query.
 *
 * @return
 * Algorithm in use.
 */
DOCA_EXPERIMENTAL
enum doca_sha_algorithm doca_sha_task_partial_hash_get_algorithm(struct doca_sha_task_partial_hash const *task);

/**
 * Identify this task as the final part of the partial SHA. Upon successful completion of this task
 *
 * This must not be set on the first partial buffer. Instead you should use a non partial hash task.
 *
 * Note: Setting the final flag on the first buffer is undefined behavior.
 *
 * @param [in] task
 * Task to configure
 *
 * @return
 * DOCA_SUCCESS- Operation succeeded
 * Typical error codes for this function include:
 *  - DOCA_ERROR_INVALID_VALUE - Invalid argument(s).
 *  - DOCA_ERROR_BAD_STATE - The partial hash is in progress
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sha_task_partial_hash_set_is_final_buf(struct doca_sha_task_partial_hash *task);

/**
 * Get if this task is marked as the final segment of a partial SHA calculation or not.
 *
 * The user is responsible to ensure this function is only called for a task which has been returned by alloc_init and
 * has not yet been released. Calling this function in that circumstance is undefined behavior.
 *
 * @param [in] task
 * Task to query.
 *
 * @return
 * 1 if this task is / was the final segment of a partial hash calculation, 0 otherwise.
 */
DOCA_EXPERIMENTAL
uint8_t doca_sha_task_partial_hash_get_is_final(struct doca_sha_task_partial_hash const *task);

/**
 * Does the destination buffer of this task hold a valid value.
 *
 * @param [in] task
 * Task to configure
 *
 * @return
 * 1 when the destination buffer holds a valid value, 0 otherwise.
 */
DOCA_EXPERIMENTAL
uint8_t doca_sha_task_partial_hash_get_has_result(struct doca_sha_task_partial_hash const *task);

/**
 * Adapt a pointer to a doca_sha partial hash task instance to a general doca_task pointer for use with core objects.
 * This does not imply any transfer of ownership. The existing owner of the task continues to be responsible for its'
 * lifetime and must ensure it remains alive until it has completed (successfully, with an error or it was flushed).
 *
 * @param [in] task
 * Task to adapt to a doca_task.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_sha_task_partial_hash_as_task(struct doca_sha_task_partial_hash *task);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_SHA_H_ */
