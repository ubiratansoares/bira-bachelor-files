//Compilar: mpicc token.c -o token -WaLL
//Executar: 


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char **argv){
   int vet_par[100], vet_imp[100];
//   int tag=1;
   int my_rank, new_rank,  par, imp, i,dado,size;
   int num_proc=100;
  // int token=0;
   dado=0;
   //MPI_Status status;
   MPI_Group grupo_velho, grupo_par, grupo_imp;
   MPI_Comm comm_impar, comm_par, communicator;
   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &num_proc);   
   
   vet_par[0]=0;
   vet_imp[0]=0;
   
   par=1; 
   imp=1;
   for(i=1; i< num_proc;i++){
      if(!(i%2))
         vet_par[par++]=i;
      else 
         vet_imp[imp++]=i;
   }






   MPI_Comm_group(MPI_COMM_WORLD, &grupo_velho);
   MPI_Group_incl(grupo_velho, par, vet_par, &grupo_par);
   MPI_Group_incl(grupo_velho, imp, vet_imp, &grupo_imp);
   
   MPI_Comm_create(MPI_COMM_WORLD, grupo_par, &comm_par);
   MPI_Comm_create(MPI_COMM_WORLD, grupo_imp, &comm_impar);   

   if(!(my_rank%2)) communicator=comm_par;
   else communicator=comm_impar;
 
   MPI_Comm_rank(communicator, &new_rank);
   
// MPI_Comm_rank(communicator, &new_rank);

   printf("my_rank = %d new_rank = %d\n", my_rank, new_rank);

MPI_Comm_size(communicator, &size);




   printf("tamanho = %d \n", size);

if(my_rank==0){
dado++;
              
printf("Processo %d enviando dado %d para %d\n", my_rank, dado,new_rank+1);

MPI_Send(&dado, 1, MPI_INT, new_rank+1, 1, comm_par); 

MPI_Comm_size(comm_par, &size); //para determinar o ultimo par

MPI_Recv(&dado, 1, MPI_INT, size-1, 1, comm_par, &status); 

MPI_Comm_size(comm_impar, &size);
dado--;

                printf("Processo %d envia dado %d para %d\n", my_rank, dado,size-1);

MPI_Send(&dado, 1, MPI_INT, size-1, 1, comm_impar);

  MPI_Recv(&dado, 1, MPI_INT, 1, 1, comm_impar, &status); 
printf(" DADO %d\n",dado);

}


else if(my_rank%2==0){
MPI_Comm_size(comm_par, &size); 
MPI_Recv(&dado, 1, MPI_INT, new_rank-1, 1, comm_par, &status); 
dado++;

if (size-1==new_rank){

        printf("Processo %d enviando dado %d para %d\n", my_rank, dado,0);
MPI_Send(&dado, 1, MPI_INT, 0, 1, comm_par); 
printf("PASSO\n");
}

else{
printf("Processo %d enviando dado %d para %d\n", my_rank, dado,new_rank+1);
MPI_Send(&dado, 1, MPI_INT, new_rank+1, 1, comm_par); 
}
}


else{
MPI_Comm_size(comm_impar, &size); 

if (size-1==new_rank) {
MPI_Recv(&dado, 1, MPI_INT,0, 1, comm_impar, &status); 
}

else
MPI_Recv(&dado, 1, MPI_INT, new_rank+1, 1, comm_impar, &status); 
dado--;

printf("Processo %d enviando dado %d para %d\n", my_rank, dado,new_rank-1);
MPI_Send(&dado, 1, MPI_INT, new_rank-1, 1, comm_impar); 

}

   
   MPI_Finalize();
   
   return 0;
}

