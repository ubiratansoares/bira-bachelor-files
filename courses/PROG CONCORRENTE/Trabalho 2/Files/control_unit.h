/****************************************************************************************************************************** 
 
* Arquivo : control_unit.h

* Data : 18/06/2010
* Professor : Paulo SL Souza
* Autores : 
		Ubiratan Soares (5634292)
		Paulo Cesar Jr. (5634890)
		Paul Kibe (5729158) 
		Petrus Gomes (5634966)	
	
******************************************************************************************************************************/

// definicoes para os blocos do caminho de dados

#include <mpi.h>

#define PR_CLOCK		0
#define PR_PC			1
#define PR_MUX_MEM		2
#define PR_MEMORY		3
#define PR_IR			4
#define PR_MDR			5
#define PR_MUX_WR		6
#define PR_MUX_WD		7
#define PR_REGISTERS	8
#define PR_SIGN_EXT		9
#define PR_REG_A		10
#define PR_REG_B		11
#define PR_MUX_ALU_A	12
#define PR_MUX_ALU_B	13
#define PR_ALU			14
#define PR_ALU_CTRL		15
#define PR_ALU_OUT		16
#define PR_CU			17

// definicoes para os opcodes das instrucoes

#define LW     35
#define RTYPE 	0
#define SW     43

//static char state = 0;

MPI_Status status;

void Next_State(char ns0, char ns1, char ns2, char ns3);

void Signals_Generate(char s0, char s1, char s2, char s3);

char ControlUnit(int ciclo);
