#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "list.h"

struct list *createList(){
        struct list *list = malloc(sizeof(struct list));
        if (list == NULL) return NULL;

        list->head = NULL;
        list->tail = NULL;
        
        list->len = 0;

        return list;
}

// CHANGE: Append it to the end
struct list *addNodeToList(struct list *list, struct client *client){
        struct listNode *node = malloc(sizeof(struct listNode));
        if (node == NULL) return NULL;

        node->data = client;
        node->next = list->head;
        list->head = node;
        
        list->len++;
        printf("Total amount of victims: %lu\n", list->len);
        
        return list;
}

struct listNode *listLast(struct list *list);
struct listNode *searchKeyinList(listNode *node) 
