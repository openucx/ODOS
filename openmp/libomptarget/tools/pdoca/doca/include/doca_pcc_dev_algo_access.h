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

#ifndef DOCA_PCC_DEV_ALGO_ACCESS_H_
#define DOCA_PCC_DEV_ALGO_ACCESS_H_

#include <doca_pcc_dev.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This struct provides meta data for a pcc user algo
 */
struct doca_pcc_dev_algo_meta_data {
	uint32_t algo_id;	     /**< algo unique identifier */
	uint32_t algo_major_version; /**< algo major version */
	uint32_t algo_minor_version; /**< algo minor version */
	uint32_t algo_desc_size;     /**< size of description string (null terminated) */
	uint64_t algo_desc_addr;     /**< pointer to description string */
};

/**
 * @brief Initialize the algo database
 *
 * This function initializes an algo data-structure.
 * Each algorithm has in index (not to be confused with the doca_pcc_dev_algo_meta_data::algo_id)
 * Algo database includes a metadata area containing basic algo information,
 * and a params and counters metadata area. The number of parameters and counters
 * is configurable at init time.
 * The space allocated for the algo data-structure using this function is visible to the PCC infrastructure.
 * This is required to allow the infrastructure to read/update param / counter information
 * directly when handling PPCC MADs or commands.
 * The user can use additional memory by allocating global variables
 *
 * This function should be called once per algo during init.
 *
 * @param[in]  algo_idx - algo index.
 * @param[in]  user_def - basic version info + pointer and size of algo description string
 * @param[in]  param_num   - max number of params (will be used to reserve param space)
 * @param[in]  counter_num - max number of counters (will be used to reserve counter space)
 *
 * @return DOCA_PCC_DEV_STATUS_FAIL if input parameters are out of range.
 */
DOCA_STABLE
doca_pcc_dev_error_t doca_pcc_dev_algo_init_metadata(uint32_t algo_idx,
						     const struct doca_pcc_dev_algo_meta_data *user_def,
						     uint32_t param_num,
						     uint32_t counter_num);

/**
 * @brief Initialize a single parameter for an algo
 *
 * This function initializes a single parameter (param_id) for a specific algo (algo_idx)
 * The param_id should be in the bounds declared by doca_pcc_dev_algo_init_metadata(...)
 * The param info is "global" to the algo on all ports. The current value of the param that is initialized
 * to the "default" value can be modified at the port level.
 *
 * @param[in]  algo_idx - Algo index.
 * @param[in]  param_id - parameter id (from 0 to doca_pcc_dev_algo_init_metadata(...).param_num)
 * @param[in]  default_value - base value.
 * @param[in]  max_value - max value that is enforced by set function.
 * @param[in]  min_value - min value that is enforced by set function..
 * @param[in]  permissions - If 1 allows value update, if 0 update is disabled.
 * @param[in]  param_desc_size - The size in bytes for the parameter descriptor string field
 * @param[in]  param_desc_addr - A pointer to the parameter descriptor string field.
 *
 * @return DOCA_PCC_DEV_STATUS_FAIL if input parameters are out of range.
 */
DOCA_STABLE
doca_pcc_dev_error_t doca_pcc_dev_algo_init_param(uint32_t algo_idx,
						  uint32_t param_id,
						  uint32_t default_value,
						  uint32_t max_value,
						  uint32_t min_value,
						  uint32_t permissions,
						  uint32_t param_desc_size,
						  uint64_t param_desc_addr);

/**
 * @brief Initialize a single counter for an algo
 *
 * This function initializes a single counter (counterid) for a specific algo (algo_idx)
 * The counter_id should be in the bounds declared by doca_pcc_dev_algo_init_metadata(...)
 * The counter info (e.g. description) is "global" to the algo on all ports.
 *
 * @param[in]  algo_idx - Algo identifier.
 * @param[in]  counter_id - counter id (from 0 to doca_pcc_dev_algo_init_metadata(...).counter_num)
 * @param[in]  max_value - max value that allowed for the counter.
 * @param[in]  permissions - If 1 allows value update, if 0 update is disabled.
 * @param[in]  counter_desc_size - The size in bytes for the counter descriptor string field
 * @param[in]  counter_desc_addr - A pointer to the counter descriptor string field.
 *
 * @return DOCA_PCC_DEV_STATUS_FAIL if input parameters are out of range.
 */
DOCA_STABLE
doca_pcc_dev_error_t doca_pcc_dev_algo_init_counter(uint32_t algo_idx,
						    uint32_t counter_id,
						    uint32_t max_value,
						    uint32_t permissions,
						    uint32_t counter_desc_size,
						    uint64_t counter_desc_addr);

/**
 * @brief Initialize the algo per port database
 *
 * This function initializes the algo per port parameter database, and maps an algo_idx (global algo index)
 * to a specific slot per port.
 * This function allocates parameters and counters per port.
 * The default parameters values are taken from the algo metadata set by @ref doca_pcc_dev_algo_init_param() .
 * The counters and parameters can be get/set by the infrastructure based on MAD and access register PPCC command
 * Function MUST be called after calls to doca_pcc_dev_algo_init_param for this algo type
 *
 * @param[in]  portid - port to be initialized
 * @param[in]  algo_slot - Algo slot identifier as referred to in the PPCC command field "algo_slot"
 * if possible it should be equal to the algo_idx
 * @param[in]  algo_idx - Algo identifier.
 * @param[in]  algo_en - 1 mark algo as enabled, if 0 algo will not be reported if queried
 *
 * @return DOCA_PCC_DEV_STATUS_FAIL if input parameters are out of range.
 */
DOCA_STABLE
doca_pcc_dev_error_t doca_pcc_dev_init_algo_slot(uint32_t portid,
						 uint32_t algo_slot,
						 uint32_t algo_idx,
						 uint32_t algo_en);

/**
 * @brief Get number of counters supported per algo on the port
 *
 * @param[in]  port_num
 * @param[in]  algo_slot - Algo slot identifier as referred to in the PPCC command field "algo_slot"
 *
 * @return number of supported counters
 */
DOCA_STABLE
uint32_t doca_pcc_dev_get_counters_num(uint32_t port_num, uint32_t algo_slot);

/**
 * @brief Get pointer to counter array of a specific algo and specific port
 *
 * This retrieves the pointer to an array of counters (up to doca_pcc_dev_get_counters_num(...) counters)
 * used by algo_slot on the port
 *
 * @param[in]  port_num
 * @param[in]  algo_slot - Algo slot identifier as referred to in the PPCC command field "algo_slot"
 *
 * @return array of 32b counters
 */
DOCA_STABLE
uint32_t *doca_pcc_dev_get_counters(uint32_t port_num, uint32_t algo_slot);

/**
 * @brief Get number of params supported per algo on the port
 *
 * @param[in]  port_num
 * @param[in]  algo_slot - Algo slot identifier as referred to in the PPCC command field "algo_slot"
 *
 * @return number of supported params
 */
DOCA_STABLE
uint32_t doca_pcc_dev_get_algo_params_num(uint32_t port_num, uint32_t algo_slot);

/**
 * @brief Get pointer to param array of a specific algo and specific port
 *
 * This retrieves the pointer to an array of param (current value) of up to doca_pcc_dev_get_algo_params_num() params
 * used by algo_slot on the port
 *
 * @param[in]  port_num
 * @param[in]  algo_slot - Algo slot identifier as referred to in the PPCC command field "algo_slot"
 *
 * @return array of 32b parameters
 */
DOCA_STABLE
uint32_t *doca_pcc_dev_get_algo_params(uint32_t port_num, uint32_t algo_slot);

/**
 * @brief Get identifier of a specific algo and specific port
 *
 * @param[in]  port_num
 * @param[in]  algo_slot - Algo slot identifier as referred to in the PPCC command field "algo_slot"
 *
 * @return Algo identifier.
 */
DOCA_STABLE
uint32_t doca_pcc_dev_get_algo_index(uint32_t port_num, uint32_t algo_slot);

#ifdef __cplusplus
}
#endif

#endif /* DOCA_PCC_DEV_ALGO_ACCESS_H_ */
