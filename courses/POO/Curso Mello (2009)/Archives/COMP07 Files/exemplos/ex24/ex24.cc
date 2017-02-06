#include "ex24.h"

Demo::Demo ( const char *msg ) {
	strcpy( Nome , msg );
	cout << "Construtor chamado por " << Nome << '\n';
}

Demo::~Demo () {
	cout << "Destrutor chamado por " << Nome << '\n';
}

void Funcao()  {
	Demo ObjetoLocalFuncao( "ObjetoLocalFuncao" );

	static Demo Objetoestatico( "ObjetoEstatico" );
		// Objeto estático: Inicializado quando a função for
		// chamada pela primeira vez e destruido no
		// encerramento do programa.

	cout << "Dentro da funcao" << endl;
}

Demo ObjetoGlobal( "ObjetoGlobal" ); 
	// Objeto global : Inicializado no começo da função 
	// main e destruído no final da execução do 
	// programa.

int main(int argc, char *argv[])
{
	Demo ObjetoLocalMain( "ObjetoLocalMain" );
		// Objeto local: inicializado no começo do bloco
		// e destruído no fim do bloco.

	cout << "\nNa main, antes de chamar a funcao" << endl;

	Funcao();

	cout << "\nNa main, depois de chamar a funcao" << endl;

	return 0;
}
