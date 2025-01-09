#ifndef PDOCA_IFACE_H__
#define PDOCA_IFACE_H__

int pdoca_iface_total_devices();

void pdoca_iface_get_name(        int n, char* iface_name  );
void pdoca_iface_get_ipv4_address(int n, char* ipv4_address);
void pdoca_iface_get_ipv6_address(int n, char* ipv6_address);

#endif
