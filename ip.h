#include "net.h"


#define IP_HDR_SIZE_MIN 20
#define IP_ADDR_STR_LEN sizeof("255.255.255.255")

typedef uint32_t ip_addr_t;

struct ip_iface
{
    struct net_iface iface;
    struct ip_iface *next;
    ip_addr_t unicast;
    ip_addr_t netmask;
    ip_addr_t broadcast;
};


int ip_init(void);
void ip_dump(const uint8_t *data, size_t len);
char *ip_addr_ntop(ip_addr_t src, char *addr, size_t len);
struct ip_iface *ip_iface_alloc(const char *unicast, const char *netmask);
