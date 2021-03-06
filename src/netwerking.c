#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>         // for socket(), bind(), connect(), close()
#include <arpa/inet.h>          // for sockaddr_in and inet_ntoa()
#include <unistd.h>

#include "netwerking.h"

#define UNUSED(X) (void)(X);

/* === Tcp Initialization and Handle Functions === */

int initTcpServer(int port){
        
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0)
                perror("ERROR opening socket");
        
        int opt = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
                perror("ERROR setsockopt");
        
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        if (netListen(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == NET_ERR)
                fd = NET_ERR;

        return fd;
}

void handleAcceptTcp(struct eventLoop *event_loop, int fd, int mask){
        UNUSED(event_loop);
        UNUSED(mask);
        
        struct sockaddr_in address;  
        socklen_t address_len = sizeof(address);

        int conn = netAccept(fd, (struct sockaddr*)&address, &address_len);
        netNonBlock(NULL, conn);
        
        printf("New client connection.\n");

        // TODO: Create the connected client here, and add it to the epoll fd loop
}

/* === Linux sys/socket Functions === */
int netListen(int s, struct sockaddr *sa, socklen_t len){
        if (bind(s, sa, len) == -1){
                close(s);
                return NET_ERR;
        }

        if (listen(s, 64) == -1){
                close(s);
                return NET_ERR;
        }

        return NET_OK;
}

int netSetBlock(char *err, int fd, int non_block){
        int getFlags, setFlags;

        // 0 = Blocking : 1 = Nonblocking  
        getFlags = fcntl(fd, F_GETFL);
        if (non_block)
                getFlags |= O_NONBLOCK;

        setFlags = fcntl(fd, F_GETFL, getFlags);

        if (setFlags == -1){
                printf("%s", err);        
                perror("setFlags");
        }

        return 0;         
}

int netNonBlock(char *err, int fd){
        return netSetBlock(err, fd, 1);
}

int netBlock(char *err, int fd){
        return netSetBlock(err, fd, 0);
}

int netAccept(int s, struct sockaddr *sa, socklen_t *len){
        int client = accept(s, sa, len);
        
        if (client < 0)
                perror("accept");
        
        return client;
}

void netRead(int fd){
        char buff[1024];
        
        if (read(fd, buff, 1024) < 0)
                perror("recv");
        
        printf("Received: %s\n", buff);
}

void netWrite(int fd){
        char *motd = "From Server: MOTD";
        
        if (write(fd, motd, strlen(motd)) < 0)
                perror("write");
}

/* === Handle Connecting Clients ===  */
