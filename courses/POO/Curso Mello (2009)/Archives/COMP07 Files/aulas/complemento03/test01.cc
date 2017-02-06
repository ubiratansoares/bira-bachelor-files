#include "Vector.h"

int main(int argc, char *argv[]) {
	Vector<int> v;
	int i;

	int a = 1;
	int b = 2;
	int c = 3;

	v.add(a);
	v.add(b);
	v.add(c);

	cout << "size: " << v.size() << endl;
	for (i = 0; i < v.size(); i++) {
		cout << v.get(i) << endl;
	}

	return 0;
}
