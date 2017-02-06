#include <iostream>
#include <math.h>

using namespace std;

#define Pot2(a) a*a

class Ponto {
	int x;
	int y;

	public:
		Ponto(int X, int Y) {
			x = X;
			y = Y;
		}
		int getX() { return x; }
		int getY() { return y; }
		float distancia(Ponto p) {
			float aux;
			aux = sqrt(Pot2(x*p.getX()) + Pot2(y*p.getY()));
			return aux;
		}

};
