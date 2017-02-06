/******************************************************************************************************************************** 
 
* Arquivo : ula.c
* Esse arquivo contem as implementacoes para a unidade logica e aritmetica componente do processador MIPS multiciclo, proposto
* no trabalho 1, na disciplina de Programação Concorrente. Tambem aqui encontram-se as implementacoes para a Unidade de Controle
* da ULA.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
		Ubiratan Soares (5634292)
		Paulo César Jr. (5634890)
		Paul Kibe (5729158) 
		Petrus Gomes (5634966)			

*******************************************/

int ULA(void *thread_args)
{
	int result;
	ULA_args* args = (ULA_args*) thread_args;
	
	args->control = args->control & mask_uc_control;

	if (args->control == 0)		// 000
		result = args->op1 & args->op2;

	if (args->control == 1)		// 001
		result = args->op1 | args->op2;

	if (args->control == 2)		// 010
		result = args->op1 + args->op2;

	if (args->control == 6)		// 110
		result = args->op1 - args->op2;
	
}

char UC_ULA(void* thread_args){
	
	UC_ULA_args* args = (UC_ULA_args*) thread_args;
	
	char code; // Codigo a ser enviado para a Unidade de Controle
	
	// Zera os 2 bits + significativos do campo de funcao
	args->field = args->field & zero_2bits;

	// Zera os 3 bits menos significativos de code
	code = code & mask_uc_control;

	if(args->ALUOp == 0) code = code | active_sum;

	if(args->ALUOp == 1) code = code | active_sub;

	if (args->ALUOp == 2) {
		
		if (args->field == 0)  // 0000
			code = code | active_sum;

		if (args->field == 2)  // 0010
			code = code | active_sub;

		if (args->field == 4)  // 0100
			code = code | active_and;

		if (args->field == 5)  // 0101
			code = code | active_or;

		if (args->field == 10)  // 1010
			code = code | active_slt;
	}

	return(code);
}