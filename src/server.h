#ifndef __SERVER_H
#define __SERVER_H

#include <unistd.h>

#include "netwerking.h"

#define DEFAULT_CLI_PORT 6000
#define DEFAULT_VICTIM_PORT 6001
#define MAX_BIND 10

struct commandServer {
        pid_t pid; 
        
        int cli_port;
        int clifd[MAX_BIND]; /* CLI TCP Socket file descriptors*/
        int clifd_count;
        int victim_port; 
        int victimfd[MAX_BIND]; /* Victim TCP Socket file descriptors*/
        int victimfd_count;
        

        char neterr[NET_ERR_LEN];
};

#endif
