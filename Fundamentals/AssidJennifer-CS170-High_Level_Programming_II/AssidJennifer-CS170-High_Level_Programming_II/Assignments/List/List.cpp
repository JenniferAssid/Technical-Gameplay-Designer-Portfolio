/*!****************************************************************************
\file    List.cpp
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
#include <iostream> // ostream, endl
#include <iomanip>  // setw
#include "List.h"   // List class

namespace CS170
{
  /*!************************************************************************
  \brief
    Number of lists created (initialized to 0)
  **************************************************************************/
  int List::list_count_ = 0;

  /*!************************************************************************
  \brief
    Number of nodes alive (initialized to 0)
  **************************************************************************/
  int List::Node::nodes_alive = 0;

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
     /*!**********************************************************************
      \brief
        Non-Default Constructor - sets the value within the node to given int

      \param value
        Value to be stored within the new node
      ************************************************************************/
  List::Node::Node(int value)
  {
    data = value;
    next = NULL;
    //nodes_alive++;
  }

      /*!**********************************************************************
      \brief
        Default Constructor - Destroys node
      ************************************************************************/
  List::Node::~Node()
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
    /*!************************************************************************
    \brief
      Default Constructor - sets all class variables to 0
    **************************************************************************/
  List::List() 
  { 
    // Setting variables to 0 as default
    list_count_++;

    head_ = 0; 
    tail_ = 0; 
    size_ = 0;
    
  }

  /*!************************************************************************
   \brief
     Non-default Constructor - creates a linked list with the values held
     within an array

   \param array
     Array whose values are going to be copied into a linked list

   \param size
     The amount of numbers within the given array
   **************************************************************************/
  List::List(const int array[], int size)
  {
    // Setting class variables to default positions
    head_ = tail_ = NULL;
    size_ = 0;

    // Iterates across the array and adding the data at the index to the list
    for (int i = 0; i < size; i++)
      push_back(array[i]);

    // Incrementing the list count
    list_count_++;
  }

  /*!************************************************************************
    \brief
      Copy Constructor - copies over the values from a given linked list

    \param rhs
      The list that is being copied into a new list
    **************************************************************************/
  List::List(const List& rhs)
  {
    // Increment the list count
    list_count_++;

    // Set the class variables to their default values
    head_ = tail_ = NULL;
    size_ = 0;

    // Iterate over the list and copy over the values
    Node* node = rhs.head_;

    while (node)
    {
      push_back(node->data);
      node = node->next;
    }

    size_ = rhs.size_;
  }

  /*!************************************************************************
    \brief
      Default Destructor - deletes the dynamic memory associated with the class
    **************************************************************************/
  List::~List()
  {
    clear();
  }

  /*!************************************************************************
    \brief
      Returns the address of the list than a variable is going to be set to

    \param rhs
      The list that is being copied to be have a variable set equal to it

    \return
      The address of the copied list
    **************************************************************************/
  List& List::operator= (const List& rhs)
  {
    // Checks if the given list is the class list
    if (this == &rhs) return *this;

    // Clear the existing nodes
    this->clear();

    // Setting the values to their default positions
    size_ = 0;
    head_ = tail_ = NULL;

    // Set the values of the class list to the values in the given list
    for (int i = 0; i < rhs.size_; i++)
      push_back(rhs[i]);

    // Return the class list
    return *this;
  }

  /*!************************************************************************
    \brief
      Returns the address of the list that has had another list appended to it

    \param rhs
      The list that is being appended onto the current list

    \return
      The address of the larger list
    **************************************************************************/
  List& List::operator+= (const List& rhs)
  {
    // Iterate over the given list and append its values to the class list
    Node* node = rhs.head_;

    while (node)
    {
      push_back(node->data);
      node = node->next;
    }

    // Return the class list
    return *this;
  }

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
  List List::operator+(const List& rhs)
  {
    // Create a new list
    List temp(*this);

    // Append the given list to the new list
    temp += rhs;

    // Returnn the new list
    return temp;
  }

  /*!************************************************************************
    \brief
      Returns the value stored in the node at the given position in the list

    \param index
      The position that is trying to be accessed

    \return
      The value stored at the current position
    **************************************************************************/
  const int& List::operator[](int index) const
  {
    // Iterate over the list until desired index
    Node* node = head_;

    for (int i = 0; i < index; i++)
      node = node->next;

    // Return the data at the given node
    return node->data;
  }

  /*!************************************************************************
    \brief
      Returns the value stored in the node at the given position in the list

    \param index
      The position that is trying to be accessed

    \return
      The value stored at the current position
    **************************************************************************/
  int& List::operator[](int index)
  {
    // Iterate over the list until desired index
    Node* node = head_;

    for (int i = 0; i < index; i++)
      node = node->next;

    // Return the data at the given node
    return node->data;
  }

  /*!************************************************************************
    \brief
      Adds given value to the front of the linked list, updating the class
      head_ pointer

    \param value
      Value that is being stored in the new node
    **************************************************************************/
  void List::push_front(int value)
  {
    // Create a new node with the given value
    Node* node = new_node(value);
    // Link the node to the class list
    node->next = head_;

    // Reassign the head pointer
    head_ = node;

    // Checks if pointer needs to be reassigned
    if (size_ == 0)
      tail_ = node;

    // Increment the size
    size_++;
  }

  /*!************************************************************************
    \brief
      Adds given value to the back of the array, updating the class tail_
      pointer

    \param value
      Value that is being stored in the new node
    **************************************************************************/
  void List::push_back(int value)
  {
    Node* head = head_;

    // Create new node with the given value
    Node* node = new_node(value);
    node->next = NULL;

    // If the list is empty, set the tail and head to point to the new node
    if (head == NULL) 
    { 
      head_ = node;
      size_++;
      return; }

    // Set the tail pointer to be the new node
    tail_ = node;

    // Iterate over the list until at the end
    while (head->next)
      head = head->next;

    // Link the new node to the list
    head->next = node;

    // Increment the size
    size_++;
  }

  /*!************************************************************************
    \brief
      Removes the head_ node and updates the head_ pointer
    **************************************************************************/
  void List::pop_front()
  {
    if (head_ == NULL) return;

    // Set new node to be value of next node
    Node* node = head_->next;

    // Delete the current head
    delete head_;

    // Decrement nodes alive
    Node::nodes_alive--;

    // Reassign the head
    head_ = node;

    // Decrement the size
    size_--;
  }

  /*!************************************************************************
    \brief
      Returns the value stored in the head_ node

    \return
      The value stored in the head_ node
    **************************************************************************/
  int List::front() const
  {
    return head_->data;
  }

  /*!************************************************************************
    \brief
      Returns how many nodes are within the list

    \return
      The number of nodes within the list
    **************************************************************************/
  int List::size() const
  {
    return size_;
  }

  /*!************************************************************************
    \brief
      Returns whether or not the list is empty

    \return
      Bool indicating whether or not the list is empty
    **************************************************************************/
  bool List::empty() const
  {
    if (size_ == 0)
      return true;

    return false;
  }

  /*!************************************************************************
    \brief
      Removes all of the nodes within the list
    **************************************************************************/
  void List::clear()
  {
    while (head_)
      pop_front();
  }

  /*!************************************************************************
    \brief
      Returns the number of lists that have been created

    \return
      The number of lists created
    **************************************************************************/
  int List::list_count()
  {
    return list_count_;
  }

  /*!************************************************************************
    \brief
      Returns the number of nodes active within the given list

    \return
      The number of nodes active
    **************************************************************************/
  int List::node_count()
  {
    return Node::nodes_alive;
  }
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /*!************************************************************************
    \brief
      Allocates new node with the given value

    \param data
      The value to be stored within the new node

    \return
      The new node
    **************************************************************************/
  List::Node* List::new_node(int data) const
  {
    Node::nodes_alive++;
    return new Node(data);
  }

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // non-member methods

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
  std::ostream& operator<<(std::ostream& os, const List& list)
  {
    // Start at the first node
    List::Node* pnode = list.head_;

    // Until we reach the end of the list
    while (pnode != 0)
    {
      os << std::setw(4) << pnode->data; // print the data in this node
      pnode = pnode->next;               // move to the next node
    }

    os << std::endl; // extra newline for readability
    return os;
  }

} //namespace CS170
