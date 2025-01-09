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

#ifndef DOCA_PCC_DEV_SERVICES_H_
#define DOCA_PCC_DEV_SERVICES_H_

/**
 * @brief declares that we are compiling for the DPA Device
 *
 * @note Must be defined before the first API use/include of DOCA
 */
#define DOCA_DPA_DEVICE

#include <doca_pcc_dev_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief User callback to process a request from host to device
 *
 * This function is called when the host calls doca_pcc_mailbox_send().
 * The mailbox format is defined by the user.
 *
 * @note: Implementation of this function is optional.
 *
 * @param[in] request - a buffer storing the request. This buffer is filled by the host side
 * @param[in] request_size - request buffer size
 * @param[in] max_response_size - max response buffer size set on host side
 * @param[out] response - a buffer storing the response. This buffer is filled by the device side
 * @param[out] response_size - response size
 *
 * @return - @see doca_pcc_dev_error_t
 *
 */
DOCA_EXPERIMENTAL
doca_pcc_dev_error_t doca_pcc_dev_user_mailbox_handle(void *request,
						      uint32_t request_size,
						      uint32_t max_response_size,
						      void *response,
						      uint32_t *response_size) __attribute__((weak));

/**
 * @brief Print to Host
 *
 * This function prints from device to host's standard output stream.
 * Multiple threads may call this routine simultaneously. Printing is a convenience service, and due to limited
 * buffering on the host, not all print statements may appear on the host
 *
 * @param[in]  format - Format string that contains the text to be written to stdout (same as from regular printf)
 */
DOCA_STABLE
void doca_pcc_dev_printf(const char *format, ...) __attribute__((format(printf, 1, 2)));

/**
 * @brief Creates trace message entry with 3 arguments
 *
 * @param[in] format_id -the template format id to print message accordingly
 * @param[in] arg1 - argument #1 to format into the template
 * @param[in] arg2 - argument #2 to format into the template
 * @param[in] arg3 - argument #3 to format into the template
 * @param[in] arg4 - argument #4 to format into the template
 * @param[in] arg5 - argument #5 to format into the template
 *
 * @return void.
 *
 */
DOCA_STABLE
void doca_pcc_dev_trace_5(int format_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);

/**
 * @brief Flush the trace message buffer to Host
 *
 * As soon as a buffer is fully occupied it is internally sent to host, however
 * user can ask partially occupied buffer to be sent to host. Its intended use
 * is at end of run to flush whatever messages left.
 *
 * @note: Frequent call to this API might cause performance issues.
 *
 * @return void.
 *
 */
DOCA_STABLE
void doca_pcc_dev_trace_flush(void);

/**
 * \brief Obtains the thread rank
 *
 * Retrieves the thread rank from the group of threads that run PCC.
 * The function returns a number in {0..N-1}, where N is the number of threads requested to run PCC
 *
 * @return
 * Returns the thread rank.
 */
DOCA_EXPERIMENTAL
unsigned int doca_pcc_dev_thread_rank(void);

#ifdef __cplusplus
}
#endif

#endif /* DOCA_PCC_DEV_SERVICES_H_ */

/** @} */
