#ifndef __EVENT_H
#define __EVENT_H

#include <sys/epoll.h>

#define EVENT_OK 0
#define EVENT_ERR -1
#define EVENT_READ 1
#define EVENT_WRITE 2
#define EVENT_BOTH 3

#define MAX_EVENTS 100

/* === Typedefs and Structs === */
struct eventLoop;

/* Callback function pointer to temporarily store functions associated with events */ 
typedef void eventFunc(struct eventLoop *event_loop, int fd, int mask, void *clientData);

/* The callback struct to hold a read function and a write function. Read/write
 * status is assiged in addEpollEvent */
struct eventCallback {
        eventFunc *rfunc;
        eventFunc *wfunc;
        void *clientData;
};

/* Used to more easily manage event_loop file descriptors, and store ready events for calling. Assigned in epollWait()  */
struct readyEvents {
        int fd;
        int mask;
}; 

/* The overall epoll_create1() structure. */
struct eventLoop {
        void *data; /* Holds the current state of the epoll data, including the epoll_create file descriptor */
        struct eventCallback *events; /* Holds the read/write functions. Will be called in processEvents() */        
        struct readyEvents *ready; /*   */
};

/* Struct to more easily manage event_loop descriptors. Stores the event_loop
 * epoll_create1() file descriptor */
struct tempState {
        int epfd; /* Temporarily stores the epoll file descriptor */
        struct epoll_event *events;
};


/* === Event Functions === */
struct eventLoop *createEpollEventLoop();
int addEpollEvent(struct eventLoop *event_loop, int fd, int mask, eventFunc *func, void *clientData);

int epollWait(struct eventLoop *event_loop);
int epollCreate();

int processEvents(struct eventLoop *event_loop);
void eventMain(struct eventLoop *event_loop);

#endif
