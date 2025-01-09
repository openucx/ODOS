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
 * @file doca_dpdk.h
 * @page doca dpdk
 * @defgroup DOCA_DPDK DOCA DPDK
 * @ingroup DOCACore
 *
 * DOCA API for integration with DPDK.
 *
 * @{
 */

#ifndef DOCA_DPDK_H_
#define DOCA_DPDK_H_

#include <doca_error.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_dev;
struct doca_devinfo;
struct doca_buf_inventory;
struct doca_buf;

struct rte_mempool;
struct rte_mbuf;

struct doca_dpdk_mempool;

/**
 * @brief Check if the device supports representors for port_probe.
 *
 * @note This function should be used before calling doca_dpdk_port_probe() in case representores are required in
 * devargs.
 * @note This function should be called with root privileges.
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [out] is_rep_port_supported
 * 1 if the device supports representors for port_probe, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_cap_is_rep_port_supported(const struct doca_devinfo *devinfo, uint8_t *is_rep_port_supported);

/**
 * @brief Attach a DPDK port specified by DOCA device.
 *
 * Thread unsafe API.
 *
 * It's the user responsibility to set the DPDK EAL initialization to
 * skip probing the PCI device associated with the given DOCA device
 * to prevent EAL from using it.
 *
 * No initialization is done for the probed PDPK port and the port is not started.
 *
 *
 * @param [in] dev
 * DOCA device to attach PDK port for.
 * @param [in] devargs
 * DPDK devargs style - must NOT contains the device's PCI address ([domain:]bus:devid.func).
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_DRIVER - in case of DPDK error during DPDK port attach.
 * - DOCA_ERROR_NO_MEMORY - in case of memory allocation failure.
 */

DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_port_probe(struct doca_dev *dev, const char *devargs);

/**
 * @brief Return the first DPDK port id associated to a DOCA device.
 * Assumption is that the doca device that was probed using doca_dpdk_port_probe().
 *
 * @param [in] dev
 * DOCA device object
 * @param [out] port_id
 * DPDK port id
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NOT_FOUND - No DPDK port matches the DOCA device.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_get_first_port_id(const struct doca_dev *dev, uint16_t *port_id);

/**
 * @brief Return the DOCA device associated with a DPDK port.
 *
 * @param [in] port_id
 * The DPDK port identifier to get the associated DOCA device for.
 * @param [out] dev
 * The DPDK DOCA device associated with the given DPDK port identifier.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NOT_FOUND - in case there is no such DPDK port associated with a DOCA device.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_port_as_dev(uint16_t port_id, struct doca_dev **dev);

/**
 * @brief Create a DOCA DPDK memory pool, with ability to convert rte_mbuf to doca_buf
 * Expected flow is as follows:
 * Control path:
 *   // Create the memory pool based on a DPDK memory pool
 *   doca_dpdk_mempool_create()
 *   // Add 1 or more DOCA devices
 *   doca_dpdk_mempool_dev_add()
 *   // Set permission level across all devices (default=LOCAL_READ/WRITE)
 *   doca_dpdk_mempool_set_permissions()
 *   // Start the pool
 *   doca_dpdk_mempool_start()
 *
 * Data path:
 *   // Convert DPDK mbuf to DOCA buf
 *   doca_dpdk_mempool_mbuf_to_buf()
 *   // Optionally release DPDK mbuf back to the DPDK pool in case it is no longer needed
 *   rte_pktmbuf_free()
 *   // Release the doca_buf once finished with it
 *   doca_buf_refcnt_rm()
 *
 * @param [in] mbuf_pool
 * A DPDK pool of mbufs, created with rte_pktmbuf_pool_create*()
 * @param [out] mempool_out
 * The newly created DOCA DPDK memory pool in case of success
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_mempool_create(const struct rte_mempool *mbuf_pool, struct doca_dpdk_mempool **mempool_out);

/**
 * @brief Destroy a DOCA DPDK memory pool
 * Before destroying need to make sure that all buffers that were acquired using doca_dpdk_mempool_mbuf_to_buf()
 * have been released
 * This must be called before destroying the originating DPDK mempool
 * @note: Once destroyed the originating DPDK memory pool, and any allocated RTE mbuf are not affected
 *
 * @param [in] mempool
 * The DOCA DPDK memory pool to destroy
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_IN_USE - at least 1 DOCA buf has been acquired and still not released
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_mempool_destroy(struct doca_dpdk_mempool *mempool);

/**
 * @brief Add a DOCA device to the mempool
 * This allows the DOCA bufs that are retrieved from the pool to be compatible with other DOCA libraries,
 * that use the DOCA device
 * @note Once device has been added it can't be removed. Only option is to destroy the doca_dpdk_mempool
 *
 * @param [in] mempool
 * The DOCA DPDK memory pool to add the device to
 * @param [in] dev
 * A DOCA device instance
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NO_MEMORY - out of memory.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_mempool_dev_add(struct doca_dpdk_mempool *mempool, struct doca_dev *dev);

/**
 * @brief Set the read/write permissions of the memory for devices
 * Default: DOCA_ACCESS_FLAG_LOCAL_READ_WRITE
 * Setting the permission will set the access that the added devices have over the memory of the DOCA buffers
 *
 * @param [in] mempool
 * The DOCA DPDK memory pool
 * @param [in] access_mask
 * The access permissions - see 'enum doca_access_flag'
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input or bad access flag combination.
 * @note: setting DOCA_ACCESS_FLAG_DPU_* flags is invalid
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_mempool_set_permissions(struct doca_dpdk_mempool *mempool, uint32_t access_mask);

/**
 * @brief Start the DOCA DPDK memory pool
 * Operations that must be done before start:
 *   Adding at least 1 device - doca_dpdk_mempool_dev_add()
 *   Optionally, setting the permission level - doca_dpdk_mempool_set_permissions()
 * Operations that are allowed after start:
 *   Acquiring a matching doca_buf from an rte_mbuf - doca_dpdk_mempool_mbuf_to_buf()
 *   Destroying the DOCA DPDK memory pool - doca_dpdk_mempool_destroy()
 *
 * @param [in] mempool
 * The DOCA DPDK memory pool to add the device to
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NO_MEMORY - out of memory.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_mempool_start(struct doca_dpdk_mempool *mempool);

/**
 * @brief Acquire a doca_buf based on an rte_mbuf
 * The acquired doca_buf attempts to be as similar as possible to the rte_mbuf
 * Level of support:
 * - After acquiring the buffer the refcount of the mbuf is increased
 * - In case mbuf is indirect refcount of the direct buffer is increased instead and metadata of the indirect
 * - mbuf is used where metadata refers to the mbuf's data offset, data length, and next pointer
 * - In case the acquired doca_buf is duplicated, then the duplication process will increase the refcount of the direct
 *   mbufs as well
 * Limitations:
 * - The mbuf must represent memory from the originating rte_mempool associated with this mempool
 *   and mbuf cannot be created from external memory
 * - Any changes made to the rte_mbuf after the acquisition will not affect the doca_buf
 * - Any changes made to the doca_buf after acquisition will not affect the rte_mbuf
 *
 * @code
 * rte_mbuf chain before calling this method:
 *
 *          buf_addr              __data_len__
 *                  \            /            \
 *                   +----------+--------------+----------+         +----------+--------------+----------+
 *                   | headroom |     data     | tailroom |  ---->  | headroom |     data     | tailroom |
 *                   +----------+--------------+----------+         +----------+--------------+----------+
 *
 * doca_buf created after calling this method:
 *
 *              head              __data_len__
 *                  \            /            \
 *                   +----------+--------------+----------+         +----------+--------------+----------+
 *                   |          |     data     |          |  ---->  |          |     data     |          |
 *                   +----------+--------------+----------+         +----------+--------------+----------+
 * @endcode
 *
 * @note: Destroying the doca_buf using 'doca_buf_dec_refcount()' will call 'rte_pktmbuf_free_seg()' on each direct mbuf
 *
 * @param [in] mempool
 * The DOCA DPDK memory pool created using the rte_mempool that created the rte_mbuf
 * @param [in] inventory
 * A DOCA Buffer Inventory to be used for allocating the doca_buf. Must be started and have enough space
 * @param [in] mbuf
 * A DPDK buffer that references memory that is within the RTE mempool associated with the DOCA DPDK mempool
 * @param [out] buf
 * A DOCA buffer that references the same memory as the provided mbuf
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NO_MEMORY - The inventory does not have enough free elements.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_dpdk_mempool_mbuf_to_buf(struct doca_dpdk_mempool *mempool,
					   struct doca_buf_inventory *inventory,
					   struct rte_mbuf *mbuf,
					   struct doca_buf **buf);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DPDK_H_ */
