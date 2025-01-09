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
 * @file doca_version.h
 * @page version
 * @defgroup VERSION Version Management
 *
 * Define functions to get the DOCA version, and compare against it.
 *
 * @{
 */

#ifndef DOCA_VERSION_H_
#define DOCA_VERSION_H_

#include <stddef.h>

#include <doca_compat.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Major version number 0-255.
 */
#define DOCA_VERSION_MAJOR 2
/**
 * @brief Minor version number 0-255.
 */
#define DOCA_VERSION_MINOR 7
/**
 * @brief Patch version number 0-9999.
 */
#define DOCA_VERSION_PATCH 85
/**
 * @brief DOCA Version String.
 */
#define DOCA_VERSION_STRING "2.7.0085"

/**
 * @brief Macro to calculate exact version number for comparisons.
 */
#define DOCA_VERSION_CALC_EXACT(major, minor, patch) ((size_t)((major) << 24 | (minor) << 16 | (patch)))

/**
 * @brief Macro of current exact version number.
 */
#define DOCA_VERSION_EXACT DOCA_VERSION_CALC_EXACT(DOCA_VERSION_MAJOR, DOCA_VERSION_MINOR, DOCA_VERSION_PATCH)

/**
 * @brief Macro to calculate official version number for comparisons.
 */
#define DOCA_VERSION_CALC(major, minor, patch) ((size_t)((major) << 24 | (minor) << 16 | (patch * 1000)))

/**
 * @brief Macro of current official version number.
 */
#define DOCA_VERSION_OFFICIAL DOCA_VERSION_CALC(DOCA_VERSION_MAJOR, DOCA_VERSION_MINOR, DOCA_VERSION_PATCH)

/**
 * @brief Check if the specified version is equal to the current official version.
 */
#define DOCA_VERSION_EQ_CURRENT(major, minor, patch) (DOCA_VERSION_CALC(major, minor, patch) == DOCA_VERSION_OFFICIAL)

/**
 * @brief Check if the specified version is less then or equal to current official version.
 */
#define DOCA_VERSION_LTE_CURRENT(major, minor, patch) (DOCA_VERSION_CALC(major, minor, patch) <= DOCA_VERSION_OFFICIAL)

/**
 * @brief Function returning DOCA's (SDK) exact version string.
 *
 * @return
 * version string, using the format major.minor.patch.
 *
 * @note Represents the SDK version a project was compiled with.
 */
static inline const char *doca_version(void)
{
	return DOCA_VERSION_STRING;
}

/**
 * @brief Function returning DOCA's (runtime) exact version string.
 *
 * @return
 * version string, using the format major.minor.patch.
 *
 * @note Represents the runtime version a project is linked against.
 */
DOCA_EXPERIMENTAL
const char *doca_version_runtime(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_VERSION_H_ */
