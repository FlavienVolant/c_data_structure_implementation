#include "hashmap.h"

#include <stdlib.h>

Hashmap_t *init_hashmap(hash_f *hash, cpy_f *cpy_key, cmp_f *cmp_key, free_f *free_key, cpy_f *cpy_value, cmp_f *cmp_value, free_f *free_value)
{
    Hashmap_t *map = malloc(sizeof(Hashmap_t));

    map->capacity = DEFAULT_CAPACITY;
    map->loadFactor = DEFAULT_LOAD_FACTOR;

    map->cpy_key = cpy_key;
    map->cmp_key = cmp_key;
    map->free_key = free_key;

    map->cpy_value = cpy_value;
    map->cmp_key = cmp_key;
    map->free_key = free_key;

    map->keyCount = 0;
    map->table = malloc(sizeof(Node_t*) * map->capacity);
    
    for(int i = 0; i < map->capacity; i++) {
        map->table[i] = NULL;
    }

    return map;
}

Node_t *create_node(const Hashmap_t *map, const void *key, const void *value) {
    Node_t *new = malloc(sizeof(Node_t));

    new->key = map->cpy_key(key);
    new->value = map->cpy_value(value);
    new->next = NULL;

    return new;
}

void free_node(const Hashmap_t *map, Node_t *head) {
    Node_t *current = head;

    while (current != NULL) {
        Node_t *tmp = current;
        current = current->next;
        map->free_key(tmp->key);
        map->free_value(tmp->value);
        free(tmp);
    }
}

void free_hashmap(Hashmap_t *map){
    for(int i = 0; i < map->capacity; i++) {
        free_node(map, map->table[i]);
    }
    free(map->table);
    free(map);
}

unsigned int hash(Hashmap_t *map, void *key) {
    return map->hash(key) % map->capacity;
}

void resize(Hashmap_t *map) {

    int count;
    Node_t *keys = get_keys_as_array(map, &count);

    for(int i = 0; i < map->capacity; i++) {
        free_node(map, map->table[i]);
    }

    map->capacity *= 2;
    map->table = realloc(map->table, sizeof(Node_t *) * map->capacity);

    for(int i = 0; i < map->capacity; i++) {
        map->table[i] = NULL;
    }

    map->keyCount = 0;

    for(int i = 0; i < count; i++) {
        put(map, keys[i].key, keys[i].value);
    }

    free_keys(map, keys, count);
}

HashMapReturnValue put(Hashmap_t *map, void *key, void *value) {
    if ((float)map->keyCount / map->capacity * 100 >= map->loadFactor)
        resize(map);

    unsigned int hash_key = hash(map, key);
    Node_t **current = &map->table[hash_key];
    while (*current != NULL) {
        if (map->cmp_key((*current)->key, key) == 0) {
            map->free_value((*current)->value);
            (*current)->value = map->cpy_value(value);
            return SUCCESS;
        }
        current = &(*current)->next;
    }
    *current = create_node(map, key, value);
    map->keyCount++;

    return SUCCESS;
}

HashMapReturnValue get(Hashmap_t *map, void *key, void **res) {
    unsigned int hash_key = hash(map, key);
    Node_t *current = map->table[hash_key];

    while(current != NULL) {
        if(map->cmp_key(current->key, key) == 0) {
            if(res != NULL)
                *res = map->cpy_value(current->value);
            return SUCCESS;
        }
        current = current->next;
    }

    return KEY_UNKNOW;
}

HashMapReturnValue del(Hashmap_t *map, void *key, void **res) {
    unsigned int hash_key = hash(map, key);
    
    Node_t *before = NULL;
    Node_t *current = map->table[hash_key];

    while(current != NULL) {
        if(map->cmp_key(current->key, key) == 0) {
            if(res != NULL)
                *res = map->cpy_value(current->value);

            if(before == NULL)
                map->table[hash_key] = current->next;
            else
                before->next = current->next;
            
            map->keyCount--;

            map->free_key(current->key);
            map->free_value(current->value);
            free(current);

            return SUCCESS;
        }

        before = current;
        current = current->next;
    }

    return KEY_UNKNOW;
}

Node_t* get_keys_as_array(const Hashmap_t *map, int *count) {
    *count = map->keyCount;

    if(map->keyCount == 0)
        return NULL;

    Node_t *res = malloc(sizeof(Node_t) * map->keyCount);

    int key_count = 0;
    for (int i = 0; i < map->capacity; i++) {
        Node_t *current = map->table[i];
        while (current != NULL) {
            res[key_count].key = map->cpy_key(current->key);
            res[key_count].value = map->cpy_value(current->value);
            res[key_count].next = NULL;
            key_count++;
            current = current->next;
        }
    }

    return res;
}

void free_keys(const Hashmap_t *map, Node_t* keys, int count) {
    if(keys == NULL)
        return;
    
    for(int i = 0; i < count; i++) {
        map->free_key(keys[i].key);
        map->free_value(keys[i].value);
    }

    free(keys);
}