#include "hashmap_types.h"

#include <stdlib.h>
#include <string.h>

// ====== INT =======

void* cpy_int(void *p) {
    int *cpy = malloc(sizeof(int));
    *cpy = *(int*)p;
    return cpy;
}

int cmp_int(void *a, void *b) {
    return (*(int*)a == *(int*)b) ? 0 : 1;
}

unsigned int hash_int(void *p) {
    return *(unsigned int*)p;
}

const HashmapParams_t INT_PARAMS = {
    cpy_int,
    cmp_int,
    free
};

// ====== FLOAT =======

void* cpy_float(void *p) {
    float *cpy = malloc(sizeof(float));
    *cpy = *(float*)p;
    return cpy;
}

int cmp_float(void *a, void *b) {
    return (*(float*)a == *(float*)b) ? 0 : 1;
}

const HashmapParams_t FLOAT_PARAMS = {
    cpy_float,
    cmp_float,
    free
};

// ====== DOUBLE =======

void* cpy_double(void *p) {
    double *cpy = malloc(sizeof(double));
    *cpy = *(double*)p;
    return cpy;
}

int cmp_double(void *a, void *b) {
    return (*(double*)a == *(double*)b) ? 0 : 1;
}

const HashmapParams_t DOUBLE_PARAMS = {
    cpy_double,
    cmp_double,
    free
};

// ====== CSTRING =======

void* cpy_cstring(void *p) {
    char *src = p;
    size_t len = strlen(src) + 1;
    char *cpy = malloc(len);
    memcpy(cpy, src, len);
    return cpy;
}

int cmp_cstring(void *a, void *b) {
    return strcmp((char*)a, (char*)b);
}

unsigned int hash_cstring(void *p) {
    char *s = p;
    unsigned int h = 5381;
    while (*s)
        h = (h << 5) + h + (unsigned char)*s++;
    return h;
}

const HashmapParams_t CSTRING_PARAMS = {
    cpy_cstring,
    cmp_cstring,
    free
};