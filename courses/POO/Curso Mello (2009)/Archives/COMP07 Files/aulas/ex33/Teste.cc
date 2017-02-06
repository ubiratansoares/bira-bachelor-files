#include "Object.h"
#include "Vector.h"

int main(int argc, char *argv[]) {
	Object *o1 = new Object();
	Object *o2 = new Object();
	Object *o3 = new Object();

	Vector *v1 = new Vector();
	v1->add(o1);
	v1->add(o2);

	Vector *v2 = new Vector();
	v2->add(o3);

	Vector *v3 = *v1 + v2;

	cout << "Size: " << v3->size() << endl;

	return 0;
}
