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

void free_node(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *tmp = current;
        current = current->next;
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

struct Node* createNode(int key, int value) {
    struct Node *new = malloc(sizeof(struct Node));

    new->key = key;
    new->value = value;
    new->next = NULL;

    return new;
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

    free(keys);
}

enum HashMapReturnValue put(struct Hashmap *map, int key, int value)
{
    if ((float)map->keyCount / map->capacity * 100 >= map->loadFactor)
        resize(map);

    unsigned int hash_key = hash(map, &key);
    struct Node **current = &map->table[hash_key];
    while (*current != NULL) {
        if ((*current)->key == key) {
            (*current)->value = value;
            map->keyCount++;
            return SUCCESS;
        }
        current = &(*current)->next;
    }
    *current = createNode(key, value);
    map->keyCount++;

    return SUCCESS;
}

enum HashMapReturnValue get(struct Hashmap *map, int key, int *res)
{
    unsigned int hash_key = hash(map, &key);

    struct Node *head = map->table[hash_key];

    while(head != NULL) {
        if(head->key == key) {
            if(res != NULL)
                *res = head->value;

            return SUCCESS;
        }
        head = head->next;
    }

    return KEY_UNKNOW;
}

enum HashMapReturnValue del(struct Hashmap *map, int key, int *res)
{
    unsigned int hash_key = hash(map, &key);
    
    struct Node *before = NULL;
    struct Node *head = map->table[hash_key];

    while(head != NULL) {
        if(head->key == key) {
            if(res != NULL)
                *res = head->value;

            if(before == NULL)
                map->table[hash_key] = head->next;
            else
                before->next = head->next;
            
            map->keyCount--;

            return SUCCESS;
        }
        before = head;
        head = head->next;
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
            res[key_count].key = current->key;
            res[key_count].value = current->value;
            res[key_count].next = NULL;
            key_count++;
            current = current->next;
        }
    }

    return res;
}