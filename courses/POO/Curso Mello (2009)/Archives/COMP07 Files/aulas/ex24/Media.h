#ifndef _MEDIA_H_
#define _MEDIA_H_

class Media: public Item {
	private:
		int time;
		int tracks;
		char *company;

	public:

		Media(char *title, int qtt, int availability,
				int time, int tracks, char *company) : 
			Item(title, qtt, availability) {
			this->setTime(time);
			this->setTracks(tracks);
			this->setCompany(company);
		}

		void setTime(int time) { this->time = time; }
		void setTracks(int tracks) { this->tracks = tracks; }
		void setCompany(char *company) { 
			this->company = new char[strlen(company) + 1];
			strcpy(this->company, company);
		}

		int getTime() { return this->time; }
		int getTracks() { return this->tracks; }
		char *getCompany() { return this->company; }
};

#endif
