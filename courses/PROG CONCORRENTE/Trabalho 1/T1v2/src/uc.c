/******************************************************************************************************************************** 
 
* Arquivo : uc.c
* Esse arquivo contém a implementação para a Unidade de Controle da CPU MIPS proposta
* no trabalho 1, na disciplina de Programação Concorrente.
* Data : 21/05/2010
* Professor : Paulo SL Souza
* Autores : 
		Ubiratan Soares (5634292)
		Paulo César Jr. (5634890)
		Paul Kibe (5729158) 
		Petrus Gomes (5634966)	


	
********************************************************************************************************************************/




void Next_State(char ns0, char ns1, char ns2) {
    state = ns0;
    state = state + (ns1 << 1);
    state = state + (ns2 << 2);
}


int Signals_Generate(char s0, char s1, char s2) {

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

	RegDst =       ( s2 &  s1 &  s0) || ( s2 &  s1 &  s0) || ( s2 &  s1 & !s0);
	RegWrite =     ( s2 & !s1 & !s0) || ( s2 & !s1 & !s0) || ( s2 &  s1 &  s0) || (!s2 &  s1 &  s0);
	ALUSrcA =      (!s2 &  s1 & !s0) || (!s2 &  s1 & !s0) || ( s2 &  s1 & !s0) || (!s2 & !s1 & !s0) || (s2 & !s1 & !s0) || (s2 &  s1 &  s0);
	ALUSrcB0 =     (!s2 & !s1 & !s0) || (!s2 & !s1 &  s0);
	ALUSrcB1 =     (!s2 & !s1 &  s0) || (!s2 &  s1 & !s0) || ( s2 &  s1 &  s0);
	ALUOp0 =       (!s2 & !s1 & !s0) || ( s2 & !s1 & !s0) || ( s2 &  s1 &  s0);
	ALUOp1 =       ( s2 &  s1 & !s0) || ( s2 &  s1 &  s0);
	PCWrite =      (!s2 & !s1 & !s0) || (!s2 & !s1 &  s0) || (!s2 &  s1 & !s0) || (s2 & !s1 &  s0);
	IorD =         (!s2 &  s1 &  s0) || ( s2 & !s1 &  s0);
	MemRead =      (!s2 & !s1 & !s0) || (!s2 &  s1 &  s0);
	MemWrite =     ( s2 & !s1 &  s0);
	MemtoReg =     ( s2 & !s1 & !s0);
	IRWrite =      (!s2 & !s1 & !s0);


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


int UnidadeControle(int opcode) {
	
	// Variaveis para bits de representacao do estado atual
	
    char s2 = 0, s1 = 0, s0 = 0;     

	// Variaveis para bits de representacao do proximo estado

    char ns2 = 0, ns1 = 0, ns0 = 0; 

    // Inicialmente, isolam-se os bits de cada posicao da variavel "estado"

    s0 = (state & 1);      // 0001
    s1 = (state & 2) >> 1; // 0010
    s2 = (state & 4) >> 2; // 0100


    // Implementacao para a Maquina de Estados Finitos (MEF), de acordo com a imagem enviada juntamente com esse codigo.

    // A MEF esta no estado 0

    if (!s2 & !s1 & !s0) {

        // Alterando os bits de proximo estado para o estado 1
        ns0 = 1;
        ns1 = 0;
        ns2 = 0;
        
    }

    // A MEF está no estado 1
    else if (!s2 & !s1 & s0) {
            if (opcode == LW || opcode == SW) {
        
        		// Alterando os bits de proximo estado para o estado 2
                ns0 = 0;
                ns1 = 1;
                ns2 = 0;
             
            } else if (opcode == RTYPE) {
                
				// Alterando os bits de proximo estado para o estado 6
                ns0 = 0;
                ns1 = 1;
                ns2 = 1;
             }
    
    // A MEF está no estado 2
    else
        if (!s2 & s1 & !s0) {
            if (opcode == LW) {
	
                // Alterando os bits de proximo estado para o estado 3
                ns0 = 1;
                ns1 = 1;
                ns2 = 0;
             
            } else if (opcode == SW) {
	
                // Alterando os bits de proximo estado para o estado 5
                ns0 = 1;
                ns1 = 0;
                ns2 = 1;
             
            }
        }

    // A MEF está no estado 3
    else
        if (!s2 &  s1 &  s0) {
	
        // Alterando os bits de proximo estado para o estado 4
        ns0 = 0;
        ns1 = 0;
        ns2 = 1;
    }

    // a MEF está no estado 4
    else
        if (s2 & !s1 & !s0) {

        // Alterando os bits de proximo estado para o estado 0
        ns0 = 0;
        ns1 = 0;
        ns2 = 0;
    }

    // A MEF está no estado 5
    else
        if (s2 & !s1 & s0) {
        // alterando os bits de proximo estado para o estado 0
        ns0 = 0;
        ns1 = 0;
        ns2 = 0;
    }

    // A MEF está no estado 6
    else
        if (s2 &  s1 & !s0) {
        // Alterando os bits de proximo estado para o estado 7
        ns0 = 1;
        ns1 = 1;
        ns2 = 1;
    }

    // A MEF está no estado 7
    else
        if (s2 &  s1 &  s0) {
        // Alterando os bits de proximo estado para o estado 0
        ns0 = 0;
        ns1 = 0;
        ns2 = 0;
    	}

    }


    // Gerando o estado futuro de acordo com o estado atual.

    Next_State(ns0, ns1, ns2);
    
    // Gerando os sinais de controle para o estado atual

    return Signals_Generate(s0, s1, s2);

}
