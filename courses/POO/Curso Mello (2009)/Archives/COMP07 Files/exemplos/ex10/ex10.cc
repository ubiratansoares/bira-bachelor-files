#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	char str[] = "Dia";
	char *p;
	int i;

	for (i = 0; i < 3; i++) {
		cout << str[i] << endl;
	}
	cout << endl;

	p = str;
	while (*p != '\0') {
		cout << *p << endl;
		p++;
	}

	return 0;
}
