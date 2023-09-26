#ifndef IP_H
#define IP_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#include "net.h"

#define IP_VERSION_IPV4 4

#define IP_HDR_SIZE_MIN 20
#define IP_HDR_SIZE_MAX 60

#define IP_TOTAL_SIZE_MAX UINT16_MAX /* maximum value of uint16 */
#define IP_PAYLOAD_SIZE_MAX (IP_TOTAL_SIZE_MAX - IP_HDR_SIZE_MIN)

#define IP_ADDR_LEN 4
#define IP_ADDR_STR_LEN 16 /* "ddd.ddd.ddd.ddd\0 "*/

#define IP_ENDPOINT_STR_LEN (IP_ADDR_STR_LEN + 6) /* xxx.xxx.xxx.xxx:yyyy\n */

/* see https://www.iana.org/assignments/protocol-numbers/protocol-numbers.txt */
#define IP_PROTOCOL_ICMP 0x01
#define IP_PRPTOCOL_TCP 0x06
#define IP_PROTOCOL_UDP 0x11

typedef uint32_t ip_addr_t;

/* Pear of IP address and Port number */
struct ip_endpoint {
    ip_addr_t addr;
    uint16_t port;
};

/* IP interface */
struct ip_iface {
    struct net_iface iface; /* interface */
    struct ip_iface *next;  /* pointer to next IP interface(iface) */
    ip_addr_t unicast;      /* unicast address */
    ip_addr_t netmask;      /* netmask address */
    ip_addr_t broadcast;    /* broakcast address */
};

extern const ip_addr_t IP_ADDR_ANY;
extern const ip_addr_t IP_ADDR_BROADCAST;

extern int ip_addr_pton(const char *p, ip_addr_t *n);
extern char *ip_addr_ntop(ip_addr_t n, char *p, size_t size);
extern int ip_endpoint_pton(const char *p, struct ip_endpoint *n);
extern char *ip_endpoint_ntop(const struct ip_endpoint *n, char *p,
                              size_t size);

extern int ip_route_set_default_gataway(struct ip_iface *iface,
                                        const char *gateway);
extern struct ip_iface *ip_route_get_iface(ip_addr_t dst);

extern struct ip_iface *ip_iface_alloc(const char *addr, const char *netmask);
extern int ip_iface_register(struct net_device *dev, struct ip_iface *iface);
extern struct ip_iface *ip_iface_select(ip_addr_t addr);

extern ssize_t ip_output(uint8_t protocol, const uint8_t *data, size_t len,
                         ip_addr_t src, ip_addr_t dst);

extern int ip_protocol_register(uint8_t type,
                                void (*handler)(const uint8_t *data, size_t len,
                                                ip_addr_t src, ip_addr_t dst,
                                                struct ip_iface *iface));

extern int ip_init(void);

#endif