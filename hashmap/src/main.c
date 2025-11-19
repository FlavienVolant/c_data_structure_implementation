#include <stdio.h>
#include "hashmap.h"

struct Person {
    char name[32];
    int age;
};

int main() {

    printf("Hashmap example: int -> struct Person\n");

    struct Hashmap *map = init_hashmap(sizeof(int), sizeof(struct Person));

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