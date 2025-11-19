#include <stdio.h>
#include <string.h>
#include "hashmap.h"

struct Person {
    char name[32];
    int age;
};

unsigned int hash_person(const void* p){
    struct Person *person = p;

    unsigned int hash = 5381;

    for(int i = 0; person->name[i]; i++) {
        hash = ((hash << 5) + hash) + person->name[i];
    }

    return hash;
}

void* cpy_person(const void *p) {
    struct Person *src = p;
    struct Person *cpy = malloc(sizeof(struct Person));

    strncpy(cpy->name, src->name, 32);
    cpy->age = src->age;

    return cpy;
}

int cmp_person(const void *a, const void *b) {
    struct Person *p_a = a;
    struct Person *p_b = b;

    return strncmp(p_a->name, p_b->name, 32) && p_a->age == p_b->age;
}

void free_person(const void *p) {
    free(p);
}

int main() {

    printf("Hashmap example: int -> struct Person\n");

    struct Hashmap *map = init_hashmap(hash_person, );

    int ids[] = {1, 2, 3};
    struct Person people[] = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 40}
    };

    for (int i = 0; i < 3; i++) {
        put(map, &ids[i], &people[i]);
    }

    struct Person p;
    if (get(map, &ids[1], &p) == SUCCESS) {
        printf("Retrieved id=%d -> name=%s, age=%d\n", ids[1], p.name, p.age);
    }

    free_hashmap(map);

    return 0;
}