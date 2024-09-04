/*****************************************************************
 * \file   element2.h
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Assignment 04
 * \date   30/10/2021
 * \brief
 *    Contains functions necessary to the Element2 class
 * 
 *********************************************************************/

#ifndef ELEMENT2_H
#define ELEMENT2_H
#include <iostream>
#include "abstract-element.h"

/*
 * A concrete class implementing AbstractElement interface
 * Uses dynamically allocated data.
 */
namespace CS225 {

  class Element2 : public AbstractElement {
    public:
      /**
       * Non-Default Constructor for the Element2 class
       */
      Element2(int _val);
      /**
       * Copy Constructor for the Element2 class
       */
      Element2(const Element2& rhs);
      /**
       * Assignment operator for the Element2 class
       */
      Element2& operator=(const Element2& rhs);
      /**
       * VIRTUAL Get the value
       */
      virtual int Get() const;
      /**
       * VIRTUAL Set the value
       */
      virtual void Set(int new_val);
      /**
       * VIRTUAL Print the value
       */
      virtual void Print() const;
      /**
       * VIRTUAL Destructor for the Element2 class
       */
      virtual ~Element2();
    private:
      int * p_val; //!< Value
  };

}
#endif
