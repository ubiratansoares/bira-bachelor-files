#ifndef _MEDIA_H_
#define _MEDIA_H_

#include <string>

using namespace std;

class Media : public Item{

	private: 

			int year;
			string distributor;

	public:

			void setYear(int year) { this->year = year; }

			void setDistributor(string distributor) { this->distributor = distributor; }

			int getYear(void){ return this->year; }

			string getDistributor(void){ return this->distributor; }

};

#endif

