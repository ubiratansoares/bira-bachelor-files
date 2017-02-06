#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[]){
    MPI_Comm inter_comm;
    MPI_Status st;
    int npes = 4;
    int array_of_errcodes[10];
    int S[] = {12, 65, 31, 24, 47, 82, 93, 2, 45, 66, 32, 14, 87, 93, 65, 17};
    int rank, i, ponto_partida, numero_espacos, tam, min, solucao;
    tam = 16;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    numero_espacos = tam/npes;
    ponto_partida = rank*numero_espacos;
    char escravo[20];
    strcpy(escravo,"escravo");
    int Sb[numero_espacos];
    MPI_Comm_spawn(escravo, MPI_ARGV_NULL, npes, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &inter_comm, array_of_errcodes);
    MPI_Send(&numero_espacos,1,MPI_INT,0,1,inter_comm);
    MPI_Scatter(S,numero_espacos,MPI_INT,Sb,numero_espacos,MPI_INT,MPI_ROOT,inter_comm);
    MPI_Recv(&solucao,1,MPI_INT,0,1,inter_comm,&st);
    printf("Resposta final: %d\n",solucao);
    MPI_Finalize();
    return 0;
}
