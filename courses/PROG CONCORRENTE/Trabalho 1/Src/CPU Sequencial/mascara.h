#define MAX 200
#define NUMREG 32

/*posicao dos bits nos sinais de controle da ULA
char C_ULA
0- controle ULA 0
1- controle ULA 1
2- controle ULA 2
3- controle ULA 3
4- nao e usado
5- nao e usado
6- nao e usado
7- nao e usado

posicao dos bits nos 16 sinais de controle determinados pela UC 
int S_C
0- RegDst	(RegDst)
1- EscReg	(RegWrite)
2- UALFonteA	(ALUSrcA)
3- UALFonteB0	(ALUSrcB0)
4- UALFonteB1	(ALUSrcB1)
5- UALOp0	(ALUOp0)
6- UALOp1	(ALUOp1)
7- FontePC0	(PCSource0)
8- FontePC1	(PCSource1)
9- PCEscCond	(PCWriteCond)
10-PCEsc	(PCWrite)
11-IouD	(IorD)
12-LerMem	(MemRead)
13-EscMem	(MemWrite)
14-MemParaReg	(MemtoReg)
15-IREsc	(IRWrite)
*/

// mascaras
// usadas para ativar e desativar o bit zero. Atuam em SC_ULA na pos. 3
#define ativa_bit_zero        0x08	// 00001000		usada com |
#define desativa_bit_zero     0xf7  // 11110111		usada com &

// usada para separar o bit zero de SC_ULA (posicao 3)
#define separa_bit_zero	0x08  // 00001000		usada com &

// usada para separar a operacao da ULA. Atua em SC_ULA 2-0
#define separa_controle_ula 0x07  // 00000111		usada com &

// usadas para ativar e zerar a operacao da ULA. Atuam em SC_ULA 2-0
#define ativa_soma		0x02	// 00000010		usada com |
#define ativa_subtracao	0x06	// 00000110		usada com |
#define ativa_and		0x00	// 00000000		usada com |
#define ativa_or		0x01	// 00000001		usada com |
#define ativa_slt		0x07	// 00000111		usada com |
#define zera_controle_ULA	0xf8	// 11111000		usada com &

// mascaras usadas para separar os campos dentro da instrucao
//									   					31  26 25 21 20 16 15 11 10  6 5    0
#define separa_cop		0xfc000000	// 111111 00000 00000 00000 00000 000000  usada com &
#define separa_rs		0x03e00000	// 000000 11111 00000 00000 00000 000000  usada com &
#define separa_rt		0x001f0000	// 000000 00000 11111 00000 00000 000000  usada com &
#define separa_rd           0x0000f800	// 000000 00000 00000 11111 00000 000000  usada com &
#define separa_cfuncao	0x0000003f	// 000000 00000 00000 00000 00000 111111  usada com &
#define separa_imediato     0x0000ffff	// 000000 00000 00000   1111111111111111  usada com &
#define separa_endereco_jump 0x03ffffff	// 000000     11111111111111111111111111  usada com &

// usada para eliminar os 2 bits mais significativos do Campo Funcao.
// Atua na variavel local dentro de UC_ULA
#define zera_2bits_cfuncao	0x0f		// 00 00 1111   usada com &

// usada para separar os 4 bits mais significativos de PC. Atua em PC.
#define separa_4bits_PC	0xf0000000	// 11110000000000000000000000000000  usada com &

// usadas para ativar os sinais de controle. Atuam em SC.
#define ativa_RegDst		  0x0001	// 0000 0000 0000 0001  usada com |
#define ativa_RegWrite	  0x0002      // 0000 0000 0000 0010  usada com |
#define ativa_ALUSrcA         0x0004      // 0000 0000 0000 0100  usada com |
#define ativa_ALUSrcB0        0x0008      // 0000 0000 0000 1000  usada com |
#define ativa_ALUSrcB1        0x0010      // 0000 0000 0001 0000  usada com |
#define ativa_ALUOp0          0x0020      // 0000 0000 0010 0000  usada com |
#define ativa_ALUOp1          0x0040      // 0000 0000 0100 0000  usada com |
#define ativa_PCSource0       0x0080      // 0000 0000 1000 0000  usada com |
#define ativa_PCSource1       0x0100      // 0000 0001 0000 0000  usada com |
#define ativa_PCWriteCond     0x0200      // 0000 0010 0000 0000  usada com |
#define ativa_PCWrite         0x0400      // 0000 0100 0000 0000  usada com |
#define ativa_IorD            0x0800      // 0000 1000 0000 0000  usada com |
#define ativa_MemRead         0x1000      // 0001 0000 0000 0000  usada com |
#define ativa_MemWrite        0x2000      // 0010 0000 0000 0000  usada com |
#define ativa_MemtoReg        0x4000      // 0100 0000 0000 0000  usada com |
#define ativa_IRWrite         0x8000      // 1000 0000 0000 0000  usada com |

// usadas para desativar os sinais de controle. Atuam em SC
#define desativa_RegDst	     0xfffe	   // 1111 1111 1111 1110  usada com &
#define desativa_RegWrite	     0xfffd      // 1111 1111 1111 1101  usada com &
#define desativa_ALUSrcA         0xfffb      // 1111 1111 1111 1011  usada com &
#define desativa_ALUSrcB0        0xfff7      // 1111 1111 1111 0111  usada com &
#define desativa_ALUSrcB1        0xffef      // 1111 1111 1110 1111  usada com &
#define desativa_ALUOp0          0xffdf      // 1111 1111 1101 1111  usada com &
#define desativa_ALUOp1          0xffbf      // 1111 1111 1011 1111  usada com &
#define desativa_PCSource0       0xff7f      // 1111 1111 0111 1111  usada com &
#define desativa_PCSource1       0xfeff      // 1111 1110 1111 1111  usada com &
#define desativa_PCWriteCond     0xfdff      // 1111 1101 1111 1111  usada com &
#define desativa_PCWrite         0xfbff      // 1111 1011 1111 1111  usada com &
#define desativa_IorD            0xf7ff      // 1111 0111 1111 1111  usada com &
#define desativa_MemRead         0xefff      // 1110 1111 1111 1111  usada com &
#define desativa_MemWrite        0xdfff      // 1101 1111 1111 1111  usada com &
#define desativa_MemtoReg        0xbfff      // 1011 1111 1111 1111  usada com &
#define desativa_IRWrite         0x7fff      // 0111 1111 1111 1111  usada com &

// usadas para separar os sinais de controle. Atuam em SC.
#define separa_RegDst	   0x0001	 // 0000 0000 0000 0001  usada com |
#define separa_RegWrite	   0x0002      // 0000 0000 0000 0010  usada com |
#define separa_ALUSrcA    	   0x0004      // 0000 0000 0000 0100  usada com |
#define separa_ALUSrcB0        0x0008      // 0000 0000 0000 1000  usada com |
#define separa_ALUSrcB1        0x0010      // 0000 0000 0001 0000  usada com |
#define separa_ALUOp0          0x0020      // 0000 0000 0010 0000  usada com |
#define separa_ALUOp1          0x0040      // 0000 0000 0100 0000  usada com |
#define separa_PCSource0       0x0080      // 0000 0000 1000 0000  usada com |
#define separa_PCSource1       0x0100      // 0000 0001 0000 0000  usada com |
#define separa_PCWriteCond     0x0200      // 0000 0010 0000 0000  usada com |
#define separa_PCWrite         0x0400      // 0000 0100 0000 0000  usada com |
#define separa_IorD            0x0800      // 0000 1000 0000 0000  usada com |
#define separa_MemRead         0x1000      // 0001 0000 0000 0000  usada com |
#define separa_MemWrite        0x2000      // 0010 0000 0000 0000  usada com |
#define separa_MemtoReg        0x4000      // 0100 0000 0000 0000  usada com |
#define separa_IRWrite         0x8000      // 1000 0000 0000 0000  usada com |

// controle de sequenciamento explicito para controle microprogramado
#define sequencia   0x00030000
#define tab_desp1   0x00010000
#define tab_desp2   0x00020000
#define volta_busca 0x00000000

// separa o controle de sequenciamento
#define separa_AddrCtrl 0x00030000

// separa bits do estado
#define separa_S0 0x01
#define separa_S1 0x02
#define separa_S2 0x04
#define separa_S3 0x08

#define separa_COp0 0x01
#define separa_COp1 0x02
#define separa_COp2 0x04
#define separa_COp3 0x08
#define separa_COp4 0x10
#define separa_COp5 0x20

#define separa_bitmenos_sig 0x01
