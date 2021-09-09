/* ------------------------------------------------------------------------- */
/* Hunter Bond and Eric Vo                                                   */
/* Floyd's Parallel Assignment 2                                             */
/* ECE 4730                                                                  */
/* print-graph.c                                                             */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
#include "graph.h"

int main (int argc, char *argv[]){ // main function and variables
  int i;
  int nodes;
  int **graph;

  //check for input file name (set default if none given)
  char *input_file = "default-make-graph-file.dat";
  if(argc != 1){
  	input_file = argv[1];
  }
  //read in file
  Read_Graph(input_file, &nodes, &graph);
  //print matrix
  Print_Graph(nodes, graph);

  for (i = 0; i < nodes; i++)
  {
    free(graph[i]); // free memory
  }
  free(graph); // free memory

  return 0;
}
