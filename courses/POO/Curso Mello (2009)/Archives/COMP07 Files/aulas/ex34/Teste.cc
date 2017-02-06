#include "Object.h"
#include "Vector.h"

int main(int argc, char *argv[]) {
	Object *o1 = new Object();
	Object *o2 = new Object();
	Object *o3 = new Object();
	Object *o4 = new Object();

	Vector v1;
	v1.add(o1);
	v1.add(o2);

	Vector v2;
	v2.add(o3);
	v2.add(o4);

	Vector *v3 = NULL;
        v3 = v1 + v2;

	cout << "v3->size() " << v3->size() << endl;
	for (int i = 0; i < v3->size(); i++) {
		cout << v3->elementAt(i) << endl;
	}

	Vector v3_1;
        v3_1 = v1 + v2;

	cout << "v3_1.size() " << v3_1.size() << endl;
	for (int i = 0; i < v3_1.size(); i++) {
		cout << v3_1.elementAt(i) << endl;
	}

	Vector v4 = v1;
	cout << "conteudo de v4" << endl;
	for (int i = 0; i < v4.size(); i++) {
		cout << v4.elementAt(i) << endl;
	}

	Vector v5;
	v5 = v2;

	cout << "conteudo de v5" << endl;
	for (int i = 0; i < v5.size(); i++) {
		cout << v5.elementAt(i) << endl;
	}

	return 0;
}
