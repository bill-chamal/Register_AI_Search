#pragma once
#include <stdbool.h>

#define INCREASE	0
#define DECREASE	1
#define DOUBLE		2
#define HALF		3
#define SQUARE		4
#define ROOT		5

const int MAX;
const int MIN;

// kind int for Data
typedef unsigned int Data;

/*
 *	The structure is a tree where a SearchTreeNode can have
 *	from none to 6 children. Each child of the SearchTreeNode
 *	is assigned a cost to it. Each SearchTreeNode has a parent
 *	other than the root.
 */
typedef struct tree {
	Data data;
	struct tree* child[6];
	int cost[6];
	int totalCost; // is g for A* algorithm
	int pos;
	struct tree* parent;
} SearchTreeNode;

bool values_in_bound(Data d);

SearchTreeNode* create_root(int init);

SearchTreeNode* get_parent(SearchTreeNode* n);

SearchTreeNode* get_childi(SearchTreeNode* n, int i);

int get_cost_to_childi(SearchTreeNode* n, int i);

SearchTreeNode create_child(SearchTreeNode* parent, int cost, Data data, int childPlace);

void produce_childrens(SearchTreeNode* parent);

bool has_children(SearchTreeNode* parent);

bool is_boundary_set(SearchTreeNode* n);

int getChildPosition(SearchTreeNode* child);

void nodeDeleteBranch(SearchTreeNode** n);

char* get_instruction_ToString(int i);