#include <iostream>
#include <stdarg.h>

using namespace std;

void imprimir(int numero, ...)
{
	va_list lista;

	va_start(lista, numero); 

	cout << "Os parâmetros são: ";

	for (int i = 0; i < numero; i++) 
		cout << va_arg(lista, int) << " ";

	cout << "\n";

	va_end(lista); // Closing macro
}

int main(int argc, char *argv[]) {

	imprimir(2, 1, 2);
	imprimir(5, 1, 2, 3, 4, 5);
	return 0;
}
