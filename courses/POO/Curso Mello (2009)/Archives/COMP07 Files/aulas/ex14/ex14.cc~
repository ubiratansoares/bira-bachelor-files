#include "ex14.h"

Demo::Demo ( const char *msg ) {
	strcpy( Nome , msg );
	cout << "Construtor chamado por " << Nome << '\n';
}

Demo::~Demo () {
	cout << "Destrutor chamado por " << Nome << '\n';
}

void Funcao()  {
	Demo ObjetoLocalFuncao( "ObjetoLocalFuncao" );
	cout << "Dentro da funcao" << endl;
}

Demo ObjetoGlobal( "ObjetoGlobal" ); 

int main(int argc, char *argv[])
{
	Demo ObjetoLocalMain( "ObjetoLocalMain" );
	cout << "\nNa main, antes de chamar a funcao" << endl;
	Funcao();
	cout << "\nNa main, depois de chamar a funcao" << endl;
	return 0;
}
