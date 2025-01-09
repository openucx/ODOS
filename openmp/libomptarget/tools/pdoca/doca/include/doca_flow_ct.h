/*
 * Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_flow_ct.h
 * @page doca flow ct
 * @defgroup Flow ct
 * DOCA HW connection tracking library.
 *
 * @{
 */

#ifndef DOCA_FLOW_CT_H_
#define DOCA_FLOW_CT_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <doca_compat.h>
#include <doca_dev.h>
#include <doca_error.h>
#include <doca_flow.h>
#include <doca_flow_net.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Meta connection type
 */
enum doca_flow_ct_meta_type {
	DOCA_FLOW_CT_META_NONE,	  /**< Regular payload traffic */
	DOCA_FLOW_CT_META_NEW,	  /**< SYN or first UDP packet */
	DOCA_FLOW_CT_META_END,	  /**< FIN or RST packet */
	DOCA_FLOW_CT_META_UPDATE, /**< Payload to update user action data */
};

/**
 * Meta type mask
 */
#define DOCA_FLOW_CT_META_TYPE_MASK 0x3

/**
 * invalid CT action handle
 */
#define DOCA_FLOW_CT_ACTION_HANDLE_INVALID (UINT32_MAX)

/**
 * @brief CT packet meta data
 */
union doca_flow_ct_meta {
	uint32_t u32; /**< CPU endian. */
	struct {
		uint32_t src : 1;     /**< Source port in multi-port E-Switch mode */
		uint32_t hairpin : 1; /**< Subject to forward using hairpin. */
		uint32_t type : 2;    /**< Refer to doca_flow_ct_meta_type. */
		uint32_t data : 28;   /**< Combination of reserved, zone, user action data and user data. */
	};
};

/**
 * @brief CT flags
 */
enum doca_flow_ct_flags {
	DOCA_FLOW_CT_FLAG_STATS = 1u << 0,		  /**< Enable counter for internal pipes */
	DOCA_FLOW_CT_FLAG_WORKER_STATS = 1u << 1,	  /**< Enable worker counter dump */
	DOCA_FLOW_CT_FLAG_NO_AGING = 1u << 2,		  /**< Bypass aging scan */
	DOCA_FLOW_CT_FLAG_SW_PKT_PARSING = 1u << 3,	  /**< Force software packet parsing */
	DOCA_FLOW_CT_FLAG_MANAGED = 1u << 4,		  /**< User managed worker thread, API only */
	DOCA_FLOW_CT_FLAG_ASYMMETRIC_TUNNEL = 1u << 5,	  /**< Tunnel or non-tunnel in different direction */
	DOCA_FLOW_CT_FLAG_ASYMMETRIC_COUNTER = 1u << 6,	  /**< Different counter in both direction */
	DOCA_FLOW_CT_FLAG_NO_COUNTER = 1u << 7,		  /**< Disable counter support */
	DOCA_FLOW_CT_FLAG_CT_PIPE_ONLY = 1u << 8,	  /** User responsible to set the CT type */
	DOCA_FLOW_CT_FLAG_WIRE_TO_WIRE = 1u << 9,	  /**< Traffic will be from wire to wire */
	DOCA_FLOW_CT_FLAG_CALC_TUN_IP_CHKSUM = 1u << 10,  /**< Enable HW to calculate and set the checksum on L3 header
							  (IPv4) */
	DOCA_FLOW_CT_FLAG_DUP_FILTER_UDP_ONLY = 1u << 11, /**< Apply connection duplication filter for UDP connections
								only */
};

/**
 * @brief CT l3 session types
 */
enum doca_flow_ct_session_type {
	DOCA_FLOW_CT_SESSION_IPV4, /**< IPv4 session. */
	DOCA_FLOW_CT_SESSION_IPV6, /**< IPv6 session. */
	DOCA_FLOW_CT_SESSION_BOTH, /**< Total session. */
	DOCA_FLOW_CT_SESSION_MAX,  /**< Max session types. */
};

/**
 * @brief CT hash table type.
 */
enum doca_flow_ct_hash_type {
	DOCA_FLOW_CT_HASH_NONE,	     /**< No hash table, besides zone, meta data bits reserved as connection ID. */
	DOCA_FLOW_CT_HASH_SYMMETRIC, /**< Hardware symmetric hash function */
};

/**
 * Flow log callback function
 */
typedef void (*doca_flow_ct_flow_log_cb)(struct doca_flow_pipe *pipe, void *entry, uint16_t queue, void *usr_ctx);

/**
 * Direction config
 */
struct doca_flow_ct_direction_cfg {
	bool match_inner;			 /**< match packet inner layer */
	struct doca_flow_meta *zone_match_mask;	 /**< Zone mask to match */
	struct doca_flow_meta *meta_modify_mask; /**< meta mask to modify */
};

/**
 * @brief CT aging user plugin context
 */
struct doca_flow_ct_aging_ctx {
	uint32_t n_total_conns;	   /**< Total connections */
	uint32_t n_total_counters; /**< Total allocated counters */
	void *user_ctx;		   /**< User set context */
};

/**
 * @brief CT aging connection info
 */
union doca_flow_ct_aging_conn {
	uint32_t v;			    /**< Union value, changed on connection change or reused */
	struct {			    /**< Connection detail */
		uint32_t valid : 1;	    /**< Connection is valid */
		uint32_t ctr_origin : 1;    /**< Need origin direction counter */
		uint32_t ctr_reply : 1;	    /**< Need reply direction counter */
		uint32_t ctr_shared : 1;    /**< Need shared counter for both direction */
		uint32_t is_tcp : 1;	    /**< Connection is TCP, default to UDP */
		uint32_t conn_version : 11; /**< +1 on connection reuse */
		uint32_t timeout : 16;	    /**< Timeout in seconds */
	};
};

/**
 * @brief CT aging user plugin connection event
 */
struct doca_flow_ct_aging_conn_event {
	enum doca_flow_entry_op op;	    /**< Callback type: add, delete or update */
	uint32_t aging_conn_id;		    /**< aging global connection ID */
	union doca_flow_ct_aging_conn conn; /**< Connection info */
};

/**
 * @brief CT aging user plugin callbacks
 */
struct doca_flow_ct_aging_ops {
	doca_error_t (*aging_init_cb)(struct doca_flow_ct_aging_ctx *ctx);
	/**< Plugin init callback */
	void (*aging_shutdown_cb)(struct doca_flow_ct_aging_ctx *ctx);
	/**< Plugin shutdown callback */
	void (*conn_sync_cb)(struct doca_flow_ct_aging_ctx *ctx,
			     struct doca_flow_ct_aging_conn_event *conn,
			     uint32_t n);
	/**< Before timer, Connection sync callback for changed connections */
	void (*aging_timer_cb)(struct doca_flow_ct_aging_ctx *ctx, uint64_t current_time_s);
	/**< Callback to check timeout connections based on counter statistics */
};

/**
 * packet callback return value, defines what CT should do with this packet
 */
enum doca_flow_ct_rule_opr {
	DOCA_FLOW_CT_RULE_OK,	   /**< all valid, create a rule */
	DOCA_FLOW_CT_RULE_DROP,	   /**< drop packet */
	DOCA_FLOW_CT_RULE_TX_ONLY, /**< do not create a rule, just forward packet */
};

/**
 * ct packet sent to user via pkt_callback
 */
struct doca_flow_ct_pkt {
	void *buf;		      /**< packet buffer, should not be changed by the user */
	uint32_t data_sz_be;	      /**< packet size, may be changed by the user (if packet change) */
	uint32_t conn_id;	      /**< connection id */
	uint32_t offset : 16;	      /**< packet buffer start sending offset (changed by the user) */
	uint32_t is_ipv6 : 1;	      /**< indication for ipv6 */
	uint32_t is_tcp : 1;	      /**< indication for tcp packet */
	uint32_t is_last : 1;	      /**< indication for last packet in burst */
	union doca_flow_ct_meta meta; /**< metadata in CPU endian */
};

/**
 * shared actions sync_acquire, called from each worker at start
 */
typedef void (*doca_flow_ct_sync_acquire_cb)(int32_t queue);

/**
 * shared actions sync_release, called from each worker on destroy
 */
typedef void (*doca_flow_ct_sync_release_cb)(int32_t queue);

/**
 * shared actions pkt, called from each worker on first packet of a flow
 */
typedef enum doca_flow_ct_rule_opr (*doca_flow_ct_rule_pkt_cb)(int32_t queue,
							       struct doca_flow_ct_pkt *pkt,
							       uint32_t *action_handle_origin,
							       uint32_t *action_handle_reply,
							       uint32_t *fwd_handle_origin,
							       uint32_t *fwd_handle_reply);

/**
 * set of callbacks used for using shared-actions in autonomous mode
 */
struct doca_flow_ct_worker_callbacks {
	doca_flow_ct_sync_acquire_cb worker_init;    /**< called at start of worker thread to sync with user context */
	doca_flow_ct_sync_release_cb worker_release; /**< called at the end of a worker thread */
	doca_flow_ct_rule_pkt_cb rule_pkt;	     /**< called on first packet of a flow */
};

/**
 * @brief doca flow ct global configuration
 */
struct doca_flow_ct_cfg {
	uint32_t nb_arm_queues;				    /**< number of ARM CT queues(thread). */
	uint32_t nb_ctrl_queues;			    /**< number of control queues. */
	uint32_t nb_user_actions;			    /**< number of user actions (shared & non-shared) */
	uint32_t nb_arm_sessions[DOCA_FLOW_CT_SESSION_MAX]; /**< number of ARM CT sessions. */
	uint16_t queue_depth;				    /**< hardware Queue depth, default 512 */
	uint32_t entry_user_ctx_size;			    /**< user entry context size */
	uint32_t flags;					    /**< CT behavior flags */
	struct doca_dev *doca_dev;			    /**< Doca device, override ib_dev */
	void *ib_dev;					    /**< IB verbs device context, deprecated */
	void *ib_pd;					    /**< device protection domain used with DPDK, deprecated */
	uint16_t aging_core;				    /**< CT aging thread bind to CPU core. */
	uint16_t aging_query_delay_s;			    /**< CT aging query delay */
	doca_flow_ct_flow_log_cb flow_log_cb;		    /**< Flow log callback function */
	struct doca_flow_ct_aging_ops *aging_ops;	    /**< Aging plugin callbacks */
	uint32_t base_core_id;				    /**< Base core id for the workers */
	uint32_t dup_filter_sz;				    /**< Number of connections to cache in duplication filter */
	union {
		/* Managed mode: */
		struct doca_flow_ct_direction_cfg direction[2]; /**< Configuration of each direction */
		/* Autonomous mode: */
		struct {
			uint16_t tcp_timeout_s;		       /**< TCP timeout in second. */
			uint16_t tcp_session_del_s;	       /**< time to delay or kill TCP session after RST/FIN. */
			uint16_t udp_timeout_s;		       /**< UDP timeout in second. */
			enum doca_flow_tun_type tunnel_type;   /**< Tunnel type */
			uint16_t vxlan_dst_port;	       /**< outer UDP destination port for VxLAN traffic. BE */
			enum doca_flow_ct_hash_type hash_type; /**< Connection hash table type. */
			uint32_t meta_user_bits;	       /**< User data bits ignored by worker */
			uint32_t meta_action_bits; /**< User action data bits carried by identified connection packet */
			/**< Mask to indicate which field/bits user used to set zone information */
			struct doca_flow_meta *meta_zone_mask;
			/**< Mask to indicate which field/bits to modify so user can read after CT pipe */
			struct doca_flow_meta *connection_id_mask;
			struct doca_flow_ct_worker_callbacks worker_cb; /**< callbacks for autonomous mode with shared
									   actions */
		};
	}; /* Exclusive configuration */
};

/**
 * @brief Initialize the doca flow ct.
 *
 * This is the global initialization function for doca flow ct. It
 * initializes all resources used by doca flow.
 *
 * Must be invoked first before any other function in this API.
 * this is a one time call, used for doca flow ct initialization and
 * global configurations.
 *
 * Must be invoked after Doca Flow initilization, before port start.
 *
 * @param cfg
 *   CT configuration.
 * @return
 *   0 on success, a negative errno value otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_init(const struct doca_flow_ct_cfg *cfg);

/**
 * @brief Check if doca device supported by CT.
 *
 * @param devinfo
 *   Doca device info.
 * @return
 *   DOCA_SUCCESS - device supported by CT.
 *   Error code - in case of failure:
 *   - DOCA_ERROR_NOT_SUPPORTED - provided devinfo does not support CT.
 *   - DOCA_ERROR_INVALID_VALUE - received invalid input.
 *   - DOCA_ERROR_DRIVER - failed to query capability support.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_cap_is_dev_supported(const struct doca_devinfo *devinfo);

/**
 * @brief Sets UDP outer destination port for VxLAN traffic.
 *
 * This is to initialization the UDP outer destination port for VxLAN traffic.
 * Sets the VxLAN dest port global variable value.
 *
 * Optional, default to 4789.
 * Must be invoked after Doca Flow and CT initialization.
 *
 * @param dst_port
 *   outer UDP destination value.
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_set_vxlan_dst_port(uint16_t dst_port);

/**
 * @brief Destroy the doca flow ct.
 *
 * Release all the resources used by doca flow ct.
 *
 * Must be invoked before doca flow destroy.
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_destroy(void);

/**
 * @brief Prepare meta with zone and default CT type.
 *
 * @param meta
 *   Doca flow meta.
 * @param zone
 *   Zone value.
 * @param is_reply
 *   Prepare reply direction zone in asymmetric mode.
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_meta_prepare(struct doca_flow_meta *meta, uint32_t zone, bool is_reply);

/**
 * @brief Prepare meta as mask with zone and CT type.
 *
 * @param meta
 *   Doca flow meta.
 * @param is_reply
 *   Prepare reply direction zone in asymmetric mode.
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_meta_mask_prepare(struct doca_flow_meta *meta, bool is_reply);

/**
 * @brief Set meta match zone data to doca_flow meta.
 *
 * @param meta
 *   doca_flow meta.
 * @param zone
 *   Zone value.
 * @param is_reply
 *   Set reply direction zone in asymmetric mode.
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_meta_set_match_zone(struct doca_flow_meta *meta, uint32_t zone, bool is_reply);

/**
 * @brief Get modify meta zone data.
 *
 * @param meta
 *   CT meta.
 * @param is_reply
 *   Get reply direction zone in asymmetric mode.
 * @return
 *   Zone value.
 */
DOCA_EXPERIMENTAL
uint32_t doca_flow_ct_meta_get_zone(uint32_t meta, bool is_reply);

/**
 * @brief Set meta zone data applies to identified connection packets.
 *
 * @param meta
 *   CT meta.
 * @param zone
 *   Zone value.
 * @param is_reply
 *   Set reply direction zone in asymmetric mode.
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_meta_set_zone(uint32_t *meta, uint32_t zone, bool is_reply);

/**
 * @brief Set meta action data applies to identified connection packets.
 *
 * @param meta
 *   CT meta.
 * @param action_data
 *   Action data.
 * @param is_reply
 *   Reply direction in asymmetric mode.
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_meta_set_action(uint32_t *meta, uint32_t action_data, bool is_reply);

/**
 * @brief Set user data in meta data field.
 *
 * User data is ignored by worker, can't be carried with identified connection packets.
 * @param meta
 *   CT meta.
 * @param user_data
 *   User data value.
 * @param is_reply
 *   Reply direction in asymmetric mode.
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_meta_set_user(uint32_t *meta, uint32_t user_data, bool is_reply);

/**
 * @brief Get zone data bit offset in meta data field.
 *
 * @param is_reply
 *   Reply direction in asymmetric mode.
 * @return
 *   Zone data bit offset.
 */
DOCA_EXPERIMENTAL
uint32_t doca_flow_ct_meta_get_zone_offset(bool is_reply);

/**
 * @brief Get action data bit offset in meta data field.
 *
 * @param is_reply
 *   Reply direction in asymmetric mode.
 * @return
 *   Action data bit offset.
 */
DOCA_EXPERIMENTAL
uint32_t doca_flow_ct_meta_get_action_offset(bool is_reply);

/**
 * @brief Get User data bit offset in meta data field.
 *
 * @param is_reply
 *   Reply direction in asymmetric mode.
 * @return
 *   User data bit offset.
 */
DOCA_EXPERIMENTAL
uint32_t doca_flow_ct_meta_get_user_offset(bool is_reply);

/**
 * @brief Get zone from packet meta
 *
 * @param meta
 *   Packet meta.
 * @param is_reply
 *   Reply direction in asymmetric mode.
 * @return
 *   Zone
 */
DOCA_EXPERIMENTAL
uint32_t doca_flow_ct_meta_get_match_zone(uint32_t meta, bool is_reply);

/*********** start of aging plugin API ***************/

/**
 * @brief CT aging counter state
 */
struct doca_flow_ct_aging_counter_state {
	uint32_t ctr_id; /**< Counter global ID */
	bool inuse;	 /**< Counter in use */
};

/**
 * @brief Get counter state inside aging plugin timer callback
 *
 * @param ctx
 *   CT Aging callback context
 * @param [out] ctrs
 *   List of counter states with counter ID set
 * @param n
 *   Number of counter states to get
 * @return
 *   DOCA_SUCCESS in case of success, others on error.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_aging_counter_state_get(struct doca_flow_ct_aging_ctx *ctx,
						  struct doca_flow_ct_aging_counter_state *ctrs,
						  uint32_t n);

/**
 * @brief Set counter state inside aging plugin timer callback
 *
 * @param ctx
 *   CT Aging callback context
 * @param ctrs
 *   List of counter states
 * @param n
 *   Number of counter states to set
 * @return
 *   DOCA_SUCCESS in case of success, others on error.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_aging_counter_state_set(struct doca_flow_ct_aging_ctx *ctx,
						  struct doca_flow_ct_aging_counter_state *ctrs,
						  uint32_t n);

/**
 * @brief Counter statistics
 */
struct doca_flow_ct_aging_counter {
	uint32_t ctr_id;      /**< Counter ID */
	uint16_t last_hit_s;  /**< Last hit time in seconds */
	uint64_t total_bytes; /**< Total bytes the counter received */
	uint64_t total_pkts;  /**< Total packets the counter received */
};

/**
 * @brief Get counter statistics inside aging plugin timer callback
 *
 * @param ctx
 *   CT Aging callback context
 * @param [out] ctrs
 *   List of counter states with counter ID set
 * @param n
 *   Number of counter statistics to get
 * @return
 *   DOCA_SUCCESS in case of success, others on error.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_aging_counter_query(struct doca_flow_ct_aging_ctx *ctx,
					      struct doca_flow_ct_aging_counter *ctrs,
					      uint32_t n);

/**
 * @brief Connection update information
 */
struct doca_flow_ct_aging_conn_update_info {
	uint32_t aging_conn_id;			 /**< Aging global connection ID*/
	union doca_flow_ct_aging_conn conn_info; /**< Connection info */
	uint32_t ctr_origin_id;			 /**< Origin direction counter ID, UINT32_MAX to disable counter */
	uint32_t ctr_reply_id;			 /**< Reply direction counter ID, UINT32_MAX to disable counter */
};

/**
 * @brief Update connection counter inside aging plugin timer callback
 *
 * @param ctx
 *   CT Aging callback context
 * @param info
 *   List of connection info
 * @param n
 *   Number of connections to update
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_aging_conn_update(struct doca_flow_ct_aging_ctx *ctx,
				    struct doca_flow_ct_aging_conn_update_info *info,
				    uint32_t n);

/**
 * @brief Remove timeout connections inside aging plugin timer callback
 *
 * @param ctx
 *   CT Aging callback context
 * @param aging_conn_ids
 *   List of aging global connection IDs
 * @param n
 *   Number of connections to remove
 */
DOCA_EXPERIMENTAL
void doca_flow_ct_aging_conn_timeout(struct doca_flow_ct_aging_ctx *ctx, uint32_t *aging_conn_ids, uint32_t n);

/*********** end of aging plugin API ***************/

/*********** start of management mode API ***************/

/**
 * doca flow CT action
 */
struct doca_flow_ct_actions {
	enum doca_flow_resource_type resource_type;
	/**< shared/non-shared */
	union {
		uint32_t action_handle;
		/**< handle of a predefined shared action */
		struct {
			uint8_t action_idx;
			/**< action template index */
			struct doca_flow_meta meta;
			/**< modify meta */
			struct doca_flow_header_l4_port l4_port;
			/**< UDP or TCP source and destination port */
			union {
				struct doca_flow_ct_ip4 ip4;
				/**< source and destination ipv4 addresses */
				struct doca_flow_ct_ip6 ip6;
				/**< source and destination ipv6 addresses */
			};
		} data;
	};
};

/**
 * @brief doca flow CT IPv4 match pattern
 */
struct doca_flow_ct_match4 {
	struct doca_flow_header_l4_port l4_port;
	/**< UDP or TCP source and destination port */
	doca_be32_t src_ip;
	/**< ip src address */
	doca_be32_t dst_ip;
	/**< ip dst address */
	doca_be32_t metadata;
	/**< metadata */
	uint8_t next_proto;
	/**< ip next protocol */
};

/**
 * @brief doca flow CT IPv6 match pattern
 */
struct doca_flow_ct_match6 {
	struct doca_flow_header_l4_port l4_port;
	/**< UDP or TCP source and destination port */
	doca_be32_t src_ip[4];
	/**< ip src address */
	doca_be32_t dst_ip[4];
	/**< ip dst address */
	doca_be32_t metadata;
	/**< metadata */
	uint8_t next_proto;
	/**< ip next protocol */
};

/**
 * @brief doca flow CT match pattern
 */
struct doca_flow_ct_match {
	union {
		struct doca_flow_ct_match4 ipv4;
		struct doca_flow_ct_match6 ipv6;
	};
};

/**
 * @brief doca flow CT entry operation flags
 */
enum doca_flow_ct_entry_flags {
	DOCA_FLOW_CT_ENTRY_FLAGS_NO_WAIT = (1 << 0),
	/**< entry will not be buffered, send to hardware immediately */
	DOCA_FLOW_CT_ENTRY_FLAGS_DIR_ORIGIN = (1 << 1),
	/**< apply to origin direction */
	DOCA_FLOW_CT_ENTRY_FLAGS_DIR_REPLY = (1 << 2),
	/**< apply to reply direction */
	DOCA_FLOW_CT_ENTRY_FLAGS_IPV6_ORIGIN = (1 << 3),
	/**< origin direction is IPv6, union in struct doca_flow_ct_match is ipv6 */
	DOCA_FLOW_CT_ENTRY_FLAGS_IPV6_REPLY = (1 << 4),
	/**< reply direction is IPv6, union in struct doca_flow_ct_match is ipv6 */
	DOCA_FLOW_CT_ENTRY_FLAGS_COUNTER_ORIGIN = (1 << 5),
	/**< Apply counter to origin direction */
	DOCA_FLOW_CT_ENTRY_FLAGS_COUNTER_REPLY = (1 << 6),
	/**< Apply counter to reply direction */
	DOCA_FLOW_CT_ENTRY_FLAGS_COUNTER_SHARED = (1 << 7),
	/**< Counter is shared for both direction */
	DOCA_FLOW_CT_ENTRY_FLAGS_FLOW_LOG = (1 << 8),
	/**< Enable flow log on entry removed */
	DOCA_FLOW_CT_ENTRY_FLAGS_ALLOC_ON_MISS = (1 << 9),
	/**< Allocate on entry not found */
	DOCA_FLOW_CT_ENTRY_FLAGS_DUP_FILTER_ORIGIN = 1u << 10,
	/**< Enable duplication filter on origin  */
	DOCA_FLOW_CT_ENTRY_FLAGS_DUP_FILTER_REPLY = 1u << 11,
	/**< Enable duplication filter on reply  */
};

/**
 * @brief Lookup recent CT entry and create on miss.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] flags
 * operation flags, see doca_flow_ct_entry_flags.
 * @param [in] match_origin
 * match pattern in origin direction.
 * @param [in] hash_origin
 * 5 tuple hash of origin direction.
 * @param [in] match_reply
 * match pattern in reply direction, default to reverse of origin pattern.
 * @param [in] hash_reply
 * 5 tuple hash of reply direction.
 * @param [out] entry
 * pointer to save the new entry
 * @param [out] conn_found
 * whether the entry is found in recent list.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - invalid pipe state.
 * - DOCA_ERROR_NOT_SUPPORTED - unsupported operation.
 * - DOCA_ERROR_FULL - pipe is full.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_entry_prepare(uint16_t queue,
					struct doca_flow_pipe *pipe,
					uint32_t flags,
					struct doca_flow_ct_match *match_origin,
					uint32_t hash_origin,
					struct doca_flow_ct_match *match_reply,
					uint32_t hash_reply,
					struct doca_flow_pipe_entry **entry,
					bool *conn_found);

/**
 * @brief Free the CT entry that hasn't been added to CT pipe.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [out] entry
 * pointer to the CT entry
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_BAD_STATE - invalid pipe state.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_entry_prepare_rollback(uint16_t queue,
						 struct doca_flow_pipe *pipe,
						 struct doca_flow_pipe_entry *entry);

/**
 * @brief Add new entry to doca flow CT pipe.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] flags
 * operation flags, see doca_flow_ct_entry_flags.
 * @param [in] match_origin
 * match pattern in origin direction.
 * @param [in] match_reply
 * match pattern in reply direction, default to reverse of origin pattern.
 * @param [in] actions_origin
 * actions to set on origin direction
 * @param [in] actions_reply
 * actions to set on reply direction
 * @param [in] fwd_handle_origin
 * fwd handle for origin direction
 * @param [in] fwd_handle_reply
 * fwd handle for reply direction
 * @param [in] timeout_s
 * aging timeout in second, 0 to disable aging
 * @param [in] usr_ctx
 * user context data to associate to entry
 * @param [out] entry
 * pointer of the CT entry
 * @return
 * DOCA_SUCCESS - in case of success.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_add_entry(uint16_t queue,
				    struct doca_flow_pipe *pipe,
				    uint32_t flags,
				    struct doca_flow_ct_match *match_origin,
				    struct doca_flow_ct_match *match_reply,
				    const struct doca_flow_ct_actions *actions_origin,
				    const struct doca_flow_ct_actions *actions_reply,
				    uint32_t fwd_handle_origin,
				    uint32_t fwd_handle_reply,
				    uint32_t timeout_s,
				    void *usr_ctx,
				    struct doca_flow_pipe_entry *entry);

/**
 * @brief Add missing direction rule to CT connection.
 *
 * The direction must be specified via flags, must be empty when the connection created.
 * Must call `doca_flow_entries_process` to polling adding result.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] flags
 * operation flags, see doca_flow_ct_entry_flags.
 * @param [in] match
 * match pattern of the direction.
 * @param [in] actions
 * actions to set of the direction
 * @param [in] fwd_handle
 * fwd handle for the input direction created
 * @param [in] entry
 * pointer of the entry
 * @return
 * DOCA_SUCCESS - in case of success.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_entry_add_dir(uint16_t queue,
					struct doca_flow_pipe *pipe,
					uint32_t flags,
					struct doca_flow_ct_match *match,
					const struct doca_flow_ct_actions *actions,
					uint32_t fwd_handle,
					struct doca_flow_pipe_entry *entry);

/**
 * @brief Update CT entry meta or counter.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] flags
 * operation flags, see doca_flow_ct_entry_flags.
 * @param [in] entry
 * The CT pipe entry to query.
 * @param [in] actions_origin
 * actions to set on origin direction
 * @param [in] actions_reply
 * actions ta to set on reply direction
 * @param [in] fwd_handle_origin
 * fwd handle for origin direction
 * @param [in] fwd_handle_reply
 * fwd handle for reply direction
 * @param [in] timeout_s
 * aging timeout in second, 0 to disable aging
 * @return
 * DOCA_SUCCESS - in case of success.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_update_entry(uint16_t queue,
				       struct doca_flow_pipe *pipe,
				       uint32_t flags,
				       struct doca_flow_pipe_entry *entry,
				       const struct doca_flow_ct_actions *actions_origin,
				       const struct doca_flow_ct_actions *actions_reply,
				       uint32_t fwd_handle_origin,
				       uint32_t fwd_handle_reply,
				       uint32_t timeout_s);

/**
 * @brief remove CT entry.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] flags
 * operation flags, see doca_flow_ct_entry_flags.
 * @param [in] entry
 * The CT pipe entry to query.
 * @return
 * DOCA_SUCCESS - in case of success.
 * DOCA_ERROR_INVALID_VALUE - in case of invalid input.
 * DOCA_ERROR_IN_PROGRESS - in case of connection is in progress of hardware processing.
 * DOCA_ERROR_NOT_FOUND - in case of entry not found or destroyed.
 * DOCA_ERROR_BAD_STATE - in case of invalid pipe or connection state.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_rm_entry(uint16_t queue,
				   struct doca_flow_pipe *pipe,
				   uint32_t flags,
				   struct doca_flow_pipe_entry *entry);

/**
 * @brief Get CT entry match pattern.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] flags
 * operation flags, see doca_flow_ct_entry_flags.
 * @param [in] entry
 * CT entry.
 * @param [out] match_origin
 * Pointer to save match pattern of origin direction
 * @param [out] match_reply
 * Pointer to save match pattern of reply direction
 * @param [out] entry_flags
 * Entry flags, see doca_flow_ct_entry_flags.
 * @return
 * DOCA_SUCCESS - in case of success.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_get_entry(uint16_t queue,
				    struct doca_flow_pipe *pipe,
				    uint32_t flags,
				    struct doca_flow_pipe_entry *entry,
				    struct doca_flow_ct_match *match_origin,
				    struct doca_flow_ct_match *match_reply,
				    uint64_t *entry_flags);

/**
 * @brief Extract information about specific entry
 *
 * Query the packet statistics about specific CT pipe entry
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] flags
 * operation flags, see doca_flow_ct_entry_flags.
 * @param [in] entry
 * The CT pipe entry to query.
 * @param [in] stats_origin
 * Data of origin direction retrieved by the query.
 * @param [in] stats_reply
 * Data of reply direction retrieved by the query.
 * @param [in] last_hit_s
 * Last hit time in the number of seconds since the Epoch.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_UNKNOWN - otherwise.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_query_entry(uint16_t queue,
				      struct doca_flow_pipe *pipe,
				      uint32_t flags,
				      struct doca_flow_pipe_entry *entry,
				      struct doca_flow_query *stats_origin,
				      struct doca_flow_query *stats_reply,
				      uint64_t *last_hit_s);

/**
 * Retrieves the user context associated with a specific CT entry.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] entry
 * The CT pipe entry to query.
 * @return
 * A pointer to the user context associated with the CT entry.
 * NULL - in case of failure.
 */
DOCA_EXPERIMENTAL
void *doca_flow_ct_entry_get_user_ctx(uint16_t queue, struct doca_flow_pipe *pipe, struct doca_flow_pipe_entry *entry);

/**
 * Retrieves the connection ID associated with a given CT entry.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] entry
 * CT entry.
 * @return
 * The connection ID associated with the CT entry.
 * UINT32_MAX - in case of failure.
 */
DOCA_EXPERIMENTAL
uint32_t doca_flow_ct_entry_get_conn_id(uint16_t queue,
					struct doca_flow_pipe *pipe,
					struct doca_flow_pipe_entry *entry);

/**
 * Retrieves the aging connection ID associated with a given CT entry.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] entry
 * CT entry.
 * @return
 * The aging connection ID associated with the CT entry.
 * UINT32_MAX - in case of failure.
 */
DOCA_EXPERIMENTAL
uint32_t doca_flow_ct_entry_get_aging_conn_id(uint16_t queue,
					      struct doca_flow_pipe *pipe,
					      struct doca_flow_pipe_entry *entry);
/**
 * @brief Retrieves the entry from CT pipe based on the given connection ID.
 *
 * @param [in] queue
 * queue ID, offset from doca_flow.nb_queues.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] conn_id
 * CT connection ID inside queue.
 * @param [out] entry
 * CT entry retrieved by the query.
 * @param [out] user_ctx
 * User context associated with the CT entry.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_BAD_STATE - wrong pipe state.
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_entry_get_by_id(uint16_t queue,
					  struct doca_flow_pipe *pipe,
					  uint32_t conn_id,
					  struct doca_flow_pipe_entry **entry,
					  void **user_ctx);

/*********** end of management mode API ***************/

/**
 * @brief Add shared modify-action
 *
 * @param [in] ctrl_queue
 * control queue id.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] actions
 * list of actions data, each updated with action id
 * @param [in] nb_actions
 * number of actions to create
 * @param [out] actions_handles
 * list of handles allocated for the input actions
 * @return
 * DOCA_SUCCESS - in case of success
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input
 * - DOCA_ERROR_EMPTY - user actions pool is empty
 * - DOCA_ERROR_BAD_STATE - wrong number of HW responses
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_actions_add_shared(uint16_t ctrl_queue,
					     struct doca_flow_pipe *pipe,
					     const struct doca_flow_ct_actions actions[],
					     uint32_t nb_actions,
					     uint32_t actions_handles[]);

/**
 * @brief Remove shared modify-action
 *
 * @param [in] ctrl_queue
 * control ctrl queue id.
 * @param [in] pipe
 * Pointer to pipe.
 * @param [in] actions_handles
 * list of action ids
 * @param [in] nb_actions
 * number of actions to create
 * @return
 * DOCA_SUCCESS - always success
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_actions_rm_shared(uint16_t ctrl_queue,
					    struct doca_flow_pipe *pipe,
					    uint32_t actions_handles[],
					    uint32_t nb_actions);

/**
 * @brief CT register multiple forwards pipes
 *
 * @param [in] port
 * Port struct.
 * @param [in] fwd_count
 * number of CT forwards
 * @param [in] fwd
 * array of CT forwards
 * @param [out] fwd_handle
 * array of forward handles
 * @return
 * DOCA_SUCCESS - in case of success
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input
 * - DOCA_ERROR_NOT_FOUND - fwd pipe not found
 * - DOCA_ERROR_NOT_SUPPORTED - fwd type not supported
 * - DOCA_ERROR_BAD_STATE - fwd register should be called before CT pipe creation
 */
DOCA_EXPERIMENTAL
doca_error_t doca_flow_ct_fwd_register(struct doca_flow_port *port,
				       uint32_t fwd_count,
				       struct doca_flow_fwd fwd[],
				       uint32_t fwd_handle[]);

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_FLOW_CT_H_ */
