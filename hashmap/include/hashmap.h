#ifndef HASHMAP_H
#define HASHMAP_H

#define DEFAULT_CAPACITY 256
#define DEFAULT_LOAD_FACTOR 75

typedef unsigned int (hash_f(const void* key));

typedef void* (cpy_f(const void *a));
typedef int (cmp_f(const void *a, const void *b));
typedef void (free_f(void *a));

typedef struct {
    int capacity;
    int loadFactor;
    int keyCount;
    Node_t **table;
    
    hash_f* hash;
    
    cpy_f *cpy_key;
    cmp_f *cmp_key;
    free_f *free_key;

    cpy_f *cpy_value;
    cmp_f *cmp_value;
    free_f *free_value;
} Hashmap_t;

typedef struct {
    void *key;
    void *value;
    Node_t *next;
} Node_t;

typedef enum {
    SUCCESS = 0,
    KEY_UNKNOW = -1
} HashMapReturnValue;

Hashmap_t* init_hashmap(
    hash_f *hash, 
    cpy_f *cpy_key,
    cmp_f *cmp_key,
    free_f *free_key,
    cpy_f *cpy_value,
    cmp_f *cmp_value,
    free_f *free_value);

void free_hashmap(Hashmap_t *map);

HashMapReturnValue put(Hashmap_t *map, void *key, void *value);
HashMapReturnValue get(Hashmap_t *map, void *key, void **res);
HashMapReturnValue del(Hashmap_t *map, void *key, void **res);

Node_t* get_keys_as_array(const Hashmap_t *map, int *count);
void free_keys(const Hashmap_t *map, Node_t* keys, int count);

#endif