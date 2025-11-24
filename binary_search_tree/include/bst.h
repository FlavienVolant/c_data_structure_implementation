#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

typedef struct __BST {
    int key;
    int value;
    struct __BST *left;
    struct __BST *right;
} BST_t;

typedef enum {
    SUCCESS = 0,
    KEY_UNKNOWN = 1
} BST_RETURN_VALUE_e;

BST_t *create_bst(int key, int value);
void init_bst(BST_t *root, int key, int value);
void free_bst(BST_t *root);

BST_t *insert(BST_t *root, int key, int value);
BST_RETURN_VALUE_e get(const BST_t *root, int key, int* value);

#endif