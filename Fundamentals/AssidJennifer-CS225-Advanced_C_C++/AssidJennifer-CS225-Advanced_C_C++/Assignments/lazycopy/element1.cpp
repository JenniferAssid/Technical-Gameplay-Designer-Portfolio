/*****************************************************************
 * \file   element1.h
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

#include "element1.h"

CS225::Element1::Element1(int _val) : AbstractElement(1),val(_val) { }

int CS225::Element1::Get() const { return val; }

void CS225::Element1::Set(int new_val) { val=new_val; }

void CS225::Element1::Print() const { 
  std::cout << "[" << Get() << "] "; 
}
