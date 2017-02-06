#include <iostream>

/*
 * Em C++ o sizeof depende da arquitetura, em Java
 * os tamanhos são fixos e não há a necessidade de um
 * sizeof
 * */

using namespace std;

int main(int argc, char *argv[]) {

	cout << "Número de bytes:" << endl;
	cout << endl;

	cout << "char = " << sizeof(char) << endl;
	cout << "bool = " << sizeof(bool) << endl;
	cout << "short = " << sizeof(short) << endl;
	cout << "int = " << sizeof(int) << endl;
	cout << "long = " << sizeof(long) << endl;
	cout << "float = " << sizeof(float) << endl;
	cout << "double = " << sizeof(double) << endl;

	return 0;
}
