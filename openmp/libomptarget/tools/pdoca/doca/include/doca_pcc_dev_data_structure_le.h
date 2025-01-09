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

#ifndef DOCA_PCC_DEV_DATA_STRUCTURE_LE_H_
#define DOCA_PCC_DEV_DATA_STRUCTURE_LE_H_

#include <stdint.h>

struct mlnx_cc_ack_nack_cnp_extra_t {	/* Little Endian */
    uint32_t			num_coalesced:16;			/* number of coalesced events, incremented on each coalesced event */
    uint32_t			reserved_at_0:16;			
/* --------------------------------------------------------- */
};

struct mlnx_cc_roce_tx_cntrs_t {	/* Little Endian */
    uint32_t			sent_32bytes:16;			/* sent 32 bytes amount, additive increase on each event */
    uint32_t			sent_pkts:16;			/* sent packets amount, additive increase on each event */
/* --------------------------------------------------------- */
};

struct mlnx_cc_fw_data_t {	/* Little Endian */
    uint32_t			data[3];			/* 3 dword fw data */
/* --------------------------------------------------------- */
};

struct mlnx_cc_rtt_tstamp_t {	/* Little Endian */
    uint32_t			req_send_timestamp;			/* request send timestamp */
	/*----------------------------------------------------------*/
    uint32_t			req_recv_timestamp;			/* request receive timestamp */
	/*----------------------------------------------------------*/
    uint32_t			resp_send_timestamp;			/* response send timestamp */
	/*----------------------------------------------------------*/
    unsigned char			reserved_at_60[4];			
/* --------------------------------------------------------- */
};

struct mlnx_cc_ack_nack_cnp_t {	/* Little Endian */
    uint32_t			first_timestamp;			/* first coalesced event timestamp */
	/*----------------------------------------------------------*/
    uint32_t			first_sn;			/* first coalesced event serial number */
	/*----------------------------------------------------------*/
    struct mlnx_cc_ack_nack_cnp_extra_t			extra;			/* extra attributes */
	/*----------------------------------------------------------*/
    unsigned char			reserved_at_60[4];			
/* --------------------------------------------------------- */
};

struct mlnx_cc_roce_tx_t {	/* Little Endian */
    uint32_t			first_timestamp;			/* first coalesced event timestamp */
	/*----------------------------------------------------------*/
    struct mlnx_cc_roce_tx_cntrs_t			cntrs;			/* tx counters */
	/*----------------------------------------------------------*/
    uint32_t			flow_qpn:24;			/* flow qp number */
    uint32_t			reserved_at_40:8;			
	/*----------------------------------------------------------*/
    unsigned char			reserved_at_60[4];			
/* --------------------------------------------------------- */
};

union mlnx_cc_event_spec_attr_t {	/* Little Endian */
    struct mlnx_cc_roce_tx_t			roce_tx;			/* tx attributes */
	/*----------------------------------------------------------*/
    struct mlnx_cc_ack_nack_cnp_t			ack_nack_cnp;			/* ack/nack/cnp attributes */
	/*----------------------------------------------------------*/
    struct mlnx_cc_rtt_tstamp_t			rtt_tstamp;			/* rtt timestamp */
	/*----------------------------------------------------------*/
    struct mlnx_cc_fw_data_t			fw_data;			/* fw data */
	/*----------------------------------------------------------*/
    unsigned char			reserved_at_0[16];			
/* --------------------------------------------------------- */
};

struct mlnx_cc_event_general_attr_t {	/* Little Endian */
    uint32_t			ev_type:8;			/* event type */
    uint32_t			ev_subtype:8;			/* event subtype */
    uint32_t			port_num:8;			/* port id */
    uint32_t			flags:8;			/* event flags */
/* --------------------------------------------------------- */
};

struct mlnx_cc_event_t {	/* Little Endian */
    struct mlnx_cc_event_general_attr_t			ev_attr;			/* event general attributes */
	/*----------------------------------------------------------*/
    uint32_t			flow_tag;			/* unique flow id */
	/*----------------------------------------------------------*/
    uint32_t			sn;			/* serial number */
	/*----------------------------------------------------------*/
    uint32_t			timestamp;			/* event timestamp */
	/*----------------------------------------------------------*/
    union mlnx_cc_event_spec_attr_t			ev_spec_attr;			/* attributes which are different for different events */
/* --------------------------------------------------------- */
};

struct mlnx_cc_algo_ctxt_t {	/* Little Endian */
    uint32_t			data[12];			/* 12 dword algorithm context */
/* --------------------------------------------------------- */
};

struct val_t {	/* Little Endian */
    uint32_t			val;			/* uint32 value */
/* --------------------------------------------------------- */
};

struct mlnx_cc_attr_t {	/* Little Endian */
    uint32_t			algo_slot:4;			/* algorithm type defined in API.h, 15 - DCQCN */
    uint32_t			overload:1;			/* overload flag */
    uint32_t			reserved_at_0:27;			
/* --------------------------------------------------------- */
};

struct mlnx_cc_results_t {	/* Little Endian */
    uint32_t			rate;			/* rate value */
	/*----------------------------------------------------------*/
    uint32_t			rtt_req;			/* probe request bit */
/* --------------------------------------------------------- */
};

union union_mlnx_cc_ack_nack_cnp_extra_t {	/* Little Endian */
    struct val_t			val;			/* entire value */
	/*----------------------------------------------------------*/
    struct mlnx_cc_ack_nack_cnp_extra_t			mlnx_cc_ack_nack_cnp_extra;			/* attributes for ack/nack/cnp */
	/*----------------------------------------------------------*/
    unsigned char			reserved_at_0[4];			
/* --------------------------------------------------------- */
};

union union_mlnx_cc_roce_tx_cntrs_t {	/* Little Endian */
    struct val_t			val;			/* entire value */
	/*----------------------------------------------------------*/
    struct mlnx_cc_roce_tx_cntrs_t			mlnx_cc_roce_tx_cntrs;			/* tx counters */
	/*----------------------------------------------------------*/
    unsigned char			reserved_at_0[4];			
/* --------------------------------------------------------- */
};

union union_mlnx_cc_event_general_attr_t {	/* Little Endian */
    struct val_t			val;			/* entire value */
	/*----------------------------------------------------------*/
    struct mlnx_cc_event_general_attr_t			mlnx_cc_event_general_attr;			/* event general attributes */
	/*----------------------------------------------------------*/
    unsigned char			reserved_at_0[4];			
/* --------------------------------------------------------- */
};

#endif /* DOCA_PCC_DEV_DATA_STRUCTURE_LE_H_ */
