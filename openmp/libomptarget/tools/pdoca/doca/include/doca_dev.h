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
 * @file doca_dev.h
 * @page doca dev
 * @defgroup DEV DOCA Device
 * @ingroup DOCACore
 * The DOCA device represents an available processing unit backed by the HW or SW implementation.
 *
 * @{
 */

#ifndef DOCA_DEV_H_
#define DOCA_DEV_H_

#include <stdint.h>

#include <doca_types.h>
#include <doca_compat.h>
#include <doca_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque structure representing a local DOCA Device Info instance.
 * This structure is used to query information about the local device.
 */
struct doca_devinfo;
/**
 * @brief Opaque structure representing a representor DOCA Device Info instance.
 * This structure is used to query information about the representor device.
 */
struct doca_devinfo_rep;

/**
 * @brief Opaque structure representing a local DOCA Device instance.
 * This structure is used by libraries for accessing the underlying local device.
 */
struct doca_dev;
/**
 * @brief Opaque structure representing a representor DOCA Device instance.
 * This structure is used by libraries for accessing the underlying representor device.
 */
struct doca_dev_rep;

/**
 * Representor device filter by flavor
 *
 * Multiple options possible but some are mutually exclusive.
 *
 */
enum doca_devinfo_rep_filter {
	DOCA_DEVINFO_REP_FILTER_ALL = 0,
	DOCA_DEVINFO_REP_FILTER_NET = 1 << 1,
	DOCA_DEVINFO_REP_FILTER_EMULATED = 1 << 2,
};

/**
 * @brief Creates list of all available local devices.
 *
 * Lists information about available devices, to start using the device you first have to call doca_dev_open(),
 * while passing an element of this list. List elements become invalid once it has been destroyed.
 *
 * @param [out] dev_list
 * Pointer to array of pointers. Output can then be accessed as follows (*dev_list)[idx].
 * @param [out] nb_devs
 * Number of available local devices.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate enough space.
 * - DOCA_ERROR_NOT_FOUND - failed to get RDMA devices list
 * @note Returned list must be destroyed using doca_devinfo_destroy_list()
 */
DOCA_STABLE
doca_error_t doca_devinfo_create_list(struct doca_devinfo ***dev_list, uint32_t *nb_devs);

/**
 * @brief Destroy list of local device info structures.
 *
 * Destroys the list of device information, once the list has been destroyed, all elements become invalid.
 *
 * @param [in] dev_list
 * List to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_IN_USE - at least one device in the list is in a corrupted state.
 */
DOCA_STABLE
doca_error_t doca_devinfo_destroy_list(struct doca_devinfo **dev_list);

/**
 * @brief Create list of available representor devices accessible by dev.
 *
 * Returns all representors managed by the provided device.
 * The provided device must be a local device.
 * The representor may represent a network function attached to the host,
 * or it can represent an emulated function attached to the host.
 *
 * @param [in] dev
 * Local device with access to representors.
 * @param [in] filter
 * Bitmap filter of representor types. See enum doca_devinfo_rep_filter for more details.
 * @param [out] dev_list_rep
 * Pointer to array of pointers. Output can then be accessed as follows (*dev_list_rep)[idx].
 * @param [out] nb_devs_rep
 * Number of available representor devices.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate memory for list.
 * - DOCA_ERROR_DRIVER - Failed to query driver.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 * @note Returned list must be destroyed using doca_devinfo_rep_destroy_list()
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_create_list(struct doca_dev *dev,
					  int filter,
					  struct doca_devinfo_rep ***dev_list_rep,
					  uint32_t *nb_devs_rep);

/**
 * @brief Destroy list of representor device info structures.
 *
 * Destroy list of representor device information, once the list has been destroyed,
 * all elements of the list are considered invalid.
 *
 * @param [in] dev_list_rep
 * List to be destroyed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_IN_USE - the doca_dev that created the list is in a corrupted state.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_destroy_list(struct doca_devinfo_rep **dev_list_rep);

/**
 * @brief Initialize local device for use.
 *
 * Opens device or increments its refcount by One. The device can later be used by other libraries.
 * For every call to doca_dev_open() there should be a call to doca_dev_close().
 *
 * @param [in] devinfo
 * The devinfo structure of the requested device.
 * @param [out] dev
 * Initialized local doca device instance on success. Valid on success only.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate protection domain for device.
 * - DOCA_ERROR_NOT_CONNECTED - failed to open device.
 * - DOCA_ERROR_INITIALIZATION - maximum number of open devices was exceeded.
 */
DOCA_STABLE
doca_error_t doca_dev_open(struct doca_devinfo *devinfo, struct doca_dev **dev);

/**
 * @brief Destroy allocated local device instance.
 *
 * Closes device or decrements its refcount by One.
 * In case the same device was opened multiple times, then only the last call to close will attempt to destroy device.
 *
 * @param [in] dev
 * The local doca device instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * - DOCA_ERROR_IN_USE - failed to deallocate device resources.
 */
DOCA_STABLE
doca_error_t doca_dev_close(struct doca_dev *dev);

/**
 * @brief Initialize representor device for use.
 *
 * @param [in] devinfo
 * The devinfo structure of the requested device.
 * @param [out] dev_rep
 * Initialized representor doca device instance on success. Valid on success only.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NO_MEMORY - failed to allocate memory for device.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 */
DOCA_STABLE
doca_error_t doca_dev_rep_open(struct doca_devinfo_rep *devinfo, struct doca_dev_rep **dev_rep);

/**
 * @brief Destroy allocated representor device instance.
 *
 * @param [in] dev
 * The representor doca device instance.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_IN_USE - failed to deallocate device resources.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 */
DOCA_STABLE
doca_error_t doca_dev_rep_close(struct doca_dev_rep *dev);

/**
 * @brief Get local device info from device.
 * This should be useful when wanting to query information about device after opening it,
 * and destroying the devinfo list.
 *
 * @param [in] dev
 * The doca device instance.
 *
 * @return
 * The matching doca_devinfo instance in case of success, NULL in case dev is invalid or was created by
 * doca_rdma_bridge_open_dev_from_pd().
 */
DOCA_STABLE
struct doca_devinfo *doca_dev_as_devinfo(const struct doca_dev *dev);

/**
 * @brief Get representor device info from device.
 * This should be useful when wanting to query information about device after opening it,
 * and destroying the devinfo list.
 *
 * @param [in] dev_rep
 * The representor doca device instance.
 *
 * @return
 * The matching doca_devinfo_rep instance in case of success, NULL in case dev_rep is invalid.
 */
DOCA_STABLE
struct doca_devinfo_rep *doca_dev_rep_as_devinfo(struct doca_dev_rep *dev_rep);

/*********************************************************************************************************************
 * DOCA Local Device Info Properties
 *********************************************************************************************************************/
/**
 * @brief Buffer size to hold VUID. Including a null terminator.
 */
#define DOCA_DEVINFO_VUID_SIZE 128
/**
 * @brief Length of IPv4 address.
 */
#define DOCA_DEVINFO_IPV4_ADDR_SIZE 4
/**
 * @brief Length of IPv6 address.
 */
#define DOCA_DEVINFO_IPV6_ADDR_SIZE 16
/**
 * @brief Length of MAC address.
 */
#define DOCA_DEVINFO_MAC_ADDR_SIZE 6
/**
 * @brief Buffer size to hold network interface name. Including a null terminator.
 */
#define DOCA_DEVINFO_IFACE_NAME_SIZE 256
/**
 * @brief Buffer size to hold Infiniband/RoCE device name. Including a null terminator.
 */
#define DOCA_DEVINFO_IBDEV_NAME_SIZE 64
/**
 * @brief Buffer size to hold PCI BDF format: "XXXX:XX:XX.X". Including a null terminator.
 */
#define DOCA_DEVINFO_PCI_ADDR_SIZE 13
/**
 * @brief Buffer size to hold PCI BDF format: "XX:XX.X". Including a null terminator.
 */
#define DOCA_DEVINFO_PCI_BDF_SIZE 8

/**
 * @brief Get the PCI address of a DOCA devinfo.
 *
 * @details The PCI address string format is "Domain:Bus:Device.Function",
 * such that each value is represented by HEX digits, e.g., "0000:3a:00.0"
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] pci_addr_str
 * The PCI address of devinfo, should be of size DOCA_DEVINFO_PCI_ADDR_SIZE at least.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_OPERATING_SYSTEM - failed to acquire the PCI address from the OS
 */
DOCA_STABLE
doca_error_t doca_devinfo_get_pci_addr_str(const struct doca_devinfo *devinfo, char *pci_addr_str);

/**
 * @brief Check if a PCI address belongs to a DOCA devinfo.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [in] pci_addr_str
 * The PCI address to check, should be as one of the following formats:
 * - "Domain:Bus:Device.Function", e.g., "0000:3a:00.0" (size DOCA_DEVINFO_PCI_ADDR_SIZE including a null terminator).
 * - "Bus:Device.Function", e.g., "3a:00.0" (size DOCA_DEVINFO_PCI_BDF_SIZE including a null terminator).
 * @param [out] is_equal
 * 1 if pci_addr_str belongs to devinfo, 0 otherwise. In case of an error, no certain value is guaranteed.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_OPERATING_SYSTEM - failed to acquire the actual PCI address from the OS for comparison.
 */
DOCA_STABLE
doca_error_t doca_devinfo_is_equal_pci_addr(const struct doca_devinfo *devinfo,
					    const char *pci_addr_str,
					    uint8_t *is_equal);

/**
 * @brief Get the IPv4 address of a DOCA devinfo.
 *
 * @details The IPv4 address type: uint8_t[DOCA_DEVINFO_IPV4_ADDR_SIZE].
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] ipv4_addr
 * The IPv4 address of devinfo.
 * @param [in] size
 * The size of the input ipv4_addr buffer, must be at least DOCA_DEVINFO_IPV4_ADDR_SIZE
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_FOUND - no IPv4 address was assigned
 * - DOCA_ERROR_OPERATING_SYSTEM - failed to acquire the IPv4 address from the OS
 */
DOCA_STABLE
doca_error_t doca_devinfo_get_ipv4_addr(const struct doca_devinfo *devinfo, uint8_t *ipv4_addr, uint32_t size);

/**
 * @brief Get the IPv6 address of a DOCA devinfo.
 *
 * @details The IPv6 address type: uint8_t[DOCA_DEVINFO_IPV6_ADDR_SIZE].
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] ipv6_addr
 * The IPv6 address of devinfo.
 * @param [in] size
 * The size of the input ipv6_addr buffer, must be at least DOCA_DEVINFO_IPV6_ADDR_SIZE
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_OPERATING_SYSTEM - failed to acquire the IPv6 address from the OS
 */
DOCA_STABLE
doca_error_t doca_devinfo_get_ipv6_addr(const struct doca_devinfo *devinfo, uint8_t *ipv6_addr, uint32_t size);
/**
 * @brief Get the MAC address of a DOCA devinfo.
 *
 * @details The MAC address type: uint8_t[DOCA_DEVINFO_MAC_ADDR_SIZE].
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] mac_addr
 * The MAC address of devinfo.
 * @param [in] size
 * The size of the input mac_addr buffer, must be at least DOCA_DEVINFO_MAC_ADDR_SIZE
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - the device port's link layer is not RoCE.
 */
DOCA_STABLE
doca_error_t doca_devinfo_get_mac_addr(const struct doca_devinfo *devinfo, uint8_t *mac_addr, uint32_t size);

/**
 * @brief Get the name of the ethernet interface of a DOCA devinfo.
 *
 * @details The name of the ethernet interface is the same as it's name in ifconfig.
 * The name of the ethernet interface type: char[DOCA_DEVINFO_IFACE_NAME_SIZE].
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] iface_name
 * The name of the ethernet interface of devinfo.
 * @param [in] size
 * The size of the input iface_name buffer, must be at least DOCA_DEVINFO_IFACE_NAME_SIZE
 * which includes the null terminating byte.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_OPERATING_SYSTEM - failed to acquire the interface name from the OS
 */
DOCA_STABLE
doca_error_t doca_devinfo_get_iface_name(const struct doca_devinfo *devinfo, char *iface_name, uint32_t size);

/**
 * @brief Get the name of the IB device represented by a DOCA devinfo.
 *
 * @details The name of the IB device type: char[DOCA_DEVINFO_IBDEV_NAME_SIZE].
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] ibdev_name
 * The name of the IB device represented by devinfo.
 * @param [in] size
 * The size of the input ibdev_name buffer, must be at least DOCA_DEVINFO_IBDEV_NAME_SIZE
 * which includes the null terminating byte.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_devinfo_get_ibdev_name(const struct doca_devinfo *devinfo, char *ibdev_name, uint32_t size);

/**
 * @brief Get the port LID of a DOCA devinfo.
 *
 * @details The port LID type: uint16_t *.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] lid
 * The port LID of devinfo.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query port LID.
 * - DOCA_ERROR_NOT_SUPPORTED - the device port's link layer is not IB.
 */
DOCA_STABLE
doca_error_t doca_devinfo_get_lid(const struct doca_devinfo *devinfo, uint16_t *lid);

/**
 * @brief Get the active rate of a DOCA devinfo.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] active_rate
 * The active rate of the given port on the device. Given in units of Gb/s.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query port rate.
 */
DOCA_EXPERIMENTAL
doca_error_t doca_devinfo_get_active_rate(const struct doca_devinfo *devinfo, double *active_rate);

/**
 * @brief Get the hotplug manager capability of a DOCA devinfo.
 *
 * @details The hotplug manager property type: uint8_t*.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] is_hotplug_manager
 * 1 if the hotplug manager capability is supported, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query capability support.
 */
DOCA_STABLE
doca_error_t doca_devinfo_cap_is_hotplug_manager_supported(const struct doca_devinfo *devinfo,
							   uint8_t *is_hotplug_manager);

/**
 * @brief Get the representor devices discovery capability of the device.
 *
 * @details Get uint8_t value defining if the device can be used to create list of representor devices.
 * In case true is returned, then this device supports at least one representor type.
 * See doca_devinfo_rep_create_list().
 * true  - device can be used with the remote list create API with filter DOCA_DEVINFO_REP_FILTER_ALL.
 * false - providing DOCA_DEVINFO_REP_FILTER_ALL is guaranteed to fail with DOCA_ERROR_NOT_SUPPORTED.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] filter_all_supported
 * 1 if the rep list all capability is supported, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query capability support.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_cap_is_filter_all_supported(const struct doca_devinfo *devinfo,
							  uint8_t *filter_all_supported);

/**
 * @brief Get the remote net discovery capability of the device.
 *
 * @details Get uint8_t value defining if the device can be used to create list of net remote devices.
 * See doca_devinfo_remote_list_create().
 * true  - device can be used with the remote list create API with filter DOCA_DEV_REMOTE_FILTER_NET.
 * false - providing DOCA_DEV_REMOTE_FILTER_NET is guaranteed to fail with DOCA_ERROR_NOT_SUPPORTED.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] filter_net_supported
 * 1 if the rep list net capability is supported, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query capability support.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_cap_is_filter_net_supported(const struct doca_devinfo *devinfo,
							  uint8_t *filter_net_supported);

/**
 * @brief Get the remote emulated device discovery capability of the device.
 *
 * @details Get uint8_t value defining if the device can be used to create list of emulated representor devices.
 * See doca_devinfo_rep_create_list().
 * true  - device can be used with the remote list create API with filter DOCA_DEVINFO_REP_FILTER_EMULATED.
 * false - providing DOCA_DEVINFO_REP_FILTER_EMULATED is guaranteed to fail with DOCA_ERROR_NOT_SUPPORTED.
 *
 * @param [in] devinfo
 * The device to query.
 * @param [out] filter_emulated_supported
 * 1 if the list emulated capability is supported, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_DRIVER - failed to query capability support.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_cap_is_filter_emulated_supported(const struct doca_devinfo *devinfo,
							       uint8_t *filter_emulated_supported);

/*********************************************************************************************************************
 * DOCA Representor Device Info Properties
 *********************************************************************************************************************/
/**
 * @brief Buffer size to hold VUID. Including a null terminator.
 */
#define DOCA_DEVINFO_REP_VUID_SIZE 128
/**
 * @brief Buffer size to hold PCI BDF format: "XXXX:XX:XX.X". Including a null terminator.
 */
#define DOCA_DEVINFO_REP_PCI_ADDR_SIZE 13
/**
 * @brief Buffer size to hold PCI BDF format: "XX:XX.X". Including a null terminator.
 */
#define DOCA_DEVINFO_REP_PCI_BDF_SIZE 8

/**
 * @brief Get the Vendor Unique ID of a representor DOCA devinfo.
 *
 * @details The Vendor Unique ID is used as stable ID of a VF/PF.
 * The Vendor Unique ID type: char[DOCA_DEVINFO_VUID_SIZE].
 *
 * @param [in] devinfo_rep
 * The representor device to query.
 * @param [out] rep_vuid
 * The Vendor Unique ID of devinfo_rep.
 * @param [in] size
 * The size of the vuid buffer, including the terminating null byte ('\0').
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_get_vuid(const struct doca_devinfo_rep *devinfo_rep, char *rep_vuid, uint32_t size);

/**
 * @brief Get the PCI address of a DOCA devinfo_rep.
 *
 * @details The PCI address string format is "Domain:Bus:Device.Function",
 * such that each value is represented by HEX digits, e.g., "0000:3a:00.0".
 *
 * @param [in] devinfo_rep
 * The device to query.
 * @param [out] pci_addr_str
 * The PCI address of devinfo_rep, should be of size DOCA_DEVINFO_REP_PCI_ADDR_SIZE at least.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 * - DOCA_ERROR_NO_MEMORY - not enough memory to generate the stringed PCI address.
 * - DOCA_ERROR_UNEXPECTED - an unexpected error occurred.
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_get_pci_addr_str(const struct doca_devinfo_rep *devinfo_rep, char *pci_addr_str);

/**
 * @brief Get the PCI function type of a DOCA devinfo_rep.
 *
 * @details The pci function type: enum doca_pci_func_type.
 *
 * @param [in] devinfo_rep
 * The representor of device to query.
 * @param [out] pci_func_type
 * The PCI function type of the devinfo_rep.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_get_pci_func_type(const struct doca_devinfo_rep *devinfo_rep,
						enum doca_pci_func_type *pci_func_type);

/**
 * @brief Query whether the representor device is a hotplugged device
 * @param [in] devinfo_rep
 * representor device info
 * @param [out] is_hotplug
 * 1 if the representor device is a hotplugged device. 0 if representor device is statically plugged.
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_get_is_hotplug(const struct doca_devinfo_rep *devinfo_rep, uint8_t *is_hotplug);

/**
 * @brief Check if a PCI address belongs to a DOCA devinfo_rep.
 *
 * @param [in] devinfo_rep
 * The representor of device to query.
 * @param [in] pci_addr_str
 * The PCI address to check, should be as one of the following formats:
 * - "Domain:Bus:Device.Function", e.g., "0000:3a:00.0" (size DOCA_DEVINFO_PCI_ADDR_SIZE including a null terminator).
 * - "Bus:Device.Function", e.g., "3a:00.0" (size DOCA_DEVINFO_PCI_BDF_SIZE including a null terminator).
 * @param [out] is_equal
 * 1 if pci_addr_str belongs to devinfo_rep, 0 otherwise.
 *
 * @return
 * DOCA_SUCCESS - in case of success.
 * Error code - in case of failure:
 * - DOCA_ERROR_INVALID_VALUE - received invalid input.
 * - DOCA_ERROR_NOT_SUPPORTED - local device does not expose representor devices,
 *     or dev was created by doca_rdma_bridge_open_dev_from_pd()
 * - DOCA_ERROR_NO_MEMORY - not enough memory to generate devinfo_rep PCI address for comparison.
 * - DOCA_ERROR_UNEXPECTED - an unexpected error occurred.
 */
DOCA_STABLE
doca_error_t doca_devinfo_rep_is_equal_pci_addr(const struct doca_devinfo_rep *devinfo_rep,
						const char *pci_addr_str,
						uint8_t *is_equal);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DEV_H_ */
