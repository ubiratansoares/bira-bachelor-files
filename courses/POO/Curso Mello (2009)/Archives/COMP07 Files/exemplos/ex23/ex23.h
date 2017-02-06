#include <iostream>

using namespace std;

class Vector {
	int size;
	int *elements;
	
	public:
		Vector(int s) {
			if (s <= 0) {
				cout << "Erro: dimensão do vetor com problemas!" << endl;
			} else {
				cout << "Aloquei..." << endl;
				elements = new int[s];
				size = s;
			}
		}

		~Vector() {
			cout << "destruindo..." << endl;
			delete elements;
		}
};
