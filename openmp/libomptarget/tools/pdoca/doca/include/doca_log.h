/*
 * Copyright (c) 2021-2023 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_log.h
 * @page logger
 * @defgroup LOGGER Logging Management
 *
 * Define functions for internal and external logging management
 *
 * To add DOCA trace level compile with "-D DOCA_LOGGING_ALLOW_TRACE"
 *
 * @{
 */

#ifndef DOCA_LOG_H_
#define DOCA_LOG_H_

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include <doca_compat.h>
#include <doca_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief log levels, sorted by verbosity level from high to low
 */
enum doca_log_level {
	DOCA_LOG_LEVEL_DISABLE = 10, /**< Disable log messages */
	DOCA_LOG_LEVEL_CRIT = 20,    /**< Critical log level */
	DOCA_LOG_LEVEL_ERROR = 30,   /**< Error log level */
	DOCA_LOG_LEVEL_WARNING = 40, /**< Warning log level */
	DOCA_LOG_LEVEL_INFO = 50,    /**< Info log level */
	DOCA_LOG_LEVEL_DEBUG = 60,   /**< Debug log level */
	DOCA_LOG_LEVEL_TRACE = 70,   /**< Trace log level */
};

/**
 * @brief logging backend for application or SDK messages
 */
struct doca_log_backend;

/**
 * @brief logging backend flush() handler
 */
typedef void (*log_flush_callback)(char *buf);

/**
 * @brief Set the lower log level of a specific logging backend for application messages.
 *
 * Dynamically change the lower log level of the given logging backend, any application message with
 * verbosity level equal or above this level will be shown.
 *
 * @param[in] backend
 * Logging backend to update.
 * @param[in] level
 * Log level enum DOCA_LOG_LEVEL.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_set_level_lower_limit(struct doca_log_backend *backend, uint32_t level);

/**
 * @brief Set the upper log level limit of a specific logging backend for application messages.
 *
 * Dynamically change the upper log level limit of the given logging backend, any application message with
 * verbosity level above this level will not be shown.
 *
 * @param[in] backend
 * Logging backend to update.
 * @param[in] upper_limit
 * Log level enum DOCA_LOG_LEVEL.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_set_level_upper_limit(struct doca_log_backend *backend, uint32_t upper_limit);

/**
 * @brief Mark the lower log level limit of a specific logging backend for application messages as strict.
 *
 * Mark the lower log level limit of a specific logging backend for application messages as strict,
 * preventing it from being lowered by any future log level changes, both global and direct.
 *
 * @param[in] backend
 * Logging backend to update.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_set_level_lower_limit_strict(struct doca_log_backend *backend);

/**
 * @brief Mark the upper log level limit of a specific application logging backend for application messages as strict.
 *
 * Mark the upper log level limit of a specific logging backend for application messages as strict,
 * preventing it from being raised by any future log level changes, both global and direct.
 *
 * @param[in] backend
 * Logging backend to update.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_set_level_upper_limit_strict(struct doca_log_backend *backend);

/**
 * @brief Set the log level of ALL logging backends for application messages.
 *
 * Dynamically change the log level of ALL the logging backends for application messages,
 * any application message with verbosity level equal or above this level will be shown.
 * Newly created logging backends for application messages will use this as their default lower log level limit.
 *
 *
 * Default value of the global lower level limit is DOCA_LOG_LEVEL_INFO.
 *
 * @param[in] level
 * Log level enum DOCA_LOG_LEVEL.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_level_set_global_lower_limit(uint32_t level);

/**
 * @brief Set the log upper level limit of ALL logging backends for application messages.
 *
 * Dynamically change the log upper level limit of ALL the application logging backends,
 * any application message with verbosity level above this level will not be shown.
 * Newly created logging backends for application messages will use this as their default upper log level limit.
 *
 * Default value of the global upper level limit is DOCA_LOG_LEVEL_CRIT.
 *
 * @param[in] upper_limit
 * Log level enum DOCA_LOG_LEVEL.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_level_set_global_upper_limit(uint32_t upper_limit);

/**
 * @brief Get the global log level for application messages.
 *
 * Dynamically query for the global lower log level, any application message with verbosity level equal or above this
 * level will be shown.
 * The global lower level is used as the initial value when a new logging backend for application messages is created.
 *
 * @return
 * Log level enum DOCA_LOG_LEVEL.
 */
DOCA_EXPERIMENTAL
uint32_t doca_log_level_get_global_lower_limit(void);

/**
 * @brief Get the global upper log level for application messages.
 *
 * Dynamically query for the global upper log level, any application message with verbosity level above this level will
 * not be shown. The global upper level is used as the initial value when a new logging backend for application messages
 * is created.
 *
 * @return
 * Log level enum DOCA_LOG_LEVEL.
 */
DOCA_EXPERIMENTAL
uint32_t doca_log_level_get_global_upper_limit(void);

/**
 * @brief Register a log source.
 *
 * Will return the identifier associated with the log source. Log source is used to describe the logging
 * module of the messages in that source file.
 *
 * @note Recommended to only be used via DOCA_LOG_REGISTER.
 *
 * @param[in] source_name
 * The string identifying the log source. Should be in an hierarchic form (i.e. DPI::Parser).
 * @param[out] source
 * Source identifier that was allocated to this log source name (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_register_source(const char *source_name, int *source);

/**
 * @brief Unregister a log source.
 *
 * Unregisters a given log source as part of the teardown process of the running program.
 *
 * @note Used automatically via DOCA_LOG_REGISTER, not recommended to call it directly.
 *
 * @param[in] source
 * The source identifier of source to be unregistered, as allocated by doca_log_register_source.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_unregister_source(int source);

/**
 * @brief Create a logging backend for application messages with a FILE* stream.
 *
 * Creates a new logging backend for application messages.
 *
 * @param[in] fptr
 * The FILE * for the logging backend stream.
 * @param[out] backend
 * Logging backend that wraps the given fptr (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_with_file(FILE *fptr, struct doca_log_backend **backend);

/**
 * @brief Create a logging backend for application messages with an fd stream.
 *
 * Creates a new logging backend for application messages.
 *
 * @param[in] fd
 * The file descriptor (int) for the logging backend.
 * @param[out] backend
 * Logging backend that wraps the given fd (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_with_fd(int fd, struct doca_log_backend **backend);

/**
 * @brief Create a logging backend for application messages with a char buffer stream.
 *
 * Creates a new logging backend for application messages.
 * The logging backend will write each log record at the beginning of this buffer and call the handler.
 *
 * @param[in] buf
 * The char buffer (char *) for the logging backend stream.
 * @param[in] capacity
 * Maximal amount of chars that could be written to the stream.
 * @param[in] handler
 * Handler to be called when the log record should be flushed from the stream.
 * @param[out] backend
 * Logging backend that wraps the given buffer (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_with_buf(char *buf,
					      size_t capacity,
					      log_flush_callback handler,
					      struct doca_log_backend **backend);

/**
 * @brief Create a logging backend for application messages with a syslog output.
 *
 * Creates a new logging backend for application messages.
 *
 * @param[in] name
 * The syslog name for the logging backend.
 * @param[out] backend
 * Logging backend that exposes the desired syslog functionality (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_with_syslog(const char *name, struct doca_log_backend **backend);

/**
 * @brief Set the log level limit for SDK logging backends.
 *
 * Dynamically change the log level limit of the given SDK logging backend, any log under this
 * level will not be shown.
 * @param[in] backend
 * SDK logging backend to update.
 * @param[in] level
 * Log level enum DOCA_LOG_LEVEL.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_set_sdk_level(struct doca_log_backend *backend, uint32_t level);

/**
 * @brief Create a logging backend with a FILE* stream for SDK messages.
 *
 * Creates a new logging backend.
 *
 * @param[in] fptr
 * The FILE * for the logging backend stream.
 * @param[out] backend
 * Logging backend that wraps the given fptr (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_with_file_sdk(FILE *fptr, struct doca_log_backend **backend);

/**
 * @brief Create a logging backend with an fd stream for SDK messages.
 *
 * Creates a new logging backend.
 *
 * @param[in] fd
 * The file descriptor (int) for the logging backend.
 * @param[out] backend
 * Logging backend that wraps the given fd (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_with_fd_sdk(int fd, struct doca_log_backend **backend);

/**
 * @brief Create a logging backend with a char buffer stream for SDK messages.
 *
 * Creates a new logging backend. The logging backend will write each log record at the
 * beginning of this buffer.
 *
 * @param[in] buf
 * The char buffer (char *) for the logging backend stream.
 * @param[in] capacity
 * Maximal amount of chars that could be written to the stream.
 * @param[in] handler
 * Handler to be called when the log record should be flushed from the stream.
 * @param[out] backend
 * Logging backend that wraps the given buffer (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_with_buf_sdk(char *buf,
						  size_t capacity,
						  log_flush_callback handler,
						  struct doca_log_backend **backend);

/**
 * @brief Create a logging backend with a syslog output for SDK messages.
 *
 * Creates a new logging backend.
 *
 * @param[in] name
 * The syslog name for the logging backend.
 * @param[out] backend
 * Logging backend that exposes the desired syslog functionality (only valid if no error occurred).
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_with_syslog_sdk(const char *name, struct doca_log_backend **backend);

/**
 * @brief Set the log level of ALL logging backends for SDK messages.
 *
 * Dynamically change the log level of ALL the logging backends for SDK messages,
 * any SDK message with verbosity level equal or above this level will be shown.
 * Newly created logging backends for SDK messages will use this as their default log level limit.
 *
 * Default value of the level limit is DOCA_LOG_LEVEL_INFO.
 *
 * @param[in] level
 * Log level enum DOCA_LOG_LEVEL.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_level_set_global_sdk_limit(uint32_t level);

/**
 * @brief Get the global log level for SDK messages.
 *
 * Dynamically query for the global log level, any SDK message with verbosity level equal or above this
 * level will be shown.
 * The global lower level is used as the initial value when a new logging backend for SDK messages is created.
 *
 * @return
 * Log level enum DOCA_LOG_LEVEL.
 */
DOCA_EXPERIMENTAL
uint32_t doca_log_level_get_global_sdk_limit(void);

/**
 * @brief Create default, non configurable backend for application messages.
 *
 * Creates a set of 2 backends for application messages:
 * stdout shall print the range from global lower level up to DOCA_LOG_LEVEL_INFO
 * stderr shall print the range from DOCA_LOG_LEVEL_WARNING up to global upper level
 *
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log_backend_create_standard(void);

/**
 * @brief Generates an application log message.
 *
 * This should not be used, please prefer using DOCA_LOG.
 *
 * @param[in] level
 * Log level enum DOCA_LOG_LEVEL.
 * @param[in] source
 * The log source identifier defined by doca_log_register_source.
 * @param[in] fname
 * The file name this log originated from.
 * @param[in] line
 * The line number this log originated from.
 * @param[in] func
 * The function name this log originated from.
 * @param[in] format
 * printf(3) arguments, format and variables.
 * @return
 * DOCA error code.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_log(uint32_t level, int source, const char *fname, int line, const char *func, const char *format, ...)
	__attribute__((format(printf, 6, 7)));

/**
 * @brief Generates an application log message.
 *
 * The DOCA_LOG() is the main log function for logging. This call affects the performance.
 * Consider using the specific level DOCA_LOG for better code readability (i.e. DOCA_LOG_ERR).
 *
 * @param level
 * Log level enum DOCA_LOG_LEVEL (just ERROR, WARNING...).
 * @param format
 * printf(3) arguments, format and variables.
 */
#define DOCA_LOG(level, format, ...) doca_log(level, log_source, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

/**
 * @brief Generates a CRITICAL application log message.
 *
 * Will generate critical application log. This call affects the performance.
 *
 * @param format
 * printf(3) arguments, format and variables.
 */
#define DOCA_LOG_CRIT(format, ...) DOCA_LOG(DOCA_LOG_LEVEL_CRIT, format, ##__VA_ARGS__)

/**
 * @brief Generates an ERROR application log message.
 *
 * Will generate error application log. This call affects the performance.
 *
 * @param format
 * printf(3) arguments, format and variables.
 */
#define DOCA_LOG_ERR(format, ...) DOCA_LOG(DOCA_LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

/**
 * @brief Generates a WARNING application log message.
 *
 * Will generate warning application log. This call affects the performance.
 *
 * @param format
 * printf(3) arguments, format and variables.
 */
#define DOCA_LOG_WARN(format, ...) DOCA_LOG(DOCA_LOG_LEVEL_WARNING, format, ##__VA_ARGS__)

/**
 * @brief Generates an INFO application log message.
 *
 * Will generate info application log. This call affects the performance.
 *
 * @param format
 * printf(3) arguments, format and variables.
 */
#define DOCA_LOG_INFO(format, ...) DOCA_LOG(DOCA_LOG_LEVEL_INFO, format, ##__VA_ARGS__)

/**
 * @brief Generates a DEBUG application log message.
 *
 * Will generate debug application log. This call affects the performance.
 *
 * @param format
 * printf(3) arguments, format and variables.
 */
#define DOCA_LOG_DBG(format, ...) DOCA_LOG(DOCA_LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)

/**
 * @brief Generates a TRACE application log message.
 *
 * To show the logs define DOCA_LOGGING_ALLOW_TRACE in the compilation variables.
 * This will not effect performance if compiled without DOCA_LOGGING_ALLOW_TRACE, as
 * it will be removed by the compiler.
 *
 * Will generate trace application log. This call affects the performance.
 *
 * @param format
 * printf(3) arguments, format and variables.
 */
#ifdef DOCA_LOGGING_ALLOW_TRACE
#define DOCA_LOG_TRC(format, ...) DOCA_LOG(DOCA_LOG_LEVEL_TRACE, format, ##__VA_ARGS__)
#else /* DOCA_LOGGING_ALLOW_TRACE */
#define DOCA_LOG_TRC(format, ...) \
	do { \
	} while (0)
#endif /* DOCA_LOGGING_ALLOW_TRACE */

/**
 * @brief Registers log source on program start.
 *
 * Should be used to register the log source.
 * For example:
 *
 * DOCA_LOG_REGISTER(dpi)
 *
 * void foo {
 *       DOCA_LOG_INFO("Message");
 * }
 *
 * @note The macro also takes care of the dtor() logic on teardown.
 *
 * @param source
 * A string representing the source name.
 */

#ifdef __linux__

#define DOCA_LOG_REGISTER(source) \
	static int log_source; \
	/* Use the highest priority so other Ctors will be able to use the log */ \
	static void __attribute__((constructor(101), used)) DOCA_LOG_CTOR_##__FILE__(void) \
	{ \
		doca_log_register_source(#source, &log_source); \
	} \
	/* Keep it symmetric */ \
	static void __attribute__((destructor(101), used)) DOCA_LOG_DTOR_##__FILE__(void) \
	{ \
		doca_log_unregister_source(log_source); \
	}

#else /* implicit windows */

#ifdef __cplusplus

class doca_log_registrator {
public:
	doca_log_registrator(const char *source_name, int &log_source) noexcept
	{
		doca_log_register_source(source_name, &log_source);
		m_log_source = log_source;
	}
	~doca_log_registrator()
	{
		doca_log_unregister_source(m_log_source);
	}

private:
	int m_log_source{0};
};

#define DOCA_LOG_REGISTER(source) \
	static int log_source{0}; \
	static doca_log_registrator g_register_struct(#source, log_source)

#else /* __cplusplus */

/**
 * MSVC CRT Initialization, used for C based Windows applications.
 * Notice that this way of registration does not invoke the doca_log_unregister_source at program termination (will not
 * lead to memory leak, doca_log_unregister_source is used to guarantee that new log is not registered during
 * destruction) .CRT$XCU holds pointers to initializers. This is why the macro below contains a pointer to a static
 * function in the data segment.
 * \#pragma section(".CRT$XCU", read) guarantees that doca_log_register_source will be called before any compiler
 * generated C++ dynamic initializer
 */
#pragma section(".CRT$XCU", read)
#define DOCA_LOG_REGISTER(source) \
	static int log_source = 0; \
	static void _log_ctor_func(void); \
	__pragma(data_seg(".CRT$XCU")) static void (*__doca_log_initializer)() = _log_ctor_func; \
	__pragma(data_seg()) static void _log_ctor_func(void) \
	{ \
		doca_log_register_source(#source, &log_source); \
	}

#endif /* __cplusplus */

#endif /* __linux__ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_LOG_H_ */
