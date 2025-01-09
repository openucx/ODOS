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

#ifndef DOCA_PCC_DEV_H_
#define DOCA_PCC_DEV_H_

/**
 * @brief declares that we are compiling for the DPA Device
 *
 * @note Must be defined before the first API use/include of DOCA
 */
#define DOCA_DPA_DEVICE

#include <doca_pcc_dev_common.h>
#include <doca_pcc_dev_utils.h>
#include <doca_pcc_dev_services.h>
#include <doca_pcc_dev_data_structures.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief CC event type
 */
typedef enum {
	DOCA_PCC_DEV_EVNT_NULL = 0,	 /**< Unspecified event type */
	DOCA_PCC_DEV_EVNT_FW = 1,	 /**< Deprecated - not used */
	DOCA_PCC_DEV_EVNT_ROCE_CNP = 2,	 /**< RoCE CNP (Congestion Notification Packet) received */
	DOCA_PCC_DEV_EVNT_ROCE_TX = 3,	 /**< TX packet burst transition ended */
	DOCA_PCC_DEV_EVNT_ROCE_ACK = 4,	 /**< RoCE ACK Packet received */
	DOCA_PCC_DEV_EVNT_ROCE_NACK = 5, /**< RoCE NACK Packet received */
	DOCA_PCC_DEV_EVNT_RTT = 6,	 /**< RTT probe response packet event */
} doca_pcc_dev_event_type_enum;

/**
 * @brief CC Nack event subtypes
 */
typedef enum {
	DOCA_PCC_DEV_NACK_EVNT_NULL = 0,     /**< Unspecified NACK type */
	DOCA_PCC_DEV_NACK_EVNT_RNR = 1,	     /**< RNR (Receiver Not Ready) NACK received */
	DOCA_PCC_DEV_NACK_EVNT_OOS = 2,	     /**< OOS (Out of Sequence) NACK received */
	DOCA_PCC_DEV_NACK_EVNT_DUP_READ = 3, /**< Duplicated Read (with same PSN) NACK received */
} doca_pcc_dev_nack_event_sub_type_enum;

/**
 * @brief TX Flag: Ack expected
 */
#define DOCA_PCC_DEV_TX_FLAG_ACK_EXPECTED (1 << 0)

/**
 * @brief TX Flag: Overloaded:
 */
#define DOCA_PCC_DEV_TX_FLAG_OVERLOADED (1 << 1)

/**
 * @brief TX Flag: RTT packet sent
 */
#define DOCA_PCC_DEV_TX_FLAG_RTT_REQ_SENT (1 << 2)

/**
 * @brief defines the fixed point fraction size of the rate limiter
 */
#define DOCA_PCC_DEV_LOG_MAX_RATE (20) /* rate format in fixed point 20 */

/**
 * @brief Max rate in rate limiter fixed point
 */
#define DOCA_PCC_DEV_MAX_RATE (1U << DOCA_PCC_DEV_LOG_MAX_RATE)

/**
 * @brief Default rate. The user overrides the default in the user algo function
 */
#define DOCA_PCC_DEV_DEFAULT_RATE ((DOCA_PCC_DEV_MAX_RATE >> 8) > (1) ? (DOCA_PCC_DEV_MAX_RATE >> 8) : (1))

/**
 * @brief Max number of NIC ports supported by the lib
 */
#define DOCA_PCC_DEV_MAX_NUM_PORTS (4)

/**
 * @brief Max number of algo slots supported by the lib
 */
#define DOCA_PCC_DEV_MAX_NUM_USER_SLOTS (8)

/**
 * @brief Max number of algos supported by the lib
 */
#define DOCA_PCC_DEV_MAX_NUM_ALGOS (8)

/**
 * @brief Max number of parameters per algo supported by the lib
 */
#define DOCA_PCC_DEV_MAX_NUM_PARAMS_PER_ALGO (0x1E)

/**
 * @brief Max number of counters per algo supported by the lib
 */
#define DOCA_PCC_DEV_MAX_NUM_COUNTERS_PER_ALGO (0xF)

/**
 * @brief Reserved algo slot for internal algo provided by the lib.
 */
#define DOCA_PCC_DEV_ALGO_SLOT_INTERNAL (0xF)

/**
 * @brief Reserved algo index for internal algo provided by the lib.
 */
#define DOCA_PCC_DEV_ALGO_INDEX_INTERNAL (0xF)

/**
 * @brief Implements the internal CC algorithm provided by the lib
 *
 * The lib provides an internal built-in CC algorithm implementation.
 * The user may call this function for flows with algo_slot
 * that is not set by the user (An unknown algo_slot can be the result of running without algo negotiation)
 *
 * @param[in]  algo_ctxt - @see doca_pcc_dev_user_algo
 * @param[in]  event -     @see doca_pcc_dev_user_algo
 * @param[in]  attr -      @see doca_pcc_dev_user_algo
 * @param[out] results -   @see doca_pcc_dev_user_algo
 *
 * @return void.
 */
DOCA_STABLE
void doca_pcc_dev_default_internal_algo(doca_pcc_dev_algo_ctxt_t *algo_ctxt,
					doca_pcc_dev_event_t *event,
					const doca_pcc_dev_attr_t *attr,
					doca_pcc_dev_results_t *results);

/**
 * @brief Entry point to the user algorithm handling code
 *
 * This code handles a single event. it receives the algorithm context,
 * the event information (opaque struct), and some attributes (algo id), and returns
 * the PCC rate
 * The event info should not be used directly through the struct. It is recommended to use
 * the supplied "getter" functions (doca_pcc_dev_event.h) to help generate more future
 * compatible code if event information placement changes
 *
 * @param[in]  algo_ctxt - pointer to user context for this flow (restored from previous iteration)
 * @param[in]  event - pointer to event data struct to be used with getter functions
 * @param[in]  attr - information about event like algo type
 * @param[out]  results - new rate information to be written to HW.
 *			  The rate is expressed as a 20b fixed point number in range (0 , 1]
 *
 * @return void.
 */
DOCA_STABLE
void doca_pcc_dev_user_algo(doca_pcc_dev_algo_ctxt_t *algo_ctxt,
			    doca_pcc_dev_event_t *event,
			    const doca_pcc_dev_attr_t *attr,
			    doca_pcc_dev_results_t *results);

/**
 * @brief Entry point to the user one time initialization code
 *
 * This is called on PCC process load and should initialize the data of
 * all user algorithms.
 *
 * @param[out]  disable_event_bitmask - a bitmaks of events that should be discarded and not passed
 * to the event processing code
 *
 * @return void.
 */
DOCA_STABLE
void doca_pcc_dev_user_init(uint32_t *disable_event_bitmask);

/**
 * @brief User callback executed then parameters are set.
 *
 * Called when the parameter change was set externally.
 * The implementation should:
 *     Check the given new_parameters values. If those are correct from the algorithm perspective,
 *     assign them to the given parameter array.
 *
 * @param[in]  port_num - index of the port
 * @param[in]  algo_slot - Algo slot identifier as referred to in the PPCC command field "algo_slot"
 * if possible it should be equal to the algo_idx
 * @param[in]  param_id_base - id of the first parameter that was changed.
 * @param[in]  param_num - number of all parameters that were changed
 * @param[in]  new_param_values - pointer to an array which holds param_num number of new values for parameters
 * @param[in]  params - pointer to an array which holds beginning of the current parameters to be changed
 *
 * @return -
 * DOCA_PCC_DEV_STATUS_OK: Parameters were set
 * DOCA_PCC_DEV_STATUS_FAIL: the values (one or more) are not legal. No parameters were changed
 *
 */
DOCA_STABLE
doca_pcc_dev_error_t doca_pcc_dev_user_set_algo_params(uint32_t port_num,
						       uint32_t algo_slot,
						       uint32_t param_id_base,
						       uint32_t param_num,
						       const uint32_t *new_param_values,
						       uint32_t *params);

#ifdef __cplusplus
}
#endif

#endif /* DOCA_PCC_DEV_H_ */

/** @} */
