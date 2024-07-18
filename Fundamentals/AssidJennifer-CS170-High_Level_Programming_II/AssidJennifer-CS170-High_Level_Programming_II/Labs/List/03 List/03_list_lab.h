#pragma once
/*!****************************************************************************
\file    List.h
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab 07
\date    06-03-2021

\brief
  This program houses functions necessary to create, analyize, and maintain
  linked lists

  The functions include:

  - List
      This function assigns initial value to the head_ pointer for the list

  - ~List
      This function frees the memory associated with each node within the
      given list

  - Merge
      Merges two sorted lists together into one list that is then made into
      the class list

  - AddToEnd
      This function adds a node with the given value to the tail of the
      given list

  - AddToFront
      This function adds a node with the given data to the head of the
      given list

  - Count
      This function get the number of nodes in the list.

  - FindItem
      This function finds if there is a node containing the given value
      within the given list

  - Insert (value, position)
      This function inserts a node with the given value at the given
      position within the given list

  - Delete (value)
      This function deletes the first instance of the given value in the class
      list

  - Concat
      This function appends the given list to the class list

  - Delete (list)
      This function deletes all occurences of the values in the given list from
      the class list

  - Insert (value)
      This function places the given value into the linked list in the order
      of smallest to largest

  - MakeNode
      This function dynamically allocates a Node and initializes the
      associated values (number / next) to 0 / NULL.

******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
#ifndef LIST_LAB_3_H
#define LIST_LAB_3_H
///////////////////////////////////////////////////////////////////////////////
#include <iostream>

namespace CS170
{
    namespace ListLab_3_NM
    {
        /*!**********************************************************************
          \brief
            The struct of a linked list node
        ************************************************************************/
        struct Node
        {
            /*!**********************************************************************
            \brief
              Data within the node
          ************************************************************************/
            int number; // data portion

            /*!**********************************************************************
            \brief
              Address to the next node within the list
          ************************************************************************/
            Node* next; // pointer to next node in list

            /*!**********************************************************************
            \brief
              Conversion constructor for a node

              \param value
                The data to be contained in the new node
          ************************************************************************/
            Node(int value)
            {
                number = value;
                next = 0;
            }
        };

        /*!**********************************************************************
          \brief
            The class of a linked list
        ************************************************************************/
        class ListLab_3
        {
        public:
            /*!**********************************************************************
              \brief
                This function assigns initial value to the head_ pointer for the list
            ************************************************************************/
            ListLab_3();

            /*!**********************************************************************
              \brief
                This function frees the memory associated with each node within the
                given list
            ************************************************************************/
            ~ListLab_3();

            /*!**********************************************************************
            \brief
              Merges two sorted lists together into one list that is then made into
              the class list

            \param rhs
              The list that will be sorted into the class list
          **************************************************************************/
            void Merge(ListLab_3& rhs);

            /*!**********************************************************************
              \brief
               This function adds a node with the given value to the tail of the
               given list

             \param value
                Data to be stored in the new node
            ************************************************************************/
            void AddToEnd(int value);

            /*!**********************************************************************
           \brief
             This function adds a node with the given data to the head of the
             given list

           \param value
             Data to be stored in the new node
         ***************************************************************************/
            void AddToFront(int value);

            /*!**********************************************************************
            \brief
              Get the number of nodes in the list.

            \return
              The number of nodes in the list.
          **************************************************************************/
            int Count() const;

            /*!**********************************************************************
           \brief
             This function finds if there is a node containing the given value
             within the given list

           \param value
             Data to be stored in the new node

           \return
             Pointer to the node that contains the given value
         ***************************************************************************/
            Node* FindItem(int value) const;

            /*!**********************************************************************
            \brief
              This function inserts a node with the given value at the given
              position within the given list

            \param value
              Data to be stored in the new node

            \param position
              Position where the node will be stored within the list
          **************************************************************************/
            void Insert(int value, int position);

            /*!**********************************************************************
            \brief
              This function deletes the first instance of the node with the given
              data

            \param value
              Data to be deleted in the new node
          **************************************************************************/
            void Delete(int value);

            /*!**********************************************************************
            \brief
              This function appends the values in the given list to the class list

            \param Source
          **************************************************************************/
            void Concat(const ListLab_3& Source);

            /*!**********************************************************************
            \brief
              This function deletes all instances of the values in the given list
              in the class list

            \param Items
          **************************************************************************/
            void Delete(const ListLab_3& Items);

            /*!**********************************************************************
            \brief
              This function inserts a node with the given value in the list in the
              order of smallest value to largest value

            \param value
              Data to be stored in the new node
          **************************************************************************/
            void Insert(int value);

            /*!**********************************************************************
            \brief
              This function prints out the data stored within the nodes in a given
              list

              \param os
                The output stream which the list is being printed to

              \param rhs
                The list being printed

              \return
                The output stream that houses the printed out linked list
          **************************************************************************/
            friend std::ostream& operator<<(std::ostream& os, const ListLab_3& rhs);

        private:
            /*!**********************************************************************
            \brief
              The head node of the linked list
          ************************************************************************/
            Node* head_;

            /*!**********************************************************************
            \brief
              This function dynamically allocates a Node and initializes the
              associated values (number / next) to 0 / NULL.

            \param value
              Data that will be stored within the new node

            \return
              Pointer to the newly created node
          **************************************************************************/
            Node* MakeNode(int value) const;

            // Other private stuff ...
        };
    } // namespace ListLab
} // namespace CS170

void list_lab_3(void);

#endif // LIST_H