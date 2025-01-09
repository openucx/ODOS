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

#ifndef DOCA_PCC_DEV_EVENT_H_
#define DOCA_PCC_DEV_EVENT_H_

#include <doca_pcc_dev.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================== Event access functions ========================================
// ========================================================================================================

/**
 * @brief For all events, return structure with general information such as event type, subtype, port and flags
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return doca_pcc_dev_event_general_attr_t
 */
DOCA_STABLE
FORCE_INLINE doca_pcc_dev_event_general_attr_t doca_pcc_dev_get_ev_attr(doca_pcc_dev_event_t *event)
{
	uint32_t value = __builtin_bswap32(*(uint32_t *)(&event->ev_attr));

	return *(doca_pcc_dev_event_general_attr_t *)(&value);
}

/**
 * @brief For all events, flow tag to indicate different flows
 *
 * @param[in]  event - pointer to opaque event structs
 *
 * @return 32 bit flow_tag
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_flowtag(doca_pcc_dev_event_t *event)
{
	return __builtin_bswap32(event->flow_tag);
}

/**
 * @brief For all events, serial number of this event
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return 32 bit sn
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_sn(doca_pcc_dev_event_t *event)
{
	return __builtin_bswap32(event->sn);
}

/**
 * @brief For all events, timestamp of this event
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return 32 bit time stamp (1 nSec granularity)
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_timestamp(doca_pcc_dev_event_t *event)
{
	return __builtin_bswap32(event->timestamp);
}

/**
 * @brief For FW events only, three DWORDs of FW data
 *
 * @param[in]  event - pointer to opaque event struct
 * @param[in]  n - dword index 0..2
 *
 * @return 32 bit DWORD information from FW
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_fw_settings(doca_pcc_dev_event_t *event, int n)
{
	return __builtin_bswap32(event->ev_spec_attr.fw_data.data[n]);
}

/**
 * @brief For TX/ACK/NACK/CNP events, first coalesced event timestamp
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return 32 bit first time stamp (1 nSec granularity)
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_roce_first_timestamp(doca_pcc_dev_event_t *event)
{
	return __builtin_bswap32(event->ev_spec_attr.ack_nack_cnp.first_timestamp);
}

/**
 * @brief For TX events only, counters including byte count and packet count
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return pcc_roce_tx_cntrs_t
 */
DOCA_STABLE
FORCE_INLINE doca_pcc_dev_roce_tx_cntrs_t doca_pcc_dev_get_roce_tx_cntrs(doca_pcc_dev_event_t *event)
{
	uint32_t value = __builtin_bswap32(*(uint32_t *)(&event->ev_spec_attr.roce_tx.cntrs));

	return *(doca_pcc_dev_roce_tx_cntrs_t *)(&value);
}

/**
 * @brief For ACK/NACK/CNP events, first coalesced event serial number
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return 32 bit first serial number
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_roce_ack_first_sn(doca_pcc_dev_event_t *event)
{
	return __builtin_bswap32(event->ev_spec_attr.ack_nack_cnp.first_sn);
}

/**
 * @brief For ACK/NACK/CNP events, extra information including number of coalesced events
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return pcc_ack_nack_cnp_extra_t
 */
DOCA_STABLE
FORCE_INLINE doca_pcc_dev_ack_nack_cnp_extra_t doca_pcc_dev_get_ack_nack_cnp_extra(doca_pcc_dev_event_t *event)
{
	uint32_t value = __builtin_bswap32(*(uint32_t *)(&event->ev_spec_attr.ack_nack_cnp.extra));

	return *(doca_pcc_dev_ack_nack_cnp_extra_t *)(&value);
}

/**
 * @brief For RTT events only, the time when RTT request is sent
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return time stamp in 1 nSec
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_rtt_req_send_timestamp(doca_pcc_dev_event_t *event)
{
	return __builtin_bswap32(event->ev_spec_attr.rtt_tstamp.req_send_timestamp);
}

/**
 * @brief For RTT events only, the time when RTT request is received
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return time stamp in 1 nSec
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_rtt_req_recv_timestamp(doca_pcc_dev_event_t *event)
{
	return __builtin_bswap32(event->ev_spec_attr.rtt_tstamp.req_recv_timestamp);
}

/**
 * @brief For RTT events only, the time when RTT response is sent
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return time stamp in 1 nSec
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_rtt_resp_send_timestamp(doca_pcc_dev_event_t *event)
{
	return __builtin_bswap32(event->ev_spec_attr.rtt_tstamp.resp_send_timestamp);
}

/**
 * @brief Returns the user defined event data
 *
 * This function is to be used together with a custom user NP implementation.
 * The user can define a custom data format in the probe response packet.
 *
 * @note that no endianness swap is performed by the function.
 *
 * @param[in]  event - pointer to opaque event struct
 *
 * @return A pointer to the user defined event data
 *
 */
DOCA_EXPERIMENTAL
FORCE_INLINE unsigned char *doca_pcc_dev_get_rtt_raw_data(doca_pcc_dev_event_t *event)
{
	return event->ev_spec_attr.reserved_at_0;
}

/**
 * @brief Returns the user defined event data size in bytes
 */
DOCA_EXPERIMENTAL
FORCE_INLINE size_t doca_pcc_dev_get_rtt_raw_data_size(doca_pcc_dev_event_t *event)
{
	(void)(event);
	return 12;
}

#ifdef __cplusplus
}
#endif

#endif /* DOCA_PCC_DEV_EVENT_H_ */
