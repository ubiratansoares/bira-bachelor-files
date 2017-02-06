#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include "book.h"
#include "cdrom.h"
#include "dvdrom.h"
#include "item.h"
#include "media.h"
#include "paper.h"
#include "publication.h"

#include <iostream>
#include <vector>


class Library{

	private :

			vector<item> v;


	public:

			Library(){}


			void add(Item item){

				v->pushback(item);
			}


			item search(string title){


				if(!v.empty()){

					iterator p;
					int find;

					p = 


					for(p = v.begin(); p!= v.end(); p++	){

					}

				}
			}


};



#endif
