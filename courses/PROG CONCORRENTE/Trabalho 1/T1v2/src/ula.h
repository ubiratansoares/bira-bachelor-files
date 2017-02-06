/******************************************************************************************************************************** 
 
* Arquivo : ula.h
* Esse arquivo contem as definicoes para a unidade logica e aritmetica componente do processador MIPS multiciclo, proposto
* no trabalho 1, na disciplina de Programação Concorrente. Tambem aqui encontram-se as implementacoes para a Unidade de Controle
* da ULA.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
		Ubiratan Soares (5634292)
		Paulo César Jr. (5634890)
		Paul Kibe (5729158) 
		Petrus Gomes (5634966)	

********************************************************************************************************************************/


// Funcao : int ULA(op1, op2, control)
// Essa funcao implementa a unidade funcional ULA do processador MIPS multiciclo


// Mascara para separar os bits de controle enviados pela unidade de controle da ULA

#include "masks.h"

typedef struct{ int op1; int op2; char control;} ULA_args;
typedef struct{ char ALUOp; char field; } UC_ULA_args;

int ULA(void* thread_args);

char UC_ULA(void* thread_args);

#include "ula.c"