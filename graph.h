/* ------------------------------------------------------------------------- */
/* Hunter Bond and Eric Vo                                                   */
/* Floyd's Parallel Assignment 2                                             */
/* ECE 4730                                                                  */
/* graph.h                                                                   */
/* ------------------------------------------------------------------------- */

#define ROOT 0

void Writes_Serial_Graph(char *name_of_file, int n, int **A);

void Writes_Graph_Parallel(char *name_of_file, int rank, int n,
   void **A, MPI_Comm grid_comm);

void write_checkerboard_matrix (char *, void **,
   MPI_Datatype,int, int, MPI_Comm);

int Check_Vertices(int vertex_a, int vertex_b);

void Read_Graph(char *name_of_file, int *n, int ***A);

void Print_Graph(int n, int **A);
