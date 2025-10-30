#include <stdio.h>
#include <stdlib.h>         
#include <assert.h>

#include "hashmap.h"
#include "c_unit.h"

void* beforeEach() {
    return init_hashmap(sizeof(int), sizeof(int));
}

void afterEach(void* map) {
    free(map);
}

int put_a_key_value_then_get_the_value_by_giving_the_key(void *params) {

    struct Hashmap *map = params;

    int key = 5;
    int value = 23;
    int res;

    ASSERT_TRUE(put(map, key, value) == SUCCESS);
    ASSERT_TRUE(get(map, key, &res) == SUCCESS);
    ASSERT_TRUE(res == value);

    return 0;
}

int put_a_key_bellow_zero_will_work(void *params) {

    struct Hashmap *map = params;

    int key = - 5;
    int value = 23;
    int res;

    ASSERT_TRUE(put(map, key, value) == SUCCESS);
    ASSERT_TRUE(get(map, key, &res) == SUCCESS);
    ASSERT_TRUE(res == value);

    return 0;
}

int get_a_key_unknow_do_nothing(void *params) {

    struct Hashmap *map = params;

    int key = 0;
    int res = -1;

    ASSERT_TRUE(get(map, key, &res) == KEY_UNKNOW);
    ASSERT_TRUE(res == -1);
    
    return 0;
}

int put_a_key_then_del_give_the_value(void *params) {

    struct Hashmap *map = params;

    int key = 0;
    int value = 0;
    int res = -1;

    put(map, key, value);

    ASSERT_TRUE(del(map, key, &res) == SUCCESS);
    ASSERT_TRUE(res == value);
    ASSERT_TRUE(get(map, key, NULL) == KEY_UNKNOW);

    return 0;
}

int put_same_key_replace_the_value(void *params) {
    struct Hashmap *map = params;

    int key = 42;
    int old_value = 100;
    int new_value = 200;
    int res;

    ASSERT_EQUALS(put(map, key, old_value), SUCCESS);
    ASSERT_EQUALS(put(map, key, new_value), SUCCESS);
    ASSERT_EQUALS(get(map, key, &res), SUCCESS);
    ASSERT_EQUALS(res, new_value);

    return 0;
}

int del_on_unknown_key_do_nothing(void *params) {
    struct Hashmap *map = params;

    int key = 999;
    int res = 123;

    ASSERT_EQUALS(del(map, key, &res), KEY_UNKNOW);
    ASSERT_EQUALS(res, 123);

    return 0;
}

int insert_multiple_keys_and_retrieve_all(void *params) {
    struct Hashmap *map = params;

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQUALS(put(map, i, i * 10), SUCCESS);
    }

    int res;
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQUALS(get(map, i, &res), SUCCESS);
        ASSERT_EQUALS(res, i * 10);
    }

    return 0;
}

int insert_and_delete_multiple_keys(void *params) {
    struct Hashmap *map = params;

    for (int i = 0; i < 5; ++i) {
        ASSERT_EQUALS(put(map, i, i + 100), SUCCESS);
    }

    int res;
    for (int i = 0; i < 3; ++i) {
        ASSERT_EQUALS(del(map, i, &res), SUCCESS);
        ASSERT_EQUALS(res, i + 100);
        ASSERT_EQUALS(get(map, i, NULL), KEY_UNKNOW);
    }

    for (int i = 3; i < 5; ++i) {
        ASSERT_EQUALS(get(map, i, &res), SUCCESS);
        ASSERT_EQUALS(res, i + 100);
    }

    return 0;
}

int stress_test_many_insertions(void *params) {
    struct Hashmap *map = params;

    const int N = 1000;
    for (int i = 0; i < N; ++i) {
        ASSERT_EQUALS(put(map, i, i * 2), SUCCESS);
    }

    int res;
    for (int i = 0; i < N; ++i) {
        ASSERT_EQUALS(get(map, i, &res), SUCCESS);
        ASSERT_EQUALS(res, i * 2);
    }

    return 0;
}

int get_keys_return_all_keys(void *params) {
    struct Hashmap *map = params;

    int keys_to_insert[] = {5, -3, 42, 0, 9999};
    int num_keys = sizeof(keys_to_insert) / sizeof(keys_to_insert[0]);
    for (int i = 0; i < num_keys; ++i) {
        ASSERT_EQUALS(put(map, keys_to_insert[i], i * 10), SUCCESS);
    }

    int count = 0;
    struct Node *keys = get_keys_as_array(map, &count);

    ASSERT_EQUALS(count, num_keys);

    int found_flags[num_keys];
    for (int i = 0; i < num_keys; ++i) found_flags[i] = 0;

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < num_keys; ++j) {
            if (keys[i].key == keys_to_insert[j]) {
                found_flags[j] = 1;
                break;
            }
        }
    }

    for (int i = 0; i < num_keys; ++i) {
        ASSERT_TRUE(found_flags[i] == 1);
    }

    free(keys);

    return 0;
}

int get_keys_empty_map(void *params) {
    struct Hashmap *map = params;
    int count;
    struct Node *keys = get_keys_as_array(map, &count);

    ASSERT_EQUALS(count, 0);
    ASSERT_TRUE(keys == NULL);

    return 0;
}


int main() {

    struct TestList *tests = create_test_list();

    ADD_TEST(tests, put_a_key_value_then_get_the_value_by_giving_the_key);
    ADD_TEST(tests, put_a_key_bellow_zero_will_work);
    ADD_TEST(tests, get_a_key_unknow_do_nothing);
    ADD_TEST(tests, put_a_key_then_del_give_the_value);

    ADD_TEST(tests, put_same_key_replace_the_value);
    ADD_TEST(tests, del_on_unknown_key_do_nothing);
    ADD_TEST(tests, insert_multiple_keys_and_retrieve_all);
    ADD_TEST(tests, insert_and_delete_multiple_keys);
    ADD_TEST(tests, stress_test_many_insertions);

    ADD_TEST(tests, get_keys_return_all_keys);
    ADD_TEST(tests, get_keys_empty_map);

    run_tests(tests, beforeEach, afterEach);

    free_test_list(tests);

    return 0;
}