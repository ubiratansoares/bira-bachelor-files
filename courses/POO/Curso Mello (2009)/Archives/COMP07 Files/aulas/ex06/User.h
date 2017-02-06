#include <iostream>

using namespace std;

class User {
	private:
		char *cpf;
		char *name;
	public:
		void setCpf  (char *v) { cpf  = new char[strlen(v) + 1]; strcpy(cpf  , v);}
		void setName (char *v) { name = new char[strlen(v) + 1]; strcpy(name , v);}
		char *getCpf  () { return cpf  ;}
		char *getName () { return name ;}
};
