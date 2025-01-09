/*
 * Copyright (c) 2023-2024 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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

#ifndef DOCA_UROM_H_
#define DOCA_UROM_H_

#include <doca_error.h>
#include <doca_types.h>
#include <doca_urom_plugin.h>

/**
 * @defgroup DOCA_UROM_SERVICE DOCA UROM Services
 * @{
 *  DOCA UROM Service interfaces and data structures.
 *
 *  The UROM Service manages Workers running on a DOCA device.
 * @}
 */

/*!
@defgroup DOCA_UROM_ENV_VARS DOCA UROM Environment Variables
@{
  DOCA UROM Environment Variables.

  DOCA UROM environment variables are described in the following table.

  Variable      | Description
  ------------- | -------------
  DOCA_UROM_SERVICE_FILE  | Path to the UROM Service File.

  \section SVC_FILE_VAR DOCA_UROM_SERVICE_FILE

  This variable sets the path to the UROM Service File. When creating
  a UROM Service, UROM performs a look up using this file, the hostname where
  an application is running, and the PCI address of the associated DOCA device
  to identify the network address, and network devices associated with the UROM
  Service.

  This file contains one entry per line describing the location of each UROM
  Service that may be used by UROM. The format of each line must be as follows:

      <app_hostname> <service_type> <dev_hostname> <dev_pci_addr> <net,devs>

  Fields are described in the following table.

  | Field         | Description |
  | ------------- | ----------- |
  | app_hostname  | Network host name (or IP address) for the node that this line applies to. |
  | service_type  | The UROM service type. Valid type is `dpu`. Type `dpu` is used for all DOCA Devices. |
  | dev_hostname  | Network host name (or IP address) for the associated DOCA device. |
  | dev_pci_addr  | PCI address of the associated DOCA device. This must match the PCI address provided by DOCA. |
  | net,devs      | Comma-separated list of network devices which are shared between the host and DOCA device. |

  As an example, an application running on a single node (named app_host1) which has a UROM
  Service running on DPU (named dpu_host1) would have a Service Directory file with the following entries:

      app_host1 dpu dpu_host1 03:00.0 dev1:1,dev2:1

 @}
 */

#include <ucs/type/cpu_set.h>
typedef ucs_cpu_set_t doca_cpu_set_t;
#define DOCA_CPU_ZERO(_cpusetp) UCS_CPU_ZERO(_cpusetp)
#define DOCA_CPU_SET(_cpu, _cpusetp) UCS_CPU_SET(_cpu, _cpusetp)
#define doca_cpu_is_set(_cpu, _cpusetp) ucs_cpu_is_set(_cpu, _cpusetp)

/**********************************************************************************************************************
 * DOCA core opaque types
 *********************************************************************************************************************/

struct doca_buf;
struct doca_ctx;
struct doca_dev;

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * Opaque structure representing a DOCA UROM Service context.
 */
struct doca_urom_service;

/**
 * @ingroup DOCA_UROM_SERVICE_TASK
 *
 * Opaque structure representing a DOCA Service get workers by group id.
 */
struct doca_urom_service_get_workers_by_gid_task;

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief Service get workers task completion callback type.
 */
typedef void (*doca_urom_service_get_workers_by_gid_task_completion_cb_t)(
	struct doca_urom_service_get_workers_by_gid_task *task,
	union doca_data task_user_data,
	union doca_data ctx_user_data);

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief This method creates a UROM Service context.
 *
 * The UROM Service runs on a DOCA Device. A Service manages UROM Workers which
 * run on the Device. A Worker runs UROM Plugins which implement application
 * logic which runs on the Device.
 *
 * A Service is created in state `DOCA_CTX_STATE_IDLE`. After creation,
 * a user may configure the Service using setter methods (e.g.
 * `doca_urom_service_set_dev()`). Before use, a Service must be transitioned
 * to state `DOCA_CTX_STATE_RUNNING` using the `doca_ctx_start()`
 * interface. A typical invocation looks like:
 *
 * `doca_ctx_start(doca_urom_service_as_ctx(service_ctx))`
 *
 * @param [out] service_ctx
 * The DOCA UROM Service
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - service_ctx argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc service_ctx.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_create(struct doca_urom_service **service_ctx);

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief This method destroys a UROM Service context.
 *
 * @param [in] service_ctx
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NOT_PERMITTED - Service isn't in IDLE state.
 * - DOCA_ERROR_IN_USE - One or more work queues are still attached. These must be detached first.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_destroy(struct doca_urom_service *service_ctx);

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief Set the maximum message size for the UROM communication channel
 *
 * @details This method sets the maximum size for message in UROM communication channel, default message size is 4096B.
 * The communication channel message is composed of the UROM urom_worker_notify/urom_worker_cmd structures
 * and the plugin payload (command/notification). It's important  to ensure that the combined size of plugins' commands
 * and notifications, along with the UROM structures size, does not exceed this maximum size.
 *
 * @param [in] service_ctx
 * The UROM Service context to set comm msg size
 * @param [in] msg_size
 * The maximum message size.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - service_ctx argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - service_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_set_max_comm_msg_size(struct doca_urom_service *service_ctx, size_t msg_size);

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief Set maximum number of UROM workers
 *
 * @details This method sets the maximum number of spawned workers
 *
 * @param [in] service_ctx
 * The UROM Service context to set number of workers.
 * @param [in] max_workers
 * The maximum number of workers, Must be a power of 2.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - service_ctx argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - service_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_set_max_workers(struct doca_urom_service *service_ctx, uint32_t max_workers);

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief This method attaches a DOCA Device to the UROM Service context.
 *
 * Attaching to a DOCA Device is mandatory, network address will be extracted from services mapping that is defined
 * in the user environment DOCA_UROM_SERVICE_FILE
 *
 * @param [in] service_ctx
 * The UROM Service context to attach the device to.
 * @param [in] dev
 * The device to attach.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - Any argument is a NULL pointer.
 * - DOCA_ERROR_NOT_SUPPORTED - UROM is not supported
 * - DOCA_ERROR_BAD_STATE - service_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_set_dev(struct doca_urom_service *service_ctx, struct doca_dev *dev);

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief Convert service_ctx instance into a generalized context for use with DOCA
 * core objects.
 *
 * @param [in] service_ctx
 * Service context instance. This must remain valid until after the context is
 * no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_urom_service_as_ctx(struct doca_urom_service *service_ctx);

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief This method gets the list of supported plugins on service's DPU side.
 *
 * @param [in] service_ctx Service context instance.
 * @param [out] plugins Set of supported plugins.
 * @param [out] plugins_count Set of plugins number.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - Any argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - service_ctx is not running.
 * - DOCA_ERROR_NOT_FOUND - No plugins were discovered on DPU side.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_get_plugins_list(struct doca_urom_service *service_ctx,
						const struct doca_urom_service_plugin_info **plugins,
						size_t *plugins_count);

/**
 * @ingroup DOCA_UROM_SERVICE
 *
 * @brief Get the allowed CPU set for the service.
 *
 * @param [in] service_ctx Service context instance.
 * @param [out] cpuset Set of allowed CPUs.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - Any argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - service_ctx is not started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_get_cpuset(struct doca_urom_service *service_ctx, doca_cpu_set_t *cpuset);

/**
 * @ingroup DOCA_UROM_SERVICE_TASK
 *
 * @brief Allocate Service get workers by gid task.
 *
 * @param [in] service_ctx
 * The service is used for the task.
 * @param [out] task
 * The new task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_get_workers_by_gid_task_allocate(struct doca_urom_service *service_ctx,
								struct doca_urom_service_get_workers_by_gid_task **task);

/**
 * @ingroup DOCA_UROM_SERVICE_TASK
 *
 * @brief Allocate Service get workers by gid task and set task attributes.
 *
 * @param [in] service_ctx
 * The service is used for the task.
 * @param [in] gid
 * Worker group id to set
 * @param [in] cb
 * Task completion callback to set
 * @param [out] task
 * The new task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_get_workers_by_gid_task_allocate_init(
	struct doca_urom_service *service_ctx,
	uint32_t gid,
	doca_urom_service_get_workers_by_gid_task_completion_cb_t cb,
	struct doca_urom_service_get_workers_by_gid_task **task);

/**
 * @ingroup DOCA_UROM_SERVICE_TASK
 *
 * @brief Release Service get workers by gid task.
 *
 * @param [in] task
 * The task to release
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NOT_PERMITTED - Task resources still in use.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_service_get_workers_by_gid_task_release(struct doca_urom_service_get_workers_by_gid_task *task);

/**
 * @ingroup DOCA_UROM_SERVICE_TASK
 *
 * @brief Set Service get workers task group id.
 *
 * @param [in] task
 * The task used to set the gid on.
 * @param [in] gid
 * The task group id to get workers by.
 */
DOCA_EXPERIMENTAL
void doca_urom_service_get_workers_by_gid_task_set_gid(struct doca_urom_service_get_workers_by_gid_task *task,
						       uint32_t gid);

/**
 * @ingroup DOCA_UROM_SERVICE_TASK
 *
 * @brief Set Service get workers task completion callback.
 *
 * @param [in] task
 * The task used to set the plugin on.
 * @param [in] cb
 * The task completion callback
 */
DOCA_EXPERIMENTAL
void doca_urom_service_get_workers_by_gid_task_set_cb(struct doca_urom_service_get_workers_by_gid_task *task,
						      doca_urom_service_get_workers_by_gid_task_completion_cb_t cb);

/**
 * @brief This method converts a service get workers task to doca_task
 *
 * @param [in] task
 * doca_urom_service_get_workers_by_gid_task task
 *
 * @return doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_urom_service_get_workers_by_gid_task_as_task(
	struct doca_urom_service_get_workers_by_gid_task *task);

/**
 * @ingroup DOCA_UROM_SERVICE_TASK
 *
 * @brief Get the number of workers returned for the requested gid
 *
 * @param [in] task
 * The task to return a worker count.
 *
 * @return
 * The workers counts
 */
DOCA_EXPERIMENTAL
size_t doca_urom_service_get_workers_by_gid_task_get_workers_count(
	struct doca_urom_service_get_workers_by_gid_task *task);

/**
 * @ingroup DOCA_UROM_SERVICE_TASK
 *
 * @brief Get Service get workers task ids array
 *
 * @param [in] task
 * The task to return a worker ids array.
 *
 * @return
 * The worker ids array
 */
DOCA_EXPERIMENTAL
const uint64_t *doca_urom_service_get_workers_by_gid_task_get_worker_ids(
	struct doca_urom_service_get_workers_by_gid_task *task);

/**
 * @defgroup DOCA_UROM_WORKER DOCA UROM Workers
 * @{
 *  DOCA UROM Worker interfaces and data structures.
 *
 *  The UROM Worker represents a process running on a DOCA device. The process
 *  may be used to offload application logic to the device. Application logic
 *  is defined in Worker plugins. A Worker may be configured to support one or
 *  more plugins concurrently. Users submit DOCA tasks to a Worker in order to
 *  initiate an offloaded operation.
 * @}
 */

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * Opaque structure representing a DOCA UROM Worker context.
 */
struct doca_urom_worker;

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * Opaque structure representing a DOCA UROM Worker Command task context.
 */
struct doca_urom_worker_cmd_task;

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief Worker Command task completion callback type.
 */
typedef void (*doca_urom_worker_cmd_task_completion_cb_t)(struct doca_urom_worker_cmd_task *task,
							  union doca_data task_user_data,
							  union doca_data ctx_user_data);

/**`
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief This method creates a UROM Worker context.
 *
 * A Worker is created in state `DOCA_CTX_STATE_IDLE`. After creation,
 * a user may configure the Worker using setter methods (e.g.
 * `doca_urom_worker_set_service()`). Before use, a Worker must be transitioned
 * to state `DOCA_CTX_STATE_RUNNING` using the `doca_ctx_start()`
 * interface. A typical invocation looks like:
 *
 * `doca_ctx_start(doca_urom_worker_as_ctx(worker_ctx))`
 *
 * @param [out] worker_ctx The new Worker context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - worker_ctx argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc worker_ctx.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_create(struct doca_urom_worker **worker_ctx);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief This method destroys a UROM Worker context.
 *
 * @param [in] worker_ctx Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - Unable to gain exclusive access to the worker_ctx instance.
 * - DOCA_ERROR_IN_USE - One or more work queues are still attached. These must be detached first.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_destroy(struct doca_urom_worker *worker_ctx);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief This method attaches a UROM Service to the Worker context.
 *
 * The Worker will be launched on the DOCA Device managed by the provided
 * Service.
 *
 * @param [in] worker_ctx
 * The UROM Worker context.
 * @param [in] service_ctx
 * The UROM Service context that will serve this worker.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - Any argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - worker_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_set_service(struct doca_urom_worker *worker_ctx, struct doca_urom_service *service_ctx);

#define DOCA_UROM_WORKER_ID_ANY -1ULL

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief This method sets the Worker context ID to be used to identify the
 * Worker. Worker IDs enable an application to establish multiple connections
 * to the same Worker process running on a DOCA Device.
 *
 * Worker ID must be unique to a UROM Service. If DOCA_UROM_WORKER_ID_ANY is
 * specified, the Service will assign a unique ID for the newly created worker.
 *
 * If a specific ID is used, the Service will look for an existing Worker with
 * matching ID. If one exists, the Service will establish a new connection to
 * the existing Worker. If a matching Worker does not exist, a new Worker will
 * be created with the specified Worker ID.
 *
 * Parallel tasks can coordinate by using a convention for worker_id which
 * includes task ID and worker index. task ID should come from a launcher.
 * Worker index can be derived using a local process rank (also from a
 * launcher) and the desired number of workers. The number of workers
 * should be based on the number of available cores for offload, which can
 * be queried through DOCA UROM. The process for setting up workers is then:
 *
 *    1. Query DOCA UROM for available cores on the Service
 *    2. Query launcher for task_id and local_rank
 *    3. Derive ID: worker_id = task_id << 32 | (local_rank % cores)
 *    4. Create a new Worker context with the derived worker_id
 *
 * @param [in] worker_ctx The UROM Worker context.
 * @param [in] worker_id The UROM Worker ID.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - worker_ctx argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - worker_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_set_id(struct doca_urom_worker *worker_ctx, uint64_t worker_id);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief This method adds plugins mask for the supported plugins by the UROM Worker on the DPU.
 *
 * @param [in] worker_ctx
 * The UROM Worker context
 * @param [in] plugins
 * The plugin types (Expected as a bit mask on the plugin index's).
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - worker_ctx argument is a NULL pointer or plugin is an unreasonable value.
 * - DOCA_ERROR_BAD_STATE - worker_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_set_plugins(struct doca_urom_worker *worker_ctx, uint64_t plugins);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief Set the allowed CPUs for the Worker. The cpuset must be a subset of
 * the service's allowed CPUs.
 *
 * @param [in] worker_ctx The UROM Worker context.
 * @param [in] cpuset The set of allowed CPUs.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * DOCA_ERROR_INVALID_VALUE - Any argument is a NULL pointer, or, the the cpuset is invalid.
 * DOCA_ERROR_BAD_STATE - worker_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_set_cpuset(struct doca_urom_worker *worker_ctx, doca_cpu_set_t cpuset);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief Set worker group ID, must be set before starting the worker context.
 *
 * @param [in] worker_ctx The UROM Worker context.
 * @param [in] gid group id
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * DOCA_ERROR_INVALID_VALUE - Any argument is a NULL pointer, or, the the cpuset is invalid.
 * DOCA_ERROR_BAD_STATE - worker_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_set_gid(struct doca_urom_worker *worker_ctx, uint32_t gid);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief Set Worker maximum in-flight tasks.
 *
 * @param [in] worker_ctx The UROM Worker context.
 * @param [in] max_tasks The maximum number of tasks to set, should be power of two.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - worker_ctx is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - worker_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_set_max_inflight_tasks(struct doca_urom_worker *worker_ctx, uint32_t max_tasks);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief Set worker env variables when spawning worker on DPU side.
 *
 * Must be set before starting the worker context, will fail spawn command if worker already spawned on the DPU.
 * The env variable value shouldn't contain spaces.
 *
 * @param [in] worker_ctx
 * The UROM Worker context.
 * @param [in] env
 * A string array containing name=value tuples.
 * @param [in] count
 * String array size.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - Any argument is a NULL pointer, or, count in invalid.
 * - DOCA_ERROR_BAD_STATE - worker_ctx is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_set_env(struct doca_urom_worker *worker_ctx, char *const env[], size_t count);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief This method gets the Worker context ID to be used to identify the
 * Worker. Worker IDs enable an application to establish multiple connections
 * to the same Worker process running on a DOCA Device.
 * If when creating the worker the user sets worker id equals to DOCA_UROM_WORKER_ID_ANY the library will generate new
 * id for the worker.
 *
 * @param [in] worker_ctx
 * The UROM Worker context.
 * @param [out] worker_id
 * The UROM Worker ID.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - worker_ctx argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_get_id(struct doca_urom_worker *worker_ctx, uint64_t *worker_id);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * @brief This method gets the Worker context group ID to be used to identify the
 * Worker group that belongs to.
 *
 * @param [in] worker_ctx
 * The UROM Worker context.
 * @param [out] worker_gid
 * The UROM Worker group ID.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - worker_ctx argument is a NULL pointer.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_get_gid(struct doca_urom_worker *worker_ctx, uint32_t *worker_gid);

/**
 * @ingroup DOCA_UROM_WORKER
 *
 * Convert worker_ctx instance into a generalized context for use with DOCA
 * core objects.
 *
 * @param [in] worker_ctx
 * Worker context instance. This must remain valid until after the context is
 * no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_urom_worker_as_ctx(struct doca_urom_worker *worker_ctx);

/**
 * @defgroup DOCA_UROM_WORKER_TASK DOCA UROM Worker Tasks
 * @{
 *  DOCA UROM Worker Task interfaces and data structures.
 *
 *  There is one main type of Worker task called a Command task. A Command task
 *  defines a plugin and a payload. The Command payload is passed to the plugin
 *  running on the associated UROM Worker. Individual plugins define the
 *  payload format and behavior of each Command.  Optionally, a Command task
 *  may return some plugin-defined response data upon completion.
 *
 *  As an example, an RDMA plugin may define a Command to perform an offloaded
 *  RDMA Send. The plugin defines the arguments required to offload the Send to
 *  the Worker. In order to perform this operation, a Command task is created
 *  and formatted to describe the Send. The user then submits the Task to
 *  initiate the operation.
 * @}
 */

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * @brief Allocate Worker Command task.
 *
 * @param [in] worker_ctx
 * The Worker used for the task.
 * @param [out] task
 * The new task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_cmd_task_allocate(struct doca_urom_worker *worker_ctx,
						struct doca_urom_worker_cmd_task **task);

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * @brief Allocate and initialize Worker Command task.
 *
 * @param [in] worker_ctx
 * The Worker used for the task.
 * @param [in] plugin
 * The plugin to use for the task.
 * @param [out] task
 * The new task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_cmd_task_allocate_init(struct doca_urom_worker *worker_ctx,
						     uint64_t plugin,
						     struct doca_urom_worker_cmd_task **task);

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * @brief Release Worker Command task.
 *
 * @param [in] task
 * The task to release
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NOT_PERMITTED - Task resources still in use.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_worker_cmd_task_release(struct doca_urom_worker_cmd_task *task);

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * @brief Set Worker Command task plugin.
 *
 * @param [in] task
 * The task used to set the plugin on.
 * @param [in] plugin
 * The task plugin to set.
 */
DOCA_EXPERIMENTAL
void doca_urom_worker_cmd_task_set_plugin(struct doca_urom_worker_cmd_task *task, uint64_t plugin);

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * @brief Set Worker Command task callback.
 *
 * @param [in] task
 * The task used to set the callback on.
 * @param [in] cb
 * The task callback to set.
 */
DOCA_EXPERIMENTAL
void doca_urom_worker_cmd_task_set_cb(struct doca_urom_worker_cmd_task *task,
				      doca_urom_worker_cmd_task_completion_cb_t cb);

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * @brief Get Worker Command task payload.
 *
 * @param [in] task
 * The task to return a payload from.
 *
 * @return
 * The payload buffer to return
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_urom_worker_cmd_task_get_payload(struct doca_urom_worker_cmd_task *task);

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * @brief Get Worker Command task response.
 *
 * @param [in] task
 * The task to return a response from.
 *
 * @return
 * The response buffer to return
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_urom_worker_cmd_task_get_response(struct doca_urom_worker_cmd_task *task);

/**
 * @ingroup DOCA_UROM_WORKER_TASK
 *
 * @brief Get Worker Command user data to populate
 *
 * @param [in] task
 * The task to return a user data
 *
 * @return
 * The user data buffer (Maximum data size is 32 Bytes)
 */
DOCA_EXPERIMENTAL
void *doca_urom_worker_cmd_task_get_user_data(struct doca_urom_worker_cmd_task *task);

/**
 * @brief This method converts a worker cmd task to doca_task
 *
 * @param [in] task
 * doca_urom_worker_cmd_task task
 *
 * @return doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_urom_worker_cmd_task_as_task(struct doca_urom_worker_cmd_task *task);

/**
 * @defgroup DOCA_UROM_DOMAIN DOCA UROM Domains
 * @{
 *  DOCA UROM Domain interfaces and data structures.
 *
 *  The UROM Domain represents a collection of UROM Workers working in
 *  coordination. Each Worker in the Domain provides a set of memory resources
 *  during creation. When the Domain is initialized, memory descriptors are
 *  exchanged between all Workers. Workers within a Domain may communicate
 *  directly using exchanged these memory descriptors.
 *
 *  Domain initialization is a collective operation. The user is responsible to
 *  provide a mechanism for non-blocking allgather communication among all
 *  processes which participate in the Domain.
 *
 *  A Domain is only required when using Worker plugins which implement a
 *  parallel communication model.
 * @}
 */

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * Opaque structure representing a DOCA UROM Domain context.
 */
struct doca_urom_domain;

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief This method creates a UROM Domain context.
 *
 * A Domain is created in state `DOCA_CTX_STATE_IDLE`. After creation,
 * a user may configure the Domain using setter methods.
 * Before use, a Domain must be transitioned
 * to state `DOCA_CTX_STATE_RUNNING` using the `doca_ctx_start()`
 * interface. A typical invocation looks like:
 *
 * `doca_ctx_start(doca_urom_domain_as_ctx(domain_ctx))`
 *
 * @param [out] domain_ctx
 * The UROM domain.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - domain_ctx argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate domain_ctx.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_domain_create(struct doca_urom_domain **domain_ctx);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @param [in] domain_ctx
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NOT_PERMITTED - Domain ctx state isn't IDLE.
 * - DOCA_ERROR_IN_USE - Unable to gain exclusive access to the domain_ctx instance.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_domain_destroy(struct doca_urom_domain *domain_ctx);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief This method attaches the number of local buffers that will be added to the Domain.
 *
 * @param [in] domain_ctx
 * The UROM Domain context to attach the buffers to.
 * @param [in] buffers_cnt
 * The number of buffers.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - domain_ctx argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - domain_ctx is not idle.
 * - DOCA_ERROR_NO_MEMORY - if buffers array allocation failed.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_domain_set_buffers_count(struct doca_urom_domain *domain_ctx, size_t buffers_cnt);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief This method attaches local buffer attributes to the Domain.
 * Should be called after calling @ref doca_urom_domain_set_buffers_count()
 *
 * @param [in] domain_ctx
 * The UROM Domain context to attach the buffers to.
 * @param [in] buffer
 * buffer ready for remote access which are given to the Domain.
 * @param [in] buf_len
 * The buffer length.
 * @param [in] memh
 * The memory handle for the exported buffer. (should be packed)
 * @param [in] memh_len
 * The memory handle size.
 * @param [in] mkey
 * The memory key for the exported buffer. (should be packed)
 * @param [in] mkey_len
 * The memory key size.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - domain_ctx argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - if buffers count wasn't set.
 * - DOCA_ERROR_FULL - if buffers array is full, reached the buffer count.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_domain_add_buffer(struct doca_urom_domain *domain_ctx,
					 void *buffer,
					 size_t buf_len,
					 void *memh,
					 size_t memh_len,
					 void *mkey,
					 size_t mkey_len);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief Callback for a non-blocking allgather operation.
 */
typedef doca_error_t (
	*doca_urom_domain_allgather_cb_t)(void *sbuf, void *rbuf, size_t msglen, void *coll_info, void **req);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief Callback to test the status of a non-blocking allgather request.
 */
typedef doca_error_t (*doca_urom_domain_req_test_cb_t)(void *req);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief Callback to free a non-blocking allgather request.
 */
typedef doca_error_t (*doca_urom_domain_req_free_cb_t)(void *req);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief Out-of-band communication descriptor for Domain creation.
 */
struct doca_urom_domain_oob_coll {
	/** Non-blocking Allgather callback */
	doca_urom_domain_allgather_cb_t allgather;

	/** Request test callback */
	doca_urom_domain_req_test_cb_t req_test;

	/** Request free callback */
	doca_urom_domain_req_free_cb_t req_free;

	/** Context or meta data required by the OOB collective */
	void *coll_info;

	/** Number of endpoints participating in the oob operation
	 * (e.g., number of client processes representing a domain
	 * workers) */
	uint32_t n_oob_indexes;

	/** Integer value that represents the position of the
	 * calling processes in the given oob op: the data specified
	 * by "src_buf" will be placed at the offset "oob_index*size"
	 * in the "recv_buf".  oob_index must be unique at every
	 * calling process and should be in the range
	 * [0:n_oob_indexes). */
	uint32_t oob_index;
};

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief Set OOB communication info to be used for Domain initialization.
 *
 * @param [in] domain_ctx
 * The UROM Domain context to set OOB info to.
 * @param [in] oob
 * The OOB communication info to set.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - domain_ctx or oob argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - domain_ctx is not idle.
 * - DOCA_ERROR_NO_MEMORY - OOB memory allocation failed.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_domain_set_oob(struct doca_urom_domain *domain_ctx, struct doca_urom_domain_oob_coll *oob);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * @brief Set the list of Workers in the domain.
 *
 * @param [in] domain_ctx
 * The UROM Domain context to attach the resources to.
 * @param [in] domain_worker_ids
 * List of Domain Worker IDs.
 * @param [in] workers
 * An array of UROM Worker contexts which should be part of the domain.
 * @param [in] workers_cnt
 * The number of workers in the given array.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - domain_ctx or workers argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - domain_ctx is not idle.
 * - DOCA_ERROR_NO_MEMORY - workers buffer allocation failed.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_urom_domain_set_workers(struct doca_urom_domain *domain_ctx,
					  uint64_t *domain_worker_ids,
					  struct doca_urom_worker **workers,
					  size_t workers_cnt);

/**
 * @ingroup DOCA_UROM_DOMAIN
 *
 * Convert domain_ctx instance into a generalized context for use with DOCA
 * core objects.
 *
 * @param [in] domain_ctx
 * Domain context instance. This must remain valid until after the context is
 * no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_urom_domain_as_ctx(struct doca_urom_domain *domain_ctx);

#endif /* DOCA_UROM_H_ */
