#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TAM_CARRINHO 3
#define TAM_VONTADE 2
#define SINAL_SAIDA -1


void passeio(int seconds){
	{
  clock_t endwait;
  endwait = clock() + seconds * CLOCKS_PER_SEC/100 ;
  while (clock() < endwait) {}
}

}

int main(int argc, char* argv[]){
	int rank, nproc, aux;
	int *sync;
	sync = (int*)malloc(sizeof(int));
	*sync = 0;
	int i;
	MPI_Status st;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	if (rank == 0){
		int pessoas_desejam_passear = nproc - 1;
		int pessoas_na_rodada;
		int viajantes[TAM_CARRINHO];
		//fflush(stdin);
		while (pessoas_desejam_passear > 0){
			pessoas_na_rodada = TAM_CARRINHO;
			if (pessoas_desejam_passear < TAM_CARRINHO) pessoas_na_rodada = pessoas_desejam_passear;
			for (i = 0; i < pessoas_na_rodada; i++){
				MPI_Recv(sync,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&st);
				viajantes[i] = *sync;
				MPI_Send(&i,1,MPI_INT,viajantes[i],viajantes[i],MPI_COMM_WORLD);
				printf("Pessoa %d entrou no carrinho\n",viajantes[i]);
			}
			for (i = 0; i < pessoas_na_rodada; i++){ /*avisa a partida do carrinho*/
				MPI_Send(&i,1,MPI_INT,viajantes[i],viajantes[i],MPI_COMM_WORLD);
			}
			printf("Tenho %d pessoas no carrinho, hora de ir!!!\n",pessoas_na_rodada);
			passeio(5);
			for (i = 0; i < pessoas_na_rodada; i++){ /*avisa a chegada do carrinho*/
				MPI_Send(&i,1,MPI_INT,viajantes[i],viajantes[i],MPI_COMM_WORLD);
				MPI_Recv(sync,1,MPI_INT,viajantes[i],viajantes[i],MPI_COMM_WORLD,&st);
				aux = viajantes[i];
				viajantes[i] = 0;
				 /*esperando o cara saber que ele pode sair do carrinho.
				Se ele estiver satisfeito, subtraio 1 do numero de pessoas que querem passear*/
				if (*sync == SINAL_SAIDA){
					pessoas_desejam_passear--;
					printf("O %d foi-se embora para nao mais voltar\n",aux);
				}
			}
			printf("O carrinho voltou\n");
		}
		printf("Passeios concluidos!\n");
	}
	else {
		int passeando = 1;
		int na_fila = 0;
		int no_carrinho = 0;
		int satisfeito = 0;
		while (satisfeito < TAM_VONTADE){
		passeio(2);/*passeio pelo parque*/
		printf("Pessoa %d na fila\n",rank);
		passeando = 0;
		na_fila = 1;
		MPI_Send(&rank,1,MPI_INT,0,0,MPI_COMM_WORLD);/*estou na fila*/
		MPI_Recv(sync,1,MPI_INT,0,rank,MPI_COMM_WORLD,&st);/*estou no carrinho*/
		no_carrinho = 1;
		na_fila = 0;
		MPI_Recv(sync,1,MPI_INT,0,rank,MPI_COMM_WORLD,&st);/*comecou*/
		printf("%d está se divertindo muito agora!!!\n", rank);
		MPI_Recv(sync,1,MPI_INT,0,rank,MPI_COMM_WORLD,&st);/*acabou*/
		printf("%d vazou do carrinho\n",rank);
		passeando = 1;
		no_carrinho = 0;
		satisfeito++;
		int acabou = SINAL_SAIDA;
		if (satisfeito == TAM_VONTADE){
			MPI_Send(&acabou,1,MPI_INT,0,rank,MPI_COMM_WORLD);/*satisfeito*/
			//break;
		}
		else
			MPI_Send(&rank,1,MPI_INT,0,rank,MPI_COMM_WORLD);/*quero mais*/
		}
	}
	printf("Processo %d terminou\n",rank);
	//MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
