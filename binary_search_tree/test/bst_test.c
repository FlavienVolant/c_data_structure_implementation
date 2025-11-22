#include <stdio.h>
#include "bst.h"
#include "c_unit.h"

void *beforeEach() {
    Node_t root = {
        .key = 5,
        .value = 10,
    };

    return create_bst(root);
}

void afterEach(void *bst) {
    free_bst(bst);
}

int create_bst_create_a_tree_with_the_root_specified(void *params) {
    BST_t *tree = params;
    ASSERT_EQUALS(tree->node.key, 5);
    ASSERT_EQUALS(tree->node.value, 10);

    return 0;
}

int main() {
    struct TestList *tests = create_test_list();

    ADD_TEST(tests, create_bst_create_a_tree_with_the_root_specified);

    run_tests(tests, beforeEach, afterEach);
    free_test_list(tests);

    return 0;
}