/*
 * Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_comch_consumer.h
 * @page comm_channel_v2
 * @defgroup DOCA_COMCH comch
 *
 * DOCA Communication Channel Consumer offers an extension the doca_comch channel for accelerated data transfer between
 * memory on the host and DPU in a FIFO format. An established doca_comch connection is required to negotiate the end
 * points of the FIFO. A consumer object can then post buffers to a remote process that it wishes to receive data on.
 * Completion of a consumer post receive message indicates that data has been populated from a remote producer. The
 * inter-process communication runs over DMA/PCIe and does not affect network bandwidth.
 *
 * @{
 */
#ifndef DOCA_COMCH_CONSUMER_H_
#define DOCA_COMCH_CONSUMER_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_buf;
struct doca_dev;
struct doca_devinfo;
struct doca_mmap;
struct doca_dpa_thread;

/* Representantion of a comms channel point to point connection */
struct doca_comch_connection;

/* Instance of a doca_comch consumer */
struct doca_comch_consumer;

/* Opaque structure representing DPA consumer completion context */
struct doca_comch_consumer_completion;

/**
 * @brief DPA handle for DPA consumer completion context
 */
typedef uint64_t doca_dpa_dev_comch_consumer_completion_t;

/**
 * @brief DPA handle for DPA consumer
 */
typedef uint64_t doca_dpa_dev_comch_consumer_t;

/*********************************************************************************************************************
 * Consumer Creation
 *********************************************************************************************************************/

/**
 * Create a DOCA Comch consumer instance.
 *
 * @param [in] comch_connection
 * An established control channel connection to create consumer across.
 * @param [in] buf_mmap
 * A registered mmap for the memory region the consumer allows buffer writes to.
 * @param [out] consumer
 * Pointer to pointer to be set to created doca_comch_consumer instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - input parameter is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_comch_consumer object or id.
 * - DOCA_ERROR_BAD_STATE - comch_connection is not established.
 * - DOCA_ERROR_NOT_PERMITTED - incompatible version of comch_connection.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_create(struct doca_comch_connection *comch_connection,
					struct doca_mmap *buf_mmap,
					struct doca_comch_consumer **consumer);

/**
 * Destory a DOCA Comch consumer instance.
 *
 * @param [in] consumer
 * Pointer to doca_comch_consumer instance to destroy.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - consumer argument is a NULL pointer.
 * - DOCA_ERROR_INITIALIZATION - failed to initialise a mutex.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_destroy(struct doca_comch_consumer *consumer);

/**
 * Check if given device is capable of running a consumer.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device can implement a consumer.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo can not implement a consumer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_cap_is_supported(const struct doca_devinfo *devinfo);

/**
 * Get the id the doca_comch_consumer instance.
 *
 * @param [in] consumer
 * The doca_comch_consumer instance.
 * @param [out] id
 * Per comch_connection unique id associated with the consumer instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_get_id(const struct doca_comch_consumer *consumer, uint32_t *id);

/**
 * Get the max number of tasks supported by the device for a doca_comch_consumer instance.
 *
 * @param [in] devinfo
 * Devinfo to query the capability for.
 * @param [out] max_num_tasks
 * The maximum number of tasks that can allocated by the instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_cap_get_max_num_tasks(const struct doca_devinfo *devinfo, uint32_t *max_num_tasks);

/**
 * Get the max size doca_buf that can be received by a doca_comch_consumer instance.
 *
 * @param [in] devinfo
 * Devinfo to query the capability for.
 * @param [out] max_buf_size
 * Maximum sized buffer that can be received by the consumer.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - if consumer is not supported on device.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_cap_get_max_buf_size(const struct doca_devinfo *devinfo, uint32_t *max_buf_size);

/**
 * Get the max length of doca_buf list that can be received by a doca_comch_consumer instance.
 *
 * @param [in] devinfo
 * Devinfo to query the capability for.
 * @param [out] max_buf_list_len
 * Maximum sized buffer list that can be received by the consumer.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - if consumer is not supported on device.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_cap_get_max_buf_list_len(const struct doca_devinfo *devinfo,
							  uint32_t *max_buf_list_len);

/**
 * Get the max number of consumers that can be associated with a doca_comch_connection.
 *
 * @param [in] devinfo
 * Devinfo to query the capability for.
 * @param [out] max_consumers
 * Maximum number of consumers that can be added to a doca_comch_connection.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_cap_get_max_consumers(const struct doca_devinfo *devinfo, uint32_t *max_consumers);

/**
 * Get the max length of immediate data supported by consumers.
 *
 * @param [in] devinfo
 * Devinfo to query the capability for.
 * @param [out] max_imm_data_len
 * Maximum length of immediate data consumers support.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_cap_get_max_imm_data_len(const struct doca_devinfo *devinfo,
							  uint32_t *max_imm_data_len);

/**
 * Get the length of immediate data supported by a consumer.
 *
 * @param [in] consumer
 * Consumer to query.
 * @param [out] imm_data_len
 * Length of immediate data the consumer is configured to support.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_get_imm_data_len(const struct doca_comch_consumer *consumer, uint32_t *imm_data_len);

/**
 * Set the length of immediate data supported by a consumer.
 *
 * Length must be less than or equal to that returned by doca_comch_consumer_cap_get_max_imm_data_len().
 *
 * @param [in] consumer
 * Consumer to set length of.
 * @param [in] imm_data_len
 * Length of immediate data to configure in the consumer.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_set_imm_data_len(struct doca_comch_consumer *consumer, uint32_t imm_data_len);

/**
 * Convert doca_comch_consumer instance into a generalised context for use with doca core objects.
 *
 * @param [in] consumer
 * Doca_comch_consumer instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_comch_consumer_as_ctx(struct doca_comch_consumer *consumer);

/*********************************************************************************************************************
 * Consumer Creation on DPA
 *********************************************************************************************************************/

/**
 * @brief Allocate DOCA Comch consumer completion context on DPA.
 *
 * @param [out] consumer_comp
 * The newly created DOCA Comch consumer completion context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_create(struct doca_comch_consumer_completion **consumer_comp);

/**
 * @brief Set the DOCA DPA thread of the completion context.
 *
 * This thread will receive notifications for completions.
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context.
 * @param [in] dpa_thread
 * The DOCA dpa thread to be associated with the completion context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_set_dpa_thread(struct doca_comch_consumer_completion *consumer_comp,
							   struct doca_dpa_thread *dpa_thread);

/**
 * @brief Destroy the DOCA Comch consumer completion context.
 *
 * The associated dpa handle will be destroyed as well.
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context to destroy.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_destroy(struct doca_comch_consumer_completion *consumer_comp);

/**
 * @brief Start DOCA Comch consumer completion context.
 *
 * @details On start verifies and finalizes the completion context configuration.
 *
 * The following is possible for started completion context:
 * - Associating DOCA Comch consumers with the completion context.
 *
 * The following is NOT possible while completion context is started:
 * - Setting the properties of the completion context
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context to start.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_start(struct doca_comch_consumer_completion *consumer_comp);

/**
 * @brief Stop DOCA Comch consumer completion context.
 *
 * @details On stop prevents execution of different operations and allows operations that were available before start.
 * For details see doca_comch_consumer_completion_start(). Completion context can't be stopped while there are
 * DOCA Comch consumers associated with it.
 *
 * The following is possible for stopped completion context:
 * - Setting the properties of the completion context
 *
 * The following is NOT possible while completion context is stopped:
 * - Associating DOCA Comch consumers with the completion context.
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context to stop.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_stop(struct doca_comch_consumer_completion *consumer_comp);

/**
 * @brief Get the DPA handle for the DOCA Comch consumer completion context.
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context previously created on DPA.
 * @param [out] consumer_comp_handle
 * A pointer to the associated DPA handle in the dpa memory space.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_get_dpa_handle(
	struct doca_comch_consumer_completion *consumer_comp,
	doca_dpa_dev_comch_consumer_completion_t *consumer_comp_handle);

/**
 * @brief Set the maximal number of consumers that can be associated with the completion context.
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context to modify.
 * @param [in] max_num_consumers
 * The maximal number of consumers that can be associated with the context. Will be rounded up to next power of 2
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_set_max_num_consumers(struct doca_comch_consumer_completion *consumer_comp,
								  uint32_t max_num_consumers);

/**
 * @brief Get the maximal number of consumers that can be associated with the completion context.
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context to query.
 * @param [out] max_num_consumers
 * The maximal number of consumers that can be associated with the context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_get_max_num_consumers(struct doca_comch_consumer_completion *consumer_comp,
								  uint32_t *max_num_consumers);

/**
 * @brief Set the maximal number of receive operations across all consumers associated with the completion context.
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context to modify.
 * @param [in] max_num_recv
 * The maximal number of recv that can be associated with the context. Will be rounded up to next power of 2
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_set_max_num_recv(struct doca_comch_consumer_completion *consumer_comp,
							     uint32_t max_num_recv);

/**
 * @brief Get the maximal number of receive operations across all consumers associated with the completion context.
 *
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context to query.
 * @param [out] max_num_recv
 * The maximal number of recv that can be associated with the context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_completion_get_max_num_recv(struct doca_comch_consumer_completion *consumer_comp,
							     uint32_t *max_num_recv);

/**
 * @brief Set the maximal number of receive operations for a DPA consumer.
 *
 * @param [in] consumer
 * The doca_comch_consumer instance.
 * @param [in] dev_num_recv
 * The maximal number of recv that can be associated with the context. Will be rounded up to next power of 2
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_set_dev_num_recv(struct doca_comch_consumer *consumer, uint32_t dev_num_recv);

/**
 * @brief Associate consumer with DPA completion context.
 *
 * This will allow the completion context to receive completions of this consumer on the DPA
 *
 * @param [in] consumer
 * The doca_comch_consumer instance. Must call doca_ctx_set_datapath_on_dpa() prior to this call
 * @param [in] consumer_comp
 * The DOCA Comch consumer completion context to associate with consumer
 * @param[in] user_data
 * User data that can be retrieved in DPA from completion elements returned by this consumer
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_set_completion(struct doca_comch_consumer *consumer,
						struct doca_comch_consumer_completion *consumer_comp,
						uint32_t user_data);

/**
 * @brief Retrieve the handle in the dpa memory space of a doca_comch_consumer
 *
 * @param [in] consumer
 * doca_comch_consumer context to get the dpa handle from.
 * @param [out] dpa_consumer
 * A pointer to the handle in the dpa memory space.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if called before calling doca_ctx_start(), or if not assigned to dpa datapath.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_get_dpa_handle(struct doca_comch_consumer *consumer,
						doca_dpa_dev_comch_consumer_t *dpa_consumer);

/*********************************************************************************************************************
 * Consumer Post Receive Task
 *********************************************************************************************************************/

/* Task instance for consumer to do a post receive */
struct doca_comch_consumer_task_post_recv;

/**
 * Function executed on doca_comch_consumer post receive completion. Used for both task success and failure.
 *
 * @param [in] task
 * Doca consumer post recv task that has completed.
 * @param [in] task_user_data
 * The task user data.
 * @param [in] ctx_user_data
 * Doca_comch context user data.
 *
 * The implementation can assume this value is not NULL.
 */
typedef void (*doca_comch_consumer_task_post_recv_completion_cb_t)(struct doca_comch_consumer_task_post_recv *task,
								   union doca_data task_user_data,
								   union doca_data ctx_user_data);

/**
 * Configure the doca_comch_consumer post receive task callback and parameters.
 *
 * @param [in] consumer
 * The doca_comch_consumer instance.
 * @param [in] task_completion_cb
 * Post receive task completion callback.
 * @param [in] task_error_cb
 * Post receive task error callback.
 * @param [in] num_post_recv_tasks
 * Number of post_recv tasks a consumer can allocate.
 * Must not exceed value returned by doca_comch_consumer_cap_get_max_num_tasks().
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - consumer instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_task_post_recv_set_conf(
	struct doca_comch_consumer *consumer,
	doca_comch_consumer_task_post_recv_completion_cb_t task_completion_cb,
	doca_comch_consumer_task_post_recv_completion_cb_t task_error_cb,
	uint32_t num_post_recv_tasks);

/**
 * @brief Allocate and initialise a doca_consumer post receive task.
 *
 * Doca buffer should be located within the registered mmap associated with consumer instance.
 * Completion callback will be triggered whenever the buffer has been populated by a consumer.
 *
 * @param [in] consumer
 * The doca_comch_consumer instance.
 * @param [in] buf
 * Doca buffer available to be populated by producers.
 * @param [out] task
 * Pointer to a doca_comch_consumer_post_recv_task instance populated with input parameters.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NO_MEMORY - no available tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_consumer_task_post_recv_alloc_init(struct doca_comch_consumer *consumer,
							   struct doca_buf *buf,
							   struct doca_comch_consumer_task_post_recv **task);

/**
 * Get the doca_buf from the doca_comch_consumer_post_recv_task instance.
 *
 * @param [in] task
 * The doca_comch_consumer_post_recv_task instance.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_comch_consumer_task_post_recv_get_buf(struct doca_comch_consumer_task_post_recv *task);

/**
 * Set the doca_buf in a doca_comch_consumer_post_recv_task instance.
 *
 * @param [in] task
 * The doca_comch_consumer_post_recv_task instance.
 * @param [in] buf
 * Buffer to set in the task.
 */
DOCA_EXPERIMENTAL
void doca_comch_consumer_task_post_recv_set_buf(struct doca_comch_consumer_task_post_recv *task, struct doca_buf *buf);

/**
 * Get the producer id from the doca_comch_consumer_post_recv_task instance.
 *
 * Producer id will only be set on post recv completion and indicates the remote producer that has written data to the
 * associated doca_buf.
 *
 * @param [in] task
 * The doca_comch_consumer_post_recv_task instance.
 *
 * @return
 * Producer id upon success, 0 otherwise.
 */
DOCA_EXPERIMENTAL
uint32_t doca_comch_consumer_task_post_recv_get_producer_id(struct doca_comch_consumer_task_post_recv *task);

/**
 * Get a pointer to any immediate data from the doca_comch_consumer_post_recv_task instance.
 *
 * Immediate data will only be set on post recv completion.
 * doca_comch_consumer_task_post_recv_get_imm_data_len() indicates the number of valid bits pointed to.
 *
 * @param [in] task
 * The doca_comch_consumer_post_recv_task instance.
 *
 * @return
 * Pointer to immediate data or nullptr if none exists.
 */
DOCA_EXPERIMENTAL
uint8_t *doca_comch_consumer_task_post_recv_get_imm_data(struct doca_comch_consumer_task_post_recv *task);

/**
 * Get the length of any immediate data from the doca_comch_consumer_post_recv_task instance.
 *
 * Immediate data length will only be set on post recv completion.
 * It indicates the valid number of bytes in the pointer return by doca_comch_consumer_task_post_recv_get_imm_data().
 *
 * @param [in] task
 * The doca_comch_consumer_post_recv_task instance.
 *
 * @return
 * The immediate data length on post receive completion.
 */
DOCA_EXPERIMENTAL
uint32_t doca_comch_consumer_task_post_recv_get_imm_data_len(struct doca_comch_consumer_task_post_recv *task);

/**
 * Convert doca_comch_consumer_post_recv_task instance into a generalised task for use with progress engine.
 *
 * @param [in] task
 * Doca_comch_consumer_post_recv_task instance.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_comch_consumer_task_post_recv_as_task(struct doca_comch_consumer_task_post_recv *task);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_COMCH_CONSUMER_H_ */
