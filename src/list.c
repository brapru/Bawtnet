#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "list.h"
//#include "server.h"

struct list *createList(){
        struct list *list = malloc(sizeof(struct list));
        if (list == NULL) return NULL;

        list->head = NULL;
        list->tail = NULL;
        
        list->len = 0;

        return list;
}

// CHANGE: Append it to the end
struct list *addNodeToList(struct list *list, void *data){
        struct listNode *node = malloc(sizeof(struct listNode));
        if (node == NULL) return NULL;

        node->data = data;
        node->next = list->head;
        list->head = node;
        
        list->len++;
        printf("Total amount of victims: %lu\n", list->len);
        
        return list;
}

struct listNode *listFirst(struct list *list){
        return list->head;
}

struct listNode *listLast(struct list *list){
        return list->tail;
}
