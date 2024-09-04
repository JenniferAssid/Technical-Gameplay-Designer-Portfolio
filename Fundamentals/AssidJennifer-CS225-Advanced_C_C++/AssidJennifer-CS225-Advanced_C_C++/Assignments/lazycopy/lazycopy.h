#pragma once
/*****************************************************************
 * \file   array.h
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Assignment 04
 * \date   30/10/2021
 * \brief
 *    Contains functions necessary to the Array class
 *
 *********************************************************************/

#ifndef ARRAY_H
#define ARRAY_H
#include "abstract-element.h"
#include "element-factory.h"
#include <iostream>

namespace CS225 {

	/*
	 * Class Array implements a simple array-like data structure.
	 * Objects inside the Array have to be derived from AbstractElement.
	 */
	class Array {
	public:
		/**
		 * Non-Default Contructor for the Array class
		 */
		Array(int* array, unsigned int _size,
			const ElementFactory* _pElementFactory
		);
		/**
		 * Copy Constructor for the Array class
		 */
		Array(const Array& rhs);
		/**
		 * Destructor for the Array class
		 */
		~Array();
		/**
		 * Assignment operator for the Array class
		 */
		Array& operator=(const Array& rhs);
		/**
		 * Gets the value stored at the given position
		 */
		int Get(unsigned int pos) const;
		/**
		 * Sets the value at the given position with given values
		 */
		void Set(int id, int pos, int value);
		/**
		 * Prints array with associated ID identifiers
		 */
		void Print() const;
	private:
		int* ref_count; //!< Dyanmic reference counter
		AbstractElement** data; //!< Dynamic data
		unsigned int size; //!< Size of the array
		const ElementFactory* pElementFactory; //!< ElementFactory tied to the array

		/**
		 * Performs a deep copy with the given data
		 */
		Array& DeepCopy(AbstractElement** _data, unsigned int _size, const ElementFactory* p_ElementFactory);
	};
}

void lazycopy_test0();
void lazycopy_test1();
void lazycopy_test2();
void lazycopy_test3();
void lazycopy_test4();
void lazycopy_test5();
void lazycopy_test6();
void lazycopy_test7();
void lazycopy_test8();
void lazycopy_test9();
void lazycopy_test10();
void lazycopy_test11();
void lazycopy_test12();
void lazycopy_test13();
void lazycopy_test14();
void lazycopy_test15();
void lazycopy_test16();

#endif
