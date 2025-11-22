#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

typedef struct BST {
    int key;
    int value;
    struct BST *left;
    struct BST *right;
} BST_t;

BST_t *create_bst(int key, int value);
void init_bst(BST_t *bst, int key, int value);
void free_bst(BST_t *bst);

void insert(BST_t *bst, int key, int value);
int find_value(const BST_t *bst, int key, int* value);

#endif