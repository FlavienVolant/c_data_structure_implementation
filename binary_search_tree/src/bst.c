#include "bst.h"
#include <stdlib.h>

BST_t *create_bst(int key, int value){
    BST_t *bst = malloc(sizeof(BST_t));
    init_bst(bst, key, value);
    return bst;
}

void init_bst(BST_t *bst, int key, int value){
    bst->key = key;
    bst->value = value;
    bst->left = NULL;
    bst->right = NULL;
}

void free_bst(BST_t *bst){
    free(bst);
}

void insert(BST_t *bst, int key, int value){
    BST_t *current = bst;
    BST_t *insert = create_bst(key, value);
    while(1) {
        if(key <= current->key){
            if(current->left == NULL){
                current->left = insert;
                return;
            }
            current = current->left;
        }
    }
}

int find_value(const BST_t *bst, int key, int *value){
    return 0;
}
