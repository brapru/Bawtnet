#ifndef __LIST_H
#define __LIST_H

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
struct list *addNodeToList(struct list *list, void *node);
int delNodefromList(struct list *list, void *node);
struct listNode *listLast(struct list *list);
//struct listNode *searchKeyInList(struct listNode *node);

#endif
