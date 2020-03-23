#ifndef __EVENT_H
#define __EVENT_H

#include <sys/epoll.h>

#define EVENT_OK 0
#define EVENT_ERR -1
#define EVENT_READ 1
#define EVENT_WRITE 2

#define MAX_EVENTS 10

struct eventLoop {
        void *data; /* Holds the state of the epoll data */
        
};

struct tempState {
        int epfd; /* Temporarily stores the epoll file descriptor */
        struct epoll_event *events;
};

struct eventLoop *createEpollEventLoop();
int addEpollEvent(struct eventLoop *event_loop, int sockfd, int mask);

int epollWait(struct eventLoop *event_loop, int sockfd);
int epollCreate();

int processEvents(struct eventLoop *event_loop, int sockfd);
void eventMain(struct eventLoop *event_loop, int sockfd);

#endif
