#include <iostream>
#include "item.h"

using namespace std;

int main(void){


	int code;
	string title;

	Item *item = new Item();

	code = 1;
	title = "Title 01";


	item->setCode(code);
	item->setTitle(title);

	cout << item->getCode() << endl << item->getTitle() << endl;

	return 0;
}
