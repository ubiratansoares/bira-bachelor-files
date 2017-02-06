/******************************************************************************************************************************** 
 
* Arquivo : muxes.h
* Esse arquivo contem as definicoes dos multiplexadores utilizados para o processador MIPS multiciclo, proposto
* no trabalho 1, na disciplina de Programação Concorrente. Tambem aqui encontra-se a definica da unidade signalextend.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
	Ubiratan Soares (5634292)
	Paulo César Jr.
	Paul Kibe
	Petrus Gomes		

*******************************************/

#include "masks.h"

// UNIDADES MULTIPLEXADORAS

// Funcao : mux0(control)
// Multiplexa o dado a ser escrito ver na memoria


int mux0(char control);

// Funcao : mux2(control)
// Multiplexa o endereco do registrador a ser escrito 

char mux2(char control);


// Funcao : mux1(control)
// Multiplexa o dado a ser escrito no banco de registradores

int mux1(char control);


// Funcao : mux3(control)
// Multiplexa a entrada do primeiro operando da ULA


int mux3(char control);



// Funcao : mux4(control)
// Multiplexa a entrada do segundo operando da ULA

int mux4(char control);


// Funcao : signalextend(value)
// Faz a extensao de sinal do valor de entrada

int signalextend(int value);

#include "muxes.c"