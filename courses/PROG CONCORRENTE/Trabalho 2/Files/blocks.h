/****************************************************************************************************************************** 
 
* Arquivo : blocos.h

* Data : 18/06/2010
* Professor : Paulo SL Souza
* Autores : 
		Ubiratan Soares (5634292)
		Paulo Cesar Jr. (5634890)
		Paul Kibe (5729158) 
		Petrus Gomes (5634966)	
	
******************************************************************************************************************************/
#include <mpi.h>

#define MAX_MEM 100

MPI_Status status;

void Clock ();

void Cu();

void Pc();

void Mux_Mem();

void Memory();

void Ir();

void Mdr();

void Mux_Wr();

void Mux_Wd();

void Registers();

void Sign_Ext();

void Reg_A();

void Reg_B();

void Mux_Alu_A();

void Mux_Alu_B();

void Alu();

void Alu_Ctrl();

void Alu_Out();
