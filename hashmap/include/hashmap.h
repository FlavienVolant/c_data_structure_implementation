#ifndef HASHMAP_H
#define HASHMAP_H

#define DEFAULT_CAPACITY 256
#define DEFAULT_LOAD_FACTOR 75

struct Hashmap {
    int capacity;
    int loadFactor;
    int keyCount;
    struct Node **table;
    int sizeOfKey;
    int sizeOfValue;
};

struct Node {
    int key;
    int value;
    struct Node *next;
};

enum HashMapReturnValue {
    SUCCESS = 0,
    KEY_UNKNOW = -1
};

struct Hashmap* init_hashmap(int sizeOfKey, int sizeOfValue);
enum HashMapReturnValue put(struct Hashmap *map, int key, int value);
enum HashMapReturnValue get(struct Hashmap *map, int key, int *res);
enum HashMapReturnValue del(struct Hashmap *map, int key, int *res);
struct Node* get_keys_as_array(struct Hashmap *map, int *count);
void free_hashmap(struct Hashmap *map);

#endif