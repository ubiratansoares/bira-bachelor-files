// Sat1.c
// Retirado de Quinn J.

#include <mpi.h>
#include <stdio.h>

#define EXTRACTBIT(n,i) ((n&(1<<i))?1:0)

void circuitCheck(int, int);

int main(int argc, char** argv){
	
	
	int i;	
	int id;	// Rank do processo
	int p;	// Numero do processo
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	
	for(i = id; i < 65536; i += p)
		circuitCheck(id,i);
		
	printf("Process %d is done. \n",id);
	fflush(stdout);
	MPI_Finalize();
	
	return 0;	
		
}//end main

void circuitCheck(int id, int z){
	
	int v[16];
	int i;
	
	for(i = 0; i < 16; i++)
		v[i] = EXTRACTBIT(z,i);
		
		
	if( (v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) 
		&& (!v[3] || !v[4]) && (v[4] || !v[5])
		&& (v[5] || !v[6]) && (v[5] || v[6])
		&& (v[6] || !v[15]) && (v[7] || !v[8])
		&& (!v[7] || !v[13]) && (v[8] || v[9])
		&& (v[8] || !v[9]) && (!v[9] || !v[10])
		&& (v[9] || v[11]) && (v[10] || v[11])
		&& (v[12] || v[13]) && (v[13] || !v[14])
		&& (v[14] || v[15])
	){
		
		printf("%d >> %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", id, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
		fflush(stdout);
			
	}	
	
}