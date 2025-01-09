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
 * @file doca_types.h
 * @page doca types
 * @defgroup TYPES DOCA Types
 * @ingroup DOCACore
 * DOCA Types introduces types that are common for many libraries.
 *
 * @{
 */

#ifndef DOCA_TYPES_H_
#define DOCA_TYPES_H_

#include <stdint.h>

#ifdef __linux__
#include <linux/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
/** 'fd' for blocking with epoll/select/poll, event type will be "read ready" */
typedef int doca_event_handle_t;
#define doca_event_invalid_handle -1		       /**< Represents invalid handle value */
#else						       /* Windows*/
typedef void *doca_event_handle_t; /**< Used for windows HANDLE or IOCompletionPort */
#define doca_event_invalid_handle INVALID_HANDLE_VALUE /**< Represents invalid handle value */
#endif

/**
 * @brief Specifies the length of a GID (Global ID) in bytes.
 */
#define DOCA_GID_BYTE_LENGTH 16

typedef doca_event_handle_t doca_notification_handle_t; /**< Type alias used with progress engine */

/**
 * @brief Convenience type for representing opaque data
 */
union doca_data {
	void *ptr;    /**< Data as a pointer */
	uint64_t u64; /**< Data as a 64-bit unsigned integer */
};

/**
 * @brief Struct to represent a gather list
 */
struct doca_gather_list {
	void *addr;		       /**< Address of buffer in the list */
	uint64_t len;		       /**< Length of current buffer in bytes */
	struct doca_gather_list *next; /**< Pointer to next buffer in the list */
};

/**
 * @brief Specifies the permission level for DOCA buffer
 *
 * @details Can be used with doca_mmap_set_permissions() to set permission level. A few notes:
 * DOCA_ACCESS_FLAG_PCI_READ_ONLY and DOCA_ACCESS_FLAG_PCI_READ_WRITE are mutually exclusive
 * Buffer can always be read locally by local device, regardless of set permissions
 * local device - doca_dev running in the same process of the mmap
 * remote device - doca_dev running on a different process on a remote machine
 * DPU device - doca_dev running on a process on the DPU OS. This is only relevant when local process is running on
 * HOST. In case local process is running on DPU the doca_dev is considered a local device.
 */
enum doca_access_flag {
	DOCA_ACCESS_FLAG_LOCAL_READ_ONLY = 0,		  /* Allows reading buffer by a local device but no write */
	DOCA_ACCESS_FLAG_LOCAL_READ_WRITE = (1 << 0),	  /* Allows reading and writing to buffer by a local device */
	DOCA_ACCESS_FLAG_RDMA_READ = (1 << 1),		  /* Allows reading buffer by a remote device using RDMA */
	DOCA_ACCESS_FLAG_RDMA_WRITE = (1 << 2),		  /* Allows writing to buffer by a remote device using RDMA */
	DOCA_ACCESS_FLAG_RDMA_ATOMIC = (1 << 3),	  /* Allows atomic operations by remote device using RDMA */
	DOCA_ACCESS_FLAG_PCI_READ_ONLY = (1 << 4),	  /**
							   * Allows reading buffer by device on same PCI
							   * but prevents write. See doca_mmap_export_pci()
							   */
	DOCA_ACCESS_FLAG_PCI_READ_WRITE = (1 << 5),	  /**
							   * Allows reading and writing to buffer by a device on
							   * same PCI. See doca_mmap_export_pci()
							   */
	DOCA_ACCESS_FLAG_PCI_RELAXED_ORDERING = (1 << 6), /**
							   * Allow system to reorder accesses to
							   * the mapped memory to improve performance
							   */
};

/**
 * @brief Specifies the PCI function type for DOCA representor device
 *
 */
enum doca_pci_func_type {
	DOCA_PCI_FUNC_TYPE_PF = 0, /* physical function */
	DOCA_PCI_FUNC_TYPE_VF,	   /* virtual function */
	DOCA_PCI_FUNC_TYPE_SF,	   /* sub function */
};

/**
 * @brief Declare DOCA endianity types
 *
 */
#ifdef __linux__
typedef __be16 doca_be16_t; /**< Unsigned 16-bit integer in Big Endian */
typedef __be32 doca_be32_t; /**< Unsigned 32-bit integer in Big Endian */
typedef __be64 doca_be64_t; /**< Unsigned 64-bit integer in Big Endian */
#else			    /* Windows */
typedef uint16_t doca_be16_t;	   /**< Unsigned 16-bit integer in Big Endian */
typedef uint32_t doca_be32_t;	   /**< Unsigned 32-bit integer in Big Endian */
typedef uint64_t doca_be64_t;	   /**< Unsigned 64-bit integer in Big Endian */
#endif

/**
 * @brief Type of memory the GPUNetIO library can allocate
 *
 */
enum doca_gpu_mem_type {
	/* GPU memory not accessible from CPU. */
	DOCA_GPU_MEM_TYPE_GPU = 0,
	/* GPU memory with direct access from CPU. */
	DOCA_GPU_MEM_TYPE_GPU_CPU = 1,
	/* CPU memory with direct access from GPU. */
	DOCA_GPU_MEM_TYPE_CPU_GPU = 2,
};

/**
 * @brief Type of wait on time the network card can support
 *
 */
enum doca_eth_wait_on_time_type {
	/* Wait on time not supported. */
	DOCA_ETH_WAIT_ON_TIME_TYPE_NONE = 0,
	/* Wait on time supported by HW. */
	DOCA_ETH_WAIT_ON_TIME_TYPE_NATIVE = 1,
	/* Wait on time supported by SW. */
	DOCA_ETH_WAIT_ON_TIME_TYPE_DPDK = 2,
};

/**
 * @brief MTU size in bytes.
 */
enum doca_mtu_size {
	DOCA_MTU_SIZE_256_BYTES = 0x0,
	DOCA_MTU_SIZE_512_BYTES = 0x1,
	DOCA_MTU_SIZE_1K_BYTES = 0x2,
	DOCA_MTU_SIZE_2K_BYTES = 0x3,
	DOCA_MTU_SIZE_4K_BYTES = 0x4,
	DOCA_MTU_SIZE_RAW_ETHERNET = 0x5, /* Reserved */
};

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_TYPES_H_ */
