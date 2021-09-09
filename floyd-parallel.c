/* ------------------------------------------------------------------------- */
/* Hunter Bond and Eric Vo                                                   */
/* Floyd's Parallel Assignment 2                                             */
/* ECE 4730                                                                  */
/* floyd-parallel.c                                                          */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <math.h>
#include "MyMPI.h"
#include "graph.h"

// Prototype for floy_parallel function
void floyd_parallel (int r, int size, int ***graph, int nodes,
   int Coordinates[], int Size_of_Grid[],
    MPI_Comm Row_Comm, MPI_Comm Column_Comm);

int main (int argc, char *argv[]){ // main function & initial variables
  int r;
  int size;
  int nodes;
  int ID;
  int Size_of_Grid[2] = {0, 0};
  int periodic[2] = {0, 0};
  int Coordinates[2] = {0, 0};
  int **graph;
  int *Single_Graph;
  double Execution_Time;
  double Floyd_Execution_Time;

  MPI_Comm grid_comm, Row_Comm, Column_Comm;

  MPI_Init(&argc, &argv); // Initialization
  MPI_Barrier(MPI_COMM_WORLD); // Start time is set

  Execution_Time = -MPI_Wtime();

  MPI_Comm_size(MPI_COMM_WORLD, &size); // # of MPI task
  MPI_Comm_rank(MPI_COMM_WORLD, &r); // Gets MPI task

  if (argc < 3){
    if (r == ROOT){
  	   fprintf(stdout,
         "The Program requires Input File and Output File Arguments.\n");
    }
  	MPI_Finalize();

  	exit(-1);
  }
  // Grid Comm is developed
  MPI_Dims_create(size, 2, Size_of_Grid);
  MPI_Cart_create(MPI_COMM_WORLD, 2, Size_of_Grid, periodic, 0, &grid_comm);
  MPI_Comm_rank(grid_comm, &ID); // Gets Cord. Values
  MPI_Cart_coords(grid_comm, ID, 2, Coordinates);
  // Creates Row Comm
  MPI_Comm_split(grid_comm, Coordinates[0], Coordinates[1], &Row_Comm);
  // Creates Column Comm
  MPI_Comm_split(grid_comm, Coordinates[1], Coordinates[0], &Column_Comm);

  if (r == ROOT){ // Reads Graph
    fprintf(stdout, "Reading Graph from the File %s\n", argv[1]);
  }
  read_checkerboard_matrix (argv[1], (void ***) &graph, (void **)
    &Single_Graph, MPI_INT, &nodes, grid_comm);

  MPI_Barrier(MPI_COMM_WORLD); // Sets start time
  Floyd_Execution_Time = -MPI_Wtime();

  floyd_parallel (r, size, &graph, nodes,
    Coordinates, Size_of_Grid, Row_Comm, Column_Comm); // Runs floyd

  MPI_Barrier(MPI_COMM_WORLD); // Sets end time
  Floyd_Execution_Time += MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD); // Short path is outputed

  Writes_Graph_Parallel(argv[2], r, nodes, (void **) graph, grid_comm);
  // Free comm space
  MPI_Comm_free(&Row_Comm);
  MPI_Comm_free(&Column_Comm);
  MPI_Comm_free(&grid_comm);
  MPI_Barrier(MPI_COMM_WORLD); // Sets end time

  Execution_Time += MPI_Wtime();

  if (r == ROOT){ // results
    fprintf(stdout, "Floyd-Parallel Execution Time:\n");
    fprintf(stdout, "\tn = %5d nodes\n", nodes);
    fprintf(stdout, "\tp = %5d cpus\n", size);
    fprintf(stdout, "\tptime = %6.5f (sec)\n", Execution_Time);
    fprintf(stdout, "\tftime = %6.5f (sec)\n", Floyd_Execution_Time);
  }
  // ends and returns
  MPI_Finalize();
  return 0;
}

// Parallel implementation of Floyd's algorithm
void floyd_parallel (int r, int size, int ***graph, int nodes,
  int Coordinates[], int Size_of_Grid[],
   MPI_Comm Row_Comm, MPI_Comm Column_Comm){ // Function and variables
   int i;
   int j;
   int k;
   int o;
   int ID;
   int Off_Set;
   int Local_Rows = BLOCK_SIZE(Coordinates[0],Size_of_Grid[0],nodes);
   int Local_Columns = BLOCK_SIZE(Coordinates[1],Size_of_Grid[1],nodes);
   int* K_Coumns = (int *) malloc(Local_Rows * sizeof(int));
   int* K_Rows = (int *) malloc(Local_Columns * sizeof(int));

   for (k = 0; k < nodes; k++) {
     // For Row
     MPI_Comm_rank(Column_Comm, &ID); // Gets ID & process
     o = BLOCK_OWNER(k, Size_of_Grid[0], nodes); // Gets owner of row

     if (ID == o){
       Off_Set = k - BLOCK_LOW(ID, Size_of_Grid[0], nodes); // Prints graph
       for (i = 0; i < Local_Columns; i++){
         K_Rows[i] = (*graph)[Off_Set][i];
       }
     }
    // For Column
     MPI_Bcast(K_Rows, Local_Columns, MPI_INT, o, Column_Comm);
     MPI_Comm_rank(Row_Comm, &ID); // Gets ID & process
     o = BLOCK_OWNER(k, Size_of_Grid[1], nodes);  // Gets owner of column

     if (ID == o){
       Off_Set = k - BLOCK_LOW(ID, Size_of_Grid[1], nodes); // Prints graph
       for (i = 0; i < Local_Rows; i++){
         K_Coumns[i] = (*graph)[i][Off_Set];
       }
     }

     MPI_Bcast(K_Coumns, Local_Rows, MPI_INT, o, Row_Comm);

		for (i = 0; i < Local_Rows; i++){ // Floyd's implementation for Kth value
			for (j = 0; j < Local_Columns; j++){
			 if (
          // First Condtiion
					(Check_Vertices(K_Rows[j], K_Coumns[i]) && (*graph)[i][j] == -1)

																 ||
          // Second Condition
					(Check_Vertices(K_Rows[j], K_Coumns[i]) && (K_Rows[j] + K_Coumns[i])
            < (*graph)[i][j])
				  )
					(*graph)[i][j] = K_Rows[j] + K_Coumns[i];
			}
		}
  }
   free(K_Rows);
   free(K_Coumns);
}
