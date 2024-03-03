#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "AI_algorithms.h"

/*
 *	DEBUG 1:
 *	Shows Pop and insert actions performed by the algorithms.
 *	Also shows how search tree is created with values.
 *	Execution time becomes slow.
 */
#define DEBUG 0
#define TIMEOUT 60	// 1 min execution time

int get_method(char* s)
{
	int i = -1;
	if (strcmp(s, "breadth") == 0)
		i = breadth;
	else if (strcmp(s, "depth") == 0)
		i = depth;
	else if (strcmp(s, "best") == 0)
		i = best;
	else if (strcmp(s, "astar") == 0)
		i = astar;
	else printf("\nINFO: No method found.");
	return i;
}

clock_t tStart;
clock_t tEnd;

void timer_start()
{
	tStart = clock();
}

bool timer_tick()
{
	bool timer = true;
	clock_t t = clock();
	if (t - tStart > CLOCKS_PER_SEC * TIMEOUT)
	{
		printf("\n = = = TIME OUT = = = ");
		timer = false;
	}
	return timer;
}

void timer_end()
{
	tEnd = clock();
}

/*
 *	Function: printSolution
 *	----------------------------
 *	Prints the steps of a solution, along with
 *	the count of steps and the total cost of the solution.
 *
 *	Parameters:
 *	- final_state: Pointer to the final state SearchTreeNode of the solution.
 *	- fileout: File pointer for output (where the results will be printed).
 */
void printSolution(SearchTreeNode* final_state, FILE* fileout)
{
	if (final_state == NULL)
		return;

	int count = 0, sumCost = 0, child_pos, prev = -1, intrCount = 0;
	SearchTreeNode* p = final_state;
	LinkedNodeList* steps;
	create_LinkedList(&steps);

	/*
	 *	Each situation that led to the solution is placed on the
	 *	stack upside down. From the state of the solution to the
	 *	initial.
	 */
	while (p != NULL)
	{
		steps = insert_LinkedList(steps, p);
		if (get_parent(p) != NULL)
			count++;

		p = get_parent(p);
	}

	sumCost = final_state->totalCost;

	(void)fprintf(fileout, "%d, %d", count, sumCost);
	(void)printf("\n< < PRINTING SOLUTION > >");

	/*
	 *	After the steps of the solution were inserted into the
	 *	variable 'steps,' which is of linked list type, we remove
	 *	them in reverse order from the sequence in which they were
	 *	added like a stack. One element is removed at a time, and
	 *	it is checked whether the SearchTreeNode has a child. If it does not
	 *	have a child because it is the root, the iteration continues
	 *	on the child, if it exists. The value of the previous SearchTreeNode
	 *	from the tree is printed using the operator that was executed
	 *	to generate the child or the solution. The iteration continues
	 *	until the entire 'steps' list is empty.
	 */
	while (!isEmpty(steps))
	{
		p = pop_LinkedList(&steps);
		child_pos = getChildPosition(p);
		if (child_pos != -1)
		{
			/*
			 *	Retrieve the string representation of the instruction
			 *	that led from the parent to the current SearchTreeNode.
			 */
			char* instr = get_instruction_ToString(child_pos);

			(void)fprintf(fileout, "\n%s, %d, %d", instr,
				p->parent->data, get_cost_to_childi(p->parent, child_pos));

			if (prev != child_pos)
			{
				if (intrCount != 0) // Avoid the first loop
					(void)printf("\n\t X %d TIMES the upper function", intrCount);

				(void)printf("\n %s, %d, %d => %d", instr,
					p->parent->data, get_cost_to_childi(p->parent, child_pos), p->data);

				intrCount = 1;
			}
			else
				intrCount++;

			// Free the allocated memory from string pointer get_instruction_ToString
			free(instr);
		}
		prev = child_pos;
	}
	(void)printf("\n\t X %d TIMES the upper function", intrCount);

	(void)printf("\nSteps count = %d, \tTotal Cost = %d, \tTime: %f sec", count, sumCost, (float)(tEnd - tStart)/CLOCKS_PER_SEC);

	free(steps);
}


void Breadth_First_Search(int init, int fin, FILE* fileout)
{
	timer_start();

	int loopCount = 0;
	SearchTreeNode* root = NULL, * currentState = NULL;
	root = create_root(init);
	LinkedNodeList* frontier = NULL;

	// Initialise frontier
	create_LinkedList(&frontier);
	const LinkedNodeList* front = frontier;
	// Insert the ROOT to stack list
	frontier = insert_LinkedList(frontier, root);

	// Save the states to break infinity loops, only copies the data
	AVL_node* loopList;
	initialize_AVL(&loopList);

	// Solution found
	bool flag = false;

	printf("\n************************"
		"\n* BREADTH FIRST SEARCH *"
		"\n************************");


	while (!isEmpty(frontier) && !flag && timer_tick())
	{
		// Pop the front state of the frontier (forehead search)
		currentState = pop_LinkedList(&frontier);

		#if DEBUG
			printf("\nPop: %d", currentState->data);
		#endif

		// If the SearchTreeNode is inside the border or already checked then loop 
		if (!dataExists_AVL(loopList, currentState->data))
		{
			// Copy Data to loopList to avoid loops
			loopList = insert_avl_node(loopList, currentState->data);

			// Check the SOLUTION
			if (currentState->data == fin)
			{
				printf("\nINFO: Solution found in Breadth!");
				flag = true;
				break;
			}

			// Expand children and connect to their parent
			produce_childrens(currentState);

			// Put children in the !BACK! of forehead
			/*
			 *	Because the new nodes are added
			 *	in the middle of the list (to
			 *	the left of null), they reverse
			 *	themselves by adding them in.
			 */
			for (int i = 0; i <= 5; i++)
			{
				if (get_childi(currentState, i) != NULL) {

					(void)insert_LinkedList(front, get_childi(currentState, i));

					#if DEBUG
						printf("\n%d new child[%d] -> %d", currentState->data, i, currentState->child[i]->data);
					#endif
				}
				else
				{
					#if DEBUG
						printf("\n%d nul child[%d] -> NULL", currentState->data, i);
					#endif
				}
			}
		}
		loopCount++;

		traverseToFront(&frontier);
	}

	timer_end();

	printf("\nSum steps: %d", loopCount);

	// Free the memory
	free(frontier);
	free(loopList);

	if (!flag)
		printf("\nINFO: No solutions found in Breadth");
	else
		printSolution(currentState, fileout);

	free(root);
}

void Depth_First_Search(int init, int fin, FILE* fileout)
{
	timer_start();

	int loopCount = 0;
	SearchTreeNode* root = NULL, * currentState = NULL;
	root = create_root(init);
	LinkedNodeList* frontier = NULL;

	// Initialise frontier
	create_LinkedList(&frontier);
	// Insert the ROOT to stack list
	frontier = insert_LinkedList(frontier, root);

	// Save the states to break infinity loops, only copies the data
	AVL_node* loopList;
	initialize_AVL(&loopList);

	// Solution found
	bool flag = false;

	printf("\n**********************"
		"\n* DEPTH FIRST SEARCH *"
		"\n**********************");

	while (!isEmpty(frontier) && !flag && timer_tick())
	{
		// Get execute the front state of the frontier (forehead search)
		currentState = pop_LinkedList(&frontier);

		#if DEBUG
			printf("\nPop: %d", currentState->data);
		#endif

		// If the SearchTreeNode is inside the border or the value already checked then loop 
		if (!dataExists_AVL(loopList, currentState->data))
		{
			// Copy Data to loopList to avoid loops
			loopList = insert_avl_node(loopList, currentState->data);

			// Check the SOLUTION
			if (currentState->data == fin)
			{
				printf("\nINFO: Solution found in depth!");
				flag = true;
				break;
			}

			// If more solutions goto loop
			// . . . 

			// Expand children and connect to their parent
			produce_childrens(currentState);

			// Put children in the !FRONT! of forehead
			for (int i = 5; i >= 0; i--)
			{
				if (get_childi(currentState, i) != NULL) {

					frontier = insert_LinkedList(frontier, get_childi(currentState, i));
					#if DEBUG
						printf("\n%d new child[%d] -> %d", currentState->data, i, frontier->node->data);
					#endif

				}
				else {
					#if DEBUG
						printf("\n%d nul child[%d] -> NULL", currentState->data, i);
					#endif
				}


			}
		}
		loopCount++;

		// If no new children produced or infinity-loop, the SearchTreeNode will be deleted
		if (!has_children(currentState))
			nodeDeleteBranch(&currentState);
	}

	timer_end();

	printf("\nSum steps: %d", loopCount);

	// Free the memory
	free(frontier);
	free(loopList);

	if (!flag)
		printf("\nINFO: No solutions found in depth");
	else
		printSolution(currentState, fileout);

	free(root);
}

void Best_First_Search(int init, int fin, FILE* fileout)
{
	timer_start();

	int loopCount = 0;
	SearchTreeNode* root = NULL, * currentState = NULL;
	root = create_root(init);
	AVLF_node* frontier = NULL;

	// Initialise frontier
	initialize_AVLF(&frontier, bestFS, fin);

	// Insert the ROOT to frontier search
	frontier = insert_AVLF_node(frontier, root);

	// Solution found
	bool flag = false;

	printf("\n*********************"
		"\n* BEST FIRST SEARCH *"
		"\n*********************");

	while (!AVLF_isEmpty(frontier) && !flag && timer_tick())
	{
		// Get execute the front state of the frontier (forehead search)
		currentState = Pop_min(&frontier);

		#if DEBUG
			printf("\nPop: %d", currentState->data);
		#endif

		// Check the SOLUTION
		if (currentState->data == fin)
		{
			printf("\nINFO: Solution found in Best!");
			flag = true;
			break;
		}

		// If more solutions goto loop
		// . . . 

		// Expand children and connect to their parent
		produce_childrens(currentState);

		// Put children in the forehead
		for (int i = 5; i >= 0; i--)
		{
			if (get_childi(currentState, i) != NULL) {

				frontier = insert_AVLF_node(frontier, get_childi(currentState, i));

				#if DEBUG
					printf("\n%d new child[%d] -> %d, \th(%d)", currentState->data, i,
						currentState->child[i]->data, heuristic(currentState->child[i]));
				#endif
			}
			else
			{
				#if DEBUG
					printf("\n%d nul child[%d] -> NULL", currentState->data, i);
				#endif
			}
		}

		loopCount++;
	}

	timer_end();

	printf("\nSum steps: %d", loopCount);

	// Free the memory
	free(frontier);

	if (!flag)
		printf("\nINFO: No solutions found in Best");
	else
		printSolution(currentState, fileout);

	free(root);
}

void AStar_Search(int init, int fin, FILE* fileout)
{
	timer_start();

	int loopCount = 0;
	SearchTreeNode* root = NULL, * currentState = NULL;
	root = create_root(init);
	AVLF_node* frontier = NULL;

	// Initialise frontier
	initialize_AVLF(&frontier, astarS, fin);
	// Insert the ROOT to AVL forehead
	frontier = insert_AVLF_node(frontier, root);

	// Save the states to break infinity loops, only copies the data
	AVLF_node* innerList = NULL;
	initialize_AVLF(&innerList, astarS, fin);

	// Solution found
	bool flag = false;

	printf("\n*****************"
		"\n* A Star SEARCH *"
		"\n*****************");

	while (!AVLF_isEmpty(frontier) && !flag && timer_tick())
	{
		// Get execute the front state of the frontier (forehead search)
		currentState = Pop_min(&frontier);

		#if DEBUG
			printf("\nPop: %d", currentState->data);
		#endif

		// If the SearchTreeNode is checked then loop 
		if (!dataExistAstar_AVLF(innerList, currentState))
		{
			// Copy Data to loopList to avoid loops
			innerList = insert_norm_AVLF_node(innerList, currentState);

			// Check the SOLUTION
			if (currentState->data == fin)
			{
				printf("\nINFO: Solution found in A Star!");
				flag = true;
				break;
			}

			// If more solutions goto loop
			// . . . 

			// Expand children and connect to their parent
			produce_childrens(currentState);

			// Put children in the forehead
			for (int i = 5; i >= 0; i--)
			{
				if (get_childi(currentState, i) != NULL) {

					frontier = insert_AVLF_node(frontier, get_childi(currentState, i));

					#if DEBUG
						printf("\n%d new child[%d] -> %d, \t%d=%d+%d", currentState->data, i, currentState->child[i]->data, f(currentState->child[i]),
							currentState->child[i]->totalCost, heuristic(currentState->child[i]));
					#endif

				}
				else
				{
					#if DEBUG
						printf("\n%d nul child[%d] -> NULL", currentState->data, i);
					#endif
				}
			}
		}

		loopCount++;
	}

	timer_end();

	printf("\nSum steps: %d", loopCount);

	// Free structures from memory
	free(innerList);
	free(frontier);

	if (!flag)
		printf("\nINFO: No solutions found in A Star");
	else
		printSolution(currentState, fileout);

	// Free search tree
	free(root);
}
