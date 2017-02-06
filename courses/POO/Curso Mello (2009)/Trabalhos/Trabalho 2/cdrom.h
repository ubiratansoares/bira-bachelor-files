#ifndef _CDROM_H_
#define _CDROM_H_

#include "media.h"

class Cdrom : public Media{

	private: 

			int tracks;
			string artist;
			string label;

	public:

			void setTracks(int tracks) { this->tracks = tracks; }

			void setArtist(string Artist) { this->artist = artist; }

			void setLabel(string label) { this->label = label; }

			int getTracks(void){ return this->tracks; }

			string getArtist(void){ return this->artist; }

			string getLabel(void){ return this->label; }

};

#endif

