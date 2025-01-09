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
 * @file doca_devemu_pci_type.h
 * @page doca devemu pci type
 * @defgroup DEVEMU device emulation pci library
 *
 * DOCA PCI type for emulated pci devices
 *
 * @{
 */

#ifndef DOCA_DEVEMU_PCI_TYPE_H_
#define DOCA_DEVEMU_PCI_TYPE_H_

#include <stdint.h>
#include <sys/uio.h>

#include <doca_error.h>
#include <doca_dev.h>
#include <doca_ctx.h>
#include <doca_pe.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************************
 * DOCA devemu pci type
 *********************************************************************************************************************/

/**
 * @brief Opaque structure representing emulated pci device type.
 * This structure is used by pci device emulation applications, libraries and services.
 */
struct doca_devemu_pci_type;

/**
 * @brief Maximal length for the NULL terminated string that describe the name of the emulated PCI device type.
 */
#define DOCA_DEVEMU_PCI_TYPE_NAME_LEN 32

/**
 * @brief Get the maximum number of PCI types that can be created by the device.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_pci_types
 * Number of PCI types that can be created using doca_devemu_pci_type_create().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_get_max_types(const struct doca_devinfo *devinfo, uint16_t *max_pci_types);

/**
 * @brief Get the minimal BAR size (in Log base 2) that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] log_bar_size
 * The minimal BAR size, given in bytes, of single BAR in Log (base 2) units.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_log_min_bar_size(const struct doca_devinfo *devinfo, uint8_t *log_bar_size);

/**
 * @brief Get the maximum BAR size (in Log base 2) that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] log_bar_size
 * The maximal BAR size, given in bytes, of single BAR in Log (base 2) units.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_log_max_bar_size(const struct doca_devinfo *devinfo, uint8_t *log_bar_size);

/**
 * @brief Get the maximal number of MSIXs that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() and for any PCI device that is associated with a PCI type that was created using
 * doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] num_msix
 * The maximal number of MSIXs.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_max_num_msix(const struct doca_devinfo *devinfo, uint16_t *num_msix);

/**
 * @brief Get the maximal number of doorbells that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() and for any PCI device that is associated with a PCI type that was created using
 * doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] num_db
 * The maximal number of doorbells.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_max_num_db(const struct doca_devinfo *devinfo, uint16_t *num_db);

/**
 * @brief Get the minimal doorbell size (in Log base 2) that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] log_db_size
 * The minimal doorbell size, given in bytes, of single doorbell in Log (base 2) units.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_log_min_db_size(const struct doca_devinfo *devinfo, uint8_t *log_db_size);

/**
 * @brief Get the maximum doorbell size (in Log base 2) that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] log_db_size
 * The maximal doorbell size, given in bytes, of single doorbell in Log (base 2) units.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_log_max_db_size(const struct doca_devinfo *devinfo, uint8_t *log_db_size);

/**
 * @brief Get the minimal stride size (in Log base 2) of a single doorbell that can be configured for any PCI type
 * that was created using doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] log_stride_size
 * The minimal single doorbell stride size, given in bytes, of single doorbell in Log (base 2) units.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_log_min_db_stride_size(const struct doca_devinfo *devinfo,
								 uint8_t *log_stride_size);

/**
 * @brief Get the maximal stride size (in Log base 2) of a single doorbell that can be configured for any PCI type
 * that was created using doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] log_stride_size
 * The maximal single doorbell stride size, given in bytes, of single doorbell in Log (base 2) units.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_log_max_db_stride_size(const struct doca_devinfo *devinfo,
								 uint8_t *log_stride_size);

/**
 * @brief Get the maximum number of BARs that can be configured to an emulated PCI device, for any PCI type that was
 * created using doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_bars
 * Number of BARs that can be configured for any pci type using doca_devemu_pci_type_*_bar_conf_set().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_max_bars(const struct doca_devinfo *devinfo, uint8_t *max_bars);

/**
 * @brief Get the maximum number of BAR regions that can be configured to an emulated PCI device, for any PCI type that
 * was created using doca_devemu_pci_type_create(), per BAR.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_bar_regions
 * Number of BAR regions that can be configured per BAR, for any pci type, using
 * doca_devemu_pci_type_bar_*_region_conf_set().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_get_max_bar_regions(const struct doca_devinfo *devinfo, uint32_t *max_bar_regions);

/**
 * @brief Get the maximum number of BAR regions that can be configured to an emulated PCI device, for any PCI type that
 * was created using doca_devemu_pci_type_create().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_bar_regions
 * Number of BAR regions that can be configured, for any pci type, using doca_devemu_pci_type_bar_*_region_conf_set().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_max_bar_regions(const struct doca_devinfo *devinfo,
							  uint32_t *max_bar_regions);

/**
 * @brief Check if the device supports configuring doorbell regions that can identify doorbells by their offset in the
 * BAR. If supported, one can configure a doorbell region using
 * doca_devemu_pci_type_set_bar_db_region_by_offset_conf().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] supported
 * 1 if the doorbell region by offset supported, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_db_region_is_identify_by_offset_supported(const struct doca_devinfo *devinfo,
									       uint8_t *supported);

/**
 * @brief Check if the device supports configuring doorbell regions that can identify doorbells by their data.
 * If supported, one can configure a doorbell region using doca_devemu_pci_type_set_bar_db_region_by_data_conf().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] supported
 * 1 if the doorbell region by data supported, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_db_region_is_identify_by_data_supported(const struct doca_devinfo *devinfo,
									     uint8_t *supported);

/**
 * @brief Get the region block size of a doorbell BAR region that can be configured to an emulated PCI device, for any
 * PCI type that was created using doca_devemu_pci_type_create() or will be created by it. The region block size is the
 * smallest allocation data unit for a BAR region. For example, if the region block size is 64B then the bar region
 * size can be 64B/128B/192B/../N*64B (N = max num region blocks per doorbell BAR region).
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] block_size
 * Region block size, in bytes, of a doorbell BAR region that will be configured using
 * doca_devemu_pci_type_bar_db_region_<*>_conf_set(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_db_region_get_region_block_size(const struct doca_devinfo *devinfo,
								     uint32_t *block_size);

/**
 * @brief Get the maximum number of region blocks of a single doorbell BAR region that can be configured for any PCI
 * type that was created using doca_devemu_pci_type_create() or will be created by it.
 * The maximal number of region blocks together with the region block size defines the maximal size of a single
 * doorbell region.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_blocks
 * Maximal number of region blocks for a single doorbell BAR region that will be configured using
 * doca_devemu_pci_type_bar_db_region_<*>_conf_set(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_db_region_get_max_num_region_blocks(const struct doca_devinfo *devinfo,
									 uint32_t *max_blocks);

/**
 * @brief Get the maximum amount of doorbell BAR regions that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() or will be created by it.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_regions
 * Maximal number of doorbell BAR regions that can be configured using
 * doca_devemu_pci_type_bar_db_region_<*>_conf_set(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_max_bar_db_regions(const struct doca_devinfo *devinfo, uint32_t *max_regions);

/**
 * @brief Get the maximum number of BAR doorbell regions that can be configured to an emulated PCI device, for any PCI
 * type that was created using doca_devemu_pci_type_create(), per BAR.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_regions
 * Number of BAR doorbell regions that can be configured per BAR, for any pci type, using
 * doca_devemu_pci_type_bar_db_region_<*>_conf_set().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_get_max_bar_db_regions(const struct doca_devinfo *devinfo, uint32_t *max_regions);

/**
 * @brief Get the doorbell BAR region start address alignment that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() or will be created by it.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] alignment
 * The start address alignment, in bytes, of doorbell BAR regions that can be configured using
 * doca_devemu_pci_type_bar_db_region_<*>_conf_set(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_db_region_get_start_addr_alignment(const struct doca_devinfo *devinfo,
									uint32_t *alignment);

/**
 * @brief Get the region block size of a stateful BAR region that can be configured to an emulated PCI device, for any
 * PCI type that was created using doca_devemu_pci_type_create() or will be created by it. The region block size is the
 * smallest allocation data unit for a BAR region. For example, if the region block size is 64B then the bar region
 * size can be 64B/128B/192B/../N*64B (N = max num region blocks per stateful BAR region).
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] block_size
 * Region block size, in bytes, of a stateful BAR region that will be configured using
 * doca_devemu_pci_type_set_bar_stateful_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_stateful_region_get_region_block_size(const struct doca_devinfo *devinfo,
									   uint32_t *block_size);

/**
 * @brief Get the maximum number of region blocks of a single stateful BAR region that can be configured for any PCI
 * type that was created using doca_devemu_pci_type_create() or will be created by it.
 * The maximal number of region blocks together with the region block size defines the maximal size of a single
 * stateful region.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_blocks
 * Maximal number of region blocks for a single stateful BAR region that will be configured using
 * doca_devemu_pci_type_set_bar_stateful_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_stateful_region_get_max_num_region_blocks(const struct doca_devinfo *devinfo,
									       uint32_t *max_blocks);

/**
 * @brief Get the maximum amount of stateful BAR regions that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() or will be created by it.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_regions
 * Maximal number of stateful BAR regions that can be configured using
 * doca_devemu_pci_type_set_bar_stateful_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_max_bar_stateful_regions(const struct doca_devinfo *devinfo,
								   uint32_t *max_regions);

/**
 * @brief Get the maximum number of BAR stateful regions that can be configured to an emulated PCI device, for any PCI
 * type that was created using doca_devemu_pci_type_create(), per BAR.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_regions
 * Number of BAR stateful regions that can be configured per BAR, for any pci type, using
 * doca_devemu_pci_type_set_bar_stateful_region_conf().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_get_max_bar_stateful_regions(const struct doca_devinfo *devinfo,
								  uint32_t *max_regions);

/**
 * @brief Get the stateful BAR region start address alignment that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() or will be created by it.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] alignment
 * The start address alignment, in bytes, of stateful BAR regions that can be configured using
 * doca_devemu_pci_type_set_bar_stateful_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_stateful_region_get_start_addr_alignment(const struct doca_devinfo *devinfo,
									      uint32_t *alignment);

/**
 * @brief Get the region block size of a MSI-X table BAR region that can be configured to an emulated PCI device, for
 * any PCI type that was created using doca_devemu_pci_type_create() or will be created by it. The region block size is
 * the smallest allocation data unit for a BAR region. For example, if the region block size is 64B then the bar region
 * size can be 64B/128B/192B/../N*64B (N = max num region blocks per MSI-X table BAR region).
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] block_size
 * Region block size, in bytes, of a MSI-X table BAR region that will be configured using
 * doca_devemu_pci_type_set_bar_msix_table_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_srror_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_msix_table_region_get_region_block_size(const struct doca_devinfo *devinfo,
									     uint32_t *block_size);

/**
 * @brief Get the maximum number of region blocks of a single MSI-X table BAR region that can be configured for any PCI
 * type that was created using doca_devemu_pci_type_create() or will be created by it.
 * The maximal number of region blocks together with the region block size defines the maximal size of a single
 * MSI-X table region.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_blocks
 * Maximal number of region blocks for a single MSI-X table BAR region that will be configured using
 * doca_devemu_pci_type_set_bar_msix_table_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_msix_table_region_get_max_num_region_blocks(const struct doca_devinfo *devinfo,
										 uint32_t *max_blocks);

/**
 * @brief Get the maximum amount of MSI-X table BAR regions that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() or will be created by it.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_regions
 * Maximal number of MSI-X table BAR regions that can be configured using
 * doca_devemu_pci_type_set_bar_msix_table_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_max_bar_msix_table_regions(const struct doca_devinfo *devinfo,
								     uint32_t *max_regions);

/**
 * @brief Get the maximum number of BAR MSI-X table regions that can be configured to an emulated PCI device, for any
 * PCI type that was created using doca_devemu_pci_type_create(), per BAR.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_regions
 * Number of BAR MSI-X table regions that can be configured per BAR, for any pci type, using
 * doca_devemu_pci_type_set_bar_msix_table_region_conf().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_get_max_bar_msix_table_regions(const struct doca_devinfo *devinfo,
								    uint32_t *max_regions);

/**
 * @brief Get the MSI-X table BAR region start address alignment that can be configured for any PCI type that was
 * created using doca_devemu_pci_type_create() or will be created by it.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] alignment
 * The start address alignment, in bytes, of MSI-X table BAR regions that can be configured using
 * doca_devemu_pci_type_set_bar_msix_table_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_msix_table_region_get_start_addr_alignment(const struct doca_devinfo *devinfo,
										uint32_t *alignment);

/**
 * @brief Get the region block size of a MSI-X PBA BAR region that can be configured to an emulated PCI device, for any
 * PCI type that was created using doca_devemu_pci_type_create() or will be created by it. The region block size is the
 * smallest allocation data unit for a BAR region. For example, if the region block size is 64B then the bar region
 * size can be 64B/128B/192B/../N*64B (N = max num region blocks per MSI-X PBA BAR region).
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] block_size
 * Region block size, in bytes, of a MSI-X PBA BAR region that will be configured using
 * doca_devemu_pci_type_set_bar_msix_pba_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_srror_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_msix_pba_region_get_region_block_size(const struct doca_devinfo *devinfo,
									   uint32_t *block_size);

/**
 * @brief Get the maximum number of region blocks of a single MSI-X PBA BAR region that can be configured for any PCI
 * type that was created using doca_devemu_pci_type_create() or will be created by it.
 * The maximal number of region blocks together with the region block size defines the maximal size of a single
 * MSI-X PBA region.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_blocks
 * Maximal number of region blocks for a single MSI-X PBA BAR region that will be configured using
 * doca_devemu_pci_type_set_bar_msix_pba_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_msix_pba_region_get_max_num_region_blocks(const struct doca_devinfo *devinfo,
									       uint32_t *max_blocks);

/**
 * @brief Get the maximum amount of MSI-X PBA BAR regions that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() or will be created by it.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_regions
 * Maximal number of MSI-X PBA BAR regions that can be configured using
 * doca_devemu_pci_type_set_bar_msix_pba_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_get_max_bar_msix_pba_regions(const struct doca_devinfo *devinfo,
								   uint32_t *max_regions);

/**
 * @brief Get the maximum number of BAR MSI-X PBA regions that can be configured to an emulated PCI device, for any PCI
 * type that was created using doca_devemu_pci_type_create(), per BAR.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] max_regions
 * Number of BAR MSI-X PBA regions that can be configured per BAR, for any pci type, using
 * doca_devemu_pci_type_set_bar_msix_pba_region_conf().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_get_max_bar_msix_pba_regions(const struct doca_devinfo *devinfo,
								  uint32_t *max_regions);

/**
 * @brief Get the MSI-X PBA BAR region start address alignment that can be configured for any PCI type that was created
 * using doca_devemu_pci_type_create() or will be created by it.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] alignment
 * The start address alignment, in bytes, of MSI-X PBA BAR regions that can be configured using
 * doca_devemu_pci_type_set_bar_msix_pba_region_conf(), for any pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_bar_msix_pba_region_get_start_addr_alignment(const struct doca_devinfo *devinfo,
									      uint32_t *alignment);

/**
 * @brief According to the PCI specification a BAR that is mapped into Memory Space can define memory types.
 */
enum doca_devemu_pci_bar_mem_type {
	/**< Base register is 32 bits wide and can be mapped anywhere in the 32 address bit Memory Space. */
	DOCA_DEVEMU_PCI_BAR_MEM_TYPE_32_BIT = 0,
	/**< Base register support memory space below 1 MB */
	DOCA_DEVEMU_PCI_BAR_MEM_TYPE_1_MB,
	/**< Base register is 64 bits wide and can be mapped anywhere in the 64 address bit Memory Space. */
	DOCA_DEVEMU_PCI_BAR_MEM_TYPE_64_BIT,
};

/**
 * @brief Get the memory BAR types capability of the device. If supported, A BAR with that memory type can be
 * configured using doca_devemu_pci_type_set_memory_bar_conf().
 *
 * @param [in] devinfo
 * The device to query.
 * @param [in] memory_type
 * The BAR memory type to query.
 * @param [out] supported
 * 1 if the BAR memory type is supported by the device, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'devinfo' or 'supported' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_is_bar_mem_type_supported(const struct doca_devinfo *devinfo,
								enum doca_devemu_pci_bar_mem_type memory_type,
								uint8_t *supported);

/**
 * @brief Create a stopped DOCA devemu pci type.
 *
 * @param [in] name
 * The name to assign to the created DOCA devemu pci type.
 * The NULL terminated string must not exceed DOCA_DEVEMU_PCI_TYPE_NAME_LEN.
 * @param [out] pci_type
 * The created and stopped DOCA devemu pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'name' or 'pci_type' are NULL
 * - DOCA_ERROR_NO_MEMORY - allocation failure
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_create(const char *name, struct doca_devemu_pci_type **pci_type);

/**
 * @brief Destroy a DOCA devemu pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type to destroy. Must be stopped.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - type can't be destroyed while started. Need to stop it first - doca_devemu_pci_type_stop()
 * - DOCA_ERROR_NOT_PERMITTED - type was not created using doca_devemu_pci_type_create()
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_destroy(struct doca_devemu_pci_type *pci_type);

/**
 * @brief Start a DOCA devemu pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type to start.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL or invalid PCI parameters were previously provided
 * - DOCA_ERROR_BAD_STATE - type is already started
 * - DOCA_ERROR_NOT_FOUND - device was not provided - use doca_devemu_pci_type_set_dev()
 * - DOCA_ERROR_NO_MEMORY - allocation failure
 * - DOCA_ERROR_DRIVER - internal doca driver error
 * @note This method upon success disable the ability to configure the DOCA devemu pci type.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_start(struct doca_devemu_pci_type *pci_type);

/**
 * @brief Stop a DOCA devemu pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type to stop. Must not be associated to any pci device.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - type is already stopped
 * - DOCA_ERROR_DRIVER - internal doca driver error
 * @note This method upon success re-enable the ability to configure the DOCA devemu pci type.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_stop(struct doca_devemu_pci_type *pci_type);

/**
 * @brief Check whether the pci type is started.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] started
 * 1 if the pci_type is started, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'started' are NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_is_started(const struct doca_devemu_pci_type *pci_type, uint8_t *started);

/**
 * @brief Create list of available representor devices for a given devemu pci type.
 *
 * @details Returns all representors that are associated with the provided devemu pci type family.
 * The provided pci type must be started.
 * A representor can either represent an emulated PCI function that is currently attached to the host PCI subsystem or
 * an emulated PCI function intended for hotplugging into the host PCI subsystem.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must be started.
 * @param [out] dev_list_rep
 * Pointer to array of pointers. Output can then be accessed as follows (*dev_list_rep)[idx].
 * @param [out] nb_devs_rep
 * Number of available representor devices.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 * @note Returned list must be destroyed using doca_devinfo_rep_destroy_list()
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_create_rep_list(struct doca_devemu_pci_type *pci_type,
						  struct doca_devinfo_rep ***dev_list_rep,
						  uint32_t *nb_devs_rep);

/**
 * @brief Get the doca device of a pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type to query.
 * @param [out] dev
 * DOCA dev.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'dev' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_dev(const struct doca_devemu_pci_type *pci_type, struct doca_dev **dev);

/**
 * @brief Set the doca device of a pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type to modify. Must not be started.
 * @param [in] dev
 * DOCA dev.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'dev' are NULL
 * - DOCA_ERROR_BAD_STATE - can't configure type after starting it
 * - DOCA_ERROR_NOT_PERMITTED - can't set a device for default pci types
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_dev(struct doca_devemu_pci_type *pci_type, struct doca_dev *dev);

/**
 * @brief Get the hotplug capability of the device for a given pci type.
 *
 * @details Get uint8_t value defining if the device can be used to hotplug devices for a specific type.
 * The hotplug capability of a device implies its management capability.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] supported
 * 1 if the hotplug capability is supported for this type, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'devinfo', 'pci_type' or 'supported' are NULL or devinfo doesn't match associated
 *   doca_dev, in case doca_dev already was assigned to the pci_type
 * - DOCA_ERROR_DRIVER - internal doca driver error
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_is_hotplug_supported(const struct doca_devinfo *devinfo,
							   const struct doca_devemu_pci_type *pci_type,
							   uint8_t *supported);

/**
 * @brief Get the management capability of the device for a given pci type.
 *
 * @details Get uint8_t value defining if the device can be used to manage devices for a specific type.
 * The management capability of a device doesn't imply its hotplug capability.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] supported
 * 1 if the management capability is supported for this type, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'devinfo', 'pci_type' or 'supported' are NULL or devinfo doesn't match associated
 *   doca_dev, in case doca_dev already was assigned to the pci_type
 * - DOCA_ERROR_DRIVER - internal doca driver error
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_cap_type_is_mgmt_supported(const struct doca_devinfo *devinfo,
							const struct doca_devemu_pci_type *pci_type,
							uint8_t *supported);

/**
 * @brief Create a new representor device for a given pci type.
 *
 * @details The representor device will not be visible to the host. The representor device will be discoverable using
 * the discovery mechanism for representors.
 *
 * @param [in] pci_type
 * The DOCA pci type. Must be started.
 * @param [out] dev_rep
 * Initialized representor doca device instance on success. Valid on success only.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'dev_rep' are NULL
 * - DOCA_ERROR_BAD_STATE - type is not started
 * - DOCA_ERROR_DRIVER - internal doca driver error
 * @note pci type must be started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_create_rep(const struct doca_devemu_pci_type *pci_type, struct doca_dev_rep **dev_rep);

/**
 * @brief Destroy a representor device that was created using doca_devemu_pci_dev_create_rep().
 *
 * @details The representor device will not be visible to the host. The representor device will not be discoverable
 * using the discovery mechanism for representors.
 *
 * @param [in] rep_dev
 * Previously initialized representor device instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'rep_dev' is NULL or corrupted
 * - DOCA_ERROR_NOT_PERMITTED - representor was not created using doca_devemu_pci_dev_create_rep()
 * - DOCA_ERROR_DRIVER - internal doca driver error
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_dev_destroy_rep(struct doca_dev_rep *rep_dev);

/**
 * @brief Get the name of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] name
 * The name of the pci type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'name' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_name(const struct doca_devemu_pci_type *pci_type,
					   char (*name)[DOCA_DEVEMU_PCI_TYPE_NAME_LEN]);

/**
 * @brief Get the PCI Device ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] device_id
 * The PCI Device ID (DID) assigned by the vendor.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'device_id' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_device_id(const struct doca_devemu_pci_type *pci_type, uint16_t *device_id);

/**
 * @brief Set the PCI Device ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to modify. Must not be started.
 * @param [in] device_id
 * The PCI Device ID (DID) to assign.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - can't configure type after starting it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_device_id(struct doca_devemu_pci_type *pci_type, uint16_t device_id);

/**
 * @brief Get the PCI Vendor ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] vendor_id
 * The PCI Vendor ID (VID) allocated by the PCI-SIG.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'vendor_id' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_vendor_id(const struct doca_devemu_pci_type *pci_type, uint16_t *vendor_id);

/**
 * @brief Set the PCI Vendor ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to modify. Must not be started.
 * @param [in] vendor_id
 * The PCI Vendor ID (VID) allocated by the PCI-SIG.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - can't configure type after starting it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_vendor_id(struct doca_devemu_pci_type *pci_type, uint16_t vendor_id);

/**
 * @brief Get the PCI Subsystem ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] subsystem_id
 * The PCI Subsystem ID (SSID) assigned by the subsystem vendor.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'subsystem_id' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_subsystem_id(const struct doca_devemu_pci_type *pci_type, uint16_t *subsystem_id);

/**
 * @brief Set the PCI Subsystem ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to modify. Must not be started.
 * @param [in] subsystem_id
 * The PCI Subsystem ID (SSID) to assign.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - can't configure type after starting it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_subsystem_id(struct doca_devemu_pci_type *pci_type, uint16_t subsystem_id);

/**
 * @brief Get the PCI Subsystem Vendor ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] subsystem_vid
 * The PCI Subsystem Vendor ID (SVID) allocated by the PCI-SIG.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'subsystem_vid' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_subsystem_vendor_id(const struct doca_devemu_pci_type *pci_type,
							  uint16_t *subsystem_vid);

/**
 * @brief Set the PCI Subsystem Vendor ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to modify. Must not be started.
 * @param [in] subsystem_vid
 * The PCI Subsystem Vendor ID (SVID) allocated by the PCI-SIG.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - can't configure type after starting it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_subsystem_vendor_id(struct doca_devemu_pci_type *pci_type,
							  uint16_t subsystem_vid);

/**
 * @brief Get the PCI Revision ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] revision_id
 * The PCI Revision ID assigned by the vendor.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'revision_id' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_revision_id(const struct doca_devemu_pci_type *pci_type, uint8_t *revision_id);

/**
 * @brief Set the PCI Revision ID of a pci type.
 *
 * @param [in] pci_type
 * The DOCA pci type to modify. Must not be started.
 * @param [in] revision_id
 * The PCI Revision ID to assign.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - can't configure type after starting it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_revision_id(struct doca_devemu_pci_type *pci_type, uint8_t revision_id);

/**
 * @brief Get the PCI Class Code of a pci type to identify generic operation.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] class_code
 * The PCI Class Code to identify generic operation. Only 24 LSBits are valid.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'class_code' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_class_code(const struct doca_devemu_pci_type *pci_type, uint32_t *class_code);

/**
 * @brief Set the PCI Class Code of a pci type to identify generic operation.
 *
 * @param [in] pci_type
 * The DOCA pci context to modify. Must not be started.
 * @param [in] class_code
 * The PCI Class Code to identify generic operation. Only 24 LSBits are valid.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - can't configure type after starting it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_class_code(struct doca_devemu_pci_type *pci_type, uint32_t class_code);

/**
 * @brief Get the size of the MSI-X Table from MSI-X Capability Registers (1 based) of a pci type. This value will be
 * used as the default num_msix value for associated DOCA devemu pci devices, unless configured otherwise.
 *
 * @param [in] pci_type
 * The DOCA pci type to query.
 * @param [out] num_msix
 * The size of the MSI-X Table from MSI-X Capability Registers (1 based).
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' or 'num_msix' are NULL
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_get_num_msix(const struct doca_devemu_pci_type *pci_type, uint16_t *num_msix);

/**
 * @brief Set the size of the MSI-X Table from MSI-X Capability Registers (1 based) of a pci type. Must conform with
 * doca_devemu_pci_cap_type_get_max_num_msix().
 *
 * @param [in] pci_type
 * The DOCA pci type to modify. Must not be started.
 * @param [in] num_msix
 * The size of the MSI-X Table from MSI-X Capability Registers (1 based).
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - 'pci_type' is NULL
 * - DOCA_ERROR_BAD_STATE - can't configure type after starting it
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_num_msix(struct doca_devemu_pci_type *pci_type, uint16_t num_msix);

/**
 * @brief Set a memory BAR layout configuration for DOCA devemu pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must not be started.
 * @param [in] id
 * The BAR id.
 * @param [in] log_sz
 * The BAR size, in Log (base 2) units. Must be set to 0 if previous BAR requires an extension (e.g. for 64-bit BARs).
 * Otherwise, this value must conform with doca_devemu_pci_cap_type_get_log_min_bar_size() and
 * doca_devemu_pci_cap_type_get_log_max_bar_size() capabilities.
 * @param [in] memory_type
 * Memory type value to expose for this BAR.
 * @param [in] prefetchable
 * Prefetchable bit value to expose for this BAR. Set to 1 if the BAR does not contain locations with side effects on
 * reads. Set to 0 if the BAR contains locations with read side effects or locations in which the function does not
 * tolerate write merging.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 * @note This configuration is applicable only for type created by doca_devemu_pci_type_create()
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_memory_bar_conf(struct doca_devemu_pci_type *pci_type,
						      uint8_t id,
						      uint8_t log_sz,
						      enum doca_devemu_pci_bar_mem_type memory_type,
						      uint8_t prefetchable);

/**
 * @brief Set an IO BAR layout configuration for DOCA devemu pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must not be started.
 * @param [in] id
 * The BAR id.
 * @param [in] log_sz
 * The BAR size, in Log (base 2) units. Must be set to 0 if previous BAR requires an extension (e.g. for 64-bit BARs).
 * Otherwise, this value must conform with doca_devemu_pci_cap_type_get_log_min_bar_size() and
 * doca_devemu_pci_cap_type_get_log_max_bar_size() capabilities.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 * @note This configuration is applicable only for type created by doca_devemu_pci_type_create()
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_io_bar_conf(struct doca_devemu_pci_type *pci_type, uint8_t id, uint8_t log_sz);

/**
 * @brief Set a doorbell BAR region configuration for a BAR layout in a DOCA devemu pci type. The doorbells that will
 * be associated with this doorbell BAR region will be identified according to their offset within the BAR.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must not be started.
 * @param [in] id
 * The BAR id that will contain the new region.
 * @param [in] start_addr
 * The start address of the region within the BAR. This value must conform with the start address alignment capability
 * from doca_devemu_pci_cap_bar_db_region_get_start_addr_alignment().
 * @param [in] size
 * The size of the region in bytes. Must conform with doca_devemu_pci_cap_bar_db_region_get_region_block_size() and
 * doca_devemu_pci_cap_bar_db_region_get_max_num_region_blocks().
 * @param [in] log_db_size
 * The size, given in bytes, of single doorbell in Log (base 2) units. This value must conform with
 * doca_devemu_pci_cap_type_get_log_min_db_size() and doca_devemu_pci_cap_type_get_log_max_db_size().
 * @param [in] log_stride_size
 * The size, given in bytes, of a single doorbell stride in Log (base 2) units. Thie value must conform with
 * doca_devemu_pci_cap_type_get_log_min_db_stride_size() and
 * doca_devemu_pci_cap_type_get_log_max_db_stride_size().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_bar_db_region_by_offset_conf(struct doca_devemu_pci_type *pci_type,
								   uint8_t id,
								   uint64_t start_addr,
								   uint64_t size,
								   uint8_t log_db_size,
								   uint8_t log_stride_size);

/**
 * @brief Set a doorbell BAR region configuration for a BAR layout in a DOCA devemu pci type. The doorbells that will
 * be associated with this doorbell BAR region will be identified according to the data written to the doorbell.
 * The doorbell identifier will be deduced from the written doorbell data by the db_id_lsbyte and db_id_msbyte
 * settings.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must not be started.
 * @param [in] id
 * The BAR id that will contain the new region.
 * @param [in] start_addr
 * The start address of the region within the BAR. This value must conform with the start address alignment capability
 * from doca_devemu_pci_cap_bar_db_region_get_start_addr_alignment().
 * @param [in] size
 * The size of the region in bytes. Must conform with doca_devemu_pci_cap_bar_db_region_get_region_block_size() and
 * doca_devemu_pci_cap_bar_db_region_get_max_num_region_blocks().
 * @param [in] log_db_size
 * The size, given in bytes, of single doorbell in Log (base 2) units. This value must conform with
 * doca_devemu_pci_cap_type_get_log_min_db_size() and doca_devemu_pci_cap_type_get_log_max_db_size().
 * @param [in] db_id_msbyte
 * The start byte of the doorbell identifier, within the doorbell data written by the driver. If the
 * db_id_msbyte > db_id_lsbyte then the doorbell identifier will be treated as Little-Endian.
 * @param [in] db_id_lsbyte
 * The end byte of the doorbell identifier, within the doorbell data written by the driver. If the
 * db_id_msbyte > db_id_lsbyte then the doorbell identifier will be treated as Little-Endian.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_bar_db_region_by_data_conf(struct doca_devemu_pci_type *pci_type,
								 uint8_t id,
								 uint64_t start_addr,
								 uint64_t size,
								 uint8_t log_db_size,
								 uint16_t db_id_msbyte,
								 uint16_t db_id_lsbyte);

/**
 * @brief Set a MSI-X table BAR region configuration for a BAR layout in a DOCA devemu pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must not be started.
 * @param [in] id
 * The BAR id that will contain the new region.
 * @param [in] start_addr
 * The start address of the region within the BAR. This value must conform with the start address alignment capability
 * from doca_devemu_pci_cap_bar_msix_table_region_get_start_addr_alignment().
 * @param [in] size
 * The size of the region in bytes. Must correlate with the num_msix value of the pci type and conform with
 * doca_devemu_pci_cap_bar_msix_table_region_get_region_block_size() and
 * doca_devemu_pci_cap_bar_msix_table_region_get_max_num_region_blocks().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_bar_msix_table_region_conf(struct doca_devemu_pci_type *pci_type,
								 uint8_t id,
								 uint64_t start_addr,
								 uint64_t size);

/**
 * @brief Set a MSI-X PBA BAR region configuration for a BAR layout in a DOCA devemu pci type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must not be started.
 * @param [in] id
 * The BAR id that will contain the new region.
 * @param [in] start_addr
 * The start address of the region within the BAR. This value must conform with the start address alignment capability
 * from doca_devemu_pci_cap_bar_msix_pba_region_get_start_addr_alignment().
 * @param [in] size
 * The size of the region in bytes. Must correlate with the num_msix value of the pci type and conform with
 * doca_devemu_pci_cap_bar_msix_pba_region_get_region_block_size() and
 * doca_devemu_pci_cap_bar_msix_pba_region_get_max_num_region_blocks().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_bar_msix_pba_region_conf(struct doca_devemu_pci_type *pci_type,
							       uint8_t id,
							       uint64_t start_addr,
							       uint64_t size);

/**
 * @brief Set a stateful BAR region configuration for a BAR layout in a DOCA devemu pci type.
 *
 * @details The stateful BAR region will be used by the device to expose registers to the driver. This region will
 * be actively maintained by the device. PCI READ transactions for this region will be automatically answered by the
 * device. PCI WRITE transactions will be stored to this region by the device. The associated pci_dev will be notified
 * upon any PCI WRITE transaction to this region initiated by the device driver using the
 * doca_devemu_pci_dev_event_bar_stateful_region_driver_write event, if this event was register.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must not be started.
 * @param [in] id
 * The BAR id that will contain the new region.
 * @param [in] start_addr
 * The start address of the region within the BAR. This value must conform with the start address alignment capability
 * from doca_devemu_pci_cap_bar_stateful_region_get_start_addr_alignment().
 * @param [in] size
 * The size of the region in bytes. Must conform with doca_devemu_pci_cap_bar_stateful_region_get_region_block_size()
 * and doca_devemu_pci_cap_bar_stateful_region_get_max_num_region_blocks().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure. see doca_error_t.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devemu_pci_type_set_bar_stateful_region_conf(struct doca_devemu_pci_type *pci_type,
							       uint8_t id,
							       uint64_t start_addr,
							       uint64_t size);

/**
 * @brief Modify default registers values for a configured stateful region in a DOCA devemu pci type.
 *
 * @details This method will modify the default values for the entire stateful region registers area in a pci type BAR
 * (before the first modification, the initial default values of the stateful region registers are zeroed).
 * The doca_dev_rep that will be created using doca_devemu_pci_dev_create_rep() from the associated pci_type will
 * inherit these default values upon creation.
 * Furthermore, each pci_dev that will be created from the associated doca_dev_rep will have a chance to update its own
 * default values by calling doca_devemu_pci_dev_modify_bar_stateful_region_default_values() and not effecting the
 * default values of the pci_type.
 *
 * @param [in] pci_type
 * The DOCA devemu pci type. Must be started and must not be associated to any pci_dev or doca_dev_rep.
 * @param [in] id
 * The BAR id that contains the stateful region. This value must conform with the id provided during
 * doca_devemu_pci_type_set_bar_stateful_region_conf().
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
doca_error_t doca_devemu_pci_type_modify_bar_stateful_region_default_values(struct doca_devemu_pci_type *pci_type,
									    uint8_t id,
									    uint64_t start_addr,
									    void *default_values,
									    uint64_t size);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DEVEMU_PCI_TYPE_H_ */
