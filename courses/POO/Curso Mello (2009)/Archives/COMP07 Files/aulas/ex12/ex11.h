#include <iostream>

using namespace std;

class Teste {
	private:
		char *str;
		char a;
		int b;
	public:
		Teste(char *str1) {
			//str = (char *) malloc(sizeof(char) * (strlen(str1) + 1));
			str = new char[strlen(str1) + 1];
			strcpy(str, str1);
			cout << "Construtor" << endl;
		}

		~Teste() {
			cout << "Destrutor" << endl;
			//free(str);
			delete str;
		}

};
