/******************************************************************************************************************************** 
 
* Arquivo : ula.c
* Esse arquivo contem as implementacoes para a unidade logica e aritmetica componente do processador MIPS multiciclo, proposto
* no trabalho 1, na disciplina de Programação Concorrente. Tambem aqui encontram-se as implementacoes para a Unidade de Controle
* da ULA.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
	Ubiratan Soares (5634292)
	Paulo César Jr.
	Paul Kibe
	Petrus Gomes		

*******************************************/

int ULA(int op1, int op2, char control)
{
	int result;

	control = control & mask_uc_control;

	if (control == 0)		// 000
		result = op1 & op2;

	if (control == 1)		// 001
		result = op1 | op2;

	if (control == 2)		// 010
		result = op1 + op2;

	if (control == 6)		// 110
		result = op1 - op2;
	
}

char UC_ULA(char ALUOp, char field){
	
	char code; // Codigo a ser enviado para a Unidade de Controle
	
	// Zera os 2 bits + significativos do campo de funcao
	field = field & zero_2bits;

	// Zera os 3 bits menos significativos de code
	code = code & mask_uc_control;

	if(ALUOp == 0) code = code | active_sum;

	if(ALUOp == 1) code = code | active_sub;

	if (ALUOp == 2) {
		
		if (field == 0)  // 0000
			code = code | active_sum;

		if (field == 2)  // 0010
			code = code | active_sub;

		if (field == 4)  // 0100
			code = code | active_and;

		if (field == 5)  // 0101
			code = code | active_or;

		if (field == 10)  // 1010
			code = code | active_slt;
	}

	return(code);
}