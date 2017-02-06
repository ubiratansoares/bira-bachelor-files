#include "book.h"
#include "cdrom.h"
#include "dvdrom.h"
#include "item.h"
#include "media.h"
#include "paper.h"
#include "publication.h"

#include <iostream>
#include <vector>

using namespace std;

int main(void){


	vector<Item> lib;


	Cdrom cd1;
	Paper paper1;
	Dvdrom dvd1;

	lib.push_back(cd1);
	lib.push_back(paper1);
	lib.push_back(dvd1);


	return 0;
}
