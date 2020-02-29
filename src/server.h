#ifndef __SERVER_H
#define __SERVER_H

#include <unistd.h>

#include "netwerking.h"

#define DEFAULT_PORT 1337
#define MAX_BIND 10

struct commandServer {
        pid_t pid; 
        int port;
        int ipfd[MAX_BIND]; /* TCP Binds */
        int ipfd_count;
        char neterr[ANET_ERR_LEN];
};

#endif
