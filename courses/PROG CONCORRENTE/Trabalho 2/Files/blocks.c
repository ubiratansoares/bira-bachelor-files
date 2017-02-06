/******************************************************************************************************************************

* Arquivo : blocos.c

* Data : 18/06/2010
* Professor : Paulo SL Souza
* Autores :
		Ubiratan Soares (5634292)
		Paulo Cesar Jr. (5634890)
		Paul Kibe (5729158)
		Petrus Gomes (5634966)

******************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "control_unit.h"
#include "blocks.h"
#include "masks.h"

int tag = 1;

void Clock (){
	//int i = 1;

	while(1){
		// fflush(NULL);
		//getchar();
		// fflush(NULL);

		MPI_Barrier(MPI_COMM_WORLD);
		// printf("\n\t PROCESS = CLOCK ; clock = %d \n\n", i++);
		// fflush(NULL);
	}
}

// funcao executada pelo processo PR_CU
void Cu(){

	unsigned char opcode;

	int ciclo = 1;

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\n\t PROCESS = CU ; CICLO = %d\n", ciclo);
		// fflush(NULL);

		opcode = ControlUnit(ciclo);

		if (((opcode == LW) && ciclo == 5) || ((opcode == RTYPE || opcode == SW)  && ciclo == 4)){
			ciclo = 1;
			// printf("\n\n =============================================\n REINICIANDO CICLO = %d", ciclo);
		}

		++ciclo;

	}
}

// funcao executada pelo processo PR_PC
void Pc(){

	char PC_Write;	// sinal de controle
	unsigned int reg_pc = 0;
	unsigned int alu;

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\n\t PROCESS = PC\n");
		// fflush(NULL);


		MPI_Recv(&PC_Write, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);	// recebe sinal de controle

		// printf("\t\tPROCESS = PC ; Recebido de CU (PC_Write = %d)\n",PC_Write);
		// fflush(NULL);

		MPI_Send(&reg_pc, 1, MPI_INT, PR_MUX_MEM, tag, MPI_COMM_WORLD); 		// envia pro Mux_Mem

		// printf("\t\tPROCESS = PC ; Enviado para MUX_MEM (reg_pc = %d)\n",reg_pc);
		// fflush(NULL);

		MPI_Send(&reg_pc, 1, MPI_INT, PR_MUX_ALU_A, tag, MPI_COMM_WORLD); 		// envia pro Mux_AluA

		// printf("\t\tPROCESS = PC ; Enviado para MUX_ALU_A (reg_pc = %d)\n",reg_pc);
		// fflush(NULL);


		MPI_Recv(&alu, 1, MPI_INT, PR_ALU, tag, MPI_COMM_WORLD, &status); 		// recebe da Alu

		// printf("\t\tPROCESS = PC ; Recebido de ALU (alu = %d)\n",alu);
		// fflush(NULL);


		if(PC_Write){
			reg_pc = alu; // PC recebe o valor vindo da Alu

			// printf("\t\tPROCESS = PC ; Atualizado PC = %d\n",reg_pc);
			// fflush(NULL);

		}
	}
}


// funcao executada pelo processo PR_MUX_MEM
void Mux_Mem(){

	char IorD;		// sinal de controle

	unsigned int pc;
	unsigned int alu_out;
	unsigned int saida_mux;

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\n\t PROCESS = MUX_MEM\n");
		// fflush(NULL);

		MPI_Recv(&IorD, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);			// recebe sinal de controle

		// printf("\t\tPROCESS = MUX_MEM ; Recebido de CU (IorD = %d)\n",IorD);
		// fflush(NULL);

		MPI_Recv(&pc, 1, MPI_INT, PR_PC, tag, MPI_COMM_WORLD, &status);				// recebe do PC

		// printf("\t\tPROCESS = MUX_MEM; Recebido de PC (pc = %d)\n",pc);
		// fflush(NULL);

		MPI_Recv(&alu_out, 1, MPI_INT, PR_ALU_OUT, tag, MPI_COMM_WORLD, &status);	// recebe do AluOut

		// printf("\t\tPROCESS = MUX_MEM; Recebido de ALU_OUT (alu_out = %d)\n",alu_out);
		// fflush(NULL);


		if(IorD)
			saida_mux = alu_out;
		else
			saida_mux = pc;

		// printf("\t\tPROCESS = MUX_MEM; Processado Saida = %d\n",saida_mux);
		// fflush(NULL);


		MPI_Send(&saida_mux, 1, MPI_INT, PR_MEMORY, tag, MPI_COMM_WORLD);			//envia pra Memory

		// printf("\t\tPROCESS = MUX_MEM; Enviado para MEMORY (saida_mux = %d)\n",saida_mux);
		// fflush(NULL);



	}
}


// funcao executada pelo processo PR_MEMORY
void Memory(){

	char MemRead; 		// sinal de controle
	char MemWrite; 		// sinal de controle

	unsigned int address = 9999999;		// endereco recebido do Mux_Mem
	int data_write = 9999999;			// dado recebido do Reg_B (a ser armazenado na Memory em um SW)
	int mem_data = 9999999;				// dado da Memory a ser enviado pro IR e pro Mdr

	int MEMORY[MAX_MEM];


	// Padrao de uma instrucao MIPS
	// opcode rs    rt    rd    shamt func_f
	// 31  26 25 21 20 16 15 11 10  6 5    0
	// 000000 01010 01001 01010 00000 001000

	// oper rd, rs, rt

	//MEMORY[0] = 0x01084020;  	// add $t0, $t0, $t0    000000 01000 01000 01000 00000 100000
	//MEMORY[1] = 0x1284020;	// add $t0, $t1, $t0    000000 01001 01000 01000 00000 100000

    MEMORY[0] = 0x14B482A;		// slt $t1, $t2, $t3	000000 01010 01011 01001 00000 101010
	//MEMORY[0] = 0x8C0A0014;		// lw $t2, 20($zero)    100011 00000 01010 00000 00000 010100
	//MEMORY[1] = 0x14B4804;		// and $t1, $t2, $t3	000000 01010 01011 01001 00000 000100
	//MEMORY[2] = 0xAC090014;		// sw $t2, 20($zero)    101011 00000 01001 00000 00000 010100
	//MEMORY[1] = 0x01084020;  	// add $t0, $t0, $t0    000000 01000 01000 01000 00000 100000
	//MEMORY[2] = 0x1084022;  	// sub $t0, $t0, $t0    000000 01000 01000 01000 00000 100010
	//MEMORY[3] = 0x14B4805;	// or $t1, $t2, $t3		000000 01010 01011 01001 00000 000101
	MEMORY[1] = -1;

	MEMORY[20] = 7;

	while (1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\n\t PROCESS = MEMORY\n");
		// // fflush(NULL);

		MPI_Recv(&MemRead, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);		// recebe sinal de controle

		// printf("\t\tPROCESS = MEMORY; Recebido de CU (MemRead = %d)\n",MemRead);
		// fflush(NULL);

		MPI_Recv(&MemWrite, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);		// recebe sinal de controle

		// printf("\t\tPROCESS = MEMORY; Recebido de CU (mem_write = %d)\n",MemWrite);
		// fflush(NULL);


		MPI_Recv(&address, 1, MPI_INT, PR_MUX_MEM, tag, MPI_COMM_WORLD, &status);	// recebe do Mux_Mem

		// printf("\t\tPROCESS = MEMORY; Recebido de MUX_MEM (address = %d)\n",address);
		// fflush(NULL);


		MPI_Recv(&data_write, 1, MPI_INT, PR_REG_B, tag, MPI_COMM_WORLD, &status);	// recebe do Reg_B

		// printf("\t\tPROCESS = MEMORY; Recebido de REG_B (data_write = %d)\n",data_write);
		// fflush(NULL);



		if(MemRead){
			mem_data = MEMORY[address];

			// printf("\t\tPROCESS = MEMORY; Processado leitura mem_data = %x\n",mem_data);
			// fflush(NULL);


		}else if(MemWrite){

			MEMORY[address] = data_write;
			// printf("\n* MEMORY[%d] ATUALIZADO = %d\n", address, MEMORY[address]);
			// fflush(NULL);

			// printf("\t\tPROCESS = MEMORY; Processado escrita data_write = %d\n",data_write);
			// fflush(NULL);


		}

		MPI_Send(&mem_data, 1, MPI_INT, PR_IR, tag, MPI_COMM_WORLD);			// envia pro IR

		// printf("\t\tPROCESS = MEMORY ; Enviado para IR (mem_data = %x)\n",mem_data);
		// fflush(NULL);

		MPI_Send(&mem_data, 1, MPI_INT, PR_MDR, tag, MPI_COMM_WORLD);			// envia pro Mdr

		// printf("\t\tPROCESS = MEMORY ; Enviado para MDR (mem_data = %x)\n",mem_data);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_IR
void Ir(){

	int error = 0;

	char IRWrite;			// sinal de controle

	unsigned int mem_inst = 9999999;		// instrucao recebida da memória
	unsigned int reg_ir = 0;
	unsigned int aux_mask;					// auxiliar para utilizar máscaras

	unsigned char	opcode;					// código da operacao a ser executada
	unsigned char	reg_1;					// número do registrador a ser lido em Registers
	unsigned char	reg_2;					// número do registrador a ser lido (e escrito) em Registers
	unsigned char	reg_wr;					// número do registrador a ser escrito em Registers
	unsigned short	sign_ext_out;			// saída para Sign_ext
	unsigned char	func_field;				// saída para Alu_Ctrl

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = IR");
		// fflush(NULL);

		MPI_Recv(&IRWrite, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);		// recebe sinal de controle

		// printf("\t\t\nPROCESS = IR ; Recebido de CU (IRWrite = %d)\n",IRWrite);
		// fflush(NULL);

		MPI_Recv(&mem_inst, 1, MPI_INT, PR_MEMORY, tag, MPI_COMM_WORLD, &status);	// recebe da Memory

		// printf("\t\tPROCESS = IR ; Recebido de MEMORY (mem_inst = %x)\n",mem_inst);
		// fflush(NULL);

		if(IRWrite){
			reg_ir = mem_inst;

			if(reg_ir == -1){
				// printf("!!!!!!!!!!!!!!SAINDO DO PROGRAMA!!!!!!!!!!!!!!!!\n\n");
				// fflush(NULL);
				MPI_Abort(MPI_COMM_WORLD, error);
			}
		}

		aux_mask = reg_ir & SEPARA_COP;							// aplica máscara em reg_ir para obter opcode
		aux_mask = aux_mask >> 26;								// shiftando para o byte menos significativo
		opcode = aux_mask;

		MPI_Send(&opcode, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD); // envia 1 int [31-26] para CU

		// printf("\t\tPROCESS = IR ; Enviado para CU (opcode = %u)\n",opcode);
		// fflush(NULL);



		aux_mask = reg_ir & SEPARA_RS;							// aplica máscara em reg_ir para obter o índice de reg_1
		aux_mask = aux_mask >> 21;								// shiftando para o byte menos significativo
		reg_1 = aux_mask;

		MPI_Send(&reg_1, 1, MPI_CHAR, PR_REGISTERS, tag, MPI_COMM_WORLD); // envia 1 char [25-21] para Registers

		// printf("\t\tPROCESS = IR ; Enviado para REGISTERS (reg25_21 = %u)\n",reg_1);
		// fflush(NULL);



		aux_mask = reg_ir & SEPARA_RT;								// aplica máscara em reg_ir para obter o índice de reg_2
		aux_mask = aux_mask >> 16;								// shiftando para o byte menos significativo
		reg_2 = aux_mask;

		MPI_Send(&reg_2, 1, MPI_CHAR, PR_REGISTERS, tag, MPI_COMM_WORLD); // envia 1 char [20-16] para Registers

		// printf("\t\tPROCESS = IR ; Enviado para REGISTERS (reg20_16 = %u)\n",reg_2);
		// fflush(NULL);

		MPI_Send(&reg_2, 1, MPI_CHAR, PR_MUX_WR, tag, MPI_COMM_WORLD); // envia 1 char [20-16] para Mux_Wr

		// printf("\t\tPROCESS = IR ; Enviado para MUX_WR (reg20_16 = %u)\n",reg_2);
		// fflush(NULL);



		aux_mask = reg_ir & SEPARA_RD;							// aplica máscara em reg_ir para obter o índice de reg_wr
		aux_mask = aux_mask >> 11;								// shiftando para o byte menos significativo
		reg_wr = aux_mask;

		MPI_Send(&reg_wr, 1, MPI_CHAR, PR_MUX_WR, tag, MPI_COMM_WORLD); // envia 1 char [15-11] para Mux_Wr

		// printf("\t\tPROCESS = IR ; Enviado para MUX_WR (reg15_11 = %u)\n",reg_wr);
		// fflush(NULL);

		aux_mask = reg_ir & SEPARA_SIGN_EXT;				// aplica máscara em reg_ir para obter a saída para Sig_ext
		sign_ext_out = aux_mask;

		MPI_Send(&sign_ext_out, 1, MPI_SHORT, PR_SIGN_EXT, tag, MPI_COMM_WORLD); // envia 1 short [15-0] para SignExt

		// printf("\t\tPROCESS = IR ; Enviado para SIGN_EXT (sig_ext_out = %u)\n",sign_ext_out);
		// fflush(NULL);



		aux_mask = reg_ir & SEPARA_CFUNCAO;					// aplica máscara em reg_ir para obter o campo de funcao
		func_field = aux_mask;

		MPI_Send(&func_field, 1, MPI_CHAR, PR_ALU_CTRL, tag, MPI_COMM_WORLD); // envia 1 char [5-0] para ALU_Ctrl

		// printf("\t\tPROCESS = IR ; Enviado para ALU_CTRL (func_field = %u)\n",func_field);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_MDR
void Mdr(){

	int reg_mdr = 0x9999999;

	while(1){
		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = MDR\n");
		// fflush(NULL);

		MPI_Send(&reg_mdr, 1, MPI_INT, PR_MUX_WD, tag, MPI_COMM_WORLD);				// envia pro Mux_Wd

		// printf("\t\tPROCESS = MDR ; Enviado para MUX_WD (reg_mdr = %x)\n",reg_mdr);
		// fflush(NULL);

		MPI_Recv(&reg_mdr, 1, MPI_INT, PR_MEMORY, tag, MPI_COMM_WORLD, &status);	// recebe da Memory

		// printf("\t\tPROCESS = MDR ; Recebido de MEMORY (reg_mdr = %x)\n",reg_mdr);
		// fflush(NULL);
	}
}


// funcao executada pelo processo PR_MUX_WR
void Mux_Wr(){
	char RegDst;	// sinal de controle

	unsigned char reg_0 = 99;
	unsigned char reg_1 = 99;
	unsigned char saida_mux = 99;

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = MUW_WR\n");
		// fflush(NULL);

		MPI_Recv(&RegDst, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);	// recebe sinal de controle

		// printf("\t\tPROCESS = MUX_WR; Recebido de CU (RegDst = %d)\n",RegDst);
		// fflush(NULL);

		MPI_Recv(&reg_0, 1, MPI_CHAR, PR_IR, tag, MPI_COMM_WORLD, &status); 	// recebe 1 char [20-16] de IR

		// printf("\t\tPROCESS = MUX_WR ; Recebido de IR (reg20_16 = %u)\n",reg_0);
		// fflush(NULL);


		MPI_Recv(&reg_1, 1, MPI_CHAR, PR_IR, tag, MPI_COMM_WORLD, &status); 	// recebe 1 char [15-11] de IR

		// printf("\t\tPROCESS = MUX_WR ; Recebido de IR (reg15_11 = %u)\n",reg_1);
		// fflush(NULL);


		if (RegDst)
			saida_mux = reg_1;
		else
			saida_mux = reg_0;

		MPI_Send(&saida_mux, 1, MPI_CHAR, PR_REGISTERS, tag, MPI_COMM_WORLD); 	// envia 1 char pro Registers

		// printf("\t\tPROCESS = MUX_WR ; Enviado para REGISTERS (saida_mux = %d)\n",saida_mux);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_MUX_WD
void Mux_Wd(){

	char MemtoReg;	// sinal de controle

	int alu_out = 9999999;
	int reg_mdr = 9999999;
	int saida_mux = 9999999;

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = MUX_WD\n");
		// fflush(NULL);


		MPI_Recv(&MemtoReg, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);		// recebe sinal de controle

		// printf("\t\tPROCESS = MUX_WD ; Recebido de CU (MemtoReg = %d)\n",MemtoReg);
		// fflush(NULL);

		MPI_Recv(&alu_out, 1, MPI_INT, PR_ALU_OUT, tag, MPI_COMM_WORLD, &status);	// recebe do AluOut

		// printf("\t\tPROCESS = MUX_WD ; Recebido de ALU_OUT (alu_out = %d)\n",alu_out);
		// fflush(NULL);


		MPI_Recv(&reg_mdr, 1, MPI_INT, PR_MDR, tag, MPI_COMM_WORLD, &status);		// recebe do Mdr

		// printf("\t\tPROCESS = MUX_WD ; Recebido de MDR (reg_mdr = %d)\n",reg_mdr);
		// fflush(NULL);


		if (MemtoReg)
			saida_mux = reg_mdr;
		else
			saida_mux = alu_out;

		MPI_Send(&saida_mux, 1, MPI_INT, PR_REGISTERS, tag, MPI_COMM_WORLD); 		// envia pro Registers

		// printf("\t\tPROCESS = MUX_WD ; Enviado para REGISTERS (saida_mux = %d)\n",saida_mux);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_REGISTERS
void Registers(){

	char RegWrite;		// sinal de controle

	int  reg[32];		// banco de registradores
	unsigned char ind_reg_1;		// índice do registrador a ser lido
	unsigned char ind_reg_2;		// índice do registrador a ser lido

	unsigned char write_reg;		// registrador a ser escrito
	int  write_data;	// dado a ser escrito no registrador write_reg

	int read_data_1 = 9999999;	// dado a ser enviado pro Reg_A
	int read_data_2 = 9999999;	// dado a ser enviado pro Reg_B

    reg[0] = 0;
	reg[8] = 15; // 1111		$t0
	reg[9] = 10; // 1010		$t1

	reg[10] = 30;  // 0011		$t2
	reg[11] = 10; // 0010		$t3

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = REGISTERS\n");
		// fflush(NULL);

		MPI_Recv(&RegWrite, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);		//recebe sinal de controle

		// printf("\t\tPROCESS = REGISTERS ; Recebido de CU (RegWrite = %d)\n",RegWrite);
		// fflush(NULL);


		MPI_Recv(&ind_reg_1, 1, MPI_CHAR, PR_IR, tag, MPI_COMM_WORLD, &status); 	// recebe 1 char [25-21] do IR

		// printf("\t\tPROCESS = REGISTERS ; Recebido de IR (ind_reg25_21 = %u)\n",ind_reg_1);
		// fflush(NULL);

		MPI_Recv(&ind_reg_2, 1, MPI_CHAR, PR_IR, tag, MPI_COMM_WORLD, &status); 	// recebe 1 char [20-16] do IR

		// printf("\t\tPROCESS = REGISTERS ; Recebido de IR (ind_reg20_16 = %u)\n",ind_reg_2);
		// fflush(NULL);

		MPI_Recv(&write_reg, 1, MPI_CHAR, PR_MUX_WR, tag, MPI_COMM_WORLD, &status); 	// recebe 1 char do Mux_Wr

		// printf("\t\tPROCESS = REGISTERS ; Recebido de MUX_WR (write_reg = %d)\n",write_reg);
		// fflush(NULL);


		MPI_Recv(&write_data, 1, MPI_INT, PR_MUX_WD, tag, MPI_COMM_WORLD, &status); 	// recebe 1 int do Mux_Wd

		// printf("\t\tPROCESS = REGISTERS ; Recebido de MUX_WD (write_data = %d)\n",write_data);
		// fflush(NULL);

		read_data_1 = reg[ind_reg_1];
		read_data_2 = reg[ind_reg_2];

		MPI_Send(&read_data_1, 1, MPI_INT, PR_REG_A, tag, MPI_COMM_WORLD); // envia pro Reg_A

		// printf("\t\tPROCESS = REGISTERS ; Enviado para REG_A (read_data_1 = %d)\n",read_data_1);
		// fflush(NULL);


		MPI_Send(&read_data_2, 1, MPI_INT, PR_REG_B, tag, MPI_COMM_WORLD); // envia pro Reg_B

		// printf("\t\tPROCESS = REGISTERES ; Enviado para REG_B (read_data_2 = %d)\n",read_data_2);
		// fflush(NULL);


		if(RegWrite){
			reg[write_reg] = write_data;

			// printf("\n* REGISTRADOR reg[%d] ATUALIZADO = %d\n\n",write_reg,reg[write_reg]);
			// fflush(NULL);
		}
	}
}


// funcao executada pelo processo PR_SIGN_EXT
void Sign_Ext(){

	unsigned short ir_data;	// dado recebido de IR
	unsigned int   sign_ext = 9999999;

	while(1){
		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = SIGN_EXT\n");
		// fflush(NULL);


		MPI_Recv(&ir_data, 1, MPI_SHORT, PR_IR, tag, MPI_COMM_WORLD, &status);	// recebe 2 char [15-0] do IR

		// printf("\t\tPROCESS = SIGN_EXT ; Recebido de IR (ir15_0 = %u)\n",ir_data);
		// fflush(NULL);

		sign_ext = (int) ir_data;	// executa a extensao de sinal

		MPI_Send(&sign_ext, 1, MPI_INT, PR_MUX_ALU_B, tag, MPI_COMM_WORLD);		// envia 1 int pro Mux_Alu_B

		// printf("\t\tPROCESS = SIGN_EXT ; Enviado para MUX_ALU_B (sign_ext = %d)\n",sign_ext);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_REG_A
void Reg_A(){
	int reg_a = 9999999;

	while(1){
		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = REG_A\n");
		// fflush(NULL);

		MPI_Send(&reg_a, 1, MPI_INT, PR_MUX_ALU_A, tag, MPI_COMM_WORLD); // envia pro Mux_Alu_A

		// printf("\t\tPROCESS = REG_A ; Enviado para MUX_ALU_A (reg_a = %d)\n",reg_a);
		// fflush(NULL);

		MPI_Recv(&reg_a, 1, MPI_INT, PR_REGISTERS, tag, MPI_COMM_WORLD, &status); // recebe do Registers

		// printf("\t\tPROCESS = REG_A ; Recebido de REGISTERS (reg_a = %d)\n",reg_a);
		// fflush(NULL);


	}
}


// funcao executada pelo processo PR_REG_B
void Reg_B(){
	int reg_b = 9999999;

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = REG_B\n");
		// fflush(NULL);

		MPI_Send(&reg_b, 1, MPI_INT, PR_MEMORY, tag, MPI_COMM_WORLD); // envia pra Memory

		// printf("\t\tPROCESS = REG_B ; Enviado para MEMORY (reg_b = %d)\n",reg_b);
		// fflush(NULL);


		MPI_Send(&reg_b, 1, MPI_INT, PR_MUX_ALU_B, tag, MPI_COMM_WORLD); // envia pro Mux_Alu_B

		// printf("\t\tPROCESS = REG_B ; Enviado para MUX_ALU_B (reg_b = %d)\n",reg_b);
		// fflush(NULL);


		MPI_Recv(&reg_b, 1, MPI_INT, PR_REGISTERS, tag, MPI_COMM_WORLD, &status); // recebe do Registers

		// printf("\t\tPROCESS = REG_B ; Recebido de REGISTERS (reg_b = %d)\n",reg_b);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_MUX_ALU_A
void Mux_Alu_A(){

	char AluSrcA;	// sinal de controle

	unsigned int pc = 9999999;
	int reg_a = 9999999;
	int saida_mux = 9999999;

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = MUX_ALU_A\n");
		// fflush(NULL);


		MPI_Recv(&AluSrcA, 1, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);	// recebe sinal de controle

		// printf("\t\tPROCESS = MUX_ALU_A ; Recebido de CU (AluSrcA = %d)\n",AluSrcA);
		// fflush(NULL);


		MPI_Recv(&pc, 1, MPI_INT, PR_PC, tag, MPI_COMM_WORLD, &status);	// recebe do PC

		// printf("\t\tPROCESS = MUX_ALU_A ; Recebido de PC (pc = %d)\n",pc);
		// fflush(NULL);

		MPI_Recv(&reg_a, 1, MPI_INT, PR_REG_A, tag, MPI_COMM_WORLD, &status);	// recebe do reg_A

		// printf("\t\tPROCESS = MUX_ALU_A ; Recebido de REG_A (reg_a = %d)\n",reg_a);
		// fflush(NULL);
		if (AluSrcA)
			saida_mux = reg_a;
		else
			saida_mux = pc;

		MPI_Send(&saida_mux, 1, MPI_INT, PR_ALU, tag, MPI_COMM_WORLD); // envia pra Alu

		// printf("\t\tPROCESS = MUX_ALU_A ; Enviado para ALU (saida_mux = %d)\n",saida_mux);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_MUX_ALU_B
void Mux_Alu_B(){

	char AluSrcB[2];	// sinal de controle

	int reg_b = 9999999;
	unsigned int sign_ext = 9999999;
	int saida_mux = 9999999;

	while(1){

		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = MUX_ALU_B\n");
		// fflush(NULL);

		MPI_Recv(AluSrcB, 2, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);		// recebe sinal de controle

		// printf("\t\tPROCESS = MUX_ALU_B ; Recebido de CU (AluScrB = %d%d)\n",AluSrcB[1], AluSrcB[0]);
		// fflush(NULL);

		MPI_Recv(&reg_b, 1, MPI_INT, PR_REG_B, tag, MPI_COMM_WORLD, &status);		// recebe do reg_B

		// printf("\t\tPROCESS = MUX_ALU_B ; Recebido de REG_B (reg_b = %d)\n",reg_b);
		// fflush(NULL);

		MPI_Recv(&sign_ext, 1, MPI_INT, PR_SIGN_EXT, tag, MPI_COMM_WORLD, &status); // recebe do Sign_Ext

		// printf("\t\tPROCESS = MUX_ALU_B ; Recebido de SIGN_EXT (sign_ext = %d)\n",sign_ext);
		// fflush(NULL);


		if ((AluSrcB[1] == 0) && (AluSrcB[0] == 0))
			saida_mux = reg_b;

		else if ((AluSrcB[1] == 0) && (AluSrcB[0] == 1))
			saida_mux = 1;															// usado para executar PC = PC + 4

			else // (AluSrcB[1] == 1) && (AluSrcB[0] == 0)
				saida_mux = sign_ext;

		MPI_Send(&saida_mux, 1, MPI_INT, PR_ALU, tag, MPI_COMM_WORLD); // envia pra Alu

		// printf("\t\tPROCESS = MUX_ALU_B ; Enviado para ALU (saida_mux = %d)\n",saida_mux);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_ALU
void Alu(){
	char op;	// sinal de controle (vindo da Alu_Ctrl)

	int entrada_Alu_A = 9999999;
	int entrada_Alu_B = 9999999;

	int saida_Alu = 9999999;


	while(1){
		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock


		// printf("\n\t PROCESS = ALU\n");
		// fflush(NULL);


		MPI_Recv(&op, 1, MPI_CHAR, PR_ALU_CTRL, tag, MPI_COMM_WORLD, &status);	// recebe sinal de controle da Alu_Ctrl (op a executar)

		// printf("\t\tPROCESS = ALU ; Recebido de ALU_CTRL (operacao = %d)\n",op);
		// fflush(NULL);

		MPI_Recv(&entrada_Alu_A, 1, MPI_INT, PR_MUX_ALU_A, tag, MPI_COMM_WORLD, &status); // recebe do Mux_Alu_A

		// printf("\t\tPROCESS = ALU ; Recebido de MUX_ALU_A (entrada_Alu_A = %d)\n",entrada_Alu_A);
		// fflush(NULL);

		MPI_Recv(&entrada_Alu_B, 1, MPI_INT, PR_MUX_ALU_B, tag, MPI_COMM_WORLD, &status); // recebe do Mux_Alu_B

		// printf("\t\tPROCESS = ALU ; Recebido de MUX_ALU_B (entrada_Alu_B = %d)\n",entrada_Alu_B);
		// fflush(NULL);

		// calcula o resultado da operacao
		// saida_Alu = entrada_Alu_A "op" entrada_Alu_B

		op = op & SEPARA_CONTROLE_ULA;

		if (op == 0)								// executa operacao AND
			saida_Alu = entrada_Alu_A & entrada_Alu_B;
		if (op == 1)								// executa operacao OR
			saida_Alu = entrada_Alu_A | entrada_Alu_B;
		if (op == 2)								// executa soma
			saida_Alu = entrada_Alu_A + entrada_Alu_B;
		if (op == 6)								// executa subtracao
			saida_Alu = entrada_Alu_A - entrada_Alu_B;
		if (op == 7) {								// executa operacao SLT
			if (entrada_Alu_A < entrada_Alu_B)
				saida_Alu = 1;
			else
				saida_Alu = 0;
		}

		MPI_Send(&saida_Alu, 1, MPI_INT, PR_ALU_OUT, tag, MPI_COMM_WORLD); 		// envia pro Alu_Out

		// printf("\t\tPROCESS = ALU ; Enviado para ALU_OUT (saida_Alu = %d)\n",saida_Alu);
		// fflush(NULL);

		MPI_Send(&saida_Alu, 1, MPI_INT, PR_PC, tag, MPI_COMM_WORLD); 			// envia pro PC

		// printf("\t\tPROCESS = ALU ; Enviado para PC (saida_Alu = %d)\n",saida_Alu);
		// fflush(NULL);


	}
}


// funcao executada pelo processo PR_ALU_CTRL
void Alu_Ctrl(){
	char AluOp[2];	// sinal de controle

	unsigned char func_field;	// campo de funcao recebido de IR
	unsigned char sc_alu = 0;		// operacao enviada pra AlU

	while(1){
		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = ALU_CRTL\n");
		// fflush(NULL);


		MPI_Recv(AluOp, 2, MPI_CHAR, PR_CU, tag, MPI_COMM_WORLD, &status);	// recebe sinal de controle

		// printf("\t\tPROCESS = ALU_CTRL ; Recebido de CU (AluOp = %d%d)\n",AluOp[1], AluOp[0]);
		// fflush(NULL);

		MPI_Recv(&func_field, 1, MPI_CHAR, PR_IR, tag, MPI_COMM_WORLD, &status); // recebe 1 char [5-0] do Ir (campo de funcao)

		// printf("\t\tPROCESS = ALU_CTRL ; Recebido de IR (func_field = %u)\n",func_field);
		// fflush(NULL);


		// verifica qual operacao a Alu vai executar

		// zera os 2 bits + signif. do campo de funcao. Nao sao necessarios.
		func_field = func_field & ZERA_2BITS_CFUNCAO;

		// zera os 3 bits menos sig de SC_ULA
		// necessario para usar | abaixo e nao atuar no bit "zero" (posicao 3)
		sc_alu = sc_alu & ZERA_CONTROLE_ALU;

		if ((AluOp[1] == 0) && (AluOp[0] == 0)) 					// executar soma na Alu
				sc_alu = (sc_alu | ATIVA_SOMA);

		else if ((AluOp[1] == 0) && (AluOp[0] == 1)) 			// executar subtracao na Alu
			sc_alu = (sc_alu | ATIVA_SUBTRACAO);

		// (AluOp[1] == 1) && (AluOp[0] == 0)
		else{	  												// executar outra operacao (verificar func_field)

			if (func_field == 0)								// executar soma na Alu
				sc_alu = (sc_alu | ATIVA_SOMA);

			else if (func_field == 2)							// executar subtracao na Alu
				sc_alu = (sc_alu | ATIVA_SUBTRACAO);

			else if (func_field == 4)							// executar operacao AND na Alu
				sc_alu = (sc_alu | ATIVA_AND);

			else if (func_field == 5)							// executar operacao OR na Alu
				sc_alu = (sc_alu | ATIVA_OR);

			else if (func_field == 10)							// executar operacao SLT na Alu
				sc_alu = (sc_alu | ATIVA_SLT);
		}
		MPI_Send(&sc_alu, 1, MPI_CHAR, PR_ALU, tag, MPI_COMM_WORLD); 	// envia pra Alu

		// printf("\t\tPROCESS = ALU_CTRL ; Enviado para ALU (operacao = %d)\n",sc_alu);
		// fflush(NULL);

	}
}


// funcao executada pelo processo PR_ALU_OUT
void Alu_Out(){
	int reg_alu_out = 9999999;

	while(1){
		MPI_Barrier(MPI_COMM_WORLD); //espera subir o clock

		// printf("\n\t PROCESS = ALU_OUT\n");
		// fflush(NULL);

		MPI_Send(&reg_alu_out, 1, MPI_INT, PR_MUX_MEM, tag, MPI_COMM_WORLD); // envia pro Mux_Mem

		// printf("\n\t\tPROCESS = ALU_OUT ; Enviado para MUX_MEM reg_alu_out = %d\n",reg_alu_out);
		// fflush(NULL);

		MPI_Send(&reg_alu_out, 1, MPI_INT, PR_MUX_WD, tag, MPI_COMM_WORLD); // envia pro Mux_Wd

		// printf("\t\tPROCESS = ALU_OUT ; Enviado para MUX_WD reg_alu_out = %d\n",reg_alu_out);
		// fflush(NULL);

		MPI_Recv(&reg_alu_out, 1, MPI_INT, PR_ALU, tag, MPI_COMM_WORLD, &status); // recebe da Alu

		// printf("\n\t\tPROCESS = ALU_OUT ; Recebido da ALU (reg_alu_out = %d)\n\n==============================\n RESULTADO FINAL do CICLO = %d\n==============================\n",reg_alu_out,reg_alu_out);
		// fflush(NULL);

		// printf("\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
		// printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
		// fflush(NULL);

	}
}

