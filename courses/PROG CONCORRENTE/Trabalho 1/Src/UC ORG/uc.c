/******************************************************************************************************************************** 
 * Arquivo : hardwired_code.c
 * Esse arquivo contém a implementação para a Unidade de Controle da CPU MIPS proposta
 * no trabalho 2, na disciplina de Organizacao de Computadores.
 * Data : 06/12/2009
 * Professor : Paulo SL Souza
 * Autor : Ubiratan Soares (5634292)
********************************************************************************************************************************/


// Definicoes para os opcodes das instrucoes

#define LW     35
#define RTYPE 0
#define SW     43


// Variavel Global para armazenar o estado em que se encontra a MEF que implementa a UC

static char state = 0;


// Funcao : Next_State(ns0,ns1,ns2,ns3)
// Essa funcao monta o rotulo (numero inteiro) do proximo estado da MEF para o qual deve-se desviar
// de acordo com o ciclo da instrucao em processamento

void Next_State(char ns0, char ns1, char ns2, char ns3) {
    state = ns0;
    state = state + (ns1 << 1);
    state = state + (ns2 << 2);
    state = state + (ns3 << 3);
}


// Funcao : Signals_Generate(s0,s1,s2,s3)
// Essa funcao gera os sinais de controles que serao externados pela UC, de acordo com o ciclo da instrucao que
// esta sendo executada. Os 16 estados colocados na MEF sao representados por 4 bits.
// 
// De acordo com a MEF que implementa a UC para as instrucoes propostas, os sinais de controle para cada
// estado sao colocados a seguir, considerando DONT CARE = 0:


// 00 - RegDst0      02- RegWrite
// 03 - ALUSrcA      04 - ALUSrcB0      05- ALUSrcB1
// 06 - ALUOp0       07 - ALUOp1        11- PCWrite
// 12 - IorD         13 - MemRead       14- MemWrite
// 15 - MemtoReg     17- IRWrite



//             | 17 | 15 | 14 | 13 | 12 | 11 | 07 | 06 | 05 | 04 | 03 | 02 | 00 |

//  0          | 1  | 0  | 0  | 1  | 0  | 1  | 0  | 0  | 0  | 1  | 0  | 0  | 0  | 

//  1          | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 1  | 0  | 0  | 0  |

//  2          | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 0  | 1  | 0  | 0  |

//  3          | 0  | 0  | 0  | 1  | 1  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  |

//  4          | 0  | 1  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 0  |

//  5          | 0  | 0  | 1  | 0  | 1  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 

//  6          | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 0  | 0  | 0  | 1  | 0  | 0  |

//  7          | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 1  |


int Signals_Generate(char s0, char s1, char s2, char s3) {

	// Incializando variavel de retorno de 32 bits, os 18 bits mais significativos representam os sinais de controle,
	// os bits de 32 a 19 sao zero.

    int signals = 0;

	// Sinais de controle, cada um representado como uma variavel de 8 bits, todos inicializados como zero

    char RegDst = 0, 
		RegWrite = 0, 
		ALUSrcA = 0, 
		ALUSrcB0 = 0, 
		ALUSrcB1 = 0, 
		ALUOp0 = 0, 
		ALUOp1 = 0, 
		PCWrite = 0, 
		IorD = 0, 
		MemRead = 0, 
		MemWrite = 0, 
		MemtoReg = 0, 
		IRWrite = 0; 


	// Equacoes logicas para cada sinal de controle de acordo com a tabela anterior

	RegDst =      (!s3 &  s2 &  s1 &  s0) || (!s3 &  s2 &  s1 &  s0) || ( s3 &  s2 &  s1 & !s0);
	RegWrite =     (!s3 &  s2 & !s1 & !s0) || (!s3 &  s2 & !s1 & !s0) || (!s3 &  s2 &  s1 &  s0) || ( s3 & !s2 &  s1 &  s0);
	ALUSrcA =      (!s3 & !s2 &  s1 & !s0) || (!s3 & !s2 &  s1 & !s0) || (!s3 &  s2 &  s1 & !s0) || ( s3 & !s2 & !s1 & !s0) || ( s3 &  s2 & !s1 & !s0) || ( s3 &  s2 &  s1 &  s0);
	ALUSrcB0 =     (!s3 & !s2 & !s1 & !s0) || (!s3 & !s2 & !s1 &  s0);
	ALUSrcB1 =     (!s3 & !s2 & !s1 &  s0) || (!s3 & !s2 &  s1 & !s0) || ( s3 &  s2 &  s1 &  s0);
	ALUOp0 =       ( s3 & !s2 & !s1 & !s0) || ( s3 &  s2 & !s1 & !s0) || ( s3 &  s2 &  s1 &  s0);
	ALUOp1 =       (!s3 &  s2 &  s1 & !s0) || ( s3 &  s2 &  s1 &  s0);
	PCWrite =      (!s3 & !s2 & !s1 & !s0) || ( s3 & !s2 & !s1 &  s0) || ( s3 & !s2 &  s1 & !s0) || ( s3 &  s2 & !s1 &  s0);
	IorD =         (!s3 & !s2 &  s1 &  s0) || (!s3 &  s2 & !s1 &  s0);
	MemRead =      (!s3 & !s2 & !s1 & !s0) || (!s3 & !s2 &  s1 &  s0);
	MemWrite =     (!s3 &  s2 & !s1 &  s0);
	MemtoReg =    (!s3 &  s2 & !s1 & !s0);
	IRWrite =      (!s3 & !s2 & !s1 & !s0);


	// Montando variavel de 32 bits de saida

    signals = signals + RegDst;
    signals = signals + (RegWrite << 2);
    signals = signals + (ALUSrcA << 3);
    signals = signals + (ALUSrcB0 << 4);
    signals = signals + (ALUSrcB1 << 5);
    signals = signals + (ALUOp0 << 6);
    signals = signals + (ALUOp1 << 7);
    signals = signals + (PCWrite << 11);
    signals = signals + (IorD << 12);
    signals = signals + (MemRead << 13);
    signals = signals + (MemWrite << 14);
    signals = signals + (MemtoReg << 15);
    signals = signals + (IRWrite << 17);

    return signals;

}

// Funcao : UnidadeControle(opcode)
// Essa funcao aceita o opcode de uma instrucao para retornar, a cada ciclo, os sinais de controle adequados
// para a execucao da instrucao. Essa tarefa consiste em determinar a partir do estado atual e do opcode da
// instrucao, para qual estado deve-se desviar no fluxo interno a MEF (funcao de proximo estado explicito),
// e gerar os sinais de controle baseados no estado atual apropriados

int UnidadeControle(int opcode) {
	
	// Variaveis para bits de representacao do estado atual
	
    char s3 = 0, s2 = 0, s1 = 0, s0 = 0;     

	// Variaveis para bits de representacao do proximo estado

    char ns3 = 0, ns2 = 0, ns1 = 0, ns0 = 0; 

    // Inicialmente, isolam-se os bits de cada posicao da variavel "estado"

    s0 = (state & 1);      // 0001
    s1 = (state & 2) >> 1; // 0010
    s2 = (state & 4) >> 2; // 0100
    s3 = (state & 8) >> 3; // 1000


    // Implementacao para a Maquina de Estados Finitos (MEF), de acordo com a imagem enviada juntamente com esse codigo.

    // A MEF esta no estado 0

    if (!s3 & !s2 & !s1 & !s0) {

        // Alterando os bits de proximo estado para o estado 1
        ns0 = 1;
        ns1 = 0;
        ns2 = 0;
        ns3 = 0;

    }

    // A MEF está no estado 1
    else if (!s3 & !s2 & !s1 & s0) {
            if (opcode == LW || opcode == SW) {
        
        		// Alterando os bits de proximo estado para o estado 2
                ns0 = 0;
                ns1 = 1;
                ns2 = 0;
                ns3 = 0;

            } else if (opcode == RTYPE) {
                
				// Alterando os bits de proximo estado para o estado 6
                ns0 = 0;
                ns1 = 1;
                ns2 = 1;
                ns3 = 0;
        }
    
    // A MEF está no estado 2
    else
        if (!s3 & !s2 & s1 & !s0) {
            if (opcode == LW) {
	
                // Alterando os bits de proximo estado para o estado 3
                ns0 = 1;
                ns1 = 1;
                ns2 = 0;
                ns3 = 0;

            } else if (opcode == SW) {
	
                // Alterando os bits de proximo estado para o estado 5
                ns0 = 1;
                ns1 = 0;
                ns2 = 1;
                ns3 = 0;

            }
        }

    // A MEF está no estado 3
    else
        if (!s3 & !s2 &  s1 &  s0) {
	
        // Alterando os bits de proximo estado para o estado 4
        ns0 = 0;
        ns1 = 0;
        ns2 = 1;
        ns3 = 0;

    }

    // a MEF está no estado 4
    else
        if (!s3 & s2 & !s1 & !s0) {

        // Alterando os bits de proximo estado para o estado 0
        ns0 = 0;
        ns1 = 0;
        ns2 = 0;
        ns3 = 0;

    }

    // A MEF está no estado 5
    else
        if (!s3 &  s2 & !s1 & s0) {
        // alterando os bits de proximo estado para o estado 0
        ns0 = 0;
        ns1 = 0;
        ns2 = 0;
        ns3 = 0;

    }

    // A MEF está no estado 6
    else
        if (!s3 &  s2 &  s1 & !s0) {
        // Alterando os bits de proximo estado para o estado 7
        ns0 = 1;
        ns1 = 1;
        ns2 = 1;
        ns3 = 0;

    }

    // A MEF está no estado 7
    else
        if (!s3 &  s2 &  s1 &  s0) {
        // Alterando os bits de proximo estado para o estado 0
        ns0 = 0;
        ns1 = 0;
        ns2 = 0;
        ns3 = 0;

    }

    }


    // Gerando o estado futuro de acordo com o estado atual.

    Next_State(ns0, ns1, ns2, ns3);
    
    // Gerando os sinais de controle para o estado atual

    return Signals_Generate(s0, s1, s2, s3);

}