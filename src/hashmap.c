#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "../headers/hashmap.h"

int HASHMAP_SIZE_LIM = 1000;


void hashmap_visualiser(Hashmap **map) {
    for (int i = 0; i < HASHMAP_SIZE_LIM; i++) {
        if (map[i] == NULL) continue;

        printf("[%d]:", i);
        hashmap_entry_visualiser(map[i]);
    }
}

void hashmap_entry_visualiser(Hashmap *curr) {
    while (curr != NULL) {
        printf(" %s ->", curr->key);
        curr = curr->next;
    }
    printf("NULL\n");
}

// pops a value from the hashmap
void *hashmap_pop(Hashmap **map) {
    int entry = -1;
    for (int i = 0; i < HASHMAP_SIZE_LIM; i++) {
        if (map[i] != NULL) {
            entry = i;
            break;
        }
    }

    if (entry == -1) {
        // nothing to pop, hashmap empty
        return NULL;
    }
    void *ret = map[entry];
    map[entry] = NULL;
    return ret;
}

void hashmap_remove(char *key, Hashmap **map) {
    uint32_t i = hash(key);
    Hashmap *curr = map[i];
    Hashmap *prev = map[i];

    if (curr == NULL) {
        printf("hashmap indexing died\n");
        exit(1);
    }

    // terminates when:
    // 0. key matches
    // 1. last item in ll
    while ((strcmp(curr->key, key) != 0) && curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }

    // found match
    if (strcmp(curr->key, key) == 0) {
        // first item
        if (prev == curr) {
            map[i] = curr->next;
        } else {
            prev = curr->next;
        }
        free(curr->key);
        free(curr->entry);
        free(curr);
    } else{
        // not found
        printf("hashmap indexing died\n");
        exit(1);
    }
}

Hashmap *hashmap_find(char *key, Hashmap **map) {
    uint32_t i = hash(key);
    Hashmap *curr = map[i];

    if (curr == NULL) {
        printf("hashmap indexing died\n");
        exit(1);
    }

    // terminates when:
    // 0. key matches
    // 1. last item in ll
    while ((strcmp(curr->key, key) != 0) && curr->next != NULL) {
        curr = curr->next;
    }


    // found
    if (strcmp(curr->key, key) == 0) {
        return curr;
    } 
    exit(1);
}

void hashmap_insert(char *key, void *entry, Hashmap **map) {
    uint32_t i = hash(key);
    Hashmap *curr = map[i];

    if (map[i] == NULL) {
        map[i] = hashentry_new(key, entry);
        return;
    }

    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = hashentry_new(key, entry);
    return;
}

Hashmap *hashentry_new(char *key, void *entry) {
    Hashmap *ret = malloc(sizeof(Hashmap));
    char *copy = malloc((strlen(key) + 1) * sizeof(char));
    memcpy(copy, key, strlen(key) + 1);
    copy[strlen(key)] = 0;

    ret->key = copy;
    ret->next = NULL;
    ret->entry = entry;
    return ret;
}

Hashmap **hashmap_new() {
    Hashmap **ret = malloc(HASHMAP_SIZE_LIM * sizeof(Hashmap *));
    for (int i = 0; i < HASHMAP_SIZE_LIM; i++) ret[i] = NULL;

    return ret;
}

void hashmap_free(Hashmap **map) {
    for (int i = 0; i < HASHMAP_SIZE_LIM; i++) {
        // free linked list in slot
        hashmap_entry_free(map[i]);
    }

    free(map);
}

void hashmap_entry_free(Hashmap *head) {
    if (head == NULL) return;

    hashmap_entry_free(head->next);

    free(head->entry);
    free(head->key);
    free(head);
}

uint32_t hash(char *name) {
    int len = strlen(name);
    int h = 0;
    for (int i = 0; i < len; i++) h += name[i] * 22 + 51;

    return h % 1000;
}
