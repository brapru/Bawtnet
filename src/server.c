#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "event.h"
#include "server.h"
#include "netwerking.h"

/* Global Vars */
struct commandServer server; /* Server global state  */

void asciiArt(void){
#include "asciilogo.h"

        char *buf = malloc(1024*16);

        snprintf(buf, 1024*16, ascii_logo, 
                KRED, 
                KNRM, KRED, 
                KNRM, KRED, server.port,
                KNRM, KRED, 
                KNRM, KRED, (long) getpid(),
                KNRM
        );
  
        printf("%s", buf);

        free(buf);
}

/* ==== Server Configurations ====  */

int openBindAddress(int port, int *fd, int *count){
        *fd = initTcpServer(port);
        (*count)++; 
        netNonBlock(NULL, *fd);
        

        return 0;
}

void initServer(void){
        server.pid = getpid();
        server.port = DEFAULT_PORT;
        server.ipfd_count = 0;
                
        /* Open Sockets */
        
        openBindAddress(server.port, server.ipfd, &server.ipfd_count);
        
        /* Initialize Server Commands */

        /* Handle Events  */
        //server.event_loop = createEventLoop();
}

int main(void){
  
        initServer();
        asciiArt();
        
        //eventMain();
        
        return 0;
}
