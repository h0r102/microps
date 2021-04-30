#ifndef LOOPBACK_H
#define LOOPBACK_H

#include "net.h"

#define LOOPBACK_MTU 256

struct net_device *loopback_init(void);

#endif