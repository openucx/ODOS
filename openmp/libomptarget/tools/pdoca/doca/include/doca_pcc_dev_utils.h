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

#ifndef DOCA_PCC_DEV_UTILS_H_
#define DOCA_PCC_DEV_UTILS_H_

/**
 * @brief declares that we are compiling for the DPA Device
 *
 * @note Must be defined before the first API use/include of DOCA
 */
#define DOCA_DPA_DEVICE

#include <dpaintrin.h>
#include <doca_pcc_dev_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief fixed point 16b reciprocal
 */
#define doca_pcc_dev_fxp_recip(a_fp) __dpa_fxp_rcp(a_fp)

/**
 * @brief fixed point 16b power of 2
 */
#define doca_pcc_dev_fxp_power2(a_fp) __dpa_fxp_pow2(a_fp)

/**
 * @brief fixed point 16b log 2
 */
#define doca_pcc_dev_fxp_log2(a_fp) __dpa_fxp_log2(a_fp)

/**
 * @brief 32b find last set
 */
#define doca_pcc_dev_fls(a) (32 - __builtin_clz(a))

/**
 * @brief mult wrapper
 */
#define doca_pcc_dev_mult(a, b) ((uint64_t)(a) * (uint64_t)(b))

/**
 * @brief fixed point 16b mult
 */
#define doca_pcc_dev_fxp_mult(a, b) ((uint32_t)((doca_pcc_dev_mult((a), (b)) >> 16) & 0xffffffff))

/**
 * @brief fence all
 */
#define doca_pcc_dev_fence_all() __dpa_thread_fence(__DPA_SYSTEM, __DPA_RW, __DPA_RW)

/**
 * @brief fence memory operations
 */
#define doca_pcc_dev_fence_memory() __dpa_thread_fence(__DPA_MEMORY, __DPA_RW, __DPA_RW)

/**
 * @brief fence io operations
 */
#define doca_pcc_dev_fence_io() __dpa_thread_fence(__DPA_MMIO, __DPA_RW, __DPA_RW)

/**
 * @brief fence w/r
 */
#define doca_pcc_dev_fence_w_r() __dpa_thread_fence(__DPA_MEMORY, __DPA_W, __DPA_R)

/**
 * @brief return 1usec tick count
 */
#define doca_pcc_dev_get_thread_time() __dpa_thread_time()

/**
 * @brief Core timer access (elapsed time in uSec) function 32 bits
 *
 * @return time in uSec.
 */
DOCA_STABLE
FORCE_INLINE uint32_t doca_pcc_dev_get_timer_lo(void)
{
	return (uint32_t)(doca_pcc_dev_get_thread_time() & 0xffffffff);
}

/**
 * @brief Core timer access (elapsed time in uSec) function 64 bits
 *
 * @return time in uSec.
 */
DOCA_STABLE
FORCE_INLINE uint64_t doca_pcc_dev_get_timer(void)
{
	return doca_pcc_dev_get_thread_time();
}

#ifdef __cplusplus
}
#endif

#endif /* DOCA_PCC_DEV_UTILS_H_ */

/** @} */
