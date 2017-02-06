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

// índice dos registradores

0		$zero	constante 0
1		$at		reservado para o assembly
2		$v0		resultado de função
3		$v1		resultado de função
4		$a0		argumento para função
5		$a1		argumento para função
6		$a2		argumento para função
7		$a3		argumento para função
8		$t0		temporário
9		$t1		temporário
10		$t2		temporário
11		$t3		temporário
12		$t4		temporário
13		$t5		temporário
14		$t6		temporário
15		$t7		temporário
16		$s0		temporário (salvo nas chamadas de função)
17		$s1		temporário (salvo nas chamadas de função)
18		$s2		temporário (salvo nas chamadas de função)
19		$s3		temporário (salvo nas chamadas de função)
20		$s4		temporário (salvo nas chamadas de função)
21		$s5		temporário (salvo nas chamadas de função)
22		$s6		temporário (salvo nas chamadas de função)
23		$s7		temporário (salvo nas chamadas de função)
24		$t8		temporário
25		$t9		temporário
26		$k0		reservado para o SO
27		$k1		reservado para o SO
28		$gp		apontador de área global
29		$sp		stack pointer
30		$fp		frame pointer
31		$ra		registrador de endereço de retorno

*/

// mascaras

// usada para separar a operacao da ULA. Atua em SC_ULA 2-0
#define SEPARA_CONTROLE_ULA 0x07  // 00000111		usada com &

// usadas para ativar e zerar a operacao da ULA. Atuam em SC_ULA 2-0
#define ATIVA_SOMA			0x02	// 00000010		usada com |
#define ATIVA_SUBTRACAO		0x06	// 00000110		usada com |
#define ATIVA_AND			0x00	// 00000000		usada com |
#define ATIVA_OR			0x01	// 00000001		usada com |
#define ATIVA_SLT			0x07	// 00000111		usada com |
#define ZERA_CONTROLE_ALU	0xf8	// 11111000		usada com &

// mascaras usadas para separar os campos dentro da instrucao
//										   31  26 25 21 20 16 15 11 10  6 5    0
#define SEPARA_COP			0xfc000000	// 111111 00000 00000 00000 00000 000000  usada com &
#define SEPARA_RS			0x03e00000	// 000000 11111 00000 00000 00000 000000  usada com &
#define SEPARA_RT			0x001f0000	// 000000 00000 11111 00000 00000 000000  usada com &
#define SEPARA_RD           0x0000f800	// 000000 00000 00000 11111 00000 000000  usada com &
#define SEPARA_SIGN_EXT		0x0000ffff  // 000000 00000 00000 11111 11111 111111  usada com &
#define SEPARA_CFUNCAO		0x0000003f	// 000000 00000 00000 00000 00000 111111  usada com &

// usada para eliminar os 2 bits mais significativos do Campo Funcao.
// Atua na variavel local dentro de UC_ULA
#define ZERA_2BITS_CFUNCAO	0x0f		// 00 00 1111   usada com &

// controle de sequenciamento explicito para controle microprogramado
#define SEQUENCIA   0x00030000
#define TAB_DESP1   0x00010000
#define TAB_DESP2   0x00020000
#define VOLTA_BUSCA 0x00000000

// separa o controle de sequenciamento
#define SEPARA_ADDRCTRL 0x00030000

// separa bits do estado
#define SEPARA_S0 0x01
#define SEPARA_S1 0x02
#define SEPARA_S2 0x04
#define SEPARA_S3 0x08

#define SEPARA_COP0 0x01
#define SEPARA_COP1 0x02
#define SEPARA_COP2 0x04
#define SEPARA_COP3 0x08
#define SEPARA_COP4 0x10
#define SEPARA_COP5 0x20

#define SEPARA_BITMENOS_SIG 0x01
