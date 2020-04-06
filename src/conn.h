#ifndef __CONN_H
#define __CONN_H

#include "event.h"

#define CONN_OK 0
#define CONN_ERR -1

/* === Typedefs and Structs ===  */

struct connection;

typedef void (*ConnectionCallbackFunc)(struct connection *conn);

struct ConnectionType {
        void (*event_callback_handler)(struct eventLoop *event_loop, int fd, int mask, void* clientData);
        int (*write)(struct connection *conn, void *buff, size_t bufflen);
        int (*read)(struct connection *conn, void *buff, size_t bufflen);
        int (*set_read_handler)(struct connection *conn, ConnectionCallbackFunc func);
        int (*set_write_handler)(struct connection *conn, ConnectionCallbackFunc func);
        int (*set_handlers)(struct connection *conn, ConnectionCallbackFunc read_func, ConnectionCallbackFunc write_func);
};

struct connection {
        int fd;
        struct ConnectionType *type;
        ConnectionCallbackFunc read_handler;
        ConnectionCallbackFunc write_handler;
        void *clientData;
        void *priv_data;
};

struct connection *connCreateConnection(int fd);
void connHandleData(struct eventLoop *event_loop, int fd, int mask);
void connEventHandler(struct eventLoop *event_loop, int fd, int mask, void *clientData);

void connSetPrivData(struct connection *conn, void *data);
void *connGetPrivData(struct connection *conn);

/* === ConnectionType Functions ===  */

static int connWrite(struct connection *conn, void *buff, size_t bufflen){
        return conn->type->write(conn, buff, bufflen);
}

static int connRead(struct connection *conn, void *buff, size_t bufflen){
        return conn->type->read(conn, buff, bufflen);
}

static int connSetReadHandler(struct connection *conn, ConnectionCallbackFunc func){
        return conn->type->set_read_handler(conn, func);
}

static int connSetWriteHandler(struct connection *conn, ConnectionCallbackFunc func){
        return conn->type->set_write_handler(conn, func);
}

static int connSetHandlers(struct connection *conn, ConnectionCallbackFunc read_func, ConnectionCallbackFunc write_func){
        return conn->type->set_handlers(conn, read_func, write_func);
}

#endif
