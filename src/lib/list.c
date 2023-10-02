#include <stdio.h>

#include "list.h"

List*
list_new()
{
        List* new_list = (List*) malloc(sizeof(List));
        new_list->head = NULL;
        new_list->length = 0;
        return new_list;
}

void
list_push(List* list, void* data)
{
        ListNode* new_node = (ListNode*) malloc(sizeof(ListNode));
        new_node->data = data;
        new_node->next = list->head;
        list->head = new_node;
        list->length++;
}

void*
list_pop(List* list)
{
        if (list->length == 0)
                return NULL;

        ListNode* head_node = list->head;
        void* data = head_node->data;
        list->head = head_node->next;
        free(head_node);
        list->length--;
        return data;
}

void
list_free(List* list)
{
        ListNode* current_node = list->head;
        while (current_node != NULL) {
                ListNode* next_node = current_node->next;
                free(current_node);
                current_node = next_node;
        }
        free(list);
}

void list_print(List* list)
{
        printf("List <>");
}
