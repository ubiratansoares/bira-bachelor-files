#include "Object.h"
#include "Vector.h"

int main(int argc, char *argv[]) {
	Object *o1 = new Object();
	Object *o2 = new Object();

	Vector v1; // STACK
	v1.add(o1);
	v1.add(o2);

//	Vector v3; // STACK
//      v3 = v1; // operator=
	
	Vector v3 = v1; // naum chamo operator=
//	Vector v3(v1);

	return 0;
}
