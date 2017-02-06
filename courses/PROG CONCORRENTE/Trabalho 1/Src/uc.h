/******************************************************************************************************************************** 
 
* Arquivo : uc.h
* Esse arquivo contém as definições para a Unidade de Controle da CPU MIPS proposta
* no trabalho 1, na disciplina de Programação Concorrente.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
	Ubiratan Soares (5634292)
	Paulo César Jr.
	Paul Kibe
	Petrus Gomes		

********************************************************************************************************************************/

#include "masks.h"

// Definicoes para os opcodes das instrucoes

#define LW     35
#define RTYPE 0
#define SW     43

static char state = 0;

// Funcao : Next_State(ns0,ns1,ns2)
// Essa funcao monta o rotulo (numero inteiro) do proximo estado da MEF para o qual deve-se desviar
// de acordo com o ciclo da instrucao em processamento


void Next_State(char ns0, char ns1, char ns2);

// Funcao : Signals_Generate(s0,s1,s2,s3)
// Essa funcao gera os sinais de controles que serao externados pela UC, de acordo com o ciclo da instrucao que
// esta sendo executada. Os 7 estados colocados na MEF sao representados por 3 bits.
// 
// De acordo com a MEF que implementa a UC para as instrucoes propostas, os sinais de controle para cada
// estado sao colocados a seguir, considerando DONT CARE = 0:


// 00 - RegDst
// 01 - RegWrite
// 02 - ALUSrcA
// 03 - ALUSrcB0
// O4 - ALUSrcB1
// 05 - ALUOp0
// 06 - ALUOp1
// 07 - PCWrite
// 08 - IorD
// 09 - MemRead
// 10 - MemWrite
// 11 - MemtoReg
// 12 - IRWrirte


//             | 12 | 11 | 10 | 09 | 08 | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |

//  0          | 1  | 0  | 0  | 1  | 0  | 1  | 0  | 0  | 0  | 1  | 0  | 0  | 0  | 

//  1          | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 1  | 0  | 0  | 0  |

//  2          | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 0  | 1  | 0  | 0  |

//  3          | 0  | 0  | 0  | 1  | 1  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  |

//  4          | 0  | 1  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 0  |

//  5          | 0  | 0  | 1  | 0  | 1  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 

//  6          | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 0  | 0  | 0  | 1  | 0  | 0  |

//  7          | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 1  |

int Signals_Generate(char s0, char s1, char s2);


// Funcao : UnidadeControle(opcode)
// Essa funcao aceita o opcode de uma instrucao para retornar, a cada ciclo, os sinais de controle adequados
// para a execucao da instrucao. Essa tarefa consiste em determinar a partir do estado atual e do opcode da
// instrucao, para qual estado deve-se desviar no fluxo interno a MEF (funcao de proximo estado explicito),
// e gerar os sinais de controle baseados no estado atual apropriados

int UnidadeControle(int opcode); 

#include "uc.c"