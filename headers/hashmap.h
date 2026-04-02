#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>


extern int HASHMAP_SIZE_LIM;

// key: key field
// entry: value field
// *next: next hashmap item OR null
typedef struct hashmap {
    char *key;
    void *entry;
    struct hashmap *next;
} Hashmap;

Hashmap **hashmap_new();
uint32_t hash(char *name);
void hashmap_free(Hashmap **map);
void hashmap_entry_free(Hashmap *head);
void hashmap_insert(char *key, void *entry, Hashmap **map);
Hashmap *hashentry_new(char *key, void *entry);
Hashmap *hashmap_find(char *key, Hashmap **map);
void hashmap_remove(char *key, Hashmap **map);
void hashmap_entry_visualiser(Hashmap *curr);
void hashmap_visualiser(Hashmap **map);
Hashmap *hashmap_pop(Hashmap **map);

#endif
