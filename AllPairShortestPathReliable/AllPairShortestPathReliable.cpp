#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PATH "C:\\Users\\Eucliwood\\Desktop\\stat(SaveMode)\\Sequential\\"
#define INF 999999
#define SIZE 8

void distance_generate(int[][SIZE]);
void distance_useexample(int[][SIZE]);
void find_AllPairShortestPath(int[][SIZE], int[][SIZE], int[][SIZE][SIZE]);
void array_print(int[][SIZE]);
void log_save(float);
void find_path(int[][SIZE], int, int);
void fix_path(int[][SIZE], int[][SIZE], int[][SIZE][SIZE], int u, int v);

int main()
{
	clock_t start, end;

	start = clock();

	int i, j, k;

	//declare distance and path
	int(*distance)[SIZE], (*path)[SIZE];
	distance = (int(*)[SIZE]) malloc(SIZE * sizeof(int[SIZE]));
	path = (int(*)[SIZE]) malloc(SIZE * sizeof(int[SIZE]));

	//Reliable : Integer structure
	int(*allpath)[SIZE][SIZE];
	allpath = (int(*)[SIZE][SIZE]) malloc(SIZE * sizeof(int[SIZE][SIZE]));
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			for (k = 0; k < SIZE; k++)
				allpath[k][i][j] = j;

	//Initial Path
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			path[i][j] = j;
		}
	}

	//generate data
	distance_useexample(distance);
	//distance_generate(distance);

	//Find Shortest Path
	find_AllPairShortestPath(distance, path, allpath);

	end = clock();

	//Print ShortestDistance
	/*printf("Shortest Distance\n");
	array_print(distance);
	printf("\n");*/

	//Print ShortestPath
	/*printf("Shortest Path\n");
	array_print(path);
	printf("\n");*/

	//Reliable
	//Before Broke
	printf("Before broke\n");
	find_path(path, 1, 7);
	printf(" distace : %d\n", distance[1][7]);
	//Fix
	fix_path(distance, path, allpath, 1, 7);
	printf("\nAfter Fix\n");
	find_path(path, 1, 7);
	printf(" distace : %d\n", distance[1][7]);
	//Edge Relate
	printf("\nOther Relate\n");
	find_path(path, 0, 7);
	printf(" distace : %d\n", distance[0][7]);

	/*
	float diff = ((float)(end - start) / 1000000.0F) * 1000;
	printf("%.4f\n", diff);

	log_save(diff);
	*/

	getchar();

	return 0;
}

void fix_path(int distance[][SIZE], int path[][SIZE], int allpath[][SIZE][SIZE], int u, int v)
{

	//Reliable : fix direct edge
	//assume select k = 3
	int k = 0;
	for (int i = SIZE - 1; i > 0; i--)
		if (allpath[i][u][v] != path[u][v])
		{
			k = i;
			break;
		}
	int newpath = allpath[k][u][v];
	path[u][v] = newpath;
	distance[u][v] = distance[u][newpath] + distance[newpath][v];

	//TODO fix all path that use this edge : O(n)
	//update other edge that use this edge
	for (int s = 0; s < SIZE; s++)
		if (path[s][v] == u)
		{
			distance[s][v] = distance[s][u] + distance[u][v];
			fix_path(distance, path, allpath, s, v);
		}
}

void find_AllPairShortestPath(int distance[][SIZE], int path[][SIZE], int allpath[][SIZE][SIZE])
{
	for (int k = 0; k < SIZE; k++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				int new_weight = distance[i][k] + distance[k][j];

				if (new_weight < distance[i][j])
				{
					distance[i][j] = new_weight;
					path[i][j] = path[i][k];

					//Method 2 : Integer structure
					for (int r = k; r < SIZE; r++)
						allpath[r][i][j] = allpath[r][i][k];
				}
			}
		}
	}
}

void distance_generate(int data[][SIZE])
{
	int i, j, r;

	for (i = 0; i < SIZE; i++)
	{
		data[i][i] = 0;
		for (j = i + 1; j < SIZE; j++)
		{
			r = (rand() % 20) + 1;
			if (r == 19)
				data[i][j] = INF;
			else
				data[i][j] = r;
			data[j][i] = data[i][j];
		}
	}
}

void array_print(int distance[][SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (distance[i][j] == INF)
				printf("%7s", "INF");
			else
				printf("%7d", distance[i][j]);
		}
		printf("\n");
	}
}

void distance_useexample(int data[][SIZE])
{
	int example[8][8] = {
		{ 0,1,9,3,INF,INF,INF,INF },
		{ 1,0,INF,1,INF,3,INF,INF },
		{ 9,INF,0,INF,INF,3,10,INF },
		{ 3,1,INF,0,5,INF,INF,8 },
		{ INF,INF,INF,5,0,2,2,1 },
		{ INF,3,3,INF,2,0,INF,INF },
		{ INF,INF,10,INF,2,INF,0,4 },
		{ INF,INF,INF,8,1,INF,4,0 }
	};

	int i, j;

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			data[i][j] = example[i][j];
		}
	}
}

void find_path(int shortestpath[][SIZE], int u, int v)
{
	if (shortestpath[u][v] == NULL)
		printf("[]");

	printf("[%d]", u);
	while (u != v) {
		u = shortestpath[u][v];
		printf("[%d]", u);
	}
}

void log_save(float diff)
{
	FILE * fp;
	char filePath[70] = PATH;
	char fileName[10];
	sprintf(fileName, "%d.txt", SIZE);
	strcat(filePath, fileName);
	fp = fopen(filePath, "a");
	fprintf(fp, "%.4f\n", diff);
	fclose(fp);
}
