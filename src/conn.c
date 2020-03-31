#include <stdio.h>
#include <stdlib.h>

#include "conn.h"
#include "server.h"

#define UNUSED(X) (void)(X)

struct ConnectionType CT_Struct;

/* === Handle Connecting Clients ===  */
struct connection *connCreateConnection(int fd){
        // Initialize the connection structure
        struct connection *conn = malloc(sizeof(struct connection)); 
        conn->type = malloc(sizeof(*(conn->type)));
        if (conn->type == NULL) goto err;

        conn->fd = fd;
        conn->type = &CT_Struct;

        return conn;

err:
        if (conn){
                free(conn->type);
                free(conn);
        }
        
        return NULL;
}

/* === ConnectionType Functions ===  */

void connEventHandler(struct eventLoop *event_loop, int fd, int mask, void *clientData){
        struct connection *conn = clientData;

        UNUSED(event_loop);
        UNUSED(fd);
        UNUSED(mask);
       
        if (mask & EVENT_READ) 
                conn->read_handler(conn);
}

int connSocketWrite(struct connection *conn, void *buff, size_t bufflen){
        int retval  = write(conn->fd, buff, bufflen);

        if (retval < 0)
                perror("write");
        
        return retval;
}

int connSocketRead(struct connection *conn, void *buff, size_t bufflen){
        int retval = read(conn->fd, buff, bufflen);
        return retval;
}

int connSocketSetReadHandler(struct connection *conn, ConnectionCallbackFunc func){
        if (conn->read_handler == func)
                return CONN_OK;
        
        conn->read_handler = func;
        // add ClientData? 
        if ((addEpollEvent(server.event_loop, conn->fd, EVENT_READ, conn->type->event_callback_handler, conn) == CONN_ERR))
                return CONN_ERR;       
        
        return CONN_OK;
}

int connSocketSetWriteHandler(struct connection *conn, ConnectionCallbackFunc func){
        if (conn->write_handler == func)
                return CONN_OK;
        
        conn->write_handler = func;
        // add ClientData? 
        if ((addEpollEvent(server.event_loop, conn->fd, EVENT_WRITE, conn->type->event_callback_handler, conn) == CONN_ERR))
                return CONN_ERR;       
        
        return CONN_OK;
}

struct ConnectionType CT_Struct = {
        .event_callback_handler = connEventHandler,
        .write = connSocketWrite,
        .read = connSocketRead,
        .set_read_handler = connSocketSetReadHandler,
        .set_write_handler = connSocketSetWriteHandler
};

//void connHandleDisconnection(struct client *c){
//        //struct listNode *ln;
//        //ln = searchKeyinList(ln, c);
//        //delNodeFromList(server.victims, ln);   
//        //
//        // // Close socket
//        //if (netClose(fd) < 0)
//                perror("netClose");
//
//        // Delete the client from active connection linked list
//        // Free memory from list, and client struct
//}

void connHandleData(struct eventLoop *event_loop, int fd, int mask){
        UNUSED(event_loop);
        UNUSED(mask);
        UNUSED(fd);

        //if (netRead(fd) == 0)
        //       connHandleDisconnection(fd); 
        
        //netWrite(fd); 
}
