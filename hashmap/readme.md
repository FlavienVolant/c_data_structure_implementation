# My implementation of a Hashmap in C

A hashmap implementation in C supporting arbitrary key and value types.

It automatically handles collisions using linked lists and resizes when the load factor is reached.
Memory for keys and values is managed internally. The hashmap makes copies of inserted keys and values and frees them when the map is freed.

---

## How to

- Run tests
``` bash
make test
```

- Run tests in debug mode with GDB
``` bash
make test_debug
```

- Clean compiled files
``` bash
make clean
```

## API

checkout `test/hashmap_test.c` and `src/hashmap.c` for examples.

### enum HashMapReturnValue

- `SUCCESS` = 0
- `KEY_UNKNOW` = -1

---

### struct Hashmap* init_hashmap(int sizeOfKey, int sizeOfValue)
Create a new generic hashmap.  

- `sizeOfKey` : size of keys in bytes  
- `sizeOfValue` : size of values in bytes  
- Returns a pointer to the new hashmap

---

### enum HashMapReturnValue put(struct Hashmap *map, void *key, void *value)
Insert a key/value pair into the hashmap.  
If the key exists, its value is updated.  

- `map` : pointer to the hashmap  
- `key` : pointer to the key  
- `value` : pointer to the value  
- Returns `SUCCESS` if insertion succeeded

---

### enum HashMapReturnValue get(struct Hashmap *map, void *key, void **res)
Get the value associated with a key.  

- `res` : optional, if not `NULL` will contain the value, the value is owned by the map. Do not free ! Do not update ! Stop using it after a put/del !
- Returns `SUCCESS` if found, `KEY_UNKNOW` otherwise

---

### enum HashMapReturnValue del(struct Hashmap *map, void *key, void **res)
Delete a key/value pair.  

- `res` : optional, if not `NULL` will contain the deleted value. Don't forget to free res !
- Returns `SUCCESS` if deletion succeeded, `KEY_UNKNOW` if the key does not exist

---

### struct Node* get_keys_as_array(struct Hashmap *map, int *count)
Return a array of all keys.  

- `count` : receives the number of keys in the array  
- Returns an array of `Node` containing keys and values (free with `free_keys`)

---

### void free_keys(struct Node* keys, int count)
Free an array of keys/values returned by `get_keys_as_array`.

---

### void free_hashmap(struct Hashmap *map)
Free the entire hashmap and all allocated keys and values.

## Hash Function

The hashmap uses a **DJB2**-inspired hash function for arbitrary key types.  

- The key is treated as a sequence of bytes.  
- Each byte is combined into the hash using the formula:
`hash = hash * 33 + byte`