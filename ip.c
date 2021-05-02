#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "net.h"
#include "ip.h"

struct ip_hdr
{
    uint8_t vhl;
    uint8_t tos;
    uint16_t total;
    uint16_t id;
    uint16_t offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t sum;
    ip_addr_t src;
    ip_addr_t dst;
};

static void ip_input(const uint8_t *data, size_t len, struct net_device *dev)
{
    struct ip_hdr *hdr;
    uint16_t offset;
    uint16_t total;

    if (len < IP_HDR_SIZE_MIN)
    {
        errorf("too short");
        return;
    }
    hdr = (struct ip_hdr *)data;
    offset = ntoh16(hdr->offset);
    if (offset & 0x2000 || offset & 0x1fff)
    {
        errorf("fragments does not support");
        return;
    }
    total = ntoh16(hdr->total);
    debugf("dev=%s, protocol=%u, total=%u", dev->name, hdr->protocol, total);
    ip_dump(data, total);
}

int ip_init(void)
{
    if (net_protocol_register(NET_PROTOCOL_TYPE_IP, ip_input) == -1)
    {
        errorf("net_protocol_register() failure");
        return -1;
    }
    return 0;
}

void ip_dump(const uint8_t *data, size_t len)
{
    struct ip_hdr *hdr;
    uint8_t v, hl, hlen;
    uint16_t total, offset;
    char addr[IP_ADDR_STR_LEN];

    flockfile(stderr);
    hdr = (struct ip_hdr *)data;
    v = (hdr->vhl & 0xf0) >> 4;
    hl = hdr->vhl & 0x0f;
    hlen = hl << 2;
    fprintf(stderr, "        vhl: 0x%02x [v: %u, hl: %u (%u)]\n", hdr->vhl, v, hl, hlen);
    fprintf(stderr, "        tos: 0x%02x\n", hdr->tos);
    total = ntoh16(hdr->total);
    fprintf(stderr, "      total: %u (payload: %u)\n", total, total - hlen);
    fprintf(stderr, "         id: %u\n", ntoh16(hdr->id));
    offset = ntoh16(hdr->offset);
    fprintf(stderr, "     offset: 0x%04x [flags=%x, offset=%u]\n", offset, (offset & 0xe000) >> 13, offset & 0x1fff);
    fprintf(stderr, "        ttl: %u\n", hdr->ttl);
    fprintf(stderr, "   protocol: %u\n", hdr->protocol);
    fprintf(stderr, "        sum: 0x%04x\n", ntoh16(hdr->sum));
    fprintf(stderr, "        src: %s\n", ip_addr_ntop(hdr->src, addr, sizeof(addr)));
    fprintf(stderr, "        dst: %s\n", ip_addr_ntop(hdr->dst, addr, sizeof(addr)));
#ifdef HEXDUMP
    hexdump(stderr, data, len);
#endif
    funlockfile(stderr);
}

char *ip_addr_ntop(ip_addr_t src, char *addr, size_t len)
{
    uint8_t ip[4];
    // int l;
    for (int i = 0; i < 4; i++)
    {
        ip[i] = (src >> (i * 8)) & 0xff;
    }

    snprintf(addr, len, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    return addr;
}


ip_addr_t *ip_addr_pton(ip_addr_t *src, char addr, size_t len)
{
    char *tp;
    tp = strtok(addr, ".");
    if (sizeof(tp) / sizeof(tp[0]) != 4){
        errorf("strtok() failure, size=%d", sizeof(tp) / sizeof(tp[0]));
        return NULL;
    }
    
    src = 0;
    for (int i = 0; i < 4; i++)
    {
        uint8_t ip = (uint8_t)atoi(tp[i]);
        src |= (ip_addr_t)(ip << (i * 8));
    }
    return src;
}


struct ip_iface *ip_iface_alloc(const char *unicast, const char *netmask){
    struct ip_iface *iface;
    
    iface = calloc(1, sizeof(*iface));
    if (!iface) {
        errorf("calloc() failure");
        return NULL;
    }
    NET_IFACE(iface)->family = NET_IFACE_FAMILY_IP;

    
}
