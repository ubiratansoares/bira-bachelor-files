all: 
	mpicc *.c -o cpu_mips_mpi -Wall

run:
	mpicc *.c -o cpu_mips_mpi -Wall
	mpirun -np 18 cpu_mips_mpi
