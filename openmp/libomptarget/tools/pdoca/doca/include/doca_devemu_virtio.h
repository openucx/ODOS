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
 * @file doca_devemu_virtio.h
 * @page doca devemu virtio
 * @defgroup VIRTIO Virtio core library
 *
 * DOCA library for emulated virtio devices logic
 *
 * @{
 */

#ifndef DOCA_DEVEMU_VIRTIO_H_
#define DOCA_DEVEMU_VIRTIO_H_

#include <stdint.h>

#include <doca_error.h>
#include <doca_dev.h>
#include <doca_devemu_pci.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque structure representing emulated Virtio pci device.
 * This structure extends the pci core doca_devemu_pci_dev structure.
 * This structure is used by Virtio device emulation applications, libraries and services.
 */
struct doca_devemu_virtio_dev;

/**
 * @brief Opaque structure representing emulated Virtio pci device type.
 * This structure extends the pci core doca_devemu_pci_type structure.
 * This structure is used by pci device emulation applications, libraries and services.
 */
struct doca_devemu_virtio_type;

/**
 * @brief Opaque structure representing emulated Virtio pci device IO context.
 * This structure is used by pci device emulation applications, libraries and services.
 */
struct doca_devemu_virtio_io;

/*********************************************************************************************************************
 * DOCA devemu Virtio device Properties
 *********************************************************************************************************************/

/**
 * @brief Get the Virtio device_feature bits (0-63) according to Virtio specification.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] features
 * The device_feature (bits 0-63) according to Virtio specification.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_device_features_63_0(const struct doca_devemu_virtio_dev *virtio_dev,
		uint64_t *features);

/**
 * @brief Set the Virtio device_feature bits (0-63) according to Virtio specification.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to modify.
 * @param [in] features
 * The device_feature (bits 0-63) according to Virtio specification.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_set_device_features_63_0(struct doca_devemu_virtio_dev *virtio_dev,
		uint64_t features);

/**
 * @brief Get the Virtio driver_feature bits (0-63) according to Virtio specification.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] features
 * The driver_feature (bits 0-63) according to Virtio specification.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_driver_features_63_0(const struct doca_devemu_virtio_dev *virtio_dev,
		uint64_t *features);

/**
 * @brief Get the Virtio config_msix_vector register according to Virtio specification.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] config_msix_vector
 * The value of the config_msix_vector register according to Virtio specification.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_config_msix_vector(const struct doca_devemu_virtio_dev *virtio_dev,
		uint16_t *config_msix_vector);

/**
 * @brief Get the Virtio device num_queues register from common configuration structure according to Virtio specification.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] num_queues
 * The value of the num_queues register.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_num_queues(const struct doca_devemu_virtio_dev *virtio_dev,
		uint16_t *num_queues);

/**
 * @brief Set the Virtio device num_queues register in common configuration structure according to Virtio specification.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to modify.
 * @param [in] num_queues
 * The device common num_queues register.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_set_num_queues(struct doca_devemu_virtio_dev *virtio_dev, uint16_t num_queues);

/**
 * @brief Get the number of enabled Virtio device queues by the driver. The driver enables a queue by setting the
 * corresponding queue index to the queue_select register and setting the queue_enable register to 1. The return value
 * of num_queues is valid only if DRIVER_OK status bit was set by the driver.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] num_queues
 * The number of enable Virtio queues for the virtio device.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_num_enabled_queues(const struct doca_devemu_virtio_dev *virtio_dev,
		uint16_t *num_queues);

/**
 * @brief Get the Virtio device_status register from common configuration structure according to Virtio specification.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] device_status
 * The value of the device_status register.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_device_status(const struct doca_devemu_virtio_dev *virtio_dev,
		uint8_t *device_status);

/**
 * @brief Get the Virtio config_generation register from common configuration structure according to Virtio specification.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] config_generation
 * The value of the config_generation register.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_config_generation(const struct doca_devemu_virtio_dev *virtio_dev,
		uint8_t *config_generation);

/**
 * @brief Get the Virtio max queue size for all Virtio queues.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] queue_size
 * The maximal queue size for all Virtio queues.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_queue_size(const struct doca_devemu_virtio_dev *virtio_dev,
		uint16_t *queue_size);

/**
 * @brief Set the Virtio max queue size for all Virtio queues.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [in] queue_size
 * The maximal queue size for all Virtio queues.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_set_queue_size(struct doca_devemu_virtio_dev *virtio_dev, uint16_t queue_size);

/**
 * @brief Get the number of required running Virtio io context's to be bounded to the Virtio device context. The Virtio
 * device context will not move to a "running" state before having this amount of running Virtio IO context's bounded
 * to it.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to query.
 * @param [out] num_virtio_io
 * The number of required running Virtio IO ctx's to be bounded to the device.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_get_num_required_running_virtio_io_ctxs(const struct doca_devemu_virtio_dev *virtio_dev,
		uint32_t *num_virtio_io);

/**
 * @brief Set the number of required running Virtio IO context's to be bounded to the Virtio device context. The Virtio
 * device context will not move to a "running" state before having this amount of running Virtio IO context's bounded
 * to it.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio device instance to modify.
 * @param [in] num_virtio_io
 * The number of required running Virtio IO ctx's to be bounded.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_set_num_required_running_virtio_io_ctxs(struct doca_devemu_virtio_dev *virtio_dev,
		uint32_t num_virtio_io);

/**
 * @brief Complete the Virtio device reset handling. Prior to calling this function, the user must ensure that all the
 * resources associated with the Virtio device are flushed back to the ownership of the device.
 *
 * @param [in] virtio_dev
 * DOCA Virtio device instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_reset_complete(struct doca_devemu_virtio_dev *virtio_dev);

/**
 * @brief Convert DOCA Virtio device instance into DOCA context.
 *
 * @param [in] virtio_dev
 * DOCA Virtio device instance. This must remain valid until after the DOCA context is no longer required.
 *
 * @return
 * doca ctx upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_devemu_virtio_dev_as_ctx(struct doca_devemu_virtio_dev *virtio_dev);

/**
 * @brief Convert DOCA Virtio device instance into DOCA devemu PCI device.
 *
 * @param [in] virtio_dev
 * DOCA Virtio device instance. This must remain valid until after the DOCA devemu PCI device is no longer required.
 *
 * @return
 * DOCA devemu pci device upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_devemu_pci_dev *doca_devemu_virtio_dev_as_pci_dev(struct doca_devemu_virtio_dev *virtio_dev);

/*********************************************************************************************************************
 * DOCA devemu Virtio device context events API
 *********************************************************************************************************************/

/**
 * @brief Function to be executed on Virtio device reset. The event handler will enable users to quiesce, flush and
 * reset the necessary resources associated with the emulated Virtio device.
 * Upon event, all PCI I/O transactions to/from the host memory are disabled.
 * Additionally, the user should flush all the outstanding resources associated with the emulated Virtio device, which
 * were initially owned by the Virtio device and moved the the ownership of the user. After flushing all the
 * outstanding resources, the user should call doca_devemu_virtio_dev_reset_complete().
 *
 * @param [in] virtio_dev
 * DOCA Virtio device instance.
 * @param [in] event_user_data
 * Same user data that was provided in doca_devemu_virtio_dev_event_reset_register().
 *
 */
typedef void (*doca_devemu_virtio_dev_event_reset_handler_cb_t)(struct doca_devemu_virtio_dev *virtio_dev,
								union doca_data event_user_data);

/**
 * @brief Register to Virtio device reset event.
 *
 * Registration can be done only if the Virtio device ctx is idle. If called multiple times then only the last call
 * will take effect.
 *
 * @param [in] virtio_dev
 * The DOCA Virtio dev context to be associated to the event. Must be idle.
 * @param [in] handler
 * Method that is invoked once event is triggered.
 * @param [in] user_data
 * User data that will be provided to the handler once invoked.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'virtio_dev' or 'handler' are NULL
 * - DOCA_ERROR_BAD_STATE - virtio_dev context is not idle
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_virtio_dev_event_reset_register(struct doca_devemu_virtio_dev *virtio_dev,
		doca_devemu_virtio_dev_event_reset_handler_cb_t handler,
		union doca_data user_data);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DEVEMU_VIRTIO_H_ */
