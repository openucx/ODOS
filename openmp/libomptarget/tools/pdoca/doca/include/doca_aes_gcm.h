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
 * @file doca_aes_gcm.h
 * @page DOCA AES-GCM
 * @defgroup DOCAAES-GCM DOCA AES-GCM engine
 * DOCA AES-GCM library. For more details please refer to the user guide on DOCA devzone.
 *
 * @{
 */
#ifndef DOCA_AES_GCM_H_
#define DOCA_AES_GCM_H_

#include <inttypes.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * DOCA core opaque types
 *********************************************************************************************************************/

struct doca_buf;
struct doca_ctx;
struct doca_dev;
struct doca_devinfo;

/*********************************************************************************************************************
 * DOCA AES-GCM Context
 *********************************************************************************************************************/

/**
 * Opaque structure representing a DOCA AES-GCM instance.
 */
struct doca_aes_gcm;

/**
 * Create a DOCA AES-GCM instance.
 *
 * @param [in] dev
 * The device to attach to the aes_gcm context
 * @param [out] aes_gcm
 * Pointer to pointer to be set to point to the created doca_aes_gcm instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - one or more of the arguments is null.
 * - DOCA_ERROR_NOT_SUPPORTED - failed to query device capabilities.
 * - DOCA_ERROR_NO_MEMORY - failed to alloc doca_aes_gcm.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_create(struct doca_dev *dev, struct doca_aes_gcm **aes_gcm);

/**
 * Destroy a DOCA AES-GCM instance.
 *
 * @param [in] aes_gcm
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_IN_USE - if unable to gain exclusive access to the aes_gcm instance
 *                       or if there are undestroyed DOCA AES-GCM keys.
 * - DOCA_ERROR_BAD_STATE - aes_gcm context is not in idle state, try to stop the context.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_destroy(struct doca_aes_gcm *aes_gcm);

/**
 * Adapt doca_aes_gcm instance into a generalized context for use with doca core objects.
 *
 * @param [in] aes_gcm
 * AES-GCM instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * Non NULL upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_aes_gcm_as_ctx(struct doca_aes_gcm *aes_gcm);

/**
 * Get the maximum number of tasks
 *
 * @details This method retrieves the maximum number of tasks for a device. Sum of num tasks should not exceed
 * this number.
 *
 * @param [in] aes_gcm
 * AES-GCM context to get max number of tasks from
 * @param [out] max_num_tasks
 * Sum of num tasks should not exceed this number (@see doca_aes_gcm_set_aes_gcm_encrypt_task_conf,
 * doca_aes_gcm_set_aes_gcm_decrypt_task_conf)
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_get_max_num_tasks(struct doca_aes_gcm *aes_gcm, uint32_t *max_num_tasks);

/*********************************************************************************************************************
 * DOCA AES-GCM KEY
 *********************************************************************************************************************/

/**
 * Opaque structure representing a DOCA AES_GCM key instance.
 */
struct doca_aes_gcm_key;

/**
 * @brief AES-GCM key type
 */
enum doca_aes_gcm_key_type {
	DOCA_AES_GCM_KEY_128 = 1, /**< key size of 128 bit */
	DOCA_AES_GCM_KEY_256 = 2, /**< key size of 256 bit */
};

/**
 * @brief Create an AES-GCM key from the user raw key to send with the task to allow encrypt/decrypt operations.
 *
 * @note Need to attach device to ctx before calling this function
 *
 * @param [in] aes_gcm
 * AES_GCM instance.
 * @param [in] raw_key
 * The raw key given by the user, only 128bit or 256bit keys are supported
 * @param [in] raw_key_type
 * The raw key type given by the user. See enum doca_aes_gcm_key_type.
 * @param [out] key
 * Pointer to pointer to be set to point to the created AES-GCM key to allow encrypt/decrypt operations.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - if key type is not supported by the device or if failed to create DOCA AES-GCM key.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_key_create(struct doca_aes_gcm *aes_gcm,
				     const void *raw_key,
				     enum doca_aes_gcm_key_type raw_key_type,
				     struct doca_aes_gcm_key **key);

/**
 * @brief Destroy AES-GCM key that was created in doca_aes_gcm_key_create.
 *
 * @param [in] key
 * The AES-GCM key to allow encrypt/decrypt operations.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_DRIVER - low level layer failure.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_key_destroy(struct doca_aes_gcm_key *key);

/************************************************************************/
/* AES-GCM ENCRYPT TASK                                                 */
/************************************************************************/

/**
 * @brief AES-GCM encrypt task
 */
struct doca_aes_gcm_task_encrypt;

/**
 * @brief Function to execute on aes_gcm encrypt task completion.
 *
 * @param [in] task
 * aes_gcm encrypt task.
 * The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * task user data
 * @param [in] ctx_user_data
 * doca_ctx user data
 */
typedef void (*doca_aes_gcm_task_encrypt_completion_cb_t)(struct doca_aes_gcm_task_encrypt *task,
							  union doca_data task_user_data,
							  union doca_data ctx_user_data);

/**
 * @brief Check if a aes_gcm encrypt task is supported by a device
 *
 * @details This method checks if a aes_gcm encrypt task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_encrypt_is_supported(const struct doca_devinfo *devinfo);

/**
 * Get aes_gcm encrypt maximum initialization vector length for a device
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_iv_len
 * The max iv length in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support encrypt task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_encrypt_get_max_iv_len(const struct doca_devinfo *devinfo, uint32_t *max_iv_len);

/**
 * Check if authentication tag of size 96-bit for encrypt task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports authentication tag of size 96-bit
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - authentication tag of size 96-bit is not supported by the device or devinfo does not
 *                              support encrypt task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_encrypt_is_tag_96_supported(const struct doca_devinfo *devinfo);

/**
 * Check if authentication tag of size 128-bit for encrypt task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports authentication tag of size 128-bit
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - authentication tag of size 128-bit is not supported by the device or devinfo does not
 *                              support encrypt task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_encrypt_is_tag_128_supported(const struct doca_devinfo *devinfo);

/**
 * Check if a given AES-GCM key type for encrypt task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [in] key_type
 * key type to check. See enum doca_aes_gcm_key_type.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the AES-GCM key type for encrypt task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - AES-GCM key type for encrypt task is not supported by the device or devinfo does not
 *                              support encrypt task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_encrypt_is_key_type_supported(const struct doca_devinfo *devinfo,
								 enum doca_aes_gcm_key_type key_type);

/**
 * @brief Get aes_gcm encrypt max buffer size
 *
 * @details This method retrieves a aes_gcm encrypt max size for a device
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_buffer_size
 * The max buffer size for aes_gcm encrypt operation in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_encrypt_get_max_buf_size(const struct doca_devinfo *devinfo,
							    uint64_t *max_buffer_size);

/**
 * Get the maximum supported number of elements in DOCA linked-list buffer for aes_gcm encrypt task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_list_num_elem
 * The maximum supported number of elements in DOCA linked-list buffer.
 * The value 1 indicates that only a single element is supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_encrypt_get_max_list_buf_num_elem(const struct doca_devinfo *devinfo,
								     uint32_t *max_list_num_elem);

/**
 * @brief This method sets the aes_gcm encrypt task configuration
 *
 * @param [in] aes_gcm
 * The aes_gcm context to config
 * @param [in] task_completion_cb
 * Task completion callback
 * @param [in] task_error_cb
 * Task error callback
 * @param [in] num_tasks
 * Number of aes_gcm encrypt tasks that the context can allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - aes_gcm context is not in idle state, try to stop the context.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_task_encrypt_set_conf(struct doca_aes_gcm *aes_gcm,
						doca_aes_gcm_task_encrypt_completion_cb_t task_completion_cb,
						doca_aes_gcm_task_encrypt_completion_cb_t task_error_cb,
						uint32_t num_tasks);

/**
 * @brief Allocate aes_gcm encrypt task
 *
 * @details This method allocates and initializes a aes_gcm encrypt task. Task parameters can be set later on
 * by setters.
 *
 * @param [in] aes_gcm
 * The aes_gcm context to allocate the task from
 * @param [in] src_buff
 * Source buffer
 * @param [in] dst_buff
 * Destination buffer
 * @param [in] key
 * DOCA AES-GCM key
 * @param [in] iv
 * Initialization vector
 * @param [in] iv_length
 * Initialization vector length in bytes, 0B-12B values are supported
 * @param [in] tag_size
 * Authentication tag size in bytes, only 12B and 16B values are supported
 * @param [in] aad_size
 * Additional authenticated data size in bytes
 * @param [in] user_data
 * doca_data that can be retrieved from the task (usually when the task is completed).
 * @param [out] task
 * The allocated task
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - all aes_gcm encrypt tasks are already allocated.
 * - DOCA_ERROR_BAD_STATE - aes_gcm context is not in running state, try to start the context.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_task_encrypt_alloc_init(struct doca_aes_gcm *aes_gcm,
						  struct doca_buf const *src_buff,
						  struct doca_buf *dst_buff,
						  struct doca_aes_gcm_key *key,
						  const uint8_t *iv,
						  uint32_t iv_length,
						  uint32_t tag_size,
						  uint32_t aad_size,
						  union doca_data user_data,
						  struct doca_aes_gcm_task_encrypt **task);

/**
 * @brief convert aes_gcm encrypt task to doca_task
 *
 * @param [in] task
 * The task to convert
 * @return doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_aes_gcm_task_encrypt_as_task(struct doca_aes_gcm_task_encrypt *task);

/**
 * @brief set aes_gcm encrypt task source
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] src_buff
 * Source buffer to set
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_encrypt_set_src(struct doca_aes_gcm_task_encrypt *task, struct doca_buf const *src_buff);

/**
 * @brief get aes_gcm encrypt task source
 *
 * @param [in] task
 * Task to get the source from
 *
 * @return source buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_aes_gcm_task_encrypt_get_src(const struct doca_aes_gcm_task_encrypt *task);

/**
 * @brief set aes_gcm encrypt task destination
 *
 * @param [in] task
 * Task to set the destination to
 * @param [in] dst_buff
 * destination buffer to set
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_encrypt_set_dst(struct doca_aes_gcm_task_encrypt *task, struct doca_buf *dst_buff);

/**
 * @brief get aes_gcm encrypt task destination
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return destination buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_aes_gcm_task_encrypt_get_dst(const struct doca_aes_gcm_task_encrypt *task);

/**
 * @brief set aes_gcm encrypt task doca_aes_gcm_key
 *
 * @param [in] task
 * Task to set the doca_aes_gcm_key to
 * @param [in] key
 * DOCA AES-GCM key
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_encrypt_set_key(struct doca_aes_gcm_task_encrypt *task, struct doca_aes_gcm_key *key);

/**
 * @brief get aes_gcm encrypt task doca_aes_gcm_key
 *
 * @param [in] task
 * Task to get the doca_aes_gcm_key from
 *
 * @return DOCA AES-GCM key.
 */
DOCA_EXPERIMENTAL
struct doca_aes_gcm_key *doca_aes_gcm_task_encrypt_get_key(const struct doca_aes_gcm_task_encrypt *task);

/**
 * @brief set aes_gcm encrypt task initialization vector
 *
 * @param [in] task
 * Task to set the initialization vector to
 * @param [in] iv
 * Initialization vector
 * @param [in] iv_length
 * Initialization vector length in bytes
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_encrypt_set_iv(struct doca_aes_gcm_task_encrypt *task, const uint8_t *iv, uint32_t iv_length);

/**
 * @brief get aes_gcm encrypt task initialization vector
 *
 * @param [in] task
 * Task to get the initialization vector from
 * @param [out] iv_length
 * Initialization vector length in bytes
 *
 * @return initialization vector
 */
DOCA_EXPERIMENTAL
const uint8_t *doca_aes_gcm_task_encrypt_get_iv(const struct doca_aes_gcm_task_encrypt *task, uint32_t *iv_length);

/**
 * @brief set aes_gcm encrypt task authentication tag size
 *
 * @param [in] task
 * Task to set the authentication tag size to
 * @param [in] tag_size
 * Authentication tag size in bytes
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_encrypt_set_tag_size(struct doca_aes_gcm_task_encrypt *task, uint32_t tag_size);

/**
 * @brief get aes_gcm encrypt task authentication tag size
 *
 * @param [in] task
 * Task to get the authentication tag size from
 *
 * @return authentication tag size in bytes
 */
DOCA_EXPERIMENTAL
uint32_t doca_aes_gcm_task_encrypt_get_tag_size(const struct doca_aes_gcm_task_encrypt *task);

/**
 * @brief set aes_gcm encrypt task additional authenticated data size
 *
 * @param [in] task
 * Task to set the additional authenticated data size to
 * @param [in] aad_size
 * Additional authenticated data size in bytes
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_encrypt_set_aad_size(struct doca_aes_gcm_task_encrypt *task, uint32_t aad_size);

/**
 * @brief get aes_gcm encrypt task additional authenticated data size
 *
 * @param [in] task
 * Task to get the additional authenticated data size from
 *
 * @return additional authenticated data size in bytes
 */
DOCA_EXPERIMENTAL
uint32_t doca_aes_gcm_task_encrypt_get_aad_size(const struct doca_aes_gcm_task_encrypt *task);

/************************************************************************/
/* AES-GCM DECRYPT TASK                                                 */
/************************************************************************/

/**
 * @brief AES-GCM decrypt task
 */
struct doca_aes_gcm_task_decrypt;

/**
 * @brief Function to execute on aes_gcm decrypt task completion.
 *
 * @param [in] task
 * aes_gcm decrypt task.
 * The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * task user data
 * @param [in] ctx_user_data
 * doca_ctx user data
 */
typedef void (*doca_aes_gcm_task_decrypt_completion_cb_t)(struct doca_aes_gcm_task_decrypt *task,
							  union doca_data task_user_data,
							  union doca_data ctx_user_data);

/**
 * @brief Check if a aes_gcm decrypt task is supported by a device
 *
 * @details This method checks if a aes_gcm decrypt task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_decrypt_is_supported(const struct doca_devinfo *devinfo);

/**
 * Get aes_gcm decrypt maximum initialization vector length for a device
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_iv_len
 * The max iv length in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support decrypt task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_decrypt_get_max_iv_len(const struct doca_devinfo *devinfo, uint32_t *max_iv_len);

/**
 * Check if authentication tag of size 96-bit for decrypt task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports authentication tag of size 96-bit
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - authentication tag of size 96-bit is not supported by the device or devinfo does not
 *                              support decrypt task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_decrypt_is_tag_96_supported(const struct doca_devinfo *devinfo);

/**
 * Check if authentication tag of size 128-bit for decrypt task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 *
 * @return
 * DOCA_SUCCESS - in case device supports authentication tag of size 128-bit
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - authentication tag of size 128-bit is not supported by the device or devinfo does not
 *                              support decrypt task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_decrypt_is_tag_128_supported(const struct doca_devinfo *devinfo);

/**
 * Check if a given AES-GCM key type for decrypt task is supported by a device
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [in] key_type
 * key type to check. See enum doca_aes_gcm_key_type.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the AES-GCM key type for decrypt task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - AES-GCM key type for decrypt task is not supported by the device or devinfo does not
 *                              support decrypt task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_decrypt_is_key_type_supported(const struct doca_devinfo *devinfo,
								 enum doca_aes_gcm_key_type key_type);

/**
 * @brief Get aes_gcm decrypt max buffer size
 *
 * @details This method retrieves aes_gcm decrypt max size
 *
 * @param [in] devinfo
 * doca device info to check
 * @param [out] max_buffer_size
 * The max buffer size for aes_gcm decrypt operation in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case device supports the task
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_decrypt_get_max_buf_size(const struct doca_devinfo *devinfo,
							    uint64_t *max_buffer_size);

/**
 * Get the maximum supported number of elements in DOCA linked-list buffer for aes_gcm decrypt task.
 *
 * @param [in] devinfo
 * The DOCA device information.
 * @param [out] max_list_num_elem
 * The maximum supported number of elements in DOCA linked-list buffer.
 * The value 1 indicates that only a single element is supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - devinfo does not support the task or failed to query device capabilities.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_cap_task_decrypt_get_max_list_buf_num_elem(const struct doca_devinfo *devinfo,
								     uint32_t *max_list_num_elem);

/**
 * @brief This method sets the aes_gcm decrypt task configuration
 *
 * @param [in] aes_gcm
 * The aes_gcm context to config
 * @param [in] task_completion_cb
 * Task completion callback
 * @param [in] task_error_cb
 * Task error callback
 * @param [in] num_tasks
 * Number of aes_gcm decrypt tasks that the context can allocate
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - aes_gcm context is not in idle state, try to stop the context.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_task_decrypt_set_conf(struct doca_aes_gcm *aes_gcm,
						doca_aes_gcm_task_decrypt_completion_cb_t task_completion_cb,
						doca_aes_gcm_task_decrypt_completion_cb_t task_error_cb,
						uint32_t num_tasks);

/**
 * @brief Allocate aes_gcm decrypt task
 *
 * @details This method allocates and initializes a aes_gcm decrypt task. Task parameters can be set later on
 * by setters.
 *
 * @param [in] aes_gcm
 * The aes_gcm context to allocate the task from
 * @param [in] src_buff
 * Source buffer
 * @param [in] dst_buff
 * Destination buffer
 * @param [in] key
 * DOCA AES-GCM key
 * @param [in] iv
 * Initialization vector
 * @param [in] iv_length
 * Initialization vector length in bytes, 0B-12B values are supported
 * @param [in] tag_size
 * Authentication tag size in bytes, only 12B and 16B values are supported
 * @param [in] aad_size
 * Additional authenticated data size in bytes
 * @param [in] user_data
 * doca_data that can be retrieved from the task (usually when the task is completed).
 * @param [out] task
 * The allocated task
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - all aes_gcm decrypt tasks are already allocated.
 * - DOCA_ERROR_BAD_STATE - aes_gcm context is not in running state, try to start the context.
 *
 */
DOCA_EXPERIMENTAL
doca_error_t doca_aes_gcm_task_decrypt_alloc_init(struct doca_aes_gcm *aes_gcm,
						  struct doca_buf const *src_buff,
						  struct doca_buf *dst_buff,
						  struct doca_aes_gcm_key *key,
						  const uint8_t *iv,
						  uint32_t iv_length,
						  uint32_t tag_size,
						  uint32_t aad_size,
						  union doca_data user_data,
						  struct doca_aes_gcm_task_decrypt **task);

/**
 * @brief convert aes_gcm decrypt task to doca_task
 *
 * @param [in] task
 * The task to convert
 * @return doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_aes_gcm_task_decrypt_as_task(struct doca_aes_gcm_task_decrypt *task);

/**
 * @brief set aes_gcm decrypt task source
 *
 * @param [in] task
 * Task to set the source to
 * @param [in] src_buff
 * Source buffer to set
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_decrypt_set_src(struct doca_aes_gcm_task_decrypt *task, struct doca_buf const *src_buff);

/**
 * @brief get aes_gcm decrypt task source
 *
 * @param [in] task
 * Task to get the source from
 *
 * @return source buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf const *doca_aes_gcm_task_decrypt_get_src(const struct doca_aes_gcm_task_decrypt *task);

/**
 * @brief set aes_gcm decrypt task destination
 *
 * @param [in] task
 * Task to set the destination to
 * @param [in] dst_buff
 * destination buffer to set
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_decrypt_set_dst(struct doca_aes_gcm_task_decrypt *task, struct doca_buf *dst_buff);

/**
 * @brief get aes_gcm decrypt task destination
 *
 * @param [in] task
 * Task to get the destination from
 *
 * @return destination buffer
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_aes_gcm_task_decrypt_get_dst(const struct doca_aes_gcm_task_decrypt *task);

/**
 * @brief set aes_gcm decrypt task doca_aes_gcm_key
 *
 * @param [in] task
 * Task to set the doca_aes_gcm_key to
 * @param [in] key
 * DOCA AES-GCM key
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_decrypt_set_key(struct doca_aes_gcm_task_decrypt *task, struct doca_aes_gcm_key *key);

/**
 * @brief get aes_gcm decrypt task doca_aes_gcm_key
 *
 * @param [in] task
 * Task to get the doca_aes_gcm_key from
 *
 * @return DOCA AES-GCM key.
 */
DOCA_EXPERIMENTAL
struct doca_aes_gcm_key *doca_aes_gcm_task_decrypt_get_key(const struct doca_aes_gcm_task_decrypt *task);

/**
 * @brief set aes_gcm decrypt task initialization vector
 *
 * @param [in] task
 * Task to set the initialization vector to
 * @param [in] iv
 * Initialization vector
 * @param [in] iv_length
 * Initialization vector length in bytes
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_decrypt_set_iv(struct doca_aes_gcm_task_decrypt *task, const uint8_t *iv, uint32_t iv_length);

/**
 * @brief get aes_gcm decrypt task initialization vector
 *
 * @param [in] task
 * Task to get the initialization vector from
 * @param [out] iv_length
 * Initialization vector length in bytes
 *
 * @return initialization vector
 */
DOCA_EXPERIMENTAL
const uint8_t *doca_aes_gcm_task_decrypt_get_iv(const struct doca_aes_gcm_task_decrypt *task, uint32_t *iv_length);

/**
 * @brief set aes_gcm decrypt task authentication tag size
 *
 * @param [in] task
 * Task to set the authentication tag size to
 * @param [in] tag_size
 * Authentication tag size in bytes
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_decrypt_set_tag_size(struct doca_aes_gcm_task_decrypt *task, uint32_t tag_size);

/**
 * @brief get aes_gcm decrypt task authentication tag size
 *
 * @param [in] task
 * Task to get the authentication tag size from
 *
 * @return authentication tag size in bytes
 */
DOCA_EXPERIMENTAL
uint32_t doca_aes_gcm_task_decrypt_get_tag_size(const struct doca_aes_gcm_task_decrypt *task);

/**
 * @brief set aes_gcm decrypt task additional authenticated data size
 *
 * @param [in] task
 * Task to set the additional authenticated data size to
 * @param [in] aad_size
 * Additional authenticated data size in bytes
 */
DOCA_EXPERIMENTAL
void doca_aes_gcm_task_decrypt_set_aad_size(struct doca_aes_gcm_task_decrypt *task, uint32_t aad_size);

/**
 * @brief get aes_gcm decrypt task additional authenticated data size
 *
 * @param [in] task
 * Task to get the additional authenticated data size from
 *
 * @return additional authenticated data size in bytes
 */
DOCA_EXPERIMENTAL
uint32_t doca_aes_gcm_task_decrypt_get_aad_size(const struct doca_aes_gcm_task_decrypt *task);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_AES_GCM_H_ */

/** @} */
