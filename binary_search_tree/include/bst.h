#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

typedef struct {
    int key;
    int value;
} Node_t;

typedef struct BST {
    Node_t node;
    struct BST *left;
    struct BST *right;
} BST_t;

BST_t *create_bst(Node_t root);
void init_bst(BST_t *bst, Node_t root);
void free_bst(BST_t *bst);

void insert(BST_t *bst, Node_t node);
int find_value(const BST_t *bst, int key, int* value);

#endif