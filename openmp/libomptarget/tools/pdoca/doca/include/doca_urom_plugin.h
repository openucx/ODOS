/*
 * Copyright (c) 2023-2024 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @defgroup DOCA_UROM_PLUGIN DOCA UROM plugins
 * @{
 *  DOCA UROM Worker Plugin interfaces and data structures.
 * @}
 */

#ifndef DOCA_UROM_PLUGIN_H_
#define DOCA_UROM_PLUGIN_H_

#include <limits.h>

#include <ucs/datastruct/list.h>

#include <doca_error.h>
#include <doca_compat.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DOCA_UROM_PLUGIN_NAME_MAX_LEN 48 /**< Maximum plugin name length */

/**
 * @brief DOCA UROM worker context
 */
struct urom_worker_ctx;

/** UROM plugin info structure */
struct doca_urom_service_plugin_info {
	uint64_t id;					 /**< Unique ID to send commands to the plugin */
	uint64_t version;				 /**< Plugin version */
	char plugin_name[DOCA_UROM_PLUGIN_NAME_MAX_LEN]; /**< The .so plugin file name, without ".so" */
};

/**
 * @brief UROM Worker command structure
 */
struct urom_worker_cmd {
	uint64_t type;	       /**< Plugin type that command belongs to */
	uint64_t urom_context; /**< UROM context, User shouldn't change it */
	uint64_t len;	       /**< Plugin command length */
	uint8_t plugin_cmd[];  /**< Plugin command buffer */
};

/* Suppress clang compilation warning on flexible-array-extensions plugin_cmd[]
 * is a flexible-array-extensions at it is at the end  of the struct urom_worker_cmd.
 * It declared last in every struct that instantiate it so this warning is just
 * too strict here.
 */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wflexible-array-extensions"
#endif

/**
 * @brief UROM Worker command descriptor structure
 */
struct urom_worker_cmd_desc {
	ucs_list_link_t entry;		   /**< Descriptor entry in linked list */
	uint64_t dest_id;		   /**< Worker communication channel ID */
	struct urom_worker_cmd worker_cmd; /**< UROM worker command */
};

/**
 * @brief UROM Worker notification structure
 */
struct urom_worker_notify {
	uint64_t type;		/**< Plugin type that notification belongs to */
	uint64_t urom_context;	/**< UROM context, it's the user's responsibility to set it
				 * to be identical to the command context */
	uint64_t len;		/**< Plugin notification length */
	doca_error_t status;	/**< Notification status in DOCA terms */
	uint32_t reserved;	/**< Reserved 4 bytes to align status address with 64 bits */
	uint8_t plugin_notif[]; /**< Plugin notification buffer */
};

/**
 * @brief UROM Worker notification descriptor structure
 */
struct urom_worker_notif_desc {
	ucs_list_link_t entry;			/**< Descriptor entry in linked list */
	uint64_t dest_id;			/**< Worker communication channel ID, it's the user's responsibility
						 * to set it to be identical to the command descriptor id */
	struct urom_worker_notify worker_notif; /**< UROM worker notification */
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

/**
 * @brief UROM Worker plugin interface
 */
struct urom_plugin_iface {
	/**
	 * Store which DOCA API version the plugin is compiled with.
	 */
	DOCA_STRUCT_START;

	/**
	 * Open worker plugin context, including resources initialization.
	 */
	doca_error_t (*open)(struct urom_worker_ctx *ctx);

	/**
	 * Close worker plugin context.
	 */
	void (*close)(struct urom_worker_ctx *ctx);

	/**
	 * Return address of worker component.
	 */
	doca_error_t (*addr)(struct urom_worker_ctx *ctx, void *addr, size_t *addr_len);

	/**
	 * Process worker commands.
	 *
	 * Worker is responsible to free each entry removed from cmd_list.
	 * Entries are of type struct urom_cmd_desc.
	 */
	doca_error_t (*worker_cmd)(struct urom_worker_ctx *ctx, ucs_list_link_t *cmd_list);

	/**
	 * Progress the worker component and add notifications to the end of notif_list.
	 *
	 * Caller is responsible to free each entry in notif_list.
	 * @note: if there is no notification should return DOCA_ERROR_EMPTY
	 */
	doca_error_t (*progress)(struct urom_worker_ctx *ctx, ucs_list_link_t *notif_list);

	/**
	 * Pack a worker notification before sending on the wire.
	 */
	doca_error_t (*notif_pack)(struct urom_worker_notify *notif, size_t *packed_notif_len, void *packed_notif);
};

/**
 * @brief UROM Worker domain lookups interface
 */
struct urom_domain_lookups_iface {
	/**
	 * Domain address lookup function.
	 */
	doca_error_t (*addr_lookup)(struct urom_worker_ctx *ctx, uint64_t domain_id, void **addr);

	/**
	 * Domain memory segment lookup function.
	 */
	doca_error_t (*seg_lookup)(struct urom_worker_ctx *ctx, uint64_t domain_id, uint64_t va, int *seg);

	/**
	 * Domain memory handle lookup function.
	 */
	doca_error_t (*memh_lookup)(struct urom_worker_ctx *ctx, uint64_t domain_id, int seg, size_t *len, void **memh);

	/**
	 * Domain memory key lookup function.
	 */
	doca_error_t (*mkey_lookup)(struct urom_worker_ctx *ctx, uint64_t domain_id, int seg, size_t *len, void **rkey);
};

/**
 * @brief UROM worker context
 */
struct urom_worker_ctx {
	struct urom_plugin_iface iface;		       /**< Worker plugin interface */
	struct urom_domain_lookups_iface domain_iface; /**< Worker domain lookups interface */
	void *plugin_ctx;			       /**< Worker plugin context */
};

/**
 * @brief Worker domain address lookup
 *
 * @param [in] ctx
 * UROM worker context
 * @param [in] domain_id
 * UROM domain id
 * @param [out] addr
 * Set domain address
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_FOUND - if domain_id is invalid.
 */
static inline doca_error_t doca_urom_worker_domain_addr_lookup(struct urom_worker_ctx *ctx,
							       uint64_t domain_id,
							       void **addr)
{
	return ctx->domain_iface.addr_lookup(ctx, domain_id, addr);
}

/**
 * @brief Worker domain segment id lookup according to the domain id and segment virtual address
 *
 * @param [in] ctx
 * UROM worker context
 * @param [in] domain_id
 * UROM domain id
 * @param [in] va
 * Segment virtual address
 * @param [out] seg
 * Set segment id
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_FOUND - in case seg doesn't exist
 */
static inline doca_error_t doca_urom_worker_domain_seg_lookup(struct urom_worker_ctx *ctx,
							      uint64_t domain_id,
							      uint64_t va,
							      int *seg)
{
	return ctx->domain_iface.seg_lookup(ctx, domain_id, va, seg);
}

/**
 * @brief Worker domain segment memory handle lookup according to the domain and segment ids
 *
 * @param [in] ctx
 * UROM worker context
 * @param [in] domain_id
 * UROM domain id
 * @param [in] seg
 * Segment id
 * @param [out] len
 * Set memory handle size
 * @param [out] memh
 * Set memory handle
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_FOUND - in case memh doesn't exist
 */
static inline doca_error_t doca_urom_worker_domain_memh_lookup(struct urom_worker_ctx *ctx,
							       uint64_t domain_id,
							       int seg,
							       size_t *len,
							       void **memh)
{
	return ctx->domain_iface.memh_lookup(ctx, domain_id, seg, len, memh);
}

/**
 * @brief Worker domain segment remote memory key lookup according to the segment and seg ids
 *
 * @param [in] ctx
 * UROM worker context
 * @param [in] domain_id
 * UROM domain id
 * @param [in] seg
 * Segment id
 * @param [out] len
 * Set memory handle size
 * @param [out] rkey
 * Set memory remote key
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * doca_error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - if an invalid input had been received.
 * - DOCA_ERROR_NOT_FOUND - in case rkey doesn't exist
 */
static inline doca_error_t doca_urom_worker_domain_mkey_lookup(struct urom_worker_ctx *ctx,
							       uint64_t domain_id,
							       int seg,
							       size_t *len,
							       void **rkey)
{
	return ctx->domain_iface.mkey_lookup(ctx, domain_id, seg, len, rkey);
}

#ifdef __cplusplus
}
#endif

#endif /* DOCA_UROM_PLUGIN_H_ */
