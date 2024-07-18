/*!****************************************************************************
\file    List.h
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab (Assignment) 04
\date    02-04-2021

\brief
  This program houses functions necessary to create, analyize, and maintain
  a linked list of numbers within a class List

  The functions include:

  - List
      Default Constructor - sets all class variables to 0

  - List
      Non-default Constructor - creates a linked list with the values held
      within an array

  - List
      Copy Constructor - copies over the values from a given linked list

  - ~List
      Default Destructor - deletes the dynamic memory associated with the class

  - push_back
      Adds given value to the back of the array, updating the class tail_
      pointer

  - push_front
      Adds given value to the front of the linked list, updating the class
      head_ pointer

  - pop_front
      Removes the head_ node and updates the head_ pointer

  - front
      Returns the value stored in the head_ node

  - size
      Returns how many nodes are within the list

  - empty
      Returns whether or not the list is empty

  - clear
      Removes all of the nodes within the list

  - operator=
      Returns the address of the list than a variable is going to be set to

  - operator+=
      Returns the address of the list that has had another list appended to it

  - operator+
      Returns a new list that contains the two list appended to each other

  - operator[]
      Returns the value stored in the node at the given position in the list

  - operator[]
      Returns the value stored in the node at the given position in the list

  - operator<<
      Outputs the given list into the given output stream

  - list_count
      Returns the number of lists that have been created

  - node_count
      Returns the number of nodes active within the given list

  - new_node
      Allocates a new node with the given value

******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
#ifndef LIST_H
#define LIST_H
///////////////////////////////////////////////////////////////////////////////

#include <iostream> // ostream

namespace CS170
{
  /*!************************************************************************
  \brief
    List Class
  **************************************************************************/
  class List
  {
  public:
    /*!************************************************************************
    \brief
      Default Constructor - sets all class variables to 0
    **************************************************************************/
    List();

    /*!************************************************************************
    \brief
      Non-default Constructor - creates a linked list with the values held
      within an array

    \param array
      Array whose values are going to be copied into a linked list

    \param size
      The amount of numbers within the given array
    **************************************************************************/
    List(const int array[], int size);


    /*!************************************************************************
    \brief
      Copy Constructor - copies over the values from a given linked list

    \param rhs
      The list that is being copied into a new list
    **************************************************************************/
    List(const List& rhs);

    /*!************************************************************************
    \brief
      Default Destructor - deletes the dynamic memory associated with the class
    **************************************************************************/
    ~List();

    /*!************************************************************************
    \brief
      Adds given value to the front of the linked list, updating the class
      head_ pointer

    \param value
      Value that is being stored in the new node
    **************************************************************************/
    void push_front(int value);

    /*!************************************************************************
    \brief
      Adds given value to the back of the array, updating the class tail_
      pointer

    \param value
      Value that is being stored in the new node
    **************************************************************************/
    void push_back(int value);

    /*!************************************************************************
    \brief
      Removes the head_ node and updates the head_ pointer
    **************************************************************************/
    void pop_front();

    /*!************************************************************************
    \brief
      Returns the value stored in the head_ node

    \return
      The value stored in the head_ node
    **************************************************************************/
    int front() const;

    /*!************************************************************************
    \brief
      Returns how many nodes are within the list

    \return
      The number of nodes within the list
    **************************************************************************/
    int size() const;

    /*!************************************************************************
    \brief
      Returns whether or not the list is empty

    \return
      Bool indicating whether or not the list is empty
    **************************************************************************/
    bool empty() const;

    /*!************************************************************************
    \brief
      Removes all of the nodes within the list
    **************************************************************************/
    void clear();

    /*!************************************************************************
    \brief
      Returns the address of the list than a variable is going to be set to

    \param rhs
      The list that is being copied to be have a variable set equal to it

    \return
      The address of the copied list
    **************************************************************************/
    List& operator= (const List& rhs);

    /*!************************************************************************
    \brief
      Returns the address of the list that has had another list appended to it

    \param rhs
      The list that is being appended onto the current list

    \return
      The address of the larger list
    **************************************************************************/
    List& operator+= (const List& rhs);

    /*!************************************************************************
    \brief
      Returns a new list that contains the two list appended to each other

    \param rhs
      The second list that is being appended onto the new list

    \param lhs
      The first list that is being appended onto the new list

    \return
      The new list
    **************************************************************************/
    List operator+(const List& rhs);

    /*!************************************************************************
    \brief
      Returns the value stored in the node at the given position in the list

    \param index
      The position that is trying to be accessed

    \return
      The value stored at the current position
    **************************************************************************/
    const int& operator[](int index) const;

    /*!************************************************************************
    \brief
      Returns the value stored in the node at the given position in the list

    \param index
      The position that is trying to be accessed

    \return
      The value stored at the current position
    **************************************************************************/
    int& operator[](int index);

    /*!************************************************************************
    \brief
      Outputs the given list into the given output stream

    \param os
      The output stream that the list is being printed to

    \param list
      The list that is being outputted

    \return
      The output stream containing the outputted stream
    **************************************************************************/
    friend std::ostream& operator<<(std::ostream& os, const List& list);

    /*!************************************************************************
    \brief
      Returns the number of lists that have been created

    \return
      The number of lists created
    **************************************************************************/
    static int list_count();

    /*!************************************************************************
    \brief
      Returns the number of nodes active within the given list

    \return
      The number of nodes active
    **************************************************************************/
    static int node_count();

  private:

    /*!************************************************************************
    \brief
      Node Class
    **************************************************************************/
    struct Node
    {
      /*!**********************************************************************
      \brief
        Non-Default Constructor - sets the value within the node to given int

      \param value
        Value to be stored within the new node
      ************************************************************************/
      Node(int value);   

      /*!**********************************************************************
      \brief
        Default Constructor - Destroys node
      ************************************************************************/
      ~Node();                

      /*!**********************************************************************
      \brief
        Pointer to next Node
      ************************************************************************/
      Node* next;            

      /*!**********************************************************************
      \brief
        Value stored within the node
      ************************************************************************/
      int data;    

      /*!**********************************************************************
      \brief
        Count of active nodes
      ************************************************************************/
      static int nodes_alive; 
    };

    /*!************************************************************************
    \brief
      Pointer to the first node within the list
    **************************************************************************/
    Node* head_;

    /*!************************************************************************
    \brief
      Pointer to the last node within the list
    **************************************************************************/
    Node* tail_;

    /*!************************************************************************
    \brief
      Number of nodes within the list
    **************************************************************************/
    int size_;  

    /*!************************************************************************
    \brief
      Number of lists created
    **************************************************************************/
    static int list_count_;

    /*!************************************************************************
    \brief
      Allocates new node with the given value

    \param data
      The value to be stored within the new node

    \return
      The new node
    **************************************************************************/
    Node* new_node(int data) const;
  };

} // namespace CS170

#endif
///////////////////////////////////////////////////////////////////////////////
