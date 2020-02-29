#ifndef __NETWERKING_H
#define __NETWERKING_H

#define ANET_ERR_LEN 256

int initTcpServer(int port);
int netSetBlock(char *err, int fd, int non_block);
int netNonBlock(char *err, int fd);
int netBlock(char *err, int fd);

#endif
