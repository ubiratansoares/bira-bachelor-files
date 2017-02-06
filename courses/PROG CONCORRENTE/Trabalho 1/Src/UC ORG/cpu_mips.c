/*******************************************************************************************************/
/* Arquivo trab2.c
   Autor: Paulo Sergio Lopes de Souza
Observacoes:
(1) Trabalho 2 - SSC0112 - Organizacao de Computadores Digitais I
(2) Disponibilizado em 11/10/2009 - Data da entrega do trabalho: 07/12/2009
(3) O arquivo trab2.c deve ser utilizado por todos os grupos. Nao o altere.
(4) Para realizar o seu trabalho, edite um arquivo texto chamado hardwired_code.c Insira nele todas as funcionalidades necessarias ao seu trabalho.
(7) A entrega do trabalho devera ser feita para o email orgcomp1@gmail.com
    Digite, obrigatoriamente, no subject: Trab02 - Grupo XXX (onde XXX indica a Turma e o nr do grupo).
    Forneça, obrigatoriamente, no corpo do email o número do grupo, os integrantes e, em anexo, apenas os arquivos hardwirde_code.zip e MEF.pdf
 */
#include <stdio.h>
#include <stdlib.h>
/***********************
prototipacao inicial
 ***********************/
int main(void);
int UnidadeControle(int opcode);
/* contem todas as funcoes desenvolvidas por voce neste Trabalho para a UC hardwired*/
#include "uc.c"

int main(void) {
    int ciclo, opcode;
    int sc = 0; // Sinais de Controle. Cada bit determina um dos sinais de controle que saem da UC
    while (1) // loop para solicitar o opcode
    {
        printf("Entre com o opcode da instrucao (valor -1 encerra o programa): ");
        scanf("%d", &opcode);
        printf("\n");
        if (opcode == -1) {
            printf("Encerrando o trab2! \n");
            exit(0);
        }
        if (opcode != 35 && opcode != 0 && opcode != 43 && opcode != 8 && opcode != 12 &&
                opcode != 4 && opcode != 2 && opcode != 3 && opcode != 20 && opcode != 21 &&
                opcode != 5) {
            printf("Opcode invalido. Tente outro! \n");
            continue;
        }
        ciclo = 1; //variavel que indica o ciclo da instrucao
        while (1) // aqui comeca o que seria a execucao da instrucao
        { // aqui comeca o que seria um novo ciclo
            // sc representa os sinais de controle vindos da UC
            sc = UnidadeControle(opcode);
            printf("instrucao: %x, ciclo da instrucao: %x, sinais de controle: %x\n", opcode, ciclo, sc);
            // criterio de parada da instrução. Como estamos simulando apenas o comportamento
            // da UC, sem o caminho de dados completo, este IF finalizará a instrução, permitindo
            // fornecer outra para execucao.
            // Determina se estamos no ultimo ciclo de cada instrucao para finalizar a execucao
            // if (lw E ciclo 5) OU
            // ((Tipo-R OU sw OU addi OU andi) E ciclo 4) OU
            // ((beq OU j OU jal OU jr OU jalr OU bne) E ciclo 3) OU
            // (esta tentando usar mais que 5 ciclos para a instrucao)
            //
            //                      então finaliza o loop da instrucao
            //
            if (((opcode == 35) && ciclo == 5) ||
                    ((opcode == 0 || opcode == 43 || opcode == 8 || opcode == 12) && ciclo == 4) ||
                    ((opcode == 4 || opcode == 2 || opcode == 3 || opcode == 20 || opcode == 21 || opcode == 5) && ciclo == 3)) {
                break; // ultimo ciclo da instrução. Termina este loop interno para pedir outro opcode ao usuario.
            }
            ciclo++; // incrementa variavel que indica o ciclo da instrucao
            // aqui termina um ciclo
        } // fim da execução da instrução
    } // fim do loop para solicitar o opcode
    exit(0);
} // fim da main


