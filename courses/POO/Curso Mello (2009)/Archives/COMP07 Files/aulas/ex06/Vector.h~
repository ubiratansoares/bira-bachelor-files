#include <iostream>
#include "Livro.h"

using namespace std;

class Vector {
	private:
		Livro **elements;
		int count;
	public:
		void init() {
			elements = NULL;
			count = 0;
		}

		void add(Livro *l) {
			int i;
			Livro ** tmp = 
				(Livro **) malloc(sizeof(Livro *) * (count + 1));
			for (i = 0; i < count; i++) {
				tmp[i] = elements[i];
			}

			tmp[count] = l;
			count++;
			free(elements);
			elements = tmp;
		}

		Livro *get(int pos) {
			return elements[pos];
		}

		int size() {
			return count;
		}
};
