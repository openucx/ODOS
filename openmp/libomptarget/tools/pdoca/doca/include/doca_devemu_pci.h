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
 * @file doca_devemu_pci.h
 * @page doca devemu pci
 * @defgroup DEVEMU device emulation PCI library
 *
 * DOCA library for emulated PCI devices
 *
 * @{
 */

#ifndef DOCA_DEVEMU_PCI_H_
#define DOCA_DEVEMU_PCI_H_

#include <stdint.h>
#include <sys/uio.h>

#include <doca_error.h>
#include <doca_dev.h>
#include <doca_mmap.h>
#include <doca_ctx.h>
#include <doca_pe.h>
#include <doca_devemu_pci_type.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque structure representing emulated PCI device.
 * This structure is used by PCI device emulation applications, libraries and services.
 */
struct doca_devemu_pci_dev;

/**
 * @brief Opaque structure representing emulated PCI device doorbell.
 * This structure is used by PCI device emulation applications, libraries and services.
 */
struct doca_devemu_pci_db;

/**
 * @brief DPA handle for emulated PCI device doorbell.
 */
typedef uint64_t doca_dpa_dev_devemu_pci_db_t;

/**
 * @brief Opaque structure representing emulated PCI device doorbell completion context.
 * This structure is used by PCI device emulation applications, libraries and services.
 */
struct doca_devemu_pci_db_completion;

/**
 * @brief DPA handle for emulated PCI device doorbell completion context.
 */
typedef uint64_t doca_dpa_dev_devemu_pci_db_completion_t;

/**
 * @brief Opaque structure representing emulated PCI device MSI-X.
 * This structure is used by PCI device emulation applications, libraries and services.
 */
struct doca_devemu_pci_msix;

/**
 * @brief DPA handle for emulated PCI device MSI-X.
 */
typedef uint64_t doca_dpa_dev_devemu_pci_msix_t;

/*********************************************************************************************************************
 * DOCA devemu pci Device Info Properties
 *********************************************************************************************************************/

/**
 * @brief Get the maximum number of PCI devices, across all PCI types, that can be hot-plugged by the device.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_hotplug_devices
 * Number of PCI devices that can be hot plugged by the device.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - internal doca driver error.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_get_max_hotplug_devices(const struct doca_devinfo *devinfo,
							 uint32_t *max_hotplug_devices);

/*********************************************************************************************************************
 * DOCA devemu PCI device API
 *********************************************************************************************************************/

/**
 * @brief Allocate DOCA devemu PCI device.
 *
 * @param [in] pci_type
 * The DOCA PCI type to be associated with the device. Must be started.
 * @param [in] dev_rep
 * Representor DOCA device.
 * @param [in] progress_engine
 * The progress engine that will be used to receive events and task completions.
 * @param [out] pci_dev
 * The newly created DOCA devemu PCI device.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type', 'dev_rep', 'progress_engine' or 'pci_dev' are NULL.
 *	Or representor type does not match the PCI type
 * - DOCA_ERROR_NO_MEMORY - allocation failure
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_create(struct doca_devemu_pci_type *pci_type,
					struct doca_dev_rep *dev_rep,
					struct doca_pe *progress_engine,
					struct doca_devemu_pci_dev **pci_dev);

/**
 * @brief Free a DOCA devemu PCI device.
 *
 * @param [in] pci_dev
 * The previously created DOCA devemu PCI device. Must be idle.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_dev' is NULL
 * - DOCA_ERROR_BAD_STATE - device is not idle. Use doca_ctx_stop() to stop it
 * - DOCA_ERROR_NOT_PERMITTED - PCI device was not created using doca_devemu_pci_dev_create()
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_destroy(struct doca_devemu_pci_dev *pci_dev);

/**
 * @brief Modify default registers values for stateful region in a DOCA devemu PCI device.
 *
 * @details This method will modify the default values for the entire stateful region registers area in a PCI device
 * BAR (before the first modification, the initial default values of the stateful region registers are taken from the
 * associated PCI type).
 * These values will override the previous default values and will become valid during the next exposure/hotplug of the
 * associated PCI device to the host or during the next FLR.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device. Must be started.
 * @param [in] id
 * The BAR id that contains the stateful region.
 * @param [in] start_addr
 * The start address of the region within the BAR. This value must conform with the start address provided during
 * doca_devemu_pci_type_set_bar_stateful_region_conf().
 * @param [in] default_values
 * Input buffer that contain the default values data.
 * @param [in] size
 * The size of the default_values buffer in bytes. The size must not be smaller than the actual size of the stateful
 * bar region that was configured using doca_devemu_pci_type_set_bar_stateful_region_conf(). If size is bigger than the
 * actual size, the first relevant bytes will be used according to the actual size. The rest of the buffer will be
 * ignored.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_modify_bar_stateful_region_default_values(struct doca_devemu_pci_dev *pci_dev,
									   uint8_t id,
									   uint64_t start_addr,
									   void *default_values,
									   uint64_t size);

/**
 * @brief Modify registers values for stateful region in a DOCA devemu PCI device.
 *
 * @details This method will modify the values of the stateful region registers in a PCI device BAR. These values will
 * override the existing values of the stateful region of the associated PCI device. Modifying registers by calling
 * this method will not trigger the registered event handler of the
 * doca_devemu_pci_dev_event_bar_stateful_region_driver_write event.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device. Must be started.
 * @param [in] id
 * The BAR id that contains the stateful region.
 * @param [in] offset
 * The offset of the registers region to modify within the BAR. Must be located within the stateful BAR region.
 * @param [in] values
 * Input buffer that contain the values data.
 * @param [in] size
 * The size of the values buffer in bytes. The (offset + size) must be located within the stateful BAR region.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_modify_bar_stateful_region_values(struct doca_devemu_pci_dev *pci_dev,
								   uint8_t id,
								   uint64_t offset,
								   void *values,
								   uint64_t size);

/**
 * @brief Query registers values of the stateful region in a DOCA devemu PCI device.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device. Must be started.
 * @param [in] id
 * The BAR id that contains the stateful region.
 * @param [in] offset
 * The offset of the registers region to query within the BAR. Must be located within the stateful BAR region.
 * @param [out] out_values
 * Output buffer that will contain the values data upon success.
 * @param [in] size
 * The size of the out_values buffer in bytes. The (offset + size) must be located within the stateful BAR region.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_query_bar_stateful_region_values(struct doca_devemu_pci_dev *pci_dev,
								  uint8_t id,
								  uint64_t offset,
								  void *out_values,
								  uint64_t size);

/**
 * @brief Convert DOCA devemu PCI device instance into DOCA context.
 *
 * @param [in] pci_dev
 * DOCA devemu PCI device. The device must remain valid until after the returned DOCA context is no longer required.
 *
 * @return
 * DOCA context upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_devemu_pci_dev_as_ctx(struct doca_devemu_pci_dev *pci_dev);

/**
 * @brief Query wheather the DOCA devemu PCI device is having FLR (Function Level Reset).
 *
 * @brief if true, all PCI I/O transactions to/from the host memory are disabled and the user should re-configure the
 * emulated PCI device. This re-configuration requires destruction of all the associated resources (e.g. DBs, MSIXs,
 * MMAPs), resetting the associated emulated PCI device (perform stop() and start() operations) and re-creating all the
 * needed resources.

 * @param [in] pci_dev
 * The DOCA devemu PCI device to query. Must be started.
 * @param [out] flr
 * 1 if the DOCA devemu PCI device is having FLR, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_dev' or 'flr' are NULL.
 * - DOCA_ERROR_BAD_STATE - PCI device is not started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_is_flr(const struct doca_devemu_pci_dev *pci_dev, uint8_t *flr);

/*********************************************************************************************************************
 * DOCA devemu PCI device hotplug API
 *********************************************************************************************************************/

/**
 * @brief DOCA devemu pci hotplug state.
 *
 * The steps for hotplug a device are:
 * 1. check the current hotplug state
 *     1.1 if state == DOCA_DEVEMU_PCI_HP_STATE_POWER_OFF then issue hotplug operation
 *         (call doca_devemu_pci_dev_hotplug()) and wait for transition to DOCA_DEVEMU_PCI_HP_STATE_POWER_ON.
 *     1.2 if state == DOCA_DEVEMU_PCI_HP_STATE_PLUG_IN_PROGRESS then wait for transition to
 *         DOCA_DEVEMU_PCI_HP_STATE_POWER_ON.
 *     1.3 if state == DOCA_DEVEMU_PCI_HP_STATE_UNPLUG_IN_PROGRESS then wait for transition to
 *         DOCA_DEVEMU_PCI_HP_STATE_POWER_OFF and go to step 1.1.
 *     1.4 if state == DOCA_DEVEMU_PCI_HP_STATE_POWER_ON then do nothing --> device is plugged.
 *
 * The steps for hot unplug a device are:
 * 1. check the current hotplug state
 *     1.1 if state == DOCA_DEVEMU_PCI_HP_STATE_POWER_ON then issue hot unplug operation
 *         (call doca_devemu_pci_dev_hotunplug()) and wait for transition to DOCA_DEVEMU_PCI_HP_STATE_POWER_OFF.
 *     1.2 if state == DOCA_DEVEMU_PCI_HP_STATE_UNPLUG_IN_PROGRESS then wait for transition to
 *         DOCA_DEVEMU_PCI_HP_STATE_POWER_OFF.
 *     1.3 if state == DOCA_DEVEMU_PCI_HP_STATE_PLUG_IN_PROGRESS then wait for transition to
 *         DOCA_DEVEMU_PCI_HP_STATE_POWER_ON and go to step 1.1 or issue hot unplug operation
 *         (call doca_devemu_pci_dev_hotunplug()) then wait for DOCA_DEVEMU_PCI_HP_STATE_POWER_OFF.
 *     1.4 if state == DOCA_DEVEMU_PCI_HP_STATE_POWER_OFF then do nothing --> device is un-plugged.
 *
 * @note It is recommended to use doca_devemu_pci_dev_event_hotplug_state_change mechanism to get notifications on
 * hotplug state changes.
 */
enum doca_devemu_pci_hotplug_state {
	/**< Device is powered off and not visible by the host. */
	DOCA_DEVEMU_PCI_HP_STATE_POWER_OFF = 0,
	/**< Device is in transitional state to become un-plugged from host. */
	DOCA_DEVEMU_PCI_HP_STATE_UNPLUG_IN_PROGRESS,
	/**< Device is in transitional state to become plugged to host. */
	DOCA_DEVEMU_PCI_HP_STATE_PLUG_IN_PROGRESS,
	/**< Device is powered on and visible by the host. */
	DOCA_DEVEMU_PCI_HP_STATE_POWER_ON,
};

/**
 * @brief Get the hotplug state of the DOCA devemu PCI device.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device. Must be started.
 * @param [out] state
 * The hotplug state of the given DOCA devemu PCI device.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_dev' or 'state' are NULL
 * - DOCA_ERROR_BAD_STATE - PCI device is not started
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_get_hotplug_state(struct doca_devemu_pci_dev *pci_dev,
						   enum doca_devemu_pci_hotplug_state *state);

/**
 * @brief Issue hotplug procedure of the DOCA devemu PCI device.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device to hotplug. Must be started.
 *
 * @return
 * DOCA_SUCCESS - in case of success. On success, pci_dev is at DOCA_DEVEMU_PCI_HP_STATE_PLUG_IN_PROGRESS state.
 *                Event will not be raised in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_dev' is NULL
 * - DOCA_ERROR_BAD_STATE - PCI device is not started, or hotplug state is not DOCA_DEVEMU_PCI_HP_STATE_POWER_OFF
 * - DOCA_ERROR_DRIVER - internal doca driver error
 * - DOCA_ERROR_NOT_SUPPORTED - The DOCA device that was set does not support hotplug, use
 *	doca_devemu_pci_cap_type_is_hotplug_supported() to find device that supports it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_hotplug(struct doca_devemu_pci_dev *pci_dev);

/**
 * @brief Issue hot unplug procedure of the DOCA devemu PCI device.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device to hot unplug. Must be started.
 *
 * @return
 * DOCA_SUCCESS - in case of success. On success, pci_dev is at DOCA_DEVEMU_PCI_HP_STATE_UNPLUG_IN_PROGRESS state.
 *                Event will not be raised in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_dev' is NULL
 * - DOCA_ERROR_BAD_STATE - PCI device is not started, or current hotplug state is not one of
 *      DOCA_DEVEMU_PCI_HP_STATE_POWER_ON or DOCA_DEVEMU_PCI_HP_STATE_PLUG_IN_PROGRESS
 * - DOCA_ERROR_DRIVER - internal doca driver error
 * - DOCA_ERROR_NOT_SUPPORTED - The DOCA device that was set does not support hot unplug, use
 *	doca_devemu_pci_cap_type_is_hotplug_supported() to find device that supports it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_hotunplug(struct doca_devemu_pci_dev *pci_dev);

/*********************************************************************************************************************
 * DOCA devemu PCI device events
 *********************************************************************************************************************/

/**
 * @brief Function to be executed on hotplug state change event occurrence.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device that is associated with the event.
 * @param [in] user_data
 * Same user data that was provided in doca_devemu_pci_dev_event_hotplug_state_change_register().
 */
typedef void (*doca_devemu_pci_dev_event_hotplug_state_change_handler_cb_t)(struct doca_devemu_pci_dev *pci_dev,
									    union doca_data user_data);

/**
 * @brief Register to hotplug state changes.
 *
 * Registration can be done only while DOCA devemu PCI device is idle. If called multiple times then only the last call
 * will take effect.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device to be associated with the event. Must be idle.
 * @param [in] handler
 * Method that is invoked once event is triggered.
 * @param [in] user_data
 * User data that will be provided to the handler once invoked.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_dev' or 'handler' are NULL
 * - DOCA_ERROR_BAD_STATE - PCI device is not idle
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_event_hotplug_state_change_register(
	struct doca_devemu_pci_dev *pci_dev,
	doca_devemu_pci_dev_event_hotplug_state_change_handler_cb_t handler,
	union doca_data user_data);

/**
 * @brief Opaque structure representing an asynchronous event for notifying upon PCI write transaction made by the
 * driver to a register located in the BAR stateful region.
 * Upon event registration the event is armed. Upon calling the registered event handler, the event becomes fired.
 * The event handler will be called to signal PCI writes for a single stateful region. The actual PCI write transaction
 * for the stateful region should be recognized by the service/application according to the device specification. For
 * example, by keeping the entire BAR stateful region copy and comparing it with the current BAR stateful region.
 * One can use doca_devemu_pci_dev_query_bar_stateful_region_values() to get the new values of the relevant stateful
 * region registers. One can update relevant stateful region registers by calling
 * doca_devemu_pci_dev_modify_bar_stateful_region_values().
 */
struct doca_devemu_pci_dev_event_bar_stateful_region_driver_write;

/**
 * @brief Function to be executed on PCI write transactions to BAR stateful region.
 *
 * @param [in] event
 * The BAR stateful region driver write event.
 * @param [in] user_data
 * Same user data that was provided in doca_devemu_pci_dev_event_bar_stateful_region_driver_write_register().
 */
typedef void (*doca_devemu_pci_dev_event_bar_stateful_region_driver_write_handler_cb_t)(
	struct doca_devemu_pci_dev_event_bar_stateful_region_driver_write *event,
	union doca_data user_data);

/**
 * @brief Register to BAR stateful region driver write event.
 *
 * Registration can be done only while DOCA devemu PCI device is idle. If called multiple times, for the same {pci_dev,
 * bar_id, bar_region_start_addr} tuple, then only the last call will take effect. The registration will be valid for
 * the entire stateful region that was configured for the associated DOCA devemu PCI device.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device to be associated with the event. Must be idle.
 * @param [in] handler
 * Method that is invoked once event is triggered.
 * @param [in] bar_id
 * The BAR id to be associated with the event. Must conform with the BAR stateful region configuration that was done
 * using doca_devemu_pci_type_set_bar_stateful_region_conf().
 * @param [in] bar_region_start_addr
 * The start address of the BAR stateful region to be associated with the event. Must conform with the BAR stateful
 * region configuration that was done using doca_devemu_pci_type_set_bar_stateful_region_conf().
 * @param [in] user_data
 * User data that will be provided to the handler once invoked.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_dev' or 'handler' are NULL
 * - DOCA_ERROR_BAD_STATE - PCI device is not idle
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_event_bar_stateful_region_driver_write_register(
	struct doca_devemu_pci_dev *pci_dev,
	doca_devemu_pci_dev_event_bar_stateful_region_driver_write_handler_cb_t handler,
	uint8_t bar_id,
	uint64_t bar_region_start_addr,
	union doca_data user_data);

/**
 * @brief Get DOCA devemu PCI device from BAR stateful region driver write event.
 *
 * @param [in] event
 * The registered BAR stateful region driver write event. Must not be NULL.
 *
 * @return
 * The DOCA devemu PCI device associated with the event.
 */
DOCA_EXPERIMENTAL
struct doca_devemu_pci_dev *doca_devemu_pci_dev_event_bar_stateful_region_driver_write_get_pci_dev(
	struct doca_devemu_pci_dev_event_bar_stateful_region_driver_write *event);

/**
 * @brief Get the BAR id from BAR stateful region driver write event.
 *
 * @param [in] event
 * The registered BAR stateful region driver write event. Must not be NULL.
 *
 * @return
 * The BAR id that is associated with the event.
 */
DOCA_EXPERIMENTAL
uint8_t doca_devemu_pci_dev_event_bar_stateful_region_driver_write_get_bar_id(
	struct doca_devemu_pci_dev_event_bar_stateful_region_driver_write *event);

/**
 * @brief Get the BAR region start address that is associated with BAR stateful region driver write event.
 *
 * @param [in] event
 * The registered BAR stateful region driver write event. Must not be NULL.
 *
 * @return
 * The start address of the BAR stateful region that is associated with the event.
 */
DOCA_EXPERIMENTAL
uint64_t doca_devemu_pci_dev_event_bar_stateful_region_driver_write_get_bar_region_start_addr(
	struct doca_devemu_pci_dev_event_bar_stateful_region_driver_write *event);

/**
 * @brief Function to be executed on PCI FLR (Function Level Reset). The event handler will enable users to quiesce,
 * flush and reset the necessary resources associated with the emulated PCI device.
 * Upon event, all PCI I/O transactions to/from the host memory are disabled.
 * Additionally, the user should re-configure the emulated PCI device. This re-configuration requires flushing of all
 * the outstanding resources associated with the emulated PCI device, which were initially owned by the PCI device and
 * moved the the ownership of the user. The re-configuration also requires destruction of all the associated resources
 * (e.g. DBs, MSIXs, MMAPs), resetting the associated emulated PCI device (perform stop() and start() operations) and
 * re-creating all the needed resources.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device that is associated with the event.
 * @param [in] user_data
 * Same user data that was provided in doca_devemu_pci_dev_event_flr_register().
 */
typedef void (*doca_devemu_pci_dev_event_flr_handler_cb_t)(struct doca_devemu_pci_dev *pci_dev,
							   union doca_data user_data);

/**
 * @brief Register to PCI FLR (Function Level Reset) event.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device to be associated with the event. Must be idle.
 * @param [in] handler
 * Method that is invoked once event is triggered.
 * @param [in] user_data
 * User data that will be provided to the handler once invoked.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_dev' or 'handler' are NULL
 * - DOCA_ERROR_BAD_STATE - PCI device is not idle
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_event_flr_register(struct doca_devemu_pci_dev *pci_dev,
						    doca_devemu_pci_dev_event_flr_handler_cb_t handler,
						    union doca_data user_data);

/*********************************************************************************************************************
 * DOCA devemu pci Doorbell
 *********************************************************************************************************************/

struct doca_dpa_thread;

/**
 * @brief Allocate DOCA devemu PCI device doorbell completion context on DPA. The created completion context will be
 * associated with a single dpa thread.
 *
 * @param [in] th
 * The DOCA dpa thread to be associated with the completion context.
 * @param [out] db_comp
 * The newly created DOCA devemu PCI device doorbell completion context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_completion_create(struct doca_dpa_thread *th,
						  struct doca_devemu_pci_db_completion **db_comp);

/**
 * @brief Destroy the DOCA devemu PCI device doorbell completion context.
 *
 * The associated dpa handle will be destroyed as well.
 *
 * @param [in] db_comp
 * The DOCA devemu PCI device doorbell completion context to destroy. Must be stopped.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_completion_destroy(struct doca_devemu_pci_db_completion *db_comp);

/**
 * @brief Start DOCA devemu PCI device doorbell completion context.
 *
 * @details On start verifies and finalizes the completion context configuration.
 *
 * The following is possible for started completion context:
 * - Associating DOCA devemu PCI device doorbells with the completion context.
 *
 * The following is NOT possible while completion context is started:
 * - Setting the properties of the completion context
 *
 * @param [in] db_comp
 * The DOCA devemu PCI device doorbell completion context to start.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_completion_start(struct doca_devemu_pci_db_completion *db_comp);

/**
 * @brief Stop DOCA devemu PCI device doorbell completion context.
 *
 * @details On stop prevents execution of different operations and allows operations that were available before start.
 * For details, see doca_devemu_pci_db_completion_start(). Completion context can't be stopped while there are
 * DOCA devemu PCI device doorbells associated with it.
 *
 * The following is possible for stopped completion context:
 * - Setting the properties of the completion context
 *
 * The following is NOT possible while completion context is stopped:
 * - Associating DOCA devemu PCI device doorbells with the completion context.
 *
 * @param [in] db_comp
 * The DOCA devemu PCI device doorbell completion context to stop.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_completion_stop(struct doca_devemu_pci_db_completion *db_comp);

/**
 * @brief Get the DPA handle for the DOCA devemu PCI device doorbell completion context.
 *
 * @param [in] db_comp
 * The DOCA devemu PCI device doorbell completion context previously created on DPA.
 * @param [out] db_comp_handle
 * A pointer to the associated DPA handle in the dpa memory space.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_completion_get_dpa_handle(struct doca_devemu_pci_db_completion *db_comp,
							  doca_dpa_dev_devemu_pci_db_completion_t *db_comp_handle);

/**
 * @brief Set the maximal number of doorbells that can be associated with the completion context.
 *
 * @param [in] db_comp
 * The DOCA devemu PCI device doorbell completion context to modify. Must be stopped.
 * @param [in] num_dbs
 * The maximal number of doorbells that can be associated with the context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_completion_set_max_num_dbs(struct doca_devemu_pci_db_completion *db_comp,
							   uint32_t num_dbs);

/**
 * @brief Get the maximal number of doorbells that can be associated with the completion context.
 *
 * @param [in] db_comp
 * The DOCA devemu PCI device doorbell completion context to query.
 * @param [out] num_dbs
 * The maximal number of doorbells that can be associated with the context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_completion_get_max_num_dbs(struct doca_devemu_pci_db_completion *db_comp,
							   uint32_t *num_dbs);

/**
 * @brief Get the current number of doorbells that are associated with the completion context.
 *
 * @param [in] db_comp
 * The DOCA devemu PCI device doorbell completion context to query.
 * @param [out] num_dbs
 * The current number of doorbells that are associated with the context.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_completion_get_curr_num_dbs(struct doca_devemu_pci_db_completion *db_comp,
							    uint32_t *num_dbs);

/**
 * @brief Allocate DOCA devemu PCI device doorbell on DPA. The created doorbell will be associated with a single
 * completion context that was also created on DPA.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device to be associated with the doorbell. Must be started.
 * @param [in] db_comp
 * The DOCA devemu PCI device doorbell completion context to be associated with the doorbell. Must be started.
 * @param [in] bar_id
 * The identifier of the BAR that contains the associated doorbell region for the created doorbell.
 * @param [in] bar_start_addr
 * The start address of the associated doorbell region within the BAR. This value must conform with the start
 * address that was configured to the doorbell region during the configuration cycle of the PCI type that is associated
 * with the given PCI device.
 * @param [in] db_id
 * The doorbell identifier that will be used to map the doorbell to its handler. This value must be in the range of
 * [0, num_db - 1] when num_db is the number of doorbells configured to the associated DOCA devemu PCI device. The
 * default num_db value configured for any DOCA devemu PCI device created by doca_devemu_pci_dev_create(), if not
 * configured otherwise, is equal to the value returned in doca_devemu_pci_cap_type_get_max_num_db().
 * @param [in] user_data_on_dpa
 * The user data that is associated with and can be retrieved by the DOCA devemu PCI device doorbell DPA handle.
 * @param [out] db
 * The newly created DOCA devemu PCI device doorbell.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_create_on_dpa(struct doca_devemu_pci_dev *pci_dev,
					      struct doca_devemu_pci_db_completion *db_comp,
					      uint8_t bar_id,
					      uint64_t bar_start_addr,
					      uint32_t db_id,
					      uint64_t user_data_on_dpa,
					      struct doca_devemu_pci_db **db);

/**
 * @brief Destroy the DOCA devemu PCI device doorbell.
 *
 * If the doorbell was created on dpa, the associated dpa handle will be destroyed as well.
 *
 * @param [in] db
 * The DOCA devemu PCI device doorbell to destroy. Must be stopped.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_destroy(struct doca_devemu_pci_db *db);

/**
 * @brief Get the DPA handle for the DOCA devemu PCI device doorbell.
 *
 * @param [in] db
 * The DOCA devemu PCI device doorbell previously created on DPA.
 * @param [out] db_handle
 * A pointer to the associated DPA handle in the dpa memory space.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_get_dpa_handle(struct doca_devemu_pci_db *db, doca_dpa_dev_devemu_pci_db_t *db_handle);

/**
 * @brief Start DOCA devemu PCI device doorbell. A started doorbell will be able to trigger completions on the
 * associated doorbell completion context. Therefore, in case the doorbell was created on DPA, one should bind
 * the associated doorbell handle to its doorbell completion context before starting the doorbell.
 *
 * @param [in] db
 * The DOCA devemu PCI device doorbell to start.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_start(struct doca_devemu_pci_db *db);

/**
 * @brief Stop DOCA devemu PCI device doorbell. A stopped doorbell will not trigger completions on the associated
 * doorbell completion context. Therefore, in case the doorbell was created on DPA, one should stop the doorbell before
 * un-binding the associated doorbell handle from its doorbell completion context.
 *
 * @param [in] db
 * The DOCA devemu PCI device doorbell to stop.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_stop(struct doca_devemu_pci_db *db);

/**
 * @brief Modify the current value of DOCA devemu PCI device doorbell. If the doorbell is started, this setting will
 * behave as if the doorbell value was modified by the PCI device driver. If doorbell value will not be modified before
 * starting the DOCA devemu PCI device doorbell, the device will keep the current value of the doorbell.
 *
 * @param [in] db
 * The DOCA devemu PCI device doorbell to modify.
 * @param [in] db_value
 * The new value of the DOCA devemu PCI device doorbell.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_modify_value(struct doca_devemu_pci_db *db, uint32_t db_value);

/**
 * @brief Query the current value of DOCA devemu PCI device doorbell.
 *
 * @param [in] db
 * The DOCA devemu PCI device doorbell to query.
 * @param [out] db_value
 * The current value of the DOCA devemu PCI device doorbell.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_db_query_value(struct doca_devemu_pci_db *db, uint32_t *db_value);

/*********************************************************************************************************************
 * DOCA devemu pci MSIX
 *********************************************************************************************************************/

/**
 * @brief Allocate DOCA devemu PCI device MSI-X context on DPA.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device to be associated with the MSI-X. Must be started.
 * @param [in] bar_id
 * The identifier of the BAR that contains the associated MSI-X table region for the created msix. This value must
 * conform with the identifier that was configured to the MSI-X table region during the configuration cycle of the pci
 * type that is associated with the given PCI device.
 * @param [in] bar_start_addr
 * The start address of the associated MSI-X table region within the BAR. This value must conform with the start
 * address that was configured to the MSI-X table region during the configuration cycle of the PCI type that is
 * associated with the given PCI device.
 * @param [in] msix_idx
 * The associated MSI-X table entry index.
 * @param [in] user_data_on_dpa
 * The user data that is associated with and can be retrieved by the DOCA devemu PCI device MSI-X DPA handle.
 * @param [out] msix
 * The newly created DOCA devemu PCI device MSI-X.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_msix_create_on_dpa(struct doca_devemu_pci_dev *pci_dev,
						uint8_t bar_id,
						uint64_t bar_start_addr,
						uint16_t msix_idx,
						uint64_t user_data_on_dpa,
						struct doca_devemu_pci_msix **msix);

/**
 * @brief Destroy the DOCA devemu PCI device MSI-X.
 *
 * If the MSI-X was created on dpa, the associated dpa handle will be destroyed as well.
 *
 * @param [in] msix
 * The DOCA devemu PCI device MSI-X context to destroy.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_msix_destroy(struct doca_devemu_pci_msix *msix);

/**
 * @brief Get the DPA handle for the DOCA devemu PCI device MSI-X.
 *
 * @param [in] msix
 * The DOCA devemu PCI device MSI-X previously created on DPA.
 * @param [out] msix_handle
 * A pointer to the associated DPA handle in the dpa memory space.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_msix_get_dpa_handle(struct doca_devemu_pci_msix *msix,
						 doca_dpa_dev_devemu_pci_msix_t *msix_handle);

/*********************************************************************************************************************
 * DOCA devemu MMAP
 *********************************************************************************************************************/

/**
 * @brief Allocates zero size memory map object with default/unset attributes associated with a DOCA devemu PCI device.
 *
 * @details The returned memory map object can be manipulated with common doca_mmap APIs.
 *
 * The created memory map object will cover a memory range in the domain that hosts the DOCA devemu PCI device.
 *
 * @param [in] pci_dev
 * The DOCA devemu PCI device to be associated with the doca_mmap. Must be started.
 * @param [out] mmap
 * DOCA memory map structure with default/unset attributes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_mmap_create(struct doca_devemu_pci_dev *pci_dev, struct doca_mmap **mmap);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DEVEMU_PCI_H_ */
