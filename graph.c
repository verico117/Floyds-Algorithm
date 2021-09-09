/* ------------------------------------------------------------------------- */
/* Hunter Bond and Eric Vo                                                   */
/* Floyd's Parallel Assignment 2                                             */
/* ECE 4730                                                                  */
/* graph.c                                                                   */
/* ------------------------------------------------------------------------- */

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <mpi.h>
 #include "MyMPI.h"
 #include "graph.h"

 void Writes_Serial_Graph(char *name_of_file, int n, int **A){ // Writes Graph
   int i; // Initialize
   int j;

   FILE *pointer = fopen(name_of_file, "wb");
   if (pointer == NULL){
     fprintf(stderr, "Could not open %s\n", name_of_file);
     exit(-1);
   }

   fprintf(stdout, "writing graph to file %s\n", name_of_file);

   fwrite(&n, sizeof(int), 1, pointer);
   for (i = 0; i < n; i++){
     for (j = 0; j < n; j++){
       fwrite(&A[i][j], sizeof(int), 1, pointer);
     }
   }
   fclose(pointer);
 }
 // Writes process portion of graph
 void Writes_Graph_Parallel(char *name_of_file, int rank, int n, void **A,
    MPI_Comm grid_comm){
   if (rank == ROOT){
     fprintf(stdout, "writing graph to file %s\n", name_of_file);

     FILE *pointer = fopen(name_of_file, "wb");
     if (pointer == NULL){
       fprintf(stderr, "Could not open %s\n", name_of_file);
       exit(-1);
     }
     fwrite(&n, sizeof(int), 1, pointer);
     fclose(pointer);
   }
   write_checkerboard_matrix (name_of_file, (void **) A, MPI_INT, n,
    n, grid_comm);
 }

int Check_Vertices(int vertex_a, int vertex_b){ // Checks vertices
   if (vertex_a == -1 || vertex_b == -1){
     return 0;
   }
   else{
     return 1;
   }
}

void Read_Graph(char *name_of_file, int *n, int ***A){ // Reads in graph

  int i;
  int j;
  int nodes;
  int **graph = NULL;
  FILE *pointer2;

  fprintf(stdout, "reading graph from file %s\n", name_of_file);

  pointer2 = fopen(name_of_file, "rb");
  if (pointer2 == NULL){
    fprintf(stderr, "Could not open %s\n", name_of_file);
    exit(-1);
  }

  fread(&nodes, sizeof(int), 1, pointer2); // Gets amount of nodes

  graph = (int **) malloc(nodes * sizeof(int *)); // Allcoation for graph
  for (i = 0; i < nodes; i++){
    graph[i] = (int *) malloc(nodes * sizeof(int));
  }

  for (i = 0; i < nodes; i++){ // Reads file in array
    for (j = 0; j < nodes; j++){
      fread(&graph[i][j], sizeof(int), 1, pointer2);
    }
  }
  fclose(pointer2);

  *n = nodes, *A = graph; // returns array and #
}

void Print_Graph(int n, int **A){ // Prints matrix

  int i;
  int row;
  int col;

  fprintf(stdout, "Array is a %d x %d Matrix\n\n", n, n);

  fprintf(stdout, "%5s", "|");
  for (i = 0; i < n; i++){
    fprintf(stdout, "%5d", i);
  }
  fprintf(stdout, "\n");

  fprintf(stdout, "%5s", "|");
  for (i = 0; i < n; i++){
    fprintf(stdout, "%s", "-----");
  }
  fprintf(stdout, "\n");

  for (row = 0; row < n; row++){

    fprintf(stdout, "%3d%2s", row, "|");

    for (col = 0; col < n; col++){
      fprintf(stdout, "%5d", A[row][col]);
    }
    fprintf(stdout, "\n");
  }
}
