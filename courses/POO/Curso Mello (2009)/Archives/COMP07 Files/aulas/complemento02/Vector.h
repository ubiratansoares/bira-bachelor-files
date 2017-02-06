#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

using namespace std;

template <class T>

class Vector {
	private:
		T *elementos;
		int tamanho;

	public:
		Vector() {
			elementos = NULL;
			tamanho = 0;
		}

		void add(T item);

		T get(int posicao) {
			return elementos[posicao];
		}

		int size() {
			return tamanho;
		}

		void remove(int i) {
			if (i > 0 && i < tamanho) {
				elementos[i] = elementos[tamanho-1];
				tamanho--;
			}
		}
};

template <class T>

void Vector<T>::add(T item) {
	elementos = (T *) realloc (elementos,
		sizeof(T) * (tamanho+1));
	elementos[tamanho++] = item;
}

#endif
