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
 * @defgroup DPA_DEVICE DPA Device
 * DOCA DPA Device library. For more details please refer to the user guide on DOCA devzone.
 *
 * @ingroup DPA
 *
 * @{
 */

#ifndef DOCA_DPA_DEV_H_
#define DOCA_DPA_DEV_H_

/**
 * @brief declares that we are compiling for the DPA Device
 *
 * @note Must be defined before the first API use/include of DOCA
 */
#define DOCA_DPA_DEVICE

#include <stdint.h>
#include <stddef.h>
/** Include to define compatibility with current version, define experimental Symbols */
#include <doca_compat.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DOCA DPA device log levels, sorted by verbosity from high to low
 */
__dpa_global__ typedef enum doca_dpa_dev_log_level {
	DOCA_DPA_DEV_LOG_LEVEL_DISABLE = 10, /**< Disable log messages */
	DOCA_DPA_DEV_LOG_LEVEL_CRIT = 20,    /**< Critical log level */
	DOCA_DPA_DEV_LOG_LEVEL_ERROR = 30,   /**< Error log level */
	DOCA_DPA_DEV_LOG_LEVEL_WARNING = 40, /**< Warning log level */
	DOCA_DPA_DEV_LOG_LEVEL_INFO = 50,    /**< Info log level */
	DOCA_DPA_DEV_LOG_LEVEL_DEBUG = 60,   /**< Debug log level */
} doca_dpa_dev_log_level_t;

/**
 * @brief DPA pointer type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_uintptr_t;

/**
 * @brief DPA hash table handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_hash_table_t;

/**
 * @brief DPA completion handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_completion_t;

/**
 * @brief DPA completion element handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_completion_element_t;

/**
 * @brief DPA asynchronous ops handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_async_ops_t;

/**
 * @brief DPA notification completion handle type definition
 */
__dpa_global__ typedef uint64_t doca_dpa_dev_notification_completion_t;

/**
 * @brief DPA completion type
 */
__dpa_global__ typedef enum {
	DOCA_DPA_DEV_COMP_SEND = 0x0,		     /**< Send completion */
	DOCA_DPA_DEV_COMP_RECV_RDMA_WRITE_IMM = 0x1, /**< Receive RDMA Write with Immediate completion */
	DOCA_DPA_DEV_COMP_RECV_SEND = 0x2,	     /**< Receive Send completion */
	DOCA_DPA_DEV_COMP_RECV_SEND_IMM = 0x3,	     /**< Receive Send with Immediate completion */
	DOCA_DPA_DEV_COMP_SEND_ERR = 0xD,	     /**< Send Error completion */
	DOCA_DPA_DEV_COMP_RECV_ERR = 0xE	     /**< Receive Error completion */
} doca_dpa_dev_completion_type_t;

/**
 * @brief static inline wrapper
 */
#ifndef __forceinline
#define __forceinline static inline __attribute__((always_inline))
#endif /* __forceinline */

/**
 * @brief Obtains the thread rank
 *
 * Retrieves the thread rank for a given kernel on the DPA.
 * This function returns a number in {0..N-1}, where N is either:
 * 1- The number of threads requested for launch during a kernel submission when using host APIs:
 *    doca_dpa_kernel_launch_update_set/add(..., N, ...).
 * 2- The number of threads in thread group running the kernel when using host APIs:
 *    doca_dpa_thread_group_create(..., N, ...)
 *
 * @return
 * Returns the thread rank for a given kernel on the DPA.
 */
DOCA_EXPERIMENTAL
unsigned int doca_dpa_dev_thread_rank(void);

/**
 * @brief Obtains the number of threads running the kernel
 *
 * Retrieves the number of threads assigned to a given kernel. This is either the value:
 * 1- `num_threads` that was passed to host API doca_dpa_kernel_launch_update_set/add(..., num_threads, ...)
 * 2- `num_threads` that was passed to host API doca_dpa_thread_group_create(..., num_threads, ...)
 *
 * @return
 * Returns the number of threads running the kernel
 */
DOCA_EXPERIMENTAL
unsigned int doca_dpa_dev_num_threads(void);

/**
 * @brief Get DPA thread local storage
 *
 * This function returns DPA thread local storage which was set previously using
 * host API doca_dpa_thread_set_local_storage().
 *
 * @note This API is relevant only for a kernel used for DPA thread ('doca_dpa_thread'), which was set previously
 * using host API doca_dpa_thread_set_func_arg().
 * This API is not relevant for a kernel used in host kernel launch APIs.
 *
 * @return
 * This function returns DPA pointer for DPA thread local storage
 */
DOCA_EXPERIMENTAL
doca_dpa_dev_uintptr_t doca_dpa_dev_thread_get_local_storage(void);

/**
 * @brief Yield a DPA thread
 *
 * This function yields a DPA thread that is running a kernel
 *
 * @note This API is relevant only for a kernel used in host kernel launch APIs
 * doca_dpa_kernel_launch_update_set/add(...).
 * This API is not relevant for a kernel used for DPA thread ('doca_dpa_thread'), which was set previously
 * using host API doca_dpa_thread_set_func_arg().
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_yield(void);

/**
 * @brief Reschedule a DPA thread
 *
 * This function reschedules a DPA thread and releases the EU and make it available for rescheduling.
 * Any new completion arrived at the attach completion context will trigger the DPA thread again.
 *
 * @note This API is relevant only for a kernel used for DPA thread ('doca_dpa_thread'), which was set previously
 * using host API doca_dpa_thread_set_func_arg().
 * This API is not relevant for a kernel used in host kernel launch APIs.
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_thread_reschedule(void);

/**
 * @brief Finish a DPA thread
 *
 * This function marks the thread not to be rescheduled.
 * Any new completion arrived at the attach completion context will not trigger the DPA thread again.
 *
 * @note This API is relevant only for a kernel used for DPA thread ('doca_dpa_thread'), which was set previously
 * using host API doca_dpa_thread_set_func_arg().
 * This API is not relevant for a kernel used in host kernel launch APIs.
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_thread_finish(void);

/**
 * @brief Notify the completion handle and trigger the attached thread
 *
 * This function notifies the given completion context which leads to triggering the attached DPA thread.
 * Please note that using this API will leads to triggering the attached thread without receiving a completion
 * on the attached completion context, hence can't read completion info using doca_dpa_dev_get_completion*() APIs
 *
 * @note This API is relevant only for a kernel used for DPA thread ('doca_dpa_thread'), which was set previously
 * using host API doca_dpa_thread_set_func_arg().
 * This API is not relevant for a kernel used in host kernel launch APIs.
 *
 * @param[in] comp_handle - DPA notification completion handle
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_thread_notify(doca_dpa_dev_notification_completion_t comp_handle);

/**
 * @brief Print logs to Host
 *
 * This function prints from device to host's standard output stream or the user defined file
 * set by doca_dpa_log_file_set_path().
 * The log level will determine the print according to the verbosity set by doca_dpa_set_log_level().
 * It is recommended to use the bellow defined MACROs for device logging for better readability.
 * Multiple threads may call these MACROs simultaneously. Printing is a convenience service, and due to limited
 * buffering on the host, not all print statements may appear on the host.
 *
 * @param[in] log_level - level for device log
 * @param[in] format - format string that contains the text to be written to host (same as from regular printf)
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_log(doca_dpa_dev_log_level_t log_level, const char *format, ...)
	__attribute__((format(printf, 2, 3)));

/**
 * @brief Generate a DOCA DPA device CRITICAL log message
 *
 * Will generate critical application log.
 *
 * @note This call affects the performance.
 *
 */
#define DOCA_DPA_DEV_LOG_CRIT(...) doca_dpa_dev_log(DOCA_DPA_DEV_LOG_LEVEL_CRIT, __VA_ARGS__)

/**
 * @brief Generate a DOCA DPA device ERROR log message
 *
 * @note This call affects the performance.
 *
 */
#define DOCA_DPA_DEV_LOG_ERR(...) doca_dpa_dev_log(DOCA_DPA_DEV_LOG_LEVEL_ERROR, __VA_ARGS__)

/**
 * @brief Generate a DOCA DPA device WARNING log message
 *
 * @note This call affects the performance.
 *
 */
#define DOCA_DPA_DEV_LOG_WARN(...) doca_dpa_dev_log(DOCA_DPA_DEV_LOG_LEVEL_WARNING, __VA_ARGS__)

/**
 * @brief Generate a DOCA DPA device INFO log message
 *
 * @note This call affects the performance.
 *
 */
#define DOCA_DPA_DEV_LOG_INFO(...) doca_dpa_dev_log(DOCA_DPA_DEV_LOG_LEVEL_INFO, __VA_ARGS__)

/**
 * @brief Generate a DOCA DPA device DEBUG log message
 *
 * @note This call affects the performance.
 *
 */
#define DOCA_DPA_DEV_LOG_DBG(...) doca_dpa_dev_log(DOCA_DPA_DEV_LOG_LEVEL_DEBUG, __VA_ARGS__)

/**
 * @brief Creates trace message entry with arguments
 *
 * This function prints traces arguments from device to host's standard output stream,
 * or to the user's defined outfile set by doca_dpa_trace_file_set_path().
 *
 * @note It is recommended to use trace for enhanced performance in logging
 *
 * @param[in] arg1 - argument #1 to format into the template
 * @param[in] arg2 - argument #2 to format into the template
 * @param[in] arg3 - argument #3 to format into the template
 * @param[in] arg4 - argument #4 to format into the template
 * @param[in] arg5 - argument #5 to format into the template
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_trace(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);

/**
 * @brief Flush the trace message buffer to Host
 *
 * As soon as a buffer is fully occupied it is internally sent to host, however
 * user can ask partially occupied buffer to be sent to host.
 * Its intended use is at end of run to flush whatever messages left
 *
 * @note: Frequent call to this API might cause performance issues.
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_trace_flush(void);

/**
 * @brief Map a specific key to the specified value in the hash table
 *
 * @note Adding a new key when the hash table is full will lead to unexpected behaviour.
 *
 * @param[in] ht_handle - DPA hash table handle
 * @param[in] key - New key to add to hash table
 * @param[in] value - New key's value to add to hash table
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_hash_table_add(doca_dpa_dev_hash_table_t ht_handle, uint32_t key, uint64_t value);

/**
 * @brief Remove the key and its corresponding value from the hash table
 *
 * @param[in] ht_handle - DPA hash table handle
 * @param[in] key - Key to remove from hash table
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_hash_table_remove(doca_dpa_dev_hash_table_t ht_handle, uint32_t key);

/**
 * @brief Returns the value to which the specified key is mapped in the hash table
 *
 * @param[in] ht_handle - DPA hash table handle
 * @param[in] key - Key to find in hash table
 * @param[out] value - Key's value
 *
 * @return
 * This function returns 1 if the key exists in the hash table. Otherwise returns 0
 */
DOCA_EXPERIMENTAL
int doca_dpa_dev_hash_table_find(doca_dpa_dev_hash_table_t ht_handle, uint32_t key, uint64_t *value);

/**
 * @brief Get DPA completion element
 *
 * This function returns a completion element which has arrived at the completion context.
 * If a DPA thread is attached to this completion context, the thread will be triggered on this completion element.
 *
 * @param[in] dpa_comp_handle - DPA completion handle
 * @param[out] comp_element - DPA completion element
 *
 * @return
 * This function returns 1 if there is completion element to return. Otherwise returns 0
 */
DOCA_EXPERIMENTAL
int doca_dpa_dev_get_completion(doca_dpa_dev_completion_t dpa_comp_handle,
				doca_dpa_dev_completion_element_t *comp_element);

/**
 * @brief Get completion element type
 *
 * @param[in] comp_element - DPA completion element
 *
 * @return
 * This function returns type completion element
 */
DOCA_EXPERIMENTAL
doca_dpa_dev_completion_type_t doca_dpa_dev_get_completion_type(doca_dpa_dev_completion_element_t comp_element);

/**
 * @brief Get completion element user data
 *
 * This function returns user data which was set previously in either host APIs:
 * 1- doca_dpa_async_ops_create(..., user_data, ...)
 *    When completion context is attached to DPA asynchronous ops context.
 * 2- doca_ctx_set_user_data(..., user_data)
 *    When completion context is attached to DOCA context, such as DOCA RDMA context.
 *
 * @param[in] comp_element - DPA completion element
 *
 * @return
 * This function returns user data of completion element
 */
DOCA_EXPERIMENTAL
uint32_t doca_dpa_dev_get_completion_user_data(doca_dpa_dev_completion_element_t comp_element);

/**
 * @brief Get completion element immediate data
 *
 * This function returns immediate data for a completion of type:
 * 1- DOCA_DPA_DEV_COMP_RECV_RDMA_WRITE_IMM
 * 2- DOCA_DPA_DEV_COMP_RECV_SEND_IMM
 *
 * @note This API is not relevant for completions of other types.
 *
 * @param[in] comp_element - DPA RDMA completion element
 *
 * @return
 * This function returns immediate of completion element
 */
DOCA_EXPERIMENTAL
uint32_t doca_dpa_dev_get_completion_immediate(doca_dpa_dev_completion_element_t comp_element);

/**
 * @brief Acknowledge that the completions have been read on the completion context
 *
 * This function releases resources of the acked completion elements in the completion context.
 * This acknowledgment enables receiving new `num_comp` completions
 *
 * @param[in] dpa_comp_handle - DPA completion handle
 * @param[in] num_comp - Number of completion elements which have been read
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_completion_ack(doca_dpa_dev_completion_t dpa_comp_handle, uint64_t num_comp);

/**
 * @brief Request notification on the DPA completion
 *
 * This function enables requesting new notifications on the DPA completion.
 * Without calling this function, DPA completion context will not be notified on new arrived completion elements
 * hence new completions will not be populated in DPA completion context.
 *
 * @param[in] dpa_comp_handle - DPA completion handle
 *
 * @return
 * This function does not return any value
 */
DOCA_EXPERIMENTAL
void doca_dpa_dev_completion_request_notification(doca_dpa_dev_completion_t dpa_comp_handle);

#ifdef __cplusplus
}
#endif

#endif /* DOCA_DPA_DEV_H_ */

/** @} */
