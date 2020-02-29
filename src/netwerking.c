#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>         // for socket(), bind(), connect()
#include <arpa/inet.h>          // for sockaddr_in and inet_ntoa()

#include "netwerking.h"


//initTcpServer(const char *addr, int port, const char *source_addr, int flags){
int initTcpServer(int port){
        // Initialize the socket connection in here. Too tired tonight
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

        if (bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
                perror("ERROR bind");

        //netListen()
        if (listen(fd, 64) < 0)
                perror("ERROR listen");

        return fd;
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
