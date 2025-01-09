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
 * @file doca_eth_txq.h
 * @page DOCA ETH TXQ
 * @defgroup DOCAETHTXQ DOCA ETH TXQ
 * DOCA ETH TXQ library.
 *
 * \note
 * There are 2 data path options to use DOCA ETH TXQ context, a GPU managed control path and a CPU managed one.
 * Other than the need to use doca_ctx_set_datapath_on_gpu() on a GPU context before starting it,
 * both cases share the same control path functions (unless mentioned otherwise in the function documentation).
 * The data path functions are different for the different options. The GPU managed data path functions are not
 * included in the DOCA ETH TXQ API (check DOCA GPUNetIO).
 *
 * @{
 */
#ifndef DOCA_ETH_TXQ_H_
#define DOCA_ETH_TXQ_H_

#include <stdint.h>

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

/*********************************************************************************************************************
 * DOCA ETH TXQ Context
 *********************************************************************************************************************/

/**
 * Opaque structure representing a DOCA ETH TXQ instance.
 */
struct doca_eth_txq;
struct doca_gpu_eth_txq;

/**
 * TX queue type.
 */
enum doca_eth_txq_type {
	DOCA_ETH_TXQ_TYPE_REGULAR = 0,
};

/**
 * TX data-path type.
 */
enum doca_eth_txq_data_path_type {
	DOCA_ETH_TXQ_DATA_PATH_TYPE_CPU = 0, /**< Enable data path management on the CPU */
	DOCA_ETH_TXQ_DATA_PATH_TYPE_GPU,     /**< Enable data path management on the GPU */
};

/**
 * @brief Create a DOCA ETH TXQ instance.
 *
 * @param [in] dev
 * Device to bind the context.
 * @param [in] max_burst_size
 * Max burst size to use in context.
 * @param [out] eth_txq
 * Pointer to pointer to be set to point to the created doca_eth_txq instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_eth_txq.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_create(struct doca_dev *dev, uint32_t max_burst_size, struct doca_eth_txq **eth_txq);

/**
 * @brief Destroy a DOCA ETH TXQ instance.
 *
 * @param [in] eth_txq
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - eth_txq argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - eth_txq context state is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_destroy(struct doca_eth_txq *eth_txq);

/**
 * @brief Convert doca_eth_txq instance into a generalized context for use with doca core objects.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_eth_txq_as_doca_ctx(struct doca_eth_txq *eth_txq);

/**
 * @brief Set max burst size property for doca_eth_txq.
 *	  This value dictates the maximal number of packets the HW can handle at the same time.
 * can only be called before calling doca_ctx_start().
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
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
doca_error_t doca_eth_txq_set_max_burst_size(struct doca_eth_txq *eth_txq, uint32_t max_burst_size);

/**
 * @brief Set the maximal send buffer list length for doca_eth_txq.
 *	  This value indicated what the maximal number of elements in a doca_buf list is.
 * can only be called before calling doca_ctx_start().
 *
 * @note The default maximal send buffer list length is 1.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] max_send_buf_list_len
 * Maximal send buffer list length to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_set_max_send_buf_list_len(struct doca_eth_txq *eth_txq, uint32_t max_send_buf_list_len);

/**
 * @brief Set the Maximum Segment Size for doca_eth_txq.
 *	  This value is the maximum data size that can be sent in each segment of the LSO packet.
 * can only be called before calling doca_ctx_start().
 *
 * @note The default MSS is 1500.
 * @note This setter is relevant only in case of using LSO send tasks.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] mss
 * Maximum Segment Size to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_set_mss(struct doca_eth_txq *eth_txq, uint16_t mss);

/**
 * @brief Set the maximum LSO header size for doca_eth_txq.
 *	  This value is the maximum header size of the LSO packet
 * can only be called before calling doca_ctx_start().
 *
 * @note The default maximum LSO header size is 74.
 * @note This setter is relevant only in case of using LSO send tasks.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] max_lso_header_size
 * Maximum LSO header size to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_set_max_lso_header_size(struct doca_eth_txq *eth_txq, uint16_t max_lso_header_size);

/**
 * @brief Set TX queue type property for doca_eth_txq.
 * can only be called before calling doca_ctx_start().
 *
 * @note The default type is DOCA_ETH_TXQ_TYPE_REGULAR.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] type
 * TX queue type - see enum doca_eth_txq_type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_set_type(struct doca_eth_txq *eth_txq, enum doca_eth_txq_type type);

/**
 * @brief Set offload for the calculation of IPv4 checksum (L3) on transmitted packets.
 *	  If the users enables L3 checksum offloading, then the HW will calculate the checksum value
 *	  and write it into the specific field in the packet.
 * can only be called before calling doca_ctx_start().
 *
 * @note The default configuration is to disable L3 checksum offloading.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] enable_l3_chksum
 * An indicator to enable/disable L3 checksum.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_set_l3_chksum_offload(struct doca_eth_txq *eth_txq, uint8_t enable_l3_chksum);

/**
 * @brief Set offload for the calculation of TCP/UDP checksum (L4) on transmitted packets.
 *	  If the users enables L4 checksum offloading, then the HW will calculate the checksum value
 *	  and write it into the specific field in the packet.
 * can only be called before calling doca_ctx_start().
 *
 * @note The default configuration is to disable L4 checksum offloading.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] enable_l4_chksum
 * An indicator to enable/disable L4 checksum.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_set_l4_chksum_offload(struct doca_eth_txq *eth_txq, uint8_t enable_l4_chksum);

/**
 * @brief Set offload to enable wait on time feature on the queue.
 * can only be called before calling doca_ctx_start().
 *
 * @note Supported for DOCA ETH TXQ instance for GPU only.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 * - DOCA_ERROR_NOT_PERMITTED - wait on time HW support but network device clock is not in REAL TIME mode.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_set_wait_on_time_offload(struct doca_eth_txq *eth_txq);

/**
 * @brief Get a gpu handle of a doca_eth_txq.
 *
 * @note Supported for DOCA ETH TXQ instance for GPU only.
 *
 * @details This method should be used after ctx is started.
 * The expected flow is as follows:
 * 1. bind the ctx to a gpu device using doca_ctx_set_datapath_on_gpu()
 * 2. start the ctx using doca_ctx_start()
 * 3. call doca_eth_txq_get_gpu_handle() to get the gpu_handle
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [out] eth_txq_ext
 * A doca gpu eth_txq handle.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is not idle.
 * - DOCA_ERROR_NOT_SUPPORTED - in case eth_txq isn't an instance for GPU.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_get_gpu_handle(const struct doca_eth_txq *eth_txq, struct doca_gpu_eth_txq **eth_txq_ext);

/**
 * @brief Get the maximum burst size supported by the device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [in] max_send_buf_list_len
 * Maximal send buffer list length to use in context.
 * (check doca_eth_txq_set_max_send_buf_list_len)
 * @param [in] max_lso_header_size
 * Maximum header size of an LSO packet to use in context.
 * Can be 0 in case user isn't planning to use LSO tasks.
 * @param [out] max_burst_size
 * The max burst size supported by the device.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_cap_get_max_burst_size(const struct doca_devinfo *devinfo,
						 uint32_t max_send_buf_list_len,
						 uint16_t max_lso_header_size,
						 uint32_t *max_burst_size);

/**
 * @brief Get the maximum send buffer list length supported by the device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [out] max_send_buf_list_len
 * Maximal send buffer list length to get.
 * (check doca_eth_txq_set_max_send_buf_list_len)
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_cap_get_max_send_buf_list_len(const struct doca_devinfo *devinfo,
							uint32_t *max_send_buf_list_len);

/**
 * @brief Get the maximum header size of an LSO packet supported by the device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [out] max_lso_header_size
 * Maximum header size of an LSO packet
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_DRIVER - error query underlying network card driver.
 * - DOCA_ERROR_NOT_SUPPORTED - if LSO is not supported.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_cap_get_max_lso_header_size(const struct doca_devinfo *devinfo,
						      uint16_t *max_lso_header_size);

/**
 * @brief Check if TX queue type is supported.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [in] type
 * TX queue type - see enum doca_eth_txq_type.
 * @param [in] data_path_type
 * TX data-path type - see enum doca_eth_txq_data_path_type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if type is not supported.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_cap_is_type_supported(const struct doca_devinfo *devinfo,
						enum doca_eth_txq_type type,
						enum doca_eth_txq_data_path_type data_path_type);

/**
 * @brief Check if L3 checksum offload is supported by the device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if L3 checksum offload is not supported.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_cap_is_l3_chksum_offload_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Check if L4 checksum offload is supported by the device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if L4 checksum offload is not supported.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_cap_is_l4_chksum_offload_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Get the max LSO message size by the network device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [out] max_lso_msg_size
 * Maximum LSO message supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_DRIVER - error query underlying network card driver.
 * - DOCA_ERROR_NOT_SUPPORTED - if LSO is not supported.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_cap_get_max_lso_msg_size(const struct doca_devinfo *devinfo, uint32_t *max_lso_msg_size);

/**
 * @brief Check if wait on time offload is supported by the network device.
 *
 * @param [in] devinfo
 * Pointer to doca_devinfo instance.
 * @param [out] wait_on_time_mode
 * Offload wait on time mode (native or DPDK).
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_DRIVER - error query underlying network card driver
 * - DOCA_ERROR_NOT_SUPPORTED - real-time clock is not enable on the network card.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_cap_get_wait_on_time_offload_supported(const struct doca_devinfo *devinfo,
								 enum doca_eth_wait_on_time_type *wait_on_time_mode);

/**
 * @brief Calculate timestamp to use when setting the wait on time on the Tx queue
 *
 * @note Supported for DOCA ETH TXQ instance for GPU only.
 *
 * @param [in] eth_txq
 * Pointer to doca_eth_txq instance.
 * @param [in] timestamp_ns
 * Timestamp to indicate when send packets.
 * @param [out] wait_on_time_value
 * Value to use to enqueue wait on time in send queue.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_DRIVER - error query underlying network card driver
 * - DOCA_ERROR_NOT_PERMITTED - wait on time clock is not enabled on the network card.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_eth_txq_calculate_timestamp(struct doca_eth_txq *eth_txq,
					      uint64_t timestamp_ns,
					      uint64_t *wait_on_time_value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_ETH_TXQ_H_ */

/** @} */
