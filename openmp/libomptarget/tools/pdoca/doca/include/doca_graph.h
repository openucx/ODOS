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
 * @file doca_graph.h
 * @page doca doca_graph
 * @defgroup DOCACore Core
 * @defgroup GRAPH DOCA Graph
 * @ingroup DOCACore
 *
 * DOCA graph facilitates submitting an ordered set of tasks and user callbacks.
 * A graph can contain nodes of the following types:
 * - Context node: A node that points to a context and contains a doca_task for that context.
 * -- A graph must contain at least one context node.
 * - User node: A node that points to a callback supplied by the user and contains a user defined doca_task.
 * - Graph node: A node that points to a graph instance and facilitates building a graph of graphs.
 *
 * Graph Instance
 * A graph creates a graph instance (or more)
 * Every node in the graph instance is set with corresponding data (task, callback, etc. depending on the type of the
 * node).
 * Node data can be set during runtime, but it is not recommended. Application should instead change the task content.
 *
 * Usage:
 * - Create a graph by adding nodes and setting dependencies.
 * -- Cyclic graph is not permitted.
 * - Create graph instance (or more).
 * - Set nodes data to every graph instance.
 * - Submit graph instances
 * - Call progress one when applicable.
 *
 * Notes
 * - Any node failure shall fail the graph progress. However, the graph progress shall complete only when all in flight
 *   nodes are completed (new nodes shall not be submitted).
 * - A graph instance shall not fail if a context is overloaded (it will continue running once the context is free).
 *
 * @code
 * Graph example (diamond graph):
 *                         +-------------+
 *                         |    Node A   |
 *                         +-------------+
 *                                |
 *                +---------------+---------------+
 *                |                               |
 *        +-------------+                  +-------------+
 *        |    Node B   |                  |    Node C   |
 *        +-------------+                  +-------------+
 *                |                               |
 *                +---------------+---------------+
 *                                |
 *                         +-------------+
 *                         |    Node D   |
 *                         +-------------+
 * @endcode
 *
 * Graph implementation example:
 * This example builds a graph with 2 nodes, creates an instance and submits it to a progress engine.
 * node1 -> node2
 * The example is focused on the graph API. It does not include progress engine, contexts creation etc. or error
 *handling.
 *
 * Create the graph and connect it to a progress engine.
 *	struct doca_graph *my_graph;
 *	doca_graph_create(pe, &my_graph);
 *	doca_graph_set_conf(my_graph, graph_completion_cb, graph_error_cb, log_num_instances);
 *
 * Create the nodes
 *	struct doca_graph_node *node1, node2;
 *	doca_graph_node_create_from_ctx(my_graph, ctx1, &node1);
 *	doca_graph_node_create_from_ctx(my_graph, ctx2, &node2);
 *
 * Set dependency (node1 -> node2)
 *	doca_graph_add_dependency(my_graph, node1, node2);
 *
 * Start the graph
 *	doca_graph_start(my_graph);
 *
 * Create a graph instance and set nodes data
 *	struct doca_graph_instance *my_graph_instance
 *	doca_graph_instance_create(my_graph, &my_graph_instance);
 *	doca_graph_instance_set_ctx_node_data(my_graph_instance, node1, &node_1_task);
 *	doca_graph_instance_set_ctx_node_data(my_graph_instance, node2, &node_2_task);
 *
 * Submit the graph instance to the progress engine
 *	doca_graph_instance_submit(my_graph_instance);
 *
 * Call progress one to tick the progress engine until graph is completed (graph instance completed callback will be
 * invoked).
 *	doca_pe_progress(pe);
 *
 * Resubmit instance
 *	Set tasks parameters if required.
 *	doca_graph_instance_submit(my_graph_instance);
 * @{
 */

#ifndef DOCA_GRAPH_H_
#define DOCA_GRAPH_H_

#include <stdint.h>
#include <stddef.h>

#include <doca_compat.h>
#include <doca_error.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct doca_ctx;
struct doca_pe;
struct doca_task;

struct doca_graph;
struct doca_graph_node;
struct doca_graph_instance;

/**
 * @brief Graph completion callback
 *
 * @param [in] instance
 * Graph instance that was completed.
 * @param [in] instance_user_data
 * Graph instance user data
 * @param [in] graph_user_data
 * Graph user data
 */
typedef void (*doca_graph_completion_cb_t)(struct doca_graph_instance *instance,
					   union doca_data instance_user_data,
					   union doca_data graph_user_data);

/**
 * @brief Creates a DOCA graph
 *
 * This method creates an empty doca_graph.
 *
 * @param [in] pe
 * Progress engine to bind the graph to
 * @param [out] graph
 * The created graph.
 * The application is expected to destroy the graph when it is no longer needed (@see doca_graph_destroy)
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate the graph.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_create(struct doca_pe *pe, struct doca_graph **graph);

/**
 * @brief Destroys a previously created doca_graph
 *
 * A DOCA graph can be destroyed only if it was stopped
 *
 * @param [in] graph
 * The graph to destroy
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - Graph is not stopped
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_destroy(struct doca_graph *graph);

/**
 * @brief Set graph configuration
 *
 * @param [in] graph
 * DOCA graph to config
 * @param [in] graph_completion_cb
 * Graph completion callback. Invoked when a graph instance is completed successfully.
 * @param [in] graph_error_cb
 * Graph error callback. Invoked when a graph instance fails.
 * @param [in] num_instances
 * Number of the instances that the graph can allocate.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - Graph is not stopped
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_set_conf(struct doca_graph *graph,
				 doca_graph_completion_cb_t graph_completion_cb,
				 doca_graph_completion_cb_t graph_error_cb,
				 uint32_t num_instances);

/**
 * @brief Set user data to the graph
 *
 * @param [in] graph
 * The graph to set the user data to
 * @param [in] user_data
 * doca_data to attach to the graph instance
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - graph is started
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_set_user_data(struct doca_graph *graph, union doca_data user_data);

/**
 * @brief Set user data to the graph
 *
 * @param [in] graph
 * The graph to set the user data to
 * @param [out] user_data
 * user data to get
 *
 * @return
 * DOCA_SUCCESS
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_get_user_data(const struct doca_graph *graph, union doca_data *user_data);

/**
 * @brief Create a context node
 *
 * This method creates a context node (A node that points to a context and contains a doca_task for the context)
 * A node is automatically added to the graph as a root when it is created
 * A node can only be added before the graph is started.
 *
 * @param [in] graph
 * The graph to add the node to.
 * @param [in] ctx
 * Context to run the task.
 * @param [out] node
 * Reference to the created graph node.
 * The node shall be used to set dependencies and set node data.
 * A node does not need to be destroyed by the application.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - Graph is already started
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate the node
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_node_create_from_ctx(struct doca_graph *graph,
					     const struct doca_ctx *ctx,
					     struct doca_graph_node **node);

/**
 * @brief User node callback
 *
 * Definition of a user node callback. @see doca_graph_node_create_from_user for more details
 *
 * @param [in] cookie
 * A cookie set to the node (@see doca_graph_instance_set_user_node_data).
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - Any doca_error_t (depends on the callback implementation)
 */
typedef doca_error_t (*doca_graph_user_node_cb_t)(void *cookie);

/**
 * @brief Create a user node
 *
 * This method creates a user node (A node that points to a callback supplied by the user and contains a user
 * defined doca_task.)
 * A node is automatically added to the graph as a root when it is created
 * A node can only be added before the graph is started.
 *
 * @param [in] graph
 * The graph to add the node to.
 * @param [in] cb
 * Callback to be called when the node is executed
 * @param [out] node
 * Reference to the created graph node.
 * The node shall be used to set dependencies and set node data.
 * A node does not need to be destroyed by the application.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - Graph is already started
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate the node
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_node_create_from_user(struct doca_graph *graph,
					      doca_graph_user_node_cb_t cb,
					      struct doca_graph_node **node);

/**
 * @brief Create a sub graph node
 *
 * This method creates a sub graph node (a node that points to a doca_graph).
 * A node is automatically added to the graph as a root when it is created
 * A node can only be added before the graph is started.
 * Sub graph must not form a circle with the graph that it is added to (e.g. Graph A -> Graph B -> Graph A)
 *
 * @param [in] graph
 * The graph to add the node to.
 * @param [in] sub_graph
 * Graph to be executed as a sub graph.
 * @param [out] node
 * Reference to the created graph node.
 * The node shall be used to set dependencies and set node data.
 * A node does not need to be destroyed by the application.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - Graph is already started or sub graph is not started.
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate the node
 * - DOCA_ERROR_NOT_PERMITTED - Sub graph forms a circle (e.g. pointing to the graph or forming a circle with one of the
 *   nodes).
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_node_create_from_graph(struct doca_graph *graph,
					       struct doca_graph *sub_graph,
					       struct doca_graph_node **node);

/**
 * @brief Set dependencies
 *
 * This method adds a dependent node to a node.
 * Node dependency can only be set before the graph is started.
 * Setting dependency must not form a circle in the graph
 *
 * @param [in] graph
 * The graph that both from node and to node reside in.
 * @param [in] from
 * Node to depend on
 * @param [in] to
 * Node that depends on the from node
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - Graph is already started.
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate dependency.
 * - DOCA_ERROR_NOT_PERMITTED - Dependency forms a circle.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_add_dependency(struct doca_graph *graph,
				       struct doca_graph_node *from,
				       struct doca_graph_node *to);

/**
 * @brief Start a graph
 *
 * This method starts a graph.
 * A doca_graph can only be used after it was started (@see details and pseudo code example at the top of the header
 * file).
 * A doca_graph can only be started if all contexts (in the context nodes) were started.
 *
 * @param [in] graph
 * Graph to start
 *
 * @return
 * DOCA_SUCCESS - in case of success or if the graph is already started.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - Graph does not contain a context node (graph must contain at least one context node) or
 *			    graph is already started.
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate run graph time data.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_start(struct doca_graph *graph);

/**
 * @brief Stop a graph
 *
 * This method stops a graph. A graph can be stopped only after all the instances created by it were destroyed.
 *
 * @param [in] graph
 * Graph to stop
 *
 * @return
 * DOCA_SUCCESS - in case of success or if the graph is already stopped.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_IN_USE - graph instances are not destroyed.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_stop(struct doca_graph *graph);

/**
 * @brief Create a graph instance
 *
 * This method creates a graph instance.
 * Graph instance contains the nodes data (tasks, callbacks, sub graphs, etc.) and is submitted to a progress engine to
 * be executed. A graph must be started before it can create an instance.
 *
 * @param [in] graph
 * Graph to create the instance from.
 * @param [out] graph_instance
 * Instance created by the graph.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - Graph is not started.
 * - DOCA_ERROR_NO_MEMORY - Failed to allocate memory for the graph instance.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_instance_create(const struct doca_graph *graph, struct doca_graph_instance **graph_instance);

/**
 * @brief Destroy graph instance
 *
 * This method destroys a graph instance
 * A graph instance can not be destroyed if it is submitted or if it is set as a sub graph node data.
 *
 * @param [in] graph_instance
 * Graph instance to destroy
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_IN_USE - graph instance is submitted.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_instance_destroy(struct doca_graph_instance *graph_instance);

/**
 * @brief Set user data to the graph instance
 *
 * @param [in] graph_instance
 * The graph instance to set the user data to
 * @param [in] user_data
 * doca_data to attach to the graph instance
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_IN_USE - graph instance is submitted.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_instance_set_user_data(struct doca_graph_instance *graph_instance, union doca_data user_data);

/**
 * @brief Set user data to the graph instance
 *
 * @param [in] graph_instance
 * The graph instance to set the user data to
 * @param [out] user_data
 * user data to get
 *
 * @return
 * DOCA_SUCCESS
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_instance_get_user_data(const struct doca_graph_instance *graph_instance,
					       union doca_data *user_data);

/**
 * @brief Set context node data
 *
 * This method sets context node data (task).
 * It is recommended to set the node data once and change the task content (if required) every instance run.
 *
 * @param [in] graph_instance
 * Graph instance to set the node data to
 * @param [in] node
 * Graph node that facilitates setting the data to the correct node in the instance.
 * - Node must belong to the graph that created the instance
 * - Node must be a context node.
 * created the instance.
 * @param [in] task
 * doca_task to set to the node. The task context must match the context of the graph node.
 * task lifespan must be >= to the lifespan of the graph instance.
 * Task callbacks (completed & error) are not invoked when used in a graph.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - instance is submitted.
 * - DOCA_ERROR_NOT_PERMITTED - node does not belong to the graph that created the instance, task type mismatch,
 *				invalid context, etc.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_instance_set_ctx_node_data(struct doca_graph_instance *graph_instance,
						   struct doca_graph_node *node,
						   struct doca_task *task);

/**
 * @brief Set user node data
 *
 * This method sets user node data
 * It is recommended to set the node data once and change the task content (if required) every instance run.
 *
 * @param [in] graph_instance
 * Graph instance to set the node data to
 * @param [in] node
 * Graph node that facilitates setting the data to the correct node in the instance.
 * - Node must belong to the graph that created the instance
 * - Node must be a user node.
 * @param [in] cookie
 * cookie supplied by the application (passed to the callback when it is executes).
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - instance is submitted.
 * - DOCA_ERROR_NOT_PERMITTED - node does not belong to the graph that created the instance
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_instance_set_user_node_data(struct doca_graph_instance *graph_instance,
						    struct doca_graph_node *node,
						    void *cookie);

/**
 * @brief Set sub graph node data
 *
 * This method sets sub graph node data
 * It is recommended to set the node data once and change the task content (if required) every instance run.
 *
 * @param [in] graph_instance
 * Graph instance to set the node data to
 * @param [in] node
 * Graph node that facilitates setting the data to the correct node in the instance.
 * - Node must belong to the graph that created the instance
 * - Node must be a sub graph node.
 * @param [in] sub_graph_instance
 * Graph instance to be run by the node.
 * -- Instance must be created by the graph that the sub graph node was created with.
 * -- Instance must not be submitted.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_BAD_STATE - instance is submitted.
 * - DOCA_ERROR_NOT_PERMITTED - node does not belong to the graph that created the instance, sub graph instance is
 *   submitted, etc.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_instance_set_sub_graph_node_data(struct doca_graph_instance *graph_instance,
							 struct doca_graph_node *node,
							 struct doca_graph_instance *sub_graph_instance);

/**
 * @brief Submit graph instance to a progress engine
 *
 * This method submits a graph instance to a progress engine
 * Graph submission executes the graph root nodes.
 * A submitted graph can't be aborted or flushed.
 *
 * @param [in] graph_instance
 * The graph instance to submit
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - invalid input received.
 * - DOCA_ERROR_IN_USE - The graph instance is already submitted
 * - other doca_error_t statuses may be popped up from root tasks submission.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_graph_instance_submit(struct doca_graph_instance *graph_instance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_GRAPH_H_ */
