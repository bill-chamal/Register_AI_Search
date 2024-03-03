#pragma once
#include <stdbool.h>

typedef int Data;

typedef struct  Node {
    Data data;
    int height;
    struct Node* lchild;
    struct Node* rchild;
} AVL_node;

int getHeight(AVL_node* n);

int getMaxHeight(AVL_node* n);

void initialize_AVL(AVL_node** n);

bool AVL_isEmpty(AVL_node* n);

AVL_node* create_avl_node(int key);

AVL_node* rightRotate(AVL_node* n);

AVL_node* leftRotate(AVL_node* n);

int getBalance(AVL_node* n);

AVL_node* insert_avl_node(AVL_node* n, int keyValue);

AVL_node* get_min_value(AVL_node* node);

bool dataExists_AVL(AVL_node* root, int keyValue);