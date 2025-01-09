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
 * @defgroup PCC_DEVICE PCC Device
 * DOCA PCC Device library. For more details please refer to the user guide on DOCA devzone.
 *
 * @ingroup PCC
 *
 * @{
 */

#ifndef DOCA_PCC_DEV_COMMON_H_
#define DOCA_PCC_DEV_COMMON_H_

/**
 * @brief declares that we are compiling for the DPA Device
 *
 * @note Must be defined before the first API use/include of DOCA
 */
#define DOCA_DPA_DEVICE

#include <stdint.h>
#include <stddef.h>
#include <doca_compat.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief static inline wrapper
 */
#define FORCE_INLINE static inline __attribute__((always_inline))

/**
 * @brief API functions return status
 */
typedef enum {
	DOCA_PCC_DEV_STATUS_OK = 0,   /**< completed successfully */
	DOCA_PCC_DEV_STATUS_FAIL = 1, /**< Failed */
} doca_pcc_dev_error_t;

/**
 * @brief NIC counter IDs
 */
typedef enum {
	DOCA_PCC_DEV_NIC_COUNTER_PORT0_RX_BYTES = 0x10, /**< port 0 RX bytes NIC counter ID */
	DOCA_PCC_DEV_NIC_COUNTER_PORT1_RX_BYTES = 0x11, /**< port 1 RX bytes NIC counter ID */
	DOCA_PCC_DEV_NIC_COUNTER_PORT2_RX_BYTES = 0x12, /**< port 2 RX bytes NIC counter ID */
	DOCA_PCC_DEV_NIC_COUNTER_PORT3_RX_BYTES = 0x13, /**< port 3 RX bytes NIC counter ID */

	DOCA_PCC_DEV_NIC_COUNTER_PORT0_TX_BYTES = 0x20, /**< port 0 TX bytes NIC counter ID */
	DOCA_PCC_DEV_NIC_COUNTER_PORT1_TX_BYTES = 0x21, /**< port 1 TX bytes NIC counter ID */
	DOCA_PCC_DEV_NIC_COUNTER_PORT2_TX_BYTES = 0x22, /**< port 2 TX bytes NIC counter ID */
	DOCA_PCC_DEV_NIC_COUNTER_PORT3_TX_BYTES = 0x23, /**< port 3 TX bytes NIC counter ID */
} doca_pcc_dev_nic_counter_ids_t;

/**
 * @brief Prepare a list of counters to read
 *
 * The list is stored in kernel memory. A single counters config per process is supported.
 *
 * @note arrays memory must be defined in global or heap memory only.
 *
 * @param[in]  counter_ids - An array of counter ids.
 * @param[in]  num_counters - number of counters in the counter_ids array
 * @param[out] values - buffer to store counters values (32b) read by doca_pcc_dev_nic_counters_sample()
 *
 * @return - void
 * process crashes in case of:
 *	counters_ids too large
 *	bad pointers of values, counter_ids
 *	unknown counter
 */
DOCA_EXPERIMENTAL
void doca_pcc_dev_nic_counters_config(uint32_t *counter_ids, uint32_t num_counters, uint32_t *values);

/**
 * @brief Sample counters according to the prior configuration call
 *
 * Sample counter_ids, num_counters and values buffer provided in the last successful call to
 * doca_pcc_dev_nic_counters_config().
 * This call ensures fastest sampling on a pre-checked counter ids and buffers.
 *
 * @return void.
 *	process crashes in case of:
 *	doca_pcc_dev_nic_counters_config() never called
 */
DOCA_EXPERIMENTAL
void doca_pcc_dev_nic_counters_sample(void);

#ifdef __cplusplus
}
#endif

#endif /* DOCA_PCC_DEV_COMMON_H_ */

/** @} */
