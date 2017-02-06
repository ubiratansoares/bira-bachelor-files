/******************************************************************************************************************************** 
 
* Arquivo : muxes.c
* Esse arquivo contem as implementacoes dos multiplexadores utilizados para o processador MIPS multiciclo, proposto
* no trabalho 1, na disciplina de Programação Concorrente. Tambem aqui encontra-se a implementacao da unidade signalextend.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
		Ubiratan Soares (5634292)
		Paulo César Jr. (5634890)
		Paul Kibe (5729158) 
		Petrus Gomes (5634966)	

*******************************************/


int mux0(void* thread_args){
	
	mux0_args* args = (mux0_args*) thread_args;
	
	if (args->control == 0)
		return(args->PC);

	if (args->control == 1)
		return(args->ALUOUT);

}



char mux2(void* thread_args){

	mux2_args* args = (mux2_args*) thread_args;

	if (args->control == 0)
		return( (char) ((args->IR & mask_rt)>>16) );  // campo RT da Instrucao: IR[20-16]

	if (args->control == 1)
		return( (char) ((args->IR & mask_rd)>>11) );  // campo RD da instrucao: IR[15-11];

}

int mux1(void* thread_args){

	mux1_args* args = (mux1_args*) thread_args;

	if (args->control == 0) return(args->ALUOUT);

	if (args->control == 1) return(args->MDR);

}

int mux3(void* thread_args){

	mux3_args* args = (mux3_args*) thread_args;
	
	if (args->control == 0) return(args->PC);

	if (args->control == 1) return(args->A);

}

int mux4(void* thread_args){

	mux4_args* args = (mux4_args*) thread_args;

	if (args->control == 0) return(args->B);

	if (args->control == 1) return((int)4);

	if (args->control == 2) return(signalextend( (int)(args->IR & mask_imediate) ) );

}


int signalextend(int value){
	
	return( (int) value);

}
