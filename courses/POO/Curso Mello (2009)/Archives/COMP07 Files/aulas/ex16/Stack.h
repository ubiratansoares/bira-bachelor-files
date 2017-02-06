#ifndef _STACK_H_
#define _STACK_H_

template <class DT>

class Stack {
	private:
		DT *base;
		DT *top;
		int count;
	public:
		Stack(int size) {
			//base = new DT[size];
			count = size;
			base = (DT *) malloc(sizeof(DT) * size);
			top = base - 1;
		}

		~Stack() {
			free(base);
		}

		void push(DT value) {
			if (count > 0) {
				top++;
				*top = value;
				count--;
			}
		}

		DT pop() {
			if (top >= base) {
				DT ret = *top;
				top--;
				count++;
				return ret;
			}
		}

		DT getTop() {
			if (top >= base)
				return *top;
		}

		int getEmpty() {
			return count;
		}
};

#endif
