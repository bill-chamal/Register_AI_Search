#include <stdio.h>
#include <stdlib.h>

#include "AVL_Tree.h"

int getHeight(AVL_node* n) {
    if (n == NULL)
        return 0;
    return n->height;
}

void initialize_AVL(AVL_node** n)
{
    *n = NULL;
}

bool AVL_isEmpty(AVL_node* n)
{
    return n == NULL;
}

int getMaxHeight(AVL_node* n)
{
    int i = 0;
    if (n != NULL)
        i = max(getHeight(n->lchild), getHeight(n->rchild));
    else
        printf("\nWARNING: Cannot get height from NULL AVLF node");
    return i;
}

AVL_node* create_avl_node(int key) {

    AVL_node* node = (AVL_node*)malloc(sizeof(AVL_node));
    if (node == NULL)
    {
        printf("\nERROR: Cannot allocate more memory!");
        exit(1);
    }

    node->data = key;
    node->lchild = NULL;
    node->rchild = NULL;
    node->height = 1;
    return (node);
}

AVL_node* rightRotate(AVL_node* n) {
    AVL_node* x = n->lchild;
    AVL_node* y = x->rchild;

    x->rchild = n;
    n->lchild = y;

    n->height = getMaxHeight(n) + 1;
    x->height = getMaxHeight(x) + 1;

    return x;
}

AVL_node* leftRotate(AVL_node* n) {
    AVL_node* x = n->rchild;
    AVL_node* y = x->lchild;

    x->lchild = n;
    n->rchild = y;

    n->height = getMaxHeight(n) + 1;
    x->height = getMaxHeight(x) + 1;

    return x;
}

int getBalance(AVL_node* n) {
    return n == NULL ? 0 : getHeight(n->lchild) - getHeight(n->rchild);
}

AVL_node* insert_avl_node(AVL_node* n, int keyValue) {

    if (n == NULL)
        return (create_avl_node(keyValue));

    if (keyValue < n->data)
        n->lchild = insert_avl_node(n->lchild, keyValue);
    else if (keyValue > n->data)
        n->rchild = insert_avl_node(n->rchild, keyValue);
    else
        return n;

    n->height = 1 + getMaxHeight(n);

    // Reconstruct Tree
    int balance = getBalance(n);
    if (balance > 1 && keyValue < n->lchild->data)
    	return rightRotate(n);

    if (balance < -1 && keyValue > n->rchild->data)
    	return leftRotate(n);

    if (balance > 1 && keyValue > n->lchild->data) {
    	n->lchild = leftRotate(n->lchild);
    	return rightRotate(n);
    }

    if (balance < -1 && keyValue < n->rchild->data) {
    	n->rchild = rightRotate(n->rchild);
    	return leftRotate(n);
    }

    return n;
}

AVL_node* get_min_value(AVL_node* node) {

    AVL_node* current = node;
    while (current->lchild != NULL)
        current = current->lchild;

    return current;
}

bool dataExists_AVL(AVL_node* root, int keyValue)
{
    bool found = false;
    while (!found && !AVL_isEmpty(root))
    {
        if (root->data == keyValue)
        {
            found = true;
        }
        else if (keyValue > root->data)
            root = root->rchild;
        else
            root = root->lchild;
    }
    return found;
}