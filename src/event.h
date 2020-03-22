#ifndef __EVENT_H
#define __EVENT_H

#define EVENT_OK 0
#define EVENT_ERR -1
#define EVENT_READ 1
#define EVENT_WRITE 2

#define MAX_EVENTS 10

int createEpollEventLoop();
int createEpollEvent(int epfd, int sockfd, int mask);
int epollWait(int epfd, int sockfd);

int processEvents(int epfd, int sockfd);
void eventMain(int epfd, int sockfd);

#endif
