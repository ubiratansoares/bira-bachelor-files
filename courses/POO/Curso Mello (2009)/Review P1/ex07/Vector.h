#ifndef _VECTOR_H_
#define _VECTOR_H_

template <class Template>

class Vector {
	Template *elements;
	int count;

	public:
		Vector() {
			this->elements = NULL;
			this->count = 0;
		}

		~Vector() {
			free(this->elements);
		}

		void add(Template e) {
			this->elements = (Template *)
				realloc(this->elements, // heap
						sizeof(Template ) * (count + 1));
			this->elements[count] = e;
			this->count++;
		}

};

#endif
