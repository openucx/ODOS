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
 * @file doca_eth_rxq.h
 * @page DOCA ETH RXQ
 * @defgroup DOCAETHRXQ DOCA ETH RXQ
 * DOCA ETH RXQ library.
 *
 * \note
 * There are 2 data path options to use DOCA ETH RXQ context, a GPU managed control path and a CPU managed one.
 * Other than the need to use doca_ctx_set_datapath_on_gpu() on a GPU context before starting it,
 * both cases share the same control path functions (unless mentioned otherwise in the function documentation).
 * The data path functions are different for the different options. The GPU managed data path functions are not
 * included in the DOCA ETH RXQ API (check DOCA GPUNetIO).
 *
 * @{
 */
#ifndef DOCA_ETH_RXQ_H_
#define DOCA_ETH_RXQ_H_

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * DOCA core opaque types
 *********************************************************************************************************************/

struct doca_ctx;
struct doca_dev;
struct doca_devinfo;
struct doca_mmap;

/*********************************************************************************************************************
 * DOCA ETH RXQ Context
 *********************************************************************************************************************/

/**
 * Opaque structure representing a DOCA ETH RXQ instance.
 */
struct doca_eth_rxq;
struct doca_gpu_eth_rxq;

/**
 * RX queue type.
 */
enum doca_eth_rxq_type {
	DOCA_ETH_RXQ_TYPE_CYCLIC = 0,	   /**< This mode is optimized for max packet rate. In this mode the library
					    *  will receive packets in a cyclic manner. The processing time of
					    *  packets should be faster than the rate in which they are received.
					    *  If the application did not process the packets fast enough, the packet
					    *  may be overrun by a new packet once the recv callback has ended.
					    *  The receive callback should finish processing the packet before
					    *  returning or copy the content. The user will supply a doca_mmap for
					    *  DOCA ETH RXQ context. The recommended size of this mmap should be
					    *  calculated using doca_eth_rxq_estimate_packet_buf_size.
					    */
	DOCA_ETH_RXQ_TYPE_MANAGED_MEMPOOL, /**< In this mode the library will manage the memory and use various HW
					    *  features to optimize memory consumption while increasing packet rate.
					    *  The user will supply a doca_mmap for DOCA ETH RXQ context. The
					    *  recommended size of this mmap should be calculated using
					    *  doca_eth_rxq_estimate_packet_buf_size.
					    *  The library assumes the user will release the packet buffer within a
					    *  bound amount of time. Keeping packets for a long time without freeing
					    *  it will block receiving incoming packets.
					    *  NOTE: This type is supported only for DOCA ETH RXQ instance for CPU
					    */
	DOCA_ETH_RXQ_TYPE_REGULAR,	   /**< In this mode the user posts a receive task, telling DOCA_RXQ to which
					    *  buffer to scatter the incoming packet.
					    *  NOTE: This type is supported only for DOCA ETH RXQ instance for CPU
					    */
};

/**
 * RX data-path type.
 */
enum doca_eth_rxq_data_path_type {
	DOCA_ETH_RXQ_DATA_PATH_TYPE_CPU = 0, /**< Enable data path management on the CPU */
	DOCA_ETH_RXQ_DATA_PATH_TYPE_GPU,     /**< Enable data path management on the GPU */
};

/**
 * @brief Create a DOCA ETH RXQ instance.
 *
 * @param [in] dev
 * Device to bind the context.
 * @param [in] max_burst_size
 * Max burst size to use in context.
 * @param [in] max_packet_size
 * Max packet size to use in context.
 * @param [out] eth_rxq
 * Pointer to pointer to be set to point to the created doca_eth_rxq instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - eth_rxq argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate resources.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_create(struct doca_dev *dev,
				 uint32_t max_burst_size,
				 uint32_t max_packet_size,
				 struct doca_eth_rxq **eth_rxq);

/**
 * @brief Destroy a DOCA ETH RXQ instance.
 *
 * @param [in] eth_rxq
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - eth_rxq argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - eth_rxq context state is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_destroy(struct doca_eth_rxq *eth_rxq);

/**
 * @brief Convert doca_eth_rxq instance into a generalized context for use with doca core objects.
 *
 * @param [in] eth_rxq
 * Pointer to doca_eth_rxq instance.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_eth_rxq_as_doca_ctx(struct doca_eth_rxq *eth_rxq);

/**
 * @brief Set max burst size property for doca_eth_rxq.
 *	  This value dictates the maximal number of packets the HW can handle at the same time.
 * can only be called before calling doca_ctx_start().
 *
 * @param [in] eth_rxq
 * Pointer to doca_eth_rxq instance.
 * @param [in] max_burst_size
 * Max burst size to use in context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_set_max_burst_size(struct doca_eth_rxq *eth_rxq, uint32_t max_burst_size);

/**
 * @brief Set max packet size property for doca_eth_rxq.
 * can only be called before calling doca_ctx_start().
 *
 * @note Function is irrelevant in the case of context of type DOCA_ETH_RXQ_TYPE_REGULAR.
 *
 * @param [in] eth_rxq
 * Pointer to doca_eth_rxq instance.
 * @param [in] max_packet_size
 * Max packet size to use in context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_set_max_packet_size(struct doca_eth_rxq *eth_rxq, uint32_t max_packet_size);

/**
 * @brief Set the maximal receive buffer list length for doca_eth_rxq.
 *	  This value indicated what the maximal number of elements in a doca_buf list is.
 * can only be called before calling doca_ctx_start().
 *
 * @note Function is relevant only in the case of context of type DOCA_ETH_RXQ_TYPE_REGULAR.
 * @note The default maximal receive buffer list length is 1.
 *
 * @param [in] eth_rxq
 * Pointer to doca_eth_rxq instance.
 * @param [in] max_recv_buf_list_len
 * Maximal receive buffer list length to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_set_max_recv_buf_list_len(struct doca_eth_rxq *eth_rxq, uint32_t max_recv_buf_list_len);

/**
 * @brief Set Eth packet buffer for a doca_eth_rxq.
 * can only be called before calling doca_ctx_start().
 *
 * @note Function is irrelevant in the case of context of type DOCA_ETH_RXQ_TYPE_REGULAR.
 *
 * @param [in] eth_rxq
 * Pointer to doca_eth_rxq instance.
 * @param [in] mmap
 * The mmap consist of the memrange for the Eth packet buffer.
 * @param [in] offset
 * The offset from mmap start to set the packet buffer.
 * @param [in] size
 * The size of the Eth packet buffer.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_set_pkt_buf(struct doca_eth_rxq *eth_rxq,
				      struct doca_mmap *mmap,
				      uint32_t offset,
				      uint32_t size);

/**
 * @brief Set RX queue type property for doca_eth_rxq.
 * can only be called before calling doca_ctx_start().
 *
 * @note The default type is DOCA_ETH_RXQ_TYPE_REGULAR.
 *
 * @param [in] eth_rxq
 * Pointer to doca_eth_rxq instance.
 * @param [in] type
 * RX queue type - see enum doca_eth_rxq_type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_set_type(struct doca_eth_rxq *eth_rxq, enum doca_eth_rxq_type type);

/**
 * @brief Get the DPDK queue ID of the doca_eth receive queue.
 * can only be called after calling doca_ctx_start().
 *
 * @param [in] eth_rxq
 * Pointer to doca_eth_rxq instance.
 * @param [out] flow_queue_id
 * The queue ID to be used in rte_flow or doca_flow.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context was not started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_get_flow_queue_id(struct doca_eth_rxq *eth_rxq, uint16_t *flow_queue_id);

/**
 * @brief Get a gpu handle of a doca_eth_rxq.
 *
 * @note Supported for DOCA ETH RXQ instance for GPU only.
 *
 * @details This method should be used after ctx is started.
 * The expected flow is as follows:
 * 1. bind the ctx to a gpu device using doca_ctx_set_datapath_on_gpu()
 * 2. start the ctx using doca_ctx_start()
 * 3. call doca_eth_rxq_get_gpu_handle() to get the gpu_handle
 *
 * @param [in] eth_rxq
 * Pointer to doca_eth_rxq instance.
 * @param [out] eth_rxq_ext
 * A doca gpu eth_rxq handle.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not started.
 * - DOCA_ERROR_NOT_SUPPORTED - in case eth_rxq isn't an instance for GPU.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_get_gpu_handle(const struct doca_eth_rxq *eth_rxq, struct doca_gpu_eth_rxq **eth_rxq_ext);

/**
 * @brief Get the maximum burst size supported by the device.
 *
 * @note Function is relevant only in the case of context of type DOCA_ETH_RXQ_TYPE_REGULAR
 *       (max_burst_size isn't limited in other modes).
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [out] max_burst_size
 * The max burst size.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_cap_get_max_burst_size(const struct doca_devinfo *devinfo, uint32_t *max_burst_size);

/**
 * @brief Get the maximum receive buffer list length supported by the device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [out] max_recv_buf_list_len
 * Maximal receive buffer list length to get.
 * (check doca_eth_rxq_set_max_recv_buf_list_len)
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_cap_get_max_recv_buf_list_len(const struct doca_devinfo *devinfo,
							uint32_t *max_recv_buf_list_len);

/**
 * @brief Get the maximum packet size supported by the device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [out] max_packet_size
 * The max packet size.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_cap_get_max_packet_size(const struct doca_devinfo *devinfo, uint32_t *max_packet_size);

/**
 * @brief Check if RX queue type is supported.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [in] type
 * RX queue type - see enum doca_eth_rxq_type.
 * @param [in] data_path_type
 * RX data-path type - see enum doca_eth_rxq_data_path_type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if type is not supported.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_cap_is_type_supported(const struct doca_devinfo *devinfo,
						enum doca_eth_rxq_type type,
						enum doca_eth_rxq_data_path_type data_path_type);

/**
 * @brief Get the recommended size for the mmap buffer of a doca_eth_rxq.
 *
 * @details This function should be used for calculating the recommended size of the doca_mmap given to
 *	    doca_eth_rxq_set_pkt_buf().
 *
 * @note Function is irrelevant in the case of context of type DOCA_ETH_RXQ_TYPE_REGULAR.
 *
 * @param [in] type
 * Type of DOCA ETH RXQ.
 * @param [in] rate
 * Rate in [MB/s] in which the doca_rxq is expected to receive traffic.
 * @param [in] pkt_max_time
 * Max time in [μs] a packet may take to be processed.
 * @param [in] max_packet_size
 * Max non-LRO packet size in [B].
 * @param [in] max_burst_size
 * Max size of packets burst.
 * @param [in] log_max_lro_pkt_sz
 * Log of max LRO packet size.
 * @param [out] buf_size
 * The recommended size for the mmap buffer in [B].
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if the given type does not require packet buffer size.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_rxq_estimate_packet_buf_size(enum doca_eth_rxq_type type,
						   uint32_t rate,
						   uint16_t pkt_max_time,
						   uint32_t max_packet_size,
						   uint32_t max_burst_size,
						   uint8_t log_max_lro_pkt_sz,
						   uint32_t *buf_size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_ETH_RXQ_H_ */

/** @} */
