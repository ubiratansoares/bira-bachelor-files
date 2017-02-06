#include <iostream>
#include "Livro.h"
#include "User.h"

using namespace std;

template <class T>

class Vector {
	private:
		T *elements;
		int count;
	public:
		void init() {
			elements = NULL;
			count = 0;
		}

		void add(T l) {
			int i;
			elements = (T *) realloc(elements,
					sizeof(T) * (count + 1));
			elements[count] = l;
			count++;
		}

		void remove(int pos) {
			if (pos >= 0 && pos < count) {
				elements[pos] = elements[count-1];
				count--;
				elements = (T *) realloc(elements,
						sizeof(T) * count);
			}
		}

		T get(int pos) {
			if (pos >= 0 && pos < count)
				return elements[pos];
			return NULL;
		}

		int size() {
			return count;
		}
};
