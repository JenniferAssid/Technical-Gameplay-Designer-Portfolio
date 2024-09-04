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
#ifndef ELEMENT1_H
#define ELEMENT1_H
#include <iostream>
#include "abstract-element.h"
/*
 * A concrete class implementing AbstractElement interface
 * Trivial.
 */
namespace CS225 {

  class Element1 : public AbstractElement {
    public:
      /**
       * Non-default Constructor for the Element1 class
       */
      Element1(int _val);
      /**
       * VIRTUAL - Gets the value
       */
      virtual int Get() const;
      /**
       * VIRTUAL - Sets the value
       */
      virtual void Set(int new_val);
      /**
       * VIRTUAL - Prints the value
       */
      virtual void Print() const;
    private:
      int val; //!< Value
  };
}
#endif
