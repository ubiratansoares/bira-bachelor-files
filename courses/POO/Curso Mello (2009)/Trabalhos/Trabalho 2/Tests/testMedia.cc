#include <iostream>
#include "item.h"
#include "media.h"

using namespace std;

int main(void){


	int code;
	string title;

	Media *media = new Media();

	media->setCode(01);
	media->setTitle("Media 01");

	media->setYear(01);
	media->setDistributor("Distributor 01");


	cout << media->getCode() << endl 
		<< media->getTitle() << endl
		<< media->getYear() << endl
		<< media->getDistributor() << endl;


	return 0;
}
