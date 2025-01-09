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
 * @file doca_comch.h
 * @page comm_channel_v2
 * @defgroup DOCA_COMCH comch
 *
 * DOCA Communication Channel library let you set a direct communication channel between the host and the DPU.
 * The channel is run over RoCE/IB protocol and is not part of the TCP/IP stack.
 * Please follow the programmer guide for usage instructions.
 *
 * @{
 */
#ifndef DOCA_COMCH_H_
#define DOCA_COMCH_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_dev;
struct doca_dev_rep;
struct doca_devinfo;

/*********************************************************************************************************************
 * DOCA Comch Connection
 *********************************************************************************************************************/

/* Representantion of a comms channel point to point connection */
struct doca_comch_connection;

/**
 * Set the user data for a given connection.
 *
 * @param [in] connection
 * DOCA Comch connection instance.
 * @param [in] user_data
 * User data for the given connection.
 *
 * @return
 * DOCA_SUCCESS on success.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_connection_set_user_data(struct doca_comch_connection *connection, union doca_data user_data);

/**
 * Get the user data from a given connection.
 *
 * @param [in] connection
 * DOCA Comch connection instance.
 *
 * @return
 * User data for the given connection.
 */
DOCA_EXPERIMENTAL
union doca_data doca_comch_connection_get_user_data(const struct doca_comch_connection *connection);

/**
 * Get the doca_comch_server context from a given connection.
 *
 * @param [in] connection
 * DOCA Comch connection instance.
 *
 * @return
 * doca_comch_server object on success.
 * NULL if the connection is related to a client context.
 */
DOCA_EXPERIMENTAL
struct doca_comch_server *doca_comch_server_get_server_ctx(const struct doca_comch_connection *connection);

/**
 * Get the doca_comch_client context from a given connection.
 *
 * @param [in] connection
 * DOCA Comch connection instance.
 *
 * @return
 * doca_comch_client object on success.
 * NULL if the connection is related to a server context.
 */
DOCA_EXPERIMENTAL
struct doca_comch_client *doca_comch_client_get_client_ctx(const struct doca_comch_connection *connection);

/*********************************************************************************************************************
 * DOCA Comch General Capabilities
 *********************************************************************************************************************/

/**
 * Get the maximum name length that can be used in a cc instance.
 *
 * @param [in] devinfo
 * devinfo to query the capability for.
 * @param [out] max_name_len
 * The cc max name length, including the terminating null byte ('\0').
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_cap_get_max_name_len(const struct doca_devinfo *devinfo, uint32_t *max_name_len);

/**
 * Get the maximum message size that can be used on any comm channel instance.
 *
 * @param [in] devinfo
 * devinfo to query the capability for.
 * @param [out] size
 * The maximum size of a message available on any cc instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_cap_get_max_msg_size(const struct doca_devinfo *devinfo, uint32_t *size);

/**
 * Get the maximal recv queue size that can be used on any comm channel instance.
 *
 * @param [in] devinfo
 * devinfo to query the capability for.
 * @param [out] size
 * The maximal recv queue size supported on any cc instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_cap_get_max_recv_queue_size(const struct doca_devinfo *devinfo, uint32_t *size);

/**
 * Get the maximal number of clients that can be connected to a single doca_comch server.
 *
 * @param [in] devinfo
 * devinfo to query the capability for.
 * @param [out] num_clients
 * The number of clients that can be connected to a single doca_comch server.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_cap_get_max_clients(const struct doca_devinfo *devinfo, uint32_t *num_clients);

/*********************************************************************************************************************
 * DOCA Comch server Context
 *********************************************************************************************************************/

/* DOCA Comch server end point instance */
struct doca_comch_server;

/**
 * Create a DOCA Comch server instance.
 *
 * @param [in] dev
 * Device to use in DOCA Comch server instance.
 * @param [in] repr
 * Representor device to use in CC server instance.
 * @param [in] name
 * Identifier for server associated with instance. Must be NULL terminated.
 * Max length, including terminating '\0', is obtained by doca_comch_cap_get_max_name_len().
 * @param [out] comch_server
 * Pointer to pointer to be set to created doca_comch server instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - one or more of the arguments is null.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_comch.
 * - DOCA_ERROR_INITIALIZATION - failed to initialise a mutex.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_create(struct doca_dev *dev,
				      struct doca_dev_rep *repr,
				      const char *name,
				      struct doca_comch_server **comch_server);

/**
 * Destroy a DOCA Comch server instance.
 *
 * @param [in] comch_server
 * DOCA Comch server instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - cc argument is a NULL pointer.
 * - DOCA_ERROR_IN_USE - Unable to gain exclusive access to the cc instance.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_destroy(struct doca_comch_server *comch_server);

/**
 * Check if given device is capable of running a comm channel server.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device can run as a server.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo can not implement a cc server.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_cap_server_is_supported(const struct doca_devinfo *devinfo);

/**
 * Set the maximum message size property for the doca_comch instance.
 * If not called, a default value will be used and can be queried using doca_comch_server_get_max_msg_size().
 *
 * @param [in] comch_server
 * DOCA Comch server instance.
 * @param [in] size
 * The maximum size of a message to set for the instance. Can be queried with doca_comch_cap_get_max_msg_size().
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_set_max_msg_size(struct doca_comch_server *comch_server, uint32_t size);

/**
 * Get the maximum message size that can be sent on the comm channel instance.
 *
 * @param [in] comch_server
 * DOCA Comch server instance.
 * @param [out] size
 * The maximum size of a message for the instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_get_max_msg_size(const struct doca_comch_server *comch_server, uint32_t *size);

/**
 * Set the recv queue size property for the doca_comch instance.
 * If not called, a default value will be used and can be queried using doca_comch_server_get_recv_queue_size().
 *
 * @param [in] comch_server
 * DOCA Comch server instance.
 * @param [in] size
 * The recv queue size set for the instance. Can be queried with doca_comch_cap_get_max_recv_queue_size().
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_set_recv_queue_size(struct doca_comch_server *comch_server, uint32_t size);

/**
 * Get the recv queue size property set on the doca_comch instance.
 *
 * @param [in] comch_server
 * DOCA Comch server instance.
 * @param [out] size
 * The recv queue size set for the instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_get_recv_queue_size(const struct doca_comch_server *comch_server, uint32_t *size);

/**
 * Get the doca device property of the associated doca_comch instance.
 *
 * @param [in] comch_server
 * DOCA Comch server instance.
 * @param [out] dev
 * Current device used in the doca_comch instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_get_device(const struct doca_comch_server *comch_server, struct doca_dev **dev);

/**
 * Get the device representor property of the associated doca_comch server instance.
 *
 * @param [in] comch_server
 * DOCA Comch server instance.
 * @param [out] repr
 * Current device representor used in the doca_comch server instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_get_device_repr(const struct doca_comch_server *comch_server,
					       struct doca_dev_rep **repr);

/**
 * Convert doca_comch_server instance into a generalised context for use with doca core objects.
 *
 * @param [in] comch_server
 * DOCA Comch server instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_comch_server_as_ctx(struct doca_comch_server *comch_server);

/**
 * Disconnect specific connection on doca_comch_server.
 *
 * This function will notify the peer of the disconnection.
 *
 * @param [in] comch_server
 * DOCA Comch server instance.
 * @param [out] connection
 * Connection representing the remote peer to disconnect.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_IN_USE - if there are active consumers/producers for the connection.
 * - DOCA_ERROR_AGAIN - cannot send disconnection message due to full queue. Requires the app to call disconnect again
 * later.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_disconnect(struct doca_comch_server *comch_server,
					  struct doca_comch_connection *connection);

/*********************************************************************************************************************
 * DOCA Comch Client Context
 *********************************************************************************************************************/

/* DOCA Comch Client end point instance */
struct doca_comch_client;

/**
 * Create a DOCA Comch client instance.
 *
 * @param [in] dev
 * Device to use in DOCA Comch client instance.
 * @param [in] name
 * Identifier for the server the client will connect to.
 * Max length, including terminating '\0', is obtained by doca_comch_cap_get_max_name_len().
 * @param [out] comch_client
 * Pointer to pointer to be set to created doca_comch client instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - one or more of the arguments is null.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_comch.
 * - DOCA_ERROR_INITIALIZATION - failed to initialise a mutex.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_create(struct doca_dev *dev, const char *name, struct doca_comch_client **comch_client);

/**
 * Destroy a DOCA Comch client instance.
 *
 * @param [in] comch_client
 * DOCA Comch client instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - cc argument is a NULL pointer.
 * - DOCA_ERROR_IN_USE - Unable to gain exclusive access to the cc instance.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_destroy(struct doca_comch_client *comch_client);

/**
 * Check if given device is capable of running a comm channel client.
 *
 * @param [in] devinfo
 * The DOCA device information.
 *
 * @return
 * DOCA_SUCCESS - in case device can run as a client.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo can not implement a cc client
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_cap_client_is_supported(const struct doca_devinfo *devinfo);

/**
 * Set the maximum message size property for the doca_comch instance.
 * If not called, a default value will be used and can be queried using doca_comch_client_get_max_msg_size().
 *
 * @param [in] comch_client
 * DOCA Comch client instance.
 * @param [in] size
 * The maximum size of a message to set for the instance. Can be queried with doca_comch_cap_get_max_msg_size().
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_set_max_msg_size(struct doca_comch_client *comch_client, uint32_t size);

/**
 * Get the maximum message size that can be sent on the comm channel instance.
 *
 * @param [in] comch_client
 * DOCA Comch client instance.
 * @param [out] size
 * The maximum size of a message for the instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_get_max_msg_size(const struct doca_comch_client *comch_client, uint32_t *size);

/**
 * Set the recv queue size property for the doca_comch instance.
 * If not called, a default value will be used and can be queried using doca_comch_client_get_recv_queue_size().
 *
 * @param [in] comch_client
 * DOCA Comch client instance.
 * @param [in] size
 * The recv queue size to set for the instance. Limit can be queried with doca_comch_cap_get_max_recv_queue_size().
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_set_recv_queue_size(struct doca_comch_client *comch_client, uint32_t size);

/**
 * Get the recv queue size property set on the doca_comch instance.
 *
 * @param [in] comch_client
 * DOCA Comch client instance.
 * @param [out] size
 * The recv queue size for the instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_get_recv_queue_size(const struct doca_comch_client *comch_client, uint32_t *size);

/**
 * Get the doca device property of the associated doca_comch instance.
 *
 * @param [in] comch_client
 * DOCA Comch client instance.
 * @param [out] dev
 * Current device used in the doca_comch instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_get_device(const struct doca_comch_client *comch_client, struct doca_dev **dev);

/**
 * Convert doca_comch instance into a generalised context for use with doca core objects.
 *
 * @param [in] comch_client
 * DOCA Comch client instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_comch_client_as_ctx(struct doca_comch_client *comch_client);

/**
 * Get the connection object associated with the client ctx. Can only be called after starting the ctx.
 *
 * @param [in] comch_client
 * DOCA Comch client instance.
 * @param [out] connection
 * The connection object associated with the client.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is not started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_get_connection(const struct doca_comch_client *comch_client,
					      struct doca_comch_connection **connection);

/*********************************************************************************************************************
 * DOCA Comch - Send Task
 *********************************************************************************************************************/

/* Task instance to send a message on the control channel */
struct doca_comch_task_send;

/**
 * Function executed on doca_comch send task completion. Used for both task success and failure.
 *
 * @param [in] task
 * DOCA Comch send task that has completed.
 * @param [in] task_user_data
 * The task user data.
 * @param [in] ctx_user_data
 * Doca_comch context user data.
 *
 * The implementation can assume this value is not NULL.
 */
typedef void (*doca_comch_task_send_completion_cb_t)(struct doca_comch_task_send *task,
						     union doca_data task_user_data,
						     union doca_data ctx_user_data);

/**
 * Get the maximal send tasks num that can be used on any cc instance.
 *
 * @param [in] devinfo
 * devinfo to query the capability for.
 * @param [out] max_send_tasks
 * The maximal supported number of send tasks for any cc instance.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_cap_get_max_send_tasks(const struct doca_devinfo *devinfo, uint32_t *max_send_tasks);

/**
 * Configure the doca_comch_server send task callback and parameters.
 *
 * @param [in] comch_server
 * The doca_comch_server instance.
 * @param [in] task_completion_cb
 * Send task completion callback.
 * @param [in] task_error_cb
 * Send task error callback.
 * @param [in] num_send_tasks
 * Number of send tasks to create.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_task_send_set_conf(struct doca_comch_server *comch_server,
						  doca_comch_task_send_completion_cb_t task_completion_cb,
						  doca_comch_task_send_completion_cb_t task_error_cb,
						  uint32_t num_send_tasks);

/**
 * Configure the doca_comch_client send task callback and parameters.
 *
 * @param [in] comch_client
 * The doca_comch_client instance.
 * @param [in] task_completion_cb
 * Send task completion callback.
 * @param [in] task_error_cb
 * Send task error callback.
 * @param [in] num_send_tasks
 * Number of send tasks to create.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - cc instance is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_task_send_set_conf(struct doca_comch_client *comch_client,
						  doca_comch_task_send_completion_cb_t task_completion_cb,
						  doca_comch_task_send_completion_cb_t task_error_cb,
						  uint32_t num_send_tasks);

/**
 * Allocate and initialise a doca_comch_server send task.
 *
 * @param [in] comch_server
 * The doca_comch_server instance.
 * @param [in] peer
 * Connected endpoint to send the message to.
 * @param [in] msg
 * Message or data to sent to associated peer.
 * @param [in] len
 * Length of the message to send.
 * @param [out] task
 * Pointer to a doca_comch_send_task instance populated with input parameters.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NO_MEMORY - no available tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_task_send_alloc_init(struct doca_comch_server *comch_server,
						    struct doca_comch_connection *peer,
						    const void *msg,
						    uint32_t len,
						    struct doca_comch_task_send **task);

/**
 * Allocate and initialise a doca_comch_client send task.
 *
 * @param [in] comch_client
 * The doca_comch_client instance.
 * @param [in] peer
 * Connected endpoint to send the message to.
 * @param [in] msg
 * Message or data to sent to associated peer.
 * @param [in] len
 * Length of the message to send.
 * @param [out] task
 * Pointer to a doca_comch_send_task instance populated with input parameters.
 *
 * @return
 * DOCA_SUCCESS on success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NO_MEMORY - no available tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_task_send_alloc_init(struct doca_comch_client *comch_client,
						    struct doca_comch_connection *peer,
						    const void *msg,
						    uint32_t len,
						    struct doca_comch_task_send **task);

/**
 * Convert a doca_comch_send_task task to doca_task.
 *
 * @param [in] task
 * Doca_comch_send_task task to convert.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_comch_task_send_as_task(struct doca_comch_task_send *task);

/*********************************************************************************************************************
 * DOCA Comch - Receive Message Event Registration
 *********************************************************************************************************************/

/* Async event instance for receiving a message from a connected endpoint */
struct doca_comch_event_msg_recv;

/**
 * Function executed on a doca_comch receive message event.
 *
 * @param [in] event
 * Doca_comch recv message event that has triggered.
 * @param [in] recv_buffer
 * Pointer to the message data associated with the event.
 * @param [in] msg_len
 * Length of the message data associated with the event.
 * @param [in] comch_connection
 * Pointer to the connection instance that generated the message event.
 *
 * The implementation can assume these values are not NULL.
 */
typedef void (*doca_comch_event_msg_recv_cb_t)(struct doca_comch_event_msg_recv *event,
					       uint8_t *recv_buffer,
					       uint32_t msg_len,
					       struct doca_comch_connection *comch_connection);

/**
 * @brief Configure the doca_comch recv event callback for server context.
 *
 * @param [in] comch_server
 * Pointer to doca_comch_server instance.
 * @param [in] recv_event_cb
 * Recv event callback.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 * - DOCA_ERROR_BAD_STATE - doca_comch context state is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_event_msg_recv_register(struct doca_comch_server *comch_server,
						       doca_comch_event_msg_recv_cb_t recv_event_cb);

/**
 * @brief Configure the doca_comch recv event callback for client context.
 *
 * @param [in] comch_client
 * Pointer to doca_comch_client instance.
 * @param [in] recv_event_cb
 * Recv event callback.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 * - DOCA_ERROR_BAD_STATE - doca_comch context state is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_event_msg_recv_register(struct doca_comch_client *comch_client,
						       doca_comch_event_msg_recv_cb_t recv_event_cb);

/*********************************************************************************************************************
 * DOCA Comch - Connection Event Registration
 *********************************************************************************************************************/

/* Async event instance for a connection status change */
struct doca_comch_event_connection_status_changed;

/**
 * Function executed on a doca_comch connection event.
 *
 * @param [in] event
 * Doca_comch connection event that has triggered.
 * @param [in] comch_connection
 * Pointer to the peer which triggered the connection event.
 * @param [in] change_successful
 * 1 if the action (connect/disconnect) was successful, 0 otherwise.
 *
 * The implementation can assume these values are not NULL.
 */
typedef void (*doca_comch_event_connection_status_changed_cb_t)(struct doca_comch_event_connection_status_changed *event,
								struct doca_comch_connection *comch_connection,
								uint8_t change_successful);

/**
 * @brief Configure the doca_comch recv event callback for server context.
 *
 * @param [in] comch_server
 * Pointer to doca_comch_server instance.
 * @param [in] connect_event_cb
 * Callback for connect event.
 * @param [in] disconnect_event_cb
 * Callback for disconnect event.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 * - DOCA_ERROR_BAD_STATE - doca_comch context state is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_event_connection_register(
	struct doca_comch_server *comch_server,
	doca_comch_event_connection_status_changed_cb_t connect_event_cb,
	doca_comch_event_connection_status_changed_cb_t disconnect_event_cb);

/*********************************************************************************************************************
 * DOCA Comch - Connection Statistics
 *********************************************************************************************************************/

/** Available counters for connection statistics query*/
enum doca_comch_counter {
	DOCA_COMCH_COUNTER_SENT_MESSAGES = 1, /* total messages sent over a given comch_connection from local side. */
	DOCA_COMCH_COUNTER_SENT_BYTES = 2,    /* total bytes sent over a given comch_connection from local side. */
	DOCA_COMCH_COUNTER_RECV_MESSAGES = 3, /* total messages received on local side over a given comch_connection. */
	DOCA_COMCH_COUNTER_RECV_BYTES = 4,    /* total bytes received on local side over a given comch_connection. */
};

/**
 * @brief update statistics for given comch_connection
 *
 * Should be used before calling to any connection information function to update the saved statistics.
 *
 * @param [in] comch_connection
 * Pointer to comch_connection to update statistics in.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if comch_connection is NULL.
 * DOCA_ERROR_CONNECTION_INPROGRESS if connection is not yet established.
 * DOCA_ERROR_CONNECTION_ABORTED if the connection failed.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_connection_update_info(struct doca_comch_connection *comch_connection);

/**
 * @brief get statistics counter for a given comch_connection
 *
 * This function will return statistics for a given comch_connection, updated to the last time
 * doca_comch_connection_update_info() was called.
 *
 * @param [in] comch_connection
 * Pointer to comch_connection to query statistics for.
 * @param [in] counter_type
 * Which statistics counter should be queried.
 * @param [out] counter_value
 * Will contain the value for the counter on the given comch_connection.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if any of the arguments are NULL or if the counter is not valid.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_connection_get_counter(const struct doca_comch_connection *comch_connection,
					       enum doca_comch_counter counter_type,
					       uint64_t *counter_value);

/*********************************************************************************************************************
 * DOCA Comch - Consumer Event Registration
 *********************************************************************************************************************/

/* Async event instance for a consumer change*/
struct doca_comch_event_consumer;

/**
 * Function executed on a doca_comch consumer event.
 *
 * @param [in] event
 * Doca_comch consumer event that has triggered.
 * @param [in] comch_connection
 * Pointer to the comch_connection which triggered that has generated the consumer event.
 * @param [in] id
 * The ID of the newly created or destroyed consumer.
 *
 * The implementation can assume these values are not NULL.
 */
typedef void (*doca_comch_event_consumer_cb_t)(struct doca_comch_event_consumer *event,
					       struct doca_comch_connection *comch_connection,
					       uint32_t id);

/**
 * @brief Configure the doca_comch callback for for receiving consumer events on server context.
 *
 * @param [in] comch_server
 * Pointer to doca_comch_server instance.
 * @param [in] new_consumer_event_cb
 * Consumer event callback on creation of a new consumer.
 * @param [in] expired_consumer_event_cb
 * Consumer event callback on when a consumer has expired.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 * - DOCA_ERROR_BAD_STATE - doca_comch_servcie context state is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_server_event_consumer_register(struct doca_comch_server *comch_server,
						       doca_comch_event_consumer_cb_t new_consumer_event_cb,
						       doca_comch_event_consumer_cb_t expired_consumer_event_cb);

/**
 * @brief Configure the doca_comch callback for for receiving consumer events on client context.
 *
 * @param [in] comch_client
 * Pointer to doca_comch_client instance.
 * @param [in] new_consumer_event_cb
 * Consumer event callback on creation of a new consumer.
 * @param [in] expired_consumer_event_cb
 * Consumer event callback on when a consumer has expired.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 * - DOCA_ERROR_BAD_STATE - doca_comch_servcie context state is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_client_event_consumer_register(struct doca_comch_client *comch_client,
						       doca_comch_event_consumer_cb_t new_consumer_event_cb,
						       doca_comch_event_consumer_cb_t expired_consumer_event_cb);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_COMCH_H_ */
