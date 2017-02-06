#ifndef _DVDROM_H_
#define _DVDROM_H_

#include "publication.h"

class Dvdrom : public Media{

	private: 

			int elapsedtime;
			string director;
			string producer;
			string cast[3];

	public:

			void setElapsedtime(int elapsedtime) { this->elapsedtime = elapsedtime; }

			void setDirector(string director) { this->director = director; }

			void setProducer(string producer) { this->producer = producer; }

			void setArtist(string artist, int k) { 

				if(k <= 3) this->cast[k - 1] = artist; 

			}

			int getElapsedtime(void){ return this->elapsedtime; }

			string getDirector(void){ return this->director; }

			string getProducer(void){ return this->producer; }

			string getArtist(int position){ return this->cast[position]; }

};

#endif

