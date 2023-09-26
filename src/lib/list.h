#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list_node {
        void* data;
        struct list_node* next;
} ListNode;

typedef struct list {
        size_t length;
        ListNode* head;
} List;

void list_push(List* head, void* data);
List* list_new();
void list_push(List* list, void* data);
void* list_pop(List* list);
void list_free(List* list);

#endif
