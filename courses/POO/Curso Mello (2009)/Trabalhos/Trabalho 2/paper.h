#ifndef _PAPER_H_
#define _PAPER_H_

#include "publication.h"

class Paper : public Publication{

	private: 

			string instituion;
			string email;

	public:


			void setInstituiton(string instituion) { this->instituion = instituion; }

			void setEmail(string email) { this->email = email; }

			string getInstituion(void) { return this->instituion; }

			string getEmail(void) { return this->email ; }

};

#endif

