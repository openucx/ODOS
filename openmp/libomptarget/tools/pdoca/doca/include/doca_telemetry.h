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
 * @file doca_telemetry.h
 * @page telemetry
 * @defgroup TELEMETRY Telemetry Service Library
 *
 * DOCA lib for exporting events to the telemetry service.
 *
 * @{
 */

#ifndef DOCA_TELEMETRY_H_
#define DOCA_TELEMETRY_H_

#include <stdint.h>
#include <stddef.h>

#include <doca_compat.h>
#include <doca_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DOCA telemetry IPC status.
 */
typedef enum doca_telemetry_ipc_status {
	DOCA_TELEMETRY_IPC_STATUS_FAILED = -1,
	DOCA_TELEMETRY_IPC_STATUS_CONNECTED,
	DOCA_TELEMETRY_IPC_STATUS_DISABLED,
} doca_telemetry_ipc_status_t;

/**
 * @brief DOCA Telemetry schema
 *
 * Schema is used to prepare user types and create DOCA Telemetry sources.
 */
struct doca_telemetry_schema;

/**
 * @brief DOCA Telemetry source internal type.
 */
struct doca_telemetry_source;

/**
 * @brief DOCA schema field type index.
 */
typedef uint8_t doca_telemetry_type_index_t;

/**
 * @brief DOCA schema type index type.
 */
typedef uint64_t doca_telemetry_timestamp_t;

/**
 * @brief DOCA schema field.
 */
struct doca_telemetry_field;

/**
 * @brief DOCA schema type.
 */
struct doca_telemetry_type;

/**
 * @brief DOCA_TELEMETRY_FIELD_TYPE_{} are data types that
 *        are used to create doca_telemetry_field;
 */
/** @brief DOCA telemetry bool type */
#define DOCA_TELEMETRY_FIELD_TYPE_BOOL "bool"
/** @brief DOCA telemetry char type */
#define DOCA_TELEMETRY_FIELD_TYPE_CHAR "char"
/** @brief DOCA telemetry short type */
#define DOCA_TELEMETRY_FIELD_TYPE_SHORT "short"
/** @brief DOCA telemetry in type */
#define DOCA_TELEMETRY_FIELD_TYPE_INT "int"
/** @brief DOCA telemetry long type */
#define DOCA_TELEMETRY_FIELD_TYPE_LONG "long"
/** @brief DOCA telemetry longlong type */
#define DOCA_TELEMETRY_FIELD_TYPE_LONGLONG "long long"
/** @brief DOCA telemetry uchar type */
#define DOCA_TELEMETRY_FIELD_TYPE_UCHAR "unsigned char"
/** @brief DOCA telemetry ushort type */
#define DOCA_TELEMETRY_FIELD_TYPE_USHORT "unsigned short"
/** @brief DOCA telemetry uint type */
#define DOCA_TELEMETRY_FIELD_TYPE_UINT "unsigned int"
/** @brief DOCA telemetry ulong type */
#define DOCA_TELEMETRY_FIELD_TYPE_ULONG "unsigned long"
/** @brief DOCA telemetry ulonglong type */
#define DOCA_TELEMETRY_FIELD_TYPE_ULONGLONG "unsigned long long"
/** @brief DOCA telemetry float type */
#define DOCA_TELEMETRY_FIELD_TYPE_FLOAT "float"
/** @brief DOCA telemetry double type */
#define DOCA_TELEMETRY_FIELD_TYPE_DOUBLE "double"
/** @brief DOCA telemetry int8 type */
#define DOCA_TELEMETRY_FIELD_TYPE_INT8 "int8_t"
/** @brief DOCA telemetry int16 type */
#define DOCA_TELEMETRY_FIELD_TYPE_INT16 "int16_t"
/** @brief DOCA telemetry int32 type */
#define DOCA_TELEMETRY_FIELD_TYPE_INT32 "int32_t"
/** @brief DOCA telemetry int64 type */
#define DOCA_TELEMETRY_FIELD_TYPE_INT64 "int64_t"
/** @brief DOCA telemetry uint8 type */
#define DOCA_TELEMETRY_FIELD_TYPE_UINT8 "uint8_t"
/** @brief DOCA telemetry uint16 type */
#define DOCA_TELEMETRY_FIELD_TYPE_UINT16 "uint16_t"
/** @brief DOCA telemetry uint32 type */
#define DOCA_TELEMETRY_FIELD_TYPE_UINT32 "uint32_t"
/** @brief DOCA telemetry uint64 type */
#define DOCA_TELEMETRY_FIELD_TYPE_UINT64 "uint64_t"
/** @brief DOCA telemetry timestamp type */
#define DOCA_TELEMETRY_FIELD_TYPE_TIMESTAMP DOCA_TELEMETRY_FIELD_TYPE_UINT64

/** @brief DOCA GUID size */
#define DOCA_GUID_SIZE 16
/**
 * @brief DOCA GUID type
 */
typedef uint8_t doca_guid_t[DOCA_GUID_SIZE];

/* ================================ DOCA SCHEMA ================================ */
/**
 * @brief Initialize DOCA schema to prepare it for setting attributes and adding types.
 * DOCA schema is used to initialize DOCA sources that will collect the data according
 * to the same schema.
 *
 * @param [in] schema_name
 * Name of the schema.
 * @param [out] doca_schema
 * Pointer to DOCA schema, NULL on error.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - failed to allocate doca_schema.
 * - DOCA_ERROR_INITIALIZATION - failed to initialise doca_schema.
 * - DOCA_ERROR_INVALID_VALUE - invalid input/output parameters.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_init(const char *schema_name, struct doca_telemetry_schema **doca_schema);

/**
 * @brief Set buffer size
 * Default value is 60000 bytes.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [in] size
 * Buffer size
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_buf_size(struct doca_telemetry_schema *doca_schema, uint64_t size);

/**
 * @brief Get buffer size
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [out] size
 * The buffer size
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_get_buf_size(struct doca_telemetry_schema *doca_schema, uint64_t *size);

/**
 * @brief Set buffer data root
 * Default path is "/opt/mellanox/doca/services/telemetry/data/"
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [in] path
 * Path to a folder where the data and schema will be stored.
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_buf_data_root(struct doca_telemetry_schema *doca_schema, const char *path);

/**
 * @brief Get data root path
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [out] path
 * Path to a folder where the data and schema will be stored.
 *
 * @note Ownership of the returned string is transferred to the caller.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate memory.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_get_buf_data_root(struct doca_telemetry_schema *doca_schema, const char **path);

/**
 * @brief Enable file write
 * file write is disabled by default.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_file_write_enabled(struct doca_telemetry_schema *doca_schema);

/**
 * @brief Set file maximum size
 * Default value is 1MB.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [in] size
 * Maximum size of binary data file. Once this size is reached, a new binary file will be created.
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_file_write_max_size(struct doca_telemetry_schema *doca_schema, size_t size);

/**
 * @brief Get file maximum size
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [out] size
 * Maximum size of binary data file.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_get_file_write_max_size(struct doca_telemetry_schema *doca_schema, size_t *size);

/**
 * @brief Set file maximum age
 * Default value is 1 hour.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [in] max_age
 * Maximum file age. Once current file is older than this threshold a new file will be created.
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_file_write_max_age(struct doca_telemetry_schema *doca_schema,
						  doca_telemetry_timestamp_t max_age);

/**
 * @brief Get file maximum age
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [out] max_age
 * Maximum file age. Once current file is older than this threshold a new file will be created.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_get_file_write_max_age(struct doca_telemetry_schema *doca_schema,
							  doca_telemetry_timestamp_t *max_age);

/**
 * @brief Enable IPC
 * IPC is disabled by default.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_ipc_enabled(struct doca_telemetry_schema *doca_schema);

/**
 * @brief Set IPC socket directory.
 * Default path is "/opt/mellanox/doca/services/telemetry/ipc_sockets"
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [in] sockets_dir
 * Path to a folder containing DOCA Telemetry Service (DTS) sockets.
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_ipc_sockets_dir(struct doca_telemetry_schema *doca_schema, const char *sockets_dir);

/**
 * @brief Get IPC socket directory.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [out] sockets_dir
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
doca_error_t doca_telemetry_schema_get_ipc_sockets_dir(struct doca_telemetry_schema *doca_schema,
						       const char **sockets_dir);

/**
 * @brief Set IPC reconnect time in milliseconds
 * Time limit for reconnect attempts. If the limit is reached, the client is considered disconnected.
 * Default value is 100 milliseconds.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [in] max_time
 * Maximum reconnect time in milliseconds
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_ipc_reconnect_time(struct doca_telemetry_schema *doca_schema, uint32_t max_time);

/**
 * @brief Get IPC reconnect time in milliseconds
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [out] max_time
 * Maximum reconnect time in milliseconds
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_get_ipc_reconnect_time(struct doca_telemetry_schema *doca_schema,
							  uint32_t *max_time);

/**
 * @brief Set maximum IPC reconnect tries.
 * Number of reconnect attempts during reconnection period.
 * Default value is 3 tries.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [in] tries
 * Maximum reconnect tries
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_ipc_reconnect_tries(struct doca_telemetry_schema *doca_schema, uint8_t tries);

/**
 * @brief Get maximum IPC reconnect tries.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 * @param [out] tries
 * Maximum reconnect tries
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_get_ipc_reconnect_tries(struct doca_telemetry_schema *doca_schema, uint8_t *tries);

/**
 * @brief Set IPC socket timeout in milliseconds
 * Timeout for IPC messaging socket. If timeout is reached during send_receive, the client is considered disconnected.
 * Default value is 3000 milliseconds.
 *
 * @param [in] doca_schema
 * Pointer to ipc timeout attribute.
 * @param [in] timeout
 * Maximum socket timeout in milliseconds
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_ipc_socket_timeout(struct doca_telemetry_schema *doca_schema, uint32_t timeout);

/**
 * @brief Get IPC socket timeout in milliseconds
 *
 * @param [in] doca_schema
 * Pointer to ipc timeout attribute.
 * @param [out] timeout
 * Maximum socket timeout in milliseconds
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_get_ipc_socket_timeout(struct doca_telemetry_schema *doca_schema, uint32_t *timeout);

/**
 * @brief Enable opaque events
 * Opaque events are disabled by default.
 *
 * @param [in] doca_schema
 * Pointer to DOCA schema.
 *
 * @note Passing a doca_schema value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_schema_set_opaque_events_enabled(struct doca_telemetry_schema *doca_schema);

/**
 * @brief Create new telemetry type.
 *
 * @param [out] type
 * Pointer to the newly allocated type.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate doca telemetry field.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_type_create(struct doca_telemetry_type **type);

/**
 * @brief Add DOCA telemetry field to type.
 * The users loses the ownership of the field after a successful invocation of the function.
 *
 * @note field should NOT be passed to another type after calling this function.
 * @param [out] type
 * Pointer to doca telemetry type.
 * @param [out] field
 * DOCA Telemetry field to add.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate doca telemetry field.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_type_add_field(struct doca_telemetry_type *type, struct doca_telemetry_field *field);

/**
 * @brief Destroy doca telemetry type previously created by @ref doca_telemetry_type_create()
 *
 * @param [in] type
 * Pointer to type.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * @note fields added to this type should NOT be used after calling this function.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_type_destroy(struct doca_telemetry_type *type);

/**
 * @brief Create new telemetry field.
 *
 * @param [out] field
 * Pointer to the newly allocated field.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate doca telemetry field.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_field_create(struct doca_telemetry_field **field);

/**
 * @brief Destroy field previously created by @ref doca_telemetry_field_create()
 *
 * @param [in] field
 * Pointer to the field.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_field_destroy(struct doca_telemetry_field *field);

/**
 * @brief Set doca telemetry field name.
 *
 * @param [in] field_info
 * Pointer to doca telemetry field.
 * @param [in] name
 * Field name.
 *
 * @note Passing a field_info value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_field_set_name(struct doca_telemetry_field *field_info, const char *name);

/**
 * @brief Set doca telemetry field description.
 *
 * @param [in] field_info
 * Pointer to doca telemetry field.
 * @param [in] desc
 * Field description.
 *
 * @note Passing a field_info value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_field_set_description(struct doca_telemetry_field *field_info, const char *desc);

/**
 * @brief Set doca telemetry field type.
 * @note Please see DOCA_TELEMETRY_FIELD_TYPE_* for possible field types
 *
 * @param [in] field_info
 * Pointer to doca telemetry field.
 * @param [in] type
 * Field type.
 *
 * @note Passing a field_info value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_field_set_type_name(struct doca_telemetry_field *field_info, const char *type);

/**
 * @brief Set doca telemetry field length.
 * @note If using single-value type (i.e char) this should be 1.
 *
 * @param [in] field_info
 * Pointer to doca telemetry field.
 * @param [in] len
 * Field length.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_field_set_array_len(struct doca_telemetry_field *field_info, uint16_t len);

/**
 * @brief Add user-defined fields to create new type in DOCA schema.
 * The users loses the ownership of the type after a successful invocation of the function.
 *
 * @param [in] doca_schema
 * Schema to create type in.
 * @param [in] new_type_name
 * Name for new type.
 * @param [in] type
 * User-defined fields.
 * @param [out] type_index
 * Type index for the created type is written to this variable.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - in case of memory allocation failure.
 * - DOCA_ERROR_INVALID_VALUE - If type name exists or any of the fields have invalid field type
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_add_type(struct doca_telemetry_schema *doca_schema,
					    const char *new_type_name,
					    struct doca_telemetry_type *type,
					    doca_telemetry_type_index_t *type_index);

/**
 * @brief Destructor for DOCA schema.
 *
 * @param [in] doca_schema
 * Schema to destroy.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if doca_schema is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_destroy(struct doca_telemetry_schema *doca_schema);

/**
 * @brief Finalizes schema setup to start creating Doca Sources from the schema.
 *
 * Do NOT add new types after this function was called.
 *
 * @param [in] doca_schema
 * Input schema to start.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INITIALIZATION - in case of failure.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_schema_start(struct doca_telemetry_schema *doca_schema);

/* ================================ DOCA SOURCE ================================ */

/**
 * @brief Creates a single DOCA source from schema.
 *
 * To create a DOCA source, first call doca_telemetry_schema_start() to prepare
 * the DOCA schema.
 *
 * @param [in] doca_schema
 * Schema from which source will be created.
 * @param [out] doca_source
 * pointer to DOCA source, or NULL on error.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - in case of memory allocation failure.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_source_create(struct doca_telemetry_schema *doca_schema,
					  struct doca_telemetry_source **doca_source);

/**
 * @brief Set source id
 *
 * @param [in] doca_source
 * Pointer to DOCA source.
 * @param [in] source_id
 * Hostname or guid.
 *
 * @note Passing a doca_source value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_source_set_id(struct doca_telemetry_source *doca_source, const char *source_id);

/**
 * @brief Set source tag
 *
 * @param [in] doca_source
 * Pointer to DOCA source.
 * @param [in] source_tag
 * User defined data-file name prefix.
 *
 * @note Passing a doca_source value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_telemetry_source_set_tag(struct doca_telemetry_source *doca_source, const char *source_tag);

/**
 * @brief Applies source attribute and starts DOCA source.
 *
 * Call this function to start reporting.
 *
 * @param [in] doca_source
 * DOCA source to start.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if source attributes are not set.
 * - DOCA_ERROR_NO_MEMORY - in case of memory allocation failure.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_source_start(struct doca_telemetry_source *doca_source);

/**
 * @brief Report events data of the same type via DOCA source.
 *
 * Data is flushed from internal buffer when the buffer is full.
 * Flushing the data immediately can be done by invoking @ref doca_telemetry_source_flush().
 * This function is not thread-safe and should not be called from different threads without proper access control.
 *
 *
 * @param [in] doca_source
 * Source to report.
 * @param [in] index
 * Type index in the DOCA schema.
 * @param [in] data
 * Data buffer.
 * @param [in] count
 * Number of events written to the data buffer.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - in case of memory allocation failure.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_source_report(struct doca_telemetry_source *doca_source,
					  doca_telemetry_type_index_t index,
					  void *data,
					  int count);

/**
 * @brief Report opaque event data via DOCA source.
 *
 * Data is flushed from internal buffer when the buffer is full.
 * Flushing the data immediately can be done by invoking @ref doca_telemetry_source_flush().
 *
 * @param [in] doca_source
 * Source to report.
 * @param [in] app_id
 * User defined application ID.
 * @param [in] user_defined1
 * User defined parameter 1.
 * @param [in] user_defined2
 * User defined parameter 2.
 * @param [in] data
 * Data buffer.
 * @param [in] data_size
 * Size of the data in the data buffer.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_NO_MEMORY - in case of memory allocation failure.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_source_opaque_report(struct doca_telemetry_source *doca_source,
						 const doca_guid_t app_id,
						 uint64_t user_defined1,
						 uint64_t user_defined2,
						 const void *data,
						 uint32_t data_size);

/**
 * @brief Get max data size for opaque report
 *
 * @param [in] doca_source
 * Source to report.
 * @param [out] max_data_size
 * Maximal data size
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - NULL parameter(s).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_source_get_opaque_report_max_data_size(struct doca_telemetry_source *doca_source,
								   uint32_t *max_data_size);

/**
 * @brief Immediately flush the data of the DOCA source.
 * This function is not thread-safe and should not be called from different threads without proper access control.
 *
 * @param [in] doca_source
 * DOCA source to flush.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if doca_source is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_source_flush(struct doca_telemetry_source *doca_source);

/**
 * @brief Destructor for DOCA source.
 *
 * @param [in] doca_source
 * Source to destroy.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if doca_source is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_source_destroy(struct doca_telemetry_source *doca_source);

/* ================================ Utilities ================================ */

/**
 * @brief Get timestamp in the proper format.
 *
 * @param [out] timestamp
 * Timestamp value
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if doca_source is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_get_timestamp(doca_telemetry_timestamp_t *timestamp);

/**
 * @brief Return status of IPC transport
 *
 * @param [in] doca_source
 * Input doca source.
 * @param [out] status
 * if return is DOCA_SUCCESS then status can be one of the following
 * - DOCA_TELEMETRY_IPC_STATUS_FAILED - if IPC is not connected.
 * - DOCA_TELEMETRY_IPC_STATUS_CONNECTED - if IPC is connected.
 * - DOCA_TELEMETRY_IPC_STATUS_DISABLED - if IPC is disabled from config.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if doca_source is NULL.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_telemetry_check_ipc_status(struct doca_telemetry_source *doca_source,
					     doca_telemetry_ipc_status_t *status);

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_TELEMETRY_H_ */
