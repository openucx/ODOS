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
 * @file doca_devemu_virtio_type.h
 * @page doca devemu virtio_type
 * @defgroup VIRTIO Virtio core library
 *
 * DOCA Virtio type
 *
 * @{
 */

#ifndef DOCA_DEVEMU_VIRTIO_TYPE_H_
#define DOCA_DEVEMU_VIRTIO_TYPE_H_

#include <stdint.h>

#include <doca_error.h>
#include <doca_dev.h>
#include <doca_devemu_pci.h>
#include <doca_devemu_virtio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************************
 * DOCA devemu Virtio type API
 *********************************************************************************************************************/

/**
 * @brief Convert DOCA Virtio type instance into DOCA PCI type.
 *
 * @param [in] virtio_type
 * DOCA Virtio type instance. This must remain valid until after the DOCA PCI type is no longer required.
 *
 * @return
 * DOCA PCI type upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_devemu_pci_type *doca_devemu_virtio_type_as_pci_type(struct doca_devemu_virtio_type *virtio_type);

/**
 * @brief Get the value of the num_queues register.
 *
 * @param [in] virtio_type
 * The DOCA Virtio type instance to query.
 * @param [out] num_queues
 * The default value to be used by devices associated with the virtio_type for virtio_common_config:num_queues register
 * according to Virtio specification, if not set otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_type_get_num_queues(const struct doca_devemu_virtio_type *virtio_type,
		uint16_t *num_queues);

/**
 * @brief Get the value of the queue_size register.
 *
 * @param [in] virtio_type
 * The DOCA Virtio type instance to query.
 * @param [out] queue_size
 * The default value to be used by devices associated with the virtio_type for virtio_common_config:queue_size register
 * according to Virtio specification, if not set otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_type_get_queue_size(const struct doca_devemu_virtio_type *virtio_type,
		uint16_t *queue_size);

/**
 * @brief Get the Virtio device_feature bits (0-63) according to Virtio specification configured for this type.
 *
 * @param [in] virtio_type
 * The DOCA Virtio type instance to query.
 * @param [out] features
 * The device_feature (bits 0-63) according to Virtio specification for devices associated with this type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_type_get_device_features_63_0(const struct doca_devemu_virtio_type *virtio_type,
		uint64_t *features);

/**
 * @brief Get the initial Virtio device config_generation register according to Virtio specification configured for this type.
 *
 * @param [in] virtio_type
 * The DOCA Virtio type instance to query.
 * @param [out] config_generation
 * The initial value of the config_generation register according to Virtio specification for devices associated with this type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_type_get_config_generation(const struct doca_devemu_virtio_type *virtio_type,
		uint8_t *config_generation);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DEVEMU_VIRTIO_TYPE_H_ */
