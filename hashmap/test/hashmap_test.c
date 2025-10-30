#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
#include "hashmap.h"
#include "c_unit.h"

void* beforeEach() {
    return init_hashmap(sizeof(int), sizeof(int));
}

void afterEach(void* map) {
    free_hashmap(map);
}

int put_a_key_value_then_get_the_value_by_giving_the_key(void *params) {
    struct Hashmap *map = params;
    int key = 5;
    int value = 23;
    int res;

    ASSERT_EQUALS(put(map, &key, &value), SUCCESS);
    ASSERT_EQUALS(get(map, &key, &res), SUCCESS);
    ASSERT_EQUALS(res, value);

    return 0;
}

int put_a_key_below_zero_will_work(void *params) {
    struct Hashmap *map = params;
    int key = -5;
    int value = 23;
    int res;

    ASSERT_EQUALS(put(map, &key, &value), SUCCESS);
    ASSERT_EQUALS(get(map, &key, &res), SUCCESS);
    ASSERT_EQUALS(res, value);

    return 0;
}

int get_a_key_unknown_do_nothing(void *params) {
    struct Hashmap *map = params;
    int key = 0;
    int res = -1;

    ASSERT_EQUALS(get(map, &key, &res), KEY_UNKNOW);
    ASSERT_EQUALS(res, -1);

    return 0;
}

int put_a_key_then_del_give_the_value(void *params) {
    struct Hashmap *map = params;
    int key = 0;
    int value = 0;
    int res = -1;

    ASSERT_EQUALS(put(map, &key, &value), SUCCESS);
    ASSERT_EQUALS(del(map, &key, &res), SUCCESS);
    ASSERT_EQUALS(res, value);
    ASSERT_EQUALS(get(map, &key, NULL), KEY_UNKNOW);

    return 0;
}

int put_same_key_replace_the_value(void *params) {
    struct Hashmap *map = params;
    int key = 42;
    int old_value = 100;
    int new_value = 200;
    int res;

    ASSERT_EQUALS(put(map, &key, &old_value), SUCCESS);
    ASSERT_EQUALS(put(map, &key, &new_value), SUCCESS);
    ASSERT_EQUALS(get(map, &key, &res), SUCCESS);
    ASSERT_EQUALS(res, new_value);

    return 0;
}

int del_on_unknown_key_do_nothing(void *params) {
    struct Hashmap *map = params;
    int key = 999;
    int res = 123;

    ASSERT_EQUALS(del(map, &key, &res), KEY_UNKNOW);
    ASSERT_EQUALS(res, 123);

    return 0;
}

int insert_multiple_keys_and_retrieve_all(void *params) {
    struct Hashmap *map = params;
    int res;

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQUALS(put(map, &i, &i), SUCCESS);
    }

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQUALS(get(map, &i, &res), SUCCESS);
        ASSERT_EQUALS(res, i);
    }

    return 0;
}

int insert_and_delete_multiple_keys(void *params) {
    struct Hashmap *map = params;
    int res;

    for (int i = 0; i < 5; ++i) {
        ASSERT_EQUALS(put(map, &i, &i), SUCCESS);
    }

    for (int i = 0; i < 3; ++i) {
        ASSERT_EQUALS(del(map, &i, &res), SUCCESS);
        ASSERT_EQUALS(res, i);
        ASSERT_EQUALS(get(map, &i, NULL), KEY_UNKNOW);
    }

    for (int i = 3; i < 5; ++i) {
        ASSERT_EQUALS(get(map, &i, &res), SUCCESS);
        ASSERT_EQUALS(res, i);
    }

    return 0;
}

int stress_test_many_insertions(void *params) {
    struct Hashmap *map = params;
    int res;
    const int N = 1000;

    for (int i = 0; i < N; ++i) {
        ASSERT_EQUALS(put(map, &i, &i), SUCCESS);
    }

    for (int i = 0; i < N; ++i) {
        ASSERT_EQUALS(get(map, &i, &res), SUCCESS);
        ASSERT_EQUALS(res, i);
    }

    return 0;
}

int get_keys_return_all_keys(void *params) {
    struct Hashmap *map = params;
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

    free_keys(keys, count);
    return 0;
}

int get_keys_empty_map(void *params) {
    struct Hashmap *map = params;
    int count = 0;
    struct Node *keys = get_keys_as_array(map, &count);

    ASSERT_EQUALS(count, 0);
    ASSERT_TRUE(keys == NULL);

    return 0;
}

int put_string_key_get_value(void *params) {
    struct Hashmap *map = params;

    map->sizeOfKey = sizeof(char*); // work because the map is empy !!

    char *keys[] = {"one", "two", "three"};
    int values[] = {10, 20, 30};

    for (int i = 0; i < 3; i++) {
        ASSERT_EQUALS(put(map, keys[i], &values[i]), SUCCESS);
    }

    for (int i = 0; i < 3; i++) {
        int res = 0;
        ASSERT_EQUALS(get(map, keys[i], &res), SUCCESS);
        ASSERT_EQUALS(res, values[i]);
    }

    int del_res = 0;
    ASSERT_EQUALS(del(map, "two", &del_res), SUCCESS);
    ASSERT_EQUALS(del_res, 20);
    ASSERT_EQUALS(get(map, "two", NULL), KEY_UNKNOW);

    return 0;
}

struct Point {
    int x, y;
};

struct Rectangle {
    int width, height;
};

int put_struct_key_and_value(void *params) {
    struct Hashmap *map = params;

    map->sizeOfKey = sizeof(struct Point);
    map->sizeOfValue = sizeof(struct Rectangle);

    struct Point keys[] = { {0,0}, {1,2}, {3,4} };
    struct Rectangle values[] = { {10,20}, {30,40}, {50,60} };

    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++) {
        ASSERT_EQUALS(put(map, &keys[i], &values[i]), SUCCESS);
    }

    for (int i = 0; i < n; i++) {
        struct Rectangle res;
        ASSERT_EQUALS(get(map, &keys[i], &res), SUCCESS);
        ASSERT_EQUALS(res.width, values[i].width);
        ASSERT_EQUALS(res.height, values[i].height);
    }

    struct Rectangle del_res;
    ASSERT_EQUALS(del(map, &keys[1], &del_res), SUCCESS);
    ASSERT_EQUALS(del_res.width, 30);
    ASSERT_EQUALS(del_res.height, 40);
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
    ADD_TEST(tests, stress_test_many_insertions);
    ADD_TEST(tests, get_keys_return_all_keys);
    ADD_TEST(tests, get_keys_empty_map);
    ADD_TEST(tests, put_string_key_get_value);
    ADD_TEST(tests, put_struct_key_and_value);

    run_tests(tests, beforeEach, afterEach);
    free_test_list(tests);

    return 0;
}
