#ifndef _PERSON_H_
#define _PERSON_H_

#include <iostream>

using namespace std;

class Person {
	private:
		char *name;
		int age;
	public:

		Person() { // construtor
			this->name = NULL;
			this->age = 0;
		}

		Person(char *name) { // construtor
			setName(name);
			this->age = 0;
		}

		Person(char *name, int age) { // construtor
			setName(name);
			setAge(age);
		}

		~Person() { // destrutor
			// liberar memoria heap
			// naum precisa, nem deve liberar memoria estatica (stack)
			//delete[] this->name;
			free(this->name);
		}

		void setName(char *name) { 
			this->name = (char *) malloc(sizeof(char) * 
					(strlen(name) + 1));
			//this->name = new char[strlen(name)+1];
			strcpy(this->name, name);
		}
		char *getName() { return name; }

		void setAge(int age) { this->age = age; }
		int getAge() { return this->age; }

		void print() {
			cout << "Name: " << this->name <<
				" Age: " << this->age << endl;
		}
};

#endif
