/*
 * Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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

#ifndef DOCA_TUNE_SERVER_H_
#define DOCA_TUNE_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <doca_flow.h>

enum doca_flow_tune_server_kpi_type {
	TUNE_SERVER_KPI_TYPE_NR_PORTS,
	/**< Number of ports */
	TUNE_SERVER_KPI_TYPE_NR_QUEUES,
	/**< Number of queues */
	TUNE_SERVER_KPI_TYPE_QUEUE_DEPTH,
	/**< Queue depth */
	TUNE_SERVER_KPI_TYPE_NR_SHARED_RESOURCES,
	/**< Number of shared resources */
	TUNE_SERVER_KPI_TYPE_NR_PIPES,
	/**< Number of pipes */
	TUNE_SERVER_KPI_TYPE_ENTRIES_OPS_ADD,
	/**< Queue add entries operations aggregated number */
	TUNE_SERVER_KPI_TYPE_ENTRIES_OPS_UPDATE,
	/**< Queue update entries operations aggregated number */
	TUNE_SERVER_KPI_TYPE_ENTRIES_OPS_REMOVE,
	/**< Queue remove entries operations aggregated number */
	TUNE_SERVER_KPI_TYPE_PENDING_OPS,
	/**< Queue pending operations number */
	TUNE_SERVER_KPI_TYPE_NO_WAIT_OPS,
	/**< Queue operations (add/update/remove) with NO_WAIT flag */
};

/**
 * @brief DOCA Flow Tune Server shared resources
 */
struct doca_flow_tune_server_shared_resources_kpi_res {
	uint64_t nr_meter;
	/**< Total number of shared meters */
	uint64_t nr_counter;
	/**< Total number of shared counters */
	uint64_t nr_rss;
	/**< Total number of shared rss */
	uint64_t nr_mirror;
	/**< Total number of shared mirrors */
	uint64_t nr_psp;
	/**< Total number of shared psp */
	uint64_t nr_encap;
	/**< Total number of shared encap */
	uint64_t nr_decap;
	/**< Total number of shared decap */
};

/**
 * @brief DOCA Flow Tune Server KPI result
 */
struct doca_flow_tune_server_kpi_res {
	enum doca_flow_tune_server_kpi_type type;
	/**< KPI result type */
	union {
		struct doca_flow_tune_server_shared_resources_kpi_res shared_resources_kpi;
		/**< Shared resources KPI result */
		struct {
			uint64_t val;
			/**< KPI result value */
		} kpi;
	};
};

/**
 * @brief Initialize a DOCA flow tune server.
 *
 * This is the global initialization function for doca flow tune server.
 * It initializes all resources used by doca flow tune server.
 *
 * Should be called after doca_flow_init().
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_NOT_SUPPORTED - functionality isn't support in this (runtime) version.
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_tune_server_init(void);

/**
 * @brief Destroy the doca flow tune server.
 *
 * Release all the resources used by doca flow tune server.
 *
 * should it be invoked before doca_flow_destroy
 *
 * Must be invoked at the end of the application, before it exits.
 */
DOCA_EXPERIMENTAL
void doca_flow_tune_server_destroy(void);

/**
 * @brief Query and dump pipes info of all ports
 *
 * Dump all pipes information.
 *
 * @param [in] fp
 * Pointer to opened file to save pipeline info
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_tune_server_query_pipe_line(FILE *fp);

/**
 * @brief Retrieve ports identification numbers.
 *
 * Retrieve Flow application's port identification numbers.
 *
 * @param [in] port_id_arr
 * Pointer to port ids array to fill.
 * @param [in] port_id_arr_len
 * Port ids array length.
 * @param [out] nr_ports
 * Numbers of port ids assigned
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - memory allocation failed.
 * - DOCA_ERROR_NOT_SUPPORTED - unsupported configuration.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_tune_server_get_port_ids(uint16_t *port_id_arr, uint16_t port_id_arr_len, uint16_t *nr_ports);

/**
 * @brief Retrieve application scope Key Performance Indicator.
 *
 * @param [in] kpi_type
 * Key Performance Indicator type to retrieve.
 * @param [out] res
 * Result struct is being filled according to KPI type.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - memory allocation failed.
 * - DOCA_ERROR_NOT_SUPPORTED - unsupported configuration.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_tune_server_get_kpi(enum doca_flow_tune_server_kpi_type kpi_type,
					   struct doca_flow_tune_server_kpi_res *res);

/**
 * @brief Retrieve Key Performance Indicator from a specific port.
 *
 * @param [in] port_id
 * KPI port id to query.
 * @param [in] kpi_type
 * Key Performance Indicator type to retrieve.
 * @param [out] res
 * Result struct is being filled according to KPI type.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - memory allocation failed.
 * - DOCA_ERROR_NOT_SUPPORTED - unsupported configuration.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_tune_server_get_port_kpi(uint16_t port_id,
						enum doca_flow_tune_server_kpi_type kpi_type,
						struct doca_flow_tune_server_kpi_res *res);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DOCA_TUNE_SERVER_H_ */
