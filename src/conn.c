#include <stdio.h>
#include <stdlib.h>

#include "conn.h"
#include "server.h"

#define UNUSED(X) (void)(X)

/* === Handle Connecting Clients ===  */
struct connection *connCreateConnection(struct eventLoop *event_loop, int fd, int mask){
        // Initialize the connection structure


        // Add it to the epoll loop 
        addEpollEvent(event_loop, fd, mask, connHandleData);
}

void connHandleDisconnection(int fd){
        listNode *ln;
        ln = searchKeyinList(ln, c);
        delNodeFromList(server.victims, ln);   
        
         // Close socket
        if (netClose(fd) < 0)
                perror("netClose");

        // Delete the client from active connection linked list
        // Free memory from list, and client struct
}

void connHandleData(struct eventLoop *event_loop, int fd, int mask){
        UNUSED(event_loop);
        UNUSED(mask);
        
        if (netRead(fd) == 0)
               connHandleDisconnection(fd); 
        
        //netWrite(fd); 
}
