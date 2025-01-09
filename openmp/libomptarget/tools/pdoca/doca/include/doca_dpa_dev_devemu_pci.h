/*
 * Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_dpa_dev_devemu_pci.h
 * @page doca dpa devemu pci
 * @defgroup DPA_DEVEMU_PCI DOCA DPA devemu PCI
 * @ingroup DPA_DEVICE
 * DOCA DPA devemu PCI
 * @{
 */

#ifndef DOCA_DPA_DEV_DEVEMU_PCI_H_
#define DOCA_DPA_DEV_DEVEMU_PCI_H_

#include <doca_dpa_dev.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DPA Devemu PCI Device DB Completion handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_devemu_pci_db_completion_t;

/**
 * @brief DPA Devemu PCI Device DB Completion Element handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_devemu_pci_db_completion_element_t;

/**
 * @brief DPA Devemu PCI Device DB handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_devemu_pci_db_t;

/**
 * @brief DPA Devemu PCI Device MSI-X handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_devemu_pci_msix_t;

/**
 * @brief Get Devemu PCI Device DB completion element
 *
 * The returned completion element will stay valid until it is acked using - doca_dpa_dev_devemu_pci_db_completion_ack
 * Once it is acked then the element should not be used, using it will cause undefined behaviour
 *
 * The expected flow for single DB is as follows:
 * 1. if doca_dpa_dev_devemu_pci_get_db_completion() != 0:
 *   1.1. doca_dpa_dev_devemu_pci_db_completion_element_get_db_properties(&db)
 *   1.2. doca_dpa_dev_devemu_pci_db_completion_ack(1)
 *   1.3. doca_dpa_dev_devemu_pci_db_completion_request_notification()
 *   1.4. doca_dpa_dev_devemu_pci_db_request_notification(db)
 *   1.5. doca_dpa_dev_devemu_pci_db_get_value(db)
 *
 * The expected flow for multiple DBs is as follows:
 * 1. doca_dpa_dev_devemu_pci_db_t dbs[MAX_NUM_DBS]
 * 2. num_completions = 0
 * 3. while doca_dpa_dev_devemu_pci_get_db_completion() != 0:
 *   3.1. doca_dpa_dev_devemu_pci_db_completion_element_get_db_properties(&dbs[num_completions++])
 * 4. doca_dpa_dev_devemu_pci_db_completion_ack(num_completions)
 * 5. doca_dpa_dev_devemu_pci_db_completion_request_notification()
 * 6. for db in dbs: (safe to assume that dbs holds unique DBs)
 *   6.1. doca_dpa_dev_devemu_pci_db_request_notification(db)
 *   6.2. doca_dpa_dev_devemu_pci_db_get_value(db)
 *
 * Failure to follow these steps may result in some unexpected behaviour
 *
 * @param[in] comp - DPA Devemu PCI Device DB completion handle
 * @param[out] comp_element - DPA Devemu PCI Device DB completion element
 *
 * @return
 * This function returns 1 if there is completion element to return. Otherwise returns 0
 */
DOCA_EXPERIMENTAL
int doca_dpa_dev_devemu_pci_get_db_completion(doca_dpa_dev_devemu_pci_db_completion_t comp,
					      doca_dpa_dev_devemu_pci_db_completion_element_t *comp_element);

/**
 * @brief Acknowledge that the completions have been read on the Devemu PCI Device DB completion context
 *
 * Acks a number of completions that were previously received using doca_dpa_dev_devemu_pci_get_db_completion
 * After this operation the acked completions are no longer valid
 *
 * @param[in] comp - DPA Devemu PCI Device DB completion handle
 * @param[in] num_comp - Number of completion elements which have been read
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_devemu_pci_db_completion_ack(doca_dpa_dev_devemu_pci_db_completion_t comp, uint64_t num_comp);

/**
 * @brief Request notification on the Devemu PCI Device DB completion context
 *
 * Use this method in case you want the associated DPA thread to wake up once any new completion element is available
 *
 * @param[in] comp - DPA Devemu PCI Device DB completion handle
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_devemu_pci_db_completion_request_notification(doca_dpa_dev_devemu_pci_db_completion_t comp);

/**
 * @brief Bind Devemu PCI Device DB handle to the Devemu PCI Device DB completion context
 *
 * @param[in] comp - DPA Devemu PCI Device DB completion handle
 * @param[in] db - DPA Devemu PCI Device DB handle
 *
 * This function creates an association between a DB handle and the DB completion context that will be receiving
 * notifications for this DB. These notifications will generate DB completion element that can be retrieved by
 * doca_dpa_dev_devemu_pci_get_db_completion().
 * It is recommended to bind a DB to a DB completion context prior allowing any notifications to arrive for this DB.
 *
 * @return
 * This function returns 0 on success. Otherwise returns a negative value.
 */
DOCA_EXPERIMENTAL
int doca_dpa_dev_devemu_pci_db_completion_bind_db(doca_dpa_dev_devemu_pci_db_completion_t comp,
						  doca_dpa_dev_devemu_pci_db_t db);

/**
 * @brief Unbind Devemu PCI Device DB handle from the Devemu PCI Device DB completion context
 *
 * @param[in] comp - DPA Devemu PCI Device DB completion handle
 * @param[in] db - DPA Devemu PCI Device DB handle
 *
 * This function destroys the association between a DB handle and the associated DB completion context.
 * An unbinded DB will not generate any DB completion element that can be retrieved by
 * doca_dpa_dev_devemu_pci_get_db_completion().
 * It is recommended to unbind a DB from a DB completion context after disallowing any notifications to arrive for this
 * DB.
 *
 * @return
 * This function returns 0 on success. Otherwise returns a negative value.
 */
DOCA_EXPERIMENTAL
int doca_dpa_dev_devemu_pci_db_completion_unbind_db(doca_dpa_dev_devemu_pci_db_completion_t comp,
						    doca_dpa_dev_devemu_pci_db_t db);

/**
 * @brief Get Devemu PCI Device DB properties from DB completion element
 *
 * @param[in] comp - DPA Devemu PCI Device DB completion handle
 * @param[in] comp_element - DPA Devemu PCI Device DB completion element
 * @param[out] db - DPA Devemu PCI Device DB handle
 * @param[out] db_user_data - DB handle user data
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_devemu_pci_db_completion_element_get_db_properties(
	doca_dpa_dev_devemu_pci_db_completion_t comp,
	doca_dpa_dev_devemu_pci_db_completion_element_t comp_element,
	doca_dpa_dev_devemu_pci_db_t *db,
	doca_dpa_dev_uintptr_t *db_user_data);

/**
 * @brief Request notification on the Devemu PCI Device DB context
 *
 * Use this method in case you want the associated doca_dpa_dev_devemu_pci_db_completion_t to generate a completion
 * once a new doorbell value is written
 * @note need to make sure that previous completion element related to this DB has already been acked
 *
 * @param[in] db - DPA Devemu PCI Device DB handle
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_devemu_pci_db_request_notification(doca_dpa_dev_devemu_pci_db_t db);

/**
 * @brief Get value writted to the doorbell
 *
 * This will return the current value written to the doorbell
 * This API must be called only after doca_dpa_dev_devemu_pci_db_request_notification(),
 * otherwise the behavior is undefined
 * @note: The DPA handler can be invoked more than once for the same DB value, if a DB value is written by the device
 * driver after doca_dpa_dev_devemu_pci_db_request_notification(), but before doca_dpa_dev_devemu_pci_db_get_value()
 *
 * @param[in] db - DPA Devemu PCI Device DB handle
 *
 * @return
 * Returns the value written to this doorbell
 */
DOCA_EXPERIMENTAL
uint32_t doca_dpa_dev_devemu_pci_db_get_value(doca_dpa_dev_devemu_pci_db_t db);

/**
 * @brief Raise MSIX notification towards the driver
 *
 * @param[in] msix - DPA Devemu PCI Device MSIX handle
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_devemu_pci_msix_raise(doca_dpa_dev_devemu_pci_msix_t msix);

/**
 * @brief Get MSIX handle user data
 *
 * @param[in] msix - DPA Devemu PCI Device MSIX handle
 *
 * @return
 * Return the user data for the MSIX handle
 */
DOCA_EXPERIMENTAL
doca_dpa_dev_uintptr_t doca_dpa_dev_devemu_pci_msix_get_user_data(doca_dpa_dev_devemu_pci_msix_t msix);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DPA_DEV_DEVEMU_PCI_H_ */
