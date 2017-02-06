#include "Object.h"
#include "Vector.h"

int main(int argc, char *argv[]) {
	Object *o1 = new Object();
	Object *o2 = new Object();
	Object *o3 = new Object();
	Object *o4 = new Object();

	Vector v1; // STACK
	v1.add(o1);
	v1.add(o2);

	cout << "Objetos de v1" << endl;
	for (int i = 0; i < v1.size(); i++)
		cout << v1.elementAt(i) << endl; // ponteiro para Object

	Vector v2; // STACK
	v2.add(o3);
	v2.add(o4);

	cout << "Objetos de v2" << endl;
	for (int i = 0; i < v2.size(); i++)
		cout << v2.elementAt(i) << endl; // ponteiro para Object

	Vector *v3 = v1 + v2; // operator+ (Vector &)

	cout << "Objetos de v3" << endl;
	for (int i = 0; i < v3->size(); i++)
		cout << v3->elementAt(i) << endl; // ponteiro para Object


	/*
	Vector *v3 = v1 + v2;
	Vector *v4 = new Vector(); // HEAP
	Vector *v5 = v1 + v4;
	*/

	return 0;
}
