/*****************************************************************
 * \file   array.cpp
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

#include "lazycopy.h"

CS225::Array::Array(int* array,
	unsigned int _size,
	const ElementFactory* _pElementFactory
)
	: ref_count(new int(1)), data(new AbstractElement* [_size]), size(_size),
	pElementFactory(_pElementFactory)
{
	for (unsigned int i = 0; i < size; ++i) {
		//create Element with id = 1 (that is Element1 )
		data[i] = pElementFactory->MakeElement(1, array[i]);
	}
}

CS225::Array::Array(const Array& rhs)
	: ref_count(rhs.ref_count), data(rhs.data), size(rhs.size), pElementFactory(rhs.pElementFactory)
{
	// Increase the ref_count
	*ref_count = *ref_count + 1;
}

CS225::Array::~Array()
{
	// If there aren't any associated classes
	if (*ref_count == 1)
	{
		// Delete data and ref_count
		for (unsigned int i = 0; i < size; ++i) delete data[i];

		delete[] data;
		delete ref_count;
	}
	// Else, decrement ref_count
	else *ref_count = *ref_count - 1;
}

CS225::Array& CS225::Array::operator=(const Array& rhs)
{
	// Check for self-assignment
	if (this == &rhs) return *this;

	// Clear the existing values of the class
	this->~Array();

	// Perform a shallow copy
	ref_count = rhs.ref_count;
	data = rhs.data;
	size = rhs.size;
	pElementFactory = rhs.pElementFactory;

	// Increment the ref_count
	*ref_count += 1;

	return *this;
}

int CS225::Array::Get(unsigned int pos) const { return data[pos]->Get(); }

void CS225::Array::Set(int id, int pos, int value)
{
	// If there are associated classes
	if (*ref_count > 1)
	{
		// Perform a deep copy
		DeepCopy(data, size, pElementFactory);
	}

	// If the value at the position isnt set to the given value
	if (data[pos]->Get() != value)
	{
		// Delete the current position
		delete data[pos];
		// Assign new element
		data[pos] = pElementFactory->MakeElement(id, value);
	}
}

void CS225::Array::Print() const {
	for (unsigned int i = 0; i < size; ++i) data[i]->Print();
	std::cout << std::endl;
}

CS225::Array& CS225::Array::DeepCopy(AbstractElement** _data, unsigned int _size, const ElementFactory* p_ElementFactory)
{
	// Clear current class
	this->~Array();

	// Perform Deep Copy
	ref_count = new int(1);
	size = _size;
	pElementFactory = p_ElementFactory;
	data = new AbstractElement * [_size];

	for (unsigned int i = 0; i < size; i++) data[i] = pElementFactory->MakeElement(_data[i]->GetID(), _data[i]->Get());


	return *this;
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include "element-factory.h"

#include <stdlib.h> /* atoi   */

void lazycopy_test0() { //basic lazycopy_test
	//client creates a factory to create elements
	//this way Array doesn't need to know about Element1 and Element2
	//this is yet another "programming to interfaces" example
	//or "old code calls new code"
	CS225::ElementFactory el_fact;

	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "modify myArray1\n";
	myArray1.Set(2, 3, 33);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}
void lazycopy_test1() { //copy
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6,7,8 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "make a copy of first array\n";
	CS225::Array myArray2(myArray1); //copy construct
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test2() { //copy 
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "modify myArray1\n";
	myArray1.Set(2, 3, 33);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "Create copy myArray2(myArray1);\n";
	CS225::Array myArray2(myArray1);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test3() { //assignment
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	int array2[] = { 1,2,3,4,5,6,7,8,9 };
	CS225::Array myArray2(array2, sizeof(array2) / sizeof(array2[0]), &el_fact);
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "Assign myArray2 = myArray1;\n";
	myArray2 = myArray1;
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test4() { //assignment 
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "Set myArray1.Set(2,3,33);\n";
	myArray1.Set(2, 3, 33);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	int array2[] = { 1,2,3,4,5,6,7,8,9 };
	CS225::Array myArray2(array2, sizeof(array2) / sizeof(array2[0]), &el_fact);
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "Assign myArray2 = myArray1;\n";
	myArray2 = myArray1;
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test5() { //copy using Element, then modify the copy
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "Create copy myArray2(myArray1);\n";
	CS225::Array myArray2(myArray1);
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "assign myArray2 = myArray1;\n";
	myArray2 = myArray1;
	std::cout << "Set myArray2.Set(2,3,33);\n";
	myArray2.Set(2, 3, 33);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();

	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test6() { //copy using Element, then modify the original
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	CS225::Array myArray2(myArray1);
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "Create copy myArray2(myArray1);\n";
	myArray2 = myArray1;
	std::cout << "Modify myArray1\n";
	myArray1.Set(2, 3, 33);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test7() { //assignment using Element2 then modify the copy
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	int array2[] = { 1,2,3,4,5,6,7,8,9 };
	CS225::Array myArray2(array2, sizeof(array2) / sizeof(array2[0]), &el_fact);
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "Assign myArray2 = myArray1;\n";
	myArray2 = myArray1;
	std::cout << "Modify myArray2\n";
	myArray2.Set(2, 3, 33);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test8() { //assignment using Element2 then modify the original
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	int array2[] = { 1,2,3,4,5,6,7,8,9 };
	CS225::Array myArray2(array2, sizeof(array2) / sizeof(array2[0]), &el_fact);
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "Assign myArray2 = myArray1;\n";
	myArray2 = myArray1;
	std::cout << "Modify myArray1\n";
	myArray1.Set(2, 3, 33);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

//function accepts by value
void foo(CS225::Array a) {
	std::cout << "in foo a = ";
	a.Print();
	std::cout << std::endl;
}

void lazycopy_test9() { //calling function that accepts by value
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	foo(myArray1);
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test10() { //copy then delete the original
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array* p_myArray1 = new CS225::Array(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "p_myArray1-Print();   ";
	p_myArray1->Print();

	CS225::Array myArray2 = *p_myArray1;
	std::cout << "p_myArray1-Print();   ";
	p_myArray1->Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "delete p_myArray1;\n";
	delete p_myArray1;
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test11() { //assign then delete the copy
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();

	{ //block
		int array2[] = { 1,2,3,4,5,6,7,8,9 };
		CS225::Array myArray2(array2, sizeof(array2) / sizeof(array2[0]), &el_fact);
		std::cout << "myArray2.Print();   ";
		myArray2.Print();

		myArray2 = myArray1;
		std::cout << "myArray1.Print();   ";
		myArray1.Print();
		std::cout << "myArray2.Print();   ";
		myArray2.Print();
	} //myArray2 is deleted

	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test12() { //chaining
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6,7,8,9 };
	CS225::Array* pArray1 = new CS225::Array(array, sizeof(array) / sizeof(array[0]), &el_fact);
	std::cout << "pArray1-Print();   ";
	pArray1->Print();

	int a[] = { 1 };
	CS225::Array* pArray2 = new CS225::Array(a, sizeof(a) / sizeof(a[0]), &el_fact);
	*pArray2 = *pArray1;
	delete pArray1;

	CS225::Array* pArray3 = new CS225::Array(a, sizeof(a) / sizeof(a[0]), &el_fact);
	*pArray3 = *pArray2;
	delete pArray2;

	std::cout << "pArray3-Print();   ";
	pArray3->Print();
	delete pArray3;
	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test13() {
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5, };
	CS225::Array  myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);

	int array2[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
	CS225::Array  myArray2(array2, sizeof(array2) / sizeof(array2[0]), &el_fact);

	int array3[] = { 1,2,3,4,5,6,7,8,9,10 };
	CS225::Array  myArray3(array3, sizeof(array3) / sizeof(array3[0]), &el_fact);

	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "myArray3.Print();   ";
	myArray3.Print();

	std::cout << "Assign myArray3=myArray1;\n";
	myArray3 = myArray1;

	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "myArray3.Print();   ";
	myArray3.Print();

	std::cout << "Assign myArray3=myArray2;\n";
	myArray3 = myArray2;

	std::cout << "myArray1.Print();   ";
	myArray1.Print();
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "myArray3.Print();   ";
	myArray3.Print();

	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

//function returns by value
CS225::Array give_my_array(int* p, unsigned int size) {
	CS225::ElementFactory el_fact;
	CS225::Array result(p, size, &el_fact);
	for (unsigned int i = 0; i < size; ++i) {
		result.Set(2, i, 2 * result.Get(i)); //double them all and change type
	}
	std::cout << "int give_my_array(...) result.Print();   ";
	result.Print();
	return result;
}

void lazycopy_test14() {
	int array[] = { 1,2,3,4,5,6,7,8,9 };
	CS225::Array  myArray1 = give_my_array(array, sizeof(array) / sizeof(array[0]));
	std::cout << "in lazycopy_test result from give_my_array     ";
	myArray1.Print();

	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
////lazycopy_test virtual constructor
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
void lazycopy_test15() { //copy using Element, then modify the copy
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	myArray1.Set(2, 3, 44);
	myArray1.Set(2, 4, 55);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();

	std::cout << "Create copy myArray2(myArray1);\n";
	CS225::Array myArray2(myArray1);
	std::cout << "myArray2.Print();   ";
	myArray2.Print();

	std::cout << "Modify myArray2\n";
	myArray2.Set(2, 1, 22);

	std::cout << "myArray1.Print();   ";
	myArray1.Print();

	std::cout << "myArray2.Print();   ";
	myArray2.Print();

	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}

void lazycopy_test16() { //copy using Element, then modify the original
	CS225::ElementFactory el_fact;
	int array[] = { 1,2,3,4,5,6 };
	int array2[] = { 1,2,3 };
	CS225::Array myArray1(array, sizeof(array) / sizeof(array[0]), &el_fact);
	myArray1.Set(2, 3, 44);
	myArray1.Set(2, 4, 55);
	std::cout << "myArray1.Print();   ";
	myArray1.Print();

	CS225::Array myArray2(array2, sizeof(array2) / sizeof(array2[0]), &el_fact);
	std::cout << "myArray2.Print();   ";
	myArray2.Print();
	std::cout << "Assign myArray2 = myArray1;\n";
	myArray2 = myArray1;
	std::cout << "myArray2.Print();   ";
	myArray2.Print();

	std::cout << "Modify myArray2\n";
	myArray2.Set(2, 1, 22);

	std::cout << "myArray1.Print();   ";
	myArray1.Print();

	std::cout << "myArray2.Print();   ";
	myArray2.Print();

	std::cout << "live  nodes  " << CS225::AbstractElement::live() << std::endl;
	std::cout << "total nodes  " << CS225::AbstractElement::total() << std::endl;
}
