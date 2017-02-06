#ifndef _VECTOR_H_
#define _VECTOR_H_

class Vector : public Object {
	private:
		Object **elements;
		int vsize;
	public:
		Vector() {
			this->elements = NULL;
			this->vsize = 0;
		}

		~Vector() {
			free(this->elements);
		}

		void add(Object *obj) {
			this->elements = (Object **)
				realloc(this->elements, (vsize+1) * sizeof(Object *));
			this->elements[vsize] = obj;
			vsize++;
		}

		void add(Vector *vector) {
			for(int i = 0; i < vector->size(); i++) {
				this->add(vector->elementAt(i));
			}
		}

		Vector *operator+(Vector *vector) {
			Vector *result = new Vector();

			for (int i = 0; i < this->vsize; i++) 
				result->add(this->elementAt(i));

			for (int i = 0; i < vector->size(); i++) 
				result->add(vector->elementAt(i));

			return result;
		}

		void operator=(Vector *vector) {
			free(this->elements);
			this->elements = NULL;
			this->vsize = 0;
			this->add(vector);
		}

		int size() {
			return this->vsize;
		}

		Object *elementAt(int pos) {
			if (pos >= 0 && pos < this->vsize)
				return this->elements[pos];
			else
				return NULL;
		}

		void remove(int pos) {
			this->elements[pos] = this->elements[vsize-1];
			vsize--;
			this->elements =  (Object **)
				realloc(this->elements, vsize * sizeof(Object *));
		}

		// NULL e fora do range tah dando pau
		void remove(Object *obj) {
			for (int i = 0; i < this->vsize; i++) {
				if (this->elements[i] == obj) {
					remove(i);
					i--;
				}
			}
		}
};

#endif
