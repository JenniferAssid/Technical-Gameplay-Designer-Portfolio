/*****************************************************************
 * \file   spvector.c
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Assignment 02
 * \date   05/10/2021
 * \brief
 *    Contains the following functions
 *
 *    -  printf_elements
 *          print elements as a vector, adding 0's for non-existing positions
 *          assume the length of the vector = dim
 *          doesn't check if the list contains positions beyond dim
 *
 *    -  print_elements
 *          print all elements in the list with positions
 *
 *    -  insert_elements
 *          insert an element into a list
 *          list is ordered using pos
 *          if position pos is already occupied, the value of the node
 *          should be updated with val
 *          if val=0, then the element should be deleted
 *          return 0 if operation is succesfull
 *                 1 if malloc failed
 *
 *   -  delete_element
 *          delete an element at position pos if it exists
 *
 *   -  get
 *          get value at the given posiiton, p_e is the head pointer
 *
 *   -  scalar_product
 *          scalar product of 2 lists
 *
 *   -  add
 *          add 2 lists as vectors, returns a new list
 *
 *   -  free_elements
 *          deallocate a list
 *
 *   -   printf_rows
 *          prints rows as a matrix, adding 0's for non-existing positions
 *          (both for non-existing rows and elements)
 *          assume the number of rows = dim
 *          assume the length of the vector = dim
 *          doesn't check if the list contains positions beyond dim
 *
 *   -  print_rows
 *           prints all non-zero elements of the list of rows
 *
 *   -  insert_row
 *           inserts a list of elements at position pos.
 *           If a row at position pos already exists, do not insert, return 2
 *           If the list p_e is empty, do nothing (don't insert a NULL pointer into p_r)
 *           returns 1 if malloc fails
 *           returns 2 if position pos already exists
 *           returns 0 otherwise
 *
 *   -  insert_element2
 *           inserts a new element at pos_col
 *           in the row corresponding to position pos_row
 *           (pos_row'th row, pos_col'th column)
 *           uses the same rules as insert_element
 *
 *   -  find_row
 *           returns a pointer to a row at position pos if it exists, NULL otherwise
 *
 *   -  transpose
 *           transposes a list of rows:
 *           it's the matrix transposition if you view rows as a matrix
 *           anyways - if an element in the original RowNode list
 *           is on row i at position j, then in the transposed
 *           it is in the row j at position i.
 *           Notice that it's possible to get empty rows in the transposed
 *           matrix (for example the following matrix
 *           [ 1 0 4 ]     [ 1 2 3 ]
 *           [ 2 0 5 ] ->  [ 0 0 0 ]
 *           [ 3 0 6 ]     [ 4 5 6 ]
 *           for full credit the transposed matrix should contain only 2
 *           rows (positions 0 and 2)
 *           Returns a new list of rows. Input list p_r should not be modified.
 *
 *   -  multi
 *           multiply two lists of RowNodes as matricies.
 *           Algorithm:
 *           1) transpose the second list.
 *           2) Now [i,j]'th element of the product matrix
 *           is the scalar product of i'th row of the first
 *           and the j'th row of the transposed.
 *           Returns a new list of rows. Input lists p_r1, p_r2 should not be modified.
 *
 *   -  free_rows
 *           deallocate a list of RowNodes
 *
 *   -  determinant
 *           Calculates the determinant of the list of RowNodes viewed as a matrix.
 *           Dimension is required!
 *           extra credit, discuss algorithm with me first
 *
 *********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "sparsevector.h"

 /* definition of Node is hidden from the client (driver) so that client CANNOT
  * use INTERNALS of the Node.  this is poor-man encapsulations - hiding
  * implementation details.  The main reason for encapsulations -- if I ever
  * decide to change Node struct no client code will break. Example: I decide to
  * change "left" to "Left", all I do is change map.h and map.c, no client code
  * is effected, since clients were FORCED to use getleft(...) function, rather
  * than field name "left".  Also see typedef in map.h
  */
struct ElementNode {
	int    data;
	int    pos;
	struct ElementNode* next;
};

struct RowNode {
	int pos;
	struct ElementNode* elements;
	struct RowNode* next;
};

typedef struct ElementNode ElementNode;
typedef struct RowNode RowNode;

void cofactor(RowNode_handle head, RowNode_handle result, int pos_row, int pos_col, int dimm);


/*print functions*/
void printf_elements(ConstElementNode_handle p_e, const char* fmt, int dim) {
	int i, last_pos = -1;
	while (p_e) {
		for (i = last_pos + 1; i < p_e->pos; ++i) { printf(fmt, 0); }
		printf(fmt, p_e->data);
		last_pos = p_e->pos;
		p_e = p_e->next;
	}
	for (i = last_pos + 1; i < dim; ++i) { printf(fmt, 0); }
}

void print_elements(ConstElementNode_handle p_e) {
	while (p_e) {
		printf("%i at pos %i, ", p_e->data, p_e->pos);
		p_e = p_e->next;
	}
}

int insert_element(ElementNode_handle* list, int pos, int data)
{
	/* Initialize all local variables */
	ElementNode* head = *list;
	ElementNode* prev = NULL;
	ElementNode* new_Node;

	/* If the list is NULL, create new head */
	if (!head)
	{
		/* If the given data is 0, return */
		if (data == 0) return 0;

		/* Attempt to create new node with the given data */
		new_Node = (ElementNode*)calloc(1, sizeof(ElementNode));
		/* Return error if allocation fails */
		if (!new_Node) return 1;

		/* Assign the given values to the newly created node */
		new_Node->data = data;
		new_Node->pos = pos;
		new_Node->next = NULL;

		/* Have the list pointer point to its new head (new_Node) */
		*list = new_Node;

		/* Return sucess code */
		return 0;
	}

	/* If the given position is before the head, create new head */
	if (pos < head->pos)
	{
		/* If the given data is 0, return */
		if (data == 0) return 0;

		/* Attempt to create new node with the given data */
		new_Node = (ElementNode*)calloc(1, sizeof(ElementNode));
		/* Return error if allocation fails */
		if (!new_Node) return 1;

		/* Assign the given values to the newly created node */
		new_Node->data = data;
		new_Node->pos = pos;
		new_Node->next = head;

		/* Have the list pointer point to its new head (new_Node) */
		*list = new_Node;

		/* Return sucess code */
		return 0;
	}

	/* If the given position is the head, set the data */
	if (pos == head->pos)
	{
		/* If the given data is 0, delete the head */
		if (data == 0) delete_element(list, pos);
		/* Else change the data out */
		else head->data = data;

		/* Return success code */
		return 0;
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
			if (data == 0) return 0;

			/* Attempt to create new node with the given data */
			new_Node = (ElementNode*)calloc(1, sizeof(ElementNode));
			/* Return error if allocation fails */
			if (!new_Node) return 1;

			/* Assign the given values to the newly created node */
			new_Node->pos = pos;
			new_Node->data = data;
			new_Node->next = head;
			prev->next = new_Node;

			/* Return success code */
			return 0;
		}

		/* If the position exists */
		if (head->pos == pos)
		{
			/* If the data is 0, delete the element*/
			if (data == 0) delete_element(list, pos);
			/* Else change the data out */
			else head->data = data;

			/* Return success code */
			return 0;
		}

		/* Adjust stepper values to continue iteration */
		prev = head;
		head = head->next;
	}

	/* Attach node with given data at the end of the list (if the data is not 0) */
	if (data == 0) return 0;

	/* Attempt to create new node with the given data */
	new_Node = (ElementNode*)calloc(1, sizeof(ElementNode));
	/* Return error if allocation fails */
	if (!new_Node) return 1;

	/* Assign the given values to the newly created node */
	new_Node->data = data;
	new_Node->pos = pos;
	new_Node->next = NULL;
	prev->next = new_Node;

	/* Return success code */
	return 0;
}

void delete_element(ElementNode_handle* list, int pos)
{
	/* Initialize all local variables */
	ElementNode* head = NULL;
	ElementNode* prev = NULL;
	ElementNode* temp = NULL;

	/* If the list is empty, return out of function */
	if (list == NULL) return;

	/* Update the stepper variables */
	head = *list;
	prev = head;

	/* If the position is at the head */
	if (head->pos == pos)
	{
		/* Set the list pointer to point to the new head */
		*list = head->next;
		/* Free the value */
		free(head);

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
			free(head);
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
		free(head);
		prev->next = NULL;

		/* Return out of function */
		return;
	}
}

int get(ConstElementNode_handle list, int pos)
{
	/* Initialize all local variables */
	ElementNode* head = (ElementNode*)list;

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

int scalar_product(ConstElementNode_handle list_1, ConstElementNode_handle list_2)
{
	/* Initialize all local variables */
	int product = 0;
	ElementNode* head_1 = (ElementNode*)list_1;
	ElementNode* head_2 = (ElementNode*)list_2;

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

ElementNode_handle add(ConstElementNode_handle list_1, ConstElementNode_handle list_2)
{
	/* Initilize all local variables */
	ElementNode* result_list = NULL;
	int pos = 0;
	ElementNode* head_1 = (ElementNode*)list_1;
	ElementNode* head_2 = (ElementNode*)list_2;

	/* Iterate over the two lists */
	while (head_1 && head_2)
	{
		/* If the two lists share the same position*/
		if (head_1->pos == head_2->pos && head_1->pos == pos)
		{
			/* Add the elements and add them to the list and increment both lists */
			insert_element(&result_list, pos, head_1->data + head_2->data);
			head_1 = head_1->next;
			head_2 = head_2->next;
		}
		/* If only list one shares the position */
		else if (head_1->pos == pos)
		{
			/* Insert the value in the first list and increment */
			insert_element(&result_list, pos, head_1->data);
			head_1 = head_1->next;
		}
		/* If only list two shares the position */
		else if (head_2->pos == pos)
		{
			/* Insert the value in the second list and increment */
			insert_element(&result_list, pos, head_2->data);
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
				insert_element(&result_list, pos, head_1->data);
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
				insert_element(&result_list, pos, head_2->data);
				head_2 = head_2->next;
			}

			/* Increment the comparitor variable */
			pos++;
		}

	}

	/* Return the result */
	return result_list;
}

void free_elements(ElementNode_handle list)
{
	/* Initiate all local variables */
	ElementNode* head = list;
	ElementNode* temp = head;

	/* Iterate through the list */
	while (head)
	{
		/* Delete elements */
		temp = head;
		head = head->next;
		free(temp);
	}
}

void print_rows(ConstRowNode_handle p_r) {
	while (p_r) {
		printf("row %i: ", p_r->pos);
		print_elements(p_r->elements);
		printf("\n");
		p_r = p_r->next;
	}
}

void printf_rows(ConstRowNode_handle p_r, const char* fmt, int dim) {
	int i, j, last_pos = -1;
	while (p_r) {
		for (i = last_pos + 1; i < p_r->pos; ++i) { /* output row of 0's */
			for (j = 0; j < dim; ++j) printf(fmt, 0);
			printf("\n");
		}
		printf_elements(p_r->elements, fmt, dim);
		printf("\n");
		last_pos = p_r->pos;
		p_r = p_r->next;
	}
	for (i = last_pos + 1; i < dim; ++i) { /* output row of 0's */
		for (j = 0; j < dim; ++j) printf(fmt, 0);
		printf("\n");
	}
}

/* meaningless implementations to get rid of warnings */
int insert_row(RowNode_handle* pp_r, int pos, ElementNode* elements)
{
	/* Initialize all local variables */
	RowNode* head = *pp_r;
	RowNode* prev = NULL;
	RowNode* new_Row = NULL;

	/* If the table is NULL, create new row */
	if (!head)
	{
		/* Attempt to create new node */
		new_Row = (RowNode*)calloc(1, sizeof(RowNode));
		/* Return error code if allocation fails */
		if (!new_Row) return 1;

		/* Assign data to new node */
		new_Row->elements = elements;
		new_Row->pos = pos;
		new_Row->next = NULL;

		/* Have the list point at the new head */
		*pp_r = new_Row;

		return 0;
	}

	/* If the given row is before the current head, create new head */
	if (pos < head->pos)
	{
		/* Attempt to create new node */
		new_Row = (RowNode*)calloc(1, sizeof(RowNode));
		/* Return error code if allocation fails */
		if (!new_Row) return 1;

		/* Assign data to new node */
		new_Row->elements = elements;
		new_Row->pos = pos;
		new_Row->next = head;

		/* Have the list point at the new head */
		*pp_r = new_Row;

		return 0;
	}

	/* If the given position is the head, set the data */
	if (pos == head->pos)
	{
		head->elements = elements;
		return 2;
	}

	/* Update local variables */
	prev = head;
	head = head->next;

	/* Iterate through the list */
	while (head)
	{
		/* If the position lies between these two nodes */
		if (prev->pos < pos && pos < head->pos)
		{
			/* Attempt to create new node */
			new_Row = (RowNode*)calloc(1, sizeof(RowNode));
			/* Return error code if allocation fails */
			if (!new_Row) return 1;

			/* Assign data to new node */
			new_Row->elements = elements;
			new_Row->pos = pos;
			new_Row->next = head;
			prev->next = new_Row;

			return 0;
		}

		/* If the position exists */
		if (head->pos == pos)
		{
			head->elements = elements;
			return 2;
		}

		/* Continue iteration */
		prev = head;
		head = head->next;
	}

	/* Attempt to create new node */
	new_Row = (RowNode*)calloc(1, sizeof(RowNode));
	/* Return error code if allocation fails */
	if (!new_Row) return 1;

	/* Assign data to new node */
	new_Row->elements = elements;
	new_Row->pos = pos;
	new_Row->next = NULL;
	prev->next = new_Row;

	return 0;
}

RowNode* find_row(RowNode_handle* pp_r, int pos)
{
	/* Initiate local variables */
	RowNode* head = *pp_r;

	if (!head) return NULL;

	/* Iterate through the list */
	while (head)
	{
		/* If position is found, return data */
		if (head->pos == pos) return head;

		head = head->next;
	}

	return NULL;
}

void free_rows(RowNode_handle p_v)
{
	/* Initialize all local variables */
	RowNode* head = p_v;
	RowNode* temp = head;

	/* Itreate over the list */
	while (head)
	{
		/* Free the elements */
		temp = head;
		head = head->next;
		free_elements(temp->elements);
		free(temp);
	}

	/* Free the overall list */
	p_v = nullptr;
}

int insert_element2(RowNode_handle* pp_r, int pos_row, int pos_col, int val)
{
	/* Initiliaze all local variables */
	RowNode* row = NULL;
	RowNode* prev = NULL;
	RowNode* head = *pp_r;
	int result = 0;

	/* Find if there is a row at the given position */
	row = find_row(pp_r, pos_row);
	/* If not, insert a new row */
	if (!row)
	{
		/* If the data is 0, return out of function */
		if (val == 0) return 0;
		/* Attempt to create new row at given position */
		result = insert_row(pp_r, pos_row, NULL);
		/* Return error if allocation fails */
		if (result != 0) return 1;
		/* Reassign the row pointer to new row */
		row = find_row(pp_r, pos_row);
	}

	/* Insert the given node using the retrieved row */
	result = insert_element(&row->elements, pos_col, val);

	/* See if the whole row has been set to 0 */
	if (row->elements == NULL)
	{
		/* If the row is the first row in the table */
		if (pos_row == 0)
		{
			/* Set the list pointer to the new head and free current head */
			*pp_r = row->next;
			free(row);

			return 0;
		}

		/* Iterate over the list */
		while (head)
		{
			/* If the next row is the new row */
			if (head->next == row)
			{
				/* Set prev accordingly */
				prev = head;
				break;
			}

			/* Continue iteration */
			head = head->next;
		}

		/* Disconnect the row and free it */
		prev->next = row->next;
		free(row);
	}
	return result;
}

RowNode* transpose(ConstRowNode_handle p_v)
{
	/* Initialize local variables */
	RowNode* head = (RowNode*)p_v;
	ElementNode* head_elements = head->elements;
	RowNode* result = NULL;

	/* Iterate over the rows of the main table */
	while (head)
	{
		/* Reset the pointer value */
		head_elements = head->elements;

		/* Iterate through the elements within the row */
		while (head_elements)
		{
			/* Insert the value into the transposed table */
			insert_element2(&result, head_elements->pos, head->pos, head_elements->data);
			/* Move to next element in the row */
			head_elements = head_elements->next;
		}

		/* Move to next row */
		head = head->next;
	}

	return result;
}

RowNode* mult(ConstRowNode_handle p_v1, ConstRowNode_handle p_v2)
{
	/* Initialize all local variables */
	RowNode* head_1 = (RowNode*)p_v1;
	RowNode* transpose_p = transpose(p_v2);
	RowNode* head_2 = transpose_p;
	RowNode* result = NULL;

	/* Create new table to store the new values */
	result = (RowNode*)calloc(1, sizeof(RowNode));

	/* Iterate through the first list */
	while (head_1)
	{
		/* Reset the pointer value */
		head_2 = transpose_p;
		/* Iterate through the second list */
		while (head_2)
		{
			/* Insert the element within the table */
			insert_element2(&result, head_1->pos, head_2->pos, scalar_product(head_1->elements, head_2->elements));

			/* Continue iteration */
			head_2 = head_2->next;
		}

		/* Continue iteration */
		head_1 = head_1->next;
	}

	/* Free the tranposed table */
	free_rows(transpose_p);

	return result;
}

int  determinant(ConstRowNode_handle p_r, int dim) {
	/* print args to get rid of warnings */
	printf("%p\n", (void*)p_r);
	printf("%i\n", dim);
	return 0;
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <stdio.h>  /* printf, NULL   */
#include <stdlib.h>
#include <string.h>
#include <time.h>

int Random(int low, int high);
void sparsevector_test0(void);
void sparsevector_test1(void);
void sparsevector_test2(void);
void sparsevector_test3(void);
void sparsevector_test4(void);
void sparsevector_test5(void);
void sparsevector_test6(void);
void sparsevector_test7(void);
void sparsevector_test8(void);
void sparsevector_test9(void);
void sparsevector_test10(void);
void sparsevector_test11(void);
void sparsevector_test12(void);
void sparsevector_test13(void);
void sparsevector_test14(void);
void sparsevector_test15(void);
void sparsevector_test16(void);
void sparsevector_test17(void);
void sparsevector_test18(void);
void sparsevector_test19(void);
void sparsevector_test20(void);
void sparsevector_test21(void);
void sparsevector_test22(void);
void sparsevector_test23(void);
void sparsevector_test24(void);

int Random(int low, int high) { /* [ low,high ) */
	int r = rand();
	return r % (high - low) + low;
}

void sparsevector_test0(void) {
	ElementNode_handle pE1 = 0;
	int i;
	printf("\n====== INSERTS BACK =============================\n");
	for (i = 0; i < 20; ++i) insert_element(&pE1, 3 * i, i * i);
	printf("vector 1 formatted:\n");
	printf_elements(pE1, "%4d", 80); printf("\n");
	printf("vector 1 raw:\n");
	print_elements(pE1); printf("\n");

	free_elements(pE1);
}

void sparsevector_test1(void) {
	ElementNode_handle pE2 = 0;
	int i;
	printf("\n====== INSERTS BACK =============================\n");

	for (i = 0; i < 20; ++i) insert_element(&pE2, 20 - i, i);
	printf("vector 2 formatted:\n");
	printf_elements(pE2, "%4d", 20); printf("\n");
	printf("vector 2 raw:\n");
	print_elements(pE2); printf("\n");

	free_elements(pE2);
}

void sparsevector_test2(void) {
	ElementNode_handle pE3 = 0;
	int i;
	printf("\n====== INSERTS MIDDLE =============================\n");

	for (i = 0; i < 20; ++i) {
		if (i % 2) insert_element(&pE3, i, i);
		else     insert_element(&pE3, 20 - i, -i);
	}
	printf("vector 3 formatted:\n");
	printf_elements(pE3, "%4d", 20); printf("\n");
	printf("vector 3 raw:\n");
	print_elements(pE3); printf("\n");

	free_elements(pE3);
}

void sparsevector_test3(void) {
	ElementNode_handle pE2 = 0;
	int i;
	printf("\n====== DELETE FRONT ====================\n");
	for (i = 0; i < 5; ++i) insert_element(&pE2, i, i + 1); /*no 0s in the list*/
	printf("vector 2 formatted:\n");
	printf_elements(pE2, "%4d", 5); printf("\n");
	printf("vector 2 raw:\n");
	print_elements(pE2); printf("\n");

	delete_element(&pE2, 0);
	printf("vector 2 formatted:\n");
	printf_elements(pE2, "%4d", 5); printf("\n");
	printf("vector 2 raw:\n");
	print_elements(pE2); printf("\n");

	free_elements(pE2);
}

void sparsevector_test4(void) {
	ElementNode_handle pE2 = 0;
	int i;
	printf("\n====== DELETE BACK ====================\n");
	for (i = 0; i < 5; ++i) insert_element(&pE2, i, i + 1); /*no 0s in the list*/
	printf("vector 2 formatted:\n");
	printf_elements(pE2, "%4d", 5); printf("\n");
	printf("vector 2 raw:\n");
	print_elements(pE2); printf("\n");

	delete_element(&pE2, 4);
	printf("vector 2 formatted:\n");
	printf_elements(pE2, "%4d", 5); printf("\n");
	printf("vector 2 raw:\n");
	print_elements(pE2); printf("\n");

	free_elements(pE2);
}

void sparsevector_test5(void) {
	ElementNode_handle pE2 = 0;
	int i;
	printf("\n====== DELETE MIDDLE ====================\n");
	for (i = 0; i < 5; ++i) insert_element(&pE2, i, i + 1); /*no 0s in the list*/
	printf("vector 2 formatted:\n");
	printf_elements(pE2, "%4d", 5); printf("\n");
	printf("vector 2 raw:\n");
	print_elements(pE2); printf("\n");

	delete_element(&pE2, 2);
	printf("vector 2 formatted:\n");
	printf_elements(pE2, "%4d", 5); printf("\n");
	printf("vector 2 raw:\n");
	print_elements(pE2); printf("\n");

	free_elements(pE2);
}


void sparsevector_test6(void) {
	ElementNode_handle pE1 = 0, pE2 = 0, pE3 = 0;
	int i;
	printf("\n==== SCALAR MULTIPLICATION =================\n");
	for (i = 0; i < 20; ++i) insert_element(&pE1, i, i * i);
	printf("vector pE1 formatted:\n");
	printf_elements(pE1, "%4d", 20); printf("\n");

	for (i = 0; i < 20; ++i) insert_element(&pE2, 40 - 2 * i, i);
	printf("vector pE2 formatted:\n");
	printf_elements(pE2, "%4d", 40); printf("\n");

	for (i = 0; i < 20; ++i) {
		if (i % 2) insert_element(&pE3, 2 * i, i);
		else     insert_element(&pE3, 41 - 2 * i, -i);
	}
	printf("vector pE3 formatted:\n");
	printf_elements(pE3, "%4d", 40); printf("\n");
	printf("scalar product pE1 and pE2 = %i\n", scalar_product(pE1, pE2));
	printf("scalar product pE2 and pE3 = %i\n", scalar_product(pE2, pE3));
	printf("scalar product pE3 and pE1 = %i\n", scalar_product(pE3, pE1));
	printf("scalar product pE1 and pE1 = %i\n", scalar_product(pE1, pE1));

	free_elements(pE1);
	free_elements(pE2);
	free_elements(pE3);
}

void sparsevector_test7(void) {
	ElementNode_handle pE1 = 0, pE2 = 0;
	int i;
	printf("\n==== SCALAR MULTIPLICATION 2 =================\n");
	for (i = 0; i < 20; ++i) insert_element(&pE1, 1000 * i, i * i);

	for (i = 0; i < 20; ++i) insert_element(&pE2, 500 * i, i);

	printf("scalar product pE1 and pE2 = %i\n", scalar_product(pE1, pE2));
	printf("scalar product pE2 and pE1 = %i\n", scalar_product(pE2, pE1));
	printf("scalar product pE1 and pE1 = %i\n", scalar_product(pE1, pE1));

	free_elements(pE1);
	free_elements(pE2);
}

void sparsevector_test8(void) {
	ElementNode_handle pE1 = 0, pE2 = 0, pE3 = 0, pRes;
	int i;
	printf("\n==== VECTOR ADDITION =================\n");
	for (i = 0; i < 20; ++i) insert_element(&pE1, i, i * i);
	printf("vector pE1 formatted:\n");
	printf_elements(pE1, "%4d", 20); printf("\n");

	for (i = 0; i < 10; ++i) insert_element(&pE2, 20 - 2 * i, i);
	printf("vector pE2 formatted:\n");
	printf_elements(pE2, "%4d", 20); printf("\n");

	for (i = 0; i < 5; ++i) {
		if (i % 2) insert_element(&pE3, 4 * i, i);
		else     insert_element(&pE3, 21 - 4 * i, -i);
	}
	printf("vector pE3 formatted:\n");
	printf_elements(pE3, "%4d", 20); printf("\n");

	pRes = add(pE1, pE2);
	printf("pE1 + pE2 = \n"); printf_elements(pRes, "%4d", 20); printf("\n");
	/* printf("raw:\n"); print_elements(pRes); printf("\n"); */
	free_elements(pRes);
	pRes = add(pE2, pE3);
	printf("pE2 + pE3 = \n"); printf_elements(pRes, "%4d", 20); printf("\n");
	/* printf("raw:\n"); print_elements(pRes); printf("\n"); */
	free_elements(pRes);
	pRes = add(pE3, pE1);
	printf("pE3 + pE1 = \n"); printf_elements(pRes, "%4d", 20); printf("\n");
	/* printf("raw:\n"); print_elements(pRes); printf("\n"); */
	free_elements(pRes);
	pRes = add(pE1, pE3);
	printf("pE1 + pE3 = \n"); printf_elements(pRes, "%4d", 20); printf("\n");
	/* printf("raw:\n"); print_elements(pRes); printf("\n"); */
	free_elements(pRes);
	free_elements(pE1);
	free_elements(pE2);
	free_elements(pE3);
}

void sparsevector_test9(void) {
	ElementNode_handle pE1 = 0, pE2 = 0, pRes;
	int i;
	printf("\n==== VECTOR ADDITION 2 =================\n");
	for (i = 0; i < 5; ++i) insert_element(&pE1, 2000 * i, i);

	for (i = 0; i < 10; ++i) insert_element(&pE2, 1000 * i, i);


	pRes = add(pE1, pE2);
	printf("pE1 + pE2 = \n");
	printf("raw:\n"); print_elements(pRes); printf("\n");
	free_elements(pRes);

	pRes = add(pE2, pE1);
	printf("pE2 + pE1 = \n");
	printf("raw:\n"); print_elements(pRes); printf("\n");
	free_elements(pRes);

	pRes = add(pE1, pE1);
	printf("pE1 + pE1 = \n");
	printf("raw:\n"); print_elements(pRes); printf("\n");
	free_elements(pRes);

	free_elements(pE1);
	free_elements(pE2);
}

void sparsevector_test10(void) {
	ElementNode_handle pE1 = 0, pE2 = 0, pRes;
	int i;
	printf("\n==== VECTOR ADDITION (ZEROES) ========\n");
	for (i = 0; i < 20; ++i) insert_element(&pE1, i, i * i);
	printf("vector pE1 formatted:\n");
	printf_elements(pE1, "%4d", 20); printf("\n");

	for (i = 0; i < 20; ++i) insert_element(&pE2, i, -i * i);
	printf("vector pE2 formatted:\n");
	printf_elements(pE2, "%4d", 20); printf("\n");


	pRes = add(pE1, pE2);
	printf("pE1 + pE2 = \n"); printf_elements(pRes, "%4d", 20); printf("\n");
	printf("raw:\n"); print_elements(pRes); printf("\n");
	free_elements(pRes);
	pRes = add(pE2, pE1);
	printf("pE2 + pE1 = \n"); printf_elements(pRes, "%4d", 20); printf("\n");
	printf("raw:\n"); print_elements(pRes); printf("\n");
	free_elements(pRes);
	free_elements(pE1);
	free_elements(pE2);
}

void sparsevector_test11(void) {
	ElementNode_handle pE1 = 0;
	int i;
	printf("\n==== sparsevector_testING FIND ========\n");
	for (i = 0; i < 20; i += 2) insert_element(&pE1, i, i * i);
	printf_elements(pE1, "%4d", 20); printf("\n");
	printf("print vector using get - very inefficient!!!\n");
	for (i = 0; i < 20; ++i) printf("index %i, value %i\n", i, get(pE1, i));
	free_elements(pE1);
}

void sparsevector_test12(void) {
	ElementNode_handle pE1 = 0;
	int pos = 10000;
	printf("\n==== sparsevector_testING FIND 2 ========\n");
	insert_element(&pE1, pos, 5);
	print_elements(pE1); printf("\n");

	printf("value %i at position %i\n", get(pE1, pos), pos);
	free_elements(pE1);
}

void sparsevector_test13(void) {
	ElementNode_handle pE1 = 0, pE2 = 0, pE3 = 0;
	RowNode_handle pR = 0;
	int i;
	printf("\n====== INSERT ROWS FRONT =========================\n");
	for (i = 0; i < 5; ++i) insert_element(&pE1, 2 * i, 10 * i - 50);
	printf_elements(pE1, "%4d", 10); printf("\n");

	for (i = 0; i < 20; ++i) insert_element(&pE2, 20 - i, i);
	printf_elements(pE2, "%4d", 10); printf("\n");

	for (i = 0; i < 10; ++i) {
		if (i % 2) insert_element(&pE3, i, i);
		else     insert_element(&pE3, 10 - i, -i);
	}

	insert_row(&pR, 8, pE3);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");
	insert_row(&pR, 5, pE2);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");
	insert_row(&pR, 0, pE1);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");

	free_rows(pR);
}

void sparsevector_test14(void) {
	ElementNode_handle pE1 = 0, pE2 = 0, pE3 = 0;
	RowNode_handle pR = 0;
	int i;
	printf("\n====== INSERT ROWS BACK =========================\n");
	for (i = 0; i < 5; ++i) insert_element(&pE1, 2 * i, 10 * i - 50);
	printf_elements(pE1, "%4d", 10); printf("\n");

	for (i = 0; i < 20; ++i) insert_element(&pE2, 20 - i, i);
	printf_elements(pE2, "%4d", 10); printf("\n");

	for (i = 0; i < 10; ++i) {
		if (i % 2) insert_element(&pE3, i, i);
		else     insert_element(&pE3, 10 - i, -i);
	}

	insert_row(&pR, 1, pE3);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");
	insert_row(&pR, 5, pE2);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");
	insert_row(&pR, 7, pE1);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");

	free_rows(pR);
}

void sparsevector_test15(void) {
	ElementNode_handle pE1 = 0, pE2 = 0, pE3 = 0;
	RowNode_handle pR = 0;
	int i;
	printf("\n====== INSERT ROWS MIDDLE =========================\n");
	for (i = 0; i < 5; ++i) insert_element(&pE1, 2 * i, 10 * i - 50);
	printf_elements(pE1, "%4d", 10); printf("\n");

	for (i = 0; i < 20; ++i) insert_element(&pE2, 20 - i, i);
	printf_elements(pE2, "%4d", 10); printf("\n");

	for (i = 0; i < 10; ++i) {
		if (i % 2) insert_element(&pE3, i, i);
		else     insert_element(&pE3, 10 - i, -i);
	}

	insert_row(&pR, 8, pE3);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");
	insert_row(&pR, 0, pE1);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");
	insert_row(&pR, 5, pE2);
	printf("after insert:\n"); printf_rows(pR, "%4i", 10); printf("\n");

	free_rows(pR);
}

void sparsevector_test16(void) {
	RowNode_handle pR = 0;
	int i, j;
	printf("\n====== INSERT ELEMENTS IN ROW LIST =========\n");

	for (i = 0; i < 15; ++i) {
		for (j = 0; j < 10; ++j) {
			insert_element2(&pR, 2 * i, 3 * j, 1 + 7 * ((i + j) % 2));
		}
	}
	printf_rows(pR, "%i", 30);
	printf("\n");
	print_rows(pR);
	free_rows(pR);
}

void sparsevector_test17(void) {
	RowNode_handle pR = 0;
	int i, j;
	printf("\n====== INSERT ELEMENTS IN ROW LIST (WITH ZEROES) ===\n");

	for (i = 0; i < 30; ++i) {
		for (j = 0; j < 30; ++j) {
			insert_element2(&pR, i, j, (i + j) % 2);
		}
	}
	printf_rows(pR, "%i", 30); printf("\n");
	print_rows(pR); printf("\n");
	free_rows(pR);
}

void sparsevector_test18(void) {
	RowNode_handle pR = 0;
	int i, j;
	printf("\n== INSERT ELEMENTS IN ROW LIST (WITH ZEROES AND OVERWRITES) ==\n");

	for (i = 0; i < 30; ++i) {
		for (j = 0; j < 30; ++j) {
			insert_element2(&pR, i, j, (i + j) % 2);
		}
	}
	printf_rows(pR, "%i", 30); printf("\n");
	/* overwrite some */
	for (i = 0; i < 10; ++i) {
		for (j = 0; j < 10; ++j) {
			insert_element2(&pR, 3 * i, 3 * j, (3 * i + 2 * j) % 5);
		}
	}
	/* overwrite with 0 */
	for (i = 0; i < 30; ++i) {
		for (j = 0; j < 30; ++j) {
			if ((i + j) % 8 == 0) insert_element2(&pR, i, j, 0);
		}
	}
	printf_rows(pR, "%i", 30); printf("\n");
	print_rows(pR);
	printf("\n");
	free_rows(pR);
}

void sparsevector_test19(void) {
	RowNode_handle pR = 0;
	int i, j = 0;
	printf("\n== OVERWRITE ENTIRE ROW 0 WITH ZEROES ==\n");

	for (i = 0; i < 10; ++i) {
		for (j = 0; j < 10; ++j) {
			insert_element2(&pR, i, j, (i + j) % 3);
		}
	}
	printf_rows(pR, "%i", 10); printf("\n");

	insert_element2(&pR, 3 * i, 3 * j, (3 * i + 2 * j) % 5);

	insert_element2(&pR, 0, 0, 0);
	insert_element2(&pR, 0, 1, 0);
	insert_element2(&pR, 0, 2, 0);
	insert_element2(&pR, 0, 3, 0);
	insert_element2(&pR, 0, 4, 0);
	insert_element2(&pR, 0, 5, 0);
	insert_element2(&pR, 0, 6, 0);
	insert_element2(&pR, 0, 7, 0);
	insert_element2(&pR, 0, 8, 0);
	insert_element2(&pR, 0, 9, 0);

	printf_rows(pR, "%i", 10); printf("\n");
	print_rows(pR);
	printf("\n");
	free_rows(pR);
}

void sparsevector_test20(void) {
	RowNode_handle pR = 0;
	int i, j = 0;
	printf("\n== OVERWRITE ENTIRE ROW 1 WITH ZEROES ==\n");

	for (i = 0; i < 10; ++i) {
		for (j = 0; j < 10; ++j) {
			insert_element2(&pR, i, j, (i + j) % 3);
		}
	}
	printf_rows(pR, "%i", 10); printf("\n");

	insert_element2(&pR, 3 * i, 3 * j, (3 * i + 2 * j) % 5);

	insert_element2(&pR, 1, 0, 0);
	insert_element2(&pR, 1, 1, 0);
	insert_element2(&pR, 1, 2, 0);
	insert_element2(&pR, 1, 3, 0);
	insert_element2(&pR, 1, 4, 0);
	insert_element2(&pR, 1, 5, 0);
	insert_element2(&pR, 1, 6, 0);
	insert_element2(&pR, 1, 7, 0);
	insert_element2(&pR, 1, 8, 0);
	insert_element2(&pR, 1, 9, 0);

	printf_rows(pR, "%i", 10); printf("\n");
	print_rows(pR);
	printf("\n");
	free_rows(pR);
}

void sparsevector_test21(void) {
	RowNode_handle pR = 0;
	int i, j = 0;
	printf("\n== OVERWRITE ENTIRE ROW 9 WITH ZEROES ==\n");

	for (i = 0; i < 10; ++i) {
		for (j = 0; j < 10; ++j) {
			insert_element2(&pR, i, j, (i + j) % 3);
		}
	}
	printf_rows(pR, "%i", 10); printf("\n");

	insert_element2(&pR, 3 * i, 3 * j, (3 * i + 2 * j) % 5);

	insert_element2(&pR, 9, 0, 0);
	insert_element2(&pR, 9, 1, 0);
	insert_element2(&pR, 9, 2, 0);
	insert_element2(&pR, 9, 3, 0);
	insert_element2(&pR, 9, 4, 0);
	insert_element2(&pR, 9, 5, 0);
	insert_element2(&pR, 9, 6, 0);
	insert_element2(&pR, 9, 7, 0);
	insert_element2(&pR, 9, 8, 0);
	insert_element2(&pR, 9, 9, 0);

	printf_rows(pR, "%i", 10); printf("\n");
	print_rows(pR);
	printf("\n");
	free_rows(pR);
}

void sparsevector_test22(void) {
	RowNode_handle pR = 0, pR2;
	int i, j;
	printf("\n===== TRANSPOSITION ========================\n");
	for (i = 0; i < 10; ++i) {
		for (j = 0; j < 10; ++j) {
			if ((i + j) % 4) insert_element2(&pR, 1000 * i, 1000 * j, 10 * i + j);
		}
	}
	pR2 = transpose(pR);

	printf("original as list of rows\n");
	print_rows(pR);
	printf("\n");
	printf("transposed as list of rows\n");
	print_rows(pR2);
	printf("\n");

	free_rows(pR);
	free_rows(pR2);
}

void sparsevector_test23(void) {
	RowNode_handle pR1 = 0, pR2 = 0, pR3, pR4, pR5;
	int i;
	printf("\n===== MULTIPLICATION =======================\n");
	for (i = 0; i < 7; ++i) insert_element2(&pR1, i, i, 1);
	insert_element2(&pR1, 0, 6, 1);
	printf("Matrix 1\n");
	printf_rows(pR1, "%i ", 7); printf("\n");

	for (i = 0; i < 7; ++i) insert_element2(&pR2, i, i, 1);
	insert_element2(&pR2, 0, 6, 1);
	printf("Matrix 2\n");
	printf_rows(pR2, "%i ", 7); printf("\n");

	pR3 = mult(pR1, pR2);
	printf("Power 2\n");
	printf_rows(pR3, "%i ", 7); printf("\n");

	pR4 = mult(pR1, pR3);
	printf("Power 3\n");
	printf_rows(pR4, "%i ", 7); printf("\n");


	pR5 = mult(pR4, pR4);
	printf("Power 6\n");
	printf_rows(pR5, "%i ", 7); printf("\n");
	/*
	 for(i=0;i<1;++i) {
	   printf("Power %d\n",i+3);
	   temp=pR3;
	   pR3=mult(pR1,pR3);
	   free(temp);
	   printf_rows(pR3,"%i ",7);
	   printf("\n");
	 }
	*/

	free_rows(pR1);
	free_rows(pR2);
	free_rows(pR3);
	free_rows(pR4);
	free_rows(pR5);
}

void sparsevector_test24(void) {
	RowNode_handle pR1 = 0;
	int i, j;
	printf("\n===== DETERMINANT ==========================\n");
	for (i = 0; i < 7; ++i) {
		insert_element2(&pR1, i, i, 2);
		for (j = i + 1; j < 7; ++j) {
			insert_element2(&pR1, i, j, i + j);
		}
	}

	printf("Matrix \n");
	printf_rows(pR1, "%i ", 7); printf("\n");
	printf("Determinant = %i\n", determinant(pR1, 7));
	free_rows(pR1);

	pR1 = 0;
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			int s = i + j;
			if (s == 2 || s == 3 || s == 7 || s == 8) insert_element2(&pR1, i, j, 1);
		}
	}

	printf("Matrix \n");
	printf_rows(pR1, "%i ", 5); printf("\n");
	printf("Determinant = %i\n", determinant(pR1, 5));
	free_rows(pR1);
}
