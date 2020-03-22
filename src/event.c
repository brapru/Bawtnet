#include <stdio.h>
#include <sys/epoll.h>

#include "event.h"
#include "netwerking.h"

// Change to returning an event data structure. Include copy of state struct
int createEpollEventLoop(){
         
        // state->epfd = epollCreate();
        int epoll_fd = epoll_create1(0);
        
        if (epoll_fd == -1){
                perror("epollCreate");
        }

        // eventLoop->data = state;
        // return eventLoop
        return epoll_fd;
}

int createEpollEvent(int epfd, int sockfd, int mask) {
        struct epoll_event ee;
       
        ee.events = 0;
        if (mask & EVENT_READ)
                ee.events |= EPOLLIN; 
        if (mask & EVENT_WRITE)
                ee.events |= EPOLLOUT;

        ee.data.fd = sockfd;

        if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ee) < 0)
                perror("epoll_ctl");

        return EVENT_OK;
}

int epollWait(int epfd, int sockfd){
        int i, numevents = 0;
        struct epoll_event events[MAX_EVENTS];

        numevents = epoll_wait(epfd, events, MAX_EVENTS, -1); 
         
        if (numevents < 0)
                perror("epoll_wait");

        for (i = 0; i < numevents; i++){
                //if (events[i].data.fd & EPOLLIN)
                //if (events[i].data.fd & EPOLLOUT)
                if (events[i].data.fd == sockfd){
                        printf("New socket connected: %d\n", events[i].data.fd);
                        netAcceptTcp(sockfd);
                        netNonBlock(NULL, sockfd);
                }
        }
        
        return numevents;
}

int processEvents(int epfd, int sockfd){
        // epollWait will poll the file descriptors, and will set the
        // read/write functions accordingly. 
        epollWait(epfd, sockfd);
        
        // Fire off the read/write functions that are ready.  
        
        return 0;
}

void eventMain(int epfd, int sockfd){
        while (1) {
                processEvents(epfd, sockfd);
        }
}
