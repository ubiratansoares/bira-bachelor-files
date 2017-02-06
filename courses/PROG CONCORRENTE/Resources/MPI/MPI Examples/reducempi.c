#include "mpi.h"

#include <stdio.h>

#include <string.h>

int main(int argc,char **argv){

int mynode,totalnodes;
int datasize,sender,receiver,tag;

datasize=1;


int databuffer,recivebuffer;

//databuffer=5;

MPI_Status status;
MPI_Init(&argc,&argv);

MPI_Comm_size(MPI_COMM_WORLD,&totalnodes);
MPI_Comm_rank(MPI_COMM_WORLD,&mynode);

tag=2;

if (mynode==1)
databuffer=2;
if (mynode==0)
databuffer=4;

if (mynode==2)
databuffer=8;

if (mynode==3)
databuffer=16;


MPI_Reduce(&databuffer,&recivebuffer,datasize,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
if (mynode==0){
printf("Hello process %d and my buffer %d ",mynode,recivebuffer);
printf("of totals %d\n",totalnodes);
}

MPI_Finalize();

}
