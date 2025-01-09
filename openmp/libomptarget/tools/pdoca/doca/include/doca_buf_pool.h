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
 * @file doca_buf_pool.h
 * @page doca buf pool
 * @defgroup DOCACore Core
 * @defgroup BUF_POOL DOCA Buffer Pool
 * @ingroup DOCACore
 * The DOCA Buffer Pool is a pool of doca_buf objects, such that each doca_buf is set with a permanent,
 * fixed size memory buffer, right from creation and till destruction, which allows immediate allocation of
 * doca_buf objects.
 *
 * @code
 * Basic structure example of a Buffer Pool (after creation):
 *
 *                                      +------------------------------------------+
 *                                      |               memory range               |
 *              +-----------+           | +--------+   +--------+       +--------+ |
 *              | doca_mmap |-----------| | buffer |   | buffer |       | buffer | |
 *              +-----------+           | +--------+   +--------+ ..... +--------+ |
 *                                      |  \            \                \         |
 *                                      +------------------------------------------+
 *                                           \            \                \
 *                                            \            \                \
 *                                      +--------------------------------------------+
 *                                      |      |            |                |       |
 *              +---------------+       | +----------+ +----------+     +----------+ |
 *              | doca_buf_pool |-------| | doca_buf | | doca_buf |     | doca_buf | |
 *              +---------------+       | +----------+ +----------+ ....+----------+ |
 *                                      +--------------------------------------------+
 * @endcode
 *
 * @{
 */

#ifndef DOCA_BUF_POOL_H_
#define DOCA_BUF_POOL_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_mmap;
struct doca_buf;
struct doca_buf_pool;

/**
 * @brief Allocates a buffer pool and sets it with doca_buf objects.
 *
 * @param [in] num_elements
 * Number of elements in the buffer pool (must be > 0).
 * @param [in] element_size
 * Size of a single element (must be > 0).
 * @param [in] mmap
 * The mmap managing the memory chunk. Must be populated with memory chunk.
 * @param [out] buf_pool
 * The newly created DOCA buf_pool.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate a doca_buf_pool.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_create(size_t num_elements,
				  size_t element_size,
				  const struct doca_mmap *mmap,
				  struct doca_buf_pool **buf_pool);

/**
 * @brief Destroy a buffer pool structure.
 *
 * @details Destroy implicitly stops the buf pool.
 *
 * @note Before Calling this method, all allocated doca_bufs should be returned back to the buffer pool.
 * Call doca_buf_dec_refcount to return a buffer to the pool (until ref count == 0).
 *
 * @param [in] buf_pool
 * The DOCA buf_pool to destroy.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_IN_USE - if not all allocated doca_bufs had been returned to buf_pool.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_destroy(struct doca_buf_pool *buf_pool);

/**
 * @brief Start a DOCA buffer pool.
 *
 * @details This method enables the allocation of doca_bufs using doca_buf_pool_buf_alloc().
 * Before calling this method, the mmap with which the buffer pool was created must be started.
 *
 * The following become possible only after start:
 * - Allocating doca_bufs using doca_buf_pool_buf_alloc().
 *
 * The following are NOT possible while buf_pool is started:
 * - Setting properties of the buffer pool with doca_buf_pool_set_*.
 *
 * @param [in] buf_pool
 * The DOCA buf_pool to start.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_INITIALIZATION - if the mmap's memory range is smaller than the
 * 				 required size according to the buf_pool's properties.
 * - DOCA_ERROR_BAD_STATE - if the mmap with which buf_pool was created is not started.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_start(struct doca_buf_pool *buf_pool);

/**
 * @brief Stop a started DOCA buffer pool.
 *
 * @details This method disables the allocation of doca_bufs, and re-enables Setting properties of
 * the buffer pool with doca_buf_pool_set_*.
 * Before Calling this method, all allocated doca_bufs should be returned back to the buffer pool.
 * Stop does not have to be called before destroy (that implicitly stops the buf pool).
 *
 * @param [in] buf_pool
 * The DOCA buf_pool to stop.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_IN_USE - if not all allocated doca_bufs had been returned to buf_pool.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_stop(struct doca_buf_pool *buf_pool);

/**
 * @brief This method acquires a doca_buf from a DOCA buffer pool, pointing to an allocated empty buffer.
 *
 * @details Call doca_buf_dec_refcount to return the buffer to the pool (until ref count == 0).
 *
 * @param [in] buf_pool
 * The DOCA buf_pool from which to acquire a doca_buf, that was set to point to a memory buffer at
 * doca_buf_pool_create().
 * @param [out] buf
 * Pointer to the allocated doca_buf.
 *
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_BAD_STATE - if buf_pool is un-started/stopped.
 * - DOCA_ERROR_EMPTY - if the buf_pool is empty (all doca_bufs are already allocated).
 */
DOCA_STABLE
doca_error_t doca_buf_pool_buf_alloc(struct doca_buf_pool *buf_pool, struct doca_buf **buf);

/**
 * @brief Set user_data for a DOCA buffer pool.
 *
 * @param [in] buf_pool
 * The DOCA buf_pool.
 * @param [in] user_data
 * The user_data to set for buf_pool.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - if buf_pool is started.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_set_user_data(struct doca_buf_pool *buf_pool, union doca_data user_data);

/**
 * @brief Set an alignment for each element in a DOCA buffer pool.
 *
 * @param [in] buf_pool
 * The DOCA buf_pool.
 * @param [in] element_alignment
 * The element alignment to set for buf_pool (minimal value is 1, must be a power of 2).
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - if buf_pool is started.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_set_element_alignment(struct doca_buf_pool *buf_pool, size_t element_alignment);

/**
 * @brief Get the number of elements that was set in the creation of a DOCA buffer pool.
 *
 * @param [in] buf_pool
 * The DOCA buf_pool.
 * @param [out] num_of_elements
 * The number of elements that was set in the creation of buf_pool.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_get_num_elements(const struct doca_buf_pool *buf_pool, uint32_t *num_of_elements);

/**
 * @brief Get the total number of free elements available for allocation in a DOCA buffer pool.
 *
 * @param [in] buf_pool
 * The DOCA buf_pool.
 * @param [out] num_of_free_elements
 * The total number of free elements in buf_pool.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_get_num_free_elements(const struct doca_buf_pool *buf_pool, uint32_t *num_of_free_elements);

/**
 * @brief Get the user_data of a DOCA buffer pool.
 *
 * @note - Unless set with doca_buf_pool_set_user_data(), user data is 0 by default.
 *
 * @param [in] buf_pool
 * The DOCA buf_pool.
 * @param [out] user_data
 * The user_data of buf_pool if set, otherwise 0.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_get_user_data(const struct doca_buf_pool *buf_pool, union doca_data *user_data);

/**
 * @brief Get the element alignment of a DOCA buffer pool.
 *
 * @note - Unless set with doca_buf_pool_set_element_alignment(),
 * element alignment is 1 by default (meaning no alignment).
 *
 * @param [in] buf_pool
 * The DOCA buf_pool.
 * @param [out] element_alignment
 * The element alignment of buf_pool.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_buf_pool_get_element_alignment(const struct doca_buf_pool *buf_pool, size_t *element_alignment);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_BUF_POOL_H_ */
