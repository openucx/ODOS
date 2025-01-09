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
 * @file doca_eth_txq_gpu_data_path.h
 * @page DOCA ETH TXQ
 * @defgroup DOCAETHTXQ DOCA ETH TXQ GPU DATA PATH
 * DOCA ETH TXQ library.
 *
 * @{
 */
#ifndef DOCA_ETH_TXQ_GPU_DATA_PATH_H_
#define DOCA_ETH_TXQ_GPU_DATA_PATH_H_

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opaque structure representing a DOCA ETH TXQ instance.
 */
struct doca_eth_txq;

/**
 * Opaque structures representing DOCA ETH TXQ events.
 */

struct doca_eth_txq_gpu_event_error_send_packet;  /**< DOCA ETH TXQ event for send packet errors. */
struct doca_eth_txq_gpu_event_notify_send_packet; /**< DOCA ETH TXQ event for send packet notify info. */

/**
 * @brief Function to be executed on send packet error event occurrence.
 *
 * @param [in] event_error
 * The send packet error event.
 * The implementation assumes this value is not NULL.
 * @param [in] event_user_data
 * user_data attached to the event.
 */
typedef void (*doca_eth_txq_gpu_event_error_send_packet_cb_t)(
	struct doca_eth_txq_gpu_event_error_send_packet *event_error,
	union doca_data event_user_data);

/**
 * @brief Function to be executed on send packet notify event occurrence.
 *
 * @param [in] event_notify
 * The send packet notify event.
 * The implementation assumes this value is not NULL.
 * @param [in] event_user_data
 * user_data attached to the event.
 */
typedef void (*doca_eth_txq_gpu_event_notify_send_packet_cb_t)(
	struct doca_eth_txq_gpu_event_notify_send_packet *event_notify,
	union doca_data event_user_data);

/**
 * @brief This method registers a doca_eth_txq_gpu_event_error_send_packet event.
 * can only be called before calling doca_ctx_start().
 *
 * @note Supported for DOCA ETH TXQ instance for GPU only.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] event_error_send_packet_cb
 * Method that is invoked once a send packet error event is triggered
 * @param [in] event_user_data
 * user_data attached to the event.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 * - DOCA_ERROR_BAD_STATE - internal error.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_gpu_event_error_send_packet_register(
	struct doca_eth_txq *eth_txq,
	doca_eth_txq_gpu_event_error_send_packet_cb_t event_error_send_packet_cb,
	union doca_data event_user_data);

/**
 * @brief This method registers a doca_eth_txq_gpu_event_notify_send_packet event.
 * can only be called before calling doca_ctx_start().
 *
 * @note Supported for DOCA ETH TXQ instance for GPU only.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] event_notify_send_packet_cb
 * Method that is invoked once a send packet notify event is triggered
 * @param [in] event_user_data
 * user_data attached to the event.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 * - DOCA_ERROR_BAD_STATE - internal error.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_gpu_event_notify_send_packet_register(
	struct doca_eth_txq *eth_txq,
	doca_eth_txq_gpu_event_notify_send_packet_cb_t event_notify_send_packet_cb,
	union doca_data event_user_data);

/**
 * @brief This method returns the index in the send queue of the packet
 * which caused the error.
 *
 * @note Supported for DOCA ETH TXQ instance for GPU only.
 *
 * @param [in] event_error
 * The send packet error event.
 * The implementation assumes this value is not NULL.
 * @param [out] packet_index
 * Send packet index in the send queue.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_gpu_event_error_send_packet_get_position(
	const struct doca_eth_txq_gpu_event_error_send_packet *event_error,
	uint16_t *packet_index);

/**
 * @brief This method returns the index in the send queue of the packet
 * which reported the notify info.
 *
 * @note Supported for DOCA ETH TXQ instance for GPU only.
 *
 * @param [in] event_notify
 * The send packet notify event.
 * The implementation assumes this value is not NULL.
 * @param [out] packet_index
 * Send packet index in the send queue.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_gpu_event_notify_send_packet_get_position(
	const struct doca_eth_txq_gpu_event_notify_send_packet *event_notify,
	uint16_t *packet_index);

/**
 * @brief This method returns the timestamp at which the packet in the send queue
 * was actually sent (fired over the network).
 *
 * @note Supported for DOCA ETH TXQ instance for GPU only.
 *
 * @param [in] event_notify
 * The send packet notify event.
 * The implementation assumes this value is not NULL.
 * @param [out] packet_timestamp
 * Send packet event timestamp in the send queue.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case one of the arguments is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_gpu_event_notify_send_packet_get_timestamp(
	const struct doca_eth_txq_gpu_event_notify_send_packet *event_notify,
	uint64_t *packet_timestamp);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_ETH_TXQ_GPU_DATA_PATH_H_ */

/** @} */
