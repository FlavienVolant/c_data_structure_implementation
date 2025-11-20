#include "hashmap.h"

#include <stdlib.h>

Hashmap_t* init_hashmap(hash_key_f *hash, HashmapParams_t key_ops, HashmapParams_t value_ops) {
    Hashmap_t *map = malloc(sizeof(Hashmap_t));

    map->capacity = DEFAULT_CAPACITY;
    map->loadFactor = DEFAULT_LOAD_FACTOR;

    map->hash = hash;

    map->key_ops = key_ops;
    map->value_ops = value_ops;

    map->keyCount = 0;
    map->table = malloc(sizeof(Node_t*) * map->capacity);
    
    for(int i = 0; i < map->capacity; i++) {
        map->table[i] = NULL;
    }

    return map;
}

Node_t *create_node(const Hashmap_t *map, void *key, void *value) {
    Node_t *node = malloc(sizeof(Node_t));

    node->key = map->key_ops.cpy_function(key);
    node->value = map->value_ops.cpy_function(value);
    node->next = NULL;

    return node;
}

void free_node(const Hashmap_t *map, Node_t *head) {
    Node_t *current = head;

    while (current != NULL) {
        Node_t *tmp = current;
        current = current->next;
        map->key_ops.free_function(tmp->key);
        map->value_ops.free_function(tmp->value);
        free(tmp);
    }
}

void free_hashmap(Hashmap_t *map)
{
    for(int i = 0; i < map->capacity; i++) {
        free_node(map, map->table[i]);
    }
    free(map->table);
    free(map);
}

unsigned int hash(const Hashmap_t *map, void *key) {
    return map->hash(key) % map->capacity;
}

void resize(Hashmap_t *map) {

    int count;
    Node_t *keys = get_keys_as_array(map, &count);

    for(int i = 0; i < map->capacity; i++) {
        free_node(map, map->table[i]);
    }

    map->capacity *= 2;
    Node_t **tmp = realloc(map->table, sizeof(Node_t *) * map->capacity);
    if(!tmp)
        return;
    map->table = tmp;

    for(int i = 0; i < map->capacity; i++) {
        map->table[i] = NULL;
    }

    map->keyCount = 0;

    for(int i = 0; i < count; i++) {
        put(map, keys[i].key, keys[i].value);
    }

    free_keys(map, keys, count);
}

HashMapReturnValue_e put(Hashmap_t *map, void *key, void *value) {
    if (map->keyCount >= map->loadFactor * map->capacity)
        resize(map);

    unsigned int hash_key = hash(map, key);
    Node_t **current = &map->table[hash_key];
    
    while (*current != NULL) {
        if (map->key_ops.cmp_function((*current)->key, key) == 0) {
            map->value_ops.free_function((*current)->value);
            (*current)->value = map->value_ops.cpy_function(value);

            return SUCCESS;
        }

        current = &(*current)->next;
    }

    *current = create_node(map, key, value);
    map->keyCount++;
    
    return SUCCESS;
}

HashMapReturnValue_e get(Hashmap_t *map, void *key, void **res) {
    unsigned int hash_key = hash(map, key);
    Node_t *current = map->table[hash_key];

    while(current != NULL) {
        if(map->key_ops.cmp_function(current->key, key) == 0) {
            if(res != NULL)
                *res = current->value;
            
            return SUCCESS;
        }

        current = current->next;
    }

    return KEY_UNKNOW;
}

HashMapReturnValue_e del(Hashmap_t *map, void *key, void **res) {
    unsigned int hash_key = hash(map, key);
    
    Node_t *before = NULL;
    Node_t *current = map->table[hash_key];

    while(current != NULL) {
        if(map->key_ops.cmp_function(current->key, key) == 0) {
            if(res != NULL)
                *res = map->value_ops.cpy_function(current->value);

            if(before == NULL)
                map->table[hash_key] = current->next;
            else
                before->next = current->next;
            
            map->keyCount--;

            map->key_ops.free_function(current->key);
            map->value_ops.free_function(current->value);
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
            res[key_count].key = map->key_ops.cpy_function(current->key);
            res[key_count].value = map->value_ops.cpy_function(current->value);
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
        map->key_ops.free_function(keys[i].key);
        map->value_ops.free_function(keys[i].value);
    }

    free(keys);
}