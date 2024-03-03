#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Forehead_AVL.h"

#include "AVL_Tree.h"

int getHeightF(AVLF_node* n) {
    return n == 0 ? 0 : n->height;
}

void initialize_AVLF(AVLF_node** n, int i, int f)
{
    *n = NULL;
    /*
     *  i=0 heuristic for best
     *  i=1 f=g+h for A-Star
     */
    option = i;
    // Final state
    fin_State = f;
}

bool AVLF_isEmpty(AVLF_node* n)
{
    return n == NULL;
}

AVLF_node* create_AVLF_node(SearchTreeNode* n) {

    AVLF_node* node = (AVLF_node*)malloc(sizeof(AVLF_node));
    if (node == NULL)
    {
        printf("\nERROR: Cannot allocate more memory!");
        exit(1);
    }

    node->clone = NULL;
    node->node = n;
    node->lchild = NULL;
    node->rchild = NULL;
    node->height = 1;
    return node;
}

int getMaxHeightF(AVLF_node* n)
{
    int i = 0;
    if (n != NULL)
        i = max(getHeightF(n->lchild), getHeightF(n->rchild));
    else
        printf("\nWARNING: Cannot get height from NULL AVLF node");
    return i;
}

AVLF_node* rightRotateF(AVLF_node* n) { 
    AVLF_node* x = n->lchild;
    AVLF_node* y = x->rchild;

    x->rchild = n;
    n->lchild = y;

    n->height = getMaxHeightF(n) + 1;
    x->height = getMaxHeightF(x) + 1;

    return x;
}

AVLF_node* leftRotateF(AVLF_node* n) {
    AVLF_node* x = n->rchild;
    AVLF_node* y = x->lchild;

    x->lchild = n;
    n->rchild = y;

    n->height = getMaxHeightF(n) + 1;
    x->height = getMaxHeightF(x) + 1;

    return x;
}

int getBalanceF(AVLF_node* n) {
    return n == NULL ? 0 : getHeightF(n->lchild) - getHeightF(n->rchild);
}

/*
 *  Because the AVL tree structure is used,
 *  the search for the smallest element will always be found
 *  on the left side of the tree.
 */
AVLF_node* get_min_value_node(AVLF_node* node) {

    AVLF_node* current = node;

    while (!AVLF_isEmpty(current->lchild))
        current = current->lchild;

    return current;
}

AVLF_node* delete_AVLF_Node(AVLF_node* root, int keyValue) {

    if (root != NULL)
    {
        if (keyValue < getData_AVLF(root))
            root->lchild = delete_AVLF_Node(root->lchild, keyValue);
        else if (keyValue > getData_AVLF(root))
            root->rchild = delete_AVLF_Node(root->rchild, keyValue);
        else { // equal ==
            if (!isEmpty(root->clone))
            {
                /*
                 *  When the SearchTreeNode to be deleted has a linked list,
				 *	instead of deleting the tree SearchTreeNode,
				 *	the value is removed (POP) from the list and replaces the value of the AVL tree (node).
				 *	The process continues until all are removed from the list,
				 *	then the SearchTreeNode of the AVL tree can be deleted.
				 */
                LinkedNodeList* ntemp = root->clone;
                ntemp = ntemp->next;
	            root->node = pop_LinkedList(&ntemp);
            }
            else if (root->lchild == NULL || root->rchild == NULL) {

                AVLF_node* temp = NULL;

                // Both children are NULL
                if (root->lchild == NULL && root->rchild == NULL) {
                    /*
					 *  In the previous IF condition, it is examined whether the node
					 *  has at most one child. Therefore, if it has two children, it
					 *  does not pass through the IF but through the ELSE.
					 */
                    temp = root;
                    root = NULL;
                }
                else
                {
                    // Get no NULL child 
                    temp = root->lchild != NULL ? root->lchild : root->rchild;
                    *root = *temp;
                }

                free(temp);
            }
            else {
                // The node has two children
                AVLF_node* temp = get_min_value_node(root->rchild);
                // Replace the node from min
                root->node = temp->node;
                // Replace the clone, if not exist then must be NULL
                root->clone = temp->clone;
                // new node for deletion
				// the node may has at most one child because it is from minimum right -> left leaf
                root->rchild = delete_AVLF_Node(root->rchild, getData_AVLF(temp));
            }
        }
    }

    // Reconstruct Tree
    if (root != NULL)
    {
        root->height = 1 + max(getHeightF(root->lchild), getHeightF(root->rchild));

        int balance = getBalanceF(root);

        if (balance > 1 && getBalanceF(root->lchild) >= 0)
            return rightRotateF(root);

        if (balance > 1 && getBalanceF(root->lchild) < 0) {
            root->lchild = leftRotateF(root->lchild);
            return rightRotateF(root);
        }

        if (balance < -1 && getBalanceF(root->rchild) <= 0)
            return leftRotateF(root);

        if (balance < -1 && getBalanceF(root->rchild) > 0) {
            root->rchild = rightRotateF(root->rchild);
            return leftRotateF(root);
        }
    }

    return root;
}

// === === === INSERT === === === //
AVLF_node* insert_AVLF_node(AVLF_node* n, SearchTreeNode* nleaf) {

    if (n == NULL) // 
        return (create_AVLF_node(nleaf));

    if (getSTData(nleaf) < getData_AVLF(n))
        n->lchild = insert_AVLF_node(n->lchild, nleaf);
    else if (getSTData(nleaf) > getData_AVLF(n))
        n->rchild = insert_AVLF_node(n->rchild, nleaf);
    else // equals
    {
		/*
		 *  In a tree structure, duplicate values are not allowed.
		 * 
		 * Since we need duplicate values and the AVL tree has good complexity,
		 * when the same value appears in a SearchTreeNode of a tree,
		 * a linked list is attached to that SearchTreeNode to keep the same values.
		 */
        if (n->clone == NULL)
            create_LinkedList(&n->clone);
        (void)insert_LinkedList(n->clone, nleaf);

        return n;
    }

    // Reconstruct tree while getting close to root
    if (!AVLF_isEmpty(n))
    {
        n->height = 1 + max(getHeightF(n->lchild), getHeightF(n->rchild));

        int balance = getBalanceF(n);

        // Left Left Case  
        if (balance > 1 && getSTData(nleaf) < getData_AVLF(n->lchild))
            return rightRotateF(n);

        // Right Right Case  
        if (balance < -1 && getSTData(nleaf) > getData_AVLF(n->rchild))
            return leftRotateF(n);

        // Left Right Case  
        if (balance > 1 && getSTData(nleaf) > getData_AVLF(n->lchild)) {
            n->lchild = leftRotateF(n->lchild);
            return rightRotateF(n);
        }

        // Right Left Case  
        if (balance < -1 && getSTData(nleaf) < getData_AVLF(n->rchild)) {
            n->rchild = rightRotateF(n->rchild);
            return leftRotateF(n);
        }
    }

    return n;
}

int heuristic(SearchTreeNode* n)
{
    int i = 0;
    if (n != NULL)
    {
        if (option == 0)
            i = abs(n->data - fin_State);
        else // A Star
            i = (int)(floor((float)abs(n->data - fin_State) / 4));

    }
    else
        (void)printf("\nWARNING: Cannot get heuristic value from a null SearchTreeNode state");

    return i;
}

int f(SearchTreeNode* n)
{
    int i = 0;
    // f = g + h
    if (n != NULL)
        i = n->totalCost + heuristic(n);
    else
        (void)printf("\nWARNING: Cannot get f value from a null node");

    return i;
}

int getData_AVLF(AVLF_node* n)
{
    if (option == bestFS)
        return heuristic(n->node);
	return f(n->node);
}

int getSTData(SearchTreeNode* n)
{
    if (option == bestFS)
        return heuristic(n);
	return f(n);
}

SearchTreeNode* Pop_min(AVLF_node** n)
{
    AVLF_node* mnode = get_min_value_node(*n);
    SearchTreeNode* temp = mnode->node;
    *n = delete_AVLF_Node(*n, getData_AVLF(mnode));
    return temp;
}

bool dataExistAstar_AVLF(AVLF_node* root, SearchTreeNode* n)
{
    bool found = false;
    while (!found && !AVLF_isEmpty(root))
    {
        if (root->node->data == n->data)
        {
        	found = true;

            if (f(n) < f(root->node) || f(n) == f(root->node) && heuristic(n) < heuristic(root->node))
            {
	            root->node = n;
                found = false;
                break;
            }
        }
        else if (n->data > root->node->data)
            root = root->rchild;
        else
            root = root->lchild;
    }
    return found;
}

/*
 *  In addition to the existing structures, we need a copy of this function to
 *  store and sort values based on their real values.This function is used for A*
 *  so that we can also calculate the costs.
 */
AVLF_node* insert_norm_AVLF_node(AVLF_node* n, SearchTreeNode* nleaf) {

    if (n == NULL)
        return (create_AVLF_node(nleaf));

    if (nleaf->data < n->node->data)
        n->lchild = insert_norm_AVLF_node(n->lchild, nleaf);
    else if (nleaf->data > n->node->data)
        n->rchild = insert_norm_AVLF_node(n->rchild, nleaf);
    else // equals
        return n;

    if (!AVLF_isEmpty(n))
    {
        n->height = 1 + max(getHeightF(n->lchild), getHeightF(n->rchild));

        int balance = getBalanceF(n);
        if (balance > 1 && nleaf->data < n->lchild->node->data)
            return rightRotateF(n);

        if (balance < -1 && nleaf->data > n->rchild->node->data)
            return leftRotateF(n);

        if (balance > 1 && nleaf->data > n->lchild->node->data) {
            n->lchild = leftRotateF(n->lchild);
            return rightRotateF(n);
        }

        if (balance < -1 && nleaf->data < n->rchild->node->data) {
            n->rchild = rightRotateF(n->rchild);
            return leftRotateF(n);
        }
    }
    return n;
}
