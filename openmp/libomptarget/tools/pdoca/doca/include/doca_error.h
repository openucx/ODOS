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
 * @file doca_error.h
 * @page doca error
 * @defgroup ERROR DOCA Error
 * @ingroup DOCACore
 * DOCA Error provides information regarding different errors caused while using the DOCA libraries.
 *
 * @{
 */

#ifndef DOCA_ERROR_H_
#define DOCA_ERROR_H_

#include <doca_compat.h>
#include <doca_common_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DOCA API return codes
 */
typedef enum doca_error {
	DOCA_SUCCESS = 0,		       /**< Success */
	DOCA_ERROR_UNKNOWN = 1,		       /**< Unknown error */
	DOCA_ERROR_NOT_PERMITTED = 2,	       /**< Operation not permitted */
	DOCA_ERROR_IN_USE = 3,		       /**< Resource already in use */
	DOCA_ERROR_NOT_SUPPORTED = 4,	       /**< Operation not supported */
	DOCA_ERROR_AGAIN = 5,		       /**< Resource temporarily unavailable, try again */
	DOCA_ERROR_INVALID_VALUE = 6,	       /**< Invalid input */
	DOCA_ERROR_NO_MEMORY = 7,	       /**< Memory allocation failure */
	DOCA_ERROR_INITIALIZATION = 8,	       /**< Resource initialization failure */
	DOCA_ERROR_TIME_OUT = 9,	       /**< Timer expired waiting for resource */
	DOCA_ERROR_SHUTDOWN = 10,	       /**< Shut down in process or completed */
	DOCA_ERROR_CONNECTION_RESET = 11,      /**< Connection reset by peer */
	DOCA_ERROR_CONNECTION_ABORTED = 12,    /**< Connection aborted */
	DOCA_ERROR_CONNECTION_INPROGRESS = 13, /**< Connection in progress */
	DOCA_ERROR_NOT_CONNECTED = 14,	       /**< Not Connected */
	DOCA_ERROR_NO_LOCK = 15,	       /**< Unable to acquire required lock */
	DOCA_ERROR_NOT_FOUND = 16,	       /**< Resource Not Found */
	DOCA_ERROR_IO_FAILED = 17,	       /**< Input/Output Operation Failed */
	DOCA_ERROR_BAD_STATE = 18,	       /**< Bad State */
	DOCA_ERROR_UNSUPPORTED_VERSION = 19,   /**< Unsupported version */
	DOCA_ERROR_OPERATING_SYSTEM = 20,      /**< Operating system call failure */
	DOCA_ERROR_DRIVER = 21,		       /**< DOCA Driver call failure */
	DOCA_ERROR_UNEXPECTED = 22,	       /**< An unexpected scenario was detected */
	DOCA_ERROR_ALREADY_EXIST = 23,	       /**< Resource already exist */
	DOCA_ERROR_FULL = 24,		       /**< No more space in resource */
	DOCA_ERROR_EMPTY = 25,		       /**< No entry is available in resource */
	DOCA_ERROR_IN_PROGRESS = 26,	       /**< Operation is in progress */
	DOCA_ERROR_TOO_BIG = 27,	       /**< Requested operation too big to be contained */
} doca_error_t;

/**
 * \brief Compiler optimized macro to check if we have an error.
 *
 * Used in cases where error is unlikely to happen.
 */
#define DOCA_IS_ERROR(r) doca_unlikely((r) != DOCA_SUCCESS)

/**
 * \brief Save the first encountered doca_error_t.
 *
 * Updates the return value variable r to hold the first error that we encountered.
 */
#define DOCA_ERROR_PROPAGATE(r, t) \
	do { \
		if (r == DOCA_SUCCESS) \
			r = t; \
	} while (0)

/**
 *
 * \brief Returns the string representation of an error code name.
 *
 * Returns a string containing the name of an error code in the enum.
 * If the error code is not recognized, "unrecognized error code" is returned.
 *
 * @param[in] error - Error code to convert to string.
 *
 * @return char* pointer to a NULL-terminated string.
 */
DOCA_STABLE
const char *doca_error_get_name(doca_error_t error);

/**
 * \brief Returns the description string of an error code.
 *
 *  This function returns the description string of an error code.
 *  If the error code is not recognized, "unrecognized error code" is returned.
 *
 * @param[in] error - Error code to convert to description string.
 *
 * @return char* pointer to a NULL-terminated string.
 */
DOCA_STABLE
const char *doca_error_get_descr(doca_error_t error);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_ERROR_H_ */
