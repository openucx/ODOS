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

#ifndef DOCA_PCC_DEV_DATA_STRUCTURES_H_
#define DOCA_PCC_DEV_DATA_STRUCTURES_H_

#include <doca_pcc_dev_data_structure_le.h>

typedef struct mlnx_cc_algo_ctxt_t doca_pcc_dev_algo_ctxt_t;
typedef struct mlnx_cc_results_t doca_pcc_dev_results_t;
typedef struct mlnx_cc_attr_t doca_pcc_dev_attr_t;
typedef struct mlnx_cc_event_t doca_pcc_dev_event_t;
typedef struct mlnx_cc_event_general_attr_t doca_pcc_dev_event_general_attr_t;
typedef struct mlnx_cc_roce_tx_cntrs_t doca_pcc_dev_roce_tx_cntrs_t;
typedef struct mlnx_cc_roce_tx_t doca_pcc_dev_roce_tx_t;
typedef struct mlnx_cc_ack_nack_cnp_extra_t doca_pcc_dev_ack_nack_cnp_extra_t;
typedef struct mlnx_cc_ack_nack_cnp_t doca_pcc_dev_ack_nack_cnp_t;
typedef struct mlnx_cc_rtt_tstamp_t doca_pcc_dev_rtt_tstamp_t;
typedef struct mlnx_cc_fw_data_t doca_pcc_dev_fw_data_t;
typedef union mlnx_cc_event_spec_attr_t doca_pcc_dev_event_spec_attr_t;

#endif /* DOCA_PCC_DEV_DATA_STRUCTURES_H_ */
