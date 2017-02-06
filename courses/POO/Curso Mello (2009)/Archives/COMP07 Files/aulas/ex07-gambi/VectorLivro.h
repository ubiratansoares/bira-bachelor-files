#include <iostream>
#include "Livro.h"

using namespace std;

class VectorLivro {
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
			elements = (Livro **) realloc(elements,
					sizeof(Livro *) * (count + 1));
			elements[count] = l;
			count++;
		}

		void remove(int pos) {
			if (pos >= 0 && pos < count) {
				elements[pos] = elements[count-1];
				count--;
				elements = (Livro **) realloc(elements,
						sizeof(Livro *) * count);
			}
		}

		Livro *get(int pos) {
			if (pos >= 0 && pos < count)
				return elements[pos];
			return NULL;
		}

		int size() {
			return count;
		}
};
