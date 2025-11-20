#ifndef HASHMAP_H
#define HASHMAP_H

#define DEFAULT_CAPACITY 256
#define DEFAULT_LOAD_FACTOR 0.75f

typedef unsigned int (hash_key_f(void* key));

typedef void* (cpy_f(void *a));
typedef int (cmp_f(void *a, void *b));
typedef void (free_f(void *a));

typedef struct Node {
    void *key;
    void *value;
    struct Node *next;
} Node_t;

typedef struct {
    int capacity;
    int loadFactor;
    int keyCount;
    Node_t **table;
    
    hash_key_f* hash;
    
    cpy_f *cpy_key;
    cmp_f *cmp_key;
    free_f *free_key;

    cpy_f *cpy_value;
    cmp_f *cmp_value;
    free_f *free_value;
} Hashmap_t;

typedef enum {
    SUCCESS = 0,
    KEY_UNKNOW = -1
} HashMapReturnValue_e;

typedef struct {
    cpy_f *cpy_function;
    cmp_f *cmp_function;
    free_f *free_function;
} HashmapParams_t;

Hashmap_t* init_hashmap(hash_key_f *hash, HashmapParams_t key_functions, HashmapParams_t value_functions);
void free_hashmap(Hashmap_t *map);

void put(Hashmap_t *map, void *key, void *value);
HashMapReturnValue_e get(Hashmap_t *map, void *key, void **res);
HashMapReturnValue_e del(Hashmap_t *map, void *key, void **res);

Node_t* get_keys_as_array(const Hashmap_t *map, int *count);
void free_keys(const Hashmap_t *map, Node_t* keys, int count);

#endif