#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event.h"
//#include "netwerking.h"

struct eventLoop *createEpollEventLoop(){
        
        struct eventLoop *event_loop = malloc(sizeof(struct eventLoop));
        if (event_loop == NULL) goto err;
        
        event_loop->data = malloc(sizeof(*(event_loop->data)));
        event_loop->events = malloc(sizeof(struct eventCallback)*10128);
        event_loop->ready = malloc(sizeof(struct readyEvents)*10128);
        if (event_loop->data == NULL || event_loop->events == NULL || event_loop ->ready == NULL) goto err;

        struct tempState *state = malloc(sizeof(struct tempState)); 
        if (state == NULL) goto err;
        
        state->events = malloc(sizeof(struct epoll_event)*10128);
        if (!state->events) goto err;

        state->epfd = epollCreate();
        event_loop->data = state;
        
        return event_loop;

err:
        if (event_loop){
                free(event_loop->ready);
                free(event_loop->events);
                free(event_loop->data);
                free(event_loop);
        }

        return NULL; 
}

int addEpollEvent(struct eventLoop *event_loop, int fd, int mask, eventFunc *func, void *clientData) {
        struct tempState *state = event_loop->data;
        
        struct epoll_event ee;
        
        ee.events = 0;
        if (mask == EVENT_BOTH)
                ee.events = EPOLLIN|EPOLLOUT;
        if (mask & EVENT_READ) 
                ee.events |= EPOLLIN; 
        if (mask & EVENT_WRITE) 
                ee.events |= EPOLLOUT;

        ee.data.fd = fd;
        
        if (epoll_ctl(state->epfd, EPOLL_CTL_ADD, fd, &ee) < 0){
                perror("epoll_ctl");
                exit(1);
        }
        struct eventCallback *event = &event_loop->events[fd];

        event->clientData = clientData;        
        if (mask & EVENT_READ) 
                event->rfunc = func;
        if (mask & EVENT_WRITE) 
                event->wfunc = func;

        return EVENT_OK;
}

int processEvents(struct eventLoop *event_loop){
        int i, numevents = 0; 
        numevents = epollWait(event_loop);
        
        // Fire off the read/write functions that are ready.  
        for (i = 0; i < numevents; i++){
                int fd = event_loop->ready[i].fd;
                int mask = event_loop->ready[i].mask;
                int called = 0; /* Events called for current fd */

                /* Here we use the fd var initialized above, specific to this current event */
                struct eventCallback *callback = &event_loop->events[fd];  
        
                /* Call the read functions first  */
                if (mask & EVENT_READ){
                        //if (callback->rfunc != callback->wfunc){
                                callback->rfunc(event_loop, fd, mask, callback->clientData);
                                called++;
                        }
                
                /* Call the write functions  */
                if (mask & EVENT_WRITE)
                        if (!called || callback->wfunc != callback->rfunc){
                                callback->wfunc(event_loop, fd, mask, callback->clientData);
                                called++;
                        }
        }        

        return 0;
}

void eventMain(struct eventLoop *event_loop){
        while (1) {
                processEvents(event_loop);
        }
}

/* === epoll command functions  === */

int epollCreate(){
        int epoll_fd = epoll_create1(0);

        if (epoll_fd == -1)
                perror("epollCreate");

        return epoll_fd;
}

int epollWait(struct eventLoop *event_loop){
        struct tempState *state = event_loop->data;
        
        int i, numevents = 0;

        numevents = epoll_wait(state->epfd, state->events, MAX_EVENTS, -1); 
         
        if (numevents < 0){
                perror("epoll_wait");
                exit(1);
        }

        for (i = 0; i < numevents; i++){
                struct epoll_event *e = state->events+i;
                int mask;
                if (e->events & EPOLLIN) mask = EVENT_READ;
                if (e->events & EPOLLOUT) mask = EVENT_WRITE;
                
                event_loop->ready[i].fd = e->data.fd;
                event_loop->ready[i].mask = mask;
        }

        return numevents;
}
