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
 * @file doca_ctx.h
 * @page doca ctx
 * @defgroup CTX DOCA Context
 * @ingroup DOCACore
 * DOCA CTX is the base class of every data-path library in DOCA.
 * It is a specific library/SDK instance object providing abstract data processing functionality.
 * The library exposes events and/or tasks that manipulate data.
 *
 * @{
 */

#ifndef DOCA_CTX_H_
#define DOCA_CTX_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************************
 * DOCA Context
 *********************************************************************************************************************/

struct doca_ctx;

/**
 * @brief This enum defines the states of a context.
 *
 *
 * @code
 * The state machine:
 *                            +-------+
 *                            |       |
 *   +----------------------->| idle  +
 *   |                        |       |
 *   |                        +---+---+
 *   |                            |
 *   |                            | doca_ctx_start
 *   |                            | Synchronous: Change state to running and return DOCA_SUCCESS
 *   |                            | Asynchronous: Change state to started and return DOCA_ERROR_IN_PROGRESS
 *   | All in flight tasks are    |
 *   | drained or flushed         +-------------------------------------------+
 *   |                            |                                           |
 *   |                            |                                           |
 *   |                            V                                           V
 *   |                       +----------+                                +---------+
 *   |                       |          | Context is connected           |         |
 *   |                       | Starting |------------------------------->| Running |
 *   |                       |          |                                |         |
 *   |                       +----+-----+                                +----+----+
 *   |                            |                                           |
 *   |                            | doca_ctx_stop                             | doca_ctx_stop
 *   |                            |                                           |
 *   |                            v                                           |
 *   |                       +----------+                                     |
 *   |                       |          |                                     |
 *   |-----------------------+ Stopping |<------------------------------------+
 *                           |          |
 *                           +----------+
 * @endcode
 *
 */
enum doca_ctx_states {
	/**
	 * ctx is created
	 * Resources are not allocated, ctx can not allocate tasks, submit tasks, allocate events or register events.
	 */
	DOCA_CTX_STATE_IDLE = 0,
	/**
	 * doca_ctx_start called, context start is asynchronous.
	 * Resources are allocated, ctx can not allocate tasks, submit tasks, allocate events or register events.
	 */
	DOCA_CTX_STATE_STARTING = 1,
	/**
	 * doca_ctx_start called (ctx start is synchronous) or ctx connection is completed.
	 * Resources are allocated, ctx can allocate tasks, submit tasks, allocate events and register events.
	 */
	DOCA_CTX_STATE_RUNNING = 2,
	/*
	 * doca_ctx_stop called.
	 * Resources are allocated, context can not allocate tasks, submit tasks, allocate events or register events.
	 * Submitted tasks are flushed or drained (during progress call).
	 * ctx will usually move to idle state when number of in-flight tasks reach 0.
	 * Application should call progress_one until the context moves to idle state.
	 */
	DOCA_CTX_STATE_STOPPING = 3,
};

/**
 * @brief Function to execute on context state change
 *
 * @details This function is called when a context state is changed.
 *
 * @param [in] user_data
 * user data supplied by the user (@see doca_ctx_set_user_data)
 * @param [in] ctx
 * doca_ctx that changed state
 * @param [in] prev_state
 * Previous context state
 * @param [in] next_state
 * Next context state (context is already in this state when the callback is called).
 */
typedef void (*doca_ctx_state_changed_callback_t)(const union doca_data user_data,
						  struct doca_ctx *ctx,
						  enum doca_ctx_states prev_state,
						  enum doca_ctx_states next_state);

/**
 * @brief Finalizes all configurations, and starts the DOCA CTX.
 *
 * @details After starting the CTX, it can't be configured any further.
 * Use doca_ctx_stop in order to reconfigure the CTX.
 *
 * The following become possible only after start:
 * - Submitting a task using doca_task_submit()
 *
 * The following are NOT possible after start and become possible again after calling doca_ctx_stop:
 * - Changing CTX properties
 * - Binding gpu device to CTX using doca_ctx_set_datapath_on_gpu()
 * - Binding dpa device to CTX using doca_ctx_set_datapath_on_dpa()
 *
 * @param [in] ctx
 * The DOCA context to start.
 *
 * @return
 * DOCA_SUCCESS - In case of success.
 * Error code - In case of failure:
 * - DOCA_ERROR_INVALID_VALUE - either an invalid input was received or no devices were added to the CTX.
 * - DOCA_ERROR_NOT_SUPPORTED - one of the provided devices is not supported by CTX.
 * - DOCA_ERROR_NOT_CONNECTED - ctx is not connected to a PE and data path on gpu or dpa was not set.
 * - DOCA_ERROR_INITIALIZATION - resource initialization failed (could be due to allocation failure),
 * 				 or the device is in a bad state or another reason caused initialization to fail.
 * - DOCA_ERROR_UNEXPECTED - ctx is corrupted.
 */
DOCA_STABLE
doca_error_t doca_ctx_start(struct doca_ctx *ctx);

/**
 * @brief Stops the context allowing reconfiguration.
 *
 * @details Once a context has started, it can't be configured any further.
 * This method should be called in case the context needs to be configured after starting.
 * For more details see doca_ctx_start().
 *
 * @param [in] ctx
 * The DOCA context to stop.
 *
 * @return
 * DOCA_SUCCESS - In case of success.
 * Error code - In case of failure:
 * - DOCA_ERROR_IN_PROGRESS - some tasks are still in progress. CTX will move to stopping state and a state changed
 * callback shall be invoked when context is fully stopped.
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_CONNECTED - ctx is not connected to a PE and data path on gpu or dpa was not set.
 * - DOCA_ERROR_UNEXPECTED - ctx is corrupted.
 */
DOCA_STABLE
doca_error_t doca_ctx_stop(struct doca_ctx *ctx);

/**
 * @brief Get number of in flight tasks in a doca context
 *
 * @details This method retrieves the number of in flight tasks in a doca context
 *
 * @param [in] ctx
 * Context to query
 * @param [out] num_inflight_tasks
 * Total number of in flight tasks in the context
 *
 * @return
 * DOCA_SUCCESS
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_ctx_get_num_inflight_tasks(const struct doca_ctx *ctx, size_t *num_inflight_tasks);

struct doca_gpu;

/**
 * @brief This function binds the DOCA context to a gpu device.
 *
 * @details The data path will be executed on the device and not on the CPU.
 *
 * @param [in] ctx
 * The library instance.
 * @param [in] gpu_dev
 * A pointer to a doca_gpu device.
 *
 * @return
 * DOCA_SUCCESS - In case of success.
 * Error code - on failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - CTX is started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ctx_set_datapath_on_gpu(struct doca_ctx *ctx, struct doca_gpu *gpu_dev);

/**
 * @brief set user data to context
 *
 * @details This method sets a user data to a context. The user data is used as a parameter in
 * doca_ctx_state_changed_callback_t
 *
 * @param [in] ctx
 * doca_ctx to set the user data to
 * @param [in] user_data
 * doca_data to set to the context
 *
 * @return
 * DOCA_SUCCESS
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_ctx_set_user_data(struct doca_ctx *ctx, union doca_data user_data);

/**
 * @brief get user data from context
 *
 * @details This method retrieves user data from a context (previously set using doca_ctx_set_user_data).
 *
 * @param [in] ctx
 * doca_ctx to get the user data from
 * @param [out] user_data
 * user data to get
 *
 * @return
 * DOCA_SUCCESS
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_ctx_get_user_data(const struct doca_ctx *ctx, union doca_data *user_data);

/**
 * @brief Set state changed callback
 *
 * @details This method sets state changed callback that is invoked every time that a context state is changed
 *
 * @param [in] ctx
 * doca_ctx to set the callback to
 * @param [in] cb
 * doca_ctx_state_changed_callback_t
 *
 * @return
 * DOCA_SUCCESS
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_ctx_set_state_changed_cb(struct doca_ctx *ctx, doca_ctx_state_changed_callback_t cb);

/**
 * @brief Get context state
 *
 * @details This method retrieves the context state
 *
 * @param [in] ctx
 * doca_ctx to get the state from
 * @param [out] state
 * Current context state
 *
 * @return
 * DOCA_SUCCESS
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_ctx_get_state(const struct doca_ctx *ctx, enum doca_ctx_states *state);

struct doca_dpa;

/**
 * @brief This function binds the DOCA context to a dpa device.
 *
 * @details The data path will be executed on the device and not on the CPU.
 *
 * @param [in] ctx
 * The library instance.
 * @param [in] dpa_dev
 * A pointer to a doca_dpa device.
 *
 * @return
 * DOCA_SUCCESS - In case of success.
 * Error code - on failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - CTX is started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ctx_set_datapath_on_dpa(struct doca_ctx *ctx, struct doca_dpa *dpa_dev);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_CTX_H_ */
