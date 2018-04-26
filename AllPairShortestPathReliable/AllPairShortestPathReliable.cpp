#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PATH "C:\\Users\\Eucliwood\\Desktop\\stat(SaveMode)\\Sequential\\"
#define INF 999999
#define SIZE 8

void distance_generate(int[][SIZE]);
void distance_useexample(int[][SIZE]);
void find_AllPairShortestPath(int[][SIZE][SIZE], int[][SIZE][SIZE]);
void array_copy(int[][SIZE], int[][SIZE]);
void array_print(int[][SIZE]);
void log_save(float);
void find_path(int[][SIZE], int, int);
void fix_path(int[][SIZE][SIZE], int[][SIZE][SIZE], int u, int v);

int main()
{
	clock_t start, end;

	start = clock();

	int i, j;

	int (*alldistance)[SIZE][SIZE], (*allpath)[SIZE][SIZE];
	alldistance = (int(*)[SIZE][SIZE]) malloc(SIZE * sizeof(int[SIZE][SIZE]));
	allpath = (int(*)[SIZE][SIZE]) malloc(SIZE * sizeof(int[SIZE][SIZE]));

	//Initial path
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
				allpath[0][i][j] = j;

	//generate data
	distance_useexample(alldistance[0]);
	//alldistance[0][1][5] = INF;
	//distance_generate(distance);

	//Find Shortest Path
	find_AllPairShortestPath(alldistance, allpath);

	end = clock();

	//fix_path(alldistance, allpath, 1 ,5);

	//Print ShortestDistance
	printf("Shortest Distance\n");
	array_print(alldistance[SIZE-1]);
	printf("\n");

	//Print ShortestPath
	printf("Shortest Path\n");
	array_print(allpath[SIZE-1]);
	printf("\n");

	//find path
	//find_path(allpath[SIZE-1], 1, 6);


	float diff = ((float)(end - start) / 1000000.0F) * 1000;
	printf("%.4f\n", diff);

	//log_save(diff);
	

	getchar();

	return 0;
}

void fix_path(int alldistance[][SIZE][SIZE], int allpath[][SIZE][SIZE], int u, int v)
{
	//mark a broken edge 
	alldistance[0][u][v] = INF;

	//useless edge
	if (allpath[SIZE-1][u][v] != v)
	{
		printf("This edge is useless\n");
		return;
	}

	//re-calculate : edge broke at node 0
	if (u == 0)
	{
		printf("Re-Calculate\n");
		find_AllPairShortestPath(alldistance, allpath);
		return;
	}	

	//find new shortest path when pass k = 0 to k < u
	for (int k = 0; k < u; k++)
	{
		int new_weight = alldistance[k][u][k] + alldistance[k][k][v];

		if (new_weight < alldistance[k][u][v])
		{
				alldistance[k][u][v] = new_weight;
				allpath[k][u][v] = allpath[k][u][k];
		}
		alldistance[k+1][u][v] = alldistance[k][u][v];
		allpath[k+1][u][v] = allpath[k][u][v];
	}

	//Copy colum v from k = u-1 to u
	for (int i = 0; i < SIZE; i++)
	{
		alldistance[u][i][v] = alldistance[u-1][i][v];
		allpath[u][i][v] = allpath[u-1][i][v];
	}

	//Begin calculate at k = u
	for (int k = u; k < SIZE; k++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				int new_weight = alldistance[k][i][k] + alldistance[k][k][j];

				if (new_weight <  alldistance[k][i][j])
				{
					allpath[k][i][j] = allpath[k][i][k];
					alldistance[k][i][j] = new_weight;
				}
			}
		}
		if (k + 1 < SIZE)
		{
			array_copy(alldistance[k], alldistance[k + 1]);
			array_copy(allpath[k], allpath[k + 1]);
		}
		/*printf("k = %d\n", k);
		array_print(alldistance[k]);
		printf("\n");*/
	}
}

void find_AllPairShortestPath(int alldistance[][SIZE][SIZE], int allpath[][SIZE][SIZE])
{
	for (int k = 0; k < SIZE; k++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				int new_weight = alldistance[k][i][k] + alldistance[k][k][j];

				if (new_weight <  alldistance[k][i][j])
				{
					allpath[k][i][j] = allpath[k][i][k];
					alldistance[k][i][j] = new_weight;
				}
			}
		}

		if (k + 1 < SIZE)
		{
			array_copy(alldistance[k], alldistance[k + 1]);
			array_copy(allpath[k], allpath[k + 1]);
		}
		printf("k = %d\n", k);
		array_print(alldistance[k]);
		printf("\n");
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

void array_copy(int sour[][SIZE], int dest[][SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			dest[i][j] = sour[i][j];
		}
	}
}
void array_print(int data[][SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (data[i][j] == INF)
				printf("%7s", "INF");
			else
				printf("%7d", data[i][j]);
		}
		printf("\n");
	}
}

void distance_useexample(int alldata[][SIZE])
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

	int i, j, k;

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			alldata[i][j] = example[i][j];
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
