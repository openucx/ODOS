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
 * @file doca_buf.h
 * @page doca buf
 * @defgroup DOCACore Core
 * @defgroup BUF DOCA Buffer
 * @ingroup DOCACore
 * The DOCA Buffer is used for reference data. It holds the information on a memory region that belongs to
 * a DOCA memory map, and its descriptor is allocated from DOCA Buffer Inventory.
 *
 * @{
 */

#ifndef DOCA_BUF_H_
#define DOCA_BUF_H_

#include <stdint.h>
#include <stddef.h>

#include <doca_compat.h>
#include <doca_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * DOCA Buffer element
 ******************************************************************************/

/**
 * @brief Opaque structure representing a data buffer, that can be read by registered DOCA devices.
 *
 * The DOCA buffer has an address and length describing a memory region.
 * Each buffer can also point to data within the region using the data address and data length.
 *
 * @code
 *
 * head   -->            +-------------------+
 *                       |                   |
 *                       |                   |
 *                       |                   |
 *                       |                   |
 *                       |                   |
 * data   -->            +-------------------+
 *                       | data              |
 *                       |                   |
 *                       |                   |
 *                       |                   |
 *                       |                   |
 *                       |                   |
 * data + data_len -->   +-------------------+
 *                       |                   |
 *                       |                   |
 *                       |                   |
 *                       |                   |
 * head + len      -->   +-------------------+
 * @endcode
 */
struct doca_buf;

/**
 * @brief Increase the object reference count by 1.
 *
 * @param [in] buf
 * DOCA Buf element.
 * @param [out] refcount
 * The number of references to the object before this operation took place.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NOT_PERMITTED - buf is the next element in some list.
 * - DOCA_ERROR_TOO_BIG - reference count already reached maximum value of UINT16_MAX.
 *
 * @note In case of list all intermediate buffers will always have a refcount of 1.
 * As such the reference count is managed for the head only.
 */
DOCA_STABLE
doca_error_t doca_buf_inc_refcount(struct doca_buf *buf, uint16_t *refcount);

/**
 * @brief Decrease the object reference count by 1, if 0 reached, return the element back to the inventory.
 *
 * @details When refcont 0 reached, all related resources should be released. For example if the element points into
 * some mmap its state will be adjusted accordingly.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 * @param [out] refcount
 * The number of references to the object before this operation took place. Can be NULL.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_NOT_PERMITTED - buf is the next element in some list.
 * - DOCA_ERROR_BAD_STATE - reference count is already 0.
 *
 * @note In case of list if head refcount reaches 0, then all buffers in the list will be released.
 */
DOCA_STABLE
doca_error_t doca_buf_dec_refcount(struct doca_buf *buf, uint16_t *refcount);

/**
 * @brief Get the reference count of the object.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 * @param [out] refcount
 * The number of references to the object. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_get_refcount(const struct doca_buf *buf, uint16_t *refcount);

/*******************************************************************************
 * DOCA Buffer: Data placement
 *******************************************************************************/

/**
 * @brief Get the buffer's length.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 * @param [out] len
 * The length of the buffer. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_get_len(const struct doca_buf *buf, size_t *len);

/**
 * @brief Get the buffer's head.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 * @param [out] head
 * The head of the buffer. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_get_head(const struct doca_buf *buf, void **head);

/**
 * @brief Get buffer's data length.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 * @param [out] data_len
 * The data length of the buffer. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_get_data_len(const struct doca_buf *buf, size_t *data_len);

/**
 * @brief Get the buffer's data.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 * @param [out] data
 * The data of the buffer. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_get_data(const struct doca_buf *buf, void **data);

/**
 * Set data pointer and data length
 *
 * @code
 * Data positioning:
 *
 *         +-----------+-----+-----------------+
 * Before  |           |data |                 |
 *         +-----------+-----+-----------------+
 *
 *                 __data_len__
 *                /            \
 *         +-----+--------------+--------------+
 * After   |     |data          |              |
 *         +-----+--------------+--------------+
 *              /
 *            data
 * @endcode
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 * @param [in] data
 * Data address. MUST NOT BE NULL.
 * @param [in] data_len
 * Data length.
 * @note The range [data, data + data_len] must be in [head, head + len]. Otherwise undefined behavior.
 *
 * @return
 * DOCA_SUCCESS - always
 */
DOCA_STABLE
doca_error_t doca_buf_set_data(struct doca_buf *buf, void *data, size_t data_len);

/**
 * Set data length
 *
 * @code
 * Data positioning:
 *                 __data_len__
 *                /            \
 *         +-----+--------------+--------------+
 * Before  |     |data          |              |
 *         +-----+--------------+--------------+
 *              /
 *            data
 *
 *                 ____data_len____
 *                /                \
 *         +-----+------------------+----------+
 * After   |     |data              |          |
 *         +-----+------------------+----------+
 *              /
 *            data
 * @endcode
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 * @param [in] data_len
 * Data length.
 * @note The range [data, data + data_len] must be in [head, head + len]. Otherwise undefined behavior.
 *
 * @return
 * DOCA_SUCCESS - always
 */
DOCA_EXPERIMENTAL
doca_error_t doca_buf_set_data_len(struct doca_buf *buf, size_t data_len);

/**
 * Reset the data length to 0 (data will still point to the same location)
 *
 * @code
 * Data positioning:
 *
 *                 __data_len__
 *                /            \
 *         +-----+--------------+--------------+
 * Before  |     |data          |              |
 *         +-----+--------------+--------------+
 *              /
 *            data
 *
 *                 data_len = 0
 *                /
 *         +-----+-----------------------------+
 * After   |     |                             |
 *         +-----+-----------------------------+
 *              /
 *            data
 * @endcode
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always
 */
DOCA_STABLE
doca_error_t doca_buf_reset_data_len(struct doca_buf *buf);

/**
 * @brief Get next DOCA Buf in linked list.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 *
 * @param [out] next_buf
 * The next DOCA Buf in the linked list, *next_buf will be NULL if the no other element in the list. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_get_next_in_list(struct doca_buf *buf, struct doca_buf **next_buf);

/**
 * @brief Get last DOCA Buf in linked list.
 *
 * @param [in] buf
 * DOCA Buf element.
 *
 * @param [out] last_buf
 * The last DOCA Buf in the linked list, which may be buf.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 */
DOCA_STABLE
doca_error_t doca_buf_get_last_in_list(struct doca_buf *buf, struct doca_buf **last_buf);

/**
 * @brief Check if provided DOCA Buf is the last element in a linked list.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 *
 * @param [out] is_last
 * 1 if buf is the last element, 0 otherwise. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_is_last_in_list(const struct doca_buf *buf, uint8_t *is_last);

/**
 * @brief Check if provided DOCA Buf is the first element in a linked list.
 *
 * @param [in] buf
 * DOCA Buf element.
 *
 * @param [out] is_first
 * 1 if buf is the first element, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_is_first_in_list(const struct doca_buf *buf, uint8_t *is_first);

/**
 * @brief Check if provided DOCA Buf is a linked list.
 *
 * @param [in] buf
 * DOCA Buf element. MUST NOT BE NULL.
 *
 * @param [out] is_in_list
 * 1 if buf is part of a linked list, 0 if it is not. MUST NOT BE NULL.
 *
 * @return
 * DOCA_SUCCESS - always.
 */
DOCA_STABLE
doca_error_t doca_buf_is_in_list(const struct doca_buf *buf, uint8_t *is_in_list);

/**
 * @brief Get the number of the elements in list.
 *
 * @param [in] buf
 * DOCA Buf element. Buf must be a head of a list.
 *
 * @param [out] num_elements
 * Number of elements in list.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_PERMITTED - if the buffer is not a head of a list.
 */
DOCA_STABLE
doca_error_t doca_buf_get_list_len(const struct doca_buf *buf, uint32_t *num_elements);

/**
 * @brief Append list2 to list1.
 *
 * @code
 *
 * Before:
 *           +----+  +----+  +----+
 * list1 ->  |1   |->|2   |->|3   |
 *           +----+  +----+  +----+
 *
 *           +----+  +----+
 * list2 ->  |4   |->|5   |
 *           +----+  +----+
 *
 * After:
 *
 *           +----+  +----+  +----+  +----+  +----+
 * list1 ->  |1   |->|2   |->|3   |->|4   |->|5   |
 *           +----+  +----+  +----+  +----+  +----+
 *                                  /
 *                               list2
 * @endcode
 *
 * @param [in] list1
 * DOCA Buf representing list1. MUST NOT BE NULL AND MUST BE HEAD OF LIST.
 *
 * @param [in] list2
 * DOCA Buf representing list2. MUST NOT BE NULL AND MUST BE HEAD OF LIST.
 * must have a refcount of 1
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_NOT_PERMITTED - if list2 has a reference count that is not 1
 */
DOCA_STABLE
doca_error_t doca_buf_chain_list(struct doca_buf *list1, struct doca_buf *list2);

/**
 * @brief Separate list2 from list1.
 *
 * @code
 *
 * Before:
 *           +----+  +----+  +----+  +----+  +----+
 * list1 ->  |1   |->|2   |->|3   |->|4   |->|5   |
 *           +----+  +----+  +----+  +----+  +----+
 *                                  /
 *                               list2
 *
 * After:
 *           +----+  +----+  +----+
 * list1 ->  |1   |->|2   |->|3   |
 *           +----+  +----+  +----+
 *
 *           +----+  +----+
 * list2 ->  |4   |->|5   |
 *           +----+  +----+
 * @endcode
 *
 * @param [in] list1
 * DOCA Buf representing list1. MUST NOT BE NULL.
 * @param [in] list2
 * DOCA Buf representing list2, list2 should be contained in list1.
 * list2 must be different from list1. MUST NOT BE NULL
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if list2 is not part of list1.
 *
 * @note reference count of list2 will always be 1 after unchaining
 */
DOCA_STABLE
doca_error_t doca_buf_unchain_list(struct doca_buf *list1, struct doca_buf *list2);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_BUF_H_ */
