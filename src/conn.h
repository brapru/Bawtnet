#ifndef __CONN_H
#define __CONN_H

#include "event.h"

struct client;

struct client {
        int fd;
};

void connCreateConnection(struct eventLoop *event_loop, int fd, int mask);
void connHandleData(struct eventLoop *event_loop, int fd, int mask);

struct client *createClient(int fd);

#endif
