#include "hashmap.h"

#include <stdlib.h>

struct Hashmap *init_hashmap(int sizeOfKey, int sizeOfValue)
{
    struct Hashmap *map = malloc(sizeof(struct Hashmap));

    map->capacity = DEFAULT_CAPACITY;
    map->loadFactor = DEFAULT_LOAD_FACTOR;

    map->sizeOfKey = sizeOfKey;
    map->sizeOfValue = sizeOfValue;

    map->keyCount = 0;
    map->table = malloc(sizeof(struct Node*) * map->capacity);
    
    for(int i = 0; i < map->capacity; i++) {
        map->table[i] = NULL;
    }
    
    return map;
}

void my_memcpy(void *dest, void *src, int size) {
    char *cdest = dest;
    char *csrc = src;
    for(int i = 0; i < size; i++) {
        cdest[i] = csrc[i];
    }
}

int my_memcmp(void *a, void *b, int size) {
    char *ca = a;
    char *cb = b;
    for(int i = 0; i < size; i++) {
        if(ca[i] != cb[i])
            return -1;
    }

    return 0;
}

struct Node *create_node(int sizeOfKey, void *key, int sizeOfValue, void *value) {
    struct Node *new = malloc(sizeof(struct Node));

    new->key = malloc(sizeOfKey);
    my_memcpy(new->key, key, sizeOfKey);

    new->value = malloc(sizeOfValue);
    my_memcpy(new->value, value, sizeOfValue);
    
    new->next = NULL;

    return new;
}


void free_node(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *tmp = current;
        current = current->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

void free_hashmap(struct Hashmap *map)
{
    for(int i = 0; i < map->capacity; i++) {
        free_node(map->table[i]);
    }
    free(map->table);
    free(map);
}

unsigned int hash(struct Hashmap *map, void* key) {
    // DJB2
    unsigned char *bytes = key;
    unsigned int hash = 5381;
    for(int i = 0; i < map->sizeOfKey; i++) {
        hash = ((hash << 5) + hash) + bytes[i];
    } 

    return hash % map->capacity;
}

void resize(struct Hashmap *map) {

    int count;
    struct Node *keys = get_keys_as_array(map, &count);

    for(int i = 0; i < map->capacity; i++) {
        free_node(map->table[i]);
    }

    map->capacity *= 2;
    map->table = realloc(map->table, sizeof(struct Node *) * map->capacity);

    for(int i = 0; i < map->capacity; i++) {
        map->table[i] = NULL;
    }

    map->keyCount = 0;

    for(int i = 0; i < count; i++) {
        put(map, keys[i].key, keys[i].value);
    }

    free_keys(keys, count);
}

enum HashMapReturnValue put(struct Hashmap *map, void *key, void *value)
{
    if ((float)map->keyCount / map->capacity * 100 >= map->loadFactor)
        resize(map);

    unsigned int hash_key = hash(map, key);
    struct Node **current = &map->table[hash_key];
    while (*current != NULL) {
        if (my_memcmp((*current)->key, key, map->sizeOfKey) == 0) {
            my_memcpy((*current)->value, value, map->sizeOfValue);
            return SUCCESS;
        }
        current = &(*current)->next;
    }
    *current = create_node(map->sizeOfKey, key, map->sizeOfValue, value);
    map->keyCount++;

    return SUCCESS;
}

enum HashMapReturnValue get(struct Hashmap *map, void *key, void *res)
{
    unsigned int hash_key = hash(map, key);
    struct Node *current = map->table[hash_key];

    while(current != NULL) {
        if(my_memcmp(current->key, key, map->sizeOfKey) == 0) {
            if(res != NULL)
                my_memcpy(res, current->value, map->sizeOfValue);
            return SUCCESS;
        }
        current = current->next;
    }

    return KEY_UNKNOW;
}

enum HashMapReturnValue del(struct Hashmap *map, void *key, void *res)
{
    unsigned int hash_key = hash(map, key);
    
    struct Node *before = NULL;
    struct Node *current = map->table[hash_key];

    while(current != NULL) {
        if(my_memcmp(current->key, key, map->sizeOfKey) == 0) {
            if(res != NULL)
                my_memcpy(res, current->value, map->sizeOfValue);

            if(before == NULL)
                map->table[hash_key] = current->next;
            else
                before->next = current->next;
            
            map->keyCount--;

            free(current->key);
            free(current->value);
            free(current);

            return SUCCESS;
        }
        before = current;
        current = current->next;
    }

    return KEY_UNKNOW;
}

struct Node* get_keys_as_array(struct Hashmap *map, int *count)
{
    *count = map->keyCount;

    if(map->keyCount == 0)
        return NULL;

    struct Node *res = malloc(sizeof(struct Node) * map->keyCount);

    int key_count = 0;
    for (int i = 0; i < map->capacity; i++) {
        struct Node *current = map->table[i];
        while (current != NULL) {
            res[key_count].key = malloc(map->sizeOfKey);
            my_memcpy(res[key_count].key, current->key, map->sizeOfKey);
            
            res[key_count].value = malloc(map->sizeOfValue);
            my_memcpy(res[key_count].value, current->value, map->sizeOfValue);

            res[key_count].next = NULL;
            key_count++;
            current = current->next;
        }
    }

    return res;
}

void free_keys(struct Node* keys, int count) {
    if(keys == NULL)
        return;
    
    for(int i = 0; i < count; i++) {
        free(keys[i].key);
        free(keys[i].value);
    }

    free(keys);
}