// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H
#include "abstract-element.h"

namespace CS225 {
	class ElementFactory {
		public:
			AbstractElement* MakeElement(int id,int value) const;	
	};
}
#endif
