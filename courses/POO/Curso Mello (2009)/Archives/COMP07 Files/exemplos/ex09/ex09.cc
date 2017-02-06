#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	int v[] = {5, 6, 7}; // int v[3];
	int i;

	cout << "endereço de v = " << &v << endl;
	cout << "conteudo de v = " << v << endl;
	cout << "para onde v aponta? = " << *v << endl;
	cout << endl;

	for (i = 0; i < 3; i++) {
		cout << "endereço de v[" << i << "] = " << &v[i] << endl;
		cout << "conteudo de v[" << i << "] = " << v[i] << endl;
		cout << endl;
	}

	return 0;
}
