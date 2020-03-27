#ifndef __LIST_H
#define __LIST_H

#include "conn.h"

struct listNode {
        void *data;
        struct listNode *next;
        struct listNode *prev;
};

struct list {
        struct listNode *head;
        struct listNode *tail;
        unsigned long len;
}; 

struct list *createList();
int addNodeToList(struct list *list, struct client *client);
int delNodefromList(struct list*list, listNode *node);

#endif
