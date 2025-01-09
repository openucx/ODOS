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
 * @file doca_buf_inventory.h
 * @page doca buf inventory
 * @defgroup BUF_INVENTORY DOCA Buffer Inventory
 * @ingroup DOCACore
 * The DOCA buffer inventory manages a pool of doca_buf objects. Each buffer obtained from an inventory is a descriptor
 * that points to a memory region from a doca_mmap memory range of the user's choice.
 *
 * @{
 */

#ifndef DOCA_BUF_INVENTORY_H_
#define DOCA_BUF_INVENTORY_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * DOCA Inventory
 ******************************************************************************/

struct doca_buf;
struct doca_mmap;

/**
 * @brief Opaque structure representing a buffer inventory.
 */
struct doca_buf_inventory;

/**
 * @brief Allocates buffer inventory with default/unset attributes.
 *
 * @details The returned object can be manipulated with doca_buf_inventory_property_set() API.
 * Once all required attributes are set, it should be reconfigured and adjusted
 * to meet the setting with doca_buf_inventory_start().
 * See doca_buf_inventory_start for the rest of the details.
 *
 * @param [in] num_elements
 * Initial number of elements in the inventory.
 * @param [out] inventory
 * Buffer inventory with default/unset attributes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_buf_inventory.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_create(size_t num_elements, struct doca_buf_inventory **inventory);

/**
 * @brief Destroy buffer inventory structure.
 *
 * @details Before calling this function all allocated elements should be returned back to the inventory. Destroy
 * implicitly stops the buf inventory.
 * Call doca_buf_dec_refcount to return a buffer to the inventory (until ref count == 0).
 *
 * @param [in] inventory
 * Buffer inventory structure.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_IN_USE - if not all allocated elements had been returned to the inventory.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_destroy(struct doca_buf_inventory *inventory);

/**
 * @brief Start element retrieval from inventory.
 *
 * @details Un-started/stopped buffer inventory rejects all attempts to retrieve element.
 * On first start verifies & finalizes the buffer inventory object configuration.
 *
 * The following become possible only after start:
 * - Retrieval of free elements from the inventory using doca_buf_inventory_buf_get_by_addr().
 * - Duplicating a buffer's content into a buffer allocated from the inventory using doca_buf_inventory_buf_dup().
 *
 * The following are NOT possible after the first time start is called:
 * - Setting the properties of the inventory using doca_buf_inventory_property_set().
 *
 * @param [in] inventory
 * Buffer inventory structure.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_start(struct doca_buf_inventory *inventory);

/**
 * @brief Stop element retrieval from inventory.
 *
 * @details No retrieval of elements with for stopped inventory. Stop does not have to be called before destroy (that
 * implicitly stops the buf inventory).
 * For details see doca_buf_inventory_start().
 *
 * @param [in] inventory
 * Buffer inventory structure.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_stop(struct doca_buf_inventory *inventory);

/*******************************************************************************
 * DOCA buffer, inventory and mmap
 ******************************************************************************/

/**
 * @brief Allocate single element from buffer inventory and point it to the buffer
 * defined by `addr`, `len`, `data` and `data_len` arguments.
 *
 * @details Call doca_buf_dec_refcount to return the buffer to the inventory (until ref count == 0).
 *
 * @param [in] inventory
 * The DOCA Buf inventory. MUST NOT BE NULL AND MUST BE STARTED.
 * @param [in] mmap
 * DOCA memory map structure. MUST NOT BE NULL AND MUST BE STARTED.
 * @param [in] addr
 * The start address of the buffer.
 * @param [in] len
 * The length in bytes of the buffer.
 * @param [in] data
 * The start address of the data inside the buffer.
 * @param [in] data_len
 * The length in bytes of the data.
 * @param [out] buf
 * Doca buf allocated and initialized with args. MUST NOT BE NULL.
 * @note The range [data, data + data_len] must fit within [addr, addr +len]. Otherwise undefined behavior.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - or if there is no suitable memory range for the given address and length.
 * - DOCA_ERROR_NO_MEMORY - if doca_buf_inventory is empty.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_buf_get_by_args(struct doca_buf_inventory *inventory,
						struct doca_mmap *mmap,
						void *addr,
						size_t len,
						void *data,
						size_t data_len,
						struct doca_buf **buf);

/**
 * @brief Allocate single element from buffer inventory and point it to the buffer
 * defined by `addr` & `len` arguments.
 *
 * @details Call doca_buf_dec_refcount to return the buffer to the inventory (until ref count == 0).
 *
 * @param [in] inventory
 * The DOCA Buf inventory. MUST NOT BE NULL AND MUST BE STARTED.
 * @param [in] mmap
 * DOCA memory map structure. MUST NOT BE NULL AND MUST BE STARTED.
 * @param [in] addr
 * The start address of the payload.
 * @param [in] len
 * The length in bytes of the payload.
 * @param [out] buf
 * Doca buf allocated and initialized with args. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - if doca_buf_inventory is empty.
 */
static inline doca_error_t doca_buf_inventory_buf_get_by_addr(struct doca_buf_inventory *inventory,
							      struct doca_mmap *mmap,
							      void *addr,
							      size_t len,
							      struct doca_buf **buf)
{
	return doca_buf_inventory_buf_get_by_args(inventory, mmap, addr, len, addr, 0, buf);
}

/**
 * @brief Allocate single element from buffer inventory and point it to the buffer
 * defined by `data` & `data_len` arguments.
 *
 * @details Call doca_buf_dec_refcount to return the buffer to the inventory (until ref count == 0).
 *
 * @param [in] inventory
 * The DOCA Buf inventory. MUST NOT BE NULL AND MUST BE STARTED.
 * @param [in] mmap
 * DOCA memory map structure. MUST NOT BE NULL AND MUST BE STARTED.
 * @param [in] data
 * The start address of the data inside the buffer.
 * @param [in] data_len
 * The length in bytes of the data.
 * @param [out] buf
 * Doca buf allocated and initialized with args. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - if doca_buf_inventory is empty.
 */
static inline doca_error_t doca_buf_inventory_buf_get_by_data(struct doca_buf_inventory *inventory,
							      struct doca_mmap *mmap,
							      void *data,
							      size_t data_len,
							      struct doca_buf **buf)
{
	return doca_buf_inventory_buf_get_by_args(inventory, mmap, data, data_len, data, data_len, buf);
}

/**
 * @brief Duplicates content of the `buf` argument into element allocated from buffer inventory. (I.e., deep copy).
 *
 * @details Call doca_buf_dec_refcount to return the buffer to the inventory (until ref count == 0).
 *
 * @param [in] inventory
 * Buffer inventory structure that will hold the new doca_buf.
 * @param [in] src_buf
 * The DOCA buf to be duplicated.
 * @param [out] dst_buf
 * A duplicate DOCA Buf.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_BAD_STATE - if src_buf mmap or input inventory unstarted/stopped.
 * - DOCA_ERROR_NOT_PERMITTED - if src_buf is part of a list and it isn't its head.
 * - DOCA_ERROR_NO_MEMORY - if cannot alloc new doca_buf from the given inventory.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_buf_dup(struct doca_buf_inventory *inventory,
					const struct doca_buf *src_buf,
					struct doca_buf **dst_buf);

/*******************************************************************************
 * DOCA Buffer Inventory properties
 ******************************************************************************/

/**
 * @brief Set user_data for a DOCA Buffer Inventory.
 *
 * @param [in] inventory
 * The DOCA Buffer Inventory.
 * @param [in] user_data
 * The user_data to set for inventory.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - if inventory is un-started/stopped.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_set_user_data(struct doca_buf_inventory *inventory, union doca_data user_data);

/**
 * @brief Read the total number of elements in a DOCA Buffer Inventory.
 *
 * @details The total number of elements type: uint32_t.
 *
 * @param [in] inventory
 * The DOCA Buffer Inventory.
 * @param [out] num_of_elements
 * The total number of elements in inventory.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_get_num_elements(const struct doca_buf_inventory *inventory, uint32_t *num_of_elements);

/**
 * @brief Get the total number of free elements in a DOCA Buffer Inventory.
 *
 * @details The total number of free elements type: uint32_t.
 *
 * @param [in] inventory
 * The DOCA Buffer Inventory.
 * @param [out] num_of_free_elements
 * The total number of free elements in inventory.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_get_num_free_elements(const struct doca_buf_inventory *inventory,
						      uint32_t *num_of_free_elements);

/**
 * @brief Get the user_data of a DOCA Buffer Inventory.
 *
 * @details The user_data that was provided to the inventory upon its creation.
 *
 * @param [in] inventory
 * The DOCA Buffer Inventory.
 * @param [out] user_data
 * The user_data of inventory if set, otherwise 0.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_buf_inventory_get_user_data(const struct doca_buf_inventory *inventory, union doca_data *user_data);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_BUF_INVENTORY_H_ */
