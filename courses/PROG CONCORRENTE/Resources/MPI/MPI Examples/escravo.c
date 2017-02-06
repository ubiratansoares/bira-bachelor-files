#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[]){
         int npes, rank, i, ponto_partida, numero_espacos, tam, min, solucao;
	 MPI_Comm inter_comm;
	 MPI_Status st;
	 MPI_Init(&argc,&argv);
    	 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    	 MPI_Comm_size(MPI_COMM_WORLD,&npes);
	 MPI_Comm_get_parent(&inter_comm);
         printf("%d foi gerado\n",rank);
	 fflush(stdout);
	 if (rank == 0)  MPI_Recv(&tam,1,MPI_INT,0,1,inter_comm,&st);
	 MPI_Bcast(&tam,1,MPI_INT,0,MPI_COMM_WORLD);
         int S[tam];
	 int Sb[tam];
	 printf("%d passou o broadcast: sei que o tamanho e de %d\n",rank,tam);
	 fflush(stdout);
	 MPI_Scatter(S,tam,MPI_INT,Sb,tam,MPI_INT,0,inter_comm);
         printf("%d vai processar agora com %d, %d, %d e %d\n",rank,Sb[0],Sb[1],Sb[2],Sb[3]);
	 fflush(stdout);
	 min = Sb[0];
         for (i = 1; i < tam; i++)
         	if (min > Sb[i]) min = Sb[i];
	 printf("%d encontrou %d\n",rank,min);
	 fflush(stdout);
	 MPI_Reduce(&min,&solucao,1,MPI_INT,MPI_MIN,0,MPI_COMM_WORLD);
	 if (rank == 0) MPI_Send(&solucao,1,MPI_INT,0,1,inter_comm);
	 MPI_Finalize();
    	 return 0;
}
