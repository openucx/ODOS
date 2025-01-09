/*
 * Copyright (c) 2021 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_comm_channel.h
 * @page comm_channel
 * @defgroup DOCA_COMM_CHANNEL Comm Channel
 *
 * DOCA Communication Channel library let you set a direct communication channel between the host and the DPU.
 * The channel is run over RoCE/IB protocol and is not part of the TCP/IP stack.
 * Please follow the programmer guide for usage instructions.
 *
 * @{
 */

#ifndef DOCA_COMM_CHANNEL_H_
#define DOCA_COMM_CHANNEL_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_types.h>
#include <doca_error.h>
#include <doca_compat.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * DOCA core opaque types
 *********************************************************************************************************************/

struct doca_devinfo;
struct doca_dev;
struct doca_dev_rep;

/**
 * @brief Flags for send/receive functions
 */
enum doca_comm_channel_msg_flags {
	DOCA_CC_MSG_FLAG_NONE = 0
};

/**
 * @brief Handle for local endpoint
 */
struct doca_comm_channel_ep_t;

/**
 * @brief Handle for peer address
 */
struct doca_comm_channel_addr_t;

/*******************************************************************************
 * DOCA comm_channel Configuration Attributes
 ******************************************************************************/
/**
 * @brief Get the comm_channel maximum Service name length.
 *
 * @param [out] max_service_name_len
 * The comm_channel max service name length, including the terminating null byte ('\0').
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if max_service_name_len is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_max_service_name_len(uint32_t *max_service_name_len);

/**
 * @brief Get the maximum message size supported by comm_channel.
 * @param [in] devinfo
 * devinfo that should be inquired for its maximum message size under comm channel limitations.
 * @param [out] max_message_size
 * the maximum message size supported by comm_channel.
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if either devinfo or max_message_size is NULL.
 * DOCA_ERROR_UNEXPECTED if an unexpected error occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_max_message_size(struct doca_devinfo *devinfo, uint32_t *max_message_size);

/**
 * @brief Get the maximum send queue size supported by comm_channel.
 * @param [in] devinfo
 * devinfo that should be inquired for its maximum send queue size under comm channel limitations.
 * @param [out] max_send_queue_size
 * the maximum send queue size supported by comm_channel.
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if either devinfo or max_send_queue_size is NULL.
 * DOCA_ERROR_UNEXPECTED if an unexpected error occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_max_send_queue_size(struct doca_devinfo *devinfo, uint32_t *max_send_queue_size);

/**
 * @brief Get the maximum receive queue size supported by comm_channel.
 * @param [in] devinfo
 * devinfo that should be inquired for its maximum receive queue size under comm channel limitations.
 * @param [out] max_recv_queue_size
 * the maximum receive queue size supported by comm_channel.
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if either devinfo or max_recv_queue_size is NULL.
 * DOCA_ERROR_UNEXPECTED if an unexpected error occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_max_recv_queue_size(struct doca_devinfo *devinfo, uint32_t *max_recv_queue_size);

/**
 * @brief Get the maximum number of connections the service can hold.
 * @note This capability should be queried only on the service side.
 * @param [in] devinfo
 * devinfo that should be inquired for its maximum number of connections.
 * @param [out] max_num_connections
 * the maximum number of connections the service can hold.
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if either devinfo or max_num_connections is NULL.
 * DOCA_ERROR_NOT_SUPPORTED if querying this capability is not supported by the device.
 * DOCA_ERROR_UNEXPECTED if an unexpected error occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_service_max_num_connections(struct doca_devinfo *devinfo,
							       uint32_t *max_num_connections);

/**
 * @brief Create local endpoint
 * The endpoint handle represents all the configuration needed for the channel to run.
 * The user needs to hold one endpoint for all actions with the comm channel on his side
 * @param [out] ep
 * handle to the newly created endpoint.
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if no ep pointer or no attribute object was given.
 * DOCA_ERROR_NO_MEMORY if memory allocation failed during ep creation.
 * DOCA_ERROR_INITIALIZATION if initialization of ep failed.
 * DOCA_ERROR_DRIVER if acquiring device attributes failed.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_create(struct doca_comm_channel_ep_t **ep);

/**
 * @brief get device property of endpoint.
 * @param [in] ep
 * endpoint from which the property should be retrieved.
 * @param [out] device
 * current device used in endpoint.
 * @return
 * DOCA_SUCCESS if property was returned successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_device(struct doca_comm_channel_ep_t *ep, struct doca_dev **device);

/**
 * @brief get device representor property of endpoint.
 * @param [in] ep
 * endpoint from which the property should be retrieved.
 * @param [out] device_rep
 * current device representor used in endpoint.
 * @return
 * DOCA_SUCCESS if property returned successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_device_rep(struct doca_comm_channel_ep_t *ep, struct doca_dev_rep **device_rep);

/**
 * @brief get maximal msg size property of endpoint.
 * The size returned is the actual size being used and might differ from the size set with
 * doca_comm_channel_ep_set_max_msg_size(), as there is a minimal size requirement.
 * If maximal msg size was not set, using doca_comm_channel_ep_set_max_msg_size(), a default value is used and can be
 * inquired by calling doca_comm_channel_ep_get_max_msg_size().
 * @param [in] ep
 * endpoint from which the property should be retrieved.
 * @param [out] max_msg_size
 * maximal msg size used by the endpoint.
 * @return
 * DOCA_SUCCESS if property was returned successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_max_msg_size(struct doca_comm_channel_ep_t *ep, uint16_t *max_msg_size);

/**
 * @brief get send queue size property of endpoint.
 * The size returned is the actual size being used and might differ from the size set with
 * doca_comm_channel_ep_set_send_queue_size(), as there is a minimal size requirement and the size is rounded up to the
 * closest power of 2.
 * If send queue size was not set, using doca_comm_channel_ep_set_send_queue_size(), a default value is used and can be
 * inquired by calling doca_comm_channel_ep_get_send_queue_size().
 * @param [in] ep
 * endpoint from which the property should be retrieved.
 * @param [out] send_queue_size
 * send queue size used by the endpoint.
 * @return
 * DOCA_SUCCESS if property was returned successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_send_queue_size(struct doca_comm_channel_ep_t *ep, uint16_t *send_queue_size);

/**
 * @brief get receive queue size property of endpoint.
 * The size returned is the actual size being used and might differ from the size set with
 * doca_comm_channel_ep_set_recv_queue_size(), as there is a minimal size requirement and the size is rounded up to the
 * closest power of 2.
 * If receive queue size was not set, using doca_comm_channel_ep_set_recv_queue_size(), a default value is used and can
 * be inquired by calling doca_comm_channel_ep_get_recv_queue_size().
 * @param [in] ep
 * endpoint from which the property should be retrieved.
 * @param [out] recv_queue_size
 * receive queue size used by the endpoint.
 * @return
 * DOCA_SUCCESS if property was returned successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_recv_queue_size(struct doca_comm_channel_ep_t *ep, uint16_t *recv_queue_size);

/**
 * @brief set device property for endpoint.
 * @param [in] ep
 * endpoint to set the property for.
 * @param [in] device
 * device to use in endpoint.
 * @return
 * DOCA_SUCCESS if property set successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 * DOCA_ERROR_BAD_STATE if endpoint is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_set_device(struct doca_comm_channel_ep_t *ep, struct doca_dev *device);

/**
 * @brief set device representor property for endpoint.
 * @param [in] ep
 * endpoint to set the property for.
 * @param [in] device_rep
 * device representor to use in endpoint.
 * @return
 * DOCA_SUCCESS if property set successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 * DOCA_ERROR_BAD_STATE if endpoint is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_set_device_rep(struct doca_comm_channel_ep_t *ep, struct doca_dev_rep *device_rep);

/**
 * @brief set maximal msg size property for endpoint.
 * The value max_msg_size may be increased internally, the actual value can be queried using
 * doca_comm_channel_ep_get_max_msg_size().
 * @param [in] ep
 * endpoint to set the property for.
 * @param [in] max_msg_size
 * maximal msg size to use in endpoint.
 * @return
 * DOCA_SUCCESS if property set successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 * DOCA_ERROR_BAD_STATE if endpoint is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_set_max_msg_size(struct doca_comm_channel_ep_t *ep, uint16_t max_msg_size);

/**
 * @brief set send queue size property for endpoint.
 * The value send_queue_size may be increased internally, the actual value can be queried using
 * doca_comm_channel_ep_get_send_queue_size().
 * @param [in] ep
 * endpoint to set the property for.
 * @param [in] send_queue_size
 * send queue size to use in endpoint.
 * @return
 * DOCA_SUCCESS if property set successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 * DOCA_ERROR_BAD_STATE if endpoint is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_set_send_queue_size(struct doca_comm_channel_ep_t *ep, uint16_t send_queue_size);

/**
 * @brief set receive queue size property for endpoint.
 * The value recv_queue_size may be increased internally, the actual value can be queried using
 * doca_comm_channel_ep_get_recv_queue_size().
 * @param [in] ep
 * endpoint to set the property for.
 * @param [in] recv_queue_size
 * receive queue size to use in endpoint.
 * @return
 * DOCA_SUCCESS if property set successfully.
 * DOCA_ERROR_INVALID_VALUE if an invalid parameter was given.
 * DOCA_ERROR_BAD_STATE if endpoint is already active.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_set_recv_queue_size(struct doca_comm_channel_ep_t *ep, uint16_t recv_queue_size);

/**
 * @brief endpoint notification file descriptor for blocking with epoll() for recv ready event
 */
typedef doca_event_handle_t doca_event_channel_t;

/**
 * @brief Extract the event_channel handles for user's use
 * When the user send/receive packets with non-blocking mode, this handle can be used to get interrupt when a new
 * event happened, using epoll() or similar function.
 * The event channels are owned by the endpoint and release when calling @ref doca_comm_channel_ep_destroy().
 * This function can be called only after calling doca_comm_channel_ep_listen() or doca_comm_channel_ep_connect().
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @param [out] send_event_channel
 * handle for send event channel.
 * @param [out] recv_event_channel
 * handle for receive event channel.
 * @return
 * DOCA_SUCCESS on success
 * DOCA_ERROR_INVALID_VALUE if no ep was provided or if both event channel output params are null.
 * DOCA_ERROR_BAD_STATE if called before calling doca_comm_channel_ep_listen() or doca_comm_channel_ep_connect().
 * DOCA_ERROR_NOT_FOUND if another error occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_event_channel(struct doca_comm_channel_ep_t *local_ep,
						    doca_event_channel_t *send_event_channel,
						    doca_event_channel_t *recv_event_channel);

/**
 * @brief Extract the service_event_channel handle for user's use
 * This handle can be used to get interrupt when one of the following events occurred: new client connected, client
 * disconnected or service moved to error state using epoll() or similar function. If an event was received, the
 * application can call @ref doca_comm_channel_ep_update_service_state_info() and it's query functions to get the
 * current service state.
 * The service event channel is armed automatically upon calling
 * doca_comm_channel_ep_update_service_state_info().
 *
 * The event channels are owned by the endpoint and release when calling @ref doca_comm_channel_ep_destroy().
 * This function can be called only after calling doca_comm_channel_ep_listen().
 *
 * This function available only for service side use.
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @param [out] service_event_channel
 * handle for service event channel.
 * @return
 * DOCA_SUCCESS on success
 * DOCA_ERROR_INVALID_VALUE if no ep was provided or if service_event_channel is NULL.
 * DOCA_ERROR_BAD_STATE if called before calling doca_comm_channel_ep_listen().
 * DOCA_ERROR_NOT_SUPPORTED if called on a non-service instant.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_service_event_channel(struct doca_comm_channel_ep_t *local_ep,
							    doca_event_channel_t *service_event_channel);

/**
 * @brief Arm the event_channel handle for transmitted messages.
 * This function arms the transmit completion queue, facilitating blocking on the transmit event channel.
 * Blocking should be implemented by the user (poll in Linux, GetQueuedCompletionStatus in Windows).
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @return
 * DOCA_SUCCESS on success
 * DOCA_ERROR_INVALID_VALUE if no ep object was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_event_handle_arm_send(struct doca_comm_channel_ep_t *local_ep);

/**
 * @brief Arm the event_channel handle for received messages.
 * This function arms the receive completion queue, facilitating blocking on the receive event channel.
 * Blocking should be implemented by the user (poll in Linux, GetQueuedCompletionStatus in Windows).
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @return
 * DOCA_SUCCESS on success
 * DOCA_ERROR_INVALID_VALUE if no ep object was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_event_handle_arm_recv(struct doca_comm_channel_ep_t *local_ep);

/**
 * @brief Service side listen on all interfaces.
 *
 * Endpoint will start listening on given devices.
 * After calling this function the user should call doca_comm_channel_ep_recvfrom() in order to get
 * new peers to communicate with.
 *
 * This function available only for service side use.
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @param [in] name
 * identifies the service. Use doca_comm_channel_get_max_service_name_len() to get the maximal service name length.
 * @return
 * DOCA_SUCCESS on success
 * DOCA_ERROR_INVALID_VALUE if no ep object or no name was given.
 * DOCA_ERROR_DRIVER if failed to query the capabilities of the device that was set for the ep.
 * DOCA_ERROR_NOT_SUPPORTED if tried to call listen on a device that doesn't have the capability to be defined as the
 *			    service side for Comm Channel.
 * DOCA_ERROR_BAD_STATE if no doca_dev or no doca_dev_rep was set.
 * DOCA_ERROR_NOT_PERMITTED if the endpoint is already listening.
 * DOCA_ERROR_NO_MEMORY if memory allocation failed.
 * DOCA_ERROR_INITIALIZATION if initialization of service failed.
 * DOCA_ERROR_CONNECTION_ABORTED if registration of service failed.
 * DOCA_ERROR_DRIVER if acquiring device attributes failed.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_listen(struct doca_comm_channel_ep_t *local_ep, const char *name);

/**
 * @brief Client side Connect
 *
 * This function available only for client-side use.
 * As part of the connection process, the client initiates an internal handshake protocol with the service.
 *
 * If the connect function is being called before the service perform listen with the same name
 * the connection will fail.
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @param [in] name
 * identifies the service. Use doca_comm_channel_get_max_service_name_len() to get the maximal service name length.
 * @param [out] peer_addr
 * handle to use for sending packets and recognize source of messages.
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if no ep object, name or peer_address pointer given.
 * DOCA_ERROR_DRIVER if failed to query the capabilities of the device that was set for the ep or acquire device
 *		     attributes.
 * DOCA_ERROR_NOT_SUPPORTED if tried to call connect on a device that doesn't have the capability to connect to Comm
 *			    Channel.
 * DOCA_ERROR_NOT_PERMITTED if the endpoint is already connected.
 * DOCA_ERROR_BAD_STATE if no doca_dev was set.
 * DOCA_ERROR_NO_MEMORY if memory allocation failed.
 * DOCA_ERROR_INITIALIZATION if initialization of ep connection failed.
 * DOCA_ERROR_CONNECTION_ABORTED if connection failed for any reason (connections rejected or failed).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_connect(struct doca_comm_channel_ep_t *local_ep,
					  const char *name,
					  struct doca_comm_channel_addr_t **peer_addr);

/**
 * @brief Send message to peer address.
 * The connection to the wanted peer_address need to be established before sending the message.
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @param [in] msg
 * pointer to the message to be sent.
 * @param [in] len
 * length in bytes of msg.
 * @param [in] flags
 * flag for send command. currently no flags are supported.
 * @param [in] peer_addr
 * destination address handle of the send operation.
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_NOT_CONNECTED if no peer_address was supplied or no connection was found.
 * DOCA_ERROR_INVALID_VALUE if the supplied len was larger than the msgsize given at ep creation or any of the input
 *                          variables are null.
 * DOCA_ERROR_AGAIN if the send queue is full.
 *		    when returned, the user can use the endpoint's doca_event_channel_t to get indication for a new
 *		    empty slot.
 * DOCA_ERROR_CONNECTION_RESET if the provided peer_addr experienced an error and it needs to be disconnected.
 * DOCA_ERROR_INITIALIZATION if initialization of the DCI after a send error failed
 * DOCA_ERROR_UNKNOWN if an unknown error occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_sendto(struct doca_comm_channel_ep_t *local_ep,
					 const void *msg,
					 size_t len,
					 int flags,
					 struct doca_comm_channel_addr_t *peer_addr);

/**
 * @brief Receive message from connected client/service.
 *
 * On service side, doca_comm_channel_ep_recvfrom() also used for accepting new connection from clients.
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @param [in] msg
 * pointer to the buffer where the message should be stored.
 * @param [in,out] len
 * input - maximum len of bytes in the msg buffer, output - len of actual received message.
 * @param [in] flags
 * flag for receive command. currently no flags are supported.
 * @param [out] peer_addr
 * received message source address handle
 * @return
 * DOCA_SUCCESS on successful receive. If a message was received, the value pointed by len will be updated with
 * the number of bytes received.
 * DOCA_ERROR_INVALID_VALUE if any of the parameters is NULL.
 * DOCA_ERROR_NOT_CONNECTED if endpoint is service and listen was not called.
 * DOCA_ERROR_AGAIN if no message was received.
 *		    when returned, the user can use the endpoint's doca_event_channel_t to get indication for a new
 *		    arrival message.
 * DOCA_ERROR_CONNECTION_RESET if the message received is from a peer_addr that has error.
 * DOCA_ERROR_INITIALIZATION if initialization of the DCI after a send error failed
 * DOCA_ERROR_UNKNOWN if an unknown error occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_recvfrom(struct doca_comm_channel_ep_t *local_ep,
					   void *msg,
					   size_t *len,
					   int flags,
					   struct doca_comm_channel_addr_t **peer_addr);

/**
 * @brief Disconnect the endpoint from the remote peer.
 * block until all resources related to peer address are freed
 * new connection could be created on the endpoint
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @param [in] peer_addr
 * peer address to be disconnect from.
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if no ep was provided.
 * DOCA_ERROR_NO_MEMORY if a memory related error has occurred.
 * DOCA_ERROR_NOT_CONNECTED if there is no connection.
 * DOCA_ERROR_UNKNOWN if an unknown error occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_disconnect(struct doca_comm_channel_ep_t *local_ep,
					     struct doca_comm_channel_addr_t *peer_addr);

/**
 * @brief Release endpoint handle.
 *
 * The function close the event_channel and release all internal resources.
 * The doca_comm_channel_ep_disconnect() is included as part of the destroy process.
 *
 * @param [in] local_ep
 * handle for the endpoint created beforehand with doca_comm_channel_ep_create().
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_NOT_CONNECTED if ep does not exist.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_destroy(struct doca_comm_channel_ep_t *local_ep);

/**
 * @brief Extract 'user_context' from peer_addr handle.
 * By default, the 'user_context' is set to 0 and can be change using @ref doca_comm_channel_peer_addr_set_user_data()
 *
 * @param [in] peer_addr
 * Pointer to peer_addr to extract user_context from.
 * @param [out] user_data
 * will contain the extracted data.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if peer_address or user_data is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_get_user_data(struct doca_comm_channel_addr_t *peer_addr, uint64_t *user_data);

/**
 * @brief Save 'user_context' in peer_addr handle
 *
 * Can be use by the user to identify the peer address received from doca_comm_channel_ep_recvfrom().
 * The user_context for new peers is initialized to 0.
 *
 * @param [in] peer_addr
 * Pointer to peer_addr to set user_context to.
 * @param [in] user_context
 * Data to set for peer_addr.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if peer_address is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_set_user_data(struct doca_comm_channel_addr_t *peer_addr,
						       uint64_t user_context);

/**
 * @brief update statistics for given peer_addr
 *
 * Should be used before calling to any peer_addr information function to update the saved statistics.
 * This function can also be used to check if connection to a given peer_addr is currently connected.
 * If a connection has failed, it is the user's responsibility to call doca_comm_channel_ep_disconnect() to free the
 * peer_addr resources.
 *
 * @param [in] peer_addr
 * Pointer to peer_addr to update statistics in.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if peer_addr is NULL.
 * DOCA_ERROR_CONNECTION_INPROGRESS if connection is not yet established.
 * DOCA_ERROR_CONNECTION_ABORTED if the connection failed.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_update_info(struct doca_comm_channel_addr_t *peer_addr);

/**
 * @brief get total messages sent to specific peer address
 *
 * This function will return the total number of messages sent to a given peer_addr, updated to the last time
 * doca_comm_channel_peer_addr_update_info() was called.
 *
 * @param [in] peer_addr
 * Pointer to peer_addr to query statistics for.
 * @param [out] send_messages
 * Will contain the number of sent messages to the given peer_addr.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if any of the arguments are NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_get_send_messages(const struct doca_comm_channel_addr_t *peer_addr,
							   uint64_t *send_messages);

/**
 * @brief get total bytes sent to specific peer address
 *
 * This function will return the total number of bytes sent to a given peer_addr, updated to the last time
 * doca_comm_channel_peer_addr_update_info() was called.
 *
 * @param [in] peer_addr
 * Pointer to peer_addr to query statistics for.
 * @param [out] send_bytes
 * Will contain the number of sent messages to the given peer_addr.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if any of the arguments are NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_get_send_bytes(const struct doca_comm_channel_addr_t *peer_addr,
							uint64_t *send_bytes);

/**
 * @brief get total messages received from specific peer address
 *
 * This function will return the total number of messages received from a given peer_addr, updated to the last time
 * doca_comm_channel_peer_addr_update_info() was called.
 *
 * @param [in] peer_addr
 * Pointer to peer_addr to query statistics for.
 * @param [out] recv_messages
 * Will contain the number of received messages from the given peer_addr.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if any of the arguments are NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_get_recv_messages(const struct doca_comm_channel_addr_t *peer_addr,
							   uint64_t *recv_messages);

/**
 * @brief get total bytes received from specific peer address
 *
 * This function will return the total number of bytes received from a given peer_addr, updated to the last time
 * doca_comm_channel_peer_addr_update_info() was called.
 *
 * @param [in] peer_addr
 * Pointer to peer_addr to query statistics for.
 * @param [out] recv_bytes
 * Will contain the number of received bytes from the given peer_addr.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if any of the arguments are NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_get_recv_bytes(const struct doca_comm_channel_addr_t *peer_addr,
							uint64_t *recv_bytes);

/**
 * @brief get number of messages in transmission to a specific peer address
 *
 * This function will return the number of messages still in transmission to a specific peer_addr, updated to the last
 * time doca_comm_channel_peer_addr_update_info() was called. This function can be used to make sure all transmissions
 * are finished before disconnection.
 *
 * @param [in] peer_addr
 * Pointer to peer_addr to query statistics for.
 * @param [out] send_in_flight_messages
 * Will contain the number of sent messages in transmission to the given peer_addr.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if any of the arguments are NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_get_send_in_flight_messages(const struct doca_comm_channel_addr_t *peer_addr,
								     uint64_t *send_in_flight_messages);

/**
 * @brief update the connections status for a given service endpoint
 *
 * Can only be called on the service side.
 * This function saves a snapshot of the current service state, which can be queried using the functions
 * doca_comm_channel_ep_get_peer_addr_list() or doca_comm_channel_ep_get_pending_connections().
 * This function can also be used to check if service is in error state, in that case it cannot be recovered and needs
 * to be destroyed.
 *
 * @note Calling this function will also invalidate any peer_addr_array received from previous calls to
 * doca_comm_channel_ep_get_peer_addr_list().
 *
 * @param [in] local_ep
 * Pointer to endpoint to update the connections status on.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if local_ep is NULL.
 * DOCA_ERROR_NOT_SUPPORTED if called on a client endpoint.
 * DOCA_ERROR_DRIVER if failed to query the service status.
 * DOCA_ERROR_AGAIN if an unexpected number of new clients joined and service status needs to be queried again.
 * DOCA_ERROR_CONNECTION_RESET if the the service is in error state.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_update_service_state_info(struct doca_comm_channel_ep_t *local_ep);

/**
 * @brief get an array of the peer_addr connected to a given service object
 *
 * This function can only be called on the service side.
 * This function will return an array of all peer_addr connected to the given service endpoint, based on the information
 * that was updated at the last time doca_comm_channel_ep_update_service_state_info() was called.
 *
 * @note When calling doca_comm_channel_ep_update_service_state_info() any previously received peer_addr_array is
 * invalidated.
 *
 * @param [in] local_ep
 * Pointer to service endpoint to get peer_addr array for.
 * @param [out] peer_addr_array
 * An array of connected peer_addr objects.
 * @param [out] peer_addr_array_len
 * The number of entries in the output peer_addr_array.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if any of the arguments are NULL.
 * DOCA_ERROR_NOT_SUPPORTED if called on a client endpoint.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_peer_addr_list(const struct doca_comm_channel_ep_t *local_ep,
						     struct doca_comm_channel_addr_t ***peer_addr_array,
						     uint32_t *peer_addr_array_len);

/**
 * @brief get the number of pending connections for a given service endpoint
 *
 * This function can only be called on the service side.
 * This function will return the number of pending connections for the given service endpoint, based on the information
 * that was updated at the last time doca_comm_channel_ep_update_service_state_info() was called.
 * Pending connections are connections that are waiting for handshake to be completed. doca_comm_channel_ep_recvfrom()
 * should be called to handle pending connections.
 *
 * @param [in] local_ep
 * Pointer to peer_addr to get pending connections for.
 * @param [out] pending_connections
 * The number of pending connections for the given service endpoint.
 *
 * @return
 * DOCA_SUCCESS on success.
 * DOCA_ERROR_INVALID_VALUE if any of the arguments are NULL.
 * DOCA_ERROR_NOT_SUPPORTED if called on a client endpoint.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_pending_connections(const struct doca_comm_channel_ep_t *local_ep,
							  uint32_t *pending_connections);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_COMM_CHANNEL_H_ */
