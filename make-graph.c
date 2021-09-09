/* ------------------------------------------------------------------------- */
/* Hunter Bond and Eric Vo                                                   */
/* Floyd's Parallel Assignment 2                                             */
/* ECE 4730                                                                  */
/* make-graph.c                                                              */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
#include "graph.h"

// prototype
void Process_Command_Line(int argc, char *argv[], int *nodes, int *amount,
   int *chance, char *ofile[]);

int main (int argc, char *argv[]) // main function and variables
{
  int i;
  int j;
  int u;
  // Set initial values
  int nodes = 5;
  int amount = 100;
  int chance = 150;
  int **graph = NULL;
  char *output_file = "default-make-graph-file.dat";
  time_t t;

  // command line arguments
  Process_Command_Line(argc, argv, &nodes, &amount, &chance, &output_file);

  graph = (int **) malloc(nodes * sizeof(int *));
  for (i = 0; i < nodes; i++)
  {
    graph[i] = (int *) malloc(nodes * sizeof(int)); // allocate space for graph
  }

  srand(time(&t)); // Random number generator

  for (i = 0; i < nodes; i++) // Edge calculator
  {
    for (j = 0; j < nodes; j++)
    {
      u = rand() % chance;

      if (i == j)
      {
        graph[i][j] = 0;
      }
      else if (u <= amount)
      {
        graph[i][j] = u;
      }
      else
      {
        graph[i][j] = -1;
      }
    }
  }

  Writes_Serial_Graph(output_file, nodes, graph); // Graph -> Output

  if (strcmp(output_file, "default-make-graph-file.dat") != 0) // default file
  {
    free(output_file); // Free space
  }

  for (i = 0; i < nodes; i++)
  {
    free(graph[i]); // Free space
  }
  free(graph); // Free space

  return 0;
}
// Allows graph to accept four arguments
void Process_Command_Line(int argc, char *argv[], int *nodes,
   int *amount, int *chance, char *ofile[])
{
    int c;
    while ((c = getopt(argc, argv, "n:r:p:o:")) != -1)
    {
      switch(c) // switch statement for case types /converts string
      {
          case 'n': // node amount
            *nodes = atoi(optarg);
            break;
          case 'a': // edge amount weight
            *amount = atoi(optarg);
            break;
          case 'c': // max chance of occuring
            *chance = atoi(optarg);
            break;
          case 'o': // output file name
            if (strcmp(optarg, "default-make-graph-file.dat") != 0)
            {
              *ofile = strdup(optarg);
            }
            break;
      }
    }
}
