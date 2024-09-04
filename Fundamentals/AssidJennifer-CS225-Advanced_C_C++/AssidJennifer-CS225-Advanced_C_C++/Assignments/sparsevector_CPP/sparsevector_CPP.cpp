/*****************************************************************
* \file   sparsevector_CPP.cpp
* \author Jennifer Assid
* \par    email: jennifer.assid@digipen.edu
* \par    DigiPen Login: jennifer.assid
* \par    Course: CS 225
* \par    Section A
* \par    Assignment 03
* \date   23/10/2021
* \brief
*
*      Contains functions needed to create the SparseVector and
*      ElementProxy classes.
*
*********************************************************************/

#include "sparsevector_CPP.h"

namespace CS225
{
	SparseVector::SparseVector(const SparseVector& rhs) : pHead(0), dimension(0)
	{
		// Initialize local variables
		ElementNode* rhs_head = rhs.pHead;
		ElementNode* head;

		// Set the dimensions to the same value
		dimension = rhs.dimension;

		if (rhs.pHead == NULL) return;

		// Create new head for the current sparsevector
		pHead = new ElementNode;
		pHead->data = rhs.pHead->data;
		pHead->pos = rhs.pHead->pos;
		pHead->next = new ElementNode;

		// Move head variables to the next node
		head = pHead->next;
		rhs_head = rhs_head->next;

		// While rhs still has nodes
		while (rhs_head)
		{
			// Copy over the data and position
			head->data = rhs_head->data;
			head->pos = rhs_head->pos;

			// If there is another node in rhs list, create new node
			if (rhs_head->next) head->next = new ElementNode;
			else head->next = NULL;

			// Continue iteration
			rhs_head = rhs_head->next;
			head = head->next;
		}
	}

	SparseVector::~SparseVector()
	{
		// Delete all of the elements within the SparseVector
		ElementNode* temp;

		while (pHead)
		{
			temp = pHead->next;
			delete pHead;
			pHead = temp;
		}
	}

	SparseVector& SparseVector::operator=(const SparseVector& rhs)
	{
		// Return out of function if the two classes are already equal
		if (rhs.pHead == pHead) return *this;

		// Initialize local variables
		ElementNode* rhs_head = rhs.pHead;
		ElementNode* head;

		// Set the dimensions to the same value
		dimension = rhs.dimension;

		// Exit out if rhs doesn't have any elements
		if (rhs.pHead == NULL)
		{
			pHead = NULL;
			return *this;
		}

		// Create new head for the current sparsevector
		pHead = new ElementNode;
		pHead->data = rhs.pHead->data;
		pHead->pos = rhs.pHead->pos;
		pHead->next = new ElementNode;

		// Move head variables to the next node
		head = pHead->next;
		rhs_head = rhs_head->next;

		// While rhs still has nodes
		while (rhs_head)
		{
			// Copy over the data and position
			head->data = rhs_head->data;
			head->pos = rhs_head->pos;

			// If there is another node in rhs list, create new node
			if (rhs_head->next) head->next = new ElementNode;
			else head->next = NULL;

			// Continue iteration
			rhs_head = rhs_head->next;
			head = head->next;
		}

		return *this;
	}

	int SparseVector::Get(long pos) const
	{
		/* Initialize all local variables */
		ElementNode* head = pHead;

		/* Iterate through the list */
		while (head)
		{
			/* If the position has been reached, return the data */
			if (head->pos == pos) return head->data;

			/* Update the stepper variable to continue iteration */
			head = head->next;
		}

		/* Return default value */
		return 0;
	}

	void SparseVector::Insert(int val, long pos)
	{
		/* Initialize all local variables */
		ElementNode* head = pHead;
		ElementNode* prev = NULL;
		ElementNode* new_Node;

		if (pos >= dimension) dimension = pos + 1;

		/* If the list is NULL, create new head */
		if (!head)
		{
			/* If the given data is 0, return */
			if (val == 0) return;

			/* Attempt to create new node with the given data */
			new_Node = new ElementNode;
			/* Return error if allocation fails */
			if (!new_Node) return;

			/* Assign the given values to the newly created node */
			new_Node->data = val;
			new_Node->pos = pos;
			new_Node->next = NULL;

			/* Have the list pointer point to its new head (new_Node) */
			pHead = new_Node;

			/* Return sucess code */
			return;
		}

		/* If the given position is before the head, create new head */
		if (pos < head->pos)
		{
			/* If the given data is 0, return */
			if (val == 0) return;

			/* Attempt to create new node with the given data */
			new_Node = new ElementNode;
			/* Return error if allocation fails */
			if (!new_Node) return;

			/* Assign the given values to the newly created node */
			new_Node->data = val;
			new_Node->pos = pos;
			new_Node->next = head;

			/* Have the list pointer point to its new head (new_Node) */
			pHead = new_Node;

			/* Return sucess code */
			return;
		}

		/* If the given position is the head, set the data */
		if (pos == head->pos)
		{
			/* If the given data is 0, delete the head */
			if (val == 0) Delete(pos);
			/* Else change the data out */
			else head->data = val;

			/* Return success code */
			return;
		}

		/* Set up the stepper variables */
		prev = head;
		head = head->next;

		/* Iterate through the list*/
		while (head)
		{
			/* If the position lies between these two nodes */
			if (prev->pos < pos && pos < head->pos)
			{
				/* If the data is 0, return out of function */
				if (val == 0) return;

				/* Attempt to create new node with the given data */
				new_Node = new ElementNode;
				/* Return error if allocation fails */
				if (!new_Node) return;

				/* Assign the given values to the newly created node */
				new_Node->pos = pos;
				new_Node->data = val;
				new_Node->next = head;
				prev->next = new_Node;

				/* Return success code */
				return;
			}

			/* If the position exists */
			if (head->pos == pos)
			{
				/* If the data is 0, delete the element*/
				if (val == 0) Delete(pos);
				/* Else change the data out */
				else head->data = val;

				/* Return success code */
				return;
			}

			/* Adjust stepper values to continue iteration */
			prev = head;
			head = head->next;
		}

		/* Attach node with given data at the end of the list (if the data is not 0) */
		if (val == 0) return;

		/* Attempt to create new node with the given data */
		new_Node = new ElementNode;
		/* Return error if allocation fails */
		if (!new_Node) return;

		/* Assign the given values to the newly created node */
		new_Node->data = val;
		new_Node->pos = pos;
		new_Node->next = NULL;
		prev->next = new_Node;
	}

	void SparseVector::Delete(long pos)
	{
		/* Initialize all local variables */
		ElementNode* head = NULL;
		ElementNode* prev = NULL;
		ElementNode* temp = NULL;

		/* If the list is empty, return out of function */
		if (pHead == NULL) return;

		/* Update the stepper variables */
		head = pHead;
		prev = head;

		/* If the position is at the head */
		if (head->pos == pos)
		{
			/* Set the list pointer to point to the new head */
			pHead = head->next;
			/* Free the value */
			delete head;

			/* Return out of function */
			return;
		}

		/* Iterate through the list to find the node at the given pos (if it exists) */
		while (head->next)
		{
			/* If the position has been found */
			if (head->pos == pos)
			{
				/* Delete the node at the pos and reconnect the list */
				temp = head->next;
				delete head;
				head = temp;
				prev->next = head;

				/* Return out of function */
				return;
			}

			/* Update stepper variables to continue iteration */
			prev = head;
			head = head->next;
		}

		if (head->pos == pos)
		{
			/* Delete the node at the pos and reconnect the list */
			delete head;
			prev->next = NULL;

			/* Return out of function */
			return;
		}
	}


	/* this is the only place where dimension is used */
	std::ostream& operator<<(std::ostream& out, const SparseVector& v) {
		int i, last_pos = -1;
		ElementNode* p_e = v.pHead;
		while (p_e) {
			for (i = last_pos + 1; i < p_e->pos; ++i) out << " " << "0";
			out << " " << p_e->data;
			last_pos = p_e->pos;
			p_e = p_e->next;
		}
		for (i = last_pos + 1; i < v.dimension; ++i) out << " " << "0";

		return out;
	}

	SparseVector& SparseVector::operator+=(const SparseVector& rhs)
	{
		int pos = 0;
		ElementNode* head_1 = pHead;
		ElementNode* head_2 = rhs.pHead;

		/* Iterate over the two lists */
		while (head_1 && head_2)
		{
			/* If the two lists share the same position*/
			if (head_1->pos == head_2->pos && head_1->pos == pos)
			{
				/* Add the elements and add them to the list and increment both lists */
				Insert(head_1->data + head_2->data, pos);
				head_1 = head_1->next;
				head_2 = head_2->next;
			}
			/* If only list one shares the position */
			else if (head_1->pos == pos)
			{
				/* Insert the value in the first list and increment */
				Insert(head_1->data, pos);
				head_1 = head_1->next;
			}
			/* If only list two shares the position */
			else if (head_2->pos == pos)
			{
				/* Insert the value in the second list and increment */
				Insert(head_2->data, pos);
				head_2 = head_2->next;
			}

			/* Increment the comparison variable */
			pos++;
		}

		/* if list one is longer than list two */
		if (head_1)
		{
			/* Iterate over the remainder of the list */
			while (head_1)
			{
				/* If the position matches the comparitor */
				if (head_1->pos == pos)
				{
					/* Insert the element and continue iterating */
					Insert(head_1->data, pos);
					head_1 = head_1->next;
				}

				/* Increment the comparitor variable */
				pos++;
			}
		}
		/* If list two is longer than list one */
		else
		{
			/* Iterate over the reaminder of list two */
			while (head_2)
			{
				/* If the position matches the comparitor */
				if (head_2->pos == pos)
				{
					/* Insert the element and continue iterating */
					Insert(head_2->data, pos);
					head_2 = head_2->next;
				}

				/* Increment the comparitor variable */
				pos++;
			}

		}

		/* Return the result */
		return *this;
	}

	SparseVector SparseVector::operator+(const SparseVector& rhs)
	{
		/* Initilize all local variables */
		SparseVector result_list;
		int pos = 0;
		ElementNode* head_1 = pHead;
		ElementNode* head_2 = rhs.pHead;

		/* Iterate over the two lists */
		while (head_1 && head_2)
		{
			/* If the two lists share the same position*/
			if (head_1->pos == head_2->pos && head_1->pos == pos)
			{
				/* Add the elements and add them to the list and increment both lists */
				result_list.Insert(head_1->data + head_2->data, pos);
				head_1 = head_1->next;
				head_2 = head_2->next;
			}
			/* If only list one shares the position */
			else if (head_1->pos == pos)
			{
				/* Insert the value in the first list and increment */
				result_list.Insert(head_1->data, pos);
				head_1 = head_1->next;
			}
			/* If only list two shares the position */
			else if (head_2->pos == pos)
			{
				/* Insert the value in the second list and increment */
				result_list.Insert(head_2->data, pos);
				head_2 = head_2->next;
			}

			/* Increment the comparison variable */
			pos++;
		}

		/* if list one is longer than list two */
		if (head_1)
		{
			/* Iterate over the remainder of the list */
			while (head_1)
			{
				/* If the position matches the comparitor */
				if (head_1->pos == pos)
				{
					/* Insert the element and continue iterating */
					result_list.Insert(head_1->data, pos);
					head_1 = head_1->next;
				}

				/* Increment the comparitor variable */
				pos++;
			}
		}
		/* If list two is longer than list one */
		else
		{
			/* Iterate over the reaminder of list two */
			while (head_2)
			{
				/* If the position matches the comparitor */
				if (head_2->pos == pos)
				{
					/* Insert the element and continue iterating */
					result_list.Insert(head_2->data, pos);
					head_2 = head_2->next;
				}

				/* Increment the comparitor variable */
				pos++;
			}

		}

		/* Return the result */
		return result_list;
	}

	SparseVector& SparseVector::operator-=(const SparseVector& rhs)
	{
		int pos = 0;
		ElementNode* head_1 = pHead;
		ElementNode* head_2 = rhs.pHead;

		/* Iterate over the two lists */
		while (head_1 && head_2)
		{
			/* If the two lists share the same position*/
			if (head_1->pos == head_2->pos && head_1->pos == pos)
			{
				/* Add the elements and add them to the list and increment both lists */
				Insert(head_1->data - head_2->data, pos);
				head_1 = head_1->next;
				head_2 = head_2->next;
			}
			/* If only list one shares the position */
			else if (head_1->pos == pos)
			{
				/* Insert the value in the first list and increment */
				Insert(head_1->data, pos);
				head_1 = head_1->next;
			}
			/* If only list two shares the position */
			else if (head_2->pos == pos)
			{
				/* Insert the value in the second list and increment */
				Insert(-head_2->data, pos);
				head_2 = head_2->next;
			}

			/* Increment the comparison variable */
			pos++;
		}

		/* if list one is longer than list two */
		if (head_1)
		{
			/* Iterate over the remainder of the list */
			while (head_1)
			{
				/* If the position matches the comparitor */
				if (head_1->pos == pos)
				{
					/* Insert the element and continue iterating */
					Insert(head_1->data, pos);
					head_1 = head_1->next;
				}

				/* Increment the comparitor variable */
				pos++;
			}
		}
		/* If list two is longer than list one */
		else
		{
			/* Iterate over the reaminder of list two */
			while (head_2)
			{
				/* If the position matches the comparitor */
				if (head_2->pos == pos)
				{
					/* Insert the element and continue iterating */
					Insert(-head_2->data, pos);
					head_2 = head_2->next;
				}

				/* Increment the comparitor variable */
				pos++;
			}

		}

		/* Return the result */
		return *this;
	}

	int SparseVector::operator*(SparseVector& rhs)
	{
		/* Initialize all local variables */
		int product = 0;
		ElementNode* head_1 = pHead;
		ElementNode* head_2 = rhs.pHead;

		/* Iterate through the two lists */
		while (head_1 && head_2)
		{
			/* If the lists share the same position, determine the product of their data */
			if (head_1->pos == head_2->pos)
			{
				product += head_1->data * head_2->data;
			}
			/* If list 1 is behind list 2 iterate over list 1 and continue through iteration */
			else if (head_1->pos < head_2->pos)
			{
				head_1 = head_1->next;
				continue;
			}
			/* If list 2 is behind list 1 iterate over list 2 and continue through iteration */
			else
			{
				head_2 = head_2->next;
				continue;
			}

			/* Update stepper variables to continue iteration */
			head_1 = head_1->next;
			head_2 = head_2->next;
		}

		/* Return the determined product */
		return product;
	}

	SparseVector SparseVector::operator*(int scalar)
	{
		// Initialize local variables
		SparseVector result(*this);
		ElementNode* head = pHead;

		// Iterate through the current list
		while (head)
		{
			// Multiply the value at the given positon in the list by the scalar
			result.Insert(head->data * scalar, head->pos);
			// Continue iterating
			head = head->next;
		}

		// Return the new scaled SparseVector
		return result;
	}

	SparseVector operator*(int scalar, SparseVector rhs)
	{
		// Call the multiplication function
		return rhs.operator*(scalar);
	}

	int operator*(const SparseVector& lhs, const SparseVector& rhs)
	{
		// Cast the variables so the class multiplication function can work
		SparseVector temp = static_cast<SparseVector>(lhs);
		SparseVector temp_ = static_cast<SparseVector>(rhs);

		return temp.operator*(temp_);
	}

	ElementProxy SparseVector::operator[](long pos)
	{
		// Create an associative ElementProxy class with the given vector and position
		ElementProxy result(*this, pos);
		return result;
	}

	int SparseVector::operator[](long pos) const
	{
		// Create an associative ElementProxy class with the given vector and position
		SparseVector temp = static_cast<SparseVector>(*this);
		ElementProxy result(temp, pos);
		return result.operator int();
	}

}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <iostream>
#include <sstream>
#include <cstdlib>

void sparsevector_CPP_test0() {
	CS225::SparseVector v1;
	for (int i = 0; i < 10; ++i) v1.Insert(i + 1, i);
	std::cout << "v1 = " << v1 << std::endl;
}

void sparsevector_CPP_test1() {
	CS225::SparseVector v1;
	for (int i = 0; i < 10; ++i) v1.Insert(i + 1, i);
	CS225::SparseVector v2(v1);
	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;
}

void sparsevector_CPP_test2() {
	CS225::SparseVector v1;
	CS225::SparseVector v2(v1);
	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;
}

void sparsevector_CPP_test3() {
	CS225::SparseVector v1;
	for (int i = 0; i < 10; ++i) v1.Insert(i + 1, i);
	{
		CS225::SparseVector v2(v1);
		for (int i = 0; i < 5; ++i) v2.Insert(10, i + 10);
		std::cout << "v1 = " << v1 << std::endl;
		std::cout << "v2 = " << v2 << std::endl;
	}
	std::cout << "v1 = " << v1 << std::endl;
}

void sparsevector_CPP_test4() {
	CS225::SparseVector v1, v2;
	for (int i = 0; i < 10; ++i) v1[i] = i + 1;
	for (int i = 0; i < 5; ++i) v2[i + 10] = i + 11;
	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;

	CS225::SparseVector v3(v1);
	std::cout << "v3 = " << v3 << std::endl;

	std::cout << "Delete every second element in v1\n";
	for (int i = 0; i < 10; i += 2) v1[i] = 0;
	std::cout << "v1 = " << v1 << std::endl;
}

void sparsevector_CPP_test5() {
	CS225::SparseVector v1, v2, v3;
	for (int i = 0; i < 10; ++i) v1[2 * i] = i + 1;
	for (int i = 0; i < 5; ++i) v2[5 * i] = i + 10;
	for (int i = 0; i < 15; ++i) v3[i] = i + 20;

	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;
	std::cout << "v3 = " << v3 << std::endl;
	CS225::SparseVector v12 = v1 + v2;
	std::cout << "v12 = " << v12 << std::endl;
	CS225::SparseVector v13 = v1 + v3;
	std::cout << "v13 = " << v13 << std::endl;
	CS225::SparseVector v23 = v2 + v3;
	std::cout << "v23 = " << v23 << std::endl;

	CS225::SparseVector v21 = v2 + v1;
	std::cout << "v21 = " << v21 << std::endl;
	CS225::SparseVector v31 = v3 + v1;
	std::cout << "v31 = " << v31 << std::endl;
	CS225::SparseVector v32 = v3 + v2;
	std::cout << "v32 = " << v32 << std::endl;
}

void sparsevector_CPP_test6() {
	CS225::SparseVector v1, v2, v3;
	for (int i = 0; i < 10; ++i) v1[2 * i] = i + 1;
	for (int i = 0; i < 5; ++i) v2[5 * i] = i + 10;
	for (int i = 0; i < 15; ++i) v3[i] = i + 20;

	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;
	std::cout << "v3 = " << v3 << std::endl;
	std::cout << "v2+v2 = " << v2 + v2 << std::endl;
	std::cout << "v1+v2+v3 = " << v1 + v2 + v3 << std::endl;
	std::cout << "Check v1,v2,v3 are the same\n";
	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;
	std::cout << "v3 = " << v3 << std::endl;
}

void sparsevector_CPP_test7() {
	CS225::SparseVector v1, v2, v3;
	for (int i = 0; i < 10; ++i) v1[2 * i] = i + 1;
	for (int i = 0; i < 5; ++i) v2[5 * i] = i + 10;
	for (int i = 0; i < 15; ++i) v3[i] = i + 20;

	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;
	std::cout << "v3 = " << v3 << std::endl;
	std::cout << "v1*10 = " << v1 * 10 << std::endl;
	std::cout << "10*v2 = " << 10 * v2 << std::endl;
	std::cout << "0*v3 = ";
	CS225::SparseVector v4 = 0 * v3;
	v4.PrintRaw();
	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;
	std::cout << "v3 = " << v3 << std::endl;
}

void sparsevector_CPP_test8() {
	CS225::SparseVector v1, v2, v3;
	for (int i = 0; i < 10; ++i) v1[2 * i] = i + 1;
	for (int i = 0; i < 5; ++i) v2[5 * i] = i + 10;
	for (int i = 0; i < 15; ++i) v3[i] = i + 20;

	std::cout << "v1*10+v3*(-1) = " << v1 * 10 + v3 * (-1) << std::endl;

	std::cout << "10*v1+(-1)*v3 = " << 10 * v1 + (-1) * v3 << std::endl;
}

void sparsevector_CPP_test9() {
	CS225::SparseVector v1, v2, v3;
	for (int i = 0; i < 10; ++i) v1[2 * i] = i + 1;
	for (int i = 0; i < 5; ++i) v2[5 * i] = i + 10;
	for (int i = 0; i < 15; ++i) v3[i] = i + 20;

	std::cout << "v1 = " << v1 << std::endl;
	std::cout << "v2 = " << v2 << std::endl;
	std::cout << "v3 = " << v3 << std::endl;

	std::cout << "v12 = " << v1 * v2 << std::endl;
	std::cout << "v13 = " << v1 * v3 << std::endl;
	std::cout << "v23 = " << v2 * v3 << std::endl;

	std::cout << "v21 = " << v2 * v1 << std::endl;
	std::cout << "v31 = " << v3 * v1 << std::endl;
	std::cout << "v32 = " << v3 * v2 << std::endl;
}

void sparsevector_CPP_test10() {
	CS225::SparseVector v;
	v[1] = 1;
	v[2] = 2;
	v[13] = 13;
	CS225::SparseVector& vr = v;
	vr[1] = 0;
	vr[2] = 0;
	vr[6] = 6;
	vr[7] = 7;
	vr[15] = 15;
	v = vr;
	std::cout << "Should be equal\n";
	std::cout << "v  = ";
	v.PrintRaw();
	std::cout << std::endl;
	std::cout << "vr = ";
	vr.PrintRaw();
	std::cout << std::endl;
}

void sparsevector_CPP_test11() {
	CS225::SparseVector v;
	for (int i = 0; i < 10; ++i) v.Insert(i + 1, i);
	std::cout << "v = " << v << std::endl;

	std::cout << "Second element is v[2]=" << v[2] << std::endl;

	v[2] = 5;
	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;

	int temp = v[5];
	std::cout << "v[5]=" << temp << std::endl;
	v.PrintRaw(); std::cout << std::endl;

	v[6] += -5; v[5] -= 3;
	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;

	v[2] = v[8];
	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;
}

void sparsevector_CPP_test12() {
	CS225::SparseVector v;
	for (int i = 0; i < 10; ++i) v.Insert(i + 1, i);

	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;

	v[1] += v[5];
	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;

	v[2] -= v[2];
	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;
}

void sparsevector_CPP_test13() {
	CS225::SparseVector v;
	for (int i = 0; i < 10; ++i) v[i] = i;

	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;

	v[1] = v[5] = v[2];
	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;

	v[1] += v[5] += v[2];
	std::cout << "v = " << v << std::endl;
	v.PrintRaw(); std::cout << std::endl;
}

void sparsevector_CPP_test14() { // sparsevector_CPP_test for existance of nodes with 0 in them
	CS225::SparseVector v;
	for (int i = 0; i < 16; ++i) v[i] = i % 2;

	v.PrintRaw();

	CS225::SparseVector v1, v2;
	for (int i = 0; i < 20; ++i) { v1[i] = i % 2; v2[i] = -v1[i]; }
	CS225::SparseVector v_sum = v1 + v2;
	v_sum.PrintRaw();

	CS225::SparseVector v3;
	for (int i = 0; i < 20; ++i)  v3[i] = i + 1;
	for (int i = 0; i < 20; i += 2) v3[i] -= v3[i];
	v3.PrintRaw();
	std::cout << "v3 = " << v3 << std::endl;
}

void sparsevector_CPP_test15() {
	CS225::SparseVector v;
	for (int i = 0; i < 6; ++i)  v[i] = i + 1;

	const CS225::SparseVector v_copy(v);
	std::cout << "v_copy = " << v_copy << std::endl;
	std::cout << "v_copy*v_copy = " << v_copy * v_copy << std::endl;

	std::cout << "10*v_copy = " << 10 * v_copy << std::endl;
}

void sparsevector_CPP_test16() {
	CS225::SparseVector v;
	for (int i = 0; i < 6; ++i)  v[i] = i + 1;
	std::cout << "v = " << v << std::endl;

	CS225::SparseVector v_1(0 * v);
	std::cout << "CS225::SparseVector v_1(0*v);\n";
	v_1.PrintRaw();
}

void sparsevector_CPP_test17() {
	CS225::SparseVector v;
	for (int i = 0; i < 6; ++i)  v[i] = i + 1;

	const CS225::SparseVector v_copy(v);
	std::cout << "v_copy = " << v_copy << std::endl;

	std::cout << "v_copy[4] = " << v_copy[4] << std::endl;

	const CS225::SparseVector v_1(0 * v_copy);
	std::cout << "const CS225::SparseVector v_1(0*v_copy);\n";
	v_1.PrintRaw();
}