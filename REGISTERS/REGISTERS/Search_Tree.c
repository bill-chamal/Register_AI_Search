#pragma once
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Search_Tree.h"

const int MAX = 1000000000;
const int MIN = 0;


bool values_in_bound(Data d)
{
	return (d > MIN && d < MAX);
}

/*
 *	Before using the algorithmic tree it is required
 *	to create the root first. The same root is returned.
 */
SearchTreeNode* create_root(int init)
{
	SearchTreeNode* root = (SearchTreeNode*)malloc(sizeof(SearchTreeNode));
	if (root == NULL)
	{
		printf("\nERROR: Cannot allocate more memory!");
		exit(1);
	}

	root->data = init;		// Initial State is the ROOT
	root->parent = NULL;	// Root does not have a parent
	root->pos = -1;			// There is no parent
	root->totalCost = 0;	// Zero cost in root
	for (int i = 0; i <= 5; i++) {
		root->child[i] = NULL; // All children are pointed to NULL
		root->cost[i] = 0;
	}
	return root;
}

/*
 *	returns the child's father
 */
SearchTreeNode* get_parent(SearchTreeNode* n)
{
	return n->parent;
}

/*
 *	returns the child i
 */
SearchTreeNode* get_childi(SearchTreeNode* n, int i)
{
	return n->child[i];
}

/*
 *	returns the cost for the child i
 */
int get_cost_to_childi(SearchTreeNode* n, int i)
{
	return n->cost[i];
}

/*
 *	Creates a child of the parent SearchTreeNode referenced by
 *	its argument. The cost is the cost of creating
 *	the child and the data is added to it. The place
 *	to be placed in the array is determined by the childPlace.
 */
SearchTreeNode create_child(SearchTreeNode* parent, int cost, Data data, int childPlace)
{
	// Create a memory chunk 
	SearchTreeNode* newChild = (SearchTreeNode*)malloc(sizeof(SearchTreeNode));
	if (newChild == NULL)
	{
		printf("\nERROR: Cannot allocate more memory!");
		exit(EXIT_FAILURE);
	}

	// Add the data to the SearchTreeNode
	newChild->data = data;

	// The initial cost to a child depending on the act
	parent->cost[childPlace] = cost;
	newChild->totalCost = parent->totalCost + cost;

	// Connect Parent and child
	parent->child[childPlace] = newChild;
	newChild->parent = parent;

	// Position child from parent array
	newChild->pos = childPlace;

	// The new child created by the SearchTreeNode is the new border
	for (int i = 0; i <= 5; i++) {
		newChild->child[i] = NULL;
		newChild->cost[i] = 0;
	}

	return *newChild;
}

/*
 *	Creates all children by performing the mathematical
 *	operators allowed. Father is the SearchTreeNode entered as an
 *	argument to the function. If the SearchTreeNode is not allowed
 *	to execute the operands, no change is made.
 */
void produce_childrens(SearchTreeNode* parent)
{
	// #1 Plus X+1
	if (parent->data < MAX)
		create_child(parent, 2, parent->data + 1, 0);

	// #2 Minus X-1
	if (parent->data > MIN)
		create_child(parent, 2, parent->data - 1, 1);

	// #3 Mult 2*X
	if (parent->data > 0 && 2 * parent->data <= MAX)
		create_child(parent, ((int)ceil((float)parent->data / 2) + 1),
			parent->data * 2, 2);

	// #4 Floor Div [X/2]
	if (parent->data > MIN)
		create_child(parent, ((int)ceil((float)parent->data / 4) + 1),
			(Data)floor((float)parent->data / 2), 3);

	//#5 Power X^2
	if (pow(parent->data, 2) <= MAX)
		create_child(parent, ((int)pow(parent->data, 2) - parent->data) / 4 + 1,
			(Data)pow(parent->data, 2), 4);

	//#6 Square Root Sqrt(x)
	if (sqrt(parent->data) == ((int)(sqrt(parent->data))) && parent->data > 1)
		create_child(parent, (parent->data - (int)sqrt(parent->data)) / 4 + 1,
			(Data)sqrt(parent->data), 5);
}

/*
 *	Returns true if the SearchTreeNode has at least 1 child,
 *	otherwise returns false for none.
 */
bool has_children(SearchTreeNode* parent)
{
	bool childFound = false;
	int i = 0;
	if (parent != NULL)
		while (i <= 5 && !childFound)
		{
			if (parent->child[i] != NULL)
				childFound = true;
			i++;
		}
	return childFound;
}

/*
 *	When a SearchTreeNode has no children it indicates that it
 *	is on the border.
 */
bool is_boundary_set(SearchTreeNode* n)
{
	return !has_children(n);
}

/*
 *	It goes one level higher to his parent,
 *	then the child is searched and the location
 *	is returned. -1 for no parent (root).
 */
int getChildPosition(SearchTreeNode* child)
{
	int i = -1;

	if (child == NULL)
		printf("\nINFO: Child is null. Returning -1");
	else if (child->parent == NULL)
		printf("\nINFO: There is no parent to get child loc. Returning -1");
	else
	{
		i = child->pos;
	}
	return i;
}

/*
 *	This function is only used by depth-first-search.
 *	In the depth-first algorithm the tested nodes
 *	must be deleted. In the following function, it
 *	deletes the specific SearchTreeNode given as an argument
 *	to it. An additional function of this function
 *	is to also check if the father has no other
 *	children. If he has no other children then based
 *	on the algorithm he should be deleted as well.
 *	The function continues at a higher level and
 *	stops when a father is found that he have at
 *	least one child.
 */
void nodeDeleteBranch(SearchTreeNode** n)
{
	// Do not delete a parent with children
	if (!has_children(*n))
	{
		SearchTreeNode* parent = get_parent(*n);
		// Unlink parent to his child
		parent->child[getChildPosition(*n)] = NULL;
		Data d = (*n)->data;
		free(*n);
		// Recursive call to a higher level
		// It will stop when it finds parent with children
		nodeDeleteBranch(&parent);
		(void)printf("\nINFO: Node %d deleted successfully ", d);
	}
}

/*
 *	Returns the name instruction by the number place
 *	of child. The allocation of memory must be freed.
 */
char* get_instruction_ToString(int i)
{
	char* c = (char*)malloc(sizeof(char) * 9);
	if (c == NULL)
	{
		printf("\nERROR: Cannot allocate more memory!");
		exit(EXIT_FAILURE);
	}

	switch (i)
	{
	case INCREASE:
		(void)strcpy(c, "increase");
		break;
	case DECREASE:
		(void)strcpy(c, "decrease");
		break;
	case DOUBLE:
		(void)strcpy(c, "double");
		break;
	case HALF:
		(void)strcpy(c, "half");
		break;
	case SQUARE:
		(void)strcpy(c, "square");
		break;
	case ROOT:
		(void)strcpy(c, "root");
		break;
	default:
		printf("\nINFO: Not instruction found.");
		(void)strcpy(c, "NoInstr");
		break;
	}

	return c;
}