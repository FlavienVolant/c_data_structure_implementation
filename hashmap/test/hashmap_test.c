#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
#include "hashmap.h"
#include "hashmap_types.h"
#include "c_unit.h"

void* beforeEach() {
    return init_hashmap(hash_int, INT_PARAMS, INT_PARAMS);
}

void afterEach(void* map) {
    free_hashmap(map);
}

int put_a_key_value_then_get_the_value_by_giving_the_key(void *params) {
    Hashmap_t *map = params;
    int key = 5;
    int value = 23;
    int *res;

    ASSERT_EQUALS(put(map, &key, &value), SUCCESS);
    ASSERT_EQUALS(get(map, &key, (void**)&res), SUCCESS);
    ASSERT_EQUALS(*res, value);

    return 0;
}

int put_a_key_below_zero_will_work(void *params) {
    Hashmap_t *map = params;
    int key = -5;
    int value = 23;
    int *res;

    ASSERT_EQUALS(put(map, &key, &value), SUCCESS);
    ASSERT_EQUALS(get(map, &key, (void**)&res), SUCCESS);
    ASSERT_EQUALS(*res, value);

    return 0;
}

int get_a_key_unknown_do_nothing(void *params) {
    Hashmap_t *map = params;
    int key = 0;
    int value = -1;
    int *res = &value;

    ASSERT_EQUALS(get(map, &key, (void**)&res), KEY_UNKNOW);
    ASSERT_EQUALS(*res, -1);

    return 0;
}

int put_a_key_then_del_give_the_value(void *params) {
    Hashmap_t *map = params;
    int key = 0;
    int value = 0;
    int *res;

    ASSERT_EQUALS(put(map, &key, &value), SUCCESS);
    ASSERT_EQUALS(del(map, &key, (void**)&res), SUCCESS);
    ASSERT_EQUALS(*res, value);
    ASSERT_EQUALS(get(map, &key, NULL), KEY_UNKNOW);

    map->value_ops.free_function(res);

    return 0;
}

int put_same_key_replace_the_value(void *params) {
    Hashmap_t *map = params;
    int key = 42;
    int old_value = 100;
    int new_value = 200;
    int *res;

    ASSERT_EQUALS(put(map, &key, &old_value), SUCCESS);
    ASSERT_EQUALS(put(map, &key, &new_value), SUCCESS);
    ASSERT_EQUALS(get(map, &key, (void**)&res), SUCCESS);
    ASSERT_EQUALS(*res, new_value);

    return 0;
}

int del_on_unknown_key_do_nothing(void *params) {
    Hashmap_t *map = params;
    int key = 999;
    int value = 123;
    int *res = &value;

    ASSERT_EQUALS(del(map, &key, (void**)&res), KEY_UNKNOW);
    ASSERT_EQUALS(*res, 123);
    return 0;
}

int insert_multiple_keys_and_retrieve_all(void *params) {
    Hashmap_t *map = params;
    int *res;

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQUALS(put(map, &i, &i), SUCCESS);
    }

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQUALS(get(map, &i, (void**)&res), SUCCESS);
        ASSERT_EQUALS(*res, i);
    }

    return 0;
}

int insert_and_delete_multiple_keys(void *params) {
    Hashmap_t *map = params;
    int *res;

    for (int i = 0; i < 5; ++i) {
        ASSERT_EQUALS(put(map, &i, &i), SUCCESS);
    }

    for (int i = 0; i < 3; ++i) {
        ASSERT_EQUALS(del(map, &i, (void**)&res), SUCCESS);
        ASSERT_EQUALS(*res, i);
        map->value_ops.free_function(res);
        ASSERT_EQUALS(get(map, &i, NULL), KEY_UNKNOW);
    }

    for (int i = 3; i < 5; ++i) {
        ASSERT_EQUALS(get(map, &i, (void**)&res), SUCCESS);
        ASSERT_EQUALS(*res, i);
    }

    return 0;
}

int stress_test_many_insertions(void *params) {
    Hashmap_t *map = params;
    int *res;
    const int N = 1000;

    for (int i = 0; i < N; ++i) {
        ASSERT_EQUALS(put(map, &i, &i), SUCCESS);
    }

    for (int i = 0; i < N; ++i) {
        ASSERT_EQUALS(get(map, &i, (void**)&res), SUCCESS);
        ASSERT_EQUALS(*res, i);
    }

    return 0;
}

int get_keys_return_all_keys(void *params) {
    Hashmap_t *map = params;
    int keys_to_insert[] = {5, -3, 42, 0, 9999};
    int num_keys = sizeof(keys_to_insert)/sizeof(keys_to_insert[0]);
    for (int i = 0; i < num_keys; ++i) {
        ASSERT_EQUALS(put(map, &keys_to_insert[i], &i), SUCCESS);
    }

    int count = 0;
    struct Node *keys = get_keys_as_array(map, &count);

    ASSERT_EQUALS(count, num_keys);

    int found_flags[num_keys];
    memset(found_flags, 0, sizeof(found_flags));

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < num_keys; ++j) {
            if (*(int*)keys[i].key == keys_to_insert[j]) {
                found_flags[j] = 1;
                break;
            }
        }
    }

    for (int i = 0; i < num_keys; ++i) {
        ASSERT_TRUE(found_flags[i] == 1);
    }

    free_keys(map, keys, count);
    return 0;
}

int get_keys_empty_map(void *params) {
    Hashmap_t *map = params;
    int count = 0;
    struct Node *keys = get_keys_as_array(map, &count);

    ASSERT_EQUALS(count, 0);
    ASSERT_TRUE(keys == NULL);

    return 0;
}

int put_string_key_put_get_delete_value(void *params) {
    Hashmap_t *map = params;

    map->key_ops = CSTRING_PARAMS; // work because the map is empty !!

    char *keys[] = {"one", "two", "three"};
    int values[] = {10, 20, 30};

    for (int i = 0; i < 3; i++) {
        ASSERT_EQUALS(put(map, keys[i], &values[i]), SUCCESS);
    }

    for (int i = 0; i < 3; i++) {
        int *res;
        ASSERT_EQUALS(get(map, keys[i], (void**)&res), SUCCESS);
        ASSERT_EQUALS(*res, values[i]);
    }

    int *del_res;
    ASSERT_EQUALS(del(map, "two", (void**)&del_res), SUCCESS);
    ASSERT_EQUALS(*del_res, 20);
    map->value_ops.free_function(del_res);
    ASSERT_EQUALS(get(map, "two", NULL), KEY_UNKNOW);

    return 0;
}

struct Point {
    int x, y;
};

unsigned int hash_point(void *p) {
    struct Point *pt = p;
    return (unsigned int)(pt->x * 31 + pt->y);
}

void* cpy_point(void *p) {
    struct Point *src = p;
    struct Point *cpy = malloc(sizeof(struct Point));
    if (!cpy) return NULL;
    cpy->x = src->x;
    cpy->y = src->y;
    return cpy;
}

int cmp_point(void *a, void *b) {
    struct Point *pa = a;
    struct Point *pb = b;
    return (pa->x == pb->x && pa->y == pb->y) ? 0 : 1;
}

struct Rectangle {
    int width, height;
};

void* cpy_rectangle(void *p) {
    struct Rectangle *src = p;
    struct Rectangle *cpy = malloc(sizeof(struct Rectangle));
    if (!cpy) return NULL;
    cpy->width = src->width;
    cpy->height = src->height;
    return cpy;
}

int cmp_rectangle(void *a, void *b) {
    struct Rectangle *ra = a;
    struct Rectangle *rb = b;
    return (ra->width == rb->width && ra->height == rb->height) ? 0 : 1;
}

int put_struct_key_and_value(void *params) {
    Hashmap_t *map = params;

    const HashmapParams_t POINT_PARAMS = {
    .cpy_function = cpy_point,
    .cmp_function = cmp_point,
    .free_function = free
    };

    const HashmapParams_t RECT_PARAMS = {
        .cpy_function = cpy_rectangle,
        .cmp_function = cmp_rectangle,
        .free_function = free
    };

    map->hash = hash_point;
    map->key_ops = POINT_PARAMS;
    map->value_ops = RECT_PARAMS;

    struct Point keys[] = { {0,0}, {1,2}, {3,4} };
    struct Rectangle values[] = { {10,20}, {30,40}, {50,60} };

    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++) {
        ASSERT_EQUALS(put(map, &keys[i], &values[i]), SUCCESS);
    }

    for (int i = 0; i < n; i++) {
        struct Rectangle *res;
        ASSERT_EQUALS(get(map, &keys[i], (void**)&res), SUCCESS);
        ASSERT_EQUALS(res->width, values[i].width);
        ASSERT_EQUALS(res->height, values[i].height);
    }

    struct Rectangle *del_res;
    ASSERT_EQUALS(del(map, &keys[1], (void**)&del_res), SUCCESS);
    ASSERT_EQUALS(del_res->width, 30);
    ASSERT_EQUALS(del_res->height, 40);
    map->value_ops.free_function(del_res);
    ASSERT_EQUALS(get(map, &keys[1], NULL), KEY_UNKNOW);

    return 0;
}

int main() {
    struct TestList *tests = create_test_list();

    ADD_TEST(tests, put_a_key_value_then_get_the_value_by_giving_the_key);
    ADD_TEST(tests, put_a_key_below_zero_will_work);
    ADD_TEST(tests, get_a_key_unknown_do_nothing);
    ADD_TEST(tests, put_a_key_then_del_give_the_value);
    ADD_TEST(tests, put_same_key_replace_the_value);
    ADD_TEST(tests, del_on_unknown_key_do_nothing);
    ADD_TEST(tests, insert_multiple_keys_and_retrieve_all);
    ADD_TEST(tests, insert_and_delete_multiple_keys);
    ADD_TEST(tests, get_keys_return_all_keys);
    ADD_TEST(tests, get_keys_empty_map);
    ADD_TEST(tests, stress_test_many_insertions);
    ADD_TEST(tests, put_string_key_put_get_delete_value);
    ADD_TEST(tests, put_struct_key_and_value);

    run_tests(tests, beforeEach, afterEach);
    free_test_list(tests);

    return 0;
}
