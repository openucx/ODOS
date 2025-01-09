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
 * @file doca_compat.h
 * @page compat
 * @defgroup COMPAT Compatibility Management
 *
 * Lib to define compatibility with current version, define experimental Symbols.
 *
 * To set a Symbol (or specifically a function) as experimental:
 *
 * DOCA_EXPERIMENTAL
 * int func_declare(int param1, int param2);
 *
 * To remove warnings of experimental compile with "-D DOCA_ALLOW_EXPERIMENTAL_API"
 * To remove warnings of deprecated compile with "-D DOCA_ALLOW_DEPRECATED_API"
 *
 * @{
 */

#ifndef DOCA_COMPAT_H_
#define DOCA_COMPAT_H_

#include <doca_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__) || defined(DOCA_DPA_DEVICE)

#define DOCA_USED __attribute__((used))

#define DOCA_STABLE __attribute__((visibility("default"))) DOCA_USED

#ifndef DOCA_ALLOW_EXPERIMENTAL_API

/**
 * @brief To set a Symbol (or specifically a function) as experimental.
 */
#define DOCA_EXPERIMENTAL \
	__attribute__((deprecated("Symbol is defined as experimental"), section(".text.experimental"))) DOCA_STABLE

#else /* DOCA_ALLOW_EXPERIMENTAL_API */

#define DOCA_EXPERIMENTAL __attribute__((section(".text.experimental"))) DOCA_STABLE

#endif /* DOCA_ALLOW_EXPERIMENTAL_API */

#ifndef DOCA_ALLOW_DEPRECATED_API

/**
 * @brief To set a Symbol (or specifically a function) as deprecated.
 */
#define DOCA_DEPRECATED \
	__attribute__((deprecated("Symbol is defined as deprecated"), section(".text.deprecated"))) DOCA_STABLE

#else /* DOCA_ALLOW_DEPRECATED_API */

#define DOCA_DEPRECATED __attribute__((section(".text.deprecated"))) DOCA_STABLE

#endif /* DOCA_ALLOW_DEPRECATED_API */

#else /* __linux__ */

/**
 * @brief To allow compiling functions and structs that are using GCC attributes
 * using __attribute__() in compilers that don't support them.
 */
#define __attribute__(_x_)

/**
 * @brief To hint the compiler that the function should be exposed to the DLL or imported from it,
 * according to the availability of DOCA_EXPORTS. By default, it should be imported.
 */
#ifdef DOCA_EXPORTS
#define DLL_EXPORT_ATTR dllexport
#else
#define DLL_EXPORT_ATTR dllimport
#endif

/**
 * @brief To set a Symbol (or specifically a function) as stable API, i.e. it won't be changed.
 */
#define DOCA_STABLE __declspec(DLL_EXPORT_ATTR)

#ifndef DOCA_ALLOW_EXPERIMENTAL_API

/**
 * @brief To set a Symbol (or specifically a function) as experimental.
 */
#define DOCA_EXPERIMENTAL __declspec(deprecated("Symbol is defined as experimental"), DLL_EXPORT_ATTR)

#else /* DOCA_ALLOW_EXPERIMENTAL_API */

#define DOCA_EXPERIMENTAL __declspec(DLL_EXPORT_ATTR)

#endif /* DOCA_ALLOW_EXPERIMENTAL_API */

#ifndef DOCA_ALLOW_DEPRECATED_API

/**
 * @brief To set a Symbol (or specifically a function) as deprecated.
 */
#define DOCA_DEPRECATED __declspec(deprecated("Symbol is defined as deprecated"), DLL_EXPORT_ATTR)

#else /* DOCA_ALLOW_DEPRECATED_API */

#define DOCA_DEPRECATED __declspec(DLL_EXPORT_ATTR)

#endif /* DOCA_ALLOW_DEPRECATED_API */

#endif /* __linux__ */

/***************************/
/** Compatibility Helpers **/
/***************************/

#ifdef DOCA_COMPAT_HELPERS

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <doca_version.h>

#define DOCA_STRUCT_START size_t __doca_api_version
#define DOCA_STRUCT_GET_VERSION(_X_) (_X_)->__doca_api_version
#define DOCA_STRUCT_CTOR(_X_) (_X_).__doca_api_version = DOCA_VERSION_EXACT
#define DOCA_STRUCT_PTR_CTOR(_X_) DOCA_STRUCT_CTOR(*(_X_))
#define DOCA_STRUCT_MEMSET_CTOR(_X_) \
	do { \
		memset(&(_X_), 0, sizeof(_X_)); \
		(_X_).__doca_api_version = DOCA_VERSION_EXACT; \
	} while (0)
#define DOCA_STRUCT_PTR_MEMSET_CTOR(_X_) DOCA_STRUCT_MEMSET_CTOR(*(_X_))
#define DOCA_STRUCT_CTOR_LIST_START .__doca_api_version = DOCA_VERSION_EXACT

#endif /* DOCA_COMPAT_HELPERS */

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_COMPAT_H_ */
