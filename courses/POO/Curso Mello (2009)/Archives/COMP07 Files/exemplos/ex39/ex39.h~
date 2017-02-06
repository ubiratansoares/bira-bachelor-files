#include <iostream>
using namespace std;
class Ponto {
	int x;
	int y;
	public:
		Ponto(int x, int y) {
			this->x = x;
			this->y = y;
		}
		int getX() { return this->x; }
		int getY() { return this->y; }
		Ponto operator+(Ponto &p) {
			return Ponto(this->x + p.getX(), this->y + p.getY());
		}
		void operator=(Ponto &p) {
			this->x = p.getX();
			this->y = p.getY();
		}
		void print() {
			cout << "x: " << this->x << " y: " << this->y << endl;
		}
};
