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

    return 0;
}

int insert_lower_key_value_than_root_should_create_a_subtree_left(void *params) {
    BST_t *tree = params;

    insert(tree, 4, 4);

    ASSERT_NOT_NULL(tree->left);
    ASSERT_EQUALS(tree->left->key, 4);
    ASSERT_EQUALS(tree->left->value, 4);

    return 0;
}

int main() {
    struct TestList *tests = create_test_list();

    ADD_TEST(tests, create_bst_create_a_tree_with_the_root_specified);
    ADD_TEST(tests, insert_lower_key_value_than_root_should_create_a_subtree_left);

    run_tests(tests, beforeEach, afterEach);
    free_test_list(tests);

    return 0;
}