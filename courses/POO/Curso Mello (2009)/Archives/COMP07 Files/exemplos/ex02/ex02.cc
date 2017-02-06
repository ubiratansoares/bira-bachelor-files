#include <iostream>

using namespace std;

void funcao(int x, int y = 5) {
	cout << "x = " << x << " y = " << y << endl;
	return;
}

int main(int argc, char *argv[]) {

	funcao(3);
	funcao(5,7);

	return 0;
}
