/*
 * Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_dpa_dev_comch_msgq.h
 * @page doca dpa comch msgq
 * @defgroup DPA_COMCH_MSQ DOCA DPA comch msgq
 * @ingroup DPA_DEVICE
 * DOCA DPA comch msgq
 * @{
 */

#ifndef DOCA_DPA_DEV_COMCH_MSGQ_H_
#define DOCA_DPA_DEV_COMCH_MSGQ_H_

#include <doca_dpa_dev.h>
#include <doca_dpa_dev_buf.h>
#include <doca_dpa_dev_sync_event.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DPA Comch consumer handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_comch_consumer_t;

/**
 * @brief DPA Comch consumer completion handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_comch_consumer_completion_t;

/**
 * @brief DPA Comch consumer completion element handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_comch_consumer_completion_element_t;

/**
 * @brief DPA Comch producer handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_comch_producer_t;

/**
 * @brief Post multiple receive operations that receive immediate data only
 *
 * @param[in] consumer - Comch consumer DPA handle
 * @param[in] num_msg - number of immediate messages to receive
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_comch_consumer_ack(doca_dpa_dev_comch_consumer_t consumer, uint32_t num_msg);

/**
 * @brief Get Comch consumer completion element
 *
 * @param[in] consumer_comp_handle - DPA Comch consumer completion handle
 * @param[out] comp_element - DPA Comch consumer completion element
 *
 * @return
 * This function returns 1 if there is completion element to return. Otherwise returns 0
 */
DOCA_EXPERIMENTAL
int doca_dpa_dev_comch_consumer_get_completion(doca_dpa_dev_comch_consumer_completion_t consumer_comp_handle,
					       doca_dpa_dev_comch_consumer_completion_element_t *comp_element);

/**
 * @brief Get Comch consumer completion immediate data
 *
 * @param[in] comp_element - DPA Comch consumer completion element
 * @param[out] imm_length - The length of the immediate data in bytes
 *
 * @return
 * This function returns pointer to immediate data that producer sent
 */
DOCA_EXPERIMENTAL
const uint8_t *doca_dpa_dev_comch_consumer_get_completion_imm(
	doca_dpa_dev_comch_consumer_completion_element_t comp_element,
	uint32_t *imm_length);

/**
 * @brief Get Comch consumer completion producer ID
 *
 * @param[in] comp_element - DPA Comch consumer completion element
 *
 * @return
 * This function returns producer ID of producer that sent the data
 */
DOCA_EXPERIMENTAL
uint32_t doca_dpa_dev_comch_consumer_get_completion_producer_id(
	doca_dpa_dev_comch_consumer_completion_element_t comp_element);

/**
 * @brief Acknowledge that the completions have been read on the Comch consumer completion context
 *
 * @param[in] consumer_comp_handle - DPA Comch consumer completion handle
 * @param[in] num_comp - Number of completion elements which have been read
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_comch_consumer_completion_ack(doca_dpa_dev_comch_consumer_completion_t consumer_comp_handle,
						uint64_t num_comp);

/**
 * @brief Request notification on the Comch consumer completion context
 *
 * @param[in] consumer_comp_handle - DPA Comch consumer completion handle
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_comch_consumer_completion_request_notification(
	doca_dpa_dev_comch_consumer_completion_t consumer_comp_handle);

/**
 * @brief Check if consumer has posted any receive requests
 *
 * @param[in] producer - DPA Comch producer handle
 * @param[in] consumer_id - The ID of the consumer to check
 *
 * @return
 * This function returns 1 if consumer does not have any receive requests. Otherwise returns 0
 */
DOCA_EXPERIMENTAL
int doca_dpa_dev_comch_producer_is_consumer_empty(doca_dpa_dev_comch_producer_t producer, uint32_t consumer_id);

/**
 * @brief Post a producer send immediate data only operation
 *
 * @param[in] producer - Comch producer DPA handle
 * @param[in] consumer_id - The ID of the consumer to send to
 * @param[in] imm - The immediate data to send
 * @param[in] imm_length - The length of the immediate data in bytes
 * @param[in] completion_requested - to raise a completion when copy data operation is done (0 or 1)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_comch_producer_post_send_imm_only(doca_dpa_dev_comch_producer_t producer,
						    uint32_t consumer_id,
						    const uint8_t *imm,
						    uint32_t imm_length,
						    uint32_t completion_requested);

/**
 * @brief Post a DMA copy operation, and send immediate data after completion
 *
 * @param[in] producer - Comch producer DPA handle
 * @param[in] consumer_id - The ID of the consumer to send to
 * @param[in] dst_mmap - The DOCA mmap DPA handle of the destination buffer
 * @param[in] dst_addr - address of destination buffer
 * @param[in] src_mmap - The DOCA mmap DPA handle of the source buffer
 * @param[in] src_addr - address of source buffer
 * @param[in] length - length of the copy operation
 * @param[in] imm - The immediate data to send
 * @param[in] imm_length - The length of the immediate data in bytes
 * @param[in] completion_requested - to raise a completion when copy data operation is done (0 or 1)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_comch_producer_dma_copy(doca_dpa_dev_comch_producer_t producer,
					  uint32_t consumer_id,
					  doca_dpa_dev_mmap_t dst_mmap,
					  uint64_t dst_addr,
					  doca_dpa_dev_mmap_t src_mmap,
					  uint64_t src_addr,
					  size_t length,
					  const uint8_t *imm,
					  uint32_t imm_length,
					  uint32_t completion_requested);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DPA_DEV_COMCH_MSGQ_H_ */
