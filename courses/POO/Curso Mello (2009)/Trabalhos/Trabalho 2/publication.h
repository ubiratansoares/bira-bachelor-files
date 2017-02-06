#ifndef _PUBLICATION_H_
#define _PUBLICATION_H_

#include "item.h"

using namespace std;

class Publication : public Item{

	private: 

			int year;
			string format;
			string author;
			string content;

	public:

			void setYear(int year) { this->year = year; }

			void setFormat(string format) { this->format = format; }

			void setAuthor(string author) { this->author = author; }

			void setContent(string contents) { this->content = content; }


			int getYear(void) { return this->year; }

			string getFormat(void) { return this->format; }

			string getAuthor(void) { return this->author; }

			string getContent(void) { return this->content ; }

};

#endif

