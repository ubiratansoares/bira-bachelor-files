/******************************************************************************************************************************** 
 
* Arquivo : muxes.c
* Esse arquivo contem as implementacoes dos multiplexadores utilizados para o processador MIPS multiciclo, proposto
* no trabalho 1, na disciplina de Programação Concorrente. Tambem aqui encontra-se a implementacao da unidade signalextend.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
	Ubiratan Soares (5634292)
	Paulo Cesar Jr.
	Paul Kibe
	Petrus Gomes		

*******************************************/


int mux0(char control){
	
	if (control == 0)
		return(PC);

	if (control == 1)
		return(ALUOUT);

}



char mux2(char control){

	if (control == 0)
		return( (char) ((IR & mask_rt)>>16) );  // campo RT da Instrucao: IR[20-16]

	if (control == 1)
		return( (char) ((IR & mask_rd)>>11) );  // campo RD da instrucao: IR[15-11];

}

int mux1(char control){

	if (control == 0) return(ALUOUT);

	if (control == 1) return(MDR);

}

int mux3(char control){

	if (control == 0) return(PC);

	if (control == 1) return(A);

}

int mux4(char control){

	if (control == 0) return(B);

	if (control == 1) return((int)4);

	if (control == 2) return( signalextend( (int)(IR & mask_imediate) ) );

}


int signalextend(int value){
	
	return( (int) value);

}
