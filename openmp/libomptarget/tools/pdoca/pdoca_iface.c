#include "pdoca_iface.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <string.h>

int pdoca_iface_total_devices()
{
    struct ifaddrs *ifaddr, *ifa;
    int count = 0;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        if (ifa->ifa_addr->sa_family == AF_PACKET)
            count++;
    }

    freeifaddrs(ifaddr);
    return count;
}

void pdoca_iface_get_name(int n, char* iface_name)
{
    struct ifaddrs *ifaddr, *ifa;
    int count = 0;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        if (ifa->ifa_addr->sa_family == AF_PACKET) {
            if (count == n) {
                strncpy(iface_name, ifa->ifa_name, IFNAMSIZ - 1);
                iface_name[IFNAMSIZ - 1] = '\0'; // Ensure null termination
                break;
            }
            count++;
        }
    }

    freeifaddrs(ifaddr);
}

void pdoca_iface_get_ipv4_address(int n, char* ipv4_address)
{
    struct ifaddrs *ifaddr, *ifa;
    int count = 0;
    struct sockaddr_in *addr;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        if (ifa->ifa_addr->sa_family == AF_INET) {
            if (count == n) {
                addr = (struct sockaddr_in *)ifa->ifa_addr;
                inet_ntop(AF_INET, &addr->sin_addr, ipv4_address, INET_ADDRSTRLEN);
		ipv4_address[INET_ADDRSTRLEN - 1] = '\0';

                break;
            }
            count++;
        }
    }

    freeifaddrs(ifaddr);
}

void pdoca_iface_get_ipv6_address(int n, char* ipv6_address)
{
    struct ifaddrs *ifaddr, *ifa;
    int count = 0;
    struct sockaddr_in6 *addr;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        if (ifa->ifa_addr->sa_family == AF_INET6) {
            if (count == n) {
                addr = (struct sockaddr_in6 *)ifa->ifa_addr;
                inet_ntop(AF_INET6, &addr->sin6_addr, ipv6_address, INET6_ADDRSTRLEN);
		ipv6_address[INET6_ADDRSTRLEN - 1] = '\0';
                break;
            }
            count++;
        }
    }

    freeifaddrs(ifaddr);
}
