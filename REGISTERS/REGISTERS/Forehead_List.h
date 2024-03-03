#pragma once

#include "Search_Tree.h"

struct LinkedList;

/*
 *	This structure is linked
 *	list with previous and next.
 *	Each LinkedNodeList has a reference
 *	to a SearchTreeNode.
 */
typedef struct LinkedList
{
	SearchTreeNode* node;
	struct LinkedList* next;
	struct LinkedList* previous;
} LinkedNodeList;

void create_LinkedList(LinkedNodeList** root);

LinkedNodeList* insert_LinkedList(LinkedNodeList* l, SearchTreeNode* n);

LinkedNodeList* delete_LinkedList(LinkedNodeList* currentList);

bool isEmpty(LinkedNodeList* currList);

SearchTreeNode* pop_LinkedList(LinkedNodeList** curr);

void traverseToFront(LinkedNodeList** l);

void traverseToBack(LinkedNodeList** l);
