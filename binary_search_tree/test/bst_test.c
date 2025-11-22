#include <stdio.h>
#include "bst.h"
#include "c_unit.h"

void *beforeEach() {
    return create_bst(5, 5);
}

void afterEach(void *bst) {
    free_bst(bst);
}

int create_bst_create_a_tree_with_the_root_specified(void *params) {
    BST_t *tree = params;

    ASSERT_EQUALS(tree->key, 5);
    ASSERT_EQUALS(tree->value, 5);
    ASSERT_NULL(tree->left);
    ASSERT_NULL(tree->right);

    return 0;
}

int insert_lower_key_value_than_root_should_create_a_subtree_left(void *params) {
    BST_t *tree = params;

    insert(tree, 4, 4);

    ASSERT_NOT_NULL(tree->left);
    ASSERT_EQUALS(tree->left->key, 4);
    ASSERT_EQUALS(tree->left->value, 4);
    ASSERT_NULL(tree->right);

    return 0;
}

int insert_higher_key_value_than_root_should_create_a_subtree_right(void *params) {
    BST_t *tree = params;

    insert(tree, 6, 6);

    ASSERT_NOT_NULL(tree->right);
    ASSERT_EQUALS(tree->right->key, 6);
    ASSERT_EQUALS(tree->right->value, 6);

    return 0;
}

int find_value_in_a_just_create_bst_should_find_the_root_specified(void *params) {
    BST_t *tree = params;
    int res;
    ASSERT_TRUE(find_value(tree, 5, &res) == 0);
    ASSERT_EQUALS(res, 5);

    return 0;
}

int find_value_after_insert(void *params) {
    BST_t *tree = params;
    int res;
    
    int keys[] = {1, 2, 3, 4, 6, 7, 8, 9, 10};
    int values[] = {1, 4, 8, 16, 32, 64, 128, 256, 512};

    for(int i = 0; i < sizeof(keys)/sizeof(int); i ++) {
        insert(tree, keys[i], values[i]);
    } 
    
    for(int i = 0; i < sizeof(keys)/sizeof(int); i ++) {
        ASSERT_TRUE(find_value(tree, keys[i], &res) == 0);
        ASSERT_EQUALS(res, values[i]);
    }

    return 0;
}

int main() {
    struct TestList *tests = create_test_list();

    ADD_TEST(tests, create_bst_create_a_tree_with_the_root_specified);
    ADD_TEST(tests, insert_lower_key_value_than_root_should_create_a_subtree_left);
    ADD_TEST(tests, insert_higher_key_value_than_root_should_create_a_subtree_right);
    ADD_TEST(tests, find_value_in_a_just_create_bst_should_find_the_root_specified);
    ADD_TEST(tests, find_value_after_insert);

    run_tests(tests, beforeEach, afterEach);
    free_test_list(tests);

    return 0;
}