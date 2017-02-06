#include <iostream>
#include "User.h"

using namespace std;

class VectorUser {
	private:
		User **elements;
		int count;
	public:
		void init() {
			elements = NULL;
			count = 0;
		}

		void add(User *l) {
			int i;
			elements = (User **) realloc(elements,
					sizeof(User *) * (count + 1));
			elements[count] = l;
			count++;
		}

		void remove(int pos) {
			if (pos >= 0 && pos < count) {
				elements[pos] = elements[count-1];
				count--;
				elements = (User **) realloc(elements,
						sizeof(User *) * count);
			}
		}

		User *get(int pos) {
			if (pos >= 0 && pos < count)
				return elements[pos];
			return NULL;
		}

		int size() {
			return count;
		}
};
