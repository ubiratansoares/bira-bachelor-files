#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv){

int mynode,totalnodes;
int datasize,sender,receiver,tag;

datasize=1;

int databuffer[5],recivebuffer;
int i ;
//databuffer=5;

MPI_Status status;
MPI_Init(&argc,&argv);

MPI_Comm_size(MPI_COMM_WORLD,&totalnodes);
MPI_Comm_rank(MPI_COMM_WORLD,&mynode);

tag=2;

if (mynode==0)
{
databuffer[0]=1;
databuffer[1]=2;
databuffer[2]=4;
databuffer[3]=8;
databuffer[4]=16;
}


MPI_Scatter(databuffer,datasize,MPI_INT,&recivebuffer,datasize,MPI_INT,0,MPI_COMM_WORLD);

printf("Hello process %d and my buffer %d ",mynode,recivebuffer);
printf("of totals %d\n",totalnodes);


MPI_Finalize();

}
