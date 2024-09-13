/*****************************************************************
 * \file   element2.cpp
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Assignment 04
 * \date   30/10/2021
 * \brief
 *    Contains functions necessary to the Element1 class
 * 
 *********************************************************************/

#include "element2.h"

CS225::Element2::Element2(int _val) : AbstractElement(2), p_val(new int(_val)) {  }

CS225::Element2::Element2(const Element2& rhs) 
	: AbstractElement(2), p_val(new int(*rhs.p_val)) 
{ }

CS225::Element2& CS225::Element2::operator=(const Element2& rhs) {
  if (this!=&rhs) {
		delete p_val;
		p_val = new int(*rhs.p_val);
	} 
  return *this;
}

int CS225::Element2::Get() const {
	return *p_val;
}

void CS225::Element2::Print() const { 
  std::cout << "(" << *p_val << ") "; 
}

void CS225::Element2::Set(int new_val) { 
	*p_val = new_val; 
}

CS225::Element2::~Element2() {
	delete p_val;
}
