/******************************************************************************************************************************** 
 
* Arquivo : muxes.h
* Esse arquivo contem as definicoes dos multiplexadores utilizados para o processador MIPS multiciclo, proposto
* no trabalho 1, na disciplina de Programação Concorrente. Tambem aqui encontra-se a definica da unidade signalextend.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
		Ubiratan Soares (5634292)
		Paulo César Jr. (5634890)
		Paul Kibe (5729158) 
		Petrus Gomes (5634966)	

*******************************************/

#include "masks.h"

typedef struct{ char control; int PC; int ALUOUT;  } mux0_args;

typedef struct{ char control; int MDR; int ALUOUT; } mux1_args;

typedef struct{ char control; int IR;} mux2_args;

typedef struct{ char control; int PC; int A; } mux3_args;

typedef struct{ char control; int B; int IR; } mux4_args;

// UNIDADES MULTIPLEXADORAS

// Funcao : mux0(control)
// Multiplexa o dado a ser escrito ver na memoria

int mux0(void* thread_args);

// Funcao : mux2(control)
// Multiplexa o endereco do registrador a ser escrito 

char mux2(void* thread_args);


// Funcao : mux1(control)
// Multiplexa o dado a ser escrito no banco de registradores

int mux1(void* thread_args);


// Funcao : mux3(control)
// Multiplexa a entrada do primeiro operando da ULA


int mux3(void* thread_args);



// Funcao : mux4(control)
// Multiplexa a entrada do segundo operando da ULA

int mux4(void* thread_args);


// Funcao : signalextend(value)
// Faz a extensao de sinal do valor de entrada

int signalextend(int value);

#include "muxes.c"