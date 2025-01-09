/*
 * Copyright (c) 2021-2022 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_telemetry_netflow.h
 * @page telemetry_netflow
 * @defgroup TELEMETRY NetFlow
 *
 * DOCA lib for exporting a netflow packet to a netflow collector through the
 * telemetry service.
 *
 * This lib simplifies and centralizes the formatting and exporting of netflow packets.
 * Netflow is a protocol for exporting information about the device network flows
 * to a netflow collector that will aggregate and analyze the data.
 * After creating conf file and invoke init function, the lib send function
 * can be called with netflow struct to send a netflow packet with the format
 * to the collector of choice specified in the conf file.
 * The lib uses the netflow protocol specified by cisco.
 * @see https://netflow.caligare.com/netflow_v9.htm
 *
 * Limitations:
 *
 * The lib supports the netflow V9 format. The lib is not thread safe.
 *
 * @{
 */

#ifndef DOCA_TELEMETRY_NETFLOW_H_
#define DOCA_TELEMETRY_NETFLOW_H_

#include <stddef.h>
#include <stdint.h>

#include <doca_telemetry_netflow_types.h>
#include <doca_telemetry.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief NetFlow Application ID
 *
 * @note This GUID cannot change
 */
/* 9910c128-3961-47e6-be6c-715a0f03add6 */
#define DOCA_NETFLOW_APP_ID \
	{ \
		0x99, 0x10, 0xc1, 0x28, 0x39, 0x61, 0x47, 0xe6, 0xbe, 0x6c, 0x71, 0x5a, 0x0f, 0x03, 0xad, 0xd6 \
	}

/** @brief NetFlow collector default port */
#define DOCA_NETFLOW_DEFAULT_PORT 2055

/**
 * @brief One field in netflow template, please refer to doca_telemetry_netflow_types.h for type macros
 */
struct doca_telemetry_netflow_flowset_field;

/**
 * @brief Template for the records.
 * struct record_example {
 *	uint32_t src_addr_V4;
 *	uint32_t dst_addr_V4;
 * }
 * doca_telemetry_netflow_template_create(&temp)
 *
 * doca_telemetry_netflow_field_create(&field1);
 * doca_telemetry_netflow_field_create(&field2);
 *
 * doca_telemetry_netflow_field_set_type(field1, DOCA_NETFLOW_IPV4_SRC_ADDR);
 * doca_telemetry_netflow_field_set_len(field1. DOCA_NETFLOW_IPV4_SRC_ADDR_DEFAULT_LENGTH);
 *
 * doca_telemetry_netflow_field_set_type(field1, DOCA_NETFLOW_IPV4_DST_ADDR);
 * doca_telemetry_netflow_field_set_len(field1. DOCA_NETFLOW_IPV4_DST_ADDR_DEFAULT_LENGTH);
 *
 * doca_telemetry_netflow_template_add_field(temp, field1);
 * doca_telemetry_netflow_template_add_field(temp, field2);

 * @note all fields are in network byte order.
 */
struct doca_telemetry_netflow_template;

/**
 * @brief Init exporter memory, set configs and open connection.
 *
 * The Source ID field is a 32-bit value that is used to guarantee uniqueness for
 * all flows exported from a particular device (see link).
 *
 * This function can be called again only after doca_telemetry_netflow_destroy was
 * called.
 *
 * @param [in] source_id
 * Unique source ID.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_BAD_STATE - if the netflow has been initialized before this call.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate memory.
 * - DOCA_ERROR_INITIALIZATION - failed to initialise netflow.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_init(uint16_t source_id);

/**
 * @brief Set collector address.
 *
 * @param [in] collector_addr
 * User defined netflow collector's IP address.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_collector_addr(const char *collector_addr);

/**
 * @brief Set collector port.
 * See DOCA_NETFLOW_DEFAULT_PORT for default value.
 *
 * @param [in] collector_port
 * User defined netflow collector's port.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_collector_port(uint16_t collector_port);

/**
 * @brief Set max packet size.
 *
 * @param [in] max_packet_size
 * User defined netflow packet's max size.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_max_packet_size(uint16_t max_packet_size);

/* ================================ NETFLOW FIELD ================================ */

/**
 * @brief Create new telemetry netflow field.
 * @param [out] field
 * Pointer to the newly allocated telemetry field.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate doca telemetry netflow field.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_field_create(struct doca_telemetry_netflow_flowset_field **field);

/**
 * @brief Set doca telemetry netflow field type.
 * @param [in] field
 * Pointer to doca telemetry netflow field.
 * @param [in] type
 * Field type.
 *
 * @note Passing a field value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_field_set_type(struct doca_telemetry_netflow_flowset_field *field, uint16_t type);

/**
 * @brief Set doca telemetry netflow field length.
 *
 * @param [in] field
 * Pointer to doca telemetry netflow field.
 * @param [in] length
 * Field type.
 *
 * @note Passing a field value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_field_set_len(struct doca_telemetry_netflow_flowset_field *field, uint16_t length);

/**
 * @brief Destructor for DOCA netflow field.
 *
 * @param [in] field
 * field to destroy.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if netflow_template is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_field_destroy(struct doca_telemetry_netflow_flowset_field *field);

/* ================================ NETFLOW TEMPLATE ================================ */

/**
 * @brief Set buffer size
 * Default value is 60000 bytes.
 *
 * @param [in] size
 * Buffer size
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_buf_size(uint64_t size);

/**
 * @brief Get buffer size
 *
 * @param [out] size
 * The buffer size
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_get_buf_size(uint64_t *size);

/**
 * @brief Set buffer data root
 * Default path is "/opt/mellanox/doca/services/telemetry/data/"
 *
 * @param [in] path
 * Path to a folder where the data and schema will be stored.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_buf_data_root(const char *path);

/**
 * @brief Get data root path
 *
 * @param [out] path
 * The buffer data root
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate memory.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_get_buf_data_root(const char **path);

/**
 * @brief Create new telemetry netflow template.
 * @param [out] netflow_template
 * Pointer to the newly allocated telemetry netflow template.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate doca telemetry netflow template.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_template_create(struct doca_telemetry_netflow_template **netflow_template);

/**
 * @brief Add DOCA telemetry netflow field to netflow_template.
 * The user loses the ownership of the field after a successful invocation of the function.
 * @note field should NOT be passed to another group after calling this function.
 * @param [in] netflow_template
 * Pointer to netflow_template.
 * @param [in] field
 * DOCA Telemetry netflow field to add.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate doca telemetry netflow field.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_template_add_field(struct doca_telemetry_netflow_template *netflow_template,
						       struct doca_telemetry_netflow_flowset_field *field);

/**
 * @brief Destructor for DOCA netflow template.
 *
 * @param [in] netflow_template
 * netflow template to destroy.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if netflow_template is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_template_destroy(struct doca_telemetry_netflow_template *netflow_template);

/**
 * @brief Enable file write
 * file write is disabled by default.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_file_write_enabled(void);

/**
 * @brief Set file maximum size
 * Default value is 1MB.
 *
 * @param [in] size
 * Maximum size of binary data file. Once this size is reached, a new binary file will be created.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_file_write_max_size(size_t size);

/**
 * @brief Get file maximum size
 *
 * @param [out] size
 * Maximum size of binary data file.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_get_file_write_max_size(size_t *size);

/**
 * @brief Set file maximum age
 * Default value is 1 hour.
 *
 * @param [in] max_age
 * Maximum file age. Once current file is older than this threshold a new file will be created.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_file_write_max_age(doca_telemetry_timestamp_t max_age);

/**
 * @brief Get file maximum age
 *
 * @param [out] max_age
 * Maximum file age. Once current file is older than this threshold a new file will be created.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_get_file_write_max_age(doca_telemetry_timestamp_t *max_age);

/**
 * @brief Enable IPC
 * IPC is disabled by default.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_ipc_enabled(void);

/**
 * @brief Set IPC socket directory.
 * Default path is "/opt/mellanox/doca/services/telemetry/ipc_sockets"
 *
 * @param [in] path
 * Path to a folder containing DOCA Telemetry Service (DTS) sockets.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_set_ipc_sockets_dir(const char *path);

/**
 * @brief Get IPC socket directory.
 *
 * @param [out] path
 * Path to a folder containing DOCA Telemetry Service (DTS) sockets.
 *
 * @note Ownership of the returned string is transferred to the caller.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate memory.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_get_ipc_sockets_dir(const char **path);

/**
 * @brief Set source id
 *
 * @param [in] source_id
 * Hostname or guid.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_source_set_id(const char *source_id);

/**
 * @brief Set source tag
 *
 * @param [in] source_tag
 * User defined data-file name prefix.
 *
 * @note This function should be called after doca_telemetry_netflow_init().
 */
DOCA_EXPERIMENTAL
void doca_telemetry_netflow_source_set_tag(const char *source_tag);

/**
 * @brief Finalizes netflow setup.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_BAD_STATE - if the netflow has not been initialized or the netflow has started.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate memory.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_start(void);

/**
 * @brief Sending netflow records. Need to init first.
 *
 * @param [in] netflow_template
 * Template pointer of how the records are structured.
 * For more info refer to doca_telemetry_netflow_template.
 * @param [in] records
 * Array of pointers to the flows structs to send, must be packed.
 * Strings must be an array in the struct, not a pointer.
 * @param [in] nof_records
 * Records array size.
 * @param [out] nof_records_sent
 * If not NULL, it will be filled with amount of records sent.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_BAD_STATE - if the netflow has not been initialized or the netflow has started.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate memory.
 * @note When sending more then 30 records the lib splits the records
 * to multiple packets because each packet can only send up to 30 records
 * (Netflow protocol limit)
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_send(const struct doca_telemetry_netflow_template *netflow_template,
					 const void **records,
					 size_t nof_records,
					 size_t *nof_records_sent);

/**
 * @brief Immediately flush the data of the DOCA internal Netflow source.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_BAD_STATE - if the netflow has not been started.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_flush(void);

/**
 * @brief Free the exporter memory and close the connection.
 * @return
 * DOCA_SUCCESS - in case of success.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_netflow_destroy(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_TELEMETRY_NETFLOW_H_ */
