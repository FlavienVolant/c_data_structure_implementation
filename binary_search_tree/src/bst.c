#include "bst.h"
#include <stdlib.h>

BST_t *create_bst(int key, int value){
    BST_t *bst = malloc(sizeof(BST_t));
    init_bst(bst, key, value);
    return bst;
}

void init_bst(BST_t *root, int key, int value){
    root->key = key;
    root->value = value;
    root->left = NULL;
    root->right = NULL;
}

void free_bst(BST_t *root){
    free(root);
}

BST_t *insert(BST_t *root, int key, int value){

    if(root == NULL)
        return create_bst(key, value);

    if(key <= root->key)
        root->left = insert(root->left, key, value);
    else
        root->right = insert(root->right, key, value);

    return root;
}

int find_value(const BST_t *bst, int key, int *value){
    return 0;
}
