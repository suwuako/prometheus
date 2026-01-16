#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../headers/linkedlist.h"


LinkedList *linkedlist_new(void *entry) {
    LinkedList *r = malloc(sizeof(LinkedList));
    r->entry = entry;
    r->next = NULL;

    return r;
}

void linkedlist_append(LinkedList *head, void *entry) {
    LinkedList *item = linkedlist_new(entry);
    LinkedList *curr = head;

    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = item;
}

void linkedlist_free(void (*free_func)(void *), LinkedList *curr) {
    free_func(curr->entry);
    free(curr);
}

LinkedList *linkedlist_match(bool (*match_func)(void *, void *), void *matching_entry,
        LinkedList *head) {
    LinkedList *curr = head;

    while (curr != NULL) {
        if (match_func(matching_entry, curr->entry)) {
            return curr;
        }
    }

    return NULL;
}

LinkedList *linkedlist_remove(bool (*match_func)(void *, void *), void (*free_func)(void *),
        void * matching_entry, LinkedList *head) {
    LinkedList *curr = head;
    LinkedList *prev = curr;


    while (curr != NULL) {
        if (match_func(matching_entry, curr->entry)) {
            // match occurred
            if (prev == curr) {
                // removal item at head
                LinkedList *ret = curr->next;
                linkedlist_free(free_func, curr);

                return ret;
            } else {
                // remove item, relink
                prev->next = curr->next;
                linkedlist_free(free_func, curr);
            }
        }

        prev = curr;
        curr = curr->next;
    }
    // failed
    printf("Linkedlist remove failed...\n");
    return head;
}

