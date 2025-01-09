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
 * @file doca_sync_event.h
 * @page doca sync event
 * @defgroup SE DOCA Sync Event
 * @ingroup DOCACore
 * DOCA Sync Event
 * DOCA Sync Event is a software synchronization mechanism of parallel execution across the CPU, DPU, DPA, GPU and a
 * remote node. It is an abstraction around 64-bit value which can be updated, read, and waited upon from any of these
 * units to achieve synchronization between executions on them.
 * @{
 */

#ifndef DOCA_SYNC_EVENT_H_
#define DOCA_SYNC_EVENT_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_ctx;
struct doca_dev;
struct doca_devinfo;
struct doca_dpa;
struct doca_gpu;

struct doca_sync_event;
struct doca_sync_event_remote_net;

struct doca_dpa_sync_event;

struct doca_buf;

/**
 * @brief DOCA Sync Event DPA handle
 */
typedef uint64_t doca_dpa_dev_sync_event_t;

/**
 * @brief DOCA Sync Event remote DPA handle
 */
typedef uint64_t doca_dpa_dev_sync_event_remote_net_t;

struct doca_gpu_sync_event;

/**
 * @brief DOCA Sync Event GPU handle
 */
typedef uint64_t doca_gpu_dev_sync_event_t;

/**
 * @brief DOCA Sync Event remote GPU handle
 */
typedef uint64_t doca_gpu_dev_sync_event_remote_net_t;

struct doca_sync_event_task_get;
struct doca_sync_event_task_notify_set;
struct doca_sync_event_task_notify_add;
struct doca_sync_event_task_wait_eq;
struct doca_sync_event_task_wait_neq;

/**
 * @brief Get task completion callback.
 *
 * @details See doca_task_completion_cb_t doc.
 *
 * @param [in] task
 * The successfully completed sync event get task.
 * The implementation can assume se_task is not NULL.
 * @param [in] task_user_data
 * Task's user data which was previously set.
 * @param [in] ctx_user_data
 * Context's user data which was previously set.
 */
typedef void (*doca_sync_event_task_get_completion_cb_t)(struct doca_sync_event_task_get *task,
							 union doca_data task_user_data,
							 union doca_data ctx_user_data);

/**
 * @brief Set task completion callback.
 *
 * @details See doca_task_completion_cb_t doc.
 *
 * @param [in] task
 * The successfully completed sync event set task.
 * The implementation can assume se_task is not NULL.
 * @param [in] task_user_data
 * Task's user data which was previously set.
 * @param [in] ctx_user_data
 * Context's user data which was previously set.
 */
typedef void (*doca_sync_event_task_notify_set_completion_cb_t)(struct doca_sync_event_task_notify_set *task,
								union doca_data task_user_data,
								union doca_data ctx_user_data);

/**
 * @brief Add task completion callback.
 *
 * @details See doca_task_completion_cb_t doc.
 *
 * @param [in] task
 * The successfully completed sync event add task.
 * The implementation can assume se_task is not NULL.
 * @param [in] task_user_data
 * Task's user data which was previously set.
 * @param [in] ctx_user_data
 * Context's user data which was previously set.
 */
typedef void (*doca_sync_event_task_notify_add_completion_cb_t)(struct doca_sync_event_task_notify_add *task,
								union doca_data task_user_data,
								union doca_data ctx_user_data);

/**
 * @brief Wait eq task completion callback.
 *
 * @details See doca_task_completion_cb_t doc.
 *
 * @param [in] task
 * The successfully completed sync event wait eq task.
 * The implementation can assume se_task is not NULL.
 * @param [in] task_user_data
 * Task's user data which was previously set.
 * @param [in] ctx_user_data
 * Context's user data which was previously set.
 */
typedef void (*doca_sync_event_task_wait_eq_completion_cb_t)(struct doca_sync_event_task_wait_eq *task,
							     union doca_data task_user_data,
							     union doca_data ctx_user_data);

/**
 * @brief Wait neq task completion callback.
 *
 * @details See doca_task_completion_cb_t doc.
 *
 * @param [in] task
 * The successfully completed sync event wait neq task.
 * The implementation can assume se_task is not NULL.
 * @param [in] task_user_data
 * Task's user data which was previously set.
 * @param [in] ctx_user_data
 * Context's user data which was previously set.
 */
typedef void (*doca_sync_event_task_wait_neq_completion_cb_t)(struct doca_sync_event_task_wait_neq *task,
							      union doca_data task_user_data,
							      union doca_data ctx_user_data);

/**
 * @brief Create a Sync Event handle.
 *
 * @details
 * Creates CPU handle - Host CPU or DPU's CPU.
 *
 * @param [out] event
 * The created doca_sync_event instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - event argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_sync_event.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_create(struct doca_sync_event **event);

/**
 * @brief Create a Sync Event handle from an export.
 *
 * @details
 * Creates a remote PCI handle.
 * The DOCA Device should be capable of importing an exported Sync Event
 * (see doca_sync_event_cap_is_create_from_export_supported capability).
 *
 * @note
 * The Sync Event can not be configured.
 *
 * @param [in] dev
 * doca_dev instance to be attached to the create doca_sync_event.
 *
 * @param [in] data
 * Exported doca_sync_event data stream, created by doca_sync_event_export_to_remote_pci call.
 *
 * @param [in] sz
 * Size of exported doca_sync_event data stream, created by doca_sync_event_export_to_remote_pci call.
 *
 * @param [out] event
 * The created doca_sync_event instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided doca_dev does not support creating Sync Event from export.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_sync_event.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_create_from_export(struct doca_dev *dev,
						const uint8_t *data,
						size_t sz,
						struct doca_sync_event **event);

/**
 * Check if given device is capable of creating Sync Event from an export.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports creating Sync Event from an export.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support importing an exported Sync Event.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_is_create_from_export_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Create a remote Sync Event handle from an export.
 *
 * @details
 * Creates a remote handle.
 * The DOCA Device should be capable of importing an exported Sync Event
 * (see doca_sync_event_cap_remote_net_is_create_from_export_supported capability).
 *
 * @note
 * The Sync Event can not be configured.
 *
 * @param [in] dev
 * doca_dev instance to be attached to the create doca_sync_event.
 *
 * @param [in] data
 * Exported doca_sync_event data stream, created by doca_sync_event_export_to_remote_net call.
 *
 * @param [in] sz
 * Size of exported doca_sync_event data stream, created by doca_sync_event_export_to_remote_net call.
 *
 * @param [out] event
 * The created doca_sync_event instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided doca_dev does not support creating a remote Sync Event from export.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_sync_event_remote_net.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_remote_net_create_from_export(struct doca_dev *dev,
							   const uint8_t *data,
							   size_t sz,
							   struct doca_sync_event_remote_net **event);

/**
 * Check if given device is capable of creating a remote Sync Event from an export.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports creating a remote Sync Event from an export.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support creating remote sync event from an exported Sync
 * Event.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_remote_net_is_create_from_export_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Destroy a Sync Event instance.
 *
 * @param [in] event
 * doca_sync_event to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - event argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_destroy(struct doca_sync_event *event);

/**
 * @brief Destroy a Sync Event instance.
 *
 * @param [in] event
 * doca_sync_event_remote_net to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - event argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_remote_net_destroy(struct doca_sync_event_remote_net *event);

/**
 * @brief Associate a CPU device context as the Sync Event publisher.
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @param [in] dev
 * doca_dev instance associated with CPU.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_publisher_location_cpu(struct doca_sync_event *event, struct doca_dev *dev);

/**
 * @brief Associate a DOCA DPA context as the Sync Event publisher.
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @param [in] dpa
 * doca_dpa instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_publisher_location_dpa(struct doca_sync_event *event, struct doca_dpa *dpa);

/**
 * @brief Associate a DOCA GPU context as the Sync Event publisher.
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @param [in] gpu
 * doca_gpu instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_publisher_location_gpu(struct doca_sync_event *event, struct doca_gpu *gpu);

/**
 * @brief Declare Sync Event publisher as remote pci.
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - event argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_publisher_location_remote_pci(struct doca_sync_event *event);

/**
 * @brief Declare Sync Event publisher as a remote peer.
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - event argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_publisher_location_remote_net(struct doca_sync_event *event);

/**
 * Associate a CPU device context as the doca_sync_event subscriber,
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @param [in] dev
 * doca_dev instance associated with CPU.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_subscriber_location_cpu(struct doca_sync_event *event, struct doca_dev *dev);

/**
 * @brief Associate a DOCA DPA context as the Sync Event subscriber.
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @param [in] dpa
 * doca_dpa instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_subscriber_location_dpa(struct doca_sync_event *event, struct doca_dpa *dpa);

/**
 * @brief Associate a DOCA GPU context as the Sync Event subscriber.
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @param [in] gpu
 * doca_gpu instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_subscriber_location_gpu(struct doca_sync_event *event, struct doca_gpu *gpu);

/**
 * @brief Declare Sync Event subscriber as remote PCI.
 *
 * @param [in] event
 * Target doca_sync_event instance to set.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - event argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_add_subscriber_location_remote_pci(struct doca_sync_event *event);

/**
 * @brief Set the 64-bit value's memory for a Sync Event.
 *
 * @param [in] event
 * Pointer to a sync event instance to be configured.
 *
 * @param [in] addr
 * Pointer to an 8-bytes memory - should points to a memory on the same address space
 * where the sync event was created (using doca_sync_event_create)
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - setting address for event which has already been started is not allowed.
 * - DOCA_ERROR_NOT_SUPPORTED - addr is in unsupported address space.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_set_addr(struct doca_sync_event *event, uint64_t *addr);

/**
 * @brief Set the 64-bit value's buffer for a Sync Event.
 *
 * @details
 * The DOCA buffer can be associated with a dma_buf fd by using mmap dmabuf feature
 * Allows Thread-to-Thread, Process-to-Process, VM-to-VM notifications on the same PCI domain
 * by sharing (using the below API) this buffer with multiple DOCA Sync Event instances.
 * DOCA Sync Event then uses this buffer to store its value
 *
 * User may utilize doca_mmap and doca_buf dmabuf feature to access GPU memory within the host.

 *
 * @param [in] event
 * Pointer to se event instance to be configured.
 *
 * @param [in] buf
 * A DOCA buffer.
 * @note
 * Buffer must be configured with read, write and atomic permissions (local and remote).
 * For exporting the sync event, remote pci read/write permissions should be set too.
 * Data section of the given doca_buf must be 8-bytes length.
 * Buffer list is not supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - setting address for event which has already been started is not allowed.
 * - DOCA_ERROR_NOT_PERMITTED - invalid buffer permissions or length
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_set_doca_buf(struct doca_sync_event *event, struct doca_buf *buf);

/**
 * @brief Start a Sync Event to be operate as stand-alone DOCA Core object only.
 *
 * @details
 * Starting a Sync Event with doca_sync_event_start means it can't be operate as (and converted to) DOCA Context.
 *
 * @param [in] event
 * Pointer to se event instance to be started.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - event argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_start(struct doca_sync_event *event);

/**
 * @brief Stop a Sync Event which has been previously started with 'doca_sync_event_start'.
 *
 * @param [in] event
 * Pointer to se event instance to be stopped.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - event argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_stop(struct doca_sync_event *event);

/**
 * @brief Convert a Sync Event to a DOCA context.
 *
 * @details
 * Set the Sync Event to operate as a DOCA Context only,
 * hence it can be interacted with through the supported DOCA Context API.
 *
 * Sync Event CTX supports the following operations: start/stop/get_event_driven_supported.
 * A device can't be attached to a sync event ctx.
 *
 * A user can use an attached (to Sync Event CTX) DOCA PE
 * to perform operations on the underlying Sync Event asynchronously by submitting tasks to the attached DOCA PE
 *
 * It is suggested to use Sync Event in this mode to wait on a Sync Event in a blocking manner.
 *
 * @param [in] event
 * The doca_sync_event to be converted
 *
 * @return
 * The matching doca_ctx instance in case of success,
 * NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_sync_event_as_ctx(struct doca_sync_event *event);

/**
 * @brief Check if a given device supports submitting a DOCA Sync Event get task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports submitting a sync event get task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support submitting a sync event get task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_task_get_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Set the DOCA Sync Event get task configuration.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] completion_cb
 * The get task completion callback.
 * @param [in] error_cb
 * The get task error callback.
 * @param [in] num_tasks
 * Number of sync event's inflight get tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received null parameter.
 * - DOCA_ERROR_NOT_PERMITTED - context not in idle state.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_get_set_conf(struct doca_sync_event *event,
					       doca_sync_event_task_get_completion_cb_t completion_cb,
					       doca_sync_event_task_get_completion_cb_t error_cb,
					       uint32_t num_tasks);

/**
 * @brief Allocate a DOCA Sync Event get task.
 *
 * @note This task gets the value of the sync event.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] ret_val_ptr
 * A pointer to the return value.
 * @param [in] user_data
 * doca_data to attach the task, which is later passed to the task's completion CBs.
 * @param [out] task
 * The allocated get task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_get_alloc_init(struct doca_sync_event *event,
						 uint64_t *ret_val_ptr,
						 union doca_data user_data,
						 struct doca_sync_event_task_get **task);

/**
 * @brief Convert a DOCA Sync Event get task to a DOCA Task.
 *
 * @param [in] task
 * The doca sync event get task.
 *
 * @return
 * The matching doca task in case of success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_sync_event_task_get_as_doca_task(struct doca_sync_event_task_get *task);

/**
 * @brief Set the return value pointer of a DOCA Sync Event get task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] ret_val_ptr
 * The return value pointer to set.
 */
DOCA_EXPERIMENTAL
void doca_sync_event_task_get_set_ret_val_ptr(struct doca_sync_event_task_get *task, uint64_t *ret_val_ptr);

/**
 * @brief Get the return value pointer of a DOCA Sync Event get task.
 *
 * @param [in] task
 * The task to get its return value pointer.
 *
 * @return
 * The task's return value pointer.
 */
DOCA_EXPERIMENTAL
uint64_t *doca_sync_event_task_get_get_ret_val_ptr(const struct doca_sync_event_task_get *task);

/**
 * @brief Check if a given device supports submitting a DOCA Sync Event notify-set task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports submitting a sync event notify-set task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support submitting a sync event notify-set task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_task_notify_set_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Set the DOCA Sync Event notify-set task configuration.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] completion_cb
 * The get task completion callback.
 * @param [in] error_cb
 * The get task error callback.
 * @param [in] num_tasks
 * Number of sync event's inflight notify-set tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received null parameter.
 * - DOCA_ERROR_NOT_PERMITTED - context not in idle state.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_notify_set_set_conf(struct doca_sync_event *event,
						      doca_sync_event_task_notify_set_completion_cb_t completion_cb,
						      doca_sync_event_task_notify_set_completion_cb_t error_cb,
						      uint32_t num_tasks);

/**
 * @brief Allocate a DOCA Sync Event notify-set task.
 *
 * @note This task sets the value of the sync event to a given value.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] set_val
 * A value to set the sync event to.
 * @param [in] user_data
 * doca_data to attach the task, which is later passed to the task's completion CBs.
 * @param [out] task
 * The allocated notify-set task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_notify_set_alloc_init(struct doca_sync_event *event,
							uint64_t set_val,
							union doca_data user_data,
							struct doca_sync_event_task_notify_set **task);

/**
 * @brief Convert a DOCA Sync Event notify-set task to a DOCA Task.
 *
 * @param [in] task
 * The doca sync event notify-set task.
 *
 * @return
 * The matching doca task in case of success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_sync_event_task_notify_set_as_doca_task(struct doca_sync_event_task_notify_set *task);

/**
 * @brief Set the set value of a DOCA Sync Event notify-set task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] set_val
 * The set value to set.
 */
DOCA_EXPERIMENTAL
void doca_sync_event_task_notify_set_set_set_val(struct doca_sync_event_task_notify_set *task, uint64_t set_val);

/**
 * @brief Get the set value of a DOCA Sync Event notify-set task.
 *
 * @param [in] task
 * The doca sync event notify-set task.
 *
 * @return
 * The task's set value.
 */
DOCA_EXPERIMENTAL
uint64_t doca_sync_event_task_notify_set_get_set_val(const struct doca_sync_event_task_notify_set *task);

/**
 * @brief Check if a given device supports submitting a DOCA Sync Event notify-add task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports submitting a sync event notify-add task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support submitting a sync event notify-add task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_task_notify_add_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Set the DOCA Sync Event notify-add task configuration.
 *
 * @param [in] event
 * The associated sync event
 * @param [in] completion_cb
 * The get task completion callback
 * @param [in] error_cb
 * The get task error callback
 * @param [in] num_tasks
 * Number of sync event's inflight notify-add tasks
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received null parameter.
 * - DOCA_ERROR_NOT_PERMITTED - context not in idle state.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_notify_add_set_conf(struct doca_sync_event *event,
						      doca_sync_event_task_notify_add_completion_cb_t completion_cb,
						      doca_sync_event_task_notify_add_completion_cb_t error_cb,
						      uint32_t num_tasks);

/**
 * @brief Allocate a DOCA Sync Event notify-add task.
 *
 * @note This task atomically increment the value of the sync event by a given value.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] inc_val
 * A value to increment the sync event by.
 * @param [in] fetched_val_ptr
 * A pointer to the fetched sync event value (prior to the increment).
 * @param [in] user_data
 * doca_data to attach the task, which is later passed to the task's completion CBs.
 * @param [out] task
 * The allocated notify-add task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_notify_add_alloc_init(struct doca_sync_event *event,
							uint64_t inc_val,
							uint64_t *fetched_val_ptr,
							union doca_data user_data,
							struct doca_sync_event_task_notify_add **task);

/**
 * @brief Convert a DOCA Sync Event notify-add task to a DOCA Task.
 *
 * @param [in] task
 * The doca sync event notify-add task.
 *
 * @return
 * The matching doca task in case of success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_sync_event_task_notify_add_as_doca_task(struct doca_sync_event_task_notify_add *task);

/**
 * @brief Set the increment value of a doca notify-add task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] inc_val
 * The increment value to set.
 */
DOCA_EXPERIMENTAL
void doca_sync_event_task_notify_add_set_inc_val(struct doca_sync_event_task_notify_add *task, uint64_t inc_val);

/**
 * @brief Get the increment value of a DOCA Sync Event notify-add task.
 *
 * @param [in] task
 * The doca sync event notify-add task.
 *
 * @return
 * The task's increment value.
 */
DOCA_EXPERIMENTAL
uint64_t doca_sync_event_task_notify_add_get_inc_val(const struct doca_sync_event_task_notify_add *task);

/**
 * @brief Set the fetched value pointer of a DOCA Sync Event notify-add task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] fetched_val_ptr
 * The fetched value pointer to set.
 */
DOCA_EXPERIMENTAL
void doca_sync_event_task_notify_add_set_fetched_val_ptr(struct doca_sync_event_task_notify_add *task,
							 uint64_t *fetched_val_ptr);

/**
 * @brief Get the fetched value pointer of a DOCA Sync Event notify-add task.
 *
 * @param [in] task
 * The task to get its fetched value pointer.
 *
 * @return
 * The task's fetched value pointer.
 */
DOCA_EXPERIMENTAL
uint64_t *doca_sync_event_task_notify_add_get_fetched_val_ptr(const struct doca_sync_event_task_notify_add *task);

/**
 * @brief Check if a given device supports submitting a DOCA Sync Event wait-equal task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports submitting a sync event wait-equal task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support submitting a sync event wait-equal task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_task_wait_eq_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Set the DOCA Sync Event wait-equal task configuration.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] completion_cb
 * The get task completion callback.
 * @param [in] error_cb
 * The get task error callback.
 * @param [in] num_tasks
 * Number of sync event's inflight wait-equal tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received null parameter.
 * - DOCA_ERROR_NOT_PERMITTED - context not in idle state.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_wait_eq_set_conf(struct doca_sync_event *event,
						   doca_sync_event_task_wait_eq_completion_cb_t completion_cb,
						   doca_sync_event_task_wait_eq_completion_cb_t error_cb,
						   uint32_t num_tasks);

/**
 * @brief Allocate a DOCA Sync Event wait-equal task.
 *
 * @note This task applies a given mask on the sync event value
 * and waits for the result to be equal to a given value.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] wait_val
 * Threshold to wait for the Sync Event to be equal to.
 * @param [in] mask
 * Mask for comparing the Sync Event value - mask must be consistent only of 0, 1, 2, 4 or 8 consecutive FFs.
 * @param [in] user_data
 * doca_data to attach the task, which is later passed to the task's completion CBs.
 * @param [out] task
 * The allocated wait-equal task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_wait_eq_alloc_init(struct doca_sync_event *event,
						     uint64_t wait_val,
						     uint64_t mask,
						     union doca_data user_data,
						     struct doca_sync_event_task_wait_eq **task);

/**
 * @brief Convert a DOCA Sync Event wait-equal task to a DOCA Task.
 *
 * @param [in] task
 * The doca sync event wait-equal task.
 *
 * @return
 * The matching doca task in case of success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_sync_event_task_wait_eq_as_doca_task(struct doca_sync_event_task_wait_eq *task);

/**
 * @brief Set the wait value of a DOCA wait-equal task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] wait_val
 * The wait value to set.
 */
DOCA_EXPERIMENTAL
void doca_sync_event_task_wait_eq_set_wait_val(struct doca_sync_event_task_wait_eq *task, uint64_t wait_val);

/**
 * @brief Get the wait value of a DOCA Sync Event wait-equal task.
 *
 * @param [in] task
 * The doca sync event wait-equal task.
 *
 * @return
 * The task's wait value.
 */
DOCA_EXPERIMENTAL
uint64_t doca_sync_event_task_wait_eq_get_wait_val(const struct doca_sync_event_task_wait_eq *task);

/**
 * @brief Set the mask of a DOCA wait-equal task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] mask
 * The mask to set.
 */
DOCA_EXPERIMENTAL
void doca_sync_event_task_wait_eq_set_mask(struct doca_sync_event_task_wait_eq *task, uint64_t mask);

/**
 * @brief Get the mask of a DOCA Sync Event wait-equal task.
 *
 * @param [in] task
 * The doca sync event wait-equal task.
 *
 * @return
 * The task's mask.
 */
DOCA_EXPERIMENTAL
uint64_t doca_sync_event_task_wait_eq_get_mask(const struct doca_sync_event_task_wait_eq *task);

/**
 * @brief Check if a given device supports submitting a DOCA Sync Event wait-not-equal task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports submitting a sync event wait-not-equal task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support submitting a sync event wait-not-equal task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_task_wait_neq_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Set the DOCA Sync Event wait-not-equal task configuration.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] completion_cb
 * The get task completion callback.
 * @param [in] error_cb
 * The get task error callback.
 * @param [in] num_tasks
 * Number of sync event's inflight wait-not-equal tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received null parameter.
 * - DOCA_ERROR_NOT_PERMITTED - context not in idle state.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_wait_neq_set_conf(struct doca_sync_event *event,
						    doca_sync_event_task_wait_neq_completion_cb_t completion_cb,
						    doca_sync_event_task_wait_neq_completion_cb_t error_cb,
						    uint32_t num_tasks);

/**
 * @brief Allocate a DOCA Sync Event wait-not-equal task.
 *
 * @note This task applies a given mask on the sync event value
 * and waits for the result to be different than a given value.
 *
 * @param [in] event
 * The associated sync event.
 * @param [in] wait_val
 * Threshold to wait for the Sync Event to be different than.
 * @param [in] mask
 * Mask for comparing the Sync Event value - mask must be consistent only of 0, 1, 2, 4 or 8 consecutive FFs.
 * @param [in] user_data
 * doca_data to attach the task, which is later passed to the task's completion CBs.
 * @param [out] task
 * The allocated wait-not-equal task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_task_wait_neq_alloc_init(struct doca_sync_event *event,
						      uint64_t wait_val,
						      uint64_t mask,
						      union doca_data user_data,
						      struct doca_sync_event_task_wait_neq **task);

/**
 * @brief Convert a DOCA Sync Event wait-not-equal task to a DOCA Task.
 *
 * @param [in] task
 * The doca sync event wait-not-equal task.
 *
 * @return
 * The matching doca task in case of success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_sync_event_task_wait_neq_as_doca_task(struct doca_sync_event_task_wait_neq *task);

/**
 * @brief Set the wait value of a DOCA wait-not-equal task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] wait_val
 * The wait value to set.
 */
DOCA_EXPERIMENTAL
void doca_sync_event_task_wait_neq_set_wait_val(struct doca_sync_event_task_wait_neq *task, uint64_t wait_val);

/**
 * @brief Get the wait value of a DOCA Sync Event wait-not-equal task.
 *
 * @param [in] task
 * The doca sync event wait-not-equal task.
 *
 * @return
 * The task's wait value.
 */
DOCA_EXPERIMENTAL
uint64_t doca_sync_event_task_wait_neq_get_wait_val(const struct doca_sync_event_task_wait_neq *task);

/**
 * @brief Set the mask of a DOCA wait-not-equal task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] mask
 * The mask to set.
 */
DOCA_EXPERIMENTAL
void doca_sync_event_task_wait_neq_set_mask(struct doca_sync_event_task_wait_neq *task, uint64_t mask);

/**
 * @brief Get the mask of a DOCA Sync Event wait-not-equal task.
 *
 * @param [in] task
 * The doca sync event wait-not-equal task.
 *
 * @return
 * The task's mask.
 */
DOCA_EXPERIMENTAL
uint64_t doca_sync_event_task_wait_neq_get_mask(const struct doca_sync_event_task_wait_neq *task);

/**
 * @brief Export Sync Event to be shared with remote PCI.
 *
 * @details
 * Create export data stream used for synchronize between CPU (HOST or DPU) and remote PCI.
 * Sync Event should be properly configured, both subscriber and publisher
 * must be declared as either CPU or remote PCI location.
 * The underlying DOCA Device should be capable of exporting to remote PCI
 * (see doca_sync_event_cap_is_export_to_remote_pci_supported capability).
 *
 * The exported data stream an be used from the remote PCI to created an exported Sync Event
 * (see doca_sync_event_create_from_export).
 *
 * @param [in] event
 * Target doca_sync_event instance to export.
 *
 * @param [in] dev
 * Target dev to export.
 *
 * @param [out] data
 * The created export data stream.
 *
 * @param [out] sz
 * Size of created export data stream.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support this Sync Event action.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc data stream.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_export_to_remote_pci(struct doca_sync_event *event,
						  struct doca_dev *dev,
						  const uint8_t **data,
						  size_t *sz);

/**
 * Check if a DOCA device is capable of exporting an associated Sync Event to remote PCI
 * using doca_sync_event_export_to_remote_pci.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports exporting an associated Sync Event to remote PCI.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support exporting an associated Sync Event to remote PCI.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_is_export_to_remote_pci_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Export Sync Event to be shared with the DPA.
 *
 * @details
 * Create Sync Event DPA handle used for synchronize between the x86 CPU HOST and the DPA.
 * Sync Event should be properly configured, either subscriber or publisher should be declared as DPA location.
 * The underlying DOCA Device should be capable of exporting to DPA
 * (see doca_sync_event_cap_is_export_to_dpa_supported capability).
 * A Sync Event can be exported from the Host CPU only.
 *
 * The DOCA DPA Sync Event is an handle to be used from the DPA to perform operations on the associated Sync Event.
 *
 * @param [in] event
 * Target doca_sync_event instance to export.
 *
 * @param [in] dpa
 * The associated DOCA DPA Context.
 *
 * @param [out] dpa_dev_se_handle
 * DOCA DPA device sync event handle that can be passed to a kernel.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support this Sync Event action.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_dpa_sync_event.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_get_dpa_handle(struct doca_sync_event *event,
					    struct doca_dpa *dpa,
					    doca_dpa_dev_sync_event_t *dpa_dev_se_handle);

/**
 * Check if a DOCA device is capable of exporting an associated Sync Event to the DPA
 * using doca_sync_event_get_dpa_handle.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports exporting an associated Sync Event to DPA.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support exporting an associated Sync Event to DPA.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_is_export_to_dpa_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Export Sync Event to be shared with the GPU.
 *
 * @details
 * Create Sync Event GPU handle used for synchronize between the x86 CPU HOST and the DPA.
 * Sync Event should be properly configured, either subscriber or publisher should be declared as GPU location.
 * The underlying DOCA Device should be capable of exporting to GPU
 * (see doca_sync_event_cap_is_export_to_gpu_supported capability).
 * A Sync Event can be exported from the Host CPU only.
 *
 * The DOCA GPU Sync Event is an handle to be used from the GPU to perform operations on the associated Sync Event.
 *
 * @param [in] event
 * Target doca_sync_event instance to export.
 *
 * @param [in] gpu
 * The associated DOCA GPU Context.
 *
 * @param [out] gpu_dev_se
 * DOCA GPU device sync event handle that can be passed to a kernel.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support this Sync Event action.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_gpu_sync_event.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_get_gpu_handle(struct doca_sync_event *event,
					    struct doca_gpu *gpu,
					    doca_gpu_dev_sync_event_t **gpu_dev_se);

/**
 * Check if a DOCA device is capable of exporting an associated Sync Event to the GPU
 * using doca_sync_event_get_gpu_handle.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports exporting an associated Sync Event to GPU.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support exporting an associated Sync Event to GPU.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_is_export_to_gpu_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Export Sync Event to be shared with a remote peer.
 *
 * @details
 * Create export data stream used for synchronize between the origin system to a remote peer.
 * Sync Event should be properly configured, publisher must be declared as remote net location.
 * The underlying DOCA Device should be capable of exporting to a remote net (see
 * doca_sync_event_get_export_to_remote_net_supported capability).
 *
 * The exported data stream should be used on a remote peer to created a Sync Event remote handle.
 * (see doca_sync_event_remote_net_create_from_export).
 *
 * @param [in] event
 * Target doca_sync_event instance to export.
 *
 * @param [out] data
 * The created export data stream.
 *
 * @param [out] sz
 * Size of created export data stream.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support this Sync Event action.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc data stream.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_export_to_remote_net(struct doca_sync_event *event, const uint8_t **data, size_t *sz);

/**
 * Check if a DOCA device is capable of exporting an associated Sync Event to a remote peer
 * using doca_sync_event_export_to_remote_net.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports exporting an associated Sync Event to a remote peer.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support exporting an associated Sync Event to a remote peer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_is_export_to_remote_net_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Export remote Sync Event to be shared with the DPA.
 *
 * @details
 * Create remote Sync Event DPA handle used for synchronize between a remote peer and the local DPA.
 * The underlying DOCA Device should be capable of exporting a remote sync event to DPA (see
 * doca_sync_event_cap_remote_net_is_export_to_dpa_supported capability).
 * A Sync Event can be exported from the Host CPU only.
 *
 * The DOCA DPA Remote Sync Event is an handle to be used from the DPA to perform operations on the associated remote
 * Sync Event.
 *
 * @param [in] event
 * Target doca_sync_event_remote_net instance to export.
 *
 * @param [in] dpa
 * The associated DOCA DPA Context.
 *
 * @param [out] dpa_remote_event
 * DOCA DPA device remote sync event handle that can be passed to a kernel.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support this Sync Event action.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_dpa_dev_sync_event_remote_net_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_remote_net_get_dpa_handle(struct doca_sync_event_remote_net *event,
						       struct doca_dpa *dpa,
						       doca_dpa_dev_sync_event_remote_net_t *dpa_remote_event);

/**
 * Check if a DOCA device is capable of exporting an associated remote Sync Event to the DPA
 * using doca_sync_event_remote_net_get_dpa_handle.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports exporting an associated Sync Event to DPA.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support exporting an associated remote Sync Event to DPA.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_remote_net_is_export_to_dpa_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Export remote Sync Event to be shared with the GPU.
 *
 * @details
 * Create remote Sync Event GPU handle used for synchronize between a remote peer and the local GPU.
 * The underlying DOCA Device should be capable of exporting a remote sync event to GPU (see
 * doca_sync_event_cap_remote_net_is_export_to_gpu_supported capability).
 * A Sync Event can be exported from the Host CPU only.
 *
 * The DOCA GPU Remote Sync Event is an handle to be used from the GPU to perform operations on the associated remote
 * Sync Event.
 *
 * @param [in] event
 * Target doca_sync_event_remote_net instance to export.
 *
 * @param [in] gpu
 * The associated DOCA GPU Context.
 *
 * @param [out] gpu_remote_event
 * DOCA GPU device remote sync event handle that can be passed to a kernel.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support this Sync Event action.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_gpu_dev_sync_event_remote_net_t .
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_remote_net_get_gpu_handle(struct doca_sync_event_remote_net *event,
						       struct doca_gpu *gpu,
						       doca_gpu_dev_sync_event_remote_net_t *gpu_remote_event);

/**
 * Check if a DOCA device is capable of exporting an associated remote Sync Event to the GPU
 * using doca_sync_event_remote_net_get_gpu_handle.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device supports exporting an associated Sync Event to GPU.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support exporting an associated remote Sync Event to GPU.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_cap_remote_net_is_export_to_gpu_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Get the value of a Sync Event synchronously.
 *
 * @param [in] event
 * Target doca_sync_event instance to read its value.
 *
 * @param [out] value
 * The returned doca_sync_event value.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_get(struct doca_sync_event *event, uint64_t *value);

/**
 * @brief Atomically increase the value of a Sync Event by some value synchronously.
 *
 * @param [in] event
 * Target doca_sync_event instance to increment.
 *
 * @param [in] value
 * The value to increment the doca_sync_event value by.
 *
 * @param [out] fetched
 * The value of the doca_sync_event before the operation.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_update_add(struct doca_sync_event *event, uint64_t value, uint64_t *fetched);

/**
 * @brief Set the value of a Sync Event to some value synchronously.
 *
 * @param [in] event
 * Target doca_sync_event instance to set its value.
 *
 * @param [in] value
 * The value to set the doca_sync_event to.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_update_set(struct doca_sync_event *event, uint64_t value);

/**
 * @brief Wait for the value of a Sync Event to be grater than some threshold value synchronously
 * in a polling busy wait manner.
 *
 * @param [in] event
 * Target doca_sync_event instance to wait on.
 *
 * @param [in] value
 * The value to wait for the doca_sync_event to be greater than.
 *
 * @param [in] mask
 * Mask to apply (bitwise AND) on the doca_sync_event value for comparison with wait threshold.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_wait_gt(struct doca_sync_event *event, uint64_t value, uint64_t mask);

/**
 * @brief Wait for the value of a Sync Event to be grater than some threshold value synchronously
 * in a periodically busy wait manner.
 *
 * @details
 * After each polling iteration, call sched_yield
 * sched_yield() causes the calling thread to relinquish the CPU.
 * The thread is moved to the end of the queue for its static priority and a new thread gets to run.
 *
 * @param [in] event
 * Target doca_sync_event instance to wait on.
 *
 * @param [in] value
 * The value to wait for the doca_sync_event to be greater than.
 *
 * @param [in] mask
 * Mask to apply (bitwise AND) on the doca_sync_event value for comparison with wait threshold.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_wait_gt_yield(struct doca_sync_event *event, uint64_t value, uint64_t mask);

/**
 * @brief Wait for the value of a Sync Event to be equal to some value synchronously in a busy wait manner.
 *
 * @param [in] event
 * Target doca_sync_event instance to wait on.
 *
 * @param [in] value
 * The value to wait for the doca_sync_event to be equal to.
 *
 * @param [in] mask
 * Mask to apply (bitwise AND) on the doca_sync_event value for comparison with wait threshold.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_wait_eq(struct doca_sync_event *event, uint64_t value, uint64_t mask);

/**
 * @brief Wait for the value of a Sync Event to be equal to some value synchronously
 * in a periodically busy wait manner.
 *
 * @details
 * After each polling iteration, call sched_yield
 * sched_yield() causes the calling thread to relinquish the CPU.
 * The thread is moved to the end of the queue for its static priority and a new thread gets to run.
 *
 * @param [in] event
 * Target doca_sync_event instance to wait on.
 *
 * @param [in] value
 * The value to wait for the doca_sync_event to be equal to.
 *
 * @param [in] mask
 * Mask to apply (bitwise AND) on the doca_sync_event value for comparison with wait threshold.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_wait_eq_yield(struct doca_sync_event *event, uint64_t value, uint64_t mask);

/**
 * @brief Wait for the value of a Sync Event to be not equal to some value synchronously in a busy wait manner.
 *
 * @param [in] event
 * Target doca_sync_event instance to wait on.
 *
 * @param [in] value
 * The value to wait for the doca_sync_event to be not equal to.
 *
 * @param [in] mask
 * Mask to apply (bitwise AND) on the doca_sync_event value for comparison with wait threshold.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_wait_neq(struct doca_sync_event *event, uint64_t value, uint64_t mask);

/**
 * @brief Wait for the value of a Sync Event to be not equal to some value synchronously
 * in a periodically busy wait manner.
 *
 * @details
 * After each polling iteration, call sched_yield
 * sched_yield() causes the calling thread to relinquish the CPU.
 * The thread is moved to the end of the queue for its static priority and a new thread gets to run.
 *
 * @param [in] event
 * Target doca_sync_event instance to wait on.
 *
 * @param [in] value
 * The value to wait for the doca_sync_event to be not equal to.
 *
 * @param [in] mask
 * Mask to apply (bitwise AND) on the doca_sync_event value for comparison with wait threshold.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - any of the arguments is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_sync_event_wait_neq_yield(struct doca_sync_event *event, uint64_t value, uint64_t mask);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_SYNC_EVENT_H_ */
