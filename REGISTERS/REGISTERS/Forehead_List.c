#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "Forehead_List.h"

/*
 *	It is required before using the structure
 *	that the first null SearchTreeNode has been created.
 *	In this way, other nodes can be added.
 */
void create_LinkedList(LinkedNodeList** root)
{
	LinkedNodeList* temp = (LinkedNodeList*)malloc(sizeof(LinkedNodeList));

	if (temp == NULL) {
		printf("\nERROR: Linked List cannot be created");
		exit(EXIT_FAILURE);
	}

	temp->next = NULL;
	temp->previous = NULL;
	temp->node = NULL;
	*root = temp;
}

/*
 *	Creates a new LinkedNodeList with SearchTreeNode and
 *	joins it in the list, AFTER the pointer l.
 */
LinkedNodeList* insert_LinkedList(LinkedNodeList* l, SearchTreeNode* n)
{
	LinkedNodeList* nl = (LinkedNodeList*)malloc(sizeof(LinkedNodeList));

	if (nl == NULL) {
		printf("\nERROR: Linked List cannot be inserted");
		exit(EXIT_FAILURE);
	}

	nl->node = n;

	if (l->next == NULL)
	{
		// Insert the list to where it ends
		l->next = nl;
		nl->next = NULL;
		nl->previous = l;
	}
	else
	{
		// Insert the list SearchTreeNode between two nodes
		nl->next = l->next;
		l->next->previous = nl;
		l->next = nl;
		nl->previous = l;
	}

	return nl;
}

/*
 *	The LinkedNodeList that is entered as an argument
 *	to the function is deleted and the previous one is
 *	returned if it exists (NULL). The referenced SearchTreeNode is
 *	not deleted.
 */
LinkedNodeList* delete_LinkedList(LinkedNodeList* currentList)
{
	if (currentList == NULL)
	{
		printf("\nWARNING: Cannot remove from empty list");
		return currentList;
	}

	if (currentList->previous != NULL) // The last SearchTreeNode
		currentList->previous->next = currentList->next;
	if (currentList->next != NULL) // remove between nodes
		currentList->next->previous = currentList->previous;

	LinkedNodeList* temp = currentList->previous; // If the last one will be NULL

	// Set pointers to NULL before freeing
	currentList->next = NULL;
	currentList->previous = NULL;

	free(currentList);
	return temp;
}

bool isEmpty(LinkedNodeList* currList)
{
	bool empty = true;
	// If null or not created to be avoided
	if (currList != NULL)
		empty = currList->previous == NULL && currList->next == NULL;

	return empty;
}

/*
 *	Derives the LinkedNodeList it accepts
 *	as an argument, deletes it, and 1)
 *	changes the curr pointer to point to
 *	its predecessor, 2) returns the
 *	referenced SearchTreeNode from the list
 */
SearchTreeNode* pop_LinkedList(LinkedNodeList** curr)
{
	SearchTreeNode* temp = (*curr)->node;
	*curr = delete_LinkedList(*curr);
	return temp;
}

void traverseToFront(LinkedNodeList** l)
{
	while ((*l)->next != NULL)
		*l = (*l)->next;
}

void traverseToBack(LinkedNodeList** l)
{
	while ((*l)->previous != NULL)
		*l = (*l)->previous;
}
