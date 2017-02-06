/*
  "Hello World" Type MPI Test Program
 */
 #include <mpi.h>
 #include <stdio.h>
 #include <string.h>
 
 #define tam 1
 #define TAG 0
 #define NProcs 2 
 
 int main(int argc, char *argv[]) {

       int numprocs;
       int msg;
       int myid;
       int i;
       MPI_Status stat; 
     
       MPI_Init(&argc,&argv); /* all MPI programs start with MPI_Init; all 'N' processes exist thereafter */

       MPI_Comm_size(MPI_COMM_WORLD,&numprocs); /* find out how big the SPMD world is */

       MPI_Comm_rank(MPI_COMM_WORLD,&myid); /* and this processes' rank is */
     
       /* At this point, all the programs are running equivalently, the rank is used to
          distinguish the roles of the programs in the SPMD model, with rank 0 often used
          specially... */


       //mensagem de erro personalizada qdo argumento n° de processos é diferente do necessario 
       if (numprocs!= NProcs) { 
            printf("Deve especificar MP_PROCS= %d. Terminando.\n", NProcs);
            MPI_Finalize();
            return 0;
       } 

       if (myid == 0) {
           printf("Mestre (rank %d): We have %d processors\n", myid, numprocs);
           while (1) {
                puts("Entre com a msg (inteiro): ");
		scanf("%d", &msg);
                MPI_Send(&msg, tam, MPI_INT, 1, TAG, MPI_COMM_WORLD);
                MPI_Recv(&msg, tam, MPI_INT, 1, TAG, MPI_COMM_WORLD, &stat);
                printf("Sou o mestre e recebi %d \n", msg); 
           }
       }
       else if (myid==1) {
            
           while (1) {
                MPI_Recv(&msg, tam, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
                printf("Sou o processador: %d e recebi a msg: %d e somei 1\n", myid, msg);
                msg=msg+1;
                MPI_Send(&msg, tam, MPI_INT, 0, TAG, MPI_COMM_WORLD);
           }
     
       MPI_Finalize(); /* MPI Programs end with MPI Finalize; this is a weak synchronization point */

       return 0;
     }
 }
