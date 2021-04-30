#include <signal.h>

#include "util.h"
#include "net.h"
#include "driver/null.h"
#include "test.h"

volatile sig_atomic_t terminate = 0;
char *info = NULL;

void on_signal(int signum) {
  terminate = 1;
}

int main(int argc, char *argv[])
{
    struct net_device *dev;

    signal(SIGINT, on_signal);
    if (net_init() == -1)
    {
        errorf("net_init() failure");
        return -1;
    }
    dev = null_init();
    if (!dev)
    {
        errorf("null_init() failure");
        return -1;
    }
    if (net_run() == -1)
    {
        errorf("net_run() failure");
        return -1;
    }
    while (!terminate)
    {
        if (net_device_output(dev, 0x0800, test_data, sizeof(test_data), NULL) == -1)
        {
            errorf("net_device_output() failure");
            break;
        }
        sleep(1);
    }
    net_shutdown();
    return 0;
}