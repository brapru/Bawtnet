#ifndef __NETWERKING_H
#define __NETWERKING_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.h"
#include "event.h"
#include "list.h"
#include "conn.h"

#define NET_OK 0
#define NET_ERR -1
#define NET_ERR_LEN 256

/* === Data Structures === */

struct client;

struct client {
        int fd;
        char *cmd;
        struct listNode *client_list_node;
};

/* === Tcp Initialization and Handle Functions === */
/* These handle networking connections, and are used as callback functions for
 * ready events  */
int initTcpServer(int port);
void handleCliAccept(struct eventLoop *event_loop, int fd, int mask, void *clientData);
void handleVictimAccept(struct eventLoop *event_loop, int fd, int mask, void *clientData);


/* === Linux sys/socket Functions  ===  */
int netSetBlock(char *err, int fd, int non_block);
int netNonBlock(char *err, int fd);
int netBlock(char *err, int fd);
int netAccept(int s, struct sockaddr *sa, socklen_t *len);
int netListen(int s, struct sockaddr *sa, socklen_t len);
void readDataFromClient(struct connection *conn);
void netWrite(int fd);
int netClose(int fd);

/* === Client Functions === */
struct client *createClient(struct connection *conn);
void linkList(struct list *list, struct client *c);

#endif
