//MPI_MM_N2.C

//mpicc mpi_mm_n2.c -o mpi_mm_n2
//mpirun -np 26 mpi_mm_n2

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define N 5                  /* dimension in matrix */
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int main (int argc, char *argv[]){
      int	numtasks,              /* number of tasks in partition */
	      taskid,                /* a task identifier */
	      numworkers,            /* number of worker tasks */
	      source,                /* task id of message source */
	      dest,                  /* task id of message destination */
	      mtype,                 /* message type */
	      rows,                  /* rows of matrix A sent to each worker */
	      column,
	      averow, extra, offset, /* used to determine rows sent to each worker */
	      i, j, k ;           /* misc */
      double	a[N][N],           /* matrix A to be multiplied */
	            b[N][N],           /* matrix B to be multiplied */
	            c[N][N];           /* result matrix C */
      MPI_Status status;

      MPI_Init(&argc,&argv);
      MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
      MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
      
      numworkers = numtasks-1; //menos o master

/**************************** master task ************************************/
   if (taskid == MASTER){
      printf("mpi_mm has started with %d tasks.\n",numtasks);
      printf("Initializing arrays...\n");
      //valores para Matriz A
      printf("\nMatrix A...\n");
      for (i=0; i<N; i++){
         for (j=0; j<N; j++){
            a[i][j]= i+j;
            printf("%6.2f   ", a[i][j]);
         }
         printf("\n"); 
      }

      //valores para matriz B
      printf("\nMatrix B...\n");      
      for (i=0; i<N; i++){
         for (j=0; j<N; j++){
            b[i][j]=(i+1)*(j+1);
            printf("%6.2f   ", b[i][j]);
         }            
         printf("\n"); 
      }
      printf("\n");          

      mtype = FROM_MASTER;   //master=1
      rows=0;
      column=0;
      for (dest=1; dest<=numworkers; dest++)
      {
         
         rows=dest/N;
         column=(dest-1)%N;
         printf("Sending %d rows to task %d and column=%d\n",rows,dest,column);
         
         MPI_Send(&column, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(a, N*N, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(b, N*N, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
      }

      /* Receive results from worker tasks */
      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++){
         source = i;
         MPI_Recv(&column, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&c[rows][column], 1, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
         printf("Received results from task %d\n",source);
      }

      /* Print results */
      printf("******************************************************\n");
      printf("Result Matrix:\n");
      for (i=0; i<N; i++)
      {
         printf("\n"); 
         for (j=0; j<N; j++) 
            printf("%6.2f   ", c[i][j]);
      }
      printf("\n******************************************************\n");
      printf ("Done.\n");
   }


/**************************** worker task ************************************/
   if (taskid > MASTER){
      mtype = FROM_MASTER;
      MPI_Recv(&column, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(a, N*N, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(b, N*N, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

	// Multiplication on worker

      i=rows;
      j=column;
      
      c[i][j] = 0.0;
      for (k=0; k<N; k++)
         c[i][j] +=  a[i][k] * b[k][j];

      mtype = FROM_WORKER;

      MPI_Send(&column, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&c[i][j], 1, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
   }
   MPI_Finalize();
}


/*
   $ mpicc mpi_mm_n2.c -o mpi_mm_n2
   $ mpirun -np 26 mpi_mm_n2
   
   Saída:
   mpi_mm has started with 26 tasks.
   Initializing arrays...

   Matrix A...
     0.00     1.00     2.00     3.00     4.00   
     1.00     2.00     3.00     4.00     5.00   
     2.00     3.00     4.00     5.00     6.00   
     3.00     4.00     5.00     6.00     7.00   
     4.00     5.00     6.00     7.00     8.00   

   Matrix B...
     1.00     2.00     3.00     4.00     5.00   
     2.00     4.00     6.00     8.00    10.00   
     3.00     6.00     9.00    12.00    15.00   
     4.00     8.00    12.00    16.00    20.00   
     5.00    10.00    15.00    20.00    25.00   

   Sending 0 rows to task 1 and column=0
   Sending 0 rows to task 2 and column=1
   Sending 0 rows to task 3 and column=2
   Sending 0 rows to task 4 and column=3
   Sending 1 rows to task 5 and column=4
   Sending 1 rows to task 6 and column=0
   Sending 1 rows to task 7 and column=1
   Sending 1 rows to task 8 and column=2
   Sending 1 rows to task 9 and column=3
   Sending 2 rows to task 10 and column=4
   Sending 2 rows to task 11 and column=0
   Sending 2 rows to task 12 and column=1
   Sending 2 rows to task 13 and column=2
   Sending 2 rows to task 14 and column=3
   Sending 3 rows to task 15 and column=4
   Sending 3 rows to task 16 and column=0
   Sending 3 rows to task 17 and column=1
   Sending 3 rows to task 18 and column=2
   Sending 3 rows to task 19 and column=3
   Sending 4 rows to task 20 and column=4
   Sending 4 rows to task 21 and column=0
   Sending 4 rows to task 22 and column=1
   Sending 4 rows to task 23 and column=2
   Sending 4 rows to task 24 and column=3
   Sending 5 rows to task 25 and column=4
   Received results from task 1
   Received results from task 2
   Received results from task 3
   Received results from task 4
   Received results from task 5
   Received results from task 6
   Received results from task 7
   Received results from task 8
   Received results from task 9
   Received results from task 10
   Received results from task 11
   Received results from task 12
   Received results from task 13
   Received results from task 14
   Received results from task 15
   Received results from task 16
   Received results from task 17
   Received results from task 18
   Received results from task 19
   Received results from task 20
   Received results from task 21
   Received results from task 22
   Received results from task 23
   Received results from task 24
   Received results from task 25
   ******************************************************
   Result Matrix:

    40.00    80.00   120.00   160.00     0.00   
    55.00   110.00   165.00   220.00   275.00   
    70.00   140.00   210.00   280.00   350.00   
    85.00   170.00   255.00   340.00   425.00   
   100.00   200.00   300.00   400.00   500.00   
   ******************************************************
   Done.

*/
