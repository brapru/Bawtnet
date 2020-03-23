#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event.h"
#include "netwerking.h"

// Change to returning an event data structure. Include copy of state struct
struct eventLoop *createEpollEventLoop(){
        
        struct eventLoop *event_loop = malloc(sizeof(struct eventLoop));
        if (event_loop == NULL)
                goto err;

        event_loop->data = malloc(sizeof(*(event_loop->data)));
        if (event_loop->data == NULL)
                goto err;

        struct tempState *state = malloc(sizeof(struct tempState)); 
        if (state == NULL)
                goto err;
                       
        state->events = malloc(sizeof(struct epoll_event) * MAX_EVENTS);
        if (!state->events)
                goto err;

        state->epfd = epollCreate();
        event_loop->data = state;
        
        return event_loop;

err:
        if (event_loop) {
                free(event_loop->data);
                free(event_loop);
        }

        return NULL; 
}

int addEpollEvent(struct eventLoop *event_loop, int sockfd, int mask) {
        struct tempState *state = event_loop->data;
        
        struct epoll_event ee;
        
        ee.events = 0;
        if (mask & EVENT_READ)
                ee.events |= EPOLLIN; 
        if (mask & EVENT_WRITE)
                ee.events |= EPOLLOUT;

        ee.data.fd = sockfd;
        
        if (epoll_ctl(state->epfd, EPOLL_CTL_ADD, sockfd, &ee) < 0)
                perror("epoll_ctl");

        return EVENT_OK;
}

int processEvents(struct eventLoop *event_loop, int sockfd){
        // epollWait will poll the file descriptors, and will set the
        // read/write functions accordingly. 
        epollWait(event_loop, sockfd);
        
        // Fire off the read/write functions that are ready.  
        
        return 0;
}

void eventMain(struct eventLoop *event_loop, int sockfd){
        while (1) {
                processEvents(event_loop, sockfd);
        }
}

/* === epoll command functions  === */

int epollCreate(){
        int epoll_fd = epoll_create1(0);

        if (epoll_fd == -1)
                perror("epollCreate");

        return epoll_fd;
}

int epollWait(struct eventLoop *event_loop, int sockfd){
        struct tempState *state = event_loop->data;
        
        int i, numevents = 0;

        numevents = epoll_wait(state->epfd, state->events, MAX_EVENTS, -1); 
         
        if (numevents < 0)
                perror("epoll_wait");

        for (i = 0; i < numevents; i++){
                //if (events[i].data.fd & EPOLLIN)
                //if (events[i].data.fd & EPOLLOUT)
                // === Problem I think lies here. sockfd is returned, and then
                // just hangs. Need to make sure it's the same sockfd sent to
                // netRead, currently isn't.
                if (state->events[i].data.fd == sockfd){
                        printf("New socket connected: %d\n", state->events[i].data.fd);
                        netAcceptTcp(sockfd);
                        netNonBlock(NULL, sockfd);
                }
                else if (state->events[i].events & EPOLLIN){
                        netRead(state->events[i].data.fd);             
                }
        }
       
        return numevents;
}
