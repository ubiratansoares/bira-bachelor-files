// Mashcipher - Monoalphabetical Substitution with Homonyms Cipher

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MULTFACTOR 10 	// Fator de multiplicidade para os homonimos de cada caractere a ser representado
#define ORDER 40 		// Quanitidade de caracteres no alfabeto a ser cifrado e decifrado
#define MAXINTERLEAVE 5 // Quantidade de alternantes para caracteres
#define MAXSIMBOL 9999	// Maior homonimo possivel para o texto cifrado
#define NUMCOLUMNS 36	// Quantidade de colunas de matrix de homonimos (26 letras + 10 digitos)

void print_help(void);

int** generate_hmatrix(int *absfrequences,int *numlines);

void release_hmatrix(int *hmatrix[],int numlines);

int print_hmatrix(int *hmatrix[], int numlines);

int** load_hmatrix(int *numlines);

int save_hmatrix(int *hmatrix[], int numlines);

int* mash_encript(char *plaintext,int **hmatrix, int numlines,int *cipherlenght);
char* mash_decript(int *ciphertext);


void save_encripted(int *ciphertext, int cipherlenght);
void save_decripted(char *ciphertext);

char* get_plaintext();

void print_encripted(int *ciphertext, int n);
void print_decripted(char *plaintext);

void normalize(char *message);


char charset[42] = {65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,48,49,50,51,52,53,55,56,57,58,44,46,58,63,32,33};
				//  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  0  1  2  3  4  5  6  7  8  9  ,   .  :  ?  ' !

int absfrequences[26] = {15,1,4,5,13,1,1,1,6,1,1,3,5,5,11,3,1,7,8,4,5,2,1,1,1,1};
					  //  A B ---              ...                     ---  Y Z


int main (int argc, char *argv[]){
	
	int args = 0;
	int verbose = 0;
	int encript = 0;
	int decript = 0;
	int newmatrix = 0;
	int printmatrix = 0;
	int usefile = 0;

	char *plaintext = NULL;
	int *ciphertext = NULL;
	int cipherlenght;
	
	int **hmatrix = NULL;
	int numcolumns = 36;
	int numlines = 0;
		
	int flag = 0;

	if(argc == 1) {
		
		print_help();
		return;
	}

	// Desativa as mensagens de erro da função getopt
	opterr = 0;

	while ((args = getopt (argc, argv, "cdefpvt:")) != -1){
		
		switch (args){
			
			case 'c':
				newmatrix = 1;			
				break;

			case 'd':				
				decript = 1;
				break;

			case 'e':
				encript = 1;
				break;
				
			case 'f':			
				usefile = 1;				
				break;
				
			case 'p':
			
				printmatrix = 1;

				break;	
				
			case 'v':
				verbose = 1;
				break;
				
			case 't':
				plaintext = optarg;
				break;
				

			case '?':
				if (optopt == 'd' || optopt == 'e')	// Esqueceu o texto para cifrar ou decifrar
					fprintf (stderr, "Options '-%c' requires a <plaintext> argument.\n", optopt);
					
				
					 else if (isprint (optopt))
				     		fprintf(stderr, "Invalid '-%c' Option.\n", optopt );
			        
						else
			            	fprintf(stderr, "Character '\\x%x' .\n", optopt );
			exit (1);
		}
	}


	if(newmatrix) {

		hmatrix = generate_hmatrix(absfrequences,&numlines);
		
		//printf("\n\n generate_matrix numlines = %d\n",numlines);
			
			
		if(hmatrix) {
			
				printf("\n\nHomonyms Matrix Sucessful Created.\n\n");
		
				flag = save_hmatrix(hmatrix, numlines);
		
				if(flag) printf("\n\nHomonyms Matrix Sucessful Saved.\n\n");
				else printf("\n\nERROR AT SAVE HOMONYMS FILE.\n\n");
				
		}
		
		else printf("\n\nERROR. Can't create Homonyms Matrix.\n\n");

		//print_hmatrix(hmatrix,numlines);

	} else {
		
		hmatrix = load_hmatrix(&numlines);
		
		if(hmatrix != NULL) 
			printf("\n\nHomonyms Matrix Sucessful Loaded.\n\n");		
		
			else printf("\n\nERROR. Can't load Homonyms Matrix\n");
			
	}
	
	if(printmatrix) print_hmatrix(hmatrix,numlines);

	if(encript && decript) print_help();
	
	else if(encript){ 
		
			if(usefile) plaintext = get_plaintext();
			
			ciphertext = mash_encript(plaintext,hmatrix,numlines,&cipherlenght);

			if(ciphertext == NULL) printf("\n\nERROR AT ENCRYPT PLAINTEXT.\n\n");		
			
			else {
					save_encripted(ciphertext,cipherlenght);
	
				 	print_encripted(ciphertext,cipherlenght);		
			}
		 }
		
		else if(decript){ 
							
				plaintext = mash_decript(ciphertext);

				if(usefile) save_decripted(plaintext);
				
				print_decripted(plaintext);
				
				
			 }
	
			else release_hmatrix(hmatrix,numlines);
				
	return 0;
}

void print_help(void){
	
	printf("\n\n == Mashcipher - Monoalphabetical Substituition with Homonyms Cipher ==\n\n"); 
	
	printf("\nUSAGE: mashcipher [-c] [-d] [-e] [-v]"); 
	printf("\n\t-c : create a new random homonymics matrix");
	printf("\n\t-e <text> : encrypt an input plaintext");
	printf("\n\t-d <text> : decrypt an input ciphered text");
	printf("\n\t-v : verbose mode");

			
	printf("\n\nV0.03 - Ubiratan Soares\n\n");
	
	
} //end print_help


int** generate_hmatrix(int *absfrequences, int *numlines){
	
	int i,j;
	int **aux;
	
	int r = 0;
	
	// Decidindo fator de multiplicidade
	
	while(r == 0) r = arc4random() % MULTFACTOR;
	
 	*numlines = r * 15;

	printf(" \n\n r = %d   numlines = % d \n\n",r,*numlines);
	
	// Alocando a nova matrix de homonimos
	
	aux = (int**)malloc(*numlines * sizeof(int*));
	
	if(aux == NULL) return 0;
	
	for(i = 0; i < *numlines; i++){
		aux[i] = malloc(36 *sizeof(int));
		if(aux == NULL){
			break;
			return 0;
		}

	}	
	
	// Preenchendo a matrix com numeros aleatorios dentre os possiveis homonimos
	
	for(j = 0; j < 36; j++)
		for(i = 0; i < *numlines; i++)
			
			if(i > r * absfrequences[j]) aux[i][j] = 0;
			else aux[i][j] = arc4random() % (MAXSIMBOL + 1);
			
	
	return aux;
	
} //end generate_hmatrix



void release_hmatrix(int *hmatrix[ORDER], int numlines){
	
	int i;
	
	for(i = 0; i < numlines; i++)
		free(hmatrix[i]);
		
	free(hmatrix);
	
	
} //end release_hmatrix


int print_hmatrix(int *hmatrix[], int numlines){
	
	int i, j;
	
	for(i = 0; i < numlines; i++){
		for(j = 0; j < 36; j++){
			
			printf("%d ",hmatrix[i][j]);
			
		}
	
		printf("\n");
	}
	
} //end print_hmatrix

int** load_hmatrix(int *numlines){
	
	int i, j;
	int n;
	FILE *hm;
	int **aux;
	
	hm = fopen("homonyms","r");
	
	if(hm == NULL) return NULL;
	
	else{
		
		fscanf(hm,"%d",numlines);
		
		//printf("\n\n load_matrix numlines = %d\n",n);
		
		aux = (int**)malloc(*numlines * sizeof(int*));

		if(aux == NULL) return 0;

		for(i = 0; i < *numlines; i++){
			aux[i] = malloc(36 *sizeof(int));
			if(aux == NULL){
				break;
				return 0;
			}

		}
		
		for(i = 0; i < *numlines; i++)
			for(j = 0; j < NUMCOLUMNS; j++)
		
			fscanf(hm,"%d",&aux[i][j]);
	
		fclose(hm);
		
		return aux;
		
	}	
	
} // end load_matrix

int save_hmatrix(int *hmatrix[NUMCOLUMNS], int numlines){
	
	int i, j;
	FILE *hm;
	
	hm = fopen("homonyms","w");
	
	if(hm == NULL) return 0;
	
	else{
		
		fprintf(hm,"%d\n",numlines);

		for(i = 0; i < numlines; i++)
			for(j = 0; j < NUMCOLUMNS; j++)
		
			fprintf(hm,"%d\n",hmatrix[i][j]);
	
		fclose(hm);
		
		return 1;
		
	}
	
	
} //end save_matrix


int* mash_encript(char *plaintext, int **hmatrix, int numlines, int *cipherlenght){


} //end mash_encript


char* mash_decript(int *ciphertext){


} // end mash_decript

void save_encripted(int *ciphertext, int cipherlenght){}
void save_decripted(char *ciphertext){}

char* get_plaintext(){}

void print_encripted(int *ciphertext, int n){}
void print_decripted(char *plaintext){}




