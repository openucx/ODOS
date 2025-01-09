/*
 * Copyright (c) 2021-2024 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_argp.h
 * @page doca argp
 * @defgroup DOCA_ARGP DOCA Arg Parser
 * DOCA Arg Parser library. For more details please refer to the user guide on DOCA DevZone.
 *
 * @{
 */

#ifndef DOCA_ARGP_H_
#define DOCA_ARGP_H_

#include <doca_compat.h>
#include <doca_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Flag callback function type */
typedef doca_error_t (*doca_argp_param_cb_t)(void *, void *);

/** @brief DPDK flags callback function type */
typedef doca_error_t (*doca_argp_dpdk_cb_t)(int argc, char **argv);

/** @brief Program validation callback function type */
typedef doca_error_t (*doca_argp_validation_cb_t)(void *);

/**
 * @brief Flag input type
 */
enum doca_argp_type {
	DOCA_ARGP_TYPE_UNKNOWN = 0,
	DOCA_ARGP_TYPE_STRING,	 /**< Input type is a string */
	DOCA_ARGP_TYPE_INT,	 /**< Input type is an integer */
	DOCA_ARGP_TYPE_BOOLEAN,	 /**< Input type is a boolean */
	DOCA_ARGP_TYPE_JSON_OBJ, /**< DPDK Param input type is a json object,
				  * only for json mode */
};

/**
 * @brief Program flag information
 *
 * @note It is the programmer's responsibility to ensure the callback will copy the content of the param passed to it.
 * The pointer pointing to the param is owned by doca_argp, and it is only valid in the scope of the called callback.
 */
struct doca_argp_param;

/**
 * @brief Print usage instructions.
 *
 * @note: When calling this function the program will exit.
 */
DOCA_EXPERIMENTAL
void doca_argp_usage(void);

/**
 * @brief Initialize the parser interface.
 *
 * @param [in] program_name
 * Name of current program, using the name for usage print.
 * @param [in] program_config
 * Program configuration struct.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * @note After a successful call to this function, doca_argp_destroy() should be called as part of program cleanup.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_init(const char *program_name, void *program_config);

/**
 * @brief Register a program flag.
 *
 * @param [in] input_param
 * Program flag details.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_INITIALIZATION - received param with missing mandatory fields initialization.
 * @note Value of is_cli_only field may be changed in this function.
 * @note ARGP takes ownership of the pointer and will free the param including in case of failure.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_register_param(struct doca_argp_param *input_param);

/**
 * @brief Register an alternative version callback.
 *
 * @param [in] callback
 * Program-specific version callback.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * @note: When calling version callback the program will exit.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_register_version_callback(doca_argp_param_cb_t callback);

/**
 * @brief Register program validation callback function.
 *
 * @param [in] callback
 * Program validation callback.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 *
 * @note When calling validation callback, will pass one argument which is the program configuration struct.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_register_validation_callback(doca_argp_validation_cb_t callback);

/**
 * @brief Parse incoming arguments (cmd line/json).
 *
 * @param [in] argc
 * Number of program command line arguments.
 * @param [in] argv
 * Program command line arguments.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_NOT_SUPPORTED - received unsupported program flag.
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_IO_FAILED - Internal errors about JSON API, reading JSON content.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate enough space.
 * - DOCA_ERROR_INITIALIZATION - initialization error.
 * @note: if the program is based on DPDK API, DPDK flags will be forwarded to it by calling the registered callback.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_start(int argc, char **argv);

/**
 * @brief ARG Parser destroy.
 *
 * cleanup all resources including the parsed DPDK flags, the program can't use them any more.
 *
 * @return
 * DOCA_SUCCESS - in case of success, the relevant error otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_destroy(void);

/**
 * @brief Set information about program configuration, if it's based on DPDK API.
 *
 * @param [in] callback
 * Once ARGP finished parsing DPDK flags will be forwarded to the program by calling this callback.
 *
 * @note Need to call doca_argp_init before setting program DPDK type.
 * @note If program is based on DPDK API, DPDK flags array will be sent using the callback,
 * the array will be released when calling doca_argp_destroy.
 */
DOCA_EXPERIMENTAL
void doca_argp_set_dpdk_program(doca_argp_dpdk_cb_t callback);

/**
 * @brief Create new program param.
 *
 * @param [out] param
 * Create program param instance on success. Valid on success only.
 *
 * @note Need to set param fields by setter functions.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate enough space.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_param_create(struct doca_argp_param **param);

/**
 * @brief Set the short name of the program param.
 *
 * @param [in] param
 * The program param.
 * @param [in] name
 * The param's short name
 *
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 * @note At least one of param names should be set.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_short_name(struct doca_argp_param *param, const char *name);

/**
 * @brief Set the long name of the program param.
 *
 * @param [in] param
 * The program param.
 * @param [in] name
 * The param's long name.
 *
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 * @note At least one of param names should be set.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_long_name(struct doca_argp_param *param, const char *name);

/**
 * @brief Set the expected arguments of the program param, used to print the program usage.
 *
 * @param [in] param
 * The program param.
 * @param [in] arguments
 * The param's arguments.
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_arguments(struct doca_argp_param *param, const char *arguments);

/**
 * @brief Set the description of the program param, used to print the program usage.
 *
 * @param [in] param
 * The program param.
 * @param [in] description
 * The param's description.
 *
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 * @note Set param description is mandatory.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_description(struct doca_argp_param *param, const char *description);

/**
 * @brief Set the callback function of the program param.
 *
 * @param [in] param
 * The program param.
 * @param [in] callback
 * The param's callback function.
 *
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 * @note Once ARGP identifies this param in CLI, will call the callback function with attaching the param argument value
 * as first argument and next the program configuration struct. Program should copy the argument value and shouldn't
 * use it directly.
 * @note Set param callback is mandatory.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_callback(struct doca_argp_param *param, doca_argp_param_cb_t callback);

/**
 * @brief Set the type of the param arguments.
 *
 * @param [in] param
 * The program param.
 * @param [in] type
 * The param arguments type.
 *
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 * @note Set param arguments type is mandatory.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_type(struct doca_argp_param *param, enum doca_argp_type type);

/**
 * @brief Set if the program param is mandatory, by default the value is false.
 *
 * @param [in] param
 * The program param.
 *
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_mandatory(struct doca_argp_param *param);

/**
 * @brief Set if the program param is supported only CLI mode and will not be used in JSON file, by default the value
 * is false.
 *
 * @param [in] param
 * The program param.
 *
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_cli_only(struct doca_argp_param *param);

/**
 * @brief Set if the program param will appear multiple times, by default the value is false.
 *
 * @param [in] param
 * The program param.
 *
 * @note Passing a "param" value of NULL will result in an undefined behavior.
 * @note Since JSON file doesn't support keys multiplicity, the multi values will be in array
 * and param argument type will indicate the values type.
 */
DOCA_EXPERIMENTAL
void doca_argp_param_set_multiplicity(struct doca_argp_param *param);

/**
 * @brief Destroy a program param.
 *
 * @param [in] param
 * The program param to destroy.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_param_destroy(struct doca_argp_param *param);

/**
 * @brief Get the log level the user inserted it.
 *
 * @param [out] log_level
 * The log level if the user inserted it, otherwise the default value of log level.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_get_log_level(int *log_level);

/**
 * @brief Get the SDK log level the user inserted it.
 *
 * @param [out] log_level
 * The log level if the user inserted it, otherwise the default value of log level.
 *
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_argp_get_sdk_log_level(int *log_level);

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_ARGP_H_ */
