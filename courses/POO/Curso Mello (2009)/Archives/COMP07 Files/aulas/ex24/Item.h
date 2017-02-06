#ifndef _ITEM_H_
#define _ITEM_H_

class Item : public Object {
	private:
		static int static_code;
		int code;
		char *title;
		int qtt;
		int availability;
	public:
		Item(char *title, int qtt, int availability) {
			this->code = static_code++;
			this->setTitle(title);
			this->setQtt(qtt);
			this->setAvailability(availability);
		}
		void setTitle(char *title) {
			this->title = new char[strlen(title) + 1];
			strcpy(this->title, title);
		}
		void setQtt(int qtt) { this->qtt = qtt; }
		void setAvailability(int availability) { 
			this->availability = availability; 
		}

		char *getTitle() { return this->title; }
		int getQtt() { return this->qtt; }
		int getAvailability() { return this->availability; }
};

int Item::static_code = 0;

#endif
