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
 * @file doca_dpa_dev_buf.h
 * @page doca dpa buf
 * @defgroup DPA_BUF DOCA DPA buf
 * @ingroup DPA_DEVICE
 * DOCA DPA buffer
 * @{
 */

#ifndef DOCA_DPA_DEV_BUF_H_
#define DOCA_DPA_DEV_BUF_H_

#include <doca_dpa_dev.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DPA mmap handle type definition
 */
__dpa_global__ typedef uint32_t doca_dpa_dev_mmap_t;

/**
 * @brief DPA buffer handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_buf_t;

/**
 * @brief DPA buffer array handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_buf_arr_t;

/**
 * @brief doca dpa device buf declaration
 *
 * User of this struct should relate to it as an opaque and not access its fields, but rather use relevant API for it
 */
struct doca_dpa_dev_buf {
	uintptr_t addr;		    /**< address held by doca dpa device buf */
	uint64_t len;		    /**< length of doca dpa device buf */
	doca_dpa_dev_mmap_t mmap;   /**< the mmap associated with the doca dpa device buf */
	unsigned char reserved[12]; /**< reserved field */
} __attribute__((__packed__));	    /**< packed */

/**
 * @brief doca dpa device buf array declaration
 *
 * User of this struct should relate to it as an opaque and not access its fields, but rather use relevant API for it
 */
struct doca_dpa_dev_buf_arr {
	struct doca_dpa_dev_buf *bufs;	    /**< doca dpa device bufs */
	unsigned char reserved[20];	    /**< reserved field */
} __attribute__((__packed__, aligned(64))); /**< packed and aligned */

/**
 * @brief Get DPA buffer handle from a DPA buffer array handle
 *
 * @param[in] buf_arr - DOCA DPA device buf array handle
 * @param[in] buf_idx - DOCA DPA buffer index
 *
 * @return
 * Handle to DPA buffer
 */
DOCA_EXPERIMENTAL
__forceinline doca_dpa_dev_buf_t doca_dpa_dev_buf_array_get_buf(doca_dpa_dev_buf_arr_t buf_arr, const uint64_t buf_idx)
{
	struct doca_dpa_dev_buf_arr *dev_buf_arr = (struct doca_dpa_dev_buf_arr *)(buf_arr);
	doca_dpa_dev_buf_t dev_buf = (doca_dpa_dev_buf_t) & (dev_buf_arr->bufs[buf_idx]);

	return dev_buf;
}

/**
 * @brief Set the address of a DPA buffer handle
 *
 * @param[in] buf - DOCA DPA device buf handle
 *
 * @param[in] addr - The address to set
 *
 */
DOCA_EXPERIMENTAL
__forceinline void doca_dpa_dev_buf_set_addr(doca_dpa_dev_buf_t buf, uintptr_t addr)
{
	((struct doca_dpa_dev_buf *)buf)->addr = addr;
}

/**
 * @brief Set the length of a DPA buffer handle
 *
 * @param[in] buf - DOCA DPA device buf handle
 *
 * @param[in] len - The length to set
 *
 */
DOCA_EXPERIMENTAL
__forceinline void doca_dpa_dev_buf_set_len(doca_dpa_dev_buf_t buf, size_t len)
{
	((struct doca_dpa_dev_buf *)buf)->len = len;
}

/**
 * @brief Set the DOCA Mmap associated with a DPA buffer handle
 *
 * @param[in] buf - DOCA DPA device buf handle
 *
 * @param[in] mmap - The Mmap DPA handle to set
 *
 */
DOCA_EXPERIMENTAL
__forceinline void doca_dpa_dev_buf_set_mmap(doca_dpa_dev_buf_t buf, doca_dpa_dev_mmap_t mmap)
{
	((struct doca_dpa_dev_buf *)buf)->mmap = mmap;
}

/**
 * @brief Get address from a DPA buffer handle
 *
 * @param[in] buf - DOCA DPA device buf handle
 *
 * @return
 * Address held by DPA buffer
 */
DOCA_EXPERIMENTAL
__forceinline uintptr_t doca_dpa_dev_buf_get_addr(doca_dpa_dev_buf_t buf)
{
	return ((struct doca_dpa_dev_buf *)buf)->addr;
}

/**
 * @brief Get length from a DPA buffer handle
 *
 * @param[in] buf - DOCA DPA device buf handle
 *
 * @return
 * Length of DPA buffer
 */
DOCA_EXPERIMENTAL
__forceinline uint64_t doca_dpa_dev_buf_get_len(doca_dpa_dev_buf_t buf)
{
	return ((struct doca_dpa_dev_buf *)buf)->len;
}

/**
 * @brief Get the DOCA Mmap associated with a DPA buffer handle
 *
 * @param[in] buf - DOCA DPA device buf handle
 *
 * @return
 * The Mmap DPA handle of DPA buffer
 *
 */
DOCA_EXPERIMENTAL
__forceinline doca_dpa_dev_mmap_t doca_dpa_dev_buf_get_mmap(doca_dpa_dev_buf_t buf)
{
	return ((struct doca_dpa_dev_buf *)buf)->mmap;
}

/**
 * \brief Obtain a pointer to externally allocated memory
 *
 * This function allows the DPA process to obtain a pointer to external memory that is held by a DPA handle.
 * The obtained pointer can be used to load/store data directly from the DPA kernel.
 * The memory being accessed through the returned device pointer is subject to 64B alignment restriction
 *
 * @param[in] buf - DOCA DPA device buf handle
 *
 * @return
 * Device address pointing to external address
 */
DOCA_EXPERIMENTAL
doca_dpa_dev_uintptr_t doca_dpa_dev_buf_get_external_ptr(doca_dpa_dev_buf_t buf);

/**
 * \brief Initiate a copy data between two DOCA buffers
 *
 * This function copies data between two DOCA buffers.
 * The destination buffer, specified by `dst_buf_handle` will contain the copied data after the memory copy is complete.
 * This is a non-blocking routine
 *
 * @note This API is relevant only for a kernel used for DPA thread ('doca_dpa_thread'), which was set previously
 * using host API doca_dpa_thread_set_func_arg().
 * This API is not relevant for a kernel used in host kernel launch APIs.
 *
 * @param[in] async_ops_handle - DOCA DPA device asynchronous ops handle
 * @param[in] dst_buf_handle - destination DOCA buffer to copy into
 * @param[in] src_buf_handle - source DOCA buffer to read from
 * @param[in] completion_requested - to raise a completion when copy data operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_post_buf_memcpy(doca_dpa_dev_async_ops_t async_ops_handle,
				  doca_dpa_dev_buf_t dst_buf_handle,
				  doca_dpa_dev_buf_t src_buf_handle,
				  uint32_t completion_requested);

/**
 * \brief Initiate a copy data between two DOCA Mmaps
 *
 * This function copies data between two DOCA Mmaps.
 * The destination DOCA Mmap, specified by `dst_mmap_handle`, `dst_addr`
 * will contain the copied data in source DOCA Mmap specified by `src_mmap_handle`, `src_addr` and `length`
 * after the memory copy is complete. This is a non-blocking routine
 *
 * @note This API is relevant only for a kernel used for DPA thread ('doca_dpa_thread'), which was set previously
 * using host API doca_dpa_thread_set_func_arg().
 * This API is not relevant for a kernel used in host kernel launch APIs.
 *
 * @param[in] async_ops_handle - DOCA DPA device asynchronous ops handle
 * @param[in] dst_mmap_handle - destination DOCA Mmap handle to copy into
 * @param[in] dst_addr - address of destination buffer
 * @param[in] src_mmap_handle - source DOCA Mmap handle to read from
 * @param[in] src_addr - address of source buffer
 * @param[in] length - size of buffer
 * @param[in] completion_requested - to raise a completion when copy data operation is done (any value greater than 0)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_post_memcpy(doca_dpa_dev_async_ops_t async_ops_handle,
			      doca_dpa_dev_mmap_t dst_mmap_handle,
			      uint64_t dst_addr,
			      doca_dpa_dev_mmap_t src_mmap_handle,
			      uint64_t src_addr,
			      size_t length,
			      uint32_t completion_requested);

/**
 * \brief Obtain a pointer to externally allocated memory
 *
 * This function allows the DPA process to obtain a pointer to external memory
 * that is held by a DOCA Mmap handle and given address.
 * The obtained pointer can be used to load/store data directly from the DPA kernel.
 * The memory being accessed through the returned device pointer is subject to 64B alignment restriction
 *
 * @param[in] mmap_handle - DOCA Mmap handle
 * @param[in] addr - external address to be accessed
 *
 * @return
 * Device address pointing to external address
 */
DOCA_EXPERIMENTAL
doca_dpa_dev_uintptr_t doca_dpa_dev_mmap_get_external_ptr(doca_dpa_dev_mmap_t mmap_handle, uint64_t addr);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DPA_DEV_BUF_H_ */
