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
 * @defgroup PCC_NP_DEVICE PCC NP Device
 * DOCA PCC NP Device library. For more details please refer to the user guide on DOCA devzone.
 *
 * @ingroup PCC
 *
 * @{
 */

#ifndef DOCA_PCC_NP_DEV_H_
#define DOCA_PCC_NP_DEV_H_

/**
 * @brief declares that we are compiling for the DPA Device
 *
 * @note Must be defined before the first API use/include of DOCA
 */
#define DOCA_DPA_DEVICE

#include <doca_pcc_dev_common.h>
#include <doca_pcc_dev_utils.h>
#include <doca_pcc_dev_services.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief structure for response packet
 */
struct doca_pcc_np_dev_response_packet {
	size_t size;   /**< size of data buffer */
	uint8_t *data; /**< data buffer for user to fill */
};

/**
 * @brief Opaque structure for input request packet
 * Used in API to get input packet info.
 */
struct doca_pcc_np_dev_request_packet;

/**
 * @brief Main user function (implemented by the user)
 * Called by the lib upon receiving a packet.
 * The user:
 *   - Retrieves the relevant fields using doca_pcc_np_dev_get_raw_packet().
 *   - Prepares the response (currently up to 12 bytes) and sets the response struct.
 * The lib:
 *   - Sends the response packet out.
 *
 * @param[in] in - opaque structure that has request packet buffer, size etc.
 * @param[in] out - buffer preallocated for response packet that will be returned
 *
 * @return
 * DOCA_PCC_NP_DEV_STATUS_OK on success, DOCA_PCC_NP_DEV_STATUS_FAIL on failure.
 */
DOCA_EXPERIMENTAL
doca_pcc_dev_error_t doca_pcc_dev_np_user_packet_handler(struct doca_pcc_np_dev_request_packet *in,
							 struct doca_pcc_np_dev_response_packet *out);

/**
 * @brief Return packet from ethernet header
 *
 * This function is to get packet from ethernet header of input
 *
 * @note that no endianness swap is performed by the function.
 *
 * @param[in] input - a buffer storing the request packet and size of the packet
 *
 * @return
 * pointer pointed to ethernet header of the request packet
 */
DOCA_EXPERIMENTAL
uint8_t *doca_pcc_np_dev_get_raw_packet(const struct doca_pcc_np_dev_request_packet *input);

/**
 * @brief Return packet size from ethernet header
 *
 * This function is to get packet size of input
 *
 * @param[in] input - a buffer storing the request packet and size of the packet
 *
 * @return
 * packet size from ethernet header
 */
DOCA_EXPERIMENTAL
size_t doca_pcc_np_dev_get_raw_packet_size(const struct doca_pcc_np_dev_request_packet *input);

/**
 * @brief Returns a pointer to the L4/udp header of the packet
 *
 * The returned buffer starting with the L4/udp header followed by
 * the packet payload.
 *
 * @note that no endianness swap is performed by the function.
 *
 * @param[in] input - probe packet context
 *
 * @return
 * A pointer to the L4/udp header of the request packet
 */
DOCA_EXPERIMENTAL
uint8_t *doca_pcc_np_dev_get_l4_header(const struct doca_pcc_np_dev_request_packet *input);

/**
 * @brief Return packet size from L4/udp header
 *
 * @param[in] input - probe packet context
 *
 * @return
 * packet size in bytes from the L4/udp header
 */
DOCA_EXPERIMENTAL
size_t doca_pcc_np_dev_get_l4_size(const struct doca_pcc_np_dev_request_packet *input);

/**
 * @brief Returns a pointer to the payload of the packet
 *
 * The returned buffer starting at payload after mad header.
 *
 * @note that no endianness swap is performed by the function.
 *
 * @param[in] input - probe packet context
 *
 * @return
 * A pointer to the payload of the request packet
 */
DOCA_EXPERIMENTAL
uint8_t *doca_pcc_np_dev_get_payload(const struct doca_pcc_np_dev_request_packet *input);

/**
 * @brief Return packet size from payload after mad header
 *
 * @param[in] input - probe packet context
 *
 * @return
 * packet size in bytes from payload
 */
DOCA_EXPERIMENTAL
size_t doca_pcc_np_dev_get_payload_size(const struct doca_pcc_np_dev_request_packet *input);

/**
 * @brief Return data buffer size of out
 *
 * This function is to get data buffer size of out
 *
 * @param[in] out - a buffer storing the part user filled in response packet
 *
 * @return
 * data buffer size of out
 */
DOCA_EXPERIMENTAL
size_t doca_pcc_np_dev_get_max_response_size(struct doca_pcc_np_dev_response_packet *out);

#ifdef __cplusplus
}
#endif

#endif /* DOCA_PCC_NP_DEV_H_ */

/** @} */
