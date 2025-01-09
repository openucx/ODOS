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

#ifndef DOCA_PE_H_
#define DOCA_PE_H_

#include <stdint.h>
#include <stdlib.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Forward declarations
 */
struct doca_ctx;
struct doca_pe;
struct doca_task;
struct doca_task_batch;

/**
 * Supported batch sizes for doca_task_batch
 */
enum doca_task_batch_max_tasks_number {
	DOCA_TASK_BATCH_MAX_TASKS_NUMBER_16,
	DOCA_TASK_BATCH_MAX_TASKS_NUMBER_32,
	DOCA_TASK_BATCH_MAX_TASKS_NUMBER_64,
	DOCA_TASK_BATCH_MAX_TASKS_NUMBER_128,
};

/**
 * Supported batch sizes for event batches
 */
enum doca_event_batch_events_number {
	DOCA_EVENT_BATCH_EVENTS_NUMBER_4,
	DOCA_EVENT_BATCH_EVENTS_NUMBER_8,
	DOCA_EVENT_BATCH_EVENTS_NUMBER_16,
	DOCA_EVENT_BATCH_EVENTS_NUMBER_32,
	DOCA_EVENT_BATCH_EVENTS_NUMBER_64,
	DOCA_EVENT_BATCH_EVENTS_NUMBER_128,
};

/**
 * @brief Function to execute on task completion.
 *
 * @details This function is called by doca_pe_progress() when related task identified as completed successfully.
 * When this function called the ownership of the task object passed from DOCA back to user.
 * User may decide on the task object inside this callback:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object. All tasks MUST be freed before
 *   stopping the context (drained tasks should be freed during stop flow).
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress for details.
 *
 * Any failure/error inside this function should be handled internally or differed;
 * due to the mode of nested in doca_pe_progress() execution this callback doesn't return error.
 *
 * NOTE: this callback type utilizes successful & failed task completions.
 *
 * @param [in] task
 * The successfully completed task.
 * The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * task user data
 * @param [in] ctx_user_data
 * doca_ctx user data
 */
typedef void (*doca_task_completion_cb_t)(struct doca_task *task,
					  union doca_data task_user_data,
					  union doca_data ctx_user_data);

/**
 * @brief Creates DOCA progress engine
 *
 * @details The progress engine is used to progress tasks and events. The progress engine is associated with one or
 * more doca contexts. A doca context can only be associated with one progress engine.
 *
 * @param [out] pe
 * progress engine to create
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate progress engine.
 */
DOCA_STABLE
doca_error_t doca_pe_create(struct doca_pe **pe);

/**
 * @brief Destroy doca progress engine
 *
 * @details This function destroys a progress engine. Progress engine can be destroyed only after all connected contexts
 * has been destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_IN_USE - contexts are still connected to the progress engine.
 */
DOCA_STABLE
doca_error_t doca_pe_destroy(struct doca_pe *pe);

/**
 * @brief Run the progress engine.
 *
 * @details Polling method for progress of submitted tasks and handling of registered events.
 * The method finds the next context with a completed task and invokes its completion callback.
 *
 * @param [in] pe
 * The progress engine object to poll for completions. MUST NOT BE NULL.
 *
 * @return 1 if progress was made, 0 otherwise.
 * Examples:
 * A task was completed (fully, partially).
 * Unsolicited event handled.
 * Context state transition (starting -> running or stopping -> idle).
 */
DOCA_STABLE
uint8_t doca_pe_progress(struct doca_pe *pe);

#ifdef __linux__
/**
 * @brief Get the completion handle for waiting on.
 *
 * @details
 * Retrieves the event handle of the progress engine, the handle does not change throughout the lifecycle of the
 * progress engine.
 *
 * @param [in] pe
 * The progress engine to query.
 * @param [out] handle
 * The event handle of the progress engine.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_pe_get_notification_handle(const struct doca_pe *pe, doca_notification_handle_t *handle);

#endif /* __linux__ */

#ifdef _WIN32

/**
 * @brief Set the completion handle for waiting on.
 *
 * @details
 * This method is supported only for Windows.
 * Windows uses io completion port that is created by the application and passed to the work queue. The work queue
 * Uses the io completion port to register events.
 *
 * @param [in] pe
 * The progress_engine to set
 * @param [in] handle
 * The IO completion port to register to
 * @param [in] completion_key
 * Completion key facilitates finding the source of the event.
 *
 * @return
 * DOCA_SUCCESS - on successfully setting notification handle.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - OS is not Windows.
 * - DOCA_ERROR_OPERATING_SYSTEM - a system call has failed.
 */
DOCA_STABLE
doca_error_t doca_pe_set_notification_handle(struct doca_pe *pe,
					     doca_notification_handle_t handle,
					     union doca_data completion_key);

#endif /* _WIN32 */

/**
 * @brief Clear triggered completions after wait.
 *
 * @details Method used for clearing of events, this method should be called after an event has been received using the
 * event handle. After this method is called, the events will no longer be triggered, and the handle can be armed again.
 * See doca_pe_request_notification() for entire flow.
 *
 * @param [in] pe
 * The progress engine object that received the events. MUST NOT BE NULL.
 * @param [in] handle
 * progress engine event handle.
 *
 * @return
 * - DOCA_SUCCESS - on successfully clearing triggered events.
 * - DOCA_ERROR_OPERATING_SYSTEM - a system call has failed.
 */
DOCA_STABLE
doca_error_t doca_pe_clear_notification(struct doca_pe *pe, doca_notification_handle_t handle);

/**
 * @brief Arm the progress engine to wait for completion.
 *
 * @details This method should be used before waiting on the completion handle.
 * The expected flow is as follows:
 * 1. Get or set event handle using doca_pe_get_notification_handle or doca_pe_set_notification_handle
 * 2. Arm the progress engine.
 * 3. Wait for an event using the event handle. E.g., using epoll_wait().
 * 4. Once the thread wakes up, call doca_pe_clear_notification().
 * 5. Call doca_pe_progress() until it returns 0 (implies that no more tasks, events or other progress is done)
 * 6. Repeat 2.
 *
 * @param [in] pe
 * The progress engine object to arm. MUST NOT BE NULL.
 *
 * @return
 * - DOCA_SUCCESS - progress engine has been successfully armed, event handle can be used to wait on events.
 */
DOCA_STABLE
doca_error_t doca_pe_request_notification(struct doca_pe *pe);

/**
 * @brief Get number of in flight tasks
 *
 * @details This method retrieves the number of in flight tasks in a progress engine
 *
 * @param [in] pe
 * Progress engine to query
 * @param [out] num_inflight_tasks
 * Total number of in flight tasks in the progress engine
 *
 * @return
 * DOCA_SUCCESS
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_pe_get_num_inflight_tasks(const struct doca_pe *pe, size_t *num_inflight_tasks);

/**
 * @brief Submit a task to a progress engine
 *
 * @details
 * This function submits a task to a progress engine.
 * The function DOES NOT validate the task.
 * The task is routed to the relevant context and will be executed asynchronously. See doca_pe_progress for
 * task completion details. The task will be owned by DOCA upon a successful submission.
 *
 * @param [in] task
 * The task to submit
 *
 * @return
 * - DOCA_SUCCESS - on successfully submitting the task.
 * - Any other doca_error_t implies task submission failure. In that case the user owns the task.
 */
DOCA_STABLE
doca_error_t doca_task_submit(struct doca_task *task);

/**
 * @brief Submit a task_batch to a progress engine
 *
 * @details
 * This function submits a task_batch to a progress engine.
 * The function DOES NOT validate the task_batch.
 * The task batch is routed to the relevant context and will be executed asynchronously. See doca_pe_progress for
 * task_batch completion details. The task_batch will be owned by DOCA upon a successful submission.
 *
 * @param [in] task_batch
 * The task_batch to submit
 *
 * @return
 * - DOCA_SUCCESS - on successfully submitting the task_batch.
 * - Any other doca_error_t implies task_batch submission failure. In that case the user owns the task_batch.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_task_batch_submit(struct doca_task_batch *task_batch);

/**
 * @brief Try to submit a task to a progress engine
 *
 * @details
 * This function submits a task to a progress engine.
 * The function validates the task (task validation is different per task). If the task is invalid it will not be
 * submitted.
 * The task is routed to the relevant context and will be executed asynchronously. See doca_pe_progress for
 * task completion details. The task will be owned by DOCA upon a successful submission.
 *
 * @param [in] task
 * The task to submit
 *
 * @return
 * - DOCA_SUCCESS - on successfully submitting the task.
 * - DOCA_ERROR_INVALID_VALUE - The task is invalid (e.g., one of the fields is incorrect or null).
 * - Any other doca_error_t implies task submission failure. In that case the user owns the task.
 */
DOCA_STABLE
doca_error_t doca_task_try_submit(struct doca_task *task);

/**
 * @brief Try to submit a task_batch to a progress engine
 *
 * @details
 * This function submits a task_batch to a progress engine.
 * The function validates the task_batch (task_batch validation is different per task_batch type). If the task_batch is
 * invalid it will not be submitted. The task_batch is routed to the relevant context and will be executed
 * asynchronously. See doca_pe_progress for task_batch completion details. The task_batch will be owned by DOCA upon a
 * successful submission.
 *
 * @param [in] task_batch
 * The task_batch to submit
 *
 * @return
 * - DOCA_SUCCESS - on successfully submitting the task_batch.
 * - DOCA_ERROR_INVALID_VALUE - The task_batch is invalid (e.g., one of the fields is incorrect or null).
 * - Any other doca_error_t implies task_batch submission failure. In that case the user owns the task_batch.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_task_batch_try_submit(struct doca_task_batch *task_batch);

/**
 * @brief Free a task back to where it was allocated from
 *
 * @details
 * This function frees a task back to the where it was allocated from (usually a doca_ctx)
 * Task allocation is a per context API.
 * All tasks MUST be freed in order for a context to be stopped. A task can be freed during completion / error callback
 * or at any other place in the program as long as it is not submitted.
 *
 * @param [in] task to free
 */
DOCA_STABLE
void doca_task_free(struct doca_task *task);

/**
 * @brief Free a task_batch back to where it was allocated from
 *
 * @details
 * This function frees a task_batch back to the where it was allocated from (usually a doca_ctx)
 * Task Batch allocation is a per context API.
 * All task_batches MUST be freed in order for a context to be stopped. A task_batch can be freed during completion /
 * error callback or at any other place in the program as long as it is not submitted.
 *
 * @param [in] task_batch
 * The task_batch to free
 */
DOCA_EXPERIMENTAL
void doca_task_batch_free(struct doca_task_batch *task_batch);

/**
 * @brief Get task status
 *
 * @details This method retrieves the status of a doca_task
 *
 * @param [in] task
 * The task to retrieve the status from
 *
 * @return
 * DOCA_SUCCESS - task execution was successful
 * Any other doca_error_t indicates that the task failed (task depended)
 */
DOCA_STABLE
doca_error_t doca_task_get_status(const struct doca_task *task);

/**
 * @brief Get task_batch status
 *
 * @details This method retrieves the status of a doca_task_batch
 *
 * @param [in] task_batch
 * The task_batch to retrieve the status from
 *
 * @return
 * DOCA_SUCCESS - task_batch execution was successful
 * Any other doca_error_t indicates that the task_batch failed (task_batch type dependent)
 */
DOCA_EXPERIMENTAL
doca_error_t doca_task_batch_get_status(const struct doca_task_batch *task_batch);

/**
 * @brief Set user data to a task
 *
 * @details This method sets a user data to a task
 *
 * @param [in] task
 * doca_task to set the user data to
 * @param [in] user_data
 * doca_data to set
 */
DOCA_STABLE
void doca_task_set_user_data(struct doca_task *task, union doca_data user_data);

/**
 * @brief Set user data to a task_batch
 *
 * @details This method sets a user data to a task_batch
 *
 * @param [in] task_batch
 * doca_task_batch to set the user data to
 * @param [in] user_data
 * doca_data to set
 */
DOCA_EXPERIMENTAL
void doca_task_batch_set_user_data(struct doca_task_batch *task_batch, union doca_data user_data);

/**
 * @brief Get user data from a task
 *
 * @details This method retrieves user data from a doca_task
 *
 * @param[in] task
 * doca_task to get the user data from
 *
 * @return doca_data - user data that was stored in the task
 */
DOCA_STABLE
union doca_data doca_task_get_user_data(const struct doca_task *task);

/**
 * @brief Get user data from a task_batch
 *
 * @details This method retrieves user data from a doca_task_batch
 *
 * @param[in] task_batch
 * doca_task_batch to get the user data from
 *
 * @return doca_data - user data that was stored in the task_batch
 */
DOCA_EXPERIMENTAL
union doca_data doca_task_batch_get_user_data(const struct doca_task_batch *task_batch);

/**
 * @brief Get context from a doca task
 *
 * @details This method retrieves the context from a doca task
 *
 * @param [in] task
 * The task to get the context from
 *
 * @return doca_ctx - the context that allocated the task.
 */
DOCA_STABLE
struct doca_ctx *doca_task_get_ctx(const struct doca_task *task);

/**
 * @brief Get context from a doca task_batch
 *
 * @details This method retrieves the context from a doca task_batch
 *
 * @param [in] task_batch
 * The task_batch to get the context from
 *
 * @return doca_ctx - the context that allocated the task_batch.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_task_batch_get_ctx(const struct doca_task_batch *task_batch);

/**
 * @brief Get context user data from a doca task_batch
 *
 * @details This method retrieves the context user data from a doca task_batch
 *
 * @param [in] task_batch
 * The task_batch to get the context user data from
 *
 * @return doca_ctx - the context user data that allocated the task_batch.
 */
DOCA_EXPERIMENTAL
union doca_data doca_task_batch_get_ctx_user_data(const struct doca_task_batch *task_batch);

/**
 * @brief Get actual tasks number from a doca task_batch
 *
 * @details This method retrieves the actual tasks number from a doca task_batch
 *
 * @param [in] task_batch
 * The task_batch to get the tasks number from
 *
 * @return uint16_t - the actual tasks number in the task_batch.
 */
DOCA_EXPERIMENTAL
uint16_t doca_task_batch_get_tasks_num(struct doca_task_batch *task_batch);

/**
 * @brief This method connects a context to a progress engine
 *
 * @details: The connection can only be broken by destroying the context.
 *
 * @param [in] pe
 * Progress engine to connect the context to
 * @param [in] ctx
 * Context to connect
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - context or pe argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate resources for the context.
 * - DOCA_ERROR_BAD_STATE - context is not idle
 * - DOCA_ERROR_NOT_PERMITTED - context is already connected to a progress engine.
 */
DOCA_STABLE
doca_error_t doca_pe_connect_ctx(struct doca_pe *pe, struct doca_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif /* DOCA_PE_H_ */
