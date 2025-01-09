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
 * @file doca_dpa_dev_rdma.h
 * @page doca dpa rdma
 * @defgroup DPA_RDMA DOCA DPA rdma
 * @ingroup DPA_DEVICE
 * DOCA DPA rdma
 * @{
 */

#ifndef DOCA_DPA_DEV_RDMA_H_
#define DOCA_DPA_DEV_RDMA_H_

#include <doca_dpa_dev.h>
#include <doca_dpa_dev_buf.h>
#include <doca_dpa_dev_sync_event.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DPA RDMA handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_rdma_t;

/**
 * @brief DPA RDMA SRQ handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_rdma_srq_t;

/**
 * @brief Synchronize all operations on an RDMA DPA handle
 *
 * @param[in] rdma - RDMA DPA handle
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_synchronize(doca_dpa_dev_rdma_t rdma);

/**
 * @brief Send an RDMA read operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] dst_mmap_handle - destination DOCA Mmap handle
 * @param[in] dst_addr - address of destination buffer
 * @param[in] src_mmap_handle - source DOCA Mmap handle
 * @param[in] src_addr - address of source buffer
 * @param[in] length - length of buffer
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_read(doca_dpa_dev_rdma_t rdma,
				 doca_dpa_dev_mmap_t dst_mmap_handle,
				 uint64_t dst_addr,
				 doca_dpa_dev_mmap_t src_mmap_handle,
				 uint64_t src_addr,
				 size_t length,
				 uint32_t completion_requested);

/**
 * @brief Post an RDMA read operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] dst_buf_handle - destination DOCA buffer DPA handle
 * @param[in] src_buf_handle - source DOCA buffer DPA handle
 * @param[in] completion_requested - to raise a completion when copy data operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_buf_read(doca_dpa_dev_rdma_t rdma,
				     doca_dpa_dev_buf_t dst_buf_handle,
				     doca_dpa_dev_buf_t src_buf_handle,
				     uint32_t completion_requested);

/**
 * @brief Post an RDMA write operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] dst_mmap_handle - destination DOCA Mmap handle
 * @param[in] dst_addr - address of destination buffer
 * @param[in] src_mmap_handle - source DOCA Mmap handle
 * @param[in] src_addr - address of source buffer
 * @param[in] length - length of buffer
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_write(doca_dpa_dev_rdma_t rdma,
				  doca_dpa_dev_mmap_t dst_mmap_handle,
				  uint64_t dst_addr,
				  doca_dpa_dev_mmap_t src_mmap_handle,
				  uint64_t src_addr,
				  size_t length,
				  uint32_t completion_requested);

/**
 * @brief Post an RDMA write operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] dst_buf_handle - destination DOCA buffer DPA handle
 * @param[in] src_buf_handle - source DOCA buffer DPA handle
 * @param[in] completion_requested - to raise a completion when copy data operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_buf_write(doca_dpa_dev_rdma_t rdma,
				      doca_dpa_dev_buf_t dst_buf_handle,
				      doca_dpa_dev_buf_t src_buf_handle,
				      uint32_t completion_requested);

/**
 * @brief Post an RDMA write with immediate operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] dst_mmap_handle - destination DOCA Mmap handle
 * @param[in] dst_addr - address of destination buffer
 * @param[in] src_mmap_handle - source DOCA Mmap handle
 * @param[in] src_addr - address of source buffer
 * @param[in] length - length of buffer
 * @param[in] immediate - immediate data
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_write_imm(doca_dpa_dev_rdma_t rdma,
				      doca_dpa_dev_mmap_t dst_mmap_handle,
				      uint64_t dst_addr,
				      doca_dpa_dev_mmap_t src_mmap_handle,
				      uint64_t src_addr,
				      size_t length,
				      uint32_t immediate,
				      uint32_t completion_requested);

/**
 * @brief Post an RDMA write with immediate operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] dst_buf_handle - destination DOCA buffer DPA handle
 * @param[in] src_buf_handle - source DOCA buffer DPA handle
 * @param[in] immediate - immediate data
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_buf_write_imm(doca_dpa_dev_rdma_t rdma,
					  doca_dpa_dev_buf_t dst_buf_handle,
					  doca_dpa_dev_buf_t src_buf_handle,
					  uint32_t immediate,
					  uint32_t completion_requested);

/**
 * @brief Post an RDMA send operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] mmap_handle - send DOCA Mmap handle
 * @param[in] addr - address of send buffer
 * @param[in] length - length of send buffer
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_send(doca_dpa_dev_rdma_t rdma,
				 doca_dpa_dev_mmap_t mmap_handle,
				 uint64_t addr,
				 size_t length,
				 uint32_t completion_requested);

/**
 * @brief Post an RDMA send operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] send_buf_handle - send DOCA buffer DPA handle
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_buf_send(doca_dpa_dev_rdma_t rdma,
				     doca_dpa_dev_buf_t send_buf_handle,
				     uint32_t completion_requested);

/**
 * @brief Post an RDMA send with immediate operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] mmap_handle - send DOCA Mmap handle
 * @param[in] addr - address of send buffer
 * @param[in] length - length of send buffer
 * @param[in] immediate - immediate data
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_send_imm(doca_dpa_dev_rdma_t rdma,
				     doca_dpa_dev_mmap_t mmap_handle,
				     uint64_t addr,
				     size_t length,
				     uint32_t immediate,
				     uint32_t completion_requested);

/**
 * @brief Post an RDMA send with immediate operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] send_buf_handle - send DOCA buffer DPA handle
 * @param[in] immediate - immediate data
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_buf_send_imm(doca_dpa_dev_rdma_t rdma,
					 doca_dpa_dev_buf_t send_buf_handle,
					 uint32_t immediate,
					 uint32_t completion_requested);

/**
 * @brief Post an RDMA receive operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] mmap_handle - received DOCA Mmap handle
 * @param[in] addr - address of received buffer
 * @param[in] length - length of received buffer
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_receive(doca_dpa_dev_rdma_t rdma,
				    doca_dpa_dev_mmap_t mmap_handle,
				    uint64_t addr,
				    size_t length);

/**
 * @brief Post an RDMA receive operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] receive_buf_handle - received DOCA buffer DPA handle
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_buf_receive(doca_dpa_dev_rdma_t rdma, doca_dpa_dev_buf_t receive_buf_handle);

/**
 * @brief Ack an RDMA receive operations to enable reposting the buffers
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] num_acked - Number of receives to ack
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_receive_ack(doca_dpa_dev_rdma_t rdma, uint32_t num_acked);

/**
 * @brief Get completion work request index
 *
 * @param[in] comp_element - DPA completion element
 *
 * @return
 * This function returns work request index of completion element
 */
DOCA_EXPERIMENTAL
uint32_t doca_dpa_dev_rdma_completion_get_wr_index(doca_dpa_dev_completion_element_t comp_element);

/**
 * @brief Update SRQ next pointer
 *
 * @param[in] rdma_srq – RDMA SRQ DPA handle
 * @param[in] next_index - next entry index to update
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_srq_post_receive(doca_dpa_dev_rdma_srq_t rdma_srq, uint32_t next_index);

/**
 * @brief Ack an SRQ receive operations to enable reposting the buffers
 *
 * @param[in] rdma_srq – RDMA SRQ DPA handle
 * @param[in] num_acked - Number of receives to ack
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_srq_receive_ack(doca_dpa_dev_rdma_srq_t rdma_srq, uint32_t num_acked);

/**
 * @brief Post an RDMA atomic fetch and add operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] dst_mmap_handle - destination DOCA Mmap handle
 * @param[in] dst_addr - address of destination buffer
 * @param[in] value - value to add to the destination buffer
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_atomic_fetch_add(doca_dpa_dev_rdma_t rdma,
					     doca_dpa_dev_mmap_t dst_mmap_handle,
					     uint64_t dst_addr,
					     uint64_t value,
					     uint32_t completion_requested);

/**
 * @brief Post an RDMA atomic fetch and add operation
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] dst_buf_handle - destination buffer DPA handle
 * @param[in] value - value to add to the destination buffer
 * @param[in] completion_requested - to raise a completion when operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_post_buf_atomic_fetch_add(doca_dpa_dev_rdma_t rdma,
						 doca_dpa_dev_buf_t dst_buf_handle,
						 uint64_t value,
						 uint32_t completion_requested);

/**
 * @brief Signal to set a remote sync event count
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] remote_sync_event - remote sync event DPA handle
 * @param[in] count - count to set
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_signal_set(doca_dpa_dev_rdma_t rdma,
				  doca_dpa_dev_sync_event_remote_net_t remote_sync_event,
				  uint64_t count);

/**
 * @brief Signal to atomically add to a remote sync event count
 *
 * @param[in] rdma - RDMA DPA handle
 * @param[in] remote_sync_event - remote sync event DPA handle
 * @param[in] count - count to add
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_rdma_signal_add(doca_dpa_dev_rdma_t rdma,
				  doca_dpa_dev_sync_event_remote_net_t remote_sync_event,
				  uint64_t count);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DPA_DEV_RDMA_H_ */
