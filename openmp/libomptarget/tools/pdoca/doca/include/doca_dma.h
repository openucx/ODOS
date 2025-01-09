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
 * @file doca_dma.h
 * @page DOCA DMA
 * @defgroup DOCADMA DOCA DMA engine
 * DOCA DMA library. For more details please refer to the user guide on DOCA devzone.
 *
 * @{
 */
#ifndef DOCA_DMA_H_
#define DOCA_DMA_H_

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

/**
 * Forward declaration for DMA memcpy task.
 *
 * This task is used to copy source doca_buf to destination doca_buf
 * @see Task APIs below for usage.
 */
struct doca_dma_task_memcpy;

/**
 * @brief Function to execute on DMA memcpy task completion.
 *
 * @details This function is called by doca_pe_progress() when related task identified as completed successfully.
 * When this function called the ownership of the task object passed from DOCA back to user.
 * Inside this callback user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * due to the mode of nested in doca_pe_progress() execution this callback doesn't return error.
 *
 * NOTE: this callback type utilized for both successful & failed task completions.
 *
 * @param [in] task
 * DMA memcpy task.
 * @param [in] task_user_data
 * The task user data
 * @param [in] ctx_user_data
 * DMA context user data
 */
typedef void (*doca_dma_task_memcpy_completion_cb_t)(struct doca_dma_task_memcpy *task,
						     union doca_data task_user_data,
						     union doca_data ctx_user_data);

/*********************************************************************************************************************
 * DOCA DMA Context
 *********************************************************************************************************************/

/**
 * Opaque structure representing a DOCA DMA instance.
 */
struct doca_dma;

/**
 * Create a DOCA DMA instance.
 * @param [in] dev
 * The device to attach to the DMA context
 * @param [out] dma
 * Pointer to pointer to be set to point to the created doca_dma instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - dma argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_dma.
 * - DOCA_ERROR_INITIALIZATION - failed to initialise a mutex.
 *
 */
DOCA_STABLE
doca_error_t doca_dma_create(struct doca_dev *dev, struct doca_dma **dma);

/**
 * @param [in] dma
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_IN_USE - Unable to gain exclusive access to the dma instance.
 * - DOCA_ERROR_IN_USE - One or more work queues are still attached. These must be detached first.
 */
DOCA_STABLE
doca_error_t doca_dma_destroy(struct doca_dma *dma);

/**
 * Check if given device is capable of executing DMA memcpy task.
 *
 * @param [in] devinfo
 * The DOCA device information
 *
 * @return
 * DOCA_SUCCESS - in case device supports memcpy.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query the device for its capabilities.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support memcpy.
 */
DOCA_STABLE
doca_error_t doca_dma_cap_task_memcpy_is_supported(const struct doca_devinfo *devinfo);

/**
 * Convert doca_dma instance into a generalised context for use with doca core objects.
 *
 * @param [in] dma
 * DMA instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_STABLE
struct doca_ctx *doca_dma_as_ctx(struct doca_dma *dma);

/**
 * Get the maximum log number of tasks
 *
 * @details This method retrieves the maximum number of tasks for a device.
 *
 * @param [in] dma
 * The dma context
 * @param [out] max_num_tasks
 * Max number of memcpy tasks
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_STABLE
doca_error_t doca_dma_cap_get_max_num_tasks(struct doca_dma *dma, uint32_t *max_num_tasks);

/**
 * Get the maximum supported number of elements in a given DOCA linked-list buffer for DMA memcpy task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @param [out] max_buf_list_len
 * The maximum supported number of elements in a given DOCA linked-list buffer,
 * such that 1 indicates no linked-list buffer support.
 *
 * @return
 * DOCA_SUCCESS - upon success
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_STABLE
doca_error_t doca_dma_cap_task_memcpy_get_max_buf_list_len(const struct doca_devinfo *devinfo,
							   uint32_t *max_buf_list_len);

/**
 * Get the maximum supported buffer size for DMA memcpy task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @param [out] buf_size
 * The maximum supported buffer size in bytes.
 *
 * @return
 * DOCA_SUCCESS - upon success
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_STABLE
doca_error_t doca_dma_cap_task_memcpy_get_max_buf_size(const struct doca_devinfo *devinfo, uint64_t *buf_size);

/**
 * @brief This method sets the DMA memcpy tasks configuration
 *
 * @param [in] dma
 * The DMA context to config
 * @param [in] task_completion_cb
 * Task completion callback
 * @param [in] task_error_cb
 * Task error callback
 * @param [in] num_memcpy_tasks
 * Number of memcpy tasks that the DMA can allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - doca_pe_dma argument is a NULL pointer.
 * - DOCA_ERROR_NOT_SUPPORTED - context is in work queue mode
 */
DOCA_STABLE
doca_error_t doca_dma_task_memcpy_set_conf(struct doca_dma *dma,
					   doca_dma_task_memcpy_completion_cb_t task_completion_cb,
					   doca_dma_task_memcpy_completion_cb_t task_error_cb,
					   uint32_t num_memcpy_tasks);

/**
 * @brief This method allocates and initializes a DMA memcpy task
 *
 * @param [in] dma
 * The DMA to allocate the task for
 * @param [in] src
 * source buffer
 * @param [in] dst
 * destination buffer
 * @param [in] user_data
 * doca_data to attach to the task
 * @param [out] task
 * memcpy task to allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMEORY - No more tasks to allocate
 */
DOCA_STABLE
doca_error_t doca_dma_task_memcpy_alloc_init(struct doca_dma *dma,
					     const struct doca_buf *src,
					     struct doca_buf *dst,
					     union doca_data user_data,
					     struct doca_dma_task_memcpy **task);

/**
 * @brief This method converts a memcpy task to doca_task
 *
 * @param [in] task
 * doca_dma_task_memcpy task
 *
 * @return doca_task
 */
DOCA_STABLE
struct doca_task *doca_dma_task_memcpy_as_task(struct doca_dma_task_memcpy *task);

/**
 * @brief This method sets source buffer to memcpy task
 *
 * @param [in] task
 * The task to set
 * @param [in] src
 * Source buffer
 */
DOCA_STABLE
void doca_dma_task_memcpy_set_src(struct doca_dma_task_memcpy *task, const struct doca_buf *src);

/**
 * @brief This method gets source buffer from memcpy task
 *
 * @param [in] task
 * The task to get
 *
 * @return source buffer
 */
DOCA_STABLE
const struct doca_buf *doca_dma_task_memcpy_get_src(const struct doca_dma_task_memcpy *task);

/**
 * @brief This method sets destination buffer to memcpy task
 *
 * @param [in] task
 * The task to set
 * @param [in] dst
 * Destination buffer
 */
DOCA_STABLE
void doca_dma_task_memcpy_set_dst(struct doca_dma_task_memcpy *task, struct doca_buf *dst);

/**
 * @brief This method gets destination buffer from memcpy task
 *
 * @param [in] task
 * The task to get
 *
 * @return destination buffer
 */
DOCA_STABLE
struct doca_buf *doca_dma_task_memcpy_get_dst(const struct doca_dma_task_memcpy *task);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_DMA_H_ */

/** @} */
