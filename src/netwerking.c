#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>         // for socket(), bind(), connect(), close()
#include <arpa/inet.h>          // for sockaddr_in and inet_ntoa()
#include <unistd.h>

#include "netwerking.h"


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

        // 0 = False : 1 = True
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

int netAcceptTcp(int fd){

        int client;
        struct sockaddr_in address;  
        socklen_t address_len = sizeof(address);

        client = accept(fd, (struct sockaddr*)&address, &address_len);

        return client;
}
