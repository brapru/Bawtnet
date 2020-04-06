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
        conn->clientData = malloc(sizeof(*(conn->clientData)));
        conn->priv_data = malloc(sizeof(*(conn->priv_data)));
        if (conn->type == NULL || conn->clientData == NULL || conn->priv_data == NULL) goto err;

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
       
        if (mask & EVENT_READ){ 
                conn->read_handler(conn);
                printf("Made it inside connEventHandler as read\n");
        }

        if (mask & EVENT_WRITE){
                conn->write_handler(conn);
                printf("Made it inside connEventHandler as write\n");
        }

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
        
        if ((addEpollEvent(server.event_loop, conn->fd, EVENT_READ, conn->type->event_callback_handler, conn) == CONN_ERR))
                return CONN_ERR;       
        
        return CONN_OK;
}

int connSocketSetWriteHandler(struct connection *conn, ConnectionCallbackFunc func){
        if (conn->write_handler == func)
                return CONN_OK;
        
        conn->write_handler = func;
        if ((addEpollEvent(server.event_loop, conn->fd, EVENT_WRITE, conn->type->event_callback_handler, conn) == CONN_ERR))
                return CONN_ERR;       
        
        return CONN_OK;
}

int connSocketSetHandlers(struct connection *conn, ConnectionCallbackFunc read_func, ConnectionCallbackFunc write_func){
        conn->write_handler = write_func;
        conn->read_handler = read_func;
        
        if ((addEpollEvent(server.event_loop, conn->fd, EVENT_BOTH, conn->type->event_callback_handler, conn) == CONN_ERR))
                return CONN_ERR;       
        
        return CONN_OK;
}

void connSetPrivData(struct connection *conn, void *data){
        conn->priv_data = data;
}

void *connGetPrivData(struct connection *conn){
        return conn->priv_data;
}

struct ConnectionType CT_Struct = {
        .event_callback_handler = connEventHandler,
        .write = connSocketWrite,
        .read = connSocketRead,
        .set_read_handler = connSocketSetReadHandler,
        .set_write_handler = connSocketSetWriteHandler,
        .set_handlers = connSocketSetHandlers
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
