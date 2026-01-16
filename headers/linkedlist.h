#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>


typedef struct linkedlist {
    void *entry;
    struct linkedlist *next;
} LinkedList;

LinkedList *linkedlist_new(void *entry);
void linkedlist_append(LinkedList *head, void *entry);
void linkedlist_free(void (*free_func)(void *), LinkedList *curr);
LinkedList *linkedlist_match(bool (*match_func)(void *, void *), void *matching_entry,
        LinkedList *head);
LinkedList *linkedlist_remove(bool (*match_func)(void *, void *), void (*free_func)(void *),
        void * matching_entry, LinkedList *head);

#endif
