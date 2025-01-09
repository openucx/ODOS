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
 * @file doca_rdma_bridge.h
 * @page doca rdma bridge
 * @defgroup DOCA_RDMA_BRIDGE DOCA RDMA BRIDGE
 * @ingroup DOCACore
 *
 * DOCA RDMA bridge.
 *
 * @{
 */

#ifndef DOCA_RDMA_BRIDGE_H_
#define DOCA_RDMA_BRIDGE_H_

#include <stdint.h>

#ifdef __linux__
#include <infiniband/verbs.h>
#else /* Windows */
/**
 * @brief Maximum possible value for errno, to use by encoding error in a pointer address
 */
#define UM_MAX_ERRNO 999
/**
 * @brief Encoding of the ENOMEM error
 */
#define UM_ENOMEM 12
/**
 * @brief Defining __DEVX_IFC_H__ discards the definitions in \\MLNX_WinOF2_DevX_SDK\\inc\\mlx5_ifc_devx.h that
 * is included by the WinOF header files below.
 */
#define __DEVX_IFC_H__

typedef uint16_t __be16; /**< Unsigned 16-bit integer in Big Endian */
typedef uint32_t __be32; /**< Unsigned 32-bit integer in Big Endian */
typedef uint64_t __be64; /**< Unsigned 64-bit integer in Big Endian */
typedef uint16_t u16;	 /**< Unsigned 16-bit integer */
typedef uint32_t u32;	 /**< Unsigned 32-bit integer */
typedef uint64_t u64;	 /**< Unsigned 64-bit integer */

#include <winsock2.h>
#include <windows.h>
#include <mlx5verbs.h>
#endif /* __linux__ */

#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_dev;
struct doca_buf;

/**
 * @brief Get the protection domain associated with a DOCA device.
 *
 * @param [in] dev
 * DOCA device to get the pd from.
 * @param [out] pd
 * The protection-domain associated with the given DOCA device.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_BAD_STATE - in case the device's pd is not valid (bad state)
 */
DOCA_STABLE
doca_error_t doca_rdma_bridge_get_dev_pd(const struct doca_dev *dev, struct ibv_pd **pd);

/**
 * @brief Open a DOCA device using an ibv_pd
 *
 * Always prefer using a DOCA device obtained from doca_devinfo_create_list()
 * This call will fail if PD was acquired by DOCA through doca_devinfo_create_list()
 * and then doca_rdma_bridge_get_dev_pd()
 *
 * This API should be used only to bridge between rdma-core and DOCA, to allow them to share memory registrations
 * E.g., application already has logic that utilizes an ibv_pd, to read and write memory using RDMA, and wants to
 * extend the logic by using libraries in DOCA, but such libraries will require a doca_dev and doca_buf instead of an
 * ibv_pd and mkey in order to read write same memory. Then this method can be used to get a doca_dev that can
 * be added to a doca_mmap, such that any doca_buf created from the doca_mmap can yield mkeys that are associated
 * with the same ibv_pd using doca_rdma_bridge_get_buf_mkey()
 *
 * For reference:
 * doca_dev - is parallel to an ibv_pd
 * doca_buf - is parallel to an ibv_mr registered on multiple devices
 * doca_mmap - is parallel to creating an ibv_mr for multiple devices
 *
 * The only APIs that are supported for the newly created device:
 * - doca_dev_close()
 * - doca_rdma_bridge_get_buf_mkey()
 * - doca_rdma_bridge_get_dev_pd()
 *
 * @param [in] pd
 * A protection domain that is not associated with any DOCA device
 * @param [out] dev
 * A newly created DOCA device with same protection domain as 'pd'
 *
 * @return
 * DOCA_SUCCESS - in case of success
 */
DOCA_STABLE
doca_error_t doca_rdma_bridge_open_dev_from_pd(struct ibv_pd *pd, struct doca_dev **dev);

/**
 * @brief Get lkey with doca_access_flag access for a DOCA buffer of a DOCA device.
 *
 * @param [in] buf
 * The DOCA buffer to get lkey for. MUST NOT BE NULL.
 *
 * @param [in] dev
 * The DOCA device to get lkey for. MUST NOT BE NULL.
 *
 * @param [out] mkey
 * The returned MKey. MUST NOT BE NULL.
 *
 * @note Access of mkey is defined by the mmap where buf was created.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if cannot find mkey by the given device.
 * - DOCA_ERROR_NOT_SUPPORTED - if the given access flags is not supported
 */
DOCA_STABLE
doca_error_t doca_rdma_bridge_get_buf_mkey(const struct doca_buf *buf, struct doca_dev *dev, uint32_t *mkey);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_RDMA_BRIDGE_H_ */
