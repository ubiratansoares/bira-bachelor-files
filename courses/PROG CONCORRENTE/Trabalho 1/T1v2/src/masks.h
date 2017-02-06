/******************************************************************************************************************************** 
 
* Arquivo : masks.h
* Esse arquivo contem as definicoes de mascaras utilizadas para os tratamentos de sinais e outros referentes ao processador
* MIPS multiciclo, proposto no trabalho 1, disciplina de Programacao Concorrente
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
	Ubiratan Soares (5634292)
	Paulo César Jr. (5634890)
	Paul Kibe (5729158) 
	Petrus Gomes (5634966)		

*******************************************/

#define mask_uc_control 0x07	// 00000111
#define zero_uc_control 0x08	// 00001000

// Mascaras de acordo com as operacoes executaveis pela ULA

#define active_sum		0x02	// 00000010		
#define active_sub		0x06	// 00000110		
#define active_and		0x00	// 00000000		
#define active_or		0x01	// 00000001	
#define active_slt		0x07	// 00000111

#define mask_rs		0x03e00000	// 000000 11111 00000 00000 00000 000000  
#define mask_rt		0x001f0000	// 000000 00000 11111 00000 00000 000000 
#define mask_rd     0x0000f800	// 000000 00000 00000 11111 00000 000000
#define mask_imediate     0x0000ffff	// 000000 00000 00000   1111111111111111

// Mascara para eliminar os 2 bits mais significativos do Campo Funcao.

#define zero_2bits		0x0f	// 00 00 1111