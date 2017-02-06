#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>

using namespace std;

class Item{

	private: 

			int code;
			string title;

	public:

			void setCode(int code) { this->code = code; }

			void setTitle(string title) { this->title = title; }

			int getCode(void){ return this->code; }

			string getTitle(void){ return this->title; }

};

#endif

