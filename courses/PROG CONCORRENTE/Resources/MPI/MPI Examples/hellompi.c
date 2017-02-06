#include "mpi.h"
#include <stdio.h>

int main(int argc,char **argv){
int mynode,totalnodes;
int datasize,sender,receiver,tag,databuffer;

MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&totalnodes);
MPI_Comm_rank(MPI_COMM_WORLD,&mynode);


tag=2;

databuffer=3;
datasize=1;
if(mynode==0)
{
databuffer++;
MPI_Send(&databuffer,datasize,MPI_INT,1,tag,MPI_COMM_WORLD);
}

if(mynode==1)
{
MPI_Recv(&databuffer,datasize,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
}

printf("Hello process %d and my buffer %d ",mynode,databuffer);
printf("of totals %d\n",totalnodes);

MPI_Finalize();

}

