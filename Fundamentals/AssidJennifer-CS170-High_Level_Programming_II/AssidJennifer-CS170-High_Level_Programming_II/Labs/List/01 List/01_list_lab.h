/*!****************************************************************************
\file    List.h
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab 05
\date    20-02-2021

\brief
  This program houses functions necessary to create, analyize, and maintain
  linked lists

  The functions include:

  - List
      Assigns initial value to the head_ pointer for the list

  - ~List
      This function frees the memory associated with each node within the
      given list

  - MakeNode
      This function dynamically allocates a Node and initializes the
      associated values (number / next) to 0 / NULL.

  - PrintList
      This function prints out the data stored within the nodes in a given
      list

  - Count
      Get the number of nodes in the list.

  - AddToEnd
      This function adds a node with the given value to the tail of the
      given list

  - AddToFront
      This function adds a node with the given data to the head of the
      given list

  - FindItem
      This function finds if there is a node containing the given value
      within the given list

  - Insert
      This function inserts a node with the given value at the given
      position within the given list
******************************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////////////
#ifndef LIST_LAB_1_H
#define LIST_LAB_1_H
////////////////////////////////////////////////////////////////////////////////

//! namespace CS170
namespace CS170
{
    //! namespace ListLab
    namespace ListLab_1_NM
    {
        //! Node structure for List class
        struct Node
        {

            int number; //!< data portion
            Node* next; //!< pointer portion 

            /*!*******************************************************************
              Conversion constructor

              \param value
                The data for the node
            *********************************************************************/
            Node(int value)
            {
                number = value;
                next = 0;
            }
        };

        //! Single-linked list class for CS170 labs
        class ListLab_1
        {
        public:
            // Default constructor
            ListLab_1();

            // Destructor
            ~ListLab_1();

            void PrintList() const;

            int Count() const;

            void AddToEnd(int value);

            void AddToFront(int value);

            Node* FindItem(int value);

            void Insert(int value, int position);

        private:
            Node* head_;  //!< Pointer to the front of the list
            Node* MakeNode(int value) const;

            // Any other private helper methods you need...
        };
    } // namespace ListLab
} // namespace CS170


void list_lab_1(void);

#endif // LIST_H