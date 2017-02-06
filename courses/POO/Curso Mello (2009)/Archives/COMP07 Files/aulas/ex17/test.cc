#include <iostream>

using namespace std;

typedef union {
	char op;
	int abc;
} DATA;

int main(int argc, char *argv[]) {
	
	DATA d;

	d.abc = 100000097;

	cout << "conteudo " << d.op << endl;

	return 0;
}
