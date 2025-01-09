/*
 * Copyright (c) 2022-2023 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_erasure_coding.h
 * @page DOCA_ERASURE_CODING
 * @defgroup DOCA_ERASURE_CODING DOCA Erasure Coding engine
 * DOCA Erasure Coding library. For more details please refer to the user guide on DOCA devzone.
 *
 * @{
 */
#ifndef DOCA_ERASURE_CODING_H_
#define DOCA_ERASURE_CODING_H_

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

/**
 * @brief Opaque structure representing a DOCA EC instance.
 */
struct doca_ec;

/**
 * @brief Opaque structure representing a DOCA EC matrix (coding_matrix).
 */
struct doca_ec_matrix;

/*********************************************************************************************************************
 * DOCA EC TASKS
 *********************************************************************************************************************/

/*****************************************
 * DOCA EC TASK - Galois multiplication  *
 *****************************************/
/**
 * @brief This task preforms a Galois multiplication.
 */
struct doca_ec_task_galois_mul;

/**
 * @brief Function to execute on completion of a Galois multiplication task.
 *
 * @details This function is called by doca_pe_progress() when a Galois multiplication task is successfully identified
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
 * The completed Galois multiplication task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_ec_task_galois_mul_completion_cb_t)(struct doca_ec_task_galois_mul *task,
							union doca_data task_user_data,
							union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a Galois multiplication task.
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
doca_error_t doca_ec_cap_task_galois_mul_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the Galois multiplication tasks configuration.
 *
 * @param [in] ec
 * The EC instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for Galois multiplication tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for Galois multiplication tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of Galois multiplication tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_task_galois_mul_set_conf(struct doca_ec *ec,
					      doca_ec_task_galois_mul_completion_cb_t successful_task_completion_cb,
					      doca_ec_task_galois_mul_completion_cb_t error_task_completion_cb,
					      uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a Galois multiplication task.
 *
 * @param [in] ec
 * The EC instance to allocate the task for.
 * @param [in] coding_matrix
 * A coding matrix as it was create by doca_ec_matrix_create() or doca_ec_matrix_create_from_raw().
 * @param [in] src_buf
 * A source buffer with data -
 * A sequence containing data blocks - block_1, block_2 ,...
 * @param [in] dst_buf
 * Destination data buffer.
 * On successful completion of this task, will hold a sequence containing all multiplication outcome blocks -
 * dst_block_1, dst_block_2 ,...
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized Galois multiplication task.
 *
 * @note The data length of src_buf and the minimal available memory in dst_buf should be in multiplication of block
 * size.
 * For example, for a given coding matrix that is 10x4 (10 original blocks, 4 redundancy blocks) -
 * src_buf data length should be: 10x64KB = 640KB.
 * dst_buf available memory should be at least: 4x64KB = 256KB.
 *
 * They also should be aligned to 64B and with a minimum size of 64B.
 * For example: 500B size should be padded to be 512B.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_task_galois_mul_allocate_init(struct doca_ec *ec,
						   const struct doca_ec_matrix *coding_matrix,
						   const struct doca_buf *src_buf,
						   struct doca_buf *dst_buf,
						   union doca_data user_data,
						   struct doca_ec_task_galois_mul **task);

/**
 * @brief This method converts an EC Galois multiplication task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_ec_task_galois_mul_as_task(struct doca_ec_task_galois_mul *task);

/**
 * @brief This method sets the coding_matrix of a Galois multiplication task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] coding_matrix
 * A coding matrix as it was create by doca_ec_matrix_create() or doca_ec_matrix_create_from_raw().
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_galois_mul_set_coding_matrix(struct doca_ec_task_galois_mul *task,
					       const struct doca_ec_matrix *coding_matrix);

/**
 * @brief This method gets the coding matrix of a Galois multiplication task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's coding_matrix.
 */
DOCA_EXPERIMENTAL
const struct doca_ec_matrix *doca_ec_task_galois_mul_get_coding_matrix(const struct doca_ec_task_galois_mul *task);

/**
 * @brief This method sets the source buffer of a Galois multiplication task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] src_buf
 * A source buffer with data -
 * A sequence containing all original data blocks - block_1, block_2 ,...
 *
 * @note The data length of src_buf should be in multiplication of block size.
 * For example, for a given coding matrix that is 10x4 (10 original blocks, 4 redundancy blocks) -
 * src_buf data length should be: 10x64KB = 640KB.
 *
 * The data length should also be aligned to 64B and with a minimum size of 64B.
 * For example: 500B size should be padded to be 512B.
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_galois_mul_set_src_buf(struct doca_ec_task_galois_mul *task, const struct doca_buf *src_buf);

/**
 * @brief This method gets the source buffer of a Galois multiplication task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's src_buf.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_ec_task_galois_mul_get_src_buf(const struct doca_ec_task_galois_mul *task);

/**
 * @brief This method sets the destination buffer of a Galois multiplication task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] dst_buf
 * Destination data buffer.
 * On successful completion of this task, will hold a sequence containing all multiplication outcome blocks -
 * dst_block_1, dst_block_2 ,...
 *
 * @note The minimal available memory in dst_buf should be in multiplication of block * size.
 * For example, for a given coding matrix that is 10x4 (10 original blocks, 4 redundancy blocks) -
 * dst_buf available memory should be at least: 4x64KB = 256KB.
 *
 * The minimal available memory should also be aligned to 64B and at least 64B.
 * For example: 500B size should be padded to be 512B at the least.
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_galois_mul_set_dst_buf(struct doca_ec_task_galois_mul *task, struct doca_buf *dst_buf);

/**
 * @brief This method gets the destination buffer of a Galois multiplication task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's dst_buf.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_ec_task_galois_mul_get_dst_buf(const struct doca_ec_task_galois_mul *task);

/*****************************************
 * DOCA EC TASK - Create		 *
 *****************************************/
/**
 * @brief This is an encoding task, creating redundancy blocks (backup blocks) to given data.
 */
struct doca_ec_task_create;

/**
 * @brief Function to execute on completion of a create task.
 *
 * @details This function is called by doca_pe_progress() when a create task is successfully identified as completed.
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
 * The completed create task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_ec_task_create_completion_cb_t)(struct doca_ec_task_create *task,
						    union doca_data task_user_data,
						    union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a create task.
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
doca_error_t doca_ec_cap_task_create_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the create tasks configuration.
 *
 * @param [in] ec
 * The EC instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for create tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for create tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of create tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_task_create_set_conf(struct doca_ec *ec,
					  doca_ec_task_create_completion_cb_t successful_task_completion_cb,
					  doca_ec_task_create_completion_cb_t error_task_completion_cb,
					  uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a create task.
 *
 * @param [in] ec
 * The EC instance to allocate the task for.
 * @param [in] coding_matrix
 * A coding matrix as it was create by doca_ec_matrix_create() or doca_ec_matrix_create_from_raw().
 * @param [in] original_data_blocks
 * A source buffer with data -
 * A sequence containing all original data blocks - block_1, block_2 ,...
 * @param [in] rdnc_blocks
 * A destination buffer for the redundancy blocks.
 * On successful completion of this task, will hold a sequence containing all redundancy blocks -
 * rdnc_block_1, rdnc_block_2 ,...
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized create task.
 *
 * @note The data length of original_data_blocks and the minimal available memory in rdnc_blocks should be in
 * multiplication of block size.
 * For example, for a given coding matrix that is 10x4 (10 original blocks, 4 redundancy blocks) -
 * original_data_blocks data length should be: 10x64KB = 640KB.
 * rdnc_blocks available memory should be at least: 4x64KB = 256KB.
 *
 * They also should be aligned to 64B and with a minimum size of 64B.
 * For example: 500B size should be padded to be 512B.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_task_create_allocate_init(struct doca_ec *ec,
					       const struct doca_ec_matrix *coding_matrix,
					       const struct doca_buf *original_data_blocks,
					       struct doca_buf *rdnc_blocks,
					       union doca_data user_data,
					       struct doca_ec_task_create **task);

/**
 * @brief This method converts an EC create task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_ec_task_create_as_task(struct doca_ec_task_create *task);

/**
 * @brief This method sets the coding_matrix of a create task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] coding_matrix
 * A coding matrix as it was create by doca_ec_matrix_create() or doca_ec_matrix_create_from_raw().
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_create_set_coding_matrix(struct doca_ec_task_create *task,
					   const struct doca_ec_matrix *coding_matrix);

/**
 * @brief This method gets the coding matrix of a create task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's coding_matrix.
 */
DOCA_EXPERIMENTAL
const struct doca_ec_matrix *doca_ec_task_create_get_coding_matrix(const struct doca_ec_task_create *task);

/**
 * @brief This method sets the original_data_blocks buffer of a create task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] original_data_blocks
 * A source buffer with data -
 * A sequence containing all original data blocks - block_1, block_2 ,...
 *
 * @note The data length of original_data_blocks should be in multiplication of block size.
 * For example, for a given coding matrix that is 10x4 (10 original blocks, 4 redundancy blocks) -
 * original_data_blocks data length should be: 10x64KB = 640KB.
 *
 * The data length should also be aligned to 64B and with a minimum size of 64B.
 * For example: 500B size should be padded to be 512B.
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_create_set_original_data_blocks(struct doca_ec_task_create *task,
						  const struct doca_buf *original_data_blocks);

/**
 * @brief This method gets the original_data_blocks buffer of a create task.
 * The original_data_blocks buffer is a source buffer with data -
 * A sequence containing all original data blocks - block_1, block_2 ,...
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's original_data_blocks buffer.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_ec_task_create_get_original_data_blocks(const struct doca_ec_task_create *task);

/**
 * @brief This method sets the rdnc_blocks buffer of a create task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] rdnc_blocks
 * A destination buffer for the redundancy blocks.
 * On successful completion of this task, will hold a sequence containing all redundancy blocks -
 * rdnc_block_1, rdnc_block_2 ,...
 *
 * @note The minimal available memory in rdnc_blocks should be in multiplication of block size.
 * For example, for a given coding matrix that is 10x4 (10 original blocks, 4 redundancy blocks) -
 * rdnc_blocks available memory should be at least: 4x64KB = 256KB.
 *
 * The minimal available memory should also be aligned to 64B and at least 64B.
 * For example: 500B size should be padded to be 512B at the least.
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_create_set_rdnc_blocks(struct doca_ec_task_create *task, struct doca_buf *rdnc_blocks);

/**
 * @brief This method gets the rdnc_blocks buffer of a create task.
 * The rdnc_blocks buffer is a destination buffer for the redundancy blocks.
 * On successful completion of this task, will hold a sequence containing all redundancy blocks -
 * rdnc_block_1, rdnc_block_2 ,...
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's rdnc_blocks buffer.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_ec_task_create_get_rdnc_blocks(const struct doca_ec_task_create *task);

/*****************************************
 * DOCA EC TASK - Update		 *
 *****************************************/
/**
 * @brief Update redundancy blocks.
 * @note This task is useful when only a few data blocks were updated and the redundancy blocks should be updated
 * 	 accordingly.
 */
struct doca_ec_task_update;

/**
 * @brief Function to execute on completion of an update task.
 *
 * @details This function is called by doca_pe_progress() when an update task is successfully identified as completed.
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
 * The completed update task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_ec_task_update_completion_cb_t)(struct doca_ec_task_update *task,
						    union doca_data task_user_data,
						    union doca_data ctx_user_data);

/**
 * Check if a given device supports executing an update task.
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
doca_error_t doca_ec_cap_task_update_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the update tasks configuration.
 *
 * @param [in] ec
 * The EC instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for update tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for update tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of update tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_task_update_set_conf(struct doca_ec *ec,
					  doca_ec_task_update_completion_cb_t successful_task_completion_cb,
					  doca_ec_task_update_completion_cb_t error_task_completion_cb,
					  uint32_t num_tasks);

/**
 * @brief This method allocates and initializes an update task.
 *
 * @param [in] ec
 * The EC instance to allocate the task for.
 * @param [in] update_matrix
 * The update coding matrix as it was created by doca_ec_matrix_create_update() or doca_ec_matrix_create_from_raw().
 * @param [in] original_updated_and_rdnc_blocks
 * A source buffer with data -
 * A sequence containing the original data block and it's updated data block, for each block that was updated,
 * followed by the old redundancy blocks -
 * old_data_block_i, updated_data_block_i, old_data_block_j, updated_data_block_j ,... ,rdnc_block_1, rdnc_block_2 ,...
 * @param [in] updated_rdnc_blocks
 * A destination buffer for the updated redundancy blocks.
 * On successful completion of this task, will hold a sequence containing all updated redundancy blocks -
 * rdnc_block_1, rdnc_block_2 ,...
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized update task.
 *
 * @note The data length of original_updated_and_rdnc_blocks and the minimal available memory in updated_rdnc_blocks
 * should be in multiplication of block size.
 * For example, for a given update_matrix that has 4 redundancy blocks, in which 3 data block were updated -
 * original_updated_and_rdnc_blocks data length should be: (3+3+4=10)x64KB = 640KB.
 * (3 original data blocks and their updated data block, and 4 redundancy blocks).
 * updated_rdnc_blocks available memory should be at least: 4x64KB = 256KB.
 *
 * They also should be aligned to 64B and with a minimum size of 64B.
 * For example: 500B size should be padded to be 512B.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_task_update_allocate_init(struct doca_ec *ec,
					       const struct doca_ec_matrix *update_matrix,
					       const struct doca_buf *original_updated_and_rdnc_blocks,
					       struct doca_buf *updated_rdnc_blocks,
					       union doca_data user_data,
					       struct doca_ec_task_update **task);

/**
 * @brief This method converts an EC update task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_ec_task_update_as_task(struct doca_ec_task_update *task);

/**
 * @brief This method sets the update_matrix of an update task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] update_matrix
 * The update coding matrix as it was created by doca_ec_matrix_create_update() or doca_ec_matrix_create_from_raw().
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_update_set_update_matrix(struct doca_ec_task_update *task,
					   const struct doca_ec_matrix *update_matrix);

/**
 * @brief This method gets the update_matrix of an update task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's update_matrix.
 */
DOCA_EXPERIMENTAL
const struct doca_ec_matrix *doca_ec_task_update_get_update_matrix(const struct doca_ec_task_update *task);

/**
 * @brief This method sets the original_updated_and_rdnc_blocks buffer of an update task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] original_updated_and_rdnc_blocks
 * A source buffer with data -
 * A sequence containing the original data block and it's updated data block, for each block that was updated,
 * followed by the old redundancy blocks -
 * old_data_block_i, updated_data_block_i, old_data_block_j, updated_data_block_j ,... ,rdnc_block_1, rdnc_block_2 ,...
 *
 * @note The data length of original_updated_and_rdnc_blocks should be in multiplication of block size.
 * For example, for a given update_matrix that has 4 redundancy blocks, in which 3 data block were updated -
 * original_updated_and_rdnc_blocks data length should be: (3+3+4=10)x64KB = 640KB.
 * (3 original data blocks and their updated data block, and 4 redundancy blocks).
 *
 * The data length should also be aligned to 64B and with a minimum size of 64B.
 * For example: 500B size should be padded to be 512B.
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_update_set_original_updated_and_rdnc_blocks(struct doca_ec_task_update *task,
							      const struct doca_buf *original_updated_and_rdnc_blocks);

/**
 * @brief This method gets the original_updated_and_rdnc_blocks buffer of an update task.
 * The original_data_blocks buffer is a source buffer with data -
 * A sequence containing the original data block and it's updated data block, for each block that was updated,
 * followed by the old redundancy blocks -
 * old_data_block_i, updated_data_block_i, old_data_block_j, updated_data_block_j ,... ,rdnc_block_1, rdnc_block_2 ,...
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's original_updated_and_rdnc_blocks buffer.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_ec_task_update_get_original_updated_and_rdnc_blocks(const struct doca_ec_task_update *task);

/**
 * @brief This method sets the updated_rdnc_blocks buffer of an update task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] updated_rdnc_blocks
 * A destination buffer for the updated redundancy blocks.
 * On successful completion of this task, will hold a sequence containing all updated redundancy blocks -
 * rdnc_block_1, rdnc_block_2 ,...
 *
 * @note The minimal available memory in updated_rdnc_blocks should be in multiplication of block size.
 * For example, for a given update_matrix that has 4 redundancy blocks, in which 3 data block were updated -
 * updated_rdnc_blocks available memory should be at least: 4x64KB = 256KB.
 *
 * The minimal available memory should also be aligned to 64B and at least 64B.
 * For example: 500B size should be padded to be 512B at the least.
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_update_set_updated_rdnc_blocks(struct doca_ec_task_update *task,
						 struct doca_buf *updated_rdnc_blocks);

/**
 * @brief This method gets the updated_rdnc_blocks buffer of an update task.
 * The rdnc_blocks buffer is a destination buffer for the redundancy blocks.
 * On successful completion of this task, will hold a sequence containing all redundancy blocks -
 * rdnc_block_1, rdnc_block_2 ,...
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's updated_rdnc_blocks buffer.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_ec_task_update_get_updated_rdnc_blocks(const struct doca_ec_task_update *task);

/*****************************************
 * DOCA EC TASK - Recover		 *
 *****************************************/
/**
 * @brief This is a decoding task, recovering lost data blocks by using the remaining data blocks and redundancy blocks.
 */
struct doca_ec_task_recover;

/**
 * @brief Function to execute on completion of a recover task.
 *
 * @details This function is called by doca_pe_progress() when a recover task is successfully identified as completed.
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
 * The completed recover task.
 * @note The implementation can assume this value is not NULL.
 * @param [in] task_user_data
 * The doca_data supplied to the task by the application (during task allocation or by a setter).
 * @param [in] ctx_user_data
 * The doca_data supplied to the doca_ctx by the application (using a setter).
 */
typedef void (*doca_ec_task_recover_completion_cb_t)(struct doca_ec_task_recover *task,
						     union doca_data task_user_data,
						     union doca_data ctx_user_data);

/**
 * Check if a given device supports executing a recover task.
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
doca_error_t doca_ec_cap_task_recover_is_supported(const struct doca_devinfo *devinfo);

/**
 * @brief This method sets the recover tasks configuration.
 *
 * @param [in] ec
 * The EC instance to config.
 * @param [in] successful_task_completion_cb
 * A callback function for recover tasks that were completed successfully.
 * @param [in] error_task_completion_cb
 * A callback function for recover tasks that were completed with an error.
 * @param [in] num_tasks
 * Number of recover tasks.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_task_recover_set_conf(struct doca_ec *ec,
					   doca_ec_task_recover_completion_cb_t successful_task_completion_cb,
					   doca_ec_task_recover_completion_cb_t error_task_completion_cb,
					   uint32_t num_tasks);

/**
 * @brief This method allocates and initializes a recover task.
 *
 * @param [in] ec
 * The EC instance to allocate the task for.
 * @param [in] recover_matrix
 * The recover coding matrix as it was created by doca_ec_matrix_create_recover() or doca_ec_matrix_create_from_raw().
 * @param [in] available_blocks
 * A source buffer with data -
 * A sequence containing available data blocks and redundancy blocks -
 * data_block_a, data_block_b, data_block_c ,... ,rdnc_block_x, rdnc_block_y ,...
 * The total number of blocks given in this buffer should be equal to the original number data blocks.
 * @param [in] recovered_data_blocks
 * A destination buffer for the recovered data blocks.
 * On successful completion of this task, will hold a sequence containing all the recovered data blocks -
 * data_block_i, data_block_j ,...
 * @param [in] user_data
 * doca_data to attach to the task.
 * @param [out] task
 * On success, an allocated and initialized recover task.
 *
 * @note The data length of available_blocks and the minimal available memory in recovered_data_blocks should be in
 * multiplication of block size.
 * For example, for a given recover_matrix based on an original 10x4 coding matrix (10 original blocks, 4 redundancy
 * blocks) -
 * 10 available blocks should be given in total (for example 7 data blocks and 3 redundancy blocks) and available_blocks
 * data length should be: 10x64KB = 640KB.
 * recovered_data_blocks minimal available memory should be according to the amount of missing data blocks:
 * in this example, 3x64KB = 192KB.
 *
 * They also should be aligned to 64B and with a minimum size of 64B.
 * For example: 500B size should be padded to be 512B.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - no more tasks to allocate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_task_recover_allocate_init(struct doca_ec *ec,
						const struct doca_ec_matrix *recover_matrix,
						const struct doca_buf *available_blocks,
						struct doca_buf *recovered_data_blocks,
						union doca_data user_data,
						struct doca_ec_task_recover **task);

/**
 * @brief This method converts an EC recover task to a doca_task.
 *
 * @param [in] task
 * The task that should be converted.
 *
 * @return
 * doca_task
 */
DOCA_EXPERIMENTAL
struct doca_task *doca_ec_task_recover_as_task(struct doca_ec_task_recover *task);

/**
 * @brief This method sets the recover_matrix of a recover task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] recover_matrix
 * The recover coding matrix as it was created by doca_ec_matrix_create_recover() or doca_ec_matrix_create_from_raw().
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_recover_set_recover_matrix(struct doca_ec_task_recover *task,
					     const struct doca_ec_matrix *recover_matrix);

/**
 * @brief This method gets the recover_matrix of a recover task.
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's recover_matrix.
 */
DOCA_EXPERIMENTAL
const struct doca_ec_matrix *doca_ec_task_recover_get_recover_matrix(const struct doca_ec_task_recover *task);

/**
 * @brief This method sets the available_blocks buffer of a recover task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] available_blocks
 * A source buffer with data -
 * A sequence containing available data blocks and redundancy blocks -
 * data_block_a, data_block_b, data_block_c ,... ,rdnc_block_x, rdnc_block_y ,...
 * The total number of blocks given in this buffer should be equal to the original number data blocks.
 *
 *  @note The data length of available_blocks should be in multiplication of block size.
 * For example, for a given recover_matrix based on an original 10x4 coding matrix (10 original blocks, 4 redundancy
 * blocks) -
 * 10 available blocks should be given in total (i data blocks and j redundancy blocks) and available_blocks data length
 * should be: 10x64KB = 640KB.
 *
 * The data length should also be aligned to 64B and with a minimum size of 64B.
 * For example: 500B size should be padded to be 512B.
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_recover_set_available_blocks(struct doca_ec_task_recover *task,
					       const struct doca_buf *available_blocks);

/**
 * @brief This method gets the available_blocks buffer of a recover task.
 * The available_blocks buffer is a source buffer with data -
 * A sequence containing all available data blocks and redundancy blocks -
 * data_block_a, data_block_b, data_block_c ,... ,rdnc_block_x, rdnc_block_y ,...
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's available_blocks buffer.
 */
DOCA_EXPERIMENTAL
const struct doca_buf *doca_ec_task_recover_get_available_blocks(const struct doca_ec_task_recover *task);

/**
 * @brief This method sets the recovered_data_blocks buffer of a recover task.
 *
 * @param [in] task
 * The task to set.
 * @param [in] recovered_data_blocks
 * A destination buffer for the recovered data blocks.
 * On successful completion of this task, will hold a sequence containing all the recovered data blocks -
 * data_block_i, data_block_j ,...
 *
 * @note The minimal available memory in recovered_data_blocks should be in multiplication of block size.
 * For example, for a given recover_matrix based on an original 10x4 coding matrix (10 original blocks, 4 redundancy
 * blocks), in which 3 data blocks need to be recovered -
 * recovered_data_blocks minimal available memory should be according to the amount of missing data blocks:
 * in this example, 3x64KB = 192KB.
 *
 * The minimal available memory should also be aligned to 64B and at least 64B.
 * For example: 500B size should be padded to be 512B at the least.
 *
 */
DOCA_EXPERIMENTAL
void doca_ec_task_recover_set_recovered_data_blocks(struct doca_ec_task_recover *task,
						    struct doca_buf *recovered_data_blocks);

/**
 * @brief This method gets the recovered_data buffer of a recover task.
 * The recovered_data buffer is a destination buffer for the recovered data blocks.
 * On successful completion of this task, will hold a sequence containing all the recovered data blocks -
 * data_block_i, data_block_j ,...
 *
 * @param [in] task
 * The task that should be queried.
 *
 * @return
 * The task's recovered_data buffer.
 */
DOCA_EXPERIMENTAL
struct doca_buf *doca_ec_task_recover_get_recovered_data(const struct doca_ec_task_recover *task);

/*********************************************************************************************************************
 * DOCA EC Context
 *********************************************************************************************************************/

/**
 * @brief Create a DOCA EC instance.
 *
 * @param [in] dev
 * The device to attach to the EC instance.
 * @param [out] ec
 * Pointer to pointer to be set to point to the created doca_ec instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - ec argument is a NULL pointer.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate sufficient memory for doca_ec.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_create(struct doca_dev *dev, struct doca_ec **ec);

/**
 * @brief Destroy a DOCA EC instance.
 *
 * @param [in] ec
 * Pointer to instance to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_BAD_STATE - EC context is not idle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_destroy(struct doca_ec *ec);

/**
 * @brief Convert EC instance into context.
 *
 * @param [in] ec
 * EC instance.
 * This must remain valid until after the context is no longer required.
 *
 * @return
 * On success doca_ctx object, otherwise NULL.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_ec_as_ctx(struct doca_ec *ec);

/**
 * @brief Get maximum block size supported for DOCA EC tasks.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 * @param [out] max_block_size
 * The max block size for DOCA EC operation in bytes.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - failed to query device capabilities
 *				or provided devinfo does not support EC.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_cap_get_max_block_size(const struct doca_devinfo *devinfo, uint64_t *max_block_size);

/**
 * @brief Get the maximum supported number of elements in DOCA linked-list buffer used for EC tasks.
 *
 * @param [in] devinfo
 * The DOCA device information that should be queried.
 * @param [out] max_buf_list_len
 * The maximum supported number of elements in DOCA linked-list buffer.
 * The value 1 indicates that only a single element is supported.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_cap_get_max_buf_list_len(const struct doca_devinfo *devinfo, uint32_t *max_buf_list_len);

/*********************************************************************************************************************
 * DOCA EC Matrix Gen
 *********************************************************************************************************************/

/**
 * @brief Types of coding matrix used for erasure codes.
 */
enum doca_ec_matrix_type {
	/**
	 * Cauchy matrix of coding.
	 *
	 * Cauchy matrix guarantees any sub-matrix will be invertible.
	 * The matrix is constructed in the following form:
	 * a_i_j = 1/(x_i + y_j)
	 * where 0 <= i < number of data blocks,
	 * 	     0 <= j < number of redundancy blocks,
	 *	     x_i = i,
	 *       y_j = j + number of data blocks
	 *
	 * This is the recommended type to use.
	 */
	DOCA_EC_MATRIX_TYPE_CAUCHY = 1,
	/**
	 * Vandermonde matrix of coding.
	 *
	 * Vandermonde matrix does not guarantee every sub-matrix will be invertible.
	 * The matrix is constructed in the following form:
	 * a_i_j = (i + 1)^j
	 * where 0 <= i < number of data blocks,
	 *       0 <= j < number of redundancy blocks
	 *
	 * Because this matrix does not guarantee invertible it is less recommended to use.
	 */
	DOCA_EC_MATRIX_TYPE_VANDERMONDE = 2,
};

/**
 * @brief Generate coding matrix for Erasure Code encode i.e. most basic encode matrix.
 * This is necessary for executing create tasks.
 *
 * @param [in] ec
 * EC instance.
 * @param [in] type
 * Provided in enum doca_ec_matrix_type, the type will be consistent in recovery/update process.
 * @param [in] data_block_count
 * The number of original data blocks.
 * @param [in] rdnc_block_count
 * The number of redundancy blocks.
 * @param [out] matrix
 * The coding matrix.
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_matrix_create(struct doca_ec *ec,
				   enum doca_ec_matrix_type type,
				   size_t data_block_count,
				   size_t rdnc_block_count,
				   struct doca_ec_matrix **matrix);

/**
 * @brief Create coding matrix for Erasure Code encode i.e. most basic encode matrix from char array.
 * This is substitute for doca_ec_matrix_create which converts a char array to an DOCA EC matrix that can be used to
 * execute different EC tasks.
 *
 * @param [in] ec
 * EC instance.
 * @param [in] data
 * The data of a coding matrix with size data_block_count * rdnc_block_count.
 * @param [in] data_block_count
 * The number of original data blocks.
 * @param [in] rdnc_block_count
 * The number of redundancy blocks.
 * @param [out] matrix
 * The coding matrix.
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_matrix_create_from_raw(struct doca_ec *ec,
					    uint8_t *data,
					    size_t data_block_count,
					    size_t rdnc_block_count,
					    struct doca_ec_matrix **matrix);

/**
 * @brief Generate coding matrix for Erasure Code update.
 * To use this function must call doca_ec_matrix_create() or doca_ec_matrix_create_from_raw() before.
 *
 * @param [in] ec
 * EC instance.
 * @param [in] coding_matrix
 * A coding matrix as it was created by doca_ec_matrix_create() or doca_ec_matrix_create_from_raw().
 * @param [in] update_indices
 * An array specifying the indices of the updated data blocks.
 * @note The indices need to be in ascending order.
 * @note The indices should match the order of the data blocks in the matrix creation function.
 * @param [in] n_updates
 * update_indices count.
 * @param [out] matrix
 * The update coding matrix.
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_matrix_create_update(struct doca_ec *ec,
					  const struct doca_ec_matrix *coding_matrix,
					  uint32_t update_indices[],
					  size_t n_updates,
					  struct doca_ec_matrix **matrix);
/**
 * @brief Generate coding matrix for Erasure Code recovery from failure.
 * To use this function must call doca_ec_matrix_create() or doca_ec_matrix_create_from_raw() before.
 *
 * @param [in] ec
 * EC instance.
 * @param [in] coding_matrix
 * A coding matrix as it was created by doca_ec_matrix_create() or doca_ec_matrix_create_from_raw().
 * @param [in] missing_indices
 * An array specifying the indices of the missing data blocks.
 * @note The indices need to be in ascending order.
 * @note The indices should match the order of the data blocks in the matrix creation function.
 * @param [in] n_missing
 * missing_indices count.
 * @param [out] matrix
 * The recover coding matrix.
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_matrix_create_recover(struct doca_ec *ec,
					   const struct doca_ec_matrix *coding_matrix,
					   uint32_t missing_indices[],
					   size_t n_missing,
					   struct doca_ec_matrix **matrix);

/**
 * @brief Destroy coding matrix.
 *
 * @param [in] matrix
 * A coding matrix as it was created by doca_ec_matrix_create() or doca_ec_matrix_create_from_raw().
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_ec_matrix_destroy(struct doca_ec_matrix *matrix);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_EC_H_ */

/** @} */
