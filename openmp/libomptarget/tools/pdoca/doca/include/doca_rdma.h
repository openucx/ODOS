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
 * @file doca_rdma.h
 * @page DOCA RDMA
 * @defgroup DOCARDMA DOCA RDMA
 * DOCA RDMA library. For more details please refer to the user guide on DOCA devzone.
 *
 * @{
 */
#ifndef DOCA_RDMA_H_
#define DOCA_RDMA_H_

#include <stdbool.h>
#include <stdint.h>

#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * DOCA core opaque types
 *********************************************************************************************************************/

struct doca_buf;
struct doca_dev;
struct doca_devinfo;
struct doca_sync_event_remote_net;

/** Available transport types for RDMA */
enum doca_rdma_transport_type {
	DOCA_RDMA_TRANSPORT_TYPE_RC, /**< RC transport type */
	DOCA_RDMA_TRANSPORT_TYPE_DC, /**< DC transport type, currently not supported */
};

/** gid struct */
struct doca_rdma_gid {
	uint8_t raw[DOCA_GID_BYTE_LENGTH]; /**< The raw value of the GID */
};

/** Shared receive queue type */
enum doca_rdma_srq_type {
	DOCA_RDMA_SRQ_TYPE_LINKED_LIST, /**< Linked list type */
	DOCA_RDMA_SRQ_TYPE_CYCLIC,	/**< Cyclic type */
};

/*********************************************************************************************************************
 * DOCA RDMA & RDMA SRQ Opaques
 *********************************************************************************************************************/

/**
 * Opaque structure representing a DOCA RDMA instance.
 */
struct doca_rdma;

/**
 * Typedef representing a DOCA RDMA DPA handle instance.
 */
typedef uint64_t doca_dpa_dev_rdma_t;

/**
 * Opaque structure representing a DOCA RDMA GPU handle instance
 */
struct doca_gpu_dev_rdma;

/**
 * Opaque structure representing a DOCA RDMA SRQ instance.
 */
struct doca_rdma_srq;

/**
 * Opaque structure representing a DPA DOCA RDMA SRQ handle instance.
 */
typedef uint64_t doca_dpa_dev_rdma_srq_t;

/*********************************************************************************************************************
 * DOCA RDMA Context
 *********************************************************************************************************************/

/**
 * @brief Create a DOCA RDMA instance.
 *
 * @param [in] dev
 * The device to attach to the RDMA instance.
 * @param [out] rdma
 * Pointer to pointer to be set to point to the created doca_rdma instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - rdma argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate resources.
 * - DOCA_ERROR_INITIALIZATION - failed to initialize rdma.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_create(struct doca_dev *dev, struct doca_rdma **rdma);

/**
 * @brief Create a DOCA RDMA instance and associate it with a DOCA RDMA SRQ instance.
 *
 * @note The given DOCA RDMA SRQ must be started before using this function.
 * @note RDMA with SRQ is supported only for contexts that are set on DPA datapath,
 * using doca_ctx_set_datapath_on_dpa() before calling doca_ctx_start().
 *
 * @param [in] dev
 * The device to attach to the RDMA instance.
 * @param [in] rdma_srq
 * The RDMA SRQ instance to assign to the the RDMA instance.
 * @param [out] rdma
 * Pointer to pointer to be set to point to the created doca_rdma instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if the rdma_srq context was not started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_create_with_srq(struct doca_dev *dev, struct doca_rdma_srq *rdma_srq, struct doca_rdma **rdma);

/**
 * @brief Destroy a DOCA RDMA instance.
 *
 * @param [in] rdma
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - rdma argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - the associated ctx was not stopped before calling doca_rdma_destroy().
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_destroy(struct doca_rdma *rdma);

/**
 * @brief Convert doca_rdma instance into a generalised context for use with doca core objects.
 *
 * @param [in] rdma
 * RDMA instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_rdma_as_ctx(struct doca_rdma *rdma);

/**
 * @brief Export doca_rdma connection details object
 * The doca_rdma_conn_details are used in doca_rdma_connect().
 * Can only be called after calling doca_ctx_start().
 *
 * @param [in] rdma
 * Pointer doca_rdma to export connection details for.
 * @param [out] local_rdma_conn_details
 * Exported doca_rdma_conn_details object.
 * @param [out] local_rdma_conn_details_size
 * Size of exported doca_rdma_conn_details object.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if any of the parameters is NULL.
 * - DOCA_ERROR_BAD_STATE - if called before calling ctx_start().
 * @note stopping and restarting an RDMA context require calling doca_rdma_export() & doca_rdma_connect() again.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_export(const struct doca_rdma *rdma,
			      const void **local_rdma_conn_details,
			      size_t *local_rdma_conn_details_size);

/**
 * @brief Connect to remote doca_rdma peer.
 * Can only be called when the ctx is in DOCA_CTX_STATE_STARTING state (after calling doca_ctx_start()).
 * Once called, doca_pe_progress() should be called, in order to transition the ctx to DOCA_CTX_STATE_RUNNING state.
 * Only after that can tasks be allocated and submitted.
 *
 * @param [in] rdma
 * Pointer to doca_rdma to connect.
 * @param [in] remote_rdma_conn_details
 * Exported doca_rdma_conn_details object from remote peer.
 * @param [in] remote_rdma_conn_details_size
 * Size of remote doca_rdma_conn_details object.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if any of the parameters is NULL.
 * - DOCA_ERROR_BAD_STATE - if context was not started or rdma instance is already connected.
 * - DOCA_ERROR_CONNECTION_ABORTED - if connection failed or connection details object was corrupted.
 * @note stopping and restarting an RDMA context require calling doca_rdma_export() & doca_rdma_connect() again.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_connect(struct doca_rdma *rdma,
			       const void *remote_rdma_conn_details,
			       size_t remote_rdma_conn_details_size);

/*********************************************************************************************************************
 * DOCA RDMA capabilities
 *********************************************************************************************************************/

/**
 * @brief Get the maximal recv queue size for a specific device.
 * @note This capability is not relevant when using RDMA SRQ.
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [out] max_recv_queue_size
 * The maximal recv queue size for the given devinfo.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_get_max_recv_queue_size(const struct doca_devinfo *devinfo, uint32_t *max_recv_queue_size);

/**
 * Get the maximal send queue size for a specific device.
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [out] max_send_queue_size
 * The of the maximal send queue size for the given devinfo.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_get_max_send_queue_size(const struct doca_devinfo *devinfo, uint32_t *max_send_queue_size);

/**
 * @brief Get the maximal buffer list length property for buffers of tasks that are sent to the remote and in which
 * linked list are supported (i.e. send, send_imm, read, write, write_imm).
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [out] max_send_buf_list_len
 * Maximal buffer list length to used for buffers that support linked list in relevant tasks, for the given devinfo.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_get_max_send_buf_list_len(const struct doca_devinfo *devinfo,
						     uint32_t *max_send_buf_list_len);

/**
 * @brief Get the maximal message size for a specific device.
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [out] max_message_size
 * The maximal message size for the given devinfo.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_get_max_message_size(const struct doca_devinfo *devinfo, uint32_t *max_message_size);

/**
 * Get the gid table size for a specific device.
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [out] gid_table_size
 * The gid table size for the given devinfo.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_get_gid_table_size(const struct doca_devinfo *devinfo, uint32_t *gid_table_size);

/**
 * Get gids for a specific device by index and number of entries.
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [in] start_index
 * The first gid index of interest
 * @param [in] num_entries
 * The number of desired gid indices
 * @param [in,out] gid_array
 * A 'struct doca_rdma_gid' array of size 'num_entries', that on success will hold the desired gids.
 * Note that it is the user's responsibility to provide an array with enough entries to prevent data corruption
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_get_gid(const struct doca_devinfo *devinfo,
				   uint32_t start_index,
				   uint32_t num_entries,
				   struct doca_rdma_gid *gid_array);

/**
 * @brief Check if DOCA RDMA supports given transport type for a specific device.
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [in] transport_type
 * Transport type to query support for.
 *
 * @return
 * DOCA_SUCCESS - in case the transport type is supported.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the given transport type.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_transport_type_is_supported(const struct doca_devinfo *devinfo,
						       enum doca_rdma_transport_type transport_type);

/*********************************************************************************************************************
 * DOCA RDMA properties
 *********************************************************************************************************************/

/**
 * @brief Set send queue size property for doca_rdma.
 * The value can be queried using doca_rdma_get_send_queue_size().
 * Queue size will be rounded to the next power of 2.
 * can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma
 * doca_rdma context to set the property for.
 * @param [in] send_queue_size
 * Send queue size to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if the given size is not supported.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_send_queue_size(struct doca_rdma *rdma, uint32_t send_queue_size);

/**
 * @brief Set recv queue size property for doca_rdma.
 * The value can be queried using doca_rdma_get_recv_queue_size().
 * Queue size will be rounded to the next power of 2.
 * Can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma
 * doca_rdma context to set the property for.
 * @param [in] recv_queue_size
 * Recv queue size to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if the given size is not supported.
 * - DOCA_ERROR_BAD_STATE - if context is already started or if the given RDMA was created with SRQ.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_recv_queue_size(struct doca_rdma *rdma, uint32_t recv_queue_size);

/**
 * @brief Set the maximum buffer list length property for local buffers of tasks that are sent to the remote and in
 * which linked list are supported (i.e. send, send_imm, read, write, write_imm).
 * The value in use can be queried using doca_rdma_get_max_send_buf_list_len().
 * @note Can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma
 * The RDMA instance to set the property for.
 * @param [in] max_send_buf_list_len
 * Maximum buffer list length to use for local buffer in relevant tasks.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_max_send_buf_list_len(struct doca_rdma *rdma, uint32_t max_send_buf_list_len);

/**
 * @brief Set transport type for doca_rdma.
 * The value can be queried using doca_rdma_get_transport_type().
 * Can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma
 * doca_rdma context to set the property for.
 * @param [in] transport_type
 * Transport type to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if the given transport type is not supported.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_transport_type(struct doca_rdma *rdma, enum doca_rdma_transport_type transport_type);

/**
 * @brief Set MTU for doca_rdma.
 * The value can be queried using doca_rdma_get_mtu().
 * Can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma
 * doca_rdma context to set the property for.
 * @param [in] mtu
 * MTU to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if the given MTU is not supported.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 * - DOCA_ERROR_UNEXPECTED - if an unexpected error has occurred.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_mtu(struct doca_rdma *rdma, enum doca_mtu_size mtu);

/**
 * @brief Set rdma permissions for doca_rdma.
 * The value can be queried using doca_rdma_get_permissions().
 * Can only be called after calling doca_ctx_dev_add() and before calling doca_ctx_start().
 * The supported permissions are the RDMA access flags.
 *
 * @param [in] rdma
 * doca_rdma context to set the property for.
 * @param [in] permissions
 * Bitwise combination of RDMA access flags - see enum doca_access_flag
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given or non-RDMA access flags were given.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_permissions(struct doca_rdma *rdma, uint32_t permissions);

/**
 * @brief Set whether to use GRH in connection.
 * The value can be queried using doca_rdma_get_grh_enabled().
 * Can only be called before calling doca_ctx_start().
 *
 * If using IB device:
 * If GRH is disabled, the address will rely on LID only.
 * If GRH is enabled, the other side must also use GRH.
 *
 * If using ETH device, GRH must be enabled.
 *
 * @param [in] rdma
 * doca_rdma context to set the property for.
 * @param [in] grh_enabled
 * 1 if GRH is used in doca_rdma, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 * - DOCA_ERROR_NOT_SUPPORTED - if GRH setting is not supported for the device.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_grh_enabled(struct doca_rdma *rdma, uint8_t grh_enabled);

/**
 * @brief Set GID index for doca_rdma.
 * The value can be queried using doca_rdma_get_gid_index().
 * Can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma
 * doca_rdma context to set the property for.
 * @param [in] gid_index
 * GID index to use in doca_rdma.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_gid_index(struct doca_rdma *rdma, uint32_t gid_index);

/**
 * @brief Set SL (service level) for doca_rdma.
 * The value can be queried using doca_rdma_get_sl().
 * Can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma
 * doca_rdma context to set the property for.
 * @param [in] sl
 * SL to use in doca_rdma.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_set_sl(struct doca_rdma *rdma, uint32_t sl);

/**
 * @brief Get send queue size property from doca_rdma.
 * Returns the current send_queue_size set for the doca_rdma_context.
 * The size returned is the actual size being used and might differ from the size set by the user,
 * as the size may be increased.
 *
 * @param [in] rdma
 * doca_rdma context to get the property from.
 * @param [out] send_queue_size
 * Send queue size set in context.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_send_queue_size(const struct doca_rdma *rdma, uint32_t *send_queue_size);

/**
 * @brief Get recv queue size property from doca_rdma.
 * Returns the current recv_queue_size set for the doca_rdma_context.
 * The size returned is the actual size being used and might differ from the size set by the user,
 * as the size may be increased.
 *
 * @param [in] rdma
 * doca_rdma context to get the property from.
 * @param [out] recv_queue_size
 * Recv queue size set in context.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is already started or if the given RDMA was created with SRQ.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_recv_queue_size(const struct doca_rdma *rdma, uint32_t *recv_queue_size);

/**
 * @brief Get the maximum buffer list length property for local buffers of tasks that are sent to the remote and in
 * which linked list are supported (i.e. send, send_imm, read, write, write_imm).
 *
 * @param [in] rdma
 * The RDMA instance to get the property from.
 * @param [out] max_send_buf_list_len
 * Maximum buffer list length to used for local buffer in relevant tasks.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_max_send_buf_list_len(const struct doca_rdma *rdma, uint32_t *max_send_buf_list_len);

/**
 * @brief Get transport_type property from doca_rdma.
 * Returns the current transport_type set for the doca_rdma_context.
 *
 * @param [in] rdma
 * doca_rdma context to get the property from.
 * @param [out] transport_type
 * Transport_type set in context.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_transport_type(const struct doca_rdma *rdma, enum doca_rdma_transport_type *transport_type);

/**
 * @brief Get the MTU property from doca_rdma.
 * Returns the current MTU set for the doca_rdma context.
 * @note If MTU wasn't set by the user explicitly (and a default value was used), it may changed upon connection.
 *
 * @param [in] rdma
 * doca_rdma context to get the property from.
 * @param [out] mtu
 * MTU set in context.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_mtu(const struct doca_rdma *rdma, enum doca_mtu_size *mtu);

/**
 * @brief Get permissions property from doca_rdma.
 * Returns the current permissions set for the doca_rdma_context.
 * Can only be called after calling doca_ctx_dev_add().
 *
 * @param [in] rdma
 * doca_rdma context to get the property from.
 * @param [out] permissions
 * Bitwise combination of RDMA access flags set in context - see enum doca_access_flag
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_permissions(struct doca_rdma *rdma, uint32_t *permissions);

/**
 * @brief Get GRH setting from doca_rdma.
 * Get the current GRH setting for doca_rdma.
 *
 * @param [in] rdma
 * doca_rdma context to get the property from.
 * @param [out] grh_enabled
 * 1 if GRH setting was used in doca_rdma, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_grh_enabled(const struct doca_rdma *rdma, uint8_t *grh_enabled);

/**
 * @brief Get GID index from doca_rdma.
 * Get the current GID index set for doca_rdma.
 *
 * @param [in] rdma
 * doca_rdma context to get the property from.
 * @param [out] gid_index
 * GID index used in doca_rdma.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_gid_index(const struct doca_rdma *rdma, uint32_t *gid_index);

/**
 * @brief Get SL (service level) from doca_rdma.
 * Get the current SL set for doca_rdma.
 *
 * @param [in] rdma
 * doca_rdma context to get the property from.
 * @param [out] sl
 * SL used in doca_rdma.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_sl(const struct doca_rdma *rdma, uint32_t *sl);

/**
 * @brief Retrieve the handle in the dpa memory space of a doca_rdma
 *
 * @param [in] rdma
 * doca_rdma context to get the dpa handle from.
 * @param [out] dpa_rdma
 * A pointer to the handle in the dpa memory space.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if called before calling ctx_start(), or if not assigned to dpa datapath.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_dpa_handle(struct doca_rdma *rdma, doca_dpa_dev_rdma_t *dpa_rdma);

/**
 * @brief Retrieve the handle in the gpu memory space of a doca_rdma
 *
 * @param [in] rdma
 * doca_rdma context to get the gpu handle from.
 * @param [out] gpu_rdma
 * A pointer to the handle in the gpu memory space.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if called before calling ctx_start(), or if not assigned to gpu datapath.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_get_gpu_handle(struct doca_rdma *rdma, struct doca_gpu_dev_rdma **gpu_rdma);

/*********************************************************************************************************************
 * DOCA RDMA Tasks
 *********************************************************************************************************************/

/********************************************
 * DOCA RDMA Task - Receive		    *
 ********************************************/

/**
 * @brief This task receives a message \ immediate sent from the peer.
 */
struct doca_rdma_task_receive;

/** Task receive result opcodes */
enum doca_rdma_opcode {
	DOCA_RDMA_OPCODE_RECV_SEND = 0,
	DOCA_RDMA_OPCODE_RECV_SEND_WITH_IMM,
	DOCA_RDMA_OPCODE_RECV_WRITE_WITH_IMM,
};

/**
 * @brief Function to execute on completion of a receive task.
 *
 * @details This function is called by doca_pe_progress() when a receive task is successfully identified as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed receive task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_receive_completion_cb_t)(struct doca_rdma_task_receive *task,
						       union doca_data task_user_data,
						       union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a receive task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_receive_is_supported(const struct doca_devinfo *devinfo);

/**
 * Get the maximal buffer list length for a destination buffer of a receive task, for the given devinfo and transport
 * type.
 * @note The actual limit depends on the property set for the task - either the default value or the value set using
 * doca_rdma_task_receive_set_dst_buf_list_len() prior to doca_ctx_start().
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [in] transport_type
 * The relevant transport type.
 * @param [out] max_buf_list_len
 * The maximal number of local buffers that can be chained with a destination buffer of a receive task, for the given
 * devinfo and transport type.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_receive_get_max_dst_buf_list_len(const struct doca_devinfo *devinfo,
								 enum doca_rdma_transport_type transport_type,
								 uint32_t *max_buf_list_len);

/**
 * @brief This method sets the receive tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for receive tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for receive tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of receive tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_receive_set_conf(struct doca_rdma *rdma,
					     doca_rdma_task_receive_completion_cb_t successful_task_completion_cb,
					     doca_rdma_task_receive_completion_cb_t error_task_completion_cb,
					     uint32_t num_tasks);

/**
 * @brief Set the maximal destination buffer list length property for receive tasks.
 * The length may be increased and the value in use can be queried using doca_rdma_get_recv_buf_list_len().
 * @note Can only be called before calling doca_ctx_start().
 * @note Cannot exceed the value returned from doca_rdma_cap_task_receive_get_max_dst_buf_list_len().
 *
 * @param [in] rdma
 * The RDMA instance to set the property for.
 * @param [in] buf_list_len
 * buf_list_len to use in doca_rdma.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is already started or if the given RDMA was created with SRQ.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_receive_set_dst_buf_list_len(struct doca_rdma *rdma, uint32_t buf_list_len);

/**
 * @brief Get the maximal destination buffer list length property for receive tasks.
 * The returned value is the actual value being used and might differ from the size set by the user, as it may be
 * increased.
 *
 * @param [in] rdma
 * The RDMA instance to get the property from.
 * @param [out] buf_list_len
 * buf_list_len used in doca_rdma.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if the given RDMA was created with SRQ.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_receive_get_dst_buf_list_len(const struct doca_rdma *rdma, uint32_t *buf_list_len);

/**
 * @brief This method allocates and initializes a receive task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] dst_buf
 * Local destination buffer, for the received data.
 * May be NULL when receiving an empty message (without data), with or without immediate.
 * @note dst_buf may be linked to other buffers, with a limit according to
 * doca_rdma_cap_task_receive_get_max_dst_buf_list_len().
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a receive task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_receive_allocate_init(struct doca_rdma *rdma,
						  struct doca_buf *dst_buf,
						  union doca_data user_data,
						  struct doca_rdma_task_receive **task);

/**
 * @brief This method converts a receive task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The receive task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_receive_as_task(struct doca_rdma_task_receive *task);

/**
 * @brief This method sets the destination buffer of a receive task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] dst_buf
 * Local destination buffer, for the received data.
 * May be NULL when receiving an empty message (without data), with or without immediate.
 * If the destination buffer is not set by the user, it will have a default value - NULL.
 * @note dst_buf may be linked to other buffers, with a limit according to
 * doca_rdma_cap_task_receive_get_max_dst_buf_list_len().
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_receive_set_dst_buf(struct doca_rdma_task_receive *task, struct doca_buf *dst_buf);

/**
 * @brief This method gets the destination buffer of a receive task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's dst_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_receive_get_dst_buf(const struct doca_rdma_task_receive *task);

/**
 * @brief This method gets the opcode of the operation executed by the peer and received by the task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The operation executed by the peer and received.
 * @note Valid only on after completion of the task. Otherwise, undefined behavior.
 */
DOCA_EXPERIMENTAL
enum doca_rdma_opcode doca_rdma_task_receive_get_result_opcode(const struct doca_rdma_task_receive *task);

/**
 * @brief This method gets the length of data received by the task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * Total length of received data in case of completion.
 * @note Valid only on successful completion of the task. Otherwise, undefined behavior.
 */
DOCA_EXPERIMENTAL
uint32_t doca_rdma_task_receive_get_result_len(const struct doca_rdma_task_receive *task);

/**
 * @brief This method gets the immediate data received by the task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * A 32-bit immediate data value, in Big-Endian, received OOB from the peer along with the message.
 * @note Valid only on successful completion of the task and when the result opcode is
 * DOCA_RDMA_OPCODE_RECV_SEND_WITH_IMM or DOCA_RDMA_OPCODE_RECV_WRITE_WITH_IMM (retrieved using
 * doca_rdma_task_receive_get_result_opcode()).
 * Otherwise, undefined behavior.
 */
DOCA_EXPERIMENTAL
doca_be32_t doca_rdma_task_receive_get_result_immediate_data(const struct doca_rdma_task_receive *task);

/********************************************
 * DOCA RDMA Task - Send		    *
 ********************************************/

/**
 * @brief This task sends a message to the peer.
 */
struct doca_rdma_task_send;

/**
 * @brief Function to execute on completion of a send task.
 *
 * @details This function is called by doca_pe_progress() when a send task is successfully identified as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed send task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_send_completion_cb_t)(struct doca_rdma_task_send *task,
						    union doca_data task_user_data,
						    union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a send task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_send_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the send tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for send tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for send tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of send tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_send_set_conf(struct doca_rdma *rdma,
					  doca_rdma_task_send_completion_cb_t successful_task_completion_cb,
					  doca_rdma_task_send_completion_cb_t error_task_completion_cb,
					  uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a send task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] src_buf
 * Local source buffer, with the data to be sent.
 * May be NULL when wishing to send an empty message (without data).
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a send task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_send_allocate_init(struct doca_rdma *rdma,
					       const struct doca_buf *src_buf,
					       union doca_data user_data,
					       struct doca_rdma_task_send **task);

/**
 * @brief This method converts a send task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The send task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_send_as_task(struct doca_rdma_task_send *task);

/**
 * @brief This method sets the source buffer of a send task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] src_buf
 * Local source buffer, with the data to be sent.
 * May be NULL when wishing to send an empty message (without data).
 * If the source buffer is not set by the user, it will have a default value - NULL.
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_send_set_src_buf(struct doca_rdma_task_send *task, const struct doca_buf *src_buf);

/**
 * @brief This method gets the source buffer of a send task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's src_buf.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_rdma_task_send_get_src_buf(const struct doca_rdma_task_send *task);

/********************************************
 * DOCA RDMA Task - Send with Immediate	    *
 ********************************************/

/**
 * @brief This task sends a message to the peer with a 32-bit immediate value sent OOB.
 */
struct doca_rdma_task_send_imm;

/**
 * @brief Function to execute on completion of a send with immediate task.
 *
 * @details This function is called by doca_pe_progress() when a send with immediate task is successfully identified as
 * completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed send with immediate task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_send_imm_completion_cb_t)(struct doca_rdma_task_send_imm *task,
							union doca_data task_user_data,
							union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a send with immediate task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_send_imm_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the send with immediate tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for send with immediate tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for send with immediate tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of send with immediate tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_send_imm_set_conf(struct doca_rdma *rdma,
					      doca_rdma_task_send_imm_completion_cb_t successful_task_completion_cb,
					      doca_rdma_task_send_imm_completion_cb_t error_task_completion_cb,
					      uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a send with immediate task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] src_buf
 * Local source buffer, with the data to be sent.
 * May be NULL when wishing to send an empty message (without data).
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 * @param [in] immediate_data
 * A 32-bit value, in Big-Endian, to be sent OOB to the peer along with the message.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a send with immediate task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_send_imm_allocate_init(struct doca_rdma *rdma,
						   const struct doca_buf *src_buf,
						   doca_be32_t immediate_data,
						   union doca_data user_data,
						   struct doca_rdma_task_send_imm **task);

/**
 * @brief This method converts a send with immediate task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The send with immediate task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_send_imm_as_task(struct doca_rdma_task_send_imm *task);

/**
 * @brief This method sets the source buffer of a send with immediate task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] src_buf
 * Local source buffer, with the data to be sent.
 * May be NULL when wishing to send an empty message (without data).
 * If the source buffer is not set by the user, it will have a default value - NULL.
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_send_imm_set_src_buf(struct doca_rdma_task_send_imm *task, const struct doca_buf *src_buf);

/**
 * @brief This method gets the source buffer of a send with immediate task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's src_buf.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_rdma_task_send_imm_get_src_buf(const struct doca_rdma_task_send_imm *task);

/**
 * @brief This method sets the immediate data of a send with immediate task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] immediate_data
 * A 32-bit value, in Big-Endian, to be sent OOB to the peer along with the message.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_send_imm_set_immediate_data(struct doca_rdma_task_send_imm *task, doca_be32_t immediate_data);

/**
 * @brief This method gets the immediate data of a send with immediate task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's immediate_data.
 */
DOCA_EXPERIMENTAL
doca_be32_t doca_rdma_task_send_imm_get_immediate_data(const struct doca_rdma_task_send_imm *task);

/********************************************
 * DOCA RDMA Task - Read		    *
 ********************************************/

/**
 * @brief This task reads data from remote memory, the memory of the peer.
 */
struct doca_rdma_task_read;

/**
 * @brief Function to execute on completion of a read task.
 *
 * @details This function is called by doca_pe_progress() when a read task is successfully identified as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed read task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_read_completion_cb_t)(struct doca_rdma_task_read *task,
						    union doca_data task_user_data,
						    union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a read task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_read_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the read tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for read tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for read tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of read tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_read_set_conf(struct doca_rdma *rdma,
					  doca_rdma_task_read_completion_cb_t successful_task_completion_cb,
					  doca_rdma_task_read_completion_cb_t error_task_completion_cb,
					  uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a read task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] src_buf
 * Remote source buffer, holding the data that should be read.
 * May be NULL when wishing to read no data.
 * @note buffer lists are not supported for src_buf, only the head will be considered for this task.
 * @param [in] dst_buf
 * Local destination buffer, to which the read data will be written.
 * May be NULL when src_buf is NULL.
 * @note dst_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a read task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_read_allocate_init(struct doca_rdma *rdma,
					       const struct doca_buf *src_buf,
					       struct doca_buf *dst_buf,
					       union doca_data user_data,
					       struct doca_rdma_task_read **task);

/**
 * @brief This method converts a read task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The read task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_read_as_task(struct doca_rdma_task_read *task);

/**
 * @brief This method sets the source buffer of a read task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] src_buf
 * Local source buffer, with the data to be sent.
 * May be NULL when wishing to send an empty message (without data).
 * If the source buffer is not set by the user, it will have a default value - NULL.
 * @note buffer lists are not supported for src_buf, only the head will be considered for this task.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_read_set_src_buf(struct doca_rdma_task_read *task, const struct doca_buf *src_buf);

/**
 * @brief This method gets the source buffer of a read task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's src_buf.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_rdma_task_read_get_src_buf(const struct doca_rdma_task_read *task);

/**
 * @brief This method sets the destination buffer of a read task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] dst_buf
 * Local destination buffer, to which the read data will be written.
 * May be NULL when src_buf is NULL.
 * If the destination buffer is not set by the user, it will have a default value - NULL.
 * @note dst_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_read_set_dst_buf(struct doca_rdma_task_read *task, struct doca_buf *dst_buf);

/**
 * @brief This method gets the destination buffer of a read task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's dst_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_read_get_dst_buf(const struct doca_rdma_task_read *task);

/**
 * @brief This method gets the length of data read by the task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * Total length of read data in case of completion.
 * @note Valid only on successful completion of the task. Otherwise, undefined behavior.
 */
DOCA_EXPERIMENTAL
uint32_t doca_rdma_task_read_get_result_len(const struct doca_rdma_task_read *task);

/********************************************
 * DOCA RDMA Task - Write		    *
 ********************************************/

/**
 * @brief This task writes data to the remote memory, the memory of the peer.
 */
struct doca_rdma_task_write;

/**
 * @brief Function to execute on completion of a write task.
 *
 * @details This function is called by doca_pe_progress() when a write task is successfully identified as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed write task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_write_completion_cb_t)(struct doca_rdma_task_write *task,
						     union doca_data task_user_data,
						     union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a write task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_write_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the write tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for write tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for write tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of write tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_write_set_conf(struct doca_rdma *rdma,
					   doca_rdma_task_write_completion_cb_t successful_task_completion_cb,
					   doca_rdma_task_write_completion_cb_t error_task_completion_cb,
					   uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a write task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] src_buf
 * Local source buffer, holding the data that should be written to the remote memory.
 * May be NULL when wishing to write no data.
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 * @param [in] dst_buf
 * Remote destination buffer, to which the data will be written.
 * May be NULL when src_buf is NULL.
 * @note buffer lists are not supported for dst_buf, only the head will be considered for this task.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a write task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_write_allocate_init(struct doca_rdma *rdma,
						const struct doca_buf *src_buf,
						struct doca_buf *dst_buf,
						union doca_data user_data,
						struct doca_rdma_task_write **task);

/**
 * @brief This method converts a write task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The write task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_write_as_task(struct doca_rdma_task_write *task);

/**
 * @brief This method sets the source buffer of a write task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] src_buf
 * Local source buffer, holding the data that should be written to the remote memory.
 * May be NULL when wishing to write no data.
 * If the source buffer is not set by the user, it will have a default value - NULL.
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_write_set_src_buf(struct doca_rdma_task_write *task, const struct doca_buf *src_buf);

/**
 * @brief This method gets the source buffer of a write task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's src_buf.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_rdma_task_write_get_src_buf(const struct doca_rdma_task_write *task);

/**
 * @brief This method sets the destination buffer of a write task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] dst_buf
 * Remote destination buffer, to which the data will be written.
 * May be NULL when src_buf is NULL.
 * If the destination buffer is not set by the user, it will have a default value - NULL.
 * @note buffer lists are not supported for dst_buf, only the head will be considered for this task.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_write_set_dst_buf(struct doca_rdma_task_write *task, struct doca_buf *dst_buf);

/**
 * @brief This method gets the destination buffer of a write task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's dst_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_write_get_dst_buf(const struct doca_rdma_task_write *task);

/********************************************
 * DOCA RDMA Task - Write with Immediate    *
 ********************************************/

/**
 * @brief This task writes data to the remote memory, the memory of the peer, along with a 32-bit immediate value sent
 * to the peer OOB.
 */
struct doca_rdma_task_write_imm;

/**
 * @brief Function to execute on completion of a write with immediate task.
 *
 * @details This function is called by doca_pe_progress() when a write with immediate task is successfully identified as
 * completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed write with immediate task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_write_imm_completion_cb_t)(struct doca_rdma_task_write_imm *task,
							 union doca_data task_user_data,
							 union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a write with immediate task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_write_imm_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the write with immediate tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for write with immediate tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for write with immediate tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of write with immediate tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_write_imm_set_conf(struct doca_rdma *rdma,
					       doca_rdma_task_write_imm_completion_cb_t successful_task_completion_cb,
					       doca_rdma_task_write_imm_completion_cb_t error_task_completion_cb,
					       uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a write with immediate task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] src_buf
 * Local source buffer, holding the data that should be written to the remote memory.
 * May be NULL when wishing to write no data.
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 * @param [in] dst_buf
 * Remote destination buffer, to which the data will be written.
 * May be NULL when src_buf is NULL.
 * @note buffer lists are not supported for dst_buf, only the head will be considered for this task.
 * @param [in] immediate_data
 * A 32-bit value, in Big-Endian, to be sent OOB to the peer along with the write data.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a write with immediate task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_write_imm_allocate_init(struct doca_rdma *rdma,
						    const struct doca_buf *src_buf,
						    struct doca_buf *dst_buf,
						    doca_be32_t immediate_data,
						    union doca_data user_data,
						    struct doca_rdma_task_write_imm **task);

/**
 * @brief This method converts a write with immediate task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The write with immediate task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_write_imm_as_task(struct doca_rdma_task_write_imm *task);

/**
 * @brief This method sets the source buffer of a write with immediate task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] src_buf
 * Local source buffer, holding the data that should be written to the remote memory.
 * May be NULL when wishing to write no data.
 * If the source buffer is not set by the user, it will have a default value - NULL.
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_write_imm_set_src_buf(struct doca_rdma_task_write_imm *task, const struct doca_buf *src_buf);

/**
 * @brief This method gets the source buffer of a write with immediate task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's src_buf.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_rdma_task_write_imm_get_src_buf(const struct doca_rdma_task_write_imm *task);

/**
 * @brief This method sets the destination buffer of a write with immediate task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] dst_buf
 * Remote destination buffer, to which the data will be written.
 * May be NULL when src_buf is NULL.
 * If the destination buffer is not set by the user, it will have a default value - NULL.
 * @note buffer lists are not supported for dst_buf, only the head will be considered for this task.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_write_imm_set_dst_buf(struct doca_rdma_task_write_imm *task, struct doca_buf *dst_buf);

/**
 * @brief This method gets the destination buffer of a write with immediate task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's dst_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_write_imm_get_dst_buf(const struct doca_rdma_task_write_imm *task);

/**
 * @brief This method sets the immediate data of a write with immediate task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] immediate_data
 * A 32-bit value, in Big-Endian, to be sent OOB to the peer along with the write data.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_write_imm_set_immediate_data(struct doca_rdma_task_write_imm *task, doca_be32_t immediate_data);

/**
 * @brief This method gets the immediate data of a write with immediate task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's immediate_data.
 */
DOCA_EXPERIMENTAL
doca_be32_t doca_rdma_task_write_imm_get_immediate_data(const struct doca_rdma_task_write_imm *task);

/********************************************
 * DOCA RDMA Task - Atomic Compare and Swap *
 ********************************************/

/**
 * @brief This task compares an 8-byte value in the remote memory (the memory of the peer) to a given 8-byte value.
 * If these values are equal, the remote 8-byte value is swapped with another given 8-byte value, and otherwise it is
 * left without change.
 * The original remote 8-byte value (before the swap, if occurred) is written to a given local buffer.
 *
 * @note The process of reading the original remote 8-byte value, comparing it and swapping it, is atomic.
 */
struct doca_rdma_task_atomic_cmp_swp;

/**
 * @brief Function to execute on completion of an atomic compare and swap task.
 *
 * @details This function is called by doca_pe_progress() when an atomic compare and swap task is successfully
 * identified as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed atomic compare and swap task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_atomic_cmp_swp_completion_cb_t)(struct doca_rdma_task_atomic_cmp_swp *task,
							      union doca_data task_user_data,
							      union doca_data ctx_user_data);

/**
 * Check if a given device supports executing an atomic compare and swap task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_atomic_cmp_swp_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the atomic compare and swap tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for atomic compare and swap tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for atomic compare and swap tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of atomic compare and swap tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_atomic_cmp_swp_set_conf(
	struct doca_rdma *rdma,
	doca_rdma_task_atomic_cmp_swp_completion_cb_t successful_task_completion_cb,
	doca_rdma_task_atomic_cmp_swp_completion_cb_t error_task_completion_cb,
	uint32_t num_tasks);

/**
 * @brief This method allocates and initializes an atomic compare and swap task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] dst_buf
 * Remote destination buffer, on which the atomic 8-byte operation will be executed.
 * @note buffer lists are not supported for dst_buf, only the first 8-bytes of data in the head buffer will be
 * considered for this task.
 * @param [in] result_buf
 * Local buffer, to which the original remote 8-byte value (before the swap, if occurred) will be written.
 * @note buffer lists are not supported for result_buf, only the head will be considered for this task.
 * @param [in] cmp_data
 * An 8-byte value that will be compared to the remote 8-byte value.
 * @param [in] swap_data
 * An 8-byte value that will be written to dst_buf, overwriting it's previous data, in case cmp_data is equal to the
 * original remote 8-byte value.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized an atomic compare and swap task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_atomic_cmp_swp_allocate_init(struct doca_rdma *rdma,
							 struct doca_buf *dst_buf,
							 struct doca_buf *result_buf,
							 uint64_t cmp_data,
							 uint64_t swap_data,
							 union doca_data user_data,
							 struct doca_rdma_task_atomic_cmp_swp **task);

/**
 * @brief This method converts an atomic compare and swap task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The atomic compare and swap task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_atomic_cmp_swp_as_task(struct doca_rdma_task_atomic_cmp_swp *task);

/**
 * @brief This method sets the destination buffer of an atomic compare and swap task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] dst_buf
 * Remote destination buffer, on which the atomic 8-byte operation will be executed.
 * @note buffer lists are not supported for dst_buf, only the first 8-bytes of data in the head buffer will be
 * considered for this task.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_atomic_cmp_swp_set_dst_buf(struct doca_rdma_task_atomic_cmp_swp *task, struct doca_buf *dst_buf);

/**
 * @brief This method gets the destination buffer of an atomic compare and swap task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's dst_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_atomic_cmp_swp_get_dst_buf(const struct doca_rdma_task_atomic_cmp_swp *task);

/**
 * @brief This method sets the result buffer of an atomic compare and swap task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] result_buf
 * Local buffer, to which the original remote 8-byte value (before the swap, if occurred) will be written.
 * @note buffer lists are not supported for result_buf, only the head will be considered for this task.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_atomic_cmp_swp_set_result_buf(struct doca_rdma_task_atomic_cmp_swp *task,
						  struct doca_buf *result_buf);

/**
 * @brief This method gets the result buffer of an atomic compare and swap task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's result_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_atomic_cmp_swp_get_result_buf(const struct doca_rdma_task_atomic_cmp_swp *task);

/**
 * @brief This method sets the compare data of an atomic compare and swap task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] cmp_data
 * An 8-byte value that will be compared to the remote 8-byte value.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_atomic_cmp_swp_set_cmp_data(struct doca_rdma_task_atomic_cmp_swp *task, uint64_t cmp_data);

/**
 * @brief This method gets the compare data of an atomic compare and swap task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's cmp_data.
 */
DOCA_EXPERIMENTAL
uint64_t doca_rdma_task_atomic_cmp_swp_get_cmp_data(const struct doca_rdma_task_atomic_cmp_swp *task);

/**
 * @brief This method sets the swap data of an atomic compare and swap task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] swap_data
 * An 8-byte value that will be written to dst_buf, overwriting it's previous data, in case cmp_data is equal to the
 * original remote 8-byte value.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_atomic_cmp_swp_set_swap_data(struct doca_rdma_task_atomic_cmp_swp *task, uint64_t swap_data);

/**
 * @brief This method gets the swap data of an atomic compare and swap task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's swap_data.
 */
DOCA_EXPERIMENTAL
uint64_t doca_rdma_task_atomic_cmp_swp_get_swap_data(const struct doca_rdma_task_atomic_cmp_swp *task);

/********************************************
 * DOCA RDMA Task - Atomic Fetch and Add    *
 ********************************************/

/**
 * @brief This task adds a given 8-byte value to an 8-byte value in the remote memory, the memory of the peer.
 * The original remote 8-byte value (before the addition) is written to a given local buffer.
 *
 * @note The process of reading the original remote 8-byte value and adding to it, is atomic.
 */
struct doca_rdma_task_atomic_fetch_add;

/**
 * @brief Function to execute on completion of an atomic fetch and add task.
 *
 * @details This function is called by doca_pe_progress() when an atomic fetch and add task is successfully identified
 * as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed atomic fetch and add task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_atomic_fetch_add_completion_cb_t)(struct doca_rdma_task_atomic_fetch_add *task,
								union doca_data task_user_data,
								union doca_data ctx_user_data);

/**
 * Check if a given device supports executing an atomic fetch and add task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_atomic_fetch_add_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the atomic fetch and add tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for atomic fetch and add tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for atomic fetch and add tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of atomic fetch and add tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_atomic_fetch_add_set_conf(
	struct doca_rdma *rdma,
	doca_rdma_task_atomic_fetch_add_completion_cb_t successful_task_completion_cb,
	doca_rdma_task_atomic_fetch_add_completion_cb_t error_task_completion_cb,
	uint32_t num_tasks);

/**
 * @brief This method allocates and initializes an atomic fetch and add task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] dst_buf
 * Remote destination buffer, on which the atomic 8-byte operation will be executed.
 * @note buffer lists are not supported for dst_buf, only the first 8-bytes of data in the head buffer will be
 * considered for this task.
 * @param [in] result_buf
 * Local buffer, to which the original remote 8-byte value (before the addition) will be written.
 * @note buffer lists are not supported for result_buf, only the head will be considered for this task.
 * @param [in] add_data
 * An 8-byte value that will be added to the remote 8-byte value in dst_buf.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized an atomic fetch and add task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_atomic_fetch_add_allocate_init(struct doca_rdma *rdma,
							   struct doca_buf *dst_buf,
							   struct doca_buf *result_buf,
							   uint64_t add_data,
							   union doca_data user_data,
							   struct doca_rdma_task_atomic_fetch_add **task);

/**
 * @brief This method converts an atomic fetch and add task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The atomic fetch and add task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_atomic_fetch_add_as_task(struct doca_rdma_task_atomic_fetch_add *task);

/**
 * @brief This method sets the destination buffer of an atomic fetch and add task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] dst_buf
 * Remote destination buffer, on which the atomic 8-byte operation will be executed.
 * @note buffer lists are not supported for dst_buf, only the first 8-bytes of data in the head buffer will be
 * considered for this task.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_atomic_fetch_add_set_dst_buf(struct doca_rdma_task_atomic_fetch_add *task,
						 struct doca_buf *dst_buf);

/**
 * @brief This method gets the destination buffer of an atomic fetch and add task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's dst_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_atomic_fetch_add_get_dst_buf(const struct doca_rdma_task_atomic_fetch_add *task);

/**
 * @brief This method sets the result buffer of an atomic fetch and add task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] result_buf
 * Local buffer, to which the original remote 8-byte value (before the addition) will be written.
 * @note buffer lists are not supported for result_buf, only the head will be considered for this task.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_atomic_fetch_add_set_result_buf(struct doca_rdma_task_atomic_fetch_add *task,
						    struct doca_buf *result_buf);

/**
 * @brief This method gets the result buffer of an atomic fetch and add task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's result_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_atomic_fetch_add_get_result_buf(const struct doca_rdma_task_atomic_fetch_add *task);

/**
 * @brief This method sets the add data of an atomic fetch and add task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] add_data
 * An 8-byte value that will be added to the remote 8-byte value in dst_buf.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_atomic_fetch_add_set_add_data(struct doca_rdma_task_atomic_fetch_add *task, uint64_t add_data);

/**
 * @brief This method gets the add data of an atomic fetch and add task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's add_data.
 */
DOCA_EXPERIMENTAL
uint64_t doca_rdma_task_atomic_fetch_add_get_add_data(const struct doca_rdma_task_atomic_fetch_add *task);

/**********************************************
 * DOCA RDMA Task - Remote Net Sync Event Get *
 **********************************************/

/**
 * @brief This task reads the value of a remote net sync event.
 */
struct doca_rdma_task_remote_net_sync_event_get;

/**
 * @brief Function to execute on completion of a remote_net_sync_event_get task.
 *
 * @details This function is called by doca_pe_progress() when a remote_net_sync_event_get task
 * is successfully identified as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed remote_net_sync_event_get task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_remote_net_sync_event_get_completion_cb_t)(
	struct doca_rdma_task_remote_net_sync_event_get *task,
	union doca_data task_user_data,
	union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a remote_net_sync_event_get task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_remote_net_sync_event_get_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the remote_net_sync_event_get tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for remote_net_sync_event_get tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for remote_net_sync_event_get tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of remote_net_sync_event_get tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_remote_net_sync_event_get_set_conf(
	struct doca_rdma *rdma,
	doca_rdma_task_remote_net_sync_event_get_completion_cb_t successful_task_completion_cb,
	doca_rdma_task_remote_net_sync_event_get_completion_cb_t error_task_completion_cb,
	uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a remote_net_sync_event_get task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] event
 * Remote net sync event to read its value.
 * @param [in] dst_buf
 * Local destination buffer, to which the read data will be written.
 * @note dst_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a remote_net_sync_event_get task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_remote_net_sync_event_get_allocate_init(
	struct doca_rdma *rdma,
	const struct doca_sync_event_remote_net *event,
	struct doca_buf *dst_buf,
	union doca_data user_data,
	struct doca_rdma_task_remote_net_sync_event_get **task);

/**
 * @brief This method converts a remote_net_sync_event_get task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The remote_net_sync_event_get task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_remote_net_sync_event_get_as_task(
	struct doca_rdma_task_remote_net_sync_event_get *task);

/**
 * @brief This method sets the remote net sync event of a remote_net_sync_event_get task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] event
 * Remote net sync event to read its value.
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_remote_net_sync_event_get_set_sync_event(struct doca_rdma_task_remote_net_sync_event_get *task,
							     const struct doca_sync_event_remote_net *event);

/**
 * @brief This method gets the remote net sync event of a remote_net_sync_event_get task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's sync_event.
 */
DOCA_EXPERIMENTAL
const struct doca_sync_event_remote_net *doca_rdma_task_remote_net_sync_event_get_get_sync_event(
	const struct doca_rdma_task_remote_net_sync_event_get *task);

/**
 * @brief This method sets the destination buffer of a remote_net_sync_event_get task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] dst_buf
 * Local destination buffer, to which the remote_net_sync_event_get data will be written.
 * @note dst_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_remote_net_sync_event_get_set_dst_buf(struct doca_rdma_task_remote_net_sync_event_get *task,
							  struct doca_buf *dst_buf);

/**
 * @brief This method gets the destination buffer of a remote_net_sync_event_get task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's dst_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_remote_net_sync_event_get_get_dst_buf(
	const struct doca_rdma_task_remote_net_sync_event_get *task);

/**
 * @brief This method gets the length of data read by the task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * Total length of read data in case of completion.
 * @note Valid only on successful completion of the task. Otherwise, undefined behavior.
 */
DOCA_EXPERIMENTAL
uint32_t doca_rdma_task_remote_net_sync_event_get_get_result_len(
	const struct doca_rdma_task_remote_net_sync_event_get *task);

/********************************************************************
 * DOCA RDMA Task - Remote Net Sync Event Notify Set		    *
 ********************************************************************/

/**
 * @brief This task sets the value of a remote net sync event to a given value.
 */
struct doca_rdma_task_remote_net_sync_event_notify_set;

/**
 * @brief Function to execute on completion of a remote_net_sync_event_notify_set task.
 *
 * @details This function is called by doca_pe_progress() when a remote_net_sync_event_notify_set task
 * is successfully identified as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed remote_net_sync_event_notify_set task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_remote_net_sync_event_notify_set_completion_cb_t)(
	struct doca_rdma_task_remote_net_sync_event_notify_set *task,
	union doca_data task_user_data,
	union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a remote_net_sync_event_notify_set task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_remote_net_sync_event_notify_set_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the remote_net_sync_event_notify_set tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for remote_net_sync_event_notify_set tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for remote_net_sync_event_notify_set tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of remote_net_sync_event_notify_set tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_remote_net_sync_event_notify_set_set_conf(
	struct doca_rdma *rdma,
	doca_rdma_task_remote_net_sync_event_notify_set_completion_cb_t successful_task_completion_cb,
	doca_rdma_task_remote_net_sync_event_notify_set_completion_cb_t error_task_completion_cb,
	uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a remote_net_sync_event_notify_set task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] event
 * The remote sync event to set.
 * @param [in] src_buf
 * Local source buffer, holding the value to set the remote net sync event to.
 * @note src_buf may be linked to other buffers, with a limit according to the max_send_buf_list_len property that can
 * be set or queried using doca_rdma_set_max_send_buf_list_len() \ doca_rdma_get_max_send_buf_list_len() respectfully.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a remote_net_sync_event_notify_set task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_remote_net_sync_event_notify_set_allocate_init(
	struct doca_rdma *rdma,
	struct doca_sync_event_remote_net *event,
	const struct doca_buf *src_buf,
	union doca_data user_data,
	struct doca_rdma_task_remote_net_sync_event_notify_set **task);

/**
 * @brief This method converts a remote_net_sync_event_notify_set task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The remote_net_sync_event_notify_set task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_remote_net_sync_event_notify_set_as_task(
	struct doca_rdma_task_remote_net_sync_event_notify_set *task);

/**
 * @brief This method sets the remote net sync event of a remote_net_sync_event_notify_set task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] event
 * The remote net sync event to set.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_remote_net_sync_event_notify_set_set_sync_event(
	struct doca_rdma_task_remote_net_sync_event_notify_set *task,
	struct doca_sync_event_remote_net *event);

/**
 * @brief This method gets the remote net sync event of a remote_net_sync_event_notify_set task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's remote net sync event.
 */
DOCA_EXPERIMENTAL
struct doca_sync_event_remote_net *doca_rdma_task_remote_net_sync_event_notify_set_get_sync_event(
	const struct doca_rdma_task_remote_net_sync_event_notify_set *task);

/**
 * @brief This method sets the source buffer of a remote_net_sync_event_notify_set task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] src_buf
 * Local source buffer, holding the value to set the remote net sync event to.
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_remote_net_sync_event_notify_set_set_src_buf(
	struct doca_rdma_task_remote_net_sync_event_notify_set *task,
	const struct doca_buf *src_buf);

/**
 * @brief This method gets the source buffer of a remote_net_sync_event_notify_set task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's src_buf.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_rdma_task_remote_net_sync_event_notify_set_get_src_buf(
	const struct doca_rdma_task_remote_net_sync_event_notify_set *task);

/********************************************************
 * DOCA RDMA Task - Remote Net Sync Event Notify Add    *
 ********************************************************/

/**
 * @brief This task adds a given value to the value of a remote net sync event atomically.
 * The original remote 8-byte value (before the addition) is written to a given local buffer.
 */
struct doca_rdma_task_remote_net_sync_event_notify_add;

/**
 * @brief Function to execute on completion of a remote_net_sync_event_notify_add task.
 *
 * @details This function is called by doca_pe_progress() when a remote_net_sync_event_notify_add task is
 * successfully identified as completed.
 * When this function is called the ownership of the task object passes from DOCA back to user.
 * Inside this callback the user may decide on the task object:
 * - re-submit task with doca_task_submit(); task object ownership passed to DOCA
 * - release task with doca_task_free(); task object ownership passed to DOCA
 * - keep the task object for future re-use; user keeps the ownership on the task object
 * Inside this callback the user shouldn't call doca_pe_progress().
 * Please see doca_pe_progress() for details.
 *
 * Any failure/error inside this function should be handled internally or deferred;
 * Since this function is nested in the execution of doca_pe_progress(), this callback doesn't return an error.
 *
 * @note This callback type is utilized for both successful & failed task completions.
 *
 * @param [in] task
 * The completed remote_net_sync_event_notify_add task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_rdma_task_remote_net_sync_event_notify_add_completion_cb_t)(
	struct doca_rdma_task_remote_net_sync_event_notify_add *task,
	union doca_data task_user_data,
	union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a remote_net_sync_event_notify_add task.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support the task.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_cap_task_remote_net_sync_event_notify_add_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the remote_net_sync_event_notify_add tasks configuration.
 *
 * @param [in] rdma
 * The RDMA instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for remote_net_sync_event_notify_add tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for remote_net_sync_event_notify_add tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of remote_net_sync_event_notify_add tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_remote_net_sync_event_notify_add_set_conf(
	struct doca_rdma *rdma,
	doca_rdma_task_remote_net_sync_event_notify_add_completion_cb_t successful_task_completion_cb,
	doca_rdma_task_remote_net_sync_event_notify_add_completion_cb_t error_task_completion_cb,
	uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a remote_net_sync_event_notify_add task.
 *
 * @param [in] rdma
 * The RDMA instance to allocate the task for.
 * @param [in] event
 * Remote sync event to atomically increment by a given value.
 * @param [in] result_buf
 * Local buffer, to which the original remote sync event value (before the addition) will be written.
 * @note buffer lists are not supported for result_buf, only the head will be considered for this task.
 * @param [in] add_data
 * An 8-byte value that will be added to the remote sync event value.
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized a remote_net_sync_event_notify_add task.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_task_remote_net_sync_event_notify_add_allocate_init(
	struct doca_rdma *rdma,
	struct doca_sync_event_remote_net *event,
	struct doca_buf *result_buf,
	uint64_t add_data,
	union doca_data user_data,
	struct doca_rdma_task_remote_net_sync_event_notify_add **task);

/**
 * @brief This method converts a remote_net_sync_event_notify_add task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * The remote_net_sync_event_notify_add task converted to doca_task.
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_rdma_task_remote_net_sync_event_notify_add_as_task(
	struct doca_rdma_task_remote_net_sync_event_notify_add *task);

/**
 * @brief This method sets the remote sync event of a remote_net_sync_event_notify_add task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] event
 * The remote sync event to increment atomically.
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_remote_net_sync_event_notify_add_set_sync_event(
	struct doca_rdma_task_remote_net_sync_event_notify_add *task,
	struct doca_sync_event_remote_net *event);

/**
 * @brief This method gets the remote sync event of a remote_net_sync_event_notify_add task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's sync event.
 */
DOCA_EXPERIMENTAL
struct doca_sync_event_remote_net *doca_rdma_task_remote_net_sync_event_notify_add_get_sync_event(
	const struct doca_rdma_task_remote_net_sync_event_notify_add *task);

/**
 * @brief This method sets the result buffer of a remote_net_sync_event_notify_add task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] result_buf
 * Local buffer, to which the original remote sync event value (before the addition) will be written.
 * @note buffer lists are not supported for result_buf, only the head will be considered for this task.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_remote_net_sync_event_notify_add_set_result_buf(
	struct doca_rdma_task_remote_net_sync_event_notify_add *task,
	struct doca_buf *result_buf);

/**
 * @brief This method gets the result buffer of a remote_net_sync_event_notify_add task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's result_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_rdma_task_remote_net_sync_event_notify_add_get_result_buf(
	const struct doca_rdma_task_remote_net_sync_event_notify_add *task);

/**
 * @brief This method sets the add data of a remote_net_sync_event_notify_add task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] add_data
 * An 8-byte value that will be atomically added to the remote sync event.
 *
 */
DOCA_EXPERIMENTAL
void doca_rdma_task_remote_net_sync_event_notify_add_set_add_data(
	struct doca_rdma_task_remote_net_sync_event_notify_add *task,
	uint64_t add_data);

/**
 * @brief This method gets the add data of a remote_net_sync_event_notify_add task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's add_data.
 */
DOCA_EXPERIMENTAL
uint64_t doca_rdma_task_remote_net_sync_event_notify_add_get_add_data(
	const struct doca_rdma_task_remote_net_sync_event_notify_add *task);

/*********************************************************************************************************************
 * DOCA RDMA SRQ Context
 *********************************************************************************************************************/

/**
 * @brief Create a DOCA RDMA Shared Receive Queue instance.
 *
 * @note The RDMA SRQ datapath is supported only on DPA. Use doca_ctx_set_datapath_on_dpa() before starting the context.
 *
 * @param [in] dev
 * The device to attach to the RDMA SRQ instance.
 * @param [out] rdma_srq
 * Pointer to pointer to be set to point to the created RDMA SRQ instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - rdma_srq argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate resources.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_create(struct doca_dev *dev, struct doca_rdma_srq **rdma_srq);

/**
 * @brief Destroy a DOCA RDMA Shared Receive Queue instance.
 *
 * @param [in] rdma_srq
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - rdma_srq argument is a NULL pointer.
 * - DOCA_ERROR_BAD_STATE - the associated ctx was not stopped before calling doca_rdma_srq_destroy().
 *
 * @note it is the user responsibility to destroy the rdma_srq object only after stopping all the rdma instances
 * that use this rdma_srq object.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_destroy(struct doca_rdma_srq *rdma_srq);

/**
 * @brief Convert DOCA RDMA Shared Receive Queue instance into a generalised context for use with doca core objects.
 *
 * @param [in] rdma_srq_ctx
 * RDMA SRQ instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_rdma_srq_as_ctx(struct doca_rdma_srq *rdma_srq_ctx);

/*********************************************************************************************************************
 * DOCA RDMA SRQ capabilities
 *********************************************************************************************************************/

/**
 * @brief Get the maximal shared receive queue size for a specific device.
 *
 * @param [in] devinfo
 * The DOCA device information
 * @param [out] max_shared_recv_queue_size
 * The maximal shared receive queue size for the given devinfo.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_cap_get_max_shared_recv_queue_size(const struct doca_devinfo *devinfo,
							      uint32_t *max_shared_recv_queue_size);

/**
 * @brief Get the maximal buffer list length for a destination buffer of a receive task, for the given devinfo
 * @note The actual limit depends on the property set for the task - either the default value or the value set using
 * doca_rdma_srq_task_receive_set_dst_buf_list_len() prior to doca_ctx_start().
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_buf_list_len
 * The maximal number of local buffers that can be chained with a destination buffer of a receive task, for the given
 * devinfo.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_cap_task_receive_get_max_dst_buf_list_len(const struct doca_devinfo *devinfo,
								     uint32_t *max_buf_list_len);

/*********************************************************************************************************************
 * DOCA RDMA SRQ properties
 *********************************************************************************************************************/

/**
 * @brief Set Shared Receive Queue size property for doca_rdma_srq.
 * The value can be queried using doca_rdma_srq_get_shared_recv_queue_size().
 * Queue size will be rounded to the next power of 2.
 * Can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma_srq
 * The RDMA SRQ instance to set the property for.
 * @param [in] shared_recv_queue_size
 * Shared receive queue size to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_NOT_SUPPORTED - if the given size is not supported.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_set_shared_recv_queue_size(struct doca_rdma_srq *rdma_srq, uint32_t shared_recv_queue_size);

/**
 * @brief Set the maximum buffer list length property for a destination buffer of a receive task.
 * The length may be increased and the value in use can be queried using
 * doca_rdma_srq_task_receive_get_dst_buf_list_len().
 * @note Can only be called before calling doca_ctx_start().
 * @note Cannot exceed the value returned from doca_rdma_srq_cap_task_receive_get_max_dst_buf_list_len().
 *
 * @param [in] rdma_srq
 * The RDMA SRQ instance to set the property for.
 * @param [in] buf_list_len
 * Maximum buffer list length to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_task_receive_set_dst_buf_list_len(struct doca_rdma_srq *rdma_srq, uint32_t buf_list_len);

/**
 * @brief Set Shared Receive Queue type property for doca_rdma_srq.
 * The value can be queried using doca_rdma_srq_get_type().
 * @note Can only be called before calling doca_ctx_start().
 *
 * @param [in] rdma_srq
 * The RDMA SRQ instance to set the property for.
 * @param [in] srq_type
 * Shared receive queue type to use in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_set_type(struct doca_rdma_srq *rdma_srq, enum doca_rdma_srq_type srq_type);

/**
 * @brief Get Shared Receive Queue type property from doca_rdma_srq.
 * Returns the current srq_type set for the doca_rdma_srq_context.
 *
 * @param [in] rdma_srq
 * The RDMA SRQ instance to set the property for.
 * @param [out] srq_type
 * Shared receive queue type set in context.
 *
 * @return
 * DOCA_SUCCESS - if property set successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_get_type(struct doca_rdma_srq *rdma_srq, enum doca_rdma_srq_type *srq_type);

/**
 * @brief Get the maximum buffer list length property from doca_rdma_srq.
 * Returns the current buf_list_len set for the doca_rdma_srq context.
 * The size returned is the actual size being used and might differ from the size set by the user,
 * as the size may be increased.
 *
 * @param [in] rdma_srq
 * The RDMA SRQ instance to get the property from.
 * @param [out] buf_list_len
 * Maximum buffer list length set in context.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_task_receive_get_dst_buf_list_len(const struct doca_rdma_srq *rdma_srq,
							     uint32_t *buf_list_len);

/**
 * @brief Get Shared Receive Queue size property from doca_rdma_srq.
 * Returns the current shared_recv_queue_size set for the doca_rdma_srq_context.
 * The size returned is the actual size being used and might differ from the size set by the user,
 * as the size may be increased.
 *
 * @param [in] rdma_srq
 * The RDMA SRQ instance to get the property from.
 * @param [out] shared_recv_queue_size
 * Shared receive queue size set in context.
 *
 * @return
 * DOCA_SUCCESS - if property retrieved successfully.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_get_shared_recv_queue_size(const struct doca_rdma_srq *rdma_srq,
						      uint32_t *shared_recv_queue_size);

/**
 * @brief Retrieve the handle in the dpa memory space of a DOCA RDMA Shared Receive Queue instance
 *
 * @param [in] rdma_srq
 * The RDMA SRQ instance to get the dpa handle from.
 * @param [out] dpa_rdma_srq
 * A pointer to the handle in the dpa memory space.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid parameter was given.
 * - DOCA_ERROR_BAD_STATE - if called before calling ctx_start(), or if not assigned to dpa datapath.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_srq_get_dpa_handle(const struct doca_rdma_srq *rdma_srq, doca_dpa_dev_rdma_srq_t *dpa_rdma_srq);

/*********************************************************************************************************************
 * DOCA RDMA DPA Completion context
 *********************************************************************************************************************/

/**
 * Opaque structure representing a DOCA DPA Completion instance.
 */
struct doca_dpa_completion;

/**
 * @brief Attach DOCA RDMA to DPA completion context
 *
 * This function must be called before DOCA RDMA context is started
 *
 * @note This API is relevant only for contexts that are set on DPA datapath,
 * using doca_ctx_set_datapath_on_dpa() before calling doca_ctx_start().
 *
 * @param [in] rdma
 * doca_rdma context
 * @param[in] dpa_comp
 * DPA completion context
 *
 * @return
 * DOCA_SUCCESS - in case of success
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - if context is already started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_rdma_dpa_completion_attach(struct doca_rdma *rdma, struct doca_dpa_completion *dpa_comp);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_RDMA_H_ */

/** @} */
