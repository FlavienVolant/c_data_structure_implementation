#ifndef HASHMAP_TYPES_H
#define HASHMAP_TYPES_H

#include "hashmap.h"

unsigned int hash_int(void *p);
unsigned int hash_cstring(void *p);

extern const HashmapParams_t INT_PARAMS;
extern const HashmapParams_t FLOAT_PARAMS;
extern const HashmapParams_t CSTRING_PARAMS;

#endif