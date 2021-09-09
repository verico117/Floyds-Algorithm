/*---------------------------------------------------------------*/
// Hunter Bond and Eric Vo
// Floyd's Parallel, Assignment 2
// ECE 4730
// floyd-serial.c
/*---------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>
#include <getopt.h>
#include <mpi.h>
#include "graph.h"

//-------------------------------------------------------//
//------Find the shortest path (Floyd's Algorithm)-------//
//-------------------------------------------------------//
void floyds_algorithm(int n, int **a)
{
	int i, j, k;
	//print_graph(n, a);
	for(k = 0; k < n; k++)
	{
		for(i = 0; i < n; i++)
		{
			for(j = 0; j < n; j++)
			{
				if((a[i][j] >= 0) && (a[i][k] >= (0)) && (a[k][j] >= (0)))
				{	

					//--------------------------------------------//
					//  The if statement below is equivalent to:  //
					//    a[i,j] = min(a[i,j],a[i,k]+a[k,j]);     //
					//--------------------------------------------//
					if(a[i][j] < (a[i][k]+a[k][j]))
					{
						a[i][j] = a[i][j];
					}
					else if( (a[i][k]+a[k][j]) < a[i][j])
					{
						a[i][j] = (a[i][k]+a[k][j]); 
					}
				}
			}
		}
	}
	//print_graph(n, temp);	
}

int main(int argc, char *argv[])
{
	//-------------------------------------------------------//
	//------------ Process Command Line Arguments -----------//	
	//-------------------------------------------------------//
	//int 	index;
	char 	*input_file = "default-make-graph-file.dat";
	char 	*output_file = "default-make-graph-file.seq";
	if(argc == 3)
	{
		input_file = argv[1];
		output_file = argv[2];

	}
	

	

	//-------------------------------------------------------//
	//---------------------Read in Graph---------------------//
	//-------------------------------------------------------//
	int 	nodes;
	int 	**graph;
	double 	ptime, ftime;

	MPI_Init(&argc, &argv);
	
	MPI_Barrier(MPI_COMM_WORLD);
	ptime = -MPI_Wtime();
	
	Read_Graph(input_file,&nodes,&graph); 

	//-------------------------------------------------------//
	//-----------Run Floyd's Algorithm and get time----------//
	//-------------------------------------------------------//
	MPI_Barrier(MPI_COMM_WORLD);
	ftime = -MPI_Wtime();
	floyds_algorithm(nodes, graph);	
	ftime += MPI_Wtime();
	Writes_Serial_Graph(output_file,nodes,graph);
	
	MPI_Barrier(MPI_COMM_WORLD);
	ptime += MPI_Wtime();

	MPI_Finalize();

	printf("\tn = %d nodes\n", nodes);
	printf("\tp = 1 cpus\n");
	printf("\tptime = %6.6f (sec)\n", ptime);
	printf("\tftime = %6.6f (sec)\n", ftime);

	return 0;	
}

