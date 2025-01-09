/*
 * Copyright (c) 2021 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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
 * @file doca_flow_net.h
 * @page doca flow net
 * @defgroup FLOW_NET flow net define
 * DOCA HW offload flow net structure define. For more details please refer to
 * the user guide on DOCA devzone.
 *
 * @{
 */

#ifndef DOCA_FLOW_NET_H_
#define DOCA_FLOW_NET_H_

#include <stdint.h>
#include <stdbool.h>
#include <doca_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DOCA_FLOW_ETHER_ADDR_LEN (6)		/**< length of ether add length. */
#define DOCA_FLOW_PROTO_ICMP (1)		/**< Internet Control Message Protocol v4. */
#define DOCA_FLOW_PROTO_ICMP6 (58)		/**< Internet Control Message Protocol v6. */
#define DOCA_FLOW_PROTO_IPV4 (4)		/**< Internet Protocol v4. */
#define DOCA_FLOW_PROTO_IPV6 (41)		/**< Internet Protocol v6. */
#define DOCA_FLOW_PROTO_TCP (6)			/**< Transmission Control Protocol. */
#define DOCA_FLOW_PROTO_UDP (17)		/**< User Datagram Protocol. */
#define DOCA_FLOW_PROTO_GRE (47)		/**< Cisco GRE tunnels (rfc 1701,1702). */
#define DOCA_FLOW_PROTO_ESP (50)		/**< Encapsulated Security Payload Protocol. */
#define DOCA_FLOW_GTPU_DEFAULT_PORT (2152)	/**< gtpu upd port id. */
#define DOCA_FLOW_VXLAN_DEFAULT_PORT (4789)	/**< default vxlan port id. */
#define DOCA_FLOW_VXLAN_GPE_DEFAULT_PORT (4790) /**< default vxlan-gpe port id. */
#define DOCA_FLOW_MPLS_DEFAULT_PORT (6635)	/**< default MPLS port id. */
#define DOCA_FLOW_GENEVE_DEFAULT_PORT (6081)	/**< default GENEVE port id. */
#define DOCA_FLOW_PSP_DEFAULT_PORT (1000)	/**< default PSP port id. */

/** Ethernet frame types */
#define DOCA_FLOW_ETHER_TYPE_IPV4 (0x0800) /**< IPv4 Protocol. */
#define DOCA_FLOW_ETHER_TYPE_IPV6 (0x86DD) /**< IPv6 Protocol. */
#define DOCA_FLOW_ETHER_TYPE_TEB (0x6558)  /**< Transparent Ethernet Bridging. */

/** UDP header length in bytes */
#define DOCA_FLOW_UDP_HEADER_LEN 8

/** IPsec ESP header maximal length in bytes */
#define DOCA_FLOW_ESP_HEADER_LEN (4 * sizeof(doca_be32_t))

/** Crypto key maximal length in bytes */
#define DOCA_FLOW_CRYPTO_KEY_LEN_MAX 32

/** Max GENEVE options length in single packet (in 4 bytes granularity). */
#define DOCA_FLOW_GENEVE_OPT_LEN_MAX 63

/** Max GENEVE options length in single encap data (in 4 bytes granularity). Encap total size is 128*/
#define DOCA_FLOW_ENCAP_GENEVE_OPT_LEN_MAX 19

/** Max data length in single GENEVE option (in 4 bytes granularity). */
#define DOCA_FLOW_GENEVE_DATA_OPTION_LEN_MAX 31

/** Upper bound for GENEVE TLV options number. */
#define DOCA_FLOW_GENEVE_NUM_OF_OPTIONS_MAX 8

/** Max MPLS labels in single match. */
#define DOCA_FLOW_MPLS_LABELS_MAX 5

/** PSP header maximal length in bytes */
#define DOCA_FLOW_PSP_HEADER_LEN (6 * sizeof(doca_be32_t))

/** VXLAN GPE next protocol definition */
#define DOCA_FLOW_VXLAN_GPE_TYPE_IPV4 1 /**< IPv4 Protocol. */
#define DOCA_FLOW_VXLAN_GPE_TYPE_IPV6 2 /**< IPv6 Protocol. */
#define DOCA_FLOW_VXLAN_GPE_TYPE_ETH 3	/**< Ethernet Protocol. */
#define DOCA_FLOW_VXLAN_GPE_TYPE_NSH 4	/**< NSH Protocol. */
#define DOCA_FLOW_VXLAN_GPE_TYPE_MPLS 5 /**< MPLS Protocol. */
#define DOCA_FLOW_VXLAN_GPE_TYPE_GBP 6	/**< GBP Protocol. */
#define DOCA_FLOW_VXLAN_GPE_TYPE_VBNG 7 /**< vBNG Protocol. */

/**
 * Crypto tunnel header may consist of:
 *  - Ethernet addresses
 *  - Ethernet type
 *  - optional VLAN and 802.1Q headers
 *  - IPv4 (with full options) or IPv6 (w/o options)
 *  - optional UDP header
 *  - PSP (or other crypto protocol) header
 */
#define DOCA_FLOW_CRYPTO_HEADER_LEN_MAX \
	(DOCA_FLOW_ETHER_ADDR_LEN * 2 + sizeof(doca_be16_t) + sizeof(doca_be16_t) * 2 * 2 + sizeof(doca_be32_t) * 15 + \
	 sizeof(doca_be32_t) * 2 + DOCA_FLOW_PSP_HEADER_LEN)

/**
 * @brief doca flow vlan header
 */
struct doca_flow_header_eth_vlan {
	doca_be16_t tci;
	/**< vlan tci */
};

/**
 * @brief doca flow layer 3 packet type
 */
enum doca_flow_l3_type {
	DOCA_FLOW_L3_TYPE_NONE = 0,
	/**< l3 type is not set */
	DOCA_FLOW_L3_TYPE_IP4,
	/**< l3 type is ipv4 */
	DOCA_FLOW_L3_TYPE_IP6,
	/**< l3 type is ipv6 */
};

/**
 * @brief doca flow eth header
 */
struct doca_flow_header_eth {
	uint8_t src_mac[DOCA_FLOW_ETHER_ADDR_LEN];
	/**< source mac address */
	uint8_t dst_mac[DOCA_FLOW_ETHER_ADDR_LEN];
	/**< destination mac address */
	doca_be16_t type;
	/**< eth type */
};

/**
 * @brief doca flow ip address
 */
struct doca_flow_ip_addr {
	enum doca_flow_l3_type type;
	/**< ip address type */
	union {
		doca_be32_t ipv4_addr;
		/**< ipv4 address if type is ipv4 */
		doca_be32_t ipv6_addr[4];
		/**< ipv6 address if type is ipv6 */
	};
};

/**
 * @brief doca flow layer 4 packet extend type
 */
enum doca_flow_l4_type_ext {
	DOCA_FLOW_L4_TYPE_EXT_NONE = 0,
	/**< l4 ext type is not set */
	DOCA_FLOW_L4_TYPE_EXT_TCP,
	/**< l4 ext type is tcp */
	DOCA_FLOW_L4_TYPE_EXT_UDP,
	/**< l4 ext type is udp */
	DOCA_FLOW_L4_TYPE_EXT_ICMP,
	/**< l4 ext type is icmp */
	DOCA_FLOW_L4_TYPE_EXT_ICMP6,
	/**< l4 ext type is icmp6 */
	DOCA_FLOW_L4_TYPE_EXT_TRANSPORT,
	/**< l4 ext type is transport */
};

/**
 * @brief doca flow tcp or udp port header in match data
 */
struct doca_flow_header_l4_port {
	doca_be16_t src_port;
	/**< source port */
	doca_be16_t dst_port;
	/**< destination port */
};

/**
 * @brief doca flow udp header in match data
 */
struct doca_flow_header_udp {
	struct doca_flow_header_l4_port l4_port;
	/**< udp source and destination port */
};

/**
 * @brief doca flow tcp header in match data
 */
struct doca_flow_header_tcp {
	struct doca_flow_header_l4_port l4_port;
	/**< tcp source and destination port */
	uint8_t data_offset;
	/**< tcp data offset */
	uint8_t flags;
	/**< tcp flags */
};

/**
 * @brief doca flow icmp header in match data
 */
struct doca_flow_header_icmp {
	uint8_t type;
	/**< icmp type */
	uint8_t code;
	/**< icmp code. */
	doca_be16_t ident;
	/**< icmp identifier. */
};

/**
 * @brief doca flow IP fragment flags
 *
 * Those flags are used in 'flags_fragment_offset' ip4 field.
 */
enum doca_flow_ip_fragment_flags {
	DOCA_FLOW_IP4_FLAG_MORE_FRAGMENTS = (1 << 13),
	/**< Ipv4 More Fragments (MF) flag */
	DOCA_FLOW_IP4_FLAG_DONT_FRAGMENT = (1 << 14),
	/**< Ipv4 Don't Fragment (DF) flag */
};

/**
 * @brief doca flow ipv4 header in match data
 */
struct doca_flow_header_ip4 {
	doca_be32_t src_ip;
	/**< ip src address */
	doca_be32_t dst_ip;
	/**< ip dst address */
	uint8_t version_ihl;
	/**< version and internet header length */
	uint8_t dscp_ecn;
	/**< dscp and ecn */
	doca_be16_t total_len;
	/**< packet total length */
	doca_be16_t identification;
	/**< ip fragment identification */
	doca_be16_t flags_fragment_offset;
	/**< ip fragment flags (3) + ip fragment offset (13) */
	uint8_t next_proto;
	/**< ip next protocol */
	uint8_t ttl;
	/**< time to live */
};

/**
 * @brief doca flow ipv6 header in match data
 */
struct doca_flow_header_ip6 {
	doca_be32_t src_ip[4];
	/**< ip src address */
	doca_be32_t dst_ip[4];
	/**< ip dst address */
	uint8_t traffic_class;
	/**< traffic class */
	doca_be32_t flow_label;
	/**< reserved (12) + flow label (20) */
	doca_be16_t payload_len;
	/**< payload length */
	uint8_t next_proto;
	/**< ip next protocol */
	uint8_t hop_limit;
	/**< hop limit */
};

/**
 * doca flow CT ipV4
 */
struct doca_flow_ct_ip4 {
	doca_be32_t src_ip;
	/**< ip src address */
	doca_be32_t dst_ip;
	/**< ip dst address */
};

/**
 * doca flow CT ipV6
 */
struct doca_flow_ct_ip6 {
	doca_be32_t src_ip[4];
	/**< ip src address */
	doca_be32_t dst_ip[4];
	/**< ip dst address */
};

/**
 * @brief doca flow MPLS header.
 *
 * @code
 * 0                   1                   2                   3
 * 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                Label                  | TC  |S|      TTL      |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * @endcode
 *
 */
struct doca_flow_header_mpls {
	doca_be32_t label;
	/**< MPLS label. */
};

/**
 * @brief doca flow GENEVE header.
 */
struct doca_flow_header_geneve {
	uint8_t ver_opt_len;
	/**< version (2) + options length (6). */
	uint8_t o_c;
	/**< OAM packet (1) + critical options present (1) + reserved (6). */
	doca_be16_t next_proto;
	/**< next protocol */
	doca_be32_t vni;
	/**< geneve vni (24) + reserved (8). */
};

/**
 * @brief doca flow GENEVE option single DW.
 *
 * This object describes single DW (4-bytes) from GENEVE option header.
 * It describes either the first DW in the option including class, type and length
 * or any other data DW.
 */
union doca_flow_geneve_option {
	struct {
		doca_be16_t class_id;
		/**< option class */
		uint8_t type;
		/**< option type */
		uint8_t length;
		/**
		 * reserved (3) + option data length (5).
		 * The length is expressed in 4-byte multiples, excluding the option header.
		 * @see DOCA_FLOW_GENEVE_DATA_OPTION_LEN_MAX for maximal value.
		 */
	};
	/**< option header. */
	doca_be32_t data;
	/**< 4 bytes of option data. */
};

/**
 * @brief doca flow psp header in match data and modify/encap actions
 */
struct doca_flow_header_psp {
	uint8_t nexthdr;
	/**< next header IP protocol number */
	uint8_t hdrextlen;
	/**< header extension length */
	uint8_t res_cryptofst;
	/**< reserved:2, crypto offset:6 */
	uint8_t s_d_ver_v;
	/**< Sample bit, drop bit, version:4, V bit */
	doca_be32_t spi;
	/**< psp session parameter index */
	doca_be64_t iv;
	/**< psp initialization vector */
	doca_be64_t vc;
	/**< psp virtualization cookie */
};

/**
 * @brief doca flow tunnel type
 */
enum doca_flow_tun_type {
	DOCA_FLOW_TUN_NONE = 0,
	/**< tunnel is not set */
	DOCA_FLOW_TUN_VXLAN,
	/**< tunnel is vxlan type */
	DOCA_FLOW_TUN_GTPU,
	/**< tunnel is gtpu type */
	DOCA_FLOW_TUN_GRE,
	/**< tunnel is gre type */
	DOCA_FLOW_TUN_ESP,
	/**< tunnel is ipsec esp type */
	DOCA_FLOW_TUN_MPLS_O_UDP,
	/**< tunnel is mpls over udp type */
	DOCA_FLOW_TUN_GENEVE,
	/**< tunnel is geneve type */
	DOCA_FLOW_TUN_PSP,
	/**< tunnel is psp type */
	DOCA_FLOW_TUN_MAX,
	/**< tunnel is geneve type */
};

/**
 * @brief doca flow tunnel extension vxlan type
 */
enum doca_flow_tun_ext_vxlan_type {
	DOCA_FLOW_TUN_EXT_VXLAN_STANDARD = 0,
	/**< Vxlan tunnel extension is Standard */
	DOCA_FLOW_TUN_EXT_VXLAN_GPE,
	/**< Vxlan tunnel extension is GPE type */
	DOCA_FLOW_TUN_EXT_VXLAN_GBP,
	/**< Vxlan tunnel extension is GBP type */
};

/**
 * @brief doca flow tun extension gre type
 */
enum doca_flow_tun_ext_gre_type {
	DOCA_FLOW_TUN_EXT_GRE_STANDARD = 0,
	/**< GRE tunnel extension is STANDARD */
	DOCA_FLOW_TUN_EXT_GRE_NVGRE,
	/**< GRE tunnel extension is NVGRE */
};

/**
 * @brief doca flow tunnel information
 */
struct doca_flow_tun {
	enum doca_flow_tun_type type;
	/**< tunnel type */
	union {
		struct {
			enum doca_flow_tun_ext_vxlan_type vxlan_type;
			/**< vxlan ext-type, standard, gpe, gbp */
			union {
				uint8_t vxlan_next_protocol;
				/**< vxlan gpe next protocol */
				doca_be16_t vxlan_group_policy_id;
				/**< vxlan gbp group policy id */
			};
			doca_be32_t vxlan_tun_id;
			/**< vxlan vni(24) + reserved (8). */
		};
		/**< vxlan information if tunnel is vxlan */
		struct {
			enum doca_flow_tun_ext_gre_type gre_type;
			doca_be16_t protocol;
			/**< next protocol */
			union {
				struct {
					bool key_present;
					/**< gre key is present*/
					doca_be32_t gre_key;
					/**< gre key */
				};
				struct {
					doca_be32_t nvgre_vs_id;
					/**< nvgre virtual subnet id(24) + reserved (8). */
					uint8_t nvgre_flow_id;
					/**< nvgre flow id. */
				};
			};
		};
		/**< gre information if tunnel is gre */
		struct {
			doca_be32_t gtp_teid;
			/**< gtp teid */
		};
		/**< gtp information if tunnel is gtp */
		struct {
			doca_be32_t esp_spi;
			/**< ipsec session parameter index */
			doca_be32_t esp_sn;
			/**< ipsec sequence number */
		};
		/**< ipsec esp header information if tunnel is esp */
		struct {
			struct doca_flow_header_mpls mpls[DOCA_FLOW_MPLS_LABELS_MAX];
			/**< mpls labels */
		};
		/**< mpls information if tunnel is mpls over udp */
		struct {
			struct doca_flow_header_geneve geneve;
			/**< geneve header */
			union doca_flow_geneve_option geneve_options[DOCA_FLOW_GENEVE_OPT_LEN_MAX];
			/**< geneve options as array of doca_be32_t */
		};
		/**< geneve information if tunnel is geneve */
		struct {
			struct doca_flow_header_psp psp;
		};
		/**< psp header information if tunnel is psp */
	};
};

#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif /* DOCA_FLOW_NET_H_ */
