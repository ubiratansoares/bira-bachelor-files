#include <iostream>

using namespace std;

#define multiplica(a, b) a * b

int main(int argc, char *argv[]) {
	int x = 1, y = 10;

	cout << "resultado = " << multiplica(5, 6) << endl;
	cout << "resultado = " << multiplica(5 * x, 6 * y) << endl;

	return 0;
}
