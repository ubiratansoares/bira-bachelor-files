/******************************************************************************************************************************

* Arquivo : mpi.c

* Data : 18/06/2010
* Professor : Paulo SL Souza
* Autores :
		Ubiratan Soares (5634292)
		Paulo Cesar Jr. (5634890)
		Paul Kibe (5729158)
		Petrus Gomes (5634966)

******************************************************************************************************************************/

#include <stdio.h>
#include <mpi.h>
#include "control_unit.h"
#include "blocks.h"

#define LW     35
#define RTYPE 	0
#define SW     43

int main (int argc, char **argv){
	 int nroProcessos; 	// número de processos
	 int IdProcesso;

	 int MEMORY[MAX_MEM];

	 MPI_Init(&argc, &argv);
	 MPI_Comm_rank(MPI_COMM_WORLD, &IdProcesso);
	 MPI_Comm_size(MPI_COMM_WORLD, &nroProcessos);

	 switch(IdProcesso){
		  //processo mestre (clock)
		case PR_CLOCK:
			// printf("\nENTRANDO EM CLOCK.");
			// fflush(NULL);
			Clock();
			break;

		  //blocos
		case PR_PC:

			// printf("\nENTRANDO EM PC.");
			// fflush(NULL);
			Pc();
			break;


		case PR_MUX_MEM:

			// printf("\nENTRANDO MUX_MEM.");
			// fflush(NULL);

			Mux_Mem();
			break;


		case PR_MEMORY:

			// printf("\nENTRANDO EM MEMORY.");
			// fflush(NULL);

			Memory(&MEMORY);
			break;


		case PR_IR:

			// printf("\nENTRANDO EM IR.");
			// fflush(NULL);

			Ir();
			break;


		case PR_MDR:

			// printf("\nENTRANDO EM MDR.");
			// fflush(NULL);


			Mdr();
			break;


		case PR_MUX_WR:

			// printf("\nENTRANDO EM MUX_WR.");
			// fflush(NULL);


			Mux_Wr();
			break;


		case PR_MUX_WD:

			// printf("\nENTRANDO EM MUX_WD.");
			// fflush(NULL);


			Mux_Wd();
			break;


		case PR_REGISTERS:

			// printf("\nENTRANDO EM REGISTERS.");
			// fflush(NULL);


			Registers();
			break;


		case PR_SIGN_EXT:

			// printf("\nENTRANDO EM SIGN_EXT.");
			// fflush(NULL);


			Sign_Ext();
			break;


		case PR_REG_A:

			// printf("\nENTRANDO EM REG_A.");
			// fflush(NULL);


			Reg_A();
			break;


		case PR_REG_B:

			// printf("\nENTRANDO EM REG_B.");
			// fflush(NULL);


			Reg_B();
			break;


		case PR_MUX_ALU_A:

			// printf("\nENTRANDO EM MUX_ALU_A.");
			// fflush(NULL);


			Mux_Alu_A();
			break;


		case PR_MUX_ALU_B:

			// printf("\nENTRANDO EM MUX_ALU_B.");
			// fflush(NULL);


			Mux_Alu_B();
			break;


		case PR_ALU:

			// printf("\nENTRANDO EM ALU.");
			// fflush(NULL);


			Alu();
			break;


		case PR_ALU_CTRL:

			// printf("\nENTRANDO EM ALU_CRTL.");
			// fflush(NULL);

			Alu_Ctrl();
			break;


		case PR_ALU_OUT:

			// printf("\nENTRANDO EM ALU_OUT.");
			// fflush(NULL);

			Alu_Out();
			break;


		case PR_CU:


			// printf("\nENTRANDO EM CU.");
			// fflush(NULL);

			Cu();
			break;
	}

	MPI_Finalize();
	return 0;
}

