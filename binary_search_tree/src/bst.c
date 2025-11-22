#include "bst.h"
#include <stdlib.h>

BST_t *create_bst(Node_t root){
    BST_t *bst = malloc(sizeof(BST_t));
    init_bst(bst, root);
    return bst;
}

void init_bst(BST_t *bst, Node_t root){
    
}

void free_bst(BST_t *bst){
}

void insert(BST_t *bst, Node_t node){
}

int find_value(const BST_t *bst, int key, int *value){
    return 0;
}
