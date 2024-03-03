#pragma once

#include "Forehead_List.h"

typedef int Data;

int option, fin_State;

enum {bestFS, astarS};

typedef struct  NodeF {
    SearchTreeNode* node;
    int height;
    LinkedNodeList* clone;
    struct NodeF* lchild;
    struct NodeF* rchild;
} AVLF_node;

int getHeightF(AVLF_node* n);

int getMaxHeightF(AVLF_node* n);

void initialize_AVLF(AVLF_node** n, int i, int f);

bool AVLF_isEmpty(AVLF_node* n);

int getData_AVLF(AVLF_node*);

int getSTData(SearchTreeNode*);

int heuristic(SearchTreeNode*);

AVLF_node* create_AVLF_node(SearchTreeNode* n);

AVLF_node* rightRotateF(AVLF_node* n);

AVLF_node* leftRotateF(AVLF_node* n);

int getBalanceF(AVLF_node* n);

AVLF_node* get_min_value_node(AVLF_node* node);

AVLF_node* delete_AVLF_Node(AVLF_node* root, int keyValue);

AVLF_node* insert_AVLF_node(AVLF_node* n, SearchTreeNode* nleaf);

int f(SearchTreeNode* n);

SearchTreeNode* Pop_min(AVLF_node** n);

bool dataExistAstar_AVLF(AVLF_node* root, SearchTreeNode* n);

AVLF_node* insert_norm_AVLF_node(AVLF_node* n, SearchTreeNode* nleaf);