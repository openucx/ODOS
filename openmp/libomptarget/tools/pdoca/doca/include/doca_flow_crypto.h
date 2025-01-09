/*
 * Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_flow_crypto.h
 * @page doca flow crypto
 * @defgroup FLOW_CRYPTO flow net define
 * DOCA HW offload flow cryptonet structure define. For more details please refer to
 * the user guide on DOCA devzone.
 *
 * @{
 */

#ifndef DOCA_FLOW_CRYPTO_H_
#define DOCA_FLOW_CRYPTO_H_

#include <doca_compat.h>
#include <doca_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief doca flow port struct
 */
struct doca_flow_port;

/**
 * @brief PSP spi key bulk struct
 */
struct doca_flow_crypto_psp_spi_key_bulk;

/**
 * @brief doca flow crypto operation resource type
 */
enum doca_flow_crypto_resource_type {
	DOCA_FLOW_CRYPTO_RESOURCE_NONE = 0,
	/**< No security resource engaged */
	DOCA_FLOW_CRYPTO_RESOURCE_IPSEC_SA,
	/**< IPsec resource action */
	DOCA_FLOW_CRYPTO_RESOURCE_PSP,
	/**< PSP resource action */
};

/**
 * @brief doca flow crypto operation action type
 */
enum doca_flow_crypto_action_type {
	DOCA_FLOW_CRYPTO_ACTION_NONE = 0,
	/**< No crypto action performed */
	DOCA_FLOW_CRYPTO_ACTION_ENCRYPT,
	/**< Perform encryption */
	DOCA_FLOW_CRYPTO_ACTION_DECRYPT,
	/**< Perform decryption/authentication */
};

/**
 * @brief doca flow crypto operation reformat type
 */
enum doca_flow_crypto_encap_action_type {
	DOCA_FLOW_CRYPTO_REFORMAT_NONE = 0,
	/**< No reformat action performed */
	DOCA_FLOW_CRYPTO_REFORMAT_ENCAP,
	/**< Perform encapsulation action */
	DOCA_FLOW_CRYPTO_REFORMAT_DECAP,
	/**< Perform decapsulation action */
};

/**
 * @brief doca flow crypto operation encapsulation header type
 */
enum doca_flow_crypto_encap_net_type {
	DOCA_FLOW_CRYPTO_HEADER_NONE = 0,
	/**< No network header involved */
	DOCA_FLOW_CRYPTO_HEADER_ESP_TUNNEL,
	/**< ESP tunnel header type */
	DOCA_FLOW_CRYPTO_HEADER_ESP_OVER_IPV4,
	/**< IPv4 network header type */
	DOCA_FLOW_CRYPTO_HEADER_ESP_OVER_IPV6,
	/**< IPv6 network header type */
	DOCA_FLOW_CRYPTO_HEADER_UDP_ESP_OVER_IPV4,
	/**< IPv4 + UDP network header type */
	DOCA_FLOW_CRYPTO_HEADER_UDP_ESP_OVER_IPV6,
	/**< IPv6 + UDP network header type */
	DOCA_FLOW_CRYPTO_HEADER_ESP_OVER_LAN,
	/**< UDP, TCP or ICMP network header type */
	DOCA_FLOW_CRYPTO_HEADER_PSP_TUNNEL,
	/**< PSP tunnel header type */
	DOCA_FLOW_CRYPTO_HEADER_PSP_OVER_IPV4,
	/**< PSP transport IPv4 network header type */
	DOCA_FLOW_CRYPTO_HEADER_PSP_OVER_IPV6,
	/**< PSP transport IPv6 network header type */
};

/**
 * @brief doca flow crypto key type
 */
enum doca_flow_crypto_key_type {
	DOCA_FLOW_CRYPTO_KEY_128,
	/**< Key type 128 */
	DOCA_FLOW_CRYPTO_KEY_256,
	/**< Key type 256 */
};

/**
 * @brief doca flow crypto ICV length
 */
enum doca_flow_crypto_icv_len {
	DOCA_FLOW_CRYPTO_ICV_LENGTH_8,
	/**< size of 8 bit */
	DOCA_FLOW_CRYPTO_ICV_LENGTH_12,
	/**< size of 12 bit */
	DOCA_FLOW_CRYPTO_ICV_LENGTH_16,
	/**< size of 16 bit */
};

/**
 * @brief doca flow crypto replay window size
 */
enum doca_flow_crypto_replay_win_size {
	DOCA_FLOW_CRYPTO_REPLAY_WIN_SIZE_32,
	/**< Replay window size of 32 bit */
	DOCA_FLOW_CRYPTO_REPLAY_WIN_SIZE_64,
	/**< Replay window size of 64 bit */
	DOCA_FLOW_CRYPTO_REPLAY_WIN_SIZE_128,
	/**< Replay window size of 128 bit */
	DOCA_FLOW_CRYPTO_REPLAY_WIN_SIZE_256,
	/**< Replay window size of 256 bit */
};

/**
 * @brief doca flow crypto SN oflload type
 */
enum doca_flow_crypto_sn_offload_type {
	DOCA_FLOW_CRYPTO_SN_OFFLOAD_INC,
	/**< Increment sequence number - encrypt direction */
	DOCA_FLOW_CRYPTO_SN_OFFLOAD_AR,
	/**< Anti-replay - decrypt direction */
};

/**
 * @brief doca flow crypto key configuration
 */
struct doca_flow_crypto_key_cfg {
	enum doca_flow_crypto_key_type key_type;
	/**< key type - 128/256 */
	uint32_t *key;
	/**< Key data based on key type */
};

/**
 * @brief Rotate PSP master key.
 *
 * This API is used to rotate PSP master key. New key will be used to generate
 * pairs of SPI and key, and old key is still valid for decryption until another
 * key rotate will be called.
 *
 * @param [in] port
 * Pointer to doca flow port.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - driver error.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_crypto_psp_master_key_rotate(struct doca_flow_port *port);

/**
 * @brief Allocate an array of spi and key pairs
 *
 * This API is used to allocate the memory needed for the array, based on the key type
 * and the number of spi keys.
 *
 * To fill the array need to call doca_flow_crypto_psp_spi_key_bulk_generate function
 * To free the memory need to call doca_flow_crypto_psp_spi_key_bulk_free function
 *
 * @param [in] port
 * Pointer to doca flow port.
 * @param [in] key_type
 * DOCA_FLOW_CRYPTO_KEY_128 or DOCA_FLOW_CRYPTO_KEY_256.
 * @param [in] nr_spi_keys
 * Array length.
 * @param [in] spi_key_bulk
 * Spi key bulk handler on success
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - memory allocation failed.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_crypto_psp_spi_key_bulk_alloc(struct doca_flow_port *port,
						     enum doca_flow_crypto_key_type key_type,
						     uint32_t nr_spi_keys,
						     struct doca_flow_crypto_psp_spi_key_bulk **spi_key_bulk);

/**
 * @brief Fill a bulk with new pairs of SPI and key
 *
 * Fill an allocated bulk object with new pairs, based on the key type and number of spi keys.
 * This API can be used more than once on allocated bulk.
 *
 * To get a pair based on an index need to call doca_flow_crypto_psp_spi_key_bulk_get function
 *
 * @param [in] spi_key_bulk
 * pointer to spi key bulk
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - memory allocation failed.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_crypto_psp_spi_key_bulk_generate(struct doca_flow_crypto_psp_spi_key_bulk *spi_key_bulk);

/**
 * @brief Get SPI and key for specific index in the bulk
 *
 * @param [in] spi_key_bulk
 * pointer to spi key bulk
 * @param [in] spi_key_idx
 * Index in the bulk
 * @param [in] spi
 * pointer to the spi
 * @param [in] key
 * pointer to the key
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_crypto_psp_spi_key_bulk_get(struct doca_flow_crypto_psp_spi_key_bulk *spi_key_bulk,
						   uint32_t spi_key_idx,
						   uint32_t *spi,
						   uint32_t *key);

/**
 * @brief Wipe the memory of a key for specific index in the bulk
 *
 * @param [in] spi_key_bulk
 * pointer to spi key bulk
 * @param [in] spi_key_idx
 * Index in the bulk
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_crypto_psp_spi_key_wipe(struct doca_flow_crypto_psp_spi_key_bulk *spi_key_bulk,
					       uint32_t spi_key_idx);

/**
 * @brief Clear the bulk data
 *
 * Clear the allocated bulk data. After all the keys in the bulk were disposed,
 * if the user is going to generate new bulk in the future,
 * it is recommended to clear the bulk memory with this API.
 *
 * @param [in] spi_key_bulk
 * pointer to spi key bulk
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_crypto_psp_spi_key_bulk_clear(struct doca_flow_crypto_psp_spi_key_bulk *spi_key_bulk);

/**
 * @brief Free the memory for spi key bulk
 *
 * @param [in] spi_key_bulk
 * pointer to spi key bulk
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_crypto_psp_spi_key_bulk_free(struct doca_flow_crypto_psp_spi_key_bulk *spi_key_bulk);

/**
 * @brief Handle ipsec resources.
 *
 * Update relevnt data according to HW state. This API must be called in order to
 * keep a valid state of a sequence number.
 *
 * Handling of resources can take too much time, so we split each cycle
 * to small chunks that are limited by some time quota.
 *
 * As long as the function doesn't return -1, more resources
 * are pending processing for this cycle.
 *
 * @param [in] port
 * Port to handle resources
 * @param [in] quota
 * Max time quota in micro seconds, 0: no limit.
 * @param [in] max_processed_resources
 * Max resource for this function to handle, 0: no limit.
 * @return
 * > 0 the number of hadled resources.
 * 0 no resource handled in current call.
 * -1 full cycle done.
 */
DOCA_EXPERIMENTAL
int doca_flow_crypto_ipsec_resource_handle(struct doca_flow_port *port,
					   uint64_t quota,
					   uint32_t max_processed_resources);

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_FLOW_CRYPTO_H_ */
