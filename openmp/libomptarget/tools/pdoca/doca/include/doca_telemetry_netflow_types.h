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

#ifndef DOCA_TELEMETRY_NETFLOW_TYPES_H_
#define DOCA_TELEMETRY_NETFLOW_TYPES_H_

/**
 * Incoming counter with length N x 8 bits for number of bytes associated with an IP Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_IN_BYTES 1
#define DOCA_TELEMETRY_NETFLOW_IN_BYTES_DEFAULT_LENGTH 4
/**
 * Incoming counter with length N x 8 bits for the number of packets associated with an IP Flow
 */
#define DOCA_TELEMETRY_NETFLOW_IN_PKTS 2
#define DOCA_TELEMETRY_NETFLOW_IN_PKTS_DEFAULT_LENGTH 4
/**
 * Number of flows that were aggregated; default for N is 4
 */
#define DOCA_TELEMETRY_NETFLOW_FLOWS 3
#define DOCA_TELEMETRY_NETFLOW_FLOWS_DEFAULT_LENGTH 4
/**
 * IP protocol byte
 */
#define DOCA_TELEMETRY_NETFLOW_PROTOCOL 4
#define DOCA_TELEMETRY_NETFLOW_PROTOCOL_DEFAULT_LENGTH 1
/**
 * Type of Service byte setting when entering incoming interface
 */
#define DOCA_TELEMETRY_NETFLOW_SRC_TOS 5
#define DOCA_TELEMETRY_NETFLOW_SRC_TOS_DEFAULT_LENGTH 1
/**
 * Cumulative of all the TCP flags seen for this flow
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_FLAGS 6
#define DOCA_TELEMETRY_NETFLOW_TCP_FLAGS_DEFAULT_LENGTH 1
/**
 * TCP/UDP source port number i.e.: FTP, Telnet, or equivalent
 */
#define DOCA_TELEMETRY_NETFLOW_L4_SRC_PORT 7
#define DOCA_TELEMETRY_NETFLOW_L4_SRC_PORT_DEFAULT_LENGTH 2
/**
 * IPv4 source address
 */
#define DOCA_TELEMETRY_NETFLOW_IPV4_SRC_ADDR 8
#define DOCA_TELEMETRY_NETFLOW_IPV4_SRC_ADDR_DEFAULT_LENGTH 4
/**
 * The number of contiguous bits in the source address subnet mask i.e.: the submask in slash
 * notation
 */
#define DOCA_TELEMETRY_NETFLOW_SRC_MASK 9
#define DOCA_TELEMETRY_NETFLOW_SRC_MASK_DEFAULT_LENGTH 1
/**
 * Input interface index; default for N is 2 but higher values could be used
 */
#define DOCA_TELEMETRY_NETFLOW_INPUT_SNMP 10
#define DOCA_TELEMETRY_NETFLOW_INPUT_SNMP_DEFAULT_LENGTH 2
/**
 * TCP/UDP destination port number i.e.: FTP, Telnet, or equivalent
 */
#define DOCA_TELEMETRY_NETFLOW_L4_DST_PORT 11
#define DOCA_TELEMETRY_NETFLOW_L4_DST_PORT_DEFAULT_LENGTH 2
/**
 * IPv4 destination address
 */
#define DOCA_TELEMETRY_NETFLOW_IPV4_DST_ADDR 12
#define DOCA_TELEMETRY_NETFLOW_IPV4_DST_ADDR_DEFAULT_LENGTH 4
/**
 * The number of contiguous bits in the destination address subnet mask i.e.: the submask in slash
 * notation
 */
#define DOCA_TELEMETRY_NETFLOW_DST_MASK 13
#define DOCA_TELEMETRY_NETFLOW_DST_MASK_DEFAULT_LENGTH 1
/**
 * Output interface index; default for N is 2 but higher values could be used
 */
#define DOCA_TELEMETRY_NETFLOW_OUTPUT_SNMP 14
#define DOCA_TELEMETRY_NETFLOW_OUTPUT_SNMP_DEFAULT_LENGTH 2
/**
 * IPv4 address of next-hop router
 */
#define DOCA_TELEMETRY_NETFLOW_IPV4_NEXT_HOP 15
#define DOCA_TELEMETRY_NETFLOW_IPV4_NEXT_HOP_DEFAULT_LENGTH 4
/**
 * Source BGP autonomous system number where N could be 2 or 4
 */
#define DOCA_TELEMETRY_NETFLOW_SRC_AS 16
#define DOCA_TELEMETRY_NETFLOW_SRC_AS_DEFAULT_LENGTH 2
/**
 * Destination BGP autonomous system number where N could be 2 or 4
 */
#define DOCA_TELEMETRY_NETFLOW_DST_AS 17
#define DOCA_TELEMETRY_NETFLOW_DST_AS_DEFAULT_LENGTH 2
/**
 * Next-hop router's IP in the BGP domain
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_IPV4_NEXT_HOP 18
#define DOCA_TELEMETRY_NETFLOW_BGP_IPV4_NEXT_HOP_DEFAULT_LENGTH 4
/**
 * IP multicast outgoing packet counter with length N x 8 bits for packets associated with the IP
 * Flow
 */
#define DOCA_TELEMETRY_NETFLOW_MUL_DST_PKTS 19
#define DOCA_TELEMETRY_NETFLOW_MUL_DST_PKTS_DEFAULT_LENGTH 4
/**
 * IP multicast outgoing byte counter with length N x 8 bits for bytes associated with the IP Flow
 */
#define DOCA_TELEMETRY_NETFLOW_MUL_DST_BYTES 20
#define DOCA_TELEMETRY_NETFLOW_MUL_DST_BYTES_DEFAULT_LENGTH 4
/**
 * System uptime at which the last packet of this flow was switched
 */
#define DOCA_TELEMETRY_NETFLOW_LAST_SWITCHED 21
#define DOCA_TELEMETRY_NETFLOW_LAST_SWITCHED_DEFAULT_LENGTH 4
/**
 * System uptime at which the first packet of this flow was switched
 */
#define DOCA_TELEMETRY_NETFLOW_FIRST_SWITCHED 22
#define DOCA_TELEMETRY_NETFLOW_FIRST_SWITCHED_DEFAULT_LENGTH 4
/**
 * Outgoing counter with length N x 8 bits for the number of bytes associated with an IP Flow
 */
#define DOCA_TELEMETRY_NETFLOW_OUT_BYTES 23
#define DOCA_TELEMETRY_NETFLOW_OUT_BYTES_DEFAULT_LENGTH 4
/**
 * Outgoing counter with length N x 8 bits for the number of packets associated with an IP Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_OUT_PKTS 24
#define DOCA_TELEMETRY_NETFLOW_OUT_PKTS_DEFAULT_LENGTH 4
/**
 * Minimum IP packet length on incoming packets of the flow
 */
#define DOCA_TELEMETRY_NETFLOW_MIN_PKT_LENGTH 25
#define DOCA_TELEMETRY_NETFLOW_MIN_PKT_LENGTH_DEFAULT_LENGTH 2
/**
 * Maximum IP packet length on incoming packets of the flow
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_PKT_LENGTH 26
#define DOCA_TELEMETRY_NETFLOW_MAX_PKT_LENGTH_DEFAULT_LENGTH 2
/**
 * IPv6 Source Address
 */
#define DOCA_TELEMETRY_NETFLOW_IPV6_SRC_ADDR 27
#define DOCA_TELEMETRY_NETFLOW_IPV6_SRC_ADDR_DEFAULT_LENGTH 16
/**
 * IPv6 Destination Address
 */
#define DOCA_TELEMETRY_NETFLOW_IPV6_DST_ADDR 28
#define DOCA_TELEMETRY_NETFLOW_IPV6_DST_ADDR_DEFAULT_LENGTH 16
/**
 * Length of the IPv6 source mask in contiguous bits
 */
#define DOCA_TELEMETRY_NETFLOW_IPV6_SRC_MASK 29
#define DOCA_TELEMETRY_NETFLOW_IPV6_SRC_MASK_DEFAULT_LENGTH 1
/**
 * Length of the IPv6 destination mask in contiguous bits
 */
#define DOCA_TELEMETRY_NETFLOW_IPV6_DST_MASK 30
#define DOCA_TELEMETRY_NETFLOW_IPV6_DST_MASK_DEFAULT_LENGTH 1
/**
 * IPv6 flow label as per RFC 2460 definition
 */
#define DOCA_TELEMETRY_NETFLOW_IPV6_FLOW_LABEL 31
#define DOCA_TELEMETRY_NETFLOW_IPV6_FLOW_LABEL_DEFAULT_LENGTH 3
/**
 * Internet Control Message Protocol (ICMP) packet type; reported as ((ICMP Type*256) + ICMP code)
 */
#define DOCA_TELEMETRY_NETFLOW_ICMP_TYPE 32
#define DOCA_TELEMETRY_NETFLOW_ICMP_TYPE_DEFAULT_LENGTH 2
/**
 * Internet Group Management Protocol (IGMP) packet type
 */
#define DOCA_TELEMETRY_NETFLOW_MUL_IGMP_TYPE 33
#define DOCA_TELEMETRY_NETFLOW_MUL_IGMP_TYPE_DEFAULT_LENGTH 1
/**
 * When using sampled NetFlow, the rate at which packets are sampled i.e.: a value of 100 indicates
 * that one of every 100 packets is sampled
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_INTERVAL 34
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_INTERVAL_DEFAULT_LENGTH 4
/**
 * The type of algorithm used for sampled NetFlow: 0x01 Deterministic Sampling ,0x02 Random Sampling
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_ALGORITHM 35
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_ALGORITHM_DEFAULT_LENGTH 1
/**
 * Timeout value (in seconds) for active flow entries in the NetFlow cache
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_ACTIVE_TIMEOUT 36
#define DOCA_TELEMETRY_NETFLOW_FLOW_ACTIVE_TIMEOUT_DEFAULT_LENGTH 2
/**
 * Timeout value (in seconds) for inactive flow entries in the NetFlow cache
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_INACTIVE_TIMEOUT 37
#define DOCA_TELEMETRY_NETFLOW_FLOW_INACTIVE_TIMEOUT_DEFAULT_LENGTH 2
/**
 * Type of flow switching engine: RP = 0, VIP/Linecard = 1
 */
#define DOCA_TELEMETRY_NETFLOW_ENGINE_TYPE 38
#define DOCA_TELEMETRY_NETFLOW_ENGINE_TYPE_DEFAULT_LENGTH 1
/**
 * ID number of the flow switching engine
 */
#define DOCA_TELEMETRY_NETFLOW_ENGINE_ID 39
#define DOCA_TELEMETRY_NETFLOW_ENGINE_ID_DEFAULT_LENGTH 1
/**
 * Counter with length N x 8 bits for bytes for the number of bytes exported by the Observation
 * Domain
 */
#define DOCA_TELEMETRY_NETFLOW_TOTAL_BYTES_EXP 40
#define DOCA_TELEMETRY_NETFLOW_TOTAL_BYTES_EXP_DEFAULT_LENGTH 4
/**
 * Counter with length N x 8 bits for bytes for the number of packets exported by the Observation
 * Domain
 */
#define DOCA_TELEMETRY_NETFLOW_TOTAL_PKTS_EXP 41
#define DOCA_TELEMETRY_NETFLOW_TOTAL_PKTS_EXP_DEFAULT_LENGTH 4
/**
 * Counter with length N x 8 bits for bytes for the number of flows exported by the Observation
 * Domain
 */
#define DOCA_TELEMETRY_NETFLOW_TOTAL_FLOWS_EXP 42
#define DOCA_TELEMETRY_NETFLOW_TOTAL_FLOWS_EXP_DEFAULT_LENGTH 4
/**
 * IPv4 source address prefix (specific for Catalyst architecture)
 */
#define DOCA_TELEMETRY_NETFLOW_IPV4_SRC_PREFIX 44
#define DOCA_TELEMETRY_NETFLOW_IPV4_SRC_PREFIX_DEFAULT_LENGTH 4
/**
 * IPv4 destination address prefix (specific for Catalyst architecture)
 */
#define DOCA_TELEMETRY_NETFLOW_IPV4_DST_PREFIX 45
#define DOCA_TELEMETRY_NETFLOW_IPV4_DST_PREFIX_DEFAULT_LENGTH 4
/**
 * MPLS Top Label Type: 0x00 UNKNOWN 0x01 TE-MIDPT 0x02 ATOM 0x03 VPN 0x04 BGP 0x05 LDP
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_TYPE 46
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_TYPE_DEFAULT_LENGTH 1
/**
 * Forwarding Equivalent Class corresponding to the MPLS Top Label
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_IP_ADDR 47
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_IP_ADDR_DEFAULT_LENGTH 4
/**
 * Identifier shown in "show flow-sampler"
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLER_ID 48
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLER_ID_DEFAULT_LENGTH 1
/**
 * The type of algorithm used for sampling data: 0x02 random sampling. Use in connection with
 * FLOW_SAMPLER_MODE
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLER_MODE 49
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLER_MODE_DEFAULT_LENGTH 1
/**
 * Packet interval at which to sample. Use in connection with FLOW_SAMPLER_MODE
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLER_RANDOM_INTERVAL 50
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLER_RANDOM_INTERVAL_DEFAULT_LENGTH 4
/**
 * Minimum TTL on incoming packets of the flow
 */
#define DOCA_TELEMETRY_NETFLOW_MIN_TTL 52
#define DOCA_TELEMETRY_NETFLOW_MIN_TTL_DEFAULT_LENGTH 1
/**
 * Maximum TTL on incoming packets of the flow
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_TTL 53
#define DOCA_TELEMETRY_NETFLOW_MAX_TTL_DEFAULT_LENGTH 1
/**
 * The IP v4 identification field
 */
#define DOCA_TELEMETRY_NETFLOW_IPV4_IDENT 54
#define DOCA_TELEMETRY_NETFLOW_IPV4_IDENT_DEFAULT_LENGTH 2
/**
 * Type of Service byte setting when exiting outgoing interface
 */
#define DOCA_TELEMETRY_NETFLOW_DST_TOS 55
#define DOCA_TELEMETRY_NETFLOW_DST_TOS_DEFAULT_LENGTH 1
/**
 * Incoming source MAC address
 */
#define DOCA_TELEMETRY_NETFLOW_IN_SRC_MAC 56
#define DOCA_TELEMETRY_NETFLOW_IN_SRC_MAC_DEFAULT_LENGTH 6
/**
 * Outgoing destination MAC address
 */
#define DOCA_TELEMETRY_NETFLOW_OUT_DST_MAC 57
#define DOCA_TELEMETRY_NETFLOW_OUT_DST_MAC_DEFAULT_LENGTH 6
/**
 * Virtual LAN identifier associated with ingress interface
 */
#define DOCA_TELEMETRY_NETFLOW_SRC_VLAN 58
#define DOCA_TELEMETRY_NETFLOW_SRC_VLAN_DEFAULT_LENGTH 2
/**
 * Virtual LAN identifier associated with egress interface
 */
#define DOCA_TELEMETRY_NETFLOW_DST_VLAN 59
#define DOCA_TELEMETRY_NETFLOW_DST_VLAN_DEFAULT_LENGTH 2
/**
 * Internet Protocol Version Set to 4 for IPv4, set to 6 for IPv6. If not present in the template,
 * then version 4 is assumed.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_PROTOCOL_VERSION 60
#define DOCA_TELEMETRY_NETFLOW_IP_PROTOCOL_VERSION_DEFAULT_LENGTH 1
/**
 * Flow direction: 0 - ingress flow, 1 - egress flow
 */
#define DOCA_TELEMETRY_NETFLOW_DIRECTION 61
#define DOCA_TELEMETRY_NETFLOW_DIRECTION_DEFAULT_LENGTH 1
/**
 * IPv6 address of the next-hop router
 */
#define DOCA_TELEMETRY_NETFLOW_IPV6_NEXT_HOP 62
#define DOCA_TELEMETRY_NETFLOW_IPV6_NEXT_HOP_DEFAULT_LENGTH 16
/**
 * Next-hop router in the BGP domain
 */
#define DOCA_TELEMETRY_NETFLOW_BPG_IPV6_NEXT_HOP 63
#define DOCA_TELEMETRY_NETFLOW_BPG_IPV6_NEXT_HOP_DEFAULT_LENGTH 16
/**
 * Bit-encoded field identifying IPv6 option headers found in the flow
 */
#define DOCA_TELEMETRY_NETFLOW_IPV6_OPTION_HEADERS 64
#define DOCA_TELEMETRY_NETFLOW_IPV6_OPTION_HEADERS_DEFAULT_LENGTH 4
/**
 * MPLS label at position 1 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_1 70
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_1_DEFAULT_LENGTH 3
/**
 * MPLS label at position 2 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_2 71
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_2_DEFAULT_LENGTH 3
/**
 * MPLS label at position 3 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_3 72
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_3_DEFAULT_LENGTH 3
/**
 * MPLS label at position 4 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_4 73
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_4_DEFAULT_LENGTH 3
/**
 * MPLS label at position 5 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_5 74
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_5_DEFAULT_LENGTH 3
/**
 * MPLS label at position 6 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_6 75
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_6_DEFAULT_LENGTH 3
/**
 * MPLS label at position 7 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_7 76
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_7_DEFAULT_LENGTH 3
/**
 * MPLS label at position 8 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_8 77
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_8_DEFAULT_LENGTH 3
/**
 * MPLS label at position 9 in the stack. This comprises 20 bits of MPLS label, 3 EXP (experimental)
 * bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_9 78
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_9_DEFAULT_LENGTH 3
/**
 * MPLS label at position 10 in the stack. This comprises 20 bits of MPLS label, 3 EXP
 * (experimental) bits and 1 S (end-of-stack) bit.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_10 79
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_10_DEFAULT_LENGTH 3
/**
 * Incoming destination MAC address
 */
#define DOCA_TELEMETRY_NETFLOW_IN_DST_MAC 80
#define DOCA_TELEMETRY_NETFLOW_IN_DST_MAC_DEFAULT_LENGTH 6
/**
 * Outgoing source MAC address
 */
#define DOCA_TELEMETRY_NETFLOW_OUT_SRC_MAC 81
#define DOCA_TELEMETRY_NETFLOW_OUT_SRC_MAC_DEFAULT_LENGTH 6
/**
 * Shortened interface name i.e.: "FE1/0"
 */
#define DOCA_TELEMETRY_NETFLOW_IF_NAME 82
#define DOCA_TELEMETRY_NETFLOW_IF_NAME_DEFAULT_LENGTH 64
/**
 * Full interface name i.e.: "'FastEthernet 1/0"
 */
#define DOCA_TELEMETRY_NETFLOW_IF_DESC 83
#define DOCA_TELEMETRY_NETFLOW_IF_DESC_DEFAULT_LENGTH 64
/**
 * Name of the flow sampler
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLER_NAME 84
#define DOCA_TELEMETRY_NETFLOW_SAMPLER_NAME_DEFAULT_LENGTH 64
/**
 * Running byte counter for a permanent flow
 */
#define DOCA_TELEMETRY_NETFLOW_IN_PERMANENT_BYTES 85
#define DOCA_TELEMETRY_NETFLOW_IN_PERMANENT_BYTES_DEFAULT_LENGTH 4
/**
 * Running packet counter for a permanent flow
 */
#define DOCA_TELEMETRY_NETFLOW_IN_PERMANENT_PKTS 86
#define DOCA_TELEMETRY_NETFLOW_IN_PERMANENT_PKTS_DEFAULT_LENGTH 4
/**
 * The fragment-offset value from fragmented IP packets
 */
#define DOCA_TELEMETRY_NETFLOW_FRAGMENT_OFFSET 88
#define DOCA_TELEMETRY_NETFLOW_FRAGMENT_OFFSET_DEFAULT_LENGTH 2
/**
 * Forwarding status is encoded on 1 byte with the 2 left bits giving the status and the 6 remaining
 * bits giving the reason code.
 */
#define DOCA_TELEMETRY_NETFLOW_FORWARDING_STATUS 89
#define DOCA_TELEMETRY_NETFLOW_FORWARDING_STATUS_DEFAULT_LENGTH 1
/**
 * MPLS PAL Route Distinguisher.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_PAL_RD 90
#define DOCA_TELEMETRY_NETFLOW_MPLS_PAL_RD_DEFAULT_LENGTH 8
/**
 * Number of consecutive bits in the MPLS prefix length.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_PREFIX_LEN 91
#define DOCA_TELEMETRY_NETFLOW_MPLS_PREFIX_LEN_DEFAULT_LENGTH 1
/**
 * BGP Policy Accounting Source Traffic Index
 */
#define DOCA_TELEMETRY_NETFLOW_SRC_TRAFFIC_INDEX 92
#define DOCA_TELEMETRY_NETFLOW_SRC_TRAFFIC_INDEX_DEFAULT_LENGTH 4
/**
 * BGP Policy Accounting Destination Traffic Index
 */
#define DOCA_TELEMETRY_NETFLOW_DST_TRAFFIC_INDEX 93
#define DOCA_TELEMETRY_NETFLOW_DST_TRAFFIC_INDEX_DEFAULT_LENGTH 4
/**
 * Application description.
 */
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_DESCRIPTION 94
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_DESCRIPTION_DEFAULT_LENGTH 64
/**
 * 8 bits of engine ID, followed by n bits of classification.
 */
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_TAG 95
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_TAG_DEFAULT_LENGTH 3
/**
 * Name associated with a classification.
 */
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_NAME 96
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_NAME_DEFAULT_LENGTH 64
/**
 * The value of a Differentiated Services Code Point (DSCP) encoded in the Differentiated Services
 * Field, after modification.
 */
#define DOCA_TELEMETRY_NETFLOW_POSTIP_DIFF_SERV_CODE_POINT 98
#define DOCA_TELEMETRY_NETFLOW_POSTIP_DIFF_SERV_CODE_POINT_DEFAULT_LENGTH 1
/**
 * Multicast replication factor.
 */
#define DOCA_TELEMETRY_NETFLOW_REPLICATION_FACTOR 99
#define DOCA_TELEMETRY_NETFLOW_REPLICATION_FACTOR_DEFAULT_LENGTH 4
/**
 * Layer 2 packet section offset. Potentially a generic offset.
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER_2_PACKET_SECTION_OFFSET 102
#define DOCA_TELEMETRY_NETFLOW_LAYER_2_PACKET_SECTION_OFFSET_DEFAULT_LENGTH 2
/**
 * Layer 2 packet section size. Potentially a generic size.
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER_2_PACKET_SECTION_SIZE 103
#define DOCA_TELEMETRY_NETFLOW_LAYER_2_PACKET_SECTION_SIZE_DEFAULT_LENGTH 2
/**
 * Layer 2 packet section data.
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER_2_PACKET_SECTION_DATA 104
#define DOCA_TELEMETRY_NETFLOW_LAYER_2_PACKET_SECTION_DATA_DEFAULT_LENGTH 3
/**
 * The autonomous system (AS) number of the first AS in the AS path to the destination IP address.
 * The path is deduced by looking up the destination IP address of the Flow in the BGP routing
 * information base. If AS path information for this Flow is only available as an unordered AS set
 * (and not as an ordered AS sequence), then the value of this Information Element is 0.
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_NEXT_ADJACENT_AS_NUMBER 128
#define DOCA_TELEMETRY_NETFLOW_BGP_NEXT_ADJACENT_AS_NUMBER_DEFAULT_LENGTH 4
/**
 * The autonomous system (AS) number of the last AS in the AS path from the source IP address. The
 * path is deduced by looking up the source IP address of the Flow in the BGP routing information
 * base. If AS path information for this Flow is only available as an unordered AS set (and not as
 * an ordered AS sequence), then the value of this Information Element is 0. In case of BGP
 * asymmetry, the bgpPrevAdjacentAsNumber might not be able to report the correct value.
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_PREV_ADJACENT_AS_NUMBER 129
#define DOCA_TELEMETRY_NETFLOW_BGP_PREV_ADJACENT_AS_NUMBER_DEFAULT_LENGTH 4
/**
 * The IPv4 address used by the Exporting Process. This is used by the Collector to identify the
 * Exporter in cases where the identity of the Exporter may have been obscured by the use of a
 * proxy.
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORTER_IPV4_ADDRESS 130
#define DOCA_TELEMETRY_NETFLOW_EXPORTER_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * The IPv6 address used by the Exporting Process. This is used by the Collector to identify the
 * Exporter in cases where the identity of the Exporter may have been obscured by the use of a
 * proxy.
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORTER_IPV6_ADDRESS 131
#define DOCA_TELEMETRY_NETFLOW_EXPORTER_IPV6_ADDRESS_DEFAULT_LENGTH 8
/**
 * The number of octets since the previous report (if any) in packets of this Flow dropped by packet
 * treatment. The number of octets includes IP header(s) and IP payload.
 */
#define DOCA_TELEMETRY_NETFLOW_DROPPED_OCTET_DELTA_COUNT 132
#define DOCA_TELEMETRY_NETFLOW_DROPPED_OCTET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The number of packets since the previous report (if any) of this Flow dropped by packet
 * treatment.
 */
#define DOCA_TELEMETRY_NETFLOW_DROPPED_PACKET_DELTA_COUNT 133
#define DOCA_TELEMETRY_NETFLOW_DROPPED_PACKET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of octets in packets of this Flow dropped by packet treatment since the Metering
 * Process (re-)initialization for this Observation Point. The number of octets includes IP
 * header(s) and IP payload.
 */
#define DOCA_TELEMETRY_NETFLOW_DROPPED_OCTET_TOTAL_COUNT 134
#define DOCA_TELEMETRY_NETFLOW_DROPPED_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The number of packets of this Flow dropped by packet treatment since the Metering Process
 * (re-)initialization for this Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_DROPPED_PACKET_TOTAL_COUNT 135
#define DOCA_TELEMETRY_NETFLOW_DROPPED_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The reason for Flow termination. Values are listed in the flowEndReason registry. See
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-flow-end-reason].
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_REASON 136
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_REASON_DEFAULT_LENGTH 1
/**
 * An identifier of a set of common properties that is unique per Observation Domain and Transport
 * Session. Typically, this Information Element is used to link to information reported in separate
 * Data Records.
 */
#define DOCA_TELEMETRY_NETFLOW_COMMON_PROPERTIES_ID 137
#define DOCA_TELEMETRY_NETFLOW_COMMON_PROPERTIES_ID_DEFAULT_LENGTH 8
/**
 * An identifier of an Observation Point that is unique per Observation Domain. It is RECOMMENDED
 * that this identifier is also unique per IPFIX Device. Typically, this Information Element is used
 * for limiting the scope of other Information Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_POINT_ID 138
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_POINT_ID_DEFAULT_LENGTH 8
/**
 * Type and Code of the IPv6 ICMP message. The combination of both values is reported as (ICMP type
 * * 256) + ICMP code.
 */
#define DOCA_TELEMETRY_NETFLOW_ICMP_TYPE_CODE_IPV6 139
#define DOCA_TELEMETRY_NETFLOW_ICMP_TYPE_CODE_IPV6_DEFAULT_LENGTH 2
/**
 * The IPv6 address of the system that the MPLS top label will cause this Flow to be forwarded to.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_IPV6_ADDRESS 140
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_IPV6_ADDRESS_DEFAULT_LENGTH 8
/**
 * An identifier of a line card that is unique per IPFIX Device hosting an Observation Point.
 * Typically, this Information Element is used for limiting the scope of other Information Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_LINE_CARD_ID 141
#define DOCA_TELEMETRY_NETFLOW_LINE_CARD_ID_DEFAULT_LENGTH 4
/**
 * An identifier of a line port that is unique per IPFIX Device hosting an Observation Point.
 * Typically, this Information Element is used for limiting the scope of other Information Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_PORT_ID 142
#define DOCA_TELEMETRY_NETFLOW_PORT_ID_DEFAULT_LENGTH 4
/**
 * An identifier of a Metering Process that is unique per IPFIX Device. Typically, this Information
 * Element is used for limiting the scope of other Information Elements. Note that process
 * identifiers are typically assigned dynamically. The Metering Process may be re-started with a
 * different ID.
 */
#define DOCA_TELEMETRY_NETFLOW_METERING_PROCESS_ID 143
#define DOCA_TELEMETRY_NETFLOW_METERING_PROCESS_ID_DEFAULT_LENGTH 4
/**
 * An identifier of an Exporting Process that is unique per IPFIX Device. Typically, this
 * Information Element is used for limiting the scope of other Information Elements. Note that
 * process identifiers are typically assigned dynamically. The Exporting Process may be re-started
 * with a different ID.
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORTING_PROCESS_ID 144
#define DOCA_TELEMETRY_NETFLOW_EXPORTING_PROCESS_ID_DEFAULT_LENGTH 4
/**
 * An identifier of a Template that is locally unique within a combination of a Transport session
 * and an Observation Domain.
 */
#define DOCA_TELEMETRY_NETFLOW_TEMPLATE_ID 145
#define DOCA_TELEMETRY_NETFLOW_TEMPLATE_ID_DEFAULT_LENGTH 2
/**
 * The identifier of the 802.11 (Wi-Fi) channel used.
 */
#define DOCA_TELEMETRY_NETFLOW_WLAN_CHANNEL_ID 146
#define DOCA_TELEMETRY_NETFLOW_WLAN_CHANNEL_ID_DEFAULT_LENGTH 1
/**
 * The Service Set IDentifier (SSID) identifying an 802.11 (Wi-Fi) network used. According to
 * IEEE.802-11.1999, the SSID is encoded into a string of up to 32 characters.
 */
#define DOCA_TELEMETRY_NETFLOW_WLAN_SSID 147
#define DOCA_TELEMETRY_NETFLOW_WLAN_SSID_DEFAULT_LENGTH 64
/**
 * An identifier of a Flow that is unique within an Observation Domain. This Information Element can
 * be used to distinguish between different Flows if Flow Keys such as IP addresses and port numbers
 * are not reported or are reported in separate records.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_ID 148
#define DOCA_TELEMETRY_NETFLOW_FLOW_ID_DEFAULT_LENGTH 8
/**
 * An identifier of an Observation Domain that is locally unique to an Exporting Process. The
 * Exporting Process uses the Observation Domain ID to uniquely identify to the Collecting Process
 * the Observation Domain where Flows were metered. It is RECOMMENDED that this identifier is also
 * unique per IPFIX Device.
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_DOMAIN_ID 149
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_DOMAIN_ID_DEFAULT_LENGTH 4
/**
 * The absolute timestamp of the first packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_SECONDS 150
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_SECONDS_DEFAULT_LENGTH 4
/**
 * The absolute timestamp of the last packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_SECONDS 151
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_SECONDS_DEFAULT_LENGTH 4
/**
 * The absolute timestamp of the first packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_MILLISECONDS 152
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_MILLISECONDS_DEFAULT_LENGTH 8
/**
 * The absolute timestamp of the last packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_MILLISECONDS 153
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_MILLISECONDS_DEFAULT_LENGTH 8
/**
 * The absolute timestamp of the first packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_MICROSECONDS 154
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_MICROSECONDS_DEFAULT_LENGTH 8
/**
 * The absolute timestamp of the last packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_MICROSECONDS 155
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_MICROSECONDS_DEFAULT_LENGTH 8
/**
 * The absolute timestamp of the first packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_NANOSECONDS 156
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_NANOSECONDS_DEFAULT_LENGTH 8
/**
 * The absolute timestamp of the last packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_NANOSECONDS 157
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_NANOSECONDS_DEFAULT_LENGTH 8
/**
 * This is a relative timestamp only valid within the scope of a single IPFIX Message. It contains
 * the negative time offset of the first observed packet of this Flow relative to the export time
 * specified in the IPFIX Message Header.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_DELTA_MICROSECONDS 158
#define DOCA_TELEMETRY_NETFLOW_FLOW_START_DELTA_MICROSECONDS_DEFAULT_LENGTH 4
/**
 * This is a relative timestamp only valid within the scope of a single IPFIX Message. It contains
 * the negative time offset of the last observed packet of this Flow relative to the export time
 * specified in the IPFIX Message Header.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_DELTA_MICROSECONDS 159
#define DOCA_TELEMETRY_NETFLOW_FLOW_END_DELTA_MICROSECONDS_DEFAULT_LENGTH 4
/**
 * The absolute timestamp of the last (re-)initialization of the IPFIX Device.
 */
#define DOCA_TELEMETRY_NETFLOW_SYSTEM_INIT_TIME_MILLISECONDS 160
#define DOCA_TELEMETRY_NETFLOW_SYSTEM_INIT_TIME_MILLISECONDS_DEFAULT_LENGTH 8
/**
 * The difference in time between the first observed packet of this Flow and the last observed
 * packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_DURATION_MILLISECONDS 161
#define DOCA_TELEMETRY_NETFLOW_FLOW_DURATION_MILLISECONDS_DEFAULT_LENGTH 4
/**
 * The difference in time between the first observed packet of this Flow and the last observed
 * packet of this Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_DURATION_MICROSECONDS 162
#define DOCA_TELEMETRY_NETFLOW_FLOW_DURATION_MICROSECONDS_DEFAULT_LENGTH 4
/**
 * The total number of Flows observed in the Observation Domain since the Metering Process
 * (re-)initialization for this Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVED_FLOW_TOTAL_COUNT 163
#define DOCA_TELEMETRY_NETFLOW_OBSERVED_FLOW_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of observed IP packets that the Metering Process did not process since the
 * (re-)initialization of the Metering Process.
 */
#define DOCA_TELEMETRY_NETFLOW_IGNORED_PACKET_TOTAL_COUNT 164
#define DOCA_TELEMETRY_NETFLOW_IGNORED_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of octets in observed IP packets (including the IP header) that the Metering
 * Process did not process since the (re-)initialization of the Metering Process.
 */
#define DOCA_TELEMETRY_NETFLOW_IGNORED_OCTET_TOTAL_COUNT 165
#define DOCA_TELEMETRY_NETFLOW_IGNORED_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of Flow Records that were generated by the Metering Process and dropped by the
 * Metering Process or by the Exporting Process instead of being sent to the Collecting Process.
 * There are several potential reasons for this including resource shortage and special Flow export
 * policies.
 */
#define DOCA_TELEMETRY_NETFLOW_NOT_SENT_FLOW_TOTAL_COUNT 166
#define DOCA_TELEMETRY_NETFLOW_NOT_SENT_FLOW_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of packets in Flow Records that were generated by the Metering Process and
 * dropped by the Metering Process or by the Exporting Process instead of being sent to the
 * Collecting Process. There are several potential reasons for this including resource shortage and
 * special Flow export policies.
 */
#define DOCA_TELEMETRY_NETFLOW_NOT_SENT_PACKET_TOTAL_COUNT 167
#define DOCA_TELEMETRY_NETFLOW_NOT_SENT_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of octets in packets in Flow Records that were generated by the Metering Process
 * and dropped by the Metering Process or by the Exporting Process instead of being sent to the
 * Collecting Process. There are several potential reasons for this including resource shortage and
 * special Flow export policies.
 */
#define DOCA_TELEMETRY_NETFLOW_NOT_SENT_OCTET_TOTAL_COUNT 168
#define DOCA_TELEMETRY_NETFLOW_NOT_SENT_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * IPv6 destination address prefix.
 */
#define DOCA_TELEMETRY_NETFLOW_DESTINATION_IPV6_PREFIX 169
#define DOCA_TELEMETRY_NETFLOW_DESTINATION_IPV6_PREFIX_DEFAULT_LENGTH 8
/**
 * IPv6 source address prefix.
 */
#define DOCA_TELEMETRY_NETFLOW_SOURCE_IPV6_PREFIX 170
#define DOCA_TELEMETRY_NETFLOW_SOURCE_IPV6_PREFIX_DEFAULT_LENGTH 8
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'octetTotalCount', except that it reports a potentially modified value caused by a middlebox
 * function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_OCTET_TOTAL_COUNT 171
#define DOCA_TELEMETRY_NETFLOW_POST_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'packetTotalCount', except that it reports a potentially modified value caused by a middlebox
 * function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_PACKET_TOTAL_COUNT 172
#define DOCA_TELEMETRY_NETFLOW_POST_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * This set of bit fields is used for marking the Information Elements of a Data Record that serve
 * as Flow Key. Each bit represents an Information Element in the Data Record, with the n-th least
 * significant bit representing the n-th Information Element. A bit set to value 1 indicates that
 * the corresponding Information Element is a Flow Key of the reported Flow. A bit set to value 0
 * indicates that this is not the case.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_KEY_INDICATOR 173
#define DOCA_TELEMETRY_NETFLOW_FLOW_KEY_INDICATOR_DEFAULT_LENGTH 8
/**
 * The total number of outgoing multicast packets sent for packets of this Flow by a multicast
 * daemon within the Observation Domain since the Metering Process (re-)initialization. This
 * property cannot necessarily be observed at the Observation Point, but may be retrieved by other
 * means.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_MCAST_PACKET_TOTAL_COUNT 174
#define DOCA_TELEMETRY_NETFLOW_POST_MCAST_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of octets in outgoing multicast packets sent for packets of this Flow by a
 * multicast daemon in the Observation Domain since the Metering Process (re-)initialization. This
 * property cannot necessarily be observed at the Observation Point, but may be retrieved by other
 * means. The number of octets includes IP header(s) and IP payload.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_MCAST_OCTET_TOTAL_COUNT 175
#define DOCA_TELEMETRY_NETFLOW_POST_MCAST_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * Type of the IPv4 ICMP message.
 */
#define DOCA_TELEMETRY_NETFLOW_ICMP_TYPE_IPV4 176
#define DOCA_TELEMETRY_NETFLOW_ICMP_TYPE_IPV4_DEFAULT_LENGTH 1
/**
 * Code of the IPv4 ICMP message.
 */
#define DOCA_TELEMETRY_NETFLOW_ICMP_CODE_IPV4 177
#define DOCA_TELEMETRY_NETFLOW_ICMP_CODE_IPV4_DEFAULT_LENGTH 1
/**
 * Type of the IPv6 ICMP message.
 */
#define DOCA_TELEMETRY_NETFLOW_ICMP_TYPE_IPV6 178
#define DOCA_TELEMETRY_NETFLOW_ICMP_TYPE_IPV6_DEFAULT_LENGTH 1
/**
 * Code of the IPv6 ICMP message.
 */
#define DOCA_TELEMETRY_NETFLOW_ICMP_CODE_IPV6 179
#define DOCA_TELEMETRY_NETFLOW_ICMP_CODE_IPV6_DEFAULT_LENGTH 1
/**
 * The source port identifier in the UDP header.
 */
#define DOCA_TELEMETRY_NETFLOW_UDP_SOURCE_PORT 180
#define DOCA_TELEMETRY_NETFLOW_UDP_SOURCE_PORT_DEFAULT_LENGTH 2
/**
 * The destination port identifier in the UDP header.
 */
#define DOCA_TELEMETRY_NETFLOW_UDP_DESTINATION_PORT 181
#define DOCA_TELEMETRY_NETFLOW_UDP_DESTINATION_PORT_DEFAULT_LENGTH 2
/**
 * The source port identifier in the TCP header.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_SOURCE_PORT 182
#define DOCA_TELEMETRY_NETFLOW_TCP_SOURCE_PORT_DEFAULT_LENGTH 2
/**
 * The destination port identifier in the TCP header.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_DESTINATION_PORT 183
#define DOCA_TELEMETRY_NETFLOW_TCP_DESTINATION_PORT_DEFAULT_LENGTH 2
/**
 * The sequence number in the TCP header.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_SEQUENCE_NUMBER 184
#define DOCA_TELEMETRY_NETFLOW_TCP_SEQUENCE_NUMBER_DEFAULT_LENGTH 4
/**
 * The acknowledgment number in the TCP header.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_ACKNOWLEDGMENT_NUMBER 185
#define DOCA_TELEMETRY_NETFLOW_TCP_ACKNOWLEDGMENT_NUMBER_DEFAULT_LENGTH 4
/**
 * The window field in the TCP header. If the TCP window scale is supported, then TCP window scale
 * must be known to fully interpret the value of this information.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_WINDOW_SIZE 186
#define DOCA_TELEMETRY_NETFLOW_TCP_WINDOW_SIZE_DEFAULT_LENGTH 2
/**
 * The urgent pointer in the TCP header.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_URGENT_POINTER 187
#define DOCA_TELEMETRY_NETFLOW_TCP_URGENT_POINTER_DEFAULT_LENGTH 2
/**
 * The length of the TCP header. Note that the value of this Information Element is different from
 * the value of the Data Offset field in the TCP header. The Data Offset field indicates the length
 * of the TCP header in units of 4 octets. This Information Elements specifies the length of the TCP
 * header in units of octets.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_HEADER_LENGTH 188
#define DOCA_TELEMETRY_NETFLOW_TCP_HEADER_LENGTH_DEFAULT_LENGTH 1
/**
 * The length of the IP header. For IPv6, the value of this Information Element is 40.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_HEADER_LENGTH 189
#define DOCA_TELEMETRY_NETFLOW_IP_HEADER_LENGTH_DEFAULT_LENGTH 1
/**
 * The total length of the IPv4 packet.
 */
#define DOCA_TELEMETRY_NETFLOW_TOTAL_LENGTH_IPV4 190
#define DOCA_TELEMETRY_NETFLOW_TOTAL_LENGTH_IPV4_DEFAULT_LENGTH 2
/**
 * This Information Element reports the value of the Payload Length field in the IPv6 header. Note
 * that IPv6 extension headers belong to the payload. Also note that in case of a jumbo payload
 * option the value of the Payload Length field in the IPv6 header is zero and so will be the value
 * reported by this Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_PAYLOAD_LENGTH_IPV6 191
#define DOCA_TELEMETRY_NETFLOW_PAYLOAD_LENGTH_IPV6_DEFAULT_LENGTH 2
/**
 * For IPv4, the value of the Information Element matches the value of the Time to Live (TTL) field
 * in the IPv4 packet header. For IPv6, the value of the Information Element matches the value of
 * the Hop Limit field in the IPv6 packet header.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_TTL 192
#define DOCA_TELEMETRY_NETFLOW_IP_TTL_DEFAULT_LENGTH 1
/**
 * The value of the Next Header field of the IPv6 header. The value identifies the type of the
 * following IPv6 extension header or of the following IP payload. Valid values are defined in the
 * IANA Protocol Numbers registry.
 */
#define DOCA_TELEMETRY_NETFLOW_NEXT_HEADER_IPV6 193
#define DOCA_TELEMETRY_NETFLOW_NEXT_HEADER_IPV6_DEFAULT_LENGTH 1
/**
 * The size of the MPLS packet without the label stack.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_PAYLOAD_LENGTH 194
#define DOCA_TELEMETRY_NETFLOW_MPLS_PAYLOAD_LENGTH_DEFAULT_LENGTH 4
/**
 * The value of a Differentiated Services Code Point (DSCP) encoded in the Differentiated Services
 * field. The Differentiated Services field spans the most significant 6 bits of the IPv4 TOS field
 * or the IPv6 Traffic Class field, respectively.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_DIFF_SERV_CODE_POINT 195
#define DOCA_TELEMETRY_NETFLOW_IP_DIFF_SERV_CODE_POINT_DEFAULT_LENGTH 1
/**
 * The value of the IP Precedence. The IP Precedence value is encoded in the first 3 bits of the
 * IPv4 TOS field or the IPv6 Traffic Class field, respectively.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_PRECEDENCE 196
#define DOCA_TELEMETRY_NETFLOW_IP_PRECEDENCE_DEFAULT_LENGTH 1
/**
 * Fragmentation properties indicated by flags in the IPv4 packet header or the IPv6 Fragment
 * header, respectively.
 */
#define DOCA_TELEMETRY_NETFLOW_FRAGMENT_FLAGS 197
#define DOCA_TELEMETRY_NETFLOW_FRAGMENT_FLAGS_DEFAULT_LENGTH 1
/**
 * The sum of the squared numbers of octets per incoming packet since the previous report (if any)
 * for this Flow at the Observation Point. The number of octets includes IP header(s) and IP
 * payload.
 */
#define DOCA_TELEMETRY_NETFLOW_OCTET_DELTA_SUM_OF_SQUARES 198
#define DOCA_TELEMETRY_NETFLOW_OCTET_DELTA_SUM_OF_SQUARES_DEFAULT_LENGTH 8
/**
 * The total sum of the squared numbers of octets in incoming packets for this Flow at the
 * Observation Point since the Metering Process (re-)initialization for this Observation Point. The
 * number of octets includes IP header(s) and IP payload.
 */
#define DOCA_TELEMETRY_NETFLOW_OCTET_TOTAL_SUM_OF_SQUARES 199
#define DOCA_TELEMETRY_NETFLOW_OCTET_TOTAL_SUM_OF_SQUARES_DEFAULT_LENGTH 8
/**
 * The TTL field from the top MPLS label stack entry, i.e., the last label that was pushed.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_TTL 200
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_TTL_DEFAULT_LENGTH 1
/**
 * The length of the MPLS label stack in units of octets.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_STACK_LENGTH 201
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_STACK_LENGTH_DEFAULT_LENGTH 4
/**
 * The number of labels in the MPLS label stack.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_STACK_DEPTH 202
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_STACK_DEPTH_DEFAULT_LENGTH 4
/**
 * The Exp field from the top MPLS label stack entry, i.e., the last label that was pushed.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_EXP 203
#define DOCA_TELEMETRY_NETFLOW_MPLS_TOP_LABEL_EXP_DEFAULT_LENGTH 1
/**
 * The effective length of the IP payload.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_PAYLOAD_LENGTH 204
#define DOCA_TELEMETRY_NETFLOW_IP_PAYLOAD_LENGTH_DEFAULT_LENGTH 4
/**
 * The value of the Length field in the UDP header.
 */
#define DOCA_TELEMETRY_NETFLOW_UDP_MESSAGE_LENGTH 205
#define DOCA_TELEMETRY_NETFLOW_UDP_MESSAGE_LENGTH_DEFAULT_LENGTH 2
/**
 * If the IP destination address is not a reserved multicast address, then the value of all bits of
 * the octet (including the reserved ones) is zero.
 */
#define DOCA_TELEMETRY_NETFLOW_IS_MULTICAST 206
#define DOCA_TELEMETRY_NETFLOW_IS_MULTICAST_DEFAULT_LENGTH 1
/**
 * The value of the Internet Header Length (IHL) field in the IPv4 header. It specifies the length
 * of the header in units of 4 octets. Please note that its unit is different from most of the other
 * Information Elements reporting length values.
 */
#define DOCA_TELEMETRY_NETFLOW_IPV4_IHL 207
#define DOCA_TELEMETRY_NETFLOW_IPV4_IHL_DEFAULT_LENGTH 1
/**
 * IPv4 options in packets of this Flow. The information is encoded in a set of bit fields. For each
 * valid IPv4 option type, there is a bit in this set. The bit is set to 1 if any observed packet of
 * this Flow contains the corresponding IPv4 option type. Otherwise, if no observed packet of this
 * Flow contained the respective IPv4 option type, the value of the corresponding bit is 0.
 */
#define DOCA_TELEMETRY_NETFLOW_IPV4_OPTIONS 208
#define DOCA_TELEMETRY_NETFLOW_IPV4_OPTIONS_DEFAULT_LENGTH 4
/**
 * TCP options in packets of this Flow. The information is encoded in a set of bit fields. For each
 * TCP option, there is a bit in this set. The bit is set to 1 if any observed packet of this Flow
 * contains the corresponding TCP option. Otherwise, if no observed packet of this Flow contained
 * the respective TCP option, the value of the corresponding bit is 0.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_OPTIONS 209
#define DOCA_TELEMETRY_NETFLOW_TCP_OPTIONS_DEFAULT_LENGTH 8
/**
 * The value of this Information Element is always a sequence of 0x00 values.
 */
#define DOCA_TELEMETRY_NETFLOW_PADDING_OCTETS 210
#define DOCA_TELEMETRY_NETFLOW_PADDING_OCTETS_DEFAULT_LENGTH 3
/**
 * An IPv4 address to which the Exporting Process sends Flow information.
 */
#define DOCA_TELEMETRY_NETFLOW_COLLECTOR_IPV4_ADDRESS 211
#define DOCA_TELEMETRY_NETFLOW_COLLECTOR_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * An IPv6 address to which the Exporting Process sends Flow information.
 */
#define DOCA_TELEMETRY_NETFLOW_COLLECTOR_IPV6_ADDRESS 212
#define DOCA_TELEMETRY_NETFLOW_COLLECTOR_IPV6_ADDRESS_DEFAULT_LENGTH 8
/**
 * The index of the interface from which IPFIX Messages sent by the Exporting Process to a Collector
 * leave the IPFIX Device. The value matches the value of managed object 'ifIndex' as defined in
 * [RFC2863]. Note that ifIndex values are not assigned statically to an interface and that the
 * interfaces may be renumbered every time the device's management system is re-initialized, as
 * specified in [RFC2863].
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORT_INTERFACE 213
#define DOCA_TELEMETRY_NETFLOW_EXPORT_INTERFACE_DEFAULT_LENGTH 4
/**
 * The protocol version used by the Exporting Process for sending Flow information. The protocol
 * version is given by the value of the Version Number field in the Message Header.
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORT_PROTOCOL_VERSION 214
#define DOCA_TELEMETRY_NETFLOW_EXPORT_PROTOCOL_VERSION_DEFAULT_LENGTH 1
/**
 * The value of the protocol number used by the Exporting Process for sending Flow information. The
 * protocol number identifies the IP packet payload type. Protocol numbers are defined in the IANA
 * Protocol Numbers registry.
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORT_TRANSPORT_PROTOCOL 215
#define DOCA_TELEMETRY_NETFLOW_EXPORT_TRANSPORT_PROTOCOL_DEFAULT_LENGTH 1
/**
 * The destination port identifier to which the Exporting Process sends Flow information. For the
 * transport protocols UDP, TCP, and SCTP, this is the destination port number. This field MAY also
 * be used for future transport protocols that have 16-bit source port identifiers.
 */
#define DOCA_TELEMETRY_NETFLOW_COLLECTOR_TRANSPORT_PORT 216
#define DOCA_TELEMETRY_NETFLOW_COLLECTOR_TRANSPORT_PORT_DEFAULT_LENGTH 2
/**
 * The source port identifier from which the Exporting Process sends Flow information. For the
 * transport protocols UDP, TCP, and SCTP, this is the source port number. This field MAY also be
 * used for future transport protocols that have 16-bit source port identifiers. This field may be
 * useful for distinguishing multiple Exporting Processes that use the same IP address.
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORTER_TRANSPORT_PORT 217
#define DOCA_TELEMETRY_NETFLOW_EXPORTER_TRANSPORT_PORT_DEFAULT_LENGTH 2
/**
 * The total number of packets of this Flow with TCP "Synchronize sequence numbers" (SYN) flag set.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_SYN_TOTAL_COUNT 218
#define DOCA_TELEMETRY_NETFLOW_TCP_SYN_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of packets of this Flow with TCP "No more data from sender" (FIN) flag set.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_FIN_TOTAL_COUNT 219
#define DOCA_TELEMETRY_NETFLOW_TCP_FIN_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of packets of this Flow with TCP "Reset the connection" (RST) flag set.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_RST_TOTAL_COUNT 220
#define DOCA_TELEMETRY_NETFLOW_TCP_RST_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of packets of this Flow with TCP "Push Function" (PSH) flag set.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_PSH_TOTAL_COUNT 221
#define DOCA_TELEMETRY_NETFLOW_TCP_PSH_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of packets of this Flow with TCP "Acknowledgment field significant" (ACK) flag
 * set.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_ACK_TOTAL_COUNT 222
#define DOCA_TELEMETRY_NETFLOW_TCP_ACK_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of packets of this Flow with TCP "Urgent Pointer field significant" (URG) flag
 * set.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_URG_TOTAL_COUNT 223
#define DOCA_TELEMETRY_NETFLOW_TCP_URG_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total length of the IP packet.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_TOTAL_LENGTH 224
#define DOCA_TELEMETRY_NETFLOW_IP_TOTAL_LENGTH_DEFAULT_LENGTH 8
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'sourceIPv4Address', except that it reports a modified value caused by a NAT middlebox function
 * after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_NAT_SOURCE_IPV4_ADDRESS 225
#define DOCA_TELEMETRY_NETFLOW_POST_NAT_SOURCE_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'destinationIPv4Address', except that it reports a modified value caused by a NAT middlebox
 * function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_NAT_DESTINATION_IPV4_ADDRESS 226
#define DOCA_TELEMETRY_NETFLOW_POST_NAT_DESTINATION_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'sourceTransportPort', except that it reports a modified value caused by a Network Address Port
 * Translation (NAPT) middlebox function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_NAPT_SOURCE_TRANSPORT_PORT 227
#define DOCA_TELEMETRY_NETFLOW_POST_NAPT_SOURCE_TRANSPORT_PORT_DEFAULT_LENGTH 2
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'destinationTransportPort', except that it reports a modified value caused by a Network Address
 * Port Translation (NAPT) middlebox function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_NAPT_DESTINATION_TRANSPORT_PORT 228
#define DOCA_TELEMETRY_NETFLOW_POST_NAPT_DESTINATION_TRANSPORT_PORT_DEFAULT_LENGTH 2
/**
 * Indicates whether the session was created because traffic originated in the private or public
 * address realm. postNATSourceIPv4Address, postNATDestinationIPv4Address,
 * postNAPTSourceTransportPort, and postNAPTDestinationTransportPort are qualified with the address
 * realm in perspective.
 */
#define DOCA_TELEMETRY_NETFLOW_NAT_ORIGINATING_ADDRESS_REALM 229
#define DOCA_TELEMETRY_NETFLOW_NAT_ORIGINATING_ADDRESS_REALM_DEFAULT_LENGTH 1
/**
 * This Information Element identifies a NAT event. This IE identifies the type of a NAT event.
 * Examples of NAT events include, but are not limited to, NAT translation create, NAT translation
 * delete, Threshold Reached, or Threshold Exceeded, etc. Values for this Information Element are
 * listed in the "NAT Event Type" registry, see
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-nat-event-type].
 */
#define DOCA_TELEMETRY_NETFLOW_NAT_EVENT 230
#define DOCA_TELEMETRY_NETFLOW_NAT_EVENT_DEFAULT_LENGTH 1
/**
 * The total number of layer 4 payload bytes in a flow from the initiator since the previous report.
 * The initiator is the device which triggered the session creation, and remains the same for the
 * life of the session.
 */
#define DOCA_TELEMETRY_NETFLOW_INITIATOR_OCTETS 231
#define DOCA_TELEMETRY_NETFLOW_INITIATOR_OCTETS_DEFAULT_LENGTH 8
/**
 * The total number of layer 4 payload bytes in a flow from the responder since the previous report.
 * The responder is the device which replies to the initiator, and remains the same for the life of
 * the session.
 */
#define DOCA_TELEMETRY_NETFLOW_RESPONDER_OCTETS 232
#define DOCA_TELEMETRY_NETFLOW_RESPONDER_OCTETS_DEFAULT_LENGTH 8
/**
 * Indicates a firewall event. Allowed values are listed in the firewallEvent registry. See
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-firewall-event].
 */
#define DOCA_TELEMETRY_NETFLOW_FIREWALL_EVENT 233
#define DOCA_TELEMETRY_NETFLOW_FIREWALL_EVENT_DEFAULT_LENGTH 1
/**
 * An unique identifier of the VRFname where the packets of this flow are being received. This
 * identifier is unique per Metering Process
 */
#define DOCA_TELEMETRY_NETFLOW_INGRESS_VRFID 234
#define DOCA_TELEMETRY_NETFLOW_INGRESS_VRFID_DEFAULT_LENGTH 4
/**
 * An unique identifier of the VRFname where the packets of this flow are being sent. This
 * identifier is unique per Metering Process
 */
#define DOCA_TELEMETRY_NETFLOW_EGRESS_VRFID 235
#define DOCA_TELEMETRY_NETFLOW_EGRESS_VRFID_DEFAULT_LENGTH 4
/**
 * The name of a VPN Routing and Forwarding table (VRF).
 */
#define DOCA_TELEMETRY_NETFLOW_VRF_NAME 236
#define DOCA_TELEMETRY_NETFLOW_VRF_NAME_DEFAULT_LENGTH 64
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'mplsTopLabelExp', except that it reports a potentially modified value caused by a middlebox
 * function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_MPLS_TOP_LABEL_EXP 237
#define DOCA_TELEMETRY_NETFLOW_POST_MPLS_TOP_LABEL_EXP_DEFAULT_LENGTH 1
/**
 * The scale of the window field in the TCP header.
 */
#define DOCA_TELEMETRY_NETFLOW_TCP_WINDOW_SCALE 238
#define DOCA_TELEMETRY_NETFLOW_TCP_WINDOW_SCALE_DEFAULT_LENGTH 2
/**
 * A description of the direction assignment method used to assign the Biflow Source and
 * Destination. This Information Element MAY be present in a Flow Data Record, or applied to all
 * flows exported from an Exporting Process or Observation Domain using IPFIX Options. If this
 * Information Element is not present in a Flow Record or associated with a Biflow via scope, it is
 * assumed that the configuration of the direction assignment method is done out-of-band. Note that
 * when using IPFIX Options to apply this Information Element to all flows within an Observation
 * Domain or from an Exporting Process, the Option SHOULD be sent reliably. If reliable transport is
 * not available (i.e., when using UDP), this Information Element SHOULD appear in each Flow Record.
 * Values are listed in the biflowDirection registry. See
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-biflow-direction].
 */
#define DOCA_TELEMETRY_NETFLOW_BIFLOW_DIRECTION 239
#define DOCA_TELEMETRY_NETFLOW_BIFLOW_DIRECTION_DEFAULT_LENGTH 1
/**
 * The difference between the length of an Ethernet frame (minus the FCS) and the length of its MAC
 * Client Data section (including any padding) as defined in section 3.1 of [IEEE.802-3.2005]. It
 * does not include the Preamble, SFD and Extension field lengths.
 */
#define DOCA_TELEMETRY_NETFLOW_ETHERNET_HEADER_LENGTH 240
#define DOCA_TELEMETRY_NETFLOW_ETHERNET_HEADER_LENGTH_DEFAULT_LENGTH 1
/**
 * The length of the MAC Client Data section (including any padding) of a frame as defined in
 * section 3.1 of [IEEE.802-3.2005].
 */
#define DOCA_TELEMETRY_NETFLOW_ETHERNET_PAYLOAD_LENGTH 241
#define DOCA_TELEMETRY_NETFLOW_ETHERNET_PAYLOAD_LENGTH_DEFAULT_LENGTH 2
/**
 * The total length of the Ethernet frame (excluding the Preamble, SFD, Extension and FCS fields) as
 * described in section 3.1 of [IEEE.802-3.2005].
 */
#define DOCA_TELEMETRY_NETFLOW_ETHERNET_TOTAL_LENGTH 242
#define DOCA_TELEMETRY_NETFLOW_ETHERNET_TOTAL_LENGTH_DEFAULT_LENGTH 2
/**
 * The value of the 12-bit VLAN Identifier portion of the Tag Control Information field of an
 * Ethernet frame. The structure and semantics within the Tag Control Information field are defined
 * in [IEEE802.1Q]. In Provider Bridged Networks, it represents the Service VLAN identifier in the
 * Service VLAN Tag (S-TAG) Tag Control Information (TCI) field or the Customer VLAN identifier in
 * the Customer VLAN Tag (C-TAG) Tag Control Information (TCI) field as described in [IEEE802.1Q].
 * In Provider Backbone Bridged Networks, it represents the Backbone VLAN identifier in the Backbone
 * VLAN Tag (B-TAG) Tag Control Information (TCI) field as described in [IEEE802.1Q]. In a virtual
 * link between a host system and EVB bridge, it represents the Service VLAN identifier indicating
 * S-channel as described in [IEEE802.1Qbg].
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_VLAN_ID 243
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_VLAN_ID_DEFAULT_LENGTH 2
/**
 * The value of the 3-bit User Priority portion of the Tag Control Information field of an Ethernet
 * frame. The structure and semantics within the Tag Control Information field are defined in
 * [IEEE802.1Q]. In the case of multi-tagged frame, it represents the 3-bit Priority Code Point
 * (PCP) portion of the outer tag's Tag Control Information (TCI) field as described in
 * [IEEE802.1Q], except for I-TAG.
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_PRIORITY 244
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_PRIORITY_DEFAULT_LENGTH 1
/**
 * The value represents the Customer VLAN identifier in the Customer VLAN Tag (C-TAG) Tag Control
 * Information (TCI) field as described in [IEEE802.1Q].
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_VLAN_ID 245
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_VLAN_ID_DEFAULT_LENGTH 2
/**
 * The value represents the 3-bit Priority Code Point (PCP) portion of the Customer VLAN Tag (C-TAG)
 * Tag Control Information (TCI) field as described in [IEEE802.1Q].
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_PRIORITY 246
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_PRIORITY_DEFAULT_LENGTH 1
/**
 * The EVC Service Attribute which uniquely identifies the Ethernet Virtual Connection (EVC) within
 * a Metro Ethernet Network, as defined in section 6.2 of MEF 10.1. The MetroEVCID is encoded in a
 * string of up to 100 characters.
 */
#define DOCA_TELEMETRY_NETFLOW_METRO_EVC_ID 247
#define DOCA_TELEMETRY_NETFLOW_METRO_EVC_ID_DEFAULT_LENGTH 64
/**
 * The 3-bit EVC Service Attribute which identifies the type of service provided by an EVC.
 */
#define DOCA_TELEMETRY_NETFLOW_METRO_EVC_TYPE 248
#define DOCA_TELEMETRY_NETFLOW_METRO_EVC_TYPE_DEFAULT_LENGTH 1
/**
 * A 32-bit non-zero connection identifier, which together with the pseudoWireType, identifies the
 * Pseudo Wire (PW) as defined in [RFC8077].
 */
#define DOCA_TELEMETRY_NETFLOW_PSEUDO_WIRE_ID 249
#define DOCA_TELEMETRY_NETFLOW_PSEUDO_WIRE_ID_DEFAULT_LENGTH 4
/**
 * The value of this information element identifies the type of MPLS Pseudo Wire (PW) as defined in
 * [RFC4446].
 */
#define DOCA_TELEMETRY_NETFLOW_PSEUDO_WIRE_TYPE 250
#define DOCA_TELEMETRY_NETFLOW_PSEUDO_WIRE_TYPE_DEFAULT_LENGTH 2
/**
 * The 32-bit Preferred Pseudo Wire (PW) MPLS Control Word as defined in Section 3 of [RFC4385].
 */
#define DOCA_TELEMETRY_NETFLOW_PSEUDO_WIRE_CONTROL_WORD 251
#define DOCA_TELEMETRY_NETFLOW_PSEUDO_WIRE_CONTROL_WORD_DEFAULT_LENGTH 4
/**
 * The index of a networking device's physical interface (example, a switch port) where packets of
 * this flow are being received.
 */
#define DOCA_TELEMETRY_NETFLOW_INGRESS_PHYSICAL_INTERFACE 252
#define DOCA_TELEMETRY_NETFLOW_INGRESS_PHYSICAL_INTERFACE_DEFAULT_LENGTH 4
/**
 * The index of a networking device's physical interface (example, a switch port) where packets of
 * this flow are being sent.
 */
#define DOCA_TELEMETRY_NETFLOW_EGRESS_PHYSICAL_INTERFACE 253
#define DOCA_TELEMETRY_NETFLOW_EGRESS_PHYSICAL_INTERFACE_DEFAULT_LENGTH 4
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'dot1qVlanId', except that it reports a potentially modified value caused by a middlebox function
 * after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_DOT1Q_VLAN_ID 254
#define DOCA_TELEMETRY_NETFLOW_POST_DOT1Q_VLAN_ID_DEFAULT_LENGTH 2
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'dot1qCustomerVlanId', except that it reports a potentially modified value caused by a middlebox
 * function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_DOT1Q_CUSTOMER_VLAN_ID 255
#define DOCA_TELEMETRY_NETFLOW_POST_DOT1Q_CUSTOMER_VLAN_ID_DEFAULT_LENGTH 2
/**
 * The Ethernet type field of an Ethernet frame that identifies the MAC client protocol carried in
 * the payload as defined in paragraph 1.4.349 of [IEEE.802-3.2005].
 */
#define DOCA_TELEMETRY_NETFLOW_ETHERNET_TYPE 256
#define DOCA_TELEMETRY_NETFLOW_ETHERNET_TYPE_DEFAULT_LENGTH 2
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'ipPrecedence', except that it reports a potentially modified value caused by a middlebox
 * function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_IP_PRECEDENCE 257
#define DOCA_TELEMETRY_NETFLOW_POST_IP_PRECEDENCE_DEFAULT_LENGTH 1
/**
 * The absolute timestamp at which the data within the scope containing this Information Element was
 * received by a Collecting Process. This Information Element SHOULD be bound to its containing
 * IPFIX Message via IPFIX Options and the messageScope Information Element, as defined below.
 */
#define DOCA_TELEMETRY_NETFLOW_COLLECTION_TIME_MILLISECONDS 258
#define DOCA_TELEMETRY_NETFLOW_COLLECTION_TIME_MILLISECONDS_DEFAULT_LENGTH 8
/**
 * The value of the SCTP Stream Identifier used by the Exporting Process for exporting IPFIX Message
 * data. This is carried in the Stream Identifier field of the header of the SCTP DATA chunk
 * containing the IPFIX Message(s).
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORT_SCTP_STREAM_ID 259
#define DOCA_TELEMETRY_NETFLOW_EXPORT_SCTP_STREAM_ID_DEFAULT_LENGTH 2
/**
 * The absolute Export Time of the latest IPFIX Message within the scope containing this Information
 * Element. This Information Element SHOULD be bound to its containing IPFIX Transport Session via
 * IPFIX Options and the sessionScope Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_EXPORT_SECONDS 260
#define DOCA_TELEMETRY_NETFLOW_MAX_EXPORT_SECONDS_DEFAULT_LENGTH 4
/**
 * The latest absolute timestamp of the last packet within any Flow within the scope containing this
 * Information Element, rounded up to the second if necessary. This Information Element SHOULD be
 * bound to its containing IPFIX Transport Session via IPFIX Options and the sessionScope
 * Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_FLOW_END_SECONDS 261
#define DOCA_TELEMETRY_NETFLOW_MAX_FLOW_END_SECONDS_DEFAULT_LENGTH 4
/**
 * The MD5 checksum of the IPFIX Message containing this record. This Information Element SHOULD be
 * bound to its containing IPFIX Message via an options record and the messageScope Information
 * Element, as defined below, and SHOULD appear only once in a given IPFIX Message. To calculate the
 * value of this Information Element, first buffer the containing IPFIX Message, setting the value
 * of this Information Element to all zeroes. Then calculate the MD5 checksum of the resulting
 * buffer as defined in [RFC1321], place the resulting value in this Information Element, and export
 * the buffered message. This Information Element is intended as a simple checksum only; therefore
 * collision resistance and algorithm agility are not required, and MD5 is an appropriate message
 * digest. This Information Element has a fixed length of 16 octets.
 */
#define DOCA_TELEMETRY_NETFLOW_MESSAGE_MD5_CHECKSUM 262
#define DOCA_TELEMETRY_NETFLOW_MESSAGE_MD5_CHECKSUM_DEFAULT_LENGTH 3
/**
 * The presence of this Information Element as scope in an Options Template signifies that the
 * options described by the Template apply to the IPFIX Message that contains them. It is defined
 * for general purpose message scoping of options, and proposed specifically to allow the attachment
 * a checksum to a message via IPFIX Options. The value of this Information Element MUST be written
 * as 0 by the File Writer or Exporting Process. The value of this Information Element MUST be
 * ignored by the File Reader or the Collecting Process.
 */
#define DOCA_TELEMETRY_NETFLOW_MESSAGE_SCOPE 263
#define DOCA_TELEMETRY_NETFLOW_MESSAGE_SCOPE_DEFAULT_LENGTH 1
/**
 * The absolute Export Time of the earliest IPFIX Message within the scope containing this
 * Information Element. This Information Element SHOULD be bound to its containing IPFIX Transport
 * Session via an options record and the sessionScope Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_MIN_EXPORT_SECONDS 264
#define DOCA_TELEMETRY_NETFLOW_MIN_EXPORT_SECONDS_DEFAULT_LENGTH 4
/**
 * The earliest absolute timestamp of the first packet within any Flow within the scope containing
 * this Information Element, rounded down to the second if necessary. This Information Element
 * SHOULD be bound to its containing IPFIX Transport Session via an options record and the
 * sessionScope Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_MIN_FLOW_START_SECONDS 265
#define DOCA_TELEMETRY_NETFLOW_MIN_FLOW_START_SECONDS_DEFAULT_LENGTH 4
/**
 * This Information Element is used to encapsulate non- IPFIX data into an IPFIX Message stream, for
 * the purpose of allowing a non-IPFIX data processor to store a data stream inline within an IPFIX
 * File. A Collecting Process or File Writer MUST NOT try to interpret this binary data. This
 * Information Element differs from paddingOctets as its contents are meaningful in some non-IPFIX
 * context, while the contents of paddingOctets MUST be 0x00 and are intended only for Information
 * Element alignment.
 */
#define DOCA_TELEMETRY_NETFLOW_OPAQUE_OCTETS 266
#define DOCA_TELEMETRY_NETFLOW_OPAQUE_OCTETS_DEFAULT_LENGTH 3
/**
 * The presence of this Information Element as scope in an Options Template signifies that the
 * options described by the Template apply to the IPFIX Transport Session that contains them. Note
 * that as all options are implicitly scoped to Transport Session and Observation Domain, this
 * Information Element is equivalent to a "null" scope. It is defined for general purpose session
 * scoping of options, and proposed specifically to allow the attachment of time window to an IPFIX
 * File via IPFIX Options. The value of this Information Element MUST be written as 0 by the File
 * Writer or Exporting Process. The value of this Information Element MUST be ignored by the File
 * Reader or the Collecting Process.
 */
#define DOCA_TELEMETRY_NETFLOW_SESSION_SCOPE 267
#define DOCA_TELEMETRY_NETFLOW_SESSION_SCOPE_DEFAULT_LENGTH 1
/**
 * The latest absolute timestamp of the last packet within any Flow within the scope containing this
 * Information Element, rounded up to the microsecond if necessary. This Information Element SHOULD
 * be bound to its containing IPFIX Transport Session via IPFIX Options and the sessionScope
 * Information Element. This Information Element SHOULD be used only in Transport Sessions
 * containing Flow Records with microsecond- precision (or better) timestamp Information Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_FLOW_END_MICROSECONDS 268
#define DOCA_TELEMETRY_NETFLOW_MAX_FLOW_END_MICROSECONDS_DEFAULT_LENGTH 8
/**
 * The latest absolute timestamp of the last packet within any Flow within the scope containing this
 * Information Element, rounded up to the millisecond if necessary. This Information Element SHOULD
 * be bound to its containing IPFIX Transport Session via IPFIX Options and the sessionScope
 * Information Element. This Information Element SHOULD be used only in Transport Sessions
 * containing Flow Records with millisecond- precision (or better) timestamp Information Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_FLOW_END_MILLISECONDS 269
#define DOCA_TELEMETRY_NETFLOW_MAX_FLOW_END_MILLISECONDS_DEFAULT_LENGTH 8
/**
 * The latest absolute timestamp of the last packet within any Flow within the scope containing this
 * Information Element. This Information Element SHOULD be bound to its containing IPFIX Transport
 * Session via IPFIX Options and the sessionScope Information Element. This Information Element
 * SHOULD be used only in Transport Sessions containing Flow Records with nanosecond-precision
 * timestamp Information Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_FLOW_END_NANOSECONDS 270
#define DOCA_TELEMETRY_NETFLOW_MAX_FLOW_END_NANOSECONDS_DEFAULT_LENGTH 8
/**
 * The earliest absolute timestamp of the first packet within any Flow within the scope containing
 * this Information Element, rounded down to the microsecond if necessary. This Information Element
 * SHOULD be bound to its containing IPFIX Transport Session via an options record and the
 * sessionScope Information Element. This Information Element SHOULD be used only in Transport
 * Sessions containing Flow Records with microsecond- precision (or better) timestamp Information
 * Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_MIN_FLOW_START_MICROSECONDS 271
#define DOCA_TELEMETRY_NETFLOW_MIN_FLOW_START_MICROSECONDS_DEFAULT_LENGTH 8
/**
 * The earliest absolute timestamp of the first packet within any Flow within the scope containing
 * this Information Element, rounded down to the millisecond if necessary. This Information Element
 * SHOULD be bound to its containing IPFIX Transport Session via an options record and the
 * sessionScope Information Element. This Information Element SHOULD be used only in Transport
 * Sessions containing Flow Records with millisecond- precision (or better) timestamp Information
 * Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_MIN_FLOW_START_MILLISECONDS 272
#define DOCA_TELEMETRY_NETFLOW_MIN_FLOW_START_MILLISECONDS_DEFAULT_LENGTH 8
/**
 * The earliest absolute timestamp of the first packet within any Flow within the scope containing
 * this Information Element. This Information Element SHOULD be bound to its containing IPFIX
 * Transport Session via an options record and the sessionScope Information Element. This
 * Information Element SHOULD be used only in Transport Sessions containing Flow Records with
 * nanosecond-precision timestamp Information Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_MIN_FLOW_START_NANOSECONDS 273
#define DOCA_TELEMETRY_NETFLOW_MIN_FLOW_START_NANOSECONDS_DEFAULT_LENGTH 8
/**
 * The full X.509 certificate, encoded in ASN.1 DER format, used by the Collector when IPFIX
 * Messages were transmitted using TLS or DTLS. This Information Element SHOULD be bound to its
 * containing IPFIX Transport Session via an options record and the sessionScope Information
 * Element, or to its containing IPFIX Message via an options record and the messageScope
 * Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_COLLECTOR_CERTIFICATE 274
#define DOCA_TELEMETRY_NETFLOW_COLLECTOR_CERTIFICATE_DEFAULT_LENGTH 3
/**
 * The full X.509 certificate, encoded in ASN.1 DER format, used by the Collector when IPFIX
 * Messages were transmitted using TLS or DTLS. This Information Element SHOULD be bound to its
 * containing IPFIX Transport Session via an options record and the sessionScope Information
 * Element, or to its containing IPFIX Message via an options record and the messageScope
 * Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_EXPORTER_CERTIFICATE 275
#define DOCA_TELEMETRY_NETFLOW_EXPORTER_CERTIFICATE_DEFAULT_LENGTH 3
/**
 * The export reliability of Data Records, within this SCTP stream, for the element(s) in the
 * Options Template scope. A typical example of an element for which the export reliability will be
 * reported is the templateID, as specified in the Data Records Reliability Options Template. A
 * value of 'True' means that the Exporting Process MUST send any Data Records associated with the
 * element(s) reliably within this SCTP stream. A value of 'False' means that the Exporting Process
 * MAY send any Data Records associated with the element(s) unreliably within this SCTP stream.
 */
#define DOCA_TELEMETRY_NETFLOW_DATA_RECORDS_RELIABILITY 276
#define DOCA_TELEMETRY_NETFLOW_DATA_RECORDS_RELIABILITY_DEFAULT_LENGTH 1
/**
 * Type of observation point. Values are listed in the observationPointType registry. See
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-observation-point-type].
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_POINT_TYPE 277
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_POINT_TYPE_DEFAULT_LENGTH 1
/**
 * This information element counts the number of TCP or UDP connections which were opened during the
 * observation period. The observation period may be specified by the flow start and end timestamps.
 */
#define DOCA_TELEMETRY_NETFLOW_NEW_CONNECTION_DELTA_COUNT 278
#define DOCA_TELEMETRY_NETFLOW_NEW_CONNECTION_DELTA_COUNT_DEFAULT_LENGTH 4
/**
 * This information element aggregates the total time in seconds for all of the TCP or UDP
 * connections which were in use during the observation period. For example if there are 5
 * concurrent connections each for 10 seconds, the value would be 50 s.
 */
#define DOCA_TELEMETRY_NETFLOW_CONNECTION_SUM_DURATION_SECONDS 279
#define DOCA_TELEMETRY_NETFLOW_CONNECTION_SUM_DURATION_SECONDS_DEFAULT_LENGTH 8
/**
 * This information element identifies a transaction within a connection. A transaction is a
 * meaningful exchange of application data between two network devices or a client and server. A
 * transactionId is assigned the first time a flow is reported, so that later reports for the same
 * flow will have the same transactionId. A different transactionId is used for each transaction
 * within a TCP or UDP connection. The identifiers need not be sequential.
 */
#define DOCA_TELEMETRY_NETFLOW_CONNECTION_TRANSACTION_ID 280
#define DOCA_TELEMETRY_NETFLOW_CONNECTION_TRANSACTION_ID_DEFAULT_LENGTH 8
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'sourceIPv6Address', except that it reports a modified value caused by a NAT64 middlebox function
 * after the packet passed the Observation Point. See [RFC8200] for the definition of the Source
 * Address field in the IPv6 header. See [RFC3234] for the definition of middleboxes. See [RFC6146]
 * for nat64 specification.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_NATSOURCE_IPV6_ADDRESS 281
#define DOCA_TELEMETRY_NETFLOW_POST_NATSOURCE_IPV6_ADDRESS_DEFAULT_LENGTH 8
/**
 * The definition of this Information Element is identical to the definition of Information Element
 * 'destinationIPv6Address', except that it reports a modified value caused by a NAT64 middlebox
 * function after the packet passed the Observation Point. See [RFC8200] for the definition of the
 * Destination Address field in the IPv6 header. See [RFC3234] for the definition of middleboxes.
 * See [RFC6146] for nat64 specification.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_NATDESTINATION_IPV6_ADDRESS 282
#define DOCA_TELEMETRY_NETFLOW_POST_NATDESTINATION_IPV6_ADDRESS_DEFAULT_LENGTH 8
/**
 * Locally unique identifier of a NAT pool.
 */
#define DOCA_TELEMETRY_NETFLOW_NAT_POOL_ID 283
#define DOCA_TELEMETRY_NETFLOW_NAT_POOL_ID_DEFAULT_LENGTH 4
/**
 * The name of a NAT pool identified by a natPoolID.
 */
#define DOCA_TELEMETRY_NETFLOW_NAT_POOL_NAME 284
#define DOCA_TELEMETRY_NETFLOW_NAT_POOL_NAME_DEFAULT_LENGTH 64
/**
 * A flag word describing specialized modifications to the anonymization policy in effect for the
 * anonymization technique applied to a referenced Information Element within a referenced Template.
 * When flags are clear (0), the normal policy (as described by anonymizationTechnique) applies
 * without modification.
 */
#define DOCA_TELEMETRY_NETFLOW_ANONYMIZATION_FLAGS 285
#define DOCA_TELEMETRY_NETFLOW_ANONYMIZATION_FLAGS_DEFAULT_LENGTH 2
/**
 * A description of the anonymization technique applied to a referenced Information Element within a
 * referenced Template. Each technique may be applicable only to certain Information Elements and
 * recommended only for certain Information Elements. Values are listed in the
 * anonymizationTechnique registry. See
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-anonymization-technique].
 */
#define DOCA_TELEMETRY_NETFLOW_ANONYMIZATION_TECHNIQUE 286
#define DOCA_TELEMETRY_NETFLOW_ANONYMIZATION_TECHNIQUE_DEFAULT_LENGTH 2
/**
 * A zero-based index of an Information Element referenced by informationElementId within a Template
 * referenced by templateId; used to disambiguate scope for templates containing multiple identical
 * Information Elements.
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_INDEX 287
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_INDEX_DEFAULT_LENGTH 2
/**
 * Specifies if the Application ID is based on peer-to-peer technology.
 */
#define DOCA_TELEMETRY_NETFLOW_P2P_TECHNOLOGY 288
#define DOCA_TELEMETRY_NETFLOW_P2P_TECHNOLOGY_DEFAULT_LENGTH 64
/**
 * Specifies if the Application ID is used as a tunnel technology.
 */
#define DOCA_TELEMETRY_NETFLOW_TUNNEL_TECHNOLOGY 289
#define DOCA_TELEMETRY_NETFLOW_TUNNEL_TECHNOLOGY_DEFAULT_LENGTH 64
/**
 * Specifies if the Application ID is an encrypted networking protocol.
 */
#define DOCA_TELEMETRY_NETFLOW_ENCRYPTED_TECHNOLOGY 290
#define DOCA_TELEMETRY_NETFLOW_ENCRYPTED_TECHNOLOGY_DEFAULT_LENGTH 64
/**
 * Specifies a generic Information Element with a basicList abstract data type. For example, a list
 * of port numbers, a list of interface indexes, etc.
 */
#define DOCA_TELEMETRY_NETFLOW_BASIC_LIST 291
#define DOCA_TELEMETRY_NETFLOW_BASIC_LIST_DEFAULT_LENGTH 64
/**
 * Specifies a generic Information Element with a subTemplateList abstract data type.
 */
#define DOCA_TELEMETRY_NETFLOW_SUB_TEMPLATE_LIST 292
#define DOCA_TELEMETRY_NETFLOW_SUB_TEMPLATE_LIST_DEFAULT_LENGTH 64
/**
 * Specifies a generic Information Element with a subTemplateMultiList abstract data type.
 */
#define DOCA_TELEMETRY_NETFLOW_SUB_TEMPLATE_MULTI_LIST 293
#define DOCA_TELEMETRY_NETFLOW_SUB_TEMPLATE_MULTI_LIST_DEFAULT_LENGTH 64
/**
 * This element describes the "validity state" of the BGP route correspondent source or destination
 * IP address. If the "validity state" for this Flow is only available, then the value of this
 * Information Element is 255.
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_VALIDITY_STATE 294
#define DOCA_TELEMETRY_NETFLOW_BGP_VALIDITY_STATE_DEFAULT_LENGTH 1
/**
 * IPSec Security Parameters Index (SPI).
 */
#define DOCA_TELEMETRY_NETFLOW_IPSEC_SPI 295
#define DOCA_TELEMETRY_NETFLOW_IPSEC_SPI_DEFAULT_LENGTH 4
/**
 * GRE key, which is used for identifying an individual traffic flow within a tunnel.
 */
#define DOCA_TELEMETRY_NETFLOW_GRE_KEY 296
#define DOCA_TELEMETRY_NETFLOW_GRE_KEY_DEFAULT_LENGTH 4
/**
 * Values are listed in the natType registry. See
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-nat-type].
 */
#define DOCA_TELEMETRY_NETFLOW_NAT_TYPE 297
#define DOCA_TELEMETRY_NETFLOW_NAT_TYPE_DEFAULT_LENGTH 1
/**
 * The total number of layer 4 packets in a flow from the initiator since the previous report. The
 * initiator is the device which triggered the session creation, and remains the same for the life
 * of the session.
 */
#define DOCA_TELEMETRY_NETFLOW_INITIATOR_PACKETS 298
#define DOCA_TELEMETRY_NETFLOW_INITIATOR_PACKETS_DEFAULT_LENGTH 8
/**
 * The total number of layer 4 packets in a flow from the responder since the previous report. The
 * responder is the device which replies to the initiator, and remains the same for the life of the
 * session.
 */
#define DOCA_TELEMETRY_NETFLOW_RESPONDER_PACKETS 299
#define DOCA_TELEMETRY_NETFLOW_RESPONDER_PACKETS_DEFAULT_LENGTH 8
/**
 * The name of an observation domain identified by an observationDomainId.
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_DOMAIN_NAME 300
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_DOMAIN_NAME_DEFAULT_LENGTH 64
/**
 * From all the packets observed at an Observation Point, a subset of the packets is selected by a
 * sequence of one or more Selectors. The selectionSequenceId is a unique value per Observation
 * Domain, specifying the Observation Point and the sequence of Selectors through which the packets
 * are selected.
 */
#define DOCA_TELEMETRY_NETFLOW_SELECTION_SEQUENCE_ID 301
#define DOCA_TELEMETRY_NETFLOW_SELECTION_SEQUENCE_ID_DEFAULT_LENGTH 8
/**
 * The Selector ID is the unique ID identifying a Primitive Selector. Each Primitive Selector must
 * have a unique ID in the Observation Domain.
 */
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID 302
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_DEFAULT_LENGTH 8
/**
 * This Information Element contains the ID of another Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_ID 303
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_ID_DEFAULT_LENGTH 2
/**
 * This Information Element identifies the packet selection methods (e.g., Filtering, Sampling) that
 * are applied by the Selection Process.
 */
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ALGORITHM 304
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ALGORITHM_DEFAULT_LENGTH 2
/**
 * This Information Element specifies the number of packets that are consecutively sampled. A value
 * of 100 means that 100 consecutive packets are sampled.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_PACKET_INTERVAL 305
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_PACKET_INTERVAL_DEFAULT_LENGTH 4
/**
 * This Information Element specifies the number of packets between two "samplingPacketInterval"s. A
 * value of 100 means that the next interval starts 100 packets (which are not sampled) after the
 * current "samplingPacketInterval" is over.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_PACKET_SPACE 306
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_PACKET_SPACE_DEFAULT_LENGTH 4
/**
 * This Information Element specifies the time interval in microseconds during which all arriving
 * packets are sampled.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_TIME_INTERVAL 307
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_TIME_INTERVAL_DEFAULT_LENGTH 4
/**
 * This Information Element specifies the time interval in microseconds between two
 * "samplingTimeInterval"s. A value of 100 means that the next interval starts 100 microseconds
 * (during which no packets are sampled) after the current "samplingTimeInterval" is over.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_TIME_SPACE 308
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_TIME_SPACE_DEFAULT_LENGTH 4
/**
 * This Information Element specifies the number of elements taken from the parent Population for
 * random Sampling methods.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_SIZE 309
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_SIZE_DEFAULT_LENGTH 4
/**
 * This Information Element specifies the number of elements in the parent Population for random
 * Sampling methods.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_POPULATION 310
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_POPULATION_DEFAULT_LENGTH 4
/**
 * This Information Element specifies the probability that a packet is sampled, expressed as a value
 * between 0 and 1. The probability is equal for every packet. A value of 0 means no packet was
 * sampled since the probability is 0.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_PROBABILITY 311
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_PROBABILITY_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the length of the selected data link frame.
 */
#define DOCA_TELEMETRY_NETFLOW_DATA_LINK_FRAME_SIZE 312
#define DOCA_TELEMETRY_NETFLOW_DATA_LINK_FRAME_SIZE_DEFAULT_LENGTH 2
/**
 * This Information Element carries a series of n octets from the IP header of a sampled packet,
 * starting sectionOffset octets into the IP header.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_HEADER_PACKET_SECTION 313
#define DOCA_TELEMETRY_NETFLOW_IP_HEADER_PACKET_SECTION_DEFAULT_LENGTH 3
/**
 * This Information Element carries a series of n octets from the IP payload of a sampled packet,
 * starting sectionOffset octets into the IP payload.
 */
#define DOCA_TELEMETRY_NETFLOW_IP_PAYLOAD_PACKET_SECTION 314
#define DOCA_TELEMETRY_NETFLOW_IP_PAYLOAD_PACKET_SECTION_DEFAULT_LENGTH 3
/**
 * This Information Element carries n octets from the data link frame of a selected frame, starting
 * sectionOffset octets into the frame.
 */
#define DOCA_TELEMETRY_NETFLOW_DATA_LINK_FRAME_SECTION 315
#define DOCA_TELEMETRY_NETFLOW_DATA_LINK_FRAME_SECTION_DEFAULT_LENGTH 3
/**
 * This Information Element carries a series of n octets from the MPLS label stack of a sampled
 * packet, starting sectionOffset octets into the MPLS label stack.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_STACK_SECTION 316
#define DOCA_TELEMETRY_NETFLOW_MPLS_LABEL_STACK_SECTION_DEFAULT_LENGTH 3
/**
 * The mplsPayloadPacketSection carries a series of n octets from the MPLS payload of a sampled
 * packet, starting sectionOffset octets into the MPLS payload, as it is data that follows
 * immediately after the MPLS label stack.
 */
#define DOCA_TELEMETRY_NETFLOW_MPLS_PAYLOAD_PACKET_SECTION 317
#define DOCA_TELEMETRY_NETFLOW_MPLS_PAYLOAD_PACKET_SECTION_DEFAULT_LENGTH 3
/**
 * This Information Element specifies the total number of packets observed by a Selector, for a
 * specific value of SelectorId.
 */
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_TOTAL_PKTS_OBSERVED 318
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_TOTAL_PKTS_OBSERVED_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the total number of packets selected by a Selector, for a
 * specific value of SelectorId.
 */
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_TOTAL_PKTS_SELECTED 319
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_TOTAL_PKTS_SELECTED_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the maximum possible measurement error of the reported value
 * for a given Information Element. The absoluteError has the same unit as the Information Element
 * with which it is associated. The real value of the metric can differ by absoluteError (positive
 * or negative) from the measured value.
 */
#define DOCA_TELEMETRY_NETFLOW_ABSOLUTE_ERROR 320
#define DOCA_TELEMETRY_NETFLOW_ABSOLUTE_ERROR_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the maximum possible positive or negative error ratio for the
 * reported value for a given Information Element as percentage of the measured value. The real
 * value of the metric can differ by relativeError percent (positive or negative) from the measured
 * value.
 */
#define DOCA_TELEMETRY_NETFLOW_RELATIVE_ERROR 321
#define DOCA_TELEMETRY_NETFLOW_RELATIVE_ERROR_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the absolute time in seconds of an observation.
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_TIME_SECONDS 322
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_TIME_SECONDS_DEFAULT_LENGTH 4
/**
 * This Information Element specifies the absolute time in milliseconds of an observation.
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_TIME_MILLISECONDS 323
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_TIME_MILLISECONDS_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the absolute time in microseconds of an observation.
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_TIME_MICROSECONDS 324
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_TIME_MICROSECONDS_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the absolute time in nanoseconds of an observation.
 */
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_TIME_NANOSECONDS 325
#define DOCA_TELEMETRY_NETFLOW_OBSERVATION_TIME_NANOSECONDS_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the value from the digest hash function. See also
 * Sections 6.2, 3.8 and 7.1 of [RFC5475].
 */
#define DOCA_TELEMETRY_NETFLOW_DIGEST_HASH_VALUE 326
#define DOCA_TELEMETRY_NETFLOW_DIGEST_HASH_VALUE_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the IP payload offset used by a Hash-based Selection Selector.
 * See also Sections 6.2, 3.8 and 7.1 of [RFC5475].
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_IPPAYLOAD_OFFSET 327
#define DOCA_TELEMETRY_NETFLOW_HASH_IPPAYLOAD_OFFSET_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the IP payload size used by a Hash-based Selection Selector.
 * See also Sections 6.2, 3.8 and 7.1 of [RFC5475].
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_IPPAYLOAD_SIZE 328
#define DOCA_TELEMETRY_NETFLOW_HASH_IPPAYLOAD_SIZE_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the value for the beginning of a hash function's potential
 * output range.
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_OUTPUT_RANGE_MIN 329
#define DOCA_TELEMETRY_NETFLOW_HASH_OUTPUT_RANGE_MIN_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the value for the end of a hash function's potential output
 * range.
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_OUTPUT_RANGE_MAX 330
#define DOCA_TELEMETRY_NETFLOW_HASH_OUTPUT_RANGE_MAX_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the value for the beginning of a hash function's selected
 * range.
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_SELECTED_RANGE_MIN 331
#define DOCA_TELEMETRY_NETFLOW_HASH_SELECTED_RANGE_MIN_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the value for the end of a hash function's selected range.
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_SELECTED_RANGE_MAX 332
#define DOCA_TELEMETRY_NETFLOW_HASH_SELECTED_RANGE_MAX_DEFAULT_LENGTH 8
/**
 * This Information Element contains a boolean value that is TRUE if the output from this hash
 * Selector has been configured to be included in the packet report as a packet digest, else FALSE.
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_DIGEST_OUTPUT 333
#define DOCA_TELEMETRY_NETFLOW_HASH_DIGEST_OUTPUT_DEFAULT_LENGTH 1
/**
 * This Information Element specifies the initializer value to the hash function.
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_INITIALIZER_VALUE 334
#define DOCA_TELEMETRY_NETFLOW_HASH_INITIALIZER_VALUE_DEFAULT_LENGTH 8
/**
 * The name of a selector identified by a selectorID. Globally unique per Metering Process.
 */
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_NAME 335
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_NAME_DEFAULT_LENGTH 64
/**
 * This Information Element specifies the upper limit of a confidence interval. It is used to
 * provide an accuracy statement for an estimated value. The confidence limits define the range in
 * which the real value is assumed to be with a certain probability p. Confidence limits always need
 * to be associated with a confidence level that defines this probability p. Please note that a
 * confidence interval only provides a probability that the real value lies within the limits. That
 * means the real value can lie outside the confidence limits.
 */
#define DOCA_TELEMETRY_NETFLOW_UPPER_CILIMIT 336
#define DOCA_TELEMETRY_NETFLOW_UPPER_CILIMIT_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the lower limit of a confidence interval. For further
 * information, see the description of upperCILimit.
 */
#define DOCA_TELEMETRY_NETFLOW_LOWER_CILIMIT 337
#define DOCA_TELEMETRY_NETFLOW_LOWER_CILIMIT_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the confidence level. It is used to provide an accuracy
 * statement for estimated values. The confidence level provides the probability p with which the
 * real value lies within a given range. A confidence level always needs to be associated with
 * confidence limits that define the range in which the real value is assumed to be.
 */
#define DOCA_TELEMETRY_NETFLOW_CONFIDENCE_LEVEL 338
#define DOCA_TELEMETRY_NETFLOW_CONFIDENCE_LEVEL_DEFAULT_LENGTH 8
/**
 * A description of the abstract data type of an IPFIX information element.These are taken from the
 * abstract data types defined in section 3.1 of the IPFIX Information Model [RFC5102]; see that
 * section for more information on the types described in the [informationElementDataType]
 * subregistry.
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_DATA_TYPE 339
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_DATA_TYPE_DEFAULT_LENGTH 1
/**
 * A UTF-8 [RFC3629] encoded Unicode string containing a human-readable description of an
 * Information Element. The content of the informationElementDescription MAY be annotated with one
 * or more language tags [RFC4646], encoded in-line [RFC2482] within the UTF-8 string, in order to
 * specify the language in which the description is written. Description text in multiple languages
 * MAY tag each section with its own language tag; in this case, the description information in each
 * language SHOULD have equivalent meaning. In the absence of any language tag, the "i-default"
 * [RFC2277] language SHOULD be assumed. See the Security Considerations section for notes on string
 * handling for Information Element type records.
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_DESCRIPTION 340
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_DESCRIPTION_DEFAULT_LENGTH 64
/**
 * A UTF-8 [RFC3629] encoded Unicode string containing the name of an Information Element, intended
 * as a simple identifier. See the Security Considerations section for notes on string handling for
 * Information Element type records
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_NAME 341
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_NAME_DEFAULT_LENGTH 64
/**
 * Contains the inclusive low end of the range of acceptable values for an Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_RANGE_BEGIN 342
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_RANGE_BEGIN_DEFAULT_LENGTH 8
/**
 * Contains the inclusive high end of the range of acceptable values for an Information Element.
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_RANGE_END 343
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_RANGE_END_DEFAULT_LENGTH 8
/**
 * A description of the semantics of an IPFIX Information Element. These are taken from the data
 * type semantics defined in section 3.2 of the IPFIX Information Model [RFC5102]; see that section
 * for more information on the types defined in the [IPFIX Information Element Semantics]
 * subregistry. This field may take the values in the semantics registry; the special value 0x00
 * (default) is used to note that no semantics apply to the field; it cannot be manipulated by a
 * Collecting Process or File Reader that does not understand it a priori.
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_SEMANTICS 344
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_SEMANTICS_DEFAULT_LENGTH 1
/**
 * A description of the units of an IPFIX Information Element. These correspond to the units
 * implicitly defined in the Information Element definitions in section 5 of the IPFIX Information
 * Model [RFC5102]; see that section for more information on the types described in the
 * informationElementsUnits subregistry. This field may take the values in Table 3 below; the
 * special value 0x00 (none) is used to note that the field is unitless.
 */
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_UNITS 345
#define DOCA_TELEMETRY_NETFLOW_INFORMATION_ELEMENT_UNITS_DEFAULT_LENGTH 2
/**
 * A private enterprise number, as assigned by IANA. Within the context of an Information Element
 * Type record, this element can be used along with the informationElementId element to scope
 * properties to a specific Information Element. To export type information about an IANA-assigned
 * Information Element, set the privateEnterpriseNumber to 0, or do not export the
 * privateEnterpriseNumber in the type record. To export type information about an
 * enterprise-specific Information Element, export the enterprise number in privateEnterpriseNumber,
 * and export the Information Element number with the Enterprise bit cleared in
 * informationElementId. The Enterprise bit in the associated informationElementId Information
 * Element MUST be ignored by the Collecting Process.
 */
#define DOCA_TELEMETRY_NETFLOW_PRIVATE_ENTERPRISE_NUMBER 346
#define DOCA_TELEMETRY_NETFLOW_PRIVATE_ENTERPRISE_NUMBER_DEFAULT_LENGTH 4
/**
 * Instance Identifier of the interface to a Virtual Station. A Virtual Station is an end station
 * instance: it can be a virtual machine or a physical host.
 */
#define DOCA_TELEMETRY_NETFLOW_VIRTUAL_STATION_INTERFACE_ID 347
#define DOCA_TELEMETRY_NETFLOW_VIRTUAL_STATION_INTERFACE_ID_DEFAULT_LENGTH 3
/**
 * Name of the interface to a Virtual Station. A Virtual Station is an end station instance: it can
 * be a virtual machine or a physical host.
 */
#define DOCA_TELEMETRY_NETFLOW_VIRTUAL_STATION_INTERFACE_NAME 348
#define DOCA_TELEMETRY_NETFLOW_VIRTUAL_STATION_INTERFACE_NAME_DEFAULT_LENGTH 64
/**
 * Unique Identifier of a Virtual Station. A Virtual Station is an end station instance: it can be a
 * virtual machine or a physical host.
 */
#define DOCA_TELEMETRY_NETFLOW_VIRTUAL_STATION_UUID 349
#define DOCA_TELEMETRY_NETFLOW_VIRTUAL_STATION_UUID_DEFAULT_LENGTH 3
/**
 * Name of a Virtual Station. A Virtual Station is an end station instance: it can be a virtual
 * machine or a physical host.
 */
#define DOCA_TELEMETRY_NETFLOW_VIRTUAL_STATION_NAME 350
#define DOCA_TELEMETRY_NETFLOW_VIRTUAL_STATION_NAME_DEFAULT_LENGTH 64
/**
 * Identifier of a layer 2 network segment in an overlay network. The most significant byte
 * identifies the layer 2 network overlay network encapsulation type:
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER2_SEGMENT_ID 351
#define DOCA_TELEMETRY_NETFLOW_LAYER2_SEGMENT_ID_DEFAULT_LENGTH 8
/**
 * The number of layer 2 octets since the previous report (if any) in incoming packets for this Flow
 * at the Observation Point. The number of octets includes layer 2 header(s) and layer 2 payload. #
 * memo: layer 2 version of octetDeltaCount (field #1)
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER2_OCTET_DELTA_COUNT 352
#define DOCA_TELEMETRY_NETFLOW_LAYER2_OCTET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of layer 2 octets in incoming packets for this Flow at the Observation Point
 * since the Metering Process (re-)initialization for this Observation Point. The number of octets
 * includes layer 2 header(s) and layer 2 payload. # memo: layer 2 version of octetTotalCount (field
 * #85)
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER2_OCTET_TOTAL_COUNT 353
#define DOCA_TELEMETRY_NETFLOW_LAYER2_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of incoming unicast packets metered at the Observation Point since the Metering
 * Process (re-)initialization for this Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_INGRESS_UNICAST_PACKET_TOTAL_COUNT 354
#define DOCA_TELEMETRY_NETFLOW_INGRESS_UNICAST_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of incoming multicast packets metered at the Observation Point since the
 * Metering Process (re-)initialization for this Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_INGRESS_MULTICAST_PACKET_TOTAL_COUNT 355
#define DOCA_TELEMETRY_NETFLOW_INGRESS_MULTICAST_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of incoming broadcast packets metered at the Observation Point since the
 * Metering Process (re-)initialization for this Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_INGRESS_BROADCAST_PACKET_TOTAL_COUNT 356
#define DOCA_TELEMETRY_NETFLOW_INGRESS_BROADCAST_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of incoming unicast packets metered at the Observation Point since the Metering
 * Process (re-)initialization for this Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_EGRESS_UNICAST_PACKET_TOTAL_COUNT 357
#define DOCA_TELEMETRY_NETFLOW_EGRESS_UNICAST_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of incoming broadcast packets metered at the Observation Point since the
 * Metering Process (re-)initialization for this Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_EGRESS_BROADCAST_PACKET_TOTAL_COUNT 358
#define DOCA_TELEMETRY_NETFLOW_EGRESS_BROADCAST_PACKET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The absolute timestamp at which the monitoring interval started. A Monitoring interval is the
 * period of time during which the Metering Process is running.
 */
#define DOCA_TELEMETRY_NETFLOW_MONITORING_INTERVAL_START_MILLI_SECONDS 359
#define DOCA_TELEMETRY_NETFLOW_MONITORING_INTERVAL_START_MILLI_SECONDS_DEFAULT_LENGTH 8
/**
 * The absolute timestamp at which the monitoring interval ended. A Monitoring interval is the
 * period of time during which the Metering Process is running.
 */
#define DOCA_TELEMETRY_NETFLOW_MONITORING_INTERVAL_END_MILLI_SECONDS 360
#define DOCA_TELEMETRY_NETFLOW_MONITORING_INTERVAL_END_MILLI_SECONDS_DEFAULT_LENGTH 8
/**
 * The port number identifying the start of a range of ports. A value of zero indicates that the
 * range start is not specified, ie the range is defined in some other way.
 */
#define DOCA_TELEMETRY_NETFLOW_PORT_RANGE_START 361
#define DOCA_TELEMETRY_NETFLOW_PORT_RANGE_START_DEFAULT_LENGTH 2
/**
 * The port number identifying the end of a range of ports. A value of zero indicates that the range
 * end is not specified, ie the range is defined in some other way.
 */
#define DOCA_TELEMETRY_NETFLOW_PORT_RANGE_END 362
#define DOCA_TELEMETRY_NETFLOW_PORT_RANGE_END_DEFAULT_LENGTH 2
/**
 * The step size in a port range. The default step size is 1, which indicates contiguous ports. A
 * value of zero indicates that the step size is not specified, ie the range is defined in some
 * other way.
 */
#define DOCA_TELEMETRY_NETFLOW_PORT_RANGE_STEP_SIZE 363
#define DOCA_TELEMETRY_NETFLOW_PORT_RANGE_STEP_SIZE_DEFAULT_LENGTH 2
/**
 * The number of ports in a port range. A value of zero indicates that the number of ports is not
 * specified, ie the range is defined in some other way.
 */
#define DOCA_TELEMETRY_NETFLOW_PORT_RANGE_NUM_PORTS 364
#define DOCA_TELEMETRY_NETFLOW_PORT_RANGE_NUM_PORTS_DEFAULT_LENGTH 2
/**
 * The IEEE 802 MAC address of a wireless station (STA).
 */
#define DOCA_TELEMETRY_NETFLOW_STA_MAC_ADDRESS 365
#define DOCA_TELEMETRY_NETFLOW_STA_MAC_ADDRESS_DEFAULT_LENGTH 6
/**
 * The IPv4 address of a wireless station (STA).
 */
#define DOCA_TELEMETRY_NETFLOW_STA_IPV4_ADDRESS 366
#define DOCA_TELEMETRY_NETFLOW_STA_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * The IEEE 802 MAC address of a wireless access point (WTP).
 */
#define DOCA_TELEMETRY_NETFLOW_WTP_MAC_ADDRESS 367
#define DOCA_TELEMETRY_NETFLOW_WTP_MAC_ADDRESS_DEFAULT_LENGTH 6
/**
 * The type of interface where packets of this Flow are being received. The value matches the value
 * of managed object 'ifType' as defined in [https://www.iana.org/assignments/ianaiftype-mib].
 */
#define DOCA_TELEMETRY_NETFLOW_INGRESS_INTERFACE_TYPE 368
#define DOCA_TELEMETRY_NETFLOW_INGRESS_INTERFACE_TYPE_DEFAULT_LENGTH 4
/**
 * The type of interface where packets of this Flow are being sent. The value matches the value of
 * managed object 'ifType' as defined in [https://www.iana.org/assignments/ianaiftype-mib].
 */
#define DOCA_TELEMETRY_NETFLOW_EGRESS_INTERFACE_TYPE 369
#define DOCA_TELEMETRY_NETFLOW_EGRESS_INTERFACE_TYPE_DEFAULT_LENGTH 4
/**
 * The RTP sequence number per [RFC3550].
 */
#define DOCA_TELEMETRY_NETFLOW_RTP_SEQUENCE_NUMBER 370
#define DOCA_TELEMETRY_NETFLOW_RTP_SEQUENCE_NUMBER_DEFAULT_LENGTH 2
/**
 * User name associated with the flow.
 */
#define DOCA_TELEMETRY_NETFLOW_USER_NAME 371
#define DOCA_TELEMETRY_NETFLOW_USER_NAME_DEFAULT_LENGTH 64
/**
 * An attribute that provides a first level categorization for each Application ID.
 */
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_CATEGORY_NAME 372
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_CATEGORY_NAME_DEFAULT_LENGTH 64
/**
 * An attribute that provides a second level categorization for each Application ID.
 */
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_SUB_CATEGORY_NAME 373
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_SUB_CATEGORY_NAME_DEFAULT_LENGTH 64
/**
 * An attribute that groups multiple Application IDs that belong to the same networking application.
 */
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_GROUP_NAME 374
#define DOCA_TELEMETRY_NETFLOW_APPLICATION_GROUP_NAME_DEFAULT_LENGTH 64
/**
 * The non-conservative count of Original Flows contributing to this Aggregated Flow.
 * Non-conservative counts need not sum to the original count on re-aggregation.
 */
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_FLOWS_PRESENT 375
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_FLOWS_PRESENT_DEFAULT_LENGTH 8
/**
 * The conservative count of Original Flows whose first packet is represented within this Aggregated
 * Flow. Conservative counts must sum to the original count on re-aggregation.
 */
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_FLOWS_INITIATED 376
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_FLOWS_INITIATED_DEFAULT_LENGTH 8
/**
 * The conservative count of Original Flows whose last packet is represented within this Aggregated
 * Flow. Conservative counts must sum to the original count on re-aggregation.
 */
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_FLOWS_COMPLETED 377
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_FLOWS_COMPLETED_DEFAULT_LENGTH 8
/**
 * The count of distinct source IP address values for Original Flows contributing to this Aggregated
 * Flow, without regard to IP version. This Information Element is preferred to the
 * IP-version-specific counters, unless it is important to separate the counts by version.
 */
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_SOURCE_IP_ADDRESS 378
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_SOURCE_IP_ADDRESS_DEFAULT_LENGTH 8
/**
 * The count of distinct destination IP address values for Original Flows contributing to this
 * Aggregated Flow, without regard to IP version. This Information Element is preferred to the
 * version-specific counters below, unless it is important to separate the counts by version.
 */
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_DESTINATION_IP_ADDRESS 379
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_DESTINATION_IP_ADDRESS_DEFAULT_LENGTH 8
/**
 * The count of distinct source IPv4 address values for Original Flows contributing to this
 * Aggregated Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_SOURCE_IPV4_ADDRESS 380
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_SOURCE_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * The count of distinct destination IPv4 address values for Original Flows contributing to this
 * Aggregated Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_DESTINATION_IPV4_ADDRESS 381
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_DESTINATION_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * The count of distinct source IPv6 address values for Original Flows contributing to this
 * Aggregated Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_SOURCE_IPV6_ADDRESS 382
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_SOURCE_IPV6_ADDRESS_DEFAULT_LENGTH 8
/**
 * The count of distinct destination IPv6 address values for Original Flows contributing to this
 * Aggregated Flow.
 */
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_DESTINATION_IPV6_ADDRESS 383
#define DOCA_TELEMETRY_NETFLOW_DISTINCT_COUNT_OF_DESTINATION_IPV6_ADDRESS_DEFAULT_LENGTH 8
/**
 * A description of the method used to distribute the counters from Contributing Flows into the
 * Aggregated Flow records described by an associated scope, generally a Template. The method is
 * deemed to apply to all the non-key Information Elements in the referenced scope for which value
 * distribution is a valid operation; if the originalFlowsInitiated and/or originalFlowsCompleted
 * Information Elements appear in the Template, they are not subject to this distribution method, as
 * they each infer their own distribution method. The valueDistributionMethod registry is intended
 * to list a complete set of possible value distribution methods. See
 * [https://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-value-distribution-method].
 */
#define DOCA_TELEMETRY_NETFLOW_VALUE_DISTRIBUTION_METHOD 384
#define DOCA_TELEMETRY_NETFLOW_VALUE_DISTRIBUTION_METHOD_DEFAULT_LENGTH 1
/**
 * Interarrival jitter as defined in section 6.4.1 of [RFC3550], measured in milliseconds.
 */
#define DOCA_TELEMETRY_NETFLOW_RFC3550_JITTER_MILLISECONDS 385
#define DOCA_TELEMETRY_NETFLOW_RFC3550_JITTER_MILLISECONDS_DEFAULT_LENGTH 4
/**
 * Interarrival jitter as defined in section 6.4.1 of [RFC3550], measured in microseconds.
 */
#define DOCA_TELEMETRY_NETFLOW_RFC3550_JITTER_MICROSECONDS 386
#define DOCA_TELEMETRY_NETFLOW_RFC3550_JITTER_MICROSECONDS_DEFAULT_LENGTH 4
/**
 * Interarrival jitter as defined in section 6.4.1 of [RFC3550], measured in nanoseconds.
 */
#define DOCA_TELEMETRY_NETFLOW_RFC3550_JITTER_NANOSECONDS 387
#define DOCA_TELEMETRY_NETFLOW_RFC3550_JITTER_NANOSECONDS_DEFAULT_LENGTH 4
/**
 * The value of the 1-bit Drop Eligible Indicator (DEI) field of the VLAN tag as described in
 * 802.1Q-2011 subclause 9.6. In case of a QinQ frame, it represents the outer tag's DEI field and
 * in case of an IEEE 802.1ad frame it represents the DEI field of the S-TAG. Note: in earlier
 * versions of 802.1Q the same bit field in the incoming packet is occupied by the Canonical Format
 * Indicator (CFI) field, except for S-TAGs.
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_DEI 388
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_DEI_DEFAULT_LENGTH 1
/**
 * In case of a QinQ frame, it represents the inner tag's Drop Eligible Indicator (DEI) field and in
 * case of an IEEE 802.1ad frame it represents the DEI field of the C-TAG.
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_DEI 389
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_DEI_DEFAULT_LENGTH 1
/**
 * This Information Element identifies the Intermediate Flow Selection Process technique (e.g.,
 * Filtering, Sampling) that is applied by the Intermediate Flow Selection Process. Most of these
 * techniques have parameters. Its configuration parameter(s) MUST be clearly specified. Further
 * Information Elements are needed to fully specify packet selection with these methods and all
 * their parameters. Further method identifiers may be added to the flowSelectorAlgorithm registry.
 * It might be necessary to define new Information Elements to specify their parameters. Please note
 * that the purpose of the flow selection techniques described in this document is the improvement
 * of measurement functions as defined in the Scope (Section 1). The Intermediate Flow Selection
 * Process Techniques identifiers are defined at
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-flowselectoralgorithm].
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SELECTOR_ALGORITHM 390
#define DOCA_TELEMETRY_NETFLOW_FLOW_SELECTOR_ALGORITHM_DEFAULT_LENGTH 2
/**
 * This Information Element specifies the volume in octets of all Flows that are selected in the
 * Intermediate Flow Selection Process since the previous report.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SELECTED_OCTET_DELTA_COUNT 391
#define DOCA_TELEMETRY_NETFLOW_FLOW_SELECTED_OCTET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the volume in packets of all Flows that were selected in the
 * Intermediate Flow Selection Process since the previous report.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SELECTED_PACKET_DELTA_COUNT 392
#define DOCA_TELEMETRY_NETFLOW_FLOW_SELECTED_PACKET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the number of Flows that were selected in the Intermediate
 * Flow Selection Process since the last report.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SELECTED_FLOW_DELTA_COUNT 393
#define DOCA_TELEMETRY_NETFLOW_FLOW_SELECTED_FLOW_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the total number of Flows observed by a Selector, for a
 * specific value of SelectorId. This Information Element should be used in an Options Template
 * scoped to the observation to which it refers. See Section 3.4.2.1 of the IPFIX protocol document
 * [RFC7011].
 */
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_TOTAL_FLOWS_OBSERVED 394
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_TOTAL_FLOWS_OBSERVED_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the total number of Flows selected by a Selector, for a
 * specific value of SelectorId. This Information Element should be used in an Options Template
 * scoped to the observation to which it refers. See Section 3.4.2.1 of the IPFIX protocol document
 * [RFC7011].
 */
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_TOTAL_FLOWS_SELECTED 395
#define DOCA_TELEMETRY_NETFLOW_SELECTOR_ID_TOTAL_FLOWS_SELECTED_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the number of Flows that are consecutively sampled. A value of
 * 100 means that 100 consecutive Flows are sampled. For example, this Information Element may be
 * used to describe the configuration of a systematic count-based Sampling Selector.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_FLOW_INTERVAL 396
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_FLOW_INTERVAL_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the number of Flows between two "samplingFlowInterval"s. A
 * value of 100 means that the next interval starts 100 Flows (which are not sampled) after the
 * current "samplingFlowInterval" is over. For example, this Information Element may be used to
 * describe the configuration of a systematic count-based Sampling Selector.
 */
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_FLOW_SPACING 397
#define DOCA_TELEMETRY_NETFLOW_SAMPLING_FLOW_SPACING_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the time interval in microseconds during which all arriving
 * Flows are sampled. For example, this Information Element may be used to describe the
 * configuration of a systematic time-based Sampling Selector.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLING_TIME_INTERVAL 398
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLING_TIME_INTERVAL_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the time interval in microseconds between two
 * "flowSamplingTimeInterval"s. A value of 100 means that the next interval starts 100 microseconds
 * (during which no Flows are sampled) after the current "flowsamplingTimeInterval" is over. For
 * example, this Information Element may used to describe the configuration of a systematic
 * time-based Sampling Selector.
 */
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLING_TIME_SPACING 399
#define DOCA_TELEMETRY_NETFLOW_FLOW_SAMPLING_TIME_SPACING_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the Information Elements that are used by the Hash-based Flow
 * Selector as the Hash Domain.
 */
#define DOCA_TELEMETRY_NETFLOW_HASH_FLOW_DOMAIN 400
#define DOCA_TELEMETRY_NETFLOW_HASH_FLOW_DOMAIN_DEFAULT_LENGTH 2
/**
 * The number of octets, excluding IP header(s) and Layer 4 transport protocol header(s), observed
 * for this Flow at the Observation Point since the previous report (if any).
 */
#define DOCA_TELEMETRY_NETFLOW_TRANSPORT_OCTET_DELTA_COUNT 401
#define DOCA_TELEMETRY_NETFLOW_TRANSPORT_OCTET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The number of packets containing at least one octet beyond the IP header(s) and Layer 4 transport
 * protocol header(s), observed for this Flow at the Observation Point since the previous report (if
 * any).
 */
#define DOCA_TELEMETRY_NETFLOW_TRANSPORT_PACKET_DELTA_COUNT 402
#define DOCA_TELEMETRY_NETFLOW_TRANSPORT_PACKET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The IPv4 address used by the Exporting Process on an Original Exporter, as seen by the Collecting
 * Process on an IPFIX Mediator. Used to provide information about the Original Observation Points
 * to a downstream Collector.
 */
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_EXPORTER_IPV4_ADDRESS 403
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_EXPORTER_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * The IPv6 address used by the Exporting Process on an Original Exporter, as seen by the Collecting
 * Process on an IPFIX Mediator. Used to provide information about the Original Observation Points
 * to a downstream Collector.
 */
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_EXPORTER_IPV6_ADDRESS 404
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_EXPORTER_IPV6_ADDRESS_DEFAULT_LENGTH 8
/**
 * The Observation Domain ID reported by the Exporting Process on an Original Exporter, as seen by
 * the Collecting Process on an IPFIX Mediator. Used to provide information about the Original
 * Observation Domain to a downstream Collector. When cascading through multiple Mediators, this
 * identifies the initial Observation Domain in the cascade.
 */
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_OBSERVATION_DOMAIN_ID 405
#define DOCA_TELEMETRY_NETFLOW_ORIGINAL_OBSERVATION_DOMAIN_ID_DEFAULT_LENGTH 4
/**
 * Description: An identifier of an Intermediate Process that is unique per IPFIX Device. Typically,
 * this Information Element is used for limiting the scope of other Information Elements. Note that
 * process identifiers may be assigned dynamically; that is, an Intermediate Process may be
 * restarted with a different ID.
 */
#define DOCA_TELEMETRY_NETFLOW_INTERMEDIATE_PROCESS_ID 406
#define DOCA_TELEMETRY_NETFLOW_INTERMEDIATE_PROCESS_ID_DEFAULT_LENGTH 4
/**
 * Description: The total number of received Data Records that the Intermediate Process did not
 * process since the (re-)initialization of the Intermediate Process; includes only Data Records not
 * examined or otherwise handled by the Intermediate Process due to resource constraints, not Data
 * Records that were examined or otherwise handled by the Intermediate Process but those that merely
 * do not contribute to any exported Data Record due to the operations performed by the Intermediate
 * Process.
 */
#define DOCA_TELEMETRY_NETFLOW_IGNORED_DATA_RECORD_TOTAL_COUNT 407
#define DOCA_TELEMETRY_NETFLOW_IGNORED_DATA_RECORD_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * This Information Element specifies the type of the selected data link frame.
 */
#define DOCA_TELEMETRY_NETFLOW_DATA_LINK_FRAME_TYPE 408
#define DOCA_TELEMETRY_NETFLOW_DATA_LINK_FRAME_TYPE_DEFAULT_LENGTH 2
/**
 * This Information Element specifies the offset of the packet section (e.g., dataLinkFrameSection,
 * ipHeaderPacketSection, ipPayloadPacketSection, mplsLabelStackSection, and
 * mplsPayloadPacketSection). If this Information Element is omitted, it defaults to zero (i.e., no
 * offset).
 */
#define DOCA_TELEMETRY_NETFLOW_SECTION_OFFSET 409
#define DOCA_TELEMETRY_NETFLOW_SECTION_OFFSET_DEFAULT_LENGTH 2
/**
 * This Information Element specifies the observed length of the packet section (e.g.,
 * dataLinkFrameSection, ipHeaderPacketSection, ipPayloadPacketSection, mplsLabelStackSection, and
 * mplsPayloadPacketSection) when padding is used.
 */
#define DOCA_TELEMETRY_NETFLOW_SECTION_EXPORTED_OCTETS 410
#define DOCA_TELEMETRY_NETFLOW_SECTION_EXPORTED_OCTETS_DEFAULT_LENGTH 2
/**
 * This Information Element, which is 16 octets long, represents the Backbone Service Instance Tag
 * (I-TAG) Tag Control Information (TCI) field of an Ethernet frame as described in [IEEE802.1Q]. It
 * encodes the Backbone Service Instance Priority Code Point (I-PCP), Backbone Service Instance Drop
 * Eligible Indicator (I-DEI), Use Customer Addresses (UCAs), Backbone Service Instance Identifier
 * (I-SID), Encapsulated Customer Destination Address (C-DA), Encapsulated Customer Source Address
 * (C-SA), and reserved fields. The structure and semantics within the Tag Control Information field
 * are defined in [IEEE802.1Q].
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_SERVICE_INSTANCE_TAG 411
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_SERVICE_INSTANCE_TAG_DEFAULT_LENGTH 3
/**
 * The value of the 24-bit Backbone Service Instance Identifier (I-SID) portion of the Backbone
 * Service Instance Tag (I-TAG) Tag Control Information (TCI) field of an Ethernet frame as
 * described in [IEEE802.1Q].
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_SERVICE_INSTANCE_ID 412
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_SERVICE_INSTANCE_ID_DEFAULT_LENGTH 4
/**
 * The value of the 3-bit Backbone Service Instance Priority Code Point (I-PCP) portion of the
 * Backbone Service Instance Tag (I-TAG) Tag Control Information (TCI) field of an Ethernet frame as
 * described in [IEEE802.1Q].
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_SERVICE_INSTANCE_PRIORITY 413
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_SERVICE_INSTANCE_PRIORITY_DEFAULT_LENGTH 1
/**
 * The value of the Encapsulated Customer Source Address (C-SA) portion of the Backbone Service
 * Instance Tag (I-TAG) Tag Control Information (TCI) field of an Ethernet frame as described in
 * [IEEE802.1Q].
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_SOURCE_MAC_ADDRESS 414
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_SOURCE_MAC_ADDRESS_DEFAULT_LENGTH 6
/**
 * The value of the Encapsulated Customer Destination Address (C-DA) portion of the Backbone Service
 * Instance Tag (I-TAG) Tag Control Information (TCI) field of an Ethernet frame as described in
 * [IEEE802.1Q].
 */
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_DESTINATION_MAC_ADDRESS 415
#define DOCA_TELEMETRY_NETFLOW_DOT1Q_CUSTOMER_DESTINATION_MAC_ADDRESS_DEFAULT_LENGTH 6
/**
 * The definition of this Information Element is identical to the definition of the
 * layer2OctetDeltaCount Information Element, except that it reports a potentially modified value
 * caused by a middlebox function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_LAYER2_OCTET_DELTA_COUNT 417
#define DOCA_TELEMETRY_NETFLOW_POST_LAYER2_OCTET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The number of layer 2 octets since the previous report (if any) in outgoing multicast packets
 * sent for packets of this Flow by a multicast daemon within the Observation Domain. This property
 * cannot necessarily be observed at the Observation Point but may be retrieved by other means. The
 * number of octets includes layer 2 header(s) and layer 2 payload.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_MCAST_LAYER2_OCTET_DELTA_COUNT 418
#define DOCA_TELEMETRY_NETFLOW_POST_MCAST_LAYER2_OCTET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The definition of this Information Element is identical to the definition of the
 * layer2OctetTotalCount Information Element, except that it reports a potentially modified value
 * caused by a middlebox function after the packet passed the Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_LAYER2_OCTET_TOTAL_COUNT 420
#define DOCA_TELEMETRY_NETFLOW_POST_LAYER2_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of layer 2 octets in outgoing multicast packets sent for packets of this Flow by
 * a multicast daemon in the Observation Domain since the Metering Process (re-)initialization. This
 * property cannot necessarily be observed at the Observation Point but may be retrieved by other
 * means. The number of octets includes layer 2 header(s) and layer 2 payload.
 */
#define DOCA_TELEMETRY_NETFLOW_POST_MCAST_LAYER2_OCTET_TOTAL_COUNT 421
#define DOCA_TELEMETRY_NETFLOW_POST_MCAST_LAYER2_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * Layer 2 length of the smallest packet observed for this Flow. The packet length includes the
 * length of the layer 2 header(s) and the length of the layer 2 payload.
 */
#define DOCA_TELEMETRY_NETFLOW_MIN_LAYER2_TOTAL_LENGTH 422
#define DOCA_TELEMETRY_NETFLOW_MIN_LAYER2_TOTAL_LENGTH_DEFAULT_LENGTH 8
/**
 * Layer 2 length of the largest packet observed for this Flow. The packet length includes the
 * length of the layer 2 header(s) and the length of the layer 2 payload.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_LAYER2_TOTAL_LENGTH 423
#define DOCA_TELEMETRY_NETFLOW_MAX_LAYER2_TOTAL_LENGTH_DEFAULT_LENGTH 8
/**
 * The number of layer 2 octets since the previous report (if any) in packets of this Flow dropped
 * by packet treatment. The number of octets includes layer 2 header(s) and layer 2 payload.
 */
#define DOCA_TELEMETRY_NETFLOW_DROPPED_LAYER2_OCTET_DELTA_COUNT 424
#define DOCA_TELEMETRY_NETFLOW_DROPPED_LAYER2_OCTET_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of octets in observed layer 2 packets (including the layer 2 header) that were
 * dropped by packet treatment since the (re-)initialization of the Metering Process.
 */
#define DOCA_TELEMETRY_NETFLOW_DROPPED_LAYER2_OCTET_TOTAL_COUNT 425
#define DOCA_TELEMETRY_NETFLOW_DROPPED_LAYER2_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of octets in observed layer 2 packets (including the layer 2 header) that the
 * Metering Process did not process since the (re-)initialization of the Metering Process.
 */
#define DOCA_TELEMETRY_NETFLOW_IGNORED_LAYER2_OCTET_TOTAL_COUNT 426
#define DOCA_TELEMETRY_NETFLOW_IGNORED_LAYER2_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of octets in observed layer 2 packets (including the layer 2 header) that the
 * Metering Process did not process since the (re-)initialization of the Metering Process.
 */
#define DOCA_TELEMETRY_NETFLOW_NOT_SENT_LAYER2_OCTET_TOTAL_COUNT 427
#define DOCA_TELEMETRY_NETFLOW_NOT_SENT_LAYER2_OCTET_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The sum of the squared numbers of layer 2 octets per incoming packet since the previous report
 * (if any) for this Flow at the Observation Point. The number of octets includes layer 2 header(s)
 * and layer 2 payload.
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER2_OCTET_DELTA_SUM_OF_SQUARES 428
#define DOCA_TELEMETRY_NETFLOW_LAYER2_OCTET_DELTA_SUM_OF_SQUARES_DEFAULT_LENGTH 8
/**
 * The total sum of the squared numbers of layer 2 octets in incoming packets for this Flow at the
 * Observation Point since the Metering Process (re-)initialization for this Observation Point. The
 * number of octets includes layer 2 header(s) and layer 2 payload.
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER2_OCTET_TOTAL_SUM_OF_SQUARES 429
#define DOCA_TELEMETRY_NETFLOW_LAYER2_OCTET_TOTAL_SUM_OF_SQUARES_DEFAULT_LENGTH 8
/**
 * The number of incoming layer 2 frames since the previous report (if any) for this Flow at the
 * Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER2_FRAME_DELTA_COUNT 430
#define DOCA_TELEMETRY_NETFLOW_LAYER2_FRAME_DELTA_COUNT_DEFAULT_LENGTH 8
/**
 * The total number of incoming layer 2 frames for this Flow at the Observation Point since the
 * Metering Process (re-)initialization for this Observation Point.
 */
#define DOCA_TELEMETRY_NETFLOW_LAYER2_FRAME_TOTAL_COUNT 431
#define DOCA_TELEMETRY_NETFLOW_LAYER2_FRAME_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * The destination IPv4 address of the PSN tunnel carrying the pseudowire.
 */
#define DOCA_TELEMETRY_NETFLOW_PSEUDO_WIRE_DESTINATION_IPV4_ADDRESS 432
#define DOCA_TELEMETRY_NETFLOW_PSEUDO_WIRE_DESTINATION_IPV4_ADDRESS_DEFAULT_LENGTH 4
/**
 * The total number of observed layer 2 frames that the Metering Process did not process since the
 * (re-)initialization of the Metering Process. This Information Element is the layer 2 version of
 * ignoredPacketTotalCount (ElementId #164).
 */
#define DOCA_TELEMETRY_NETFLOW_IGNORED_LAYER2_FRAME_TOTAL_COUNT 433
#define DOCA_TELEMETRY_NETFLOW_IGNORED_LAYER2_FRAME_TOTAL_COUNT_DEFAULT_LENGTH 8
/**
 * An IPFIX Information Element that denotes that the integer value of a MIB object will be
 * exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be exported in a
 * MIB Field Option or via another means. This Information Element is used for MIB objects with the
 * Base syntax of Integer32 and INTEGER with IPFIX reduced-size encoding used as required. The value
 * is encoded as per the standard IPFIX Abstract Data Type of signed32.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_INTEGER 434
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_INTEGER_DEFAULT_LENGTH 4
/**
 * An IPFIX Information Element that denotes that an Octet String or Opaque value of a MIB object
 * will be exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be
 * exported in a MIB Field Option or via another means. This Information Element is used for MIB
 * objects with the Base syntax of OCTET STRING and Opaque. The value is encoded as per the standard
 * IPFIX Abstract Data Type of octetArray.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_OCTET_STRING 435
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_OCTET_STRING_DEFAULT_LENGTH 3
/**
 * An IPFIX Information Element that denotes that an Object Identifier or OID value of a MIB object
 * will be exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be
 * exported in a MIB Field Option or via another means. This Information Element is used for MIB
 * objects with the Base syntax of OBJECT IDENTIFIER. Note: In this case, the "mibObjectIdentifier"
 * defines which MIB object is being exported, and the "mibObjectValueOID" field will contain the
 * OID value of that MIB object. The mibObjectValueOID Information Element is encoded as ASN.1/BER
 * [X.690] in an octetArray.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_OID 436
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_OID_DEFAULT_LENGTH 3
/**
 * An IPFIX Information Element that denotes that a set of Enumerated flags or bits from a MIB
 * object will be exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be
 * exported in a MIB Field Option or via another means. This Information Element is used for MIB
 * objects with the Base syntax of BITS. The flags or bits are encoded as per the standard IPFIX
 * Abstract Data Type of octetArray, with sufficient length to accommodate the required number of
 * bits. If the number of bits is not an integer multiple of octets, then the most significant bits
 * at the end of the octetArray MUST be set to 0.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_BITS 437
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_BITS_DEFAULT_LENGTH 3
/**
 * An IPFIX Information Element that denotes that the IPv4 address value of a MIB object will be
 * exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be exported in a
 * MIB Field Option or via another means. This Information Element is used for MIB objects with the
 * Base syntax of IpAddress. The value is encoded as per the standard IPFIX Abstract Data Type of
 * ipv4Address.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_IPADDRESS 438
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_IPADDRESS_DEFAULT_LENGTH 4
/**
 * An IPFIX Information Element that denotes that the counter value of a MIB object will be
 * exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be exported in a
 * MIB Field Option or via another means. This Information Element is used for MIB objects with the
 * Base syntax of Counter32 or Counter64 with IPFIX reduced-size encoding used as required. The
 * value is encoded as per the standard IPFIX Abstract Data Type of unsigned64.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_COUNTER 439
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_COUNTER_DEFAULT_LENGTH 8
/**
 * An IPFIX Information Element that denotes that the Gauge value of a MIB object will be exported.
 * The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be exported in a MIB Field
 * Option or via another means. This Information Element is used for MIB objects with the Base
 * syntax of Gauge32. The value is encoded as per the standard IPFIX Abstract Data Type of
 * unsigned32. This value represents a non-negative integer that may increase or decrease but that
 * shall never exceed a maximum value or fall below a minimum value.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_GAUGE 440
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_GAUGE_DEFAULT_LENGTH 4
/**
 * An IPFIX Information Element that denotes that the TimeTicks value of a MIB object will be
 * exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be exported in a
 * MIB Field Option or via another means. This Information Element is used for MIB objects with the
 * Base syntax of TimeTicks. The value is encoded as per the standard IPFIX Abstract Data Type of
 * unsigned32.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_TIME_TICKS 441
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_TIME_TICKS_DEFAULT_LENGTH 4
/**
 * An IPFIX Information Element that denotes that an unsigned integer value of a MIB object will be
 * exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be exported in a
 * MIB Field Option or via another means. This Information Element is used for MIB objects with the
 * Base syntax of unsigned32 with IPFIX reduced-size encoding used as required. The value is encoded
 * as per the standard IPFIX Abstract Data Type of unsigned32.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_UNSIGNED 442
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_UNSIGNED_DEFAULT_LENGTH 4
/**
 * An IPFIX Information Element that denotes that a complete or partial conceptual table will be
 * exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be exported in a
 * MIB Field Option or via another means. This Information Element is used for MIB objects with a
 * syntax of SEQUENCE OF. This is encoded as a subTemplateList of mibObjectValue Information
 * Elements. The Template specified in the subTemplateList MUST be an Options Template and MUST
 * include all the objects listed in the INDEX clause as Scope Fields.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_TABLE 443
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_TABLE_DEFAULT_LENGTH 64
/**
 * An IPFIX Information Element that denotes that a single row of a conceptual table will be
 * exported. The MIB Object Identifier ("mibObjectIdentifier") for this field MUST be exported in a
 * MIB Field Option or via another means. This Information Element is used for MIB objects with a
 * syntax of SEQUENCE. This is encoded as a subTemplateList of mibObjectValue Information Elements.
 * The subTemplateList exported MUST contain exactly one row (i.e., one instance of the
 * subTemplate). The Template specified in the subTemplateList MUST be an Options Template and MUST
 * include all the objects listed in the INDEX clause as Scope Fields.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_ROW 444
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_VALUE_ROW_DEFAULT_LENGTH 64
/**
 * An IPFIX Information Element that denotes that a MIB Object Identifier (MIB OID) is exported in
 * the (Options) Template Record. The mibObjectIdentifier Information Element contains the OID
 * assigned to the MIB object type definition encoded as ASN.1/BER [X.690].
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_IDENTIFIER 445
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_IDENTIFIER_DEFAULT_LENGTH 3
/**
 * A non-negative sub-identifier of an Object Identifier (OID).
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_SUB_IDENTIFIER 446
#define DOCA_TELEMETRY_NETFLOW_MIB_SUB_IDENTIFIER_DEFAULT_LENGTH 4
/**
 * A set of bit fields that is used for marking the Information Elements of a Data Record that serve
 * as INDEX MIB objects for an indexed columnar MIB object. Each bit represents an Information
 * Element in the Data Record, with the n-th least significant bit representing the n-th Information
 * Element. A bit set to 1 indicates that the corresponding Information Element is an index of the
 * columnar object represented by the mibObjectValue. A bit set to 0 indicates that this is not the
 * case.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_INDEX_INDICATOR 447
#define DOCA_TELEMETRY_NETFLOW_MIB_INDEX_INDICATOR_DEFAULT_LENGTH 8
/**
 * Indicates when in the lifetime of the Flow the MIB value was retrieved from the MIB for a
 * mibObjectIdentifier. This is used to indicate if the value exported was collected from the MIB
 * closer to Flow creation or Flow export time and refers to the Timestamp fields included in the
 * same Data Record. This field SHOULD be used when exporting a mibObjectValue that specifies
 * counters or statistics.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_CAPTURE_TIME_SEMANTICS 448
#define DOCA_TELEMETRY_NETFLOW_MIB_CAPTURE_TIME_SEMANTICS_DEFAULT_LENGTH 1
/**
 * A mibContextEngineID that specifies the SNMP engine ID for a MIB field being exported over IPFIX.
 * Definition as per [RFC3411], Section 3.3.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_CONTEXT_ENGINE_ID 449
#define DOCA_TELEMETRY_NETFLOW_MIB_CONTEXT_ENGINE_ID_DEFAULT_LENGTH 3
/**
 * An Information Element that denotes that a MIB context name is specified for a MIB field being
 * exported over IPFIX. Reference [RFC3411], Section 3.3.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_CONTEXT_NAME 450
#define DOCA_TELEMETRY_NETFLOW_MIB_CONTEXT_NAME_DEFAULT_LENGTH 64
/**
 * The name (called a descriptor in [RFC2578] of an object type definition.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_NAME 451
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_NAME_DEFAULT_LENGTH 64
/**
 * The value of the DESCRIPTION clause of a MIB object type definition.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_DESCRIPTION 452
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_DESCRIPTION_DEFAULT_LENGTH 64
/**
 * The value of the SYNTAX clause of a MIB object type definition, which may include a textual
 * convention or sub-typing. See [RFC2578].
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_SYNTAX 453
#define DOCA_TELEMETRY_NETFLOW_MIB_OBJECT_SYNTAX_DEFAULT_LENGTH 64
/**
 * The textual name of the MIB module that defines a MIB object.
 */
#define DOCA_TELEMETRY_NETFLOW_MIB_MODULE_NAME 454
#define DOCA_TELEMETRY_NETFLOW_MIB_MODULE_NAME_DEFAULT_LENGTH 64
/**
 * The International Mobile Subscription Identity (IMSI). The IMSI is a decimal digit string with up
 * to a maximum of 15 ASCII/UTF-8 encoded digits (0x30 - 0x39).
 */
#define DOCA_TELEMETRY_NETFLOW_MOBILE_IMSI 455
#define DOCA_TELEMETRY_NETFLOW_MOBILE_IMSI_DEFAULT_LENGTH 64
/**
 * The Mobile Station International Subscriber Directory Number (MSISDN). The MSISDN is a decimal
 * digit string with up to a maximum of 15 ASCII/UTF-8 encoded digits (0x30 - 0x39).
 */
#define DOCA_TELEMETRY_NETFLOW_MOBILE_MSISDN 456
#define DOCA_TELEMETRY_NETFLOW_MOBILE_MSISDN_DEFAULT_LENGTH 64
/**
 * The HTTP Response Status Code, as defined in section 6 of [RFC7231], associated with a flow.
 * Implies that the flow record represents a flow containing an HTTP Response.
 */
#define DOCA_TELEMETRY_NETFLOW_HTTP_STATUS_CODE 457
#define DOCA_TELEMETRY_NETFLOW_HTTP_STATUS_CODE_DEFAULT_LENGTH 2
/**
 * This Information Element contains the maximum number of IP source transport ports that can be
 * used by an end user when sending IP packets; each user is associated with one or more (source)
 * IPv4 or IPv6 addresses. This Information Element is particularly useful in address-sharing
 * deployments that adhere to REQ-4 of [RFC6888]. Limiting the number of ports assigned to each user
 * ensures fairness among users and mitigates the denial-of-service attack that a user could launch
 * against other users through the address-sharing device in order to grab more ports.
 */
#define DOCA_TELEMETRY_NETFLOW_SOURCE_TRANSPORT_PORTS_LIMIT 458
#define DOCA_TELEMETRY_NETFLOW_SOURCE_TRANSPORT_PORTS_LIMIT_DEFAULT_LENGTH 2
/**
 * The HTTP request method, as defined in section 4 of [RFC7231], associated with a flow. String
 * with up to 8 UTF-8 characters.
 */
#define DOCA_TELEMETRY_NETFLOW_HTTP_REQUEST_METHOD 459
#define DOCA_TELEMETRY_NETFLOW_HTTP_REQUEST_METHOD_DEFAULT_LENGTH 64
/**
 * The HTTP request host, as defined in section 5.4 of [RFC7230] or, in the case of HTTP/2, the
 * content of the :authority pseudo-header field as defined in section 8.1.2.3 of [RFC7240]. Encoded
 * in UTF-8.
 */
#define DOCA_TELEMETRY_NETFLOW_HTTP_REQUEST_HOST 460
#define DOCA_TELEMETRY_NETFLOW_HTTP_REQUEST_HOST_DEFAULT_LENGTH 64
/**
 * The HTTP request target, as defined in section 2 of [RFC7231] and in section 5.3 of [RFC7230],
 * associated with a flow. Or the HTTP/2 ":path" pseudo-header field as defined in section 8.1.2.3
 * of [RFC7240]. Encoded in UTF-8.
 */
#define DOCA_TELEMETRY_NETFLOW_HTTP_REQUEST_TARGET 461
#define DOCA_TELEMETRY_NETFLOW_HTTP_REQUEST_TARGET_DEFAULT_LENGTH 64
/**
 * The version of an HTTP/1.1 message as indicated by the HTTP-version field, defined in section 2.6
 * of [RFC7230], or the version identification of an HTTP/2 frame as defined in [RFC7240]
 * section 3.1. The length of this field is limited to 10 characters, UTF-8 encoded.
 */
#define DOCA_TELEMETRY_NETFLOW_HTTP_MESSAGE_VERSION 462
#define DOCA_TELEMETRY_NETFLOW_HTTP_MESSAGE_VERSION_DEFAULT_LENGTH 64
/**
 * This Information Element uniquely identifies an Instance of the NAT that runs on a NAT middlebox
 * function after the packet passes the Observation Point. natInstanceID is defined in [RFC7659].
 */
#define DOCA_TELEMETRY_NETFLOW_NAT_INSTANCE_ID 463
#define DOCA_TELEMETRY_NETFLOW_NAT_INSTANCE_ID_DEFAULT_LENGTH 4
/**
 * This Information Element represents the internal address realm where the packet is originated
 * from or destined to. By definition, a NAT mapping can be created from two address realms, one
 * from internal and one from external. Realms are implementation dependent and can represent a
 * Virtual Routing and Forwarding (VRF) ID, a VLAN ID, or some unique identifier. Realms are
 * optional and, when left unspecified, would mean that the external and internal realms are the
 * same.
 */
#define DOCA_TELEMETRY_NETFLOW_INTERNAL_ADDRESS_REALM 464
#define DOCA_TELEMETRY_NETFLOW_INTERNAL_ADDRESS_REALM_DEFAULT_LENGTH 3
/**
 * This Information Element represents the external address realm where the packet is originated
 * from or destined to. The detailed definition is in the internal address realm as specified above.
 */
#define DOCA_TELEMETRY_NETFLOW_EXTERNAL_ADDRESS_REALM 465
#define DOCA_TELEMETRY_NETFLOW_EXTERNAL_ADDRESS_REALM_DEFAULT_LENGTH 3
/**
 * This Information Element identifies the type of a NAT Quota Exceeded event. Values for this
 * Information Element are listed in the "NAT Quota Exceeded Event Type" registry, see
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-nat-quota-exceeded-event].
 */
#define DOCA_TELEMETRY_NETFLOW_NAT_QUOTA_EXCEEDED_EVENT 466
#define DOCA_TELEMETRY_NETFLOW_NAT_QUOTA_EXCEEDED_EVENT_DEFAULT_LENGTH 4
/**
 * This Information Element identifies a type of a NAT Threshold event. Values for this Information
 * Element are listed in the "NAT Threshold Event Type" registry, see
 * [http://www.iana.org/assignments/ipfix/ipfix.xhtml#ipfix-nat-threshold-event].
 */
#define DOCA_TELEMETRY_NETFLOW_NAT_THRESHOLD_EVENT 467
#define DOCA_TELEMETRY_NETFLOW_NAT_THRESHOLD_EVENT_DEFAULT_LENGTH 4
/**
 * The HTTP User-Agent header field as defined in section 5.5.3 of [RFC7231]. Encoded in UTF-8.
 */
#define DOCA_TELEMETRY_NETFLOW_HTTP_USER_AGENT 468
#define DOCA_TELEMETRY_NETFLOW_HTTP_USER_AGENT_DEFAULT_LENGTH 64
/**
 * The HTTP Content-Type header field as defined in section 3.1.1.5 of [RFC7231]. Encoded in UTF-8.
 */
#define DOCA_TELEMETRY_NETFLOW_HTTP_CONTENT_TYPE 469
#define DOCA_TELEMETRY_NETFLOW_HTTP_CONTENT_TYPE_DEFAULT_LENGTH 64
/**
 * The HTTP reason phrase as defined in section 6.1 of [RFC7231].
 */
#define DOCA_TELEMETRY_NETFLOW_HTTP_REASON_PHRASE 470
#define DOCA_TELEMETRY_NETFLOW_HTTP_REASON_PHRASE_DEFAULT_LENGTH 64
/**
 * This element represents the maximum session entries that can be created by the NAT device.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_SESSION_ENTRIES 471
#define DOCA_TELEMETRY_NETFLOW_MAX_SESSION_ENTRIES_DEFAULT_LENGTH 4
/**
 * This element represents the maximum BIB entries that can be created by the NAT device.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_BIBENTRIES 472
#define DOCA_TELEMETRY_NETFLOW_MAX_BIBENTRIES_DEFAULT_LENGTH 4
/**
 * This element represents the maximum NAT entries that can be created per user by the NAT device.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_ENTRIES_PER_USER 473
#define DOCA_TELEMETRY_NETFLOW_MAX_ENTRIES_PER_USER_DEFAULT_LENGTH 4
/**
 * This element represents the maximum subscribers or maximum hosts that are allowed by the NAT
 * device.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_SUBSCRIBERS 474
#define DOCA_TELEMETRY_NETFLOW_MAX_SUBSCRIBERS_DEFAULT_LENGTH 4
/**
 * This element represents the maximum fragments that the NAT device can store for reassembling the
 * packet.
 */
#define DOCA_TELEMETRY_NETFLOW_MAX_FRAGMENTS_PENDING_REASSEMBLY 475
#define DOCA_TELEMETRY_NETFLOW_MAX_FRAGMENTS_PENDING_REASSEMBLY_DEFAULT_LENGTH 4
/**
 * This element represents the high threshold value of the number of public IP addresses in the
 * address pool.
 */
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_POOL_HIGH_THRESHOLD 476
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_POOL_HIGH_THRESHOLD_DEFAULT_LENGTH 4
/**
 * This element represents the low threshold value of the number of public IP addresses in the
 * address pool.
 */
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_POOL_LOW_THRESHOLD 477
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_POOL_LOW_THRESHOLD_DEFAULT_LENGTH 4
/**
 * This element represents the high threshold value of the number of address and port mappings.
 */
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_PORT_MAPPING_HIGH_THRESHOLD 478
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_PORT_MAPPING_HIGH_THRESHOLD_DEFAULT_LENGTH 4
/**
 * This element represents the low threshold value of the number of address and port mappings.
 */
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_PORT_MAPPING_LOW_THRESHOLD 479
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_PORT_MAPPING_LOW_THRESHOLD_DEFAULT_LENGTH 4
/**
 * This element represents the high threshold value of the number of address and port mappings that
 * a single user is allowed to create on a NAT device.
 */
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_PORT_MAPPING_PER_USER_HIGH_THRESHOLD 480
#define DOCA_TELEMETRY_NETFLOW_ADDRESS_PORT_MAPPING_PER_USER_HIGH_THRESHOLD_DEFAULT_LENGTH 4
/**
 * This element represents the high threshold value of the number of address and port mappings that
 * a single user is allowed to create on a NAT device in a paired address pooling behavior.
 */
#define DOCA_TELEMETRY_NETFLOW_GLOBAL_ADDRESS_MAPPING_HIGH_THRESHOLD 481
#define DOCA_TELEMETRY_NETFLOW_GLOBAL_ADDRESS_MAPPING_HIGH_THRESHOLD_DEFAULT_LENGTH 4
/**
 * VPN ID in the format specified by [RFC2685]. The size of this Information Element is 7 octets.
 */
#define DOCA_TELEMETRY_NETFLOW_VPN_IDENTIFIER 482
#define DOCA_TELEMETRY_NETFLOW_VPN_IDENTIFIER_DEFAULT_LENGTH 3
/**
 * BGP community as defined in [RFC1997]
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_COMMUNITY 483
#define DOCA_TELEMETRY_NETFLOW_BGP_COMMUNITY_DEFAULT_LENGTH 4
/**
 * basicList of zero or more bgpCommunity IEs, containing the BGP communities corresponding with
 * source IP address of a specific flow
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_SOURCE_COMMUNITY_LIST 484
#define DOCA_TELEMETRY_NETFLOW_BGP_SOURCE_COMMUNITY_LIST_DEFAULT_LENGTH 64
/**
 * basicList of zero or more bgpCommunity IEs, containing the BGP communities corresponding with
 * destination IP address of a specific flow
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_DESTINATION_COMMUNITY_LIST 485
#define DOCA_TELEMETRY_NETFLOW_BGP_DESTINATION_COMMUNITY_LIST_DEFAULT_LENGTH 64
/**
 * BGP Extended Community as defined in [RFC4360]; the size of this IE MUST be 8 octets
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_EXTENDED_COMMUNITY 486
#define DOCA_TELEMETRY_NETFLOW_BGP_EXTENDED_COMMUNITY_DEFAULT_LENGTH 3
/**
 * basicList of zero or more bgpExtendedCommunity IEs, containing the BGP Extended Communities
 * corresponding with source IP address of a specific flow
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_SOURCE_EXTENDED_COMMUNITY_LIST 487
#define DOCA_TELEMETRY_NETFLOW_BGP_SOURCE_EXTENDED_COMMUNITY_LIST_DEFAULT_LENGTH 64
/**
 * basicList of zero or more bgpExtendedCommunity IEs, containing the BGP Extended Communities
 * corresponding with destination IP address of a specific flow
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_DESTINATION_EXTENDED_COMMUNITY_LIST 488
#define DOCA_TELEMETRY_NETFLOW_BGP_DESTINATION_EXTENDED_COMMUNITY_LIST_DEFAULT_LENGTH 64
/**
 * BGP Large Community as defined in [RFC8092]; the size of this IE MUST be 12 octets.
 */
#define DOCA_TELEMETRY_NETFLOW_BGP_LARGE_COMMUNITY 489
#define DOCA_TELEMETRY_NETFLOW_BGP_LARGE_COMMUNITY_DEFAULT_LENGTH 3

#endif /* DOCA_TELEMETRY_NETFLOW_TYPES_H_ */
