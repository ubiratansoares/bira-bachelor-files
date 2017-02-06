#include <iostream>

using namespace std;

class Pessoa {
	private:
		int id;
		char *name;
	public:
		void setId(int v) { id = v; }
		void setName(char *n) {
			name = new char[strlen(n) + 1];
			// name = (char *) malloc(sizeof(char) * (strlen(n) + 1));
			strcpy(name, n);
		}
		int getId() { return id; }
		char *getName() { return name; }
};
