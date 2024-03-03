#pragma once

#include "Forehead_AVL.h"
#include "AVL_Tree.h"

enum
{
	breadth, depth, best, astar
};

int get_method(char* s);

void printSolution(SearchTreeNode* finState, FILE* fileout);

void Depth_First_Search(int init, int fin, FILE* fileout);

void Breadth_First_Search(int init, int fin, FILE* fileout);

void Best_First_Search(int init, int fin, FILE* fileout);

void AStar_Search(int init, int fin, FILE* fileout);