#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AI_algorithms.h"

void check_input(char **v, int *init, int *fin, int *alg_i, FILE **file)
{
	// Get the state numbers
	char *algname;
	int initial = -1;
	int final = -1;

	algname = v[1];	// Algorithm name

	initial = strtol(v[2], NULL, 10);	// string to integer
	final = strtol(v[3], NULL, 10);

	*file = fopen(v[4], "w");

	if (*file == NULL)
	{
		printf("\nERROR: Cannot open/create file");
		exit(EXIT_FAILURE);
	}

	bool flag;
	do
	{
		flag = false;
		if (initial > MAX || initial < MIN)
		{
			flag = true;
			printf("\nThe initial state (%d) is out of bound, please enter a proper value: ", initial);
			(void)fflush(stdin);
			(void)scanf("%d", &initial);
		}

		if (final > MAX || final < MIN)
		{
			flag = true;
			printf("\nThe final state (%d) is out of bound, please enter a proper value: ", final);
			(void)fflush(stdin);
			(void)scanf("%d", &final);
		}
	}
	while (flag);
	
	do
	{
		flag = true;
		if (strcmp(algname, "breadth") == 0 || strcmp(algname, "depth") == 0 ||
			strcmp(algname, "best") == 0 || strcmp(algname, "astar") == 0)
			flag = false;

		if (flag == true)
		{
			printf("\nThe algorithm name you entered \"%s\" is not supported, please enter one below:\n"
		  "breadth (Breadth-First-Search), depth (Depth-First-Search), \nbest (Best First), astar (A*).\n$ ", algname);
			fflush(stdin);
			(void)scanf("%s", algname);
		}
	}
	while (flag);

	*init = initial;
	*fin = final;
	*alg_i = get_method(algname);
}

int main(int argc, char **argv)
{
	int initial_state=0, final_state=0, alg_i;
	FILE* fileout = NULL;

	printf  ("***************************\n"
			"* CHAMALIDIS     iis22183 *\n"
			"* University of Macedonia *\n"
			"***************************\n");

	if (argc >= 3)
	check_input(argv, &initial_state, &final_state, &alg_i, &fileout);
	else
	{
		printf("\nERROR: Not correct input!");
		exit(1);
	}

	printf("\nInitial State: %d, Final State: %d\n", initial_state, final_state);

	switch (alg_i)
	{
	case breadth:
		Breadth_First_Search(initial_state, final_state, fileout);
		break;
	case depth:
		Depth_First_Search(initial_state, final_state, fileout);
		break;
	case best:
		Best_First_Search(initial_state, final_state, fileout);
		break;
	case astar:
		AStar_Search(initial_state, final_state, fileout);
		break;
	 default:
		printf("\nINFO: Could not find algorithm's index on alg_i");
		break;
	}

	(void)fclose(fileout);

	printf("\n\n * * * Program END * * *\n");
	return EXIT_SUCCESS;
}