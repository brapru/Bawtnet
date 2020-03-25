#ifndef __NETWERKING_H
#define __NETWERKING_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "event.h"

#define NET_OK 0
#define NET_ERR -1
#define NET_ERR_LEN 256

/* === Tcp Initialization and Handle Functions === */
/* These handle networking connections, and are used as callback functions for
 * ready events  */
int initTcpServer(int port);
void handleAcceptTcp(struct eventLoop *event_loop, int fd, int mask);


/* === Linux sys/socket Functions  ===  */
int netSetBlock(char *err, int fd, int non_block);
int netNonBlock(char *err, int fd);
int netBlock(char *err, int fd);
int netAccept(int s, struct sockaddr *sa, socklen_t *len);
int netListen(int s, struct sockaddr *sa, socklen_t len);
void netRead(int fd);
void netWrite(int fd);

/* === Network Connections === */


#endif
