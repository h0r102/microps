#include "net.h"

#define IP_VERSION_IPV4 4

#define IP_HDR_SIZE_MIN 20
#define IP_ADDR_STR_LEN sizeof("255.255.255.255")

#define IP_TOTAL_SIZE_MAX UINT16_MAX


typedef uint32_t ip_addr_t;

struct ip_iface
{
    struct net_iface iface;
    struct ip_iface *next;
    ip_addr_t unicast;
    ip_addr_t netmask;
    ip_addr_t broadcast;
};

ssize_t ip_output(uint8_t protocol, const uint8_t *data, size_t len, ip_addr_t src, ip_addr_t dst);
int ip_init(void);
void ip_dump(const uint8_t *data, size_t len);
char *ip_addr_ntop(ip_addr_t src, char *addr, size_t len);
int ip_addr_pton(const char *addr, ip_addr_t *src);
struct ip_iface *ip_iface_alloc(const char *unicast, const char *netmask);
int ip_iface_register(struct net_device *dev, struct ip_iface *iface);
struct ip_iface *ip_iface_select(ip_addr_t addr);
