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
 * @file doca_devemu_vfs_type.h
 * @page doca devemu vfs_type
 * @defgroup VFS Vfs library
 *
 * DOCA Virtio FS type
 *
 * @{
 */

#ifndef DOCA_DEVEMU_VFS_TYPE_H_
#define DOCA_DEVEMU_VFS_TYPE_H_

#include <stdint.h>

#include <doca_error.h>
#include <doca_dev.h>
#include <doca_devemu_pci.h>
#include <doca_devemu_virtio.h>
#include <doca_devemu_virtio_type.h>
#include <doca_devemu_vfs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************************
 * DOCA devemu Virtio FS type API
 *********************************************************************************************************************/

/**
 * @brief Check if the default DOCA Virtio FS type is supported by the device.
 *
 * @details Get uint8_t value defining if the device can be used to manage DOCA Virtio FS emulated devices associated
 * with the default Virtio FS type.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] supported
 * 1 if the default Virtio FS type is supported by the device, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'devinfo' or 'supported' are NULL.
 * - DOCA_ERROR_DRIVER - internal doca driver error
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_vfs_is_default_vfs_type_supported(const struct doca_devinfo *devinfo,
		uint8_t *supported);

/**
 * @brief Find the default DOCA Virtio FS type associated with the device.
 *
 * @param [in] dev
 * The doca dev associated with the default type.
 * @param [out] vfs_type
 * Started DOCA Virtio FS default type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_vfs_find_default_vfs_type_by_dev(struct doca_dev *dev,
		struct doca_devemu_vfs_type **vfs_type);

/**
 * @brief Convert DOCA Virtio FS type instance into DOCA Virtio type.
 *
 * @param [in] vfs_type
 * DOCA Virtio FS type instance. This must remain valid until after the DOCA Virtio type is no longer required.
 *
 * @return
 * DOCA Virtio type upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_devemu_virtio_type *doca_devemu_vfs_type_as_virtio_type(struct doca_devemu_vfs_type *vfs_type);

/**
 * @brief Convert DOCA Virtio FS type instance into DOCA PCI type.
 *
 * @param [in] vfs_type
 * DOCA Virtio FS type instance. This must remain valid until after the DOCA PCI type is no longer required.
 *
 * @return
 * DOCA PCI type upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_devemu_pci_type *doca_devemu_vfs_type_as_pci_type(struct doca_devemu_vfs_type *vfs_type);

/**
 * @brief Get the value of the num_request_queues register.
 *
 * @param [in] vfs_type
 * The DOCA Virtio FS type instance to query.
 * @param [out] num_request_queues
 * The virtio_fs_config:num_request_queues register value according to virtio specification to be used, by default, by
 * devices associated with the vfs_type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'vfs_type' or 'num_request_queues' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_vfs_type_get_num_request_queues(const struct doca_devemu_vfs_type *vfs_type,
		uint32_t *num_request_queues);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DEVEMU_VFS_TYPE_H_ */
