#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "hashmap_types.h"

struct Person {
    char name[32];
    int age;
};

void* cpy_person(void *p) {
    struct Person *src = p;
    struct Person *cpy = malloc(sizeof(struct Person));

    strncpy(cpy->name, src->name, 32);
    cpy->age = src->age;

    return cpy;
}

int cmp_person(void *a, void *b) {
    struct Person *p_a = a;
    struct Person *p_b = b;

    return (strncmp(p_a->name, p_b->name, 32) == 0 &&
            p_a->age == p_b->age) ? 0 : 1; 
}

int main() {

    printf("Hashmap example: int -> struct Person\n");

    HashmapParams_t value_f = {
        .cpy_function = cpy_person,
        .cmp_function = cmp_person,
        .free_function = free,
    };

    Hashmap_t *map = init_hashmap(hash_int, INT_PARAMS, value_f);

    int ids[] = {1, 2, 3};
    struct Person people[] = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 40}
    };

    for (int i = 0; i < 3; i++) {
        put(map, &ids[i], &people[i]);
    }

    struct Person *p;

    for(int i = 0; i < 3; i ++) {
        if (get(map, &ids[i], (void**)&p) == SUCCESS) {
            printf("Retrieved id=%d -> name=%s, age=%d\n", ids[i], p->name, p->age);
        }else {
            printf("Error can't retrieve id=%d -> name=%s, age=%d\n", ids[i], p->name, p->age);
        }
    }

    free_hashmap(map);

    return 0;
}