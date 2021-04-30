#include <stddef.h>
#include <stdint.h>

#define NET_DEVICE_TYPE_NULL 0x0000
#define NET_DEVICE_TYPE_LOOPBACK 0x0001

#define NET_DEVICE_FLAG_UP 0x0001
#define NET_DEVICE_FLAG_LOOPBACK 0x0010

#define NET_DEVICE_ADDR_LEN 16

#define NET_DEVICE_IS_UP(x) ((x)->flags & NET_DEVICE_FLAG_UP)
#define NET_DEVICE_STATE(x) (NET_DEVICE_IS_UP(x) ? "up" : "down")

struct net_device; /* forward declaration */

struct net_device
{
    struct net_device *next;
    struct net_iface *ifaces; /* NOTE: if you want to add/delete the entries after net_run(), you need to protect ifaces with a mutex. */
    unsigned int index;
    char name[IFNAMSIZ];
    uint16_t type;
    uint16_t mtu;
    uint16_t flags;
    uint16_t hlen; /* header length */
    uint16_t alen; /* address length */
    uint8_t addr[NET_DEVICE_ADDR_LEN];
    union
    {
        uint8_t peer[NET_DEVICE_ADDR_LEN];
        uint8_t broadcast[NET_DEVICE_ADDR_LEN];
    };
    struct net_device_ops *ops;
    void *priv;
};

struct net_device_ops
{
    int (*open)(struct net_device *dev);
    int (*close)(struct net_device *dev);
    int (*transmit)(struct net_device *dev, uint16_t type, const uint8_t *data, size_t len, const void *dst);
    int (*poll)(struct net_device *dev);
};
