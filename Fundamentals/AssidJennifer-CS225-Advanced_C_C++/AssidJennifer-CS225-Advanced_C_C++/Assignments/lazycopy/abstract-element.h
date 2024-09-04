/*****************************************************************
 * \file   abstrct-elemt.h
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Assignment 04
 * \date   30/10/2021
 * \brief
 *    Contains functions necessary for the AbstractElement class
 * 
 *********************************************************************/

#ifndef ABSTRACT_ELEMENT_H
#define ABSTRACT_ELEMENT_H
#include <iostream>
#include "CRTP.h"

namespace CS225 {
  class AbstractElement : public ObjectCounter<AbstractElement> { // CRTP
    public:
      /**
       * Non-default constructor (assigns the id value of the class)
       */
      AbstractElement(int id) : id(id) {}
      /**
       * VIRTUAL - Get the value stored (default -1 for base class)
       */
      virtual int Get() const {return -1;}
      /**
       * VIRTUAL - Set the value stored (not relevant for base class)
       */
      virtual void Set(int new_val) {new_val++;}
      /**
       * VIRTUAL - Print the value with associated id visual (not relevent for base class)
       */
      virtual void Print() const {}
      /**
       * Get the ID value for the class
       */
      int GetID() const {return id;}
    protected:
      int id; //!< Stores the ID value
  };
}
#endif
