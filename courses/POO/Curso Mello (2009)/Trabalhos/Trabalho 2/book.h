#ifndef _BOOK_H_
#define _BOOK_H_

#include "publication.h"

class Book : public Publication{

	private: 

			int isbn;
			int numpages;
			string press;
			int edition;

	public:

			void setIsbn(int isbn) { this->isbn = isbn; }

			void setNumpages(int numpages) { this->numpages = numpages; }

			void setPress(string press) { this->press = press; }

			void setEdition(int edition) { this->edition = edition; }


			int getIsbn(void) { return this->isbn; }

			int getNumpages(void) { return this->numpages; }

			string getPress(void) { return this->press; }

			int getEdition(void) { return this->edition ; }

};

#endif

