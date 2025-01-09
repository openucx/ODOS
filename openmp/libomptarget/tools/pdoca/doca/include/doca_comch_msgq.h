/*
 * Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_comch_msgq.h
 * @page comm_channel_v2
 * @defgroup DOCA_COMCH comch
 *
 * DOCA Communication Channel MsgQ library lets you set a direct communication channel between Host/DPU and DPA.
 * The channel is not part of the TCP/IP stack.
 * Please follow the programmer guide for usage instructions.
 *
 * @{
 */
#ifndef DOCA_COMCH_MSGQ_H_
#define DOCA_COMCH_MSGQ_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_compat.h>
#include <doca_error.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_comch_msgq;
struct doca_comch_producer;
struct doca_comch_consumer;

struct doca_dev;
struct doca_dpa;

/*********************************************************************************************************************
 * DOCA Comch MsgQ
 *********************************************************************************************************************/

/**
 * @brief Create a DOCA Comch MsgQ instance.
 *
 * @param [in] dev
 * A DOCA device that will be used to send/receive the messages.
 * @param [out] msgq
 * Pointer to pointer to be set to created doca_comch_msgq instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_create(struct doca_dev *dev, struct doca_comch_msgq **msgq);

/**
 * @brief Destroy a DOCA Comch MsgQ instance.
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to destroy.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_destroy(struct doca_comch_msgq *msgq);

/**
 * @brief Set consumers to DPA. All consumers on this MsgQ will be created for DPA.
 *
 * This configuration specifies that all consumers on this MsgQ will be created and used on this DPA instance.
 * By default consumers are set on Host (or DPU).
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to modify.
 * @param [in] dpa
 * The DPA context that will use the consumers. Must be on same device as the MsgQ.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_set_dpa_consumer(struct doca_comch_msgq *msgq, struct doca_dpa *dpa);

/**
 * @brief Set the maximal number of consumers that can be created using this MsgQ.
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to modify.
 * @param [in] max_num_consumers
 * The maximal number of consumers that can be created.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_set_max_num_consumers(struct doca_comch_msgq *msgq, uint32_t max_num_consumers);

/**
 * @brief Set producers to DPA. All producers on this MsgQ will be created for DPA.
 *
 * This configuration specifies that all producers on this MsgQ will be created and used on this DPA instance.
 * By default producers are set on Host (or DPU).
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to modify.
 * @param [in] dpa
 * The DPA context that will use the producers. Must be on same device as the MsgQ.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_set_dpa_producer(struct doca_comch_msgq *msgq, struct doca_dpa *dpa);

/**
 * @brief Set the maximal number of producers that can be created using this MsgQ.
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to modify.
 * @param [in] max_num_producers
 * The maximal number of producers that can be created.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_set_max_num_producers(struct doca_comch_msgq *msgq, uint32_t max_num_producers);

/**
 * @brief Start DOCA Comch MsgQ.
 *
 * @details On start verifies and finalizes the MsgQ configuration.
 *
 * The following is possible for started MsgQ:
 * - Creating consumers and producers.
 *
 * The following is NOT possible while completion context is started:
 * - Setting the properties of the MsgQ
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to start.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_start(struct doca_comch_msgq *msgq);

/**
 * @brief Stop DOCA Comch MsgQ.
 *
 * @details On stop prevents execution of different operations and allows operations that were available before start.
 * For details see doca_comch_msgq_start(). MsgQ can't be stopped while there are DOCA Comch consumers/producers
 * associated with it.
 *
 * The following is possible for stopped MsgQ:
 * - Setting the properties of the MsgQ
 *
 * The following is NOT possible while MsgQ is stopped:
 * - Creating DOCA Comch consumers or producers from MsgQ.
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to stop.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_stop(struct doca_comch_msgq *msgq);

/**
 * @brief Create a DOCA Comch consumer instance.
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to be used for creating the consumer
 * @param [out] consumer
 * Pointer to pointer to be set to created doca_comch_consumer instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_consumer_create(struct doca_comch_msgq *msgq, struct doca_comch_consumer **consumer);

/**
 * @brief Create a DOCA Comch producer instance.
 *
 * @param [in] msgq
 * The DOCA Comch MsgQ to be used for creating the producer
 * @param [out] producer
 * Pointer to pointer to be set to created doca_comch_producer instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_comch_msgq_producer_create(struct doca_comch_msgq *msgq, struct doca_comch_producer **producer);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_COMCH_MSGQ_H_ */
