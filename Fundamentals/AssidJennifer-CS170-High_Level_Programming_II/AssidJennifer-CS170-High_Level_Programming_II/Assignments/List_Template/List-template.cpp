/*!****************************************************************************
\file    List-template.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab (Assignment) 04
\date    02-04-2021

\brief
  This program houses functions necessary to create, analyize, and maintain
  a linked list of numbers within a class List

  The functions include:

  - operator<<
      Outputs the given list into the given output stream

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

  - node_count
      Returns the number of nodes active within the given list

  - new_node
      Allocates a new node with the given value

******************************************************************************/

#pragma once
#include "List-template.h"
namespace CS170
{

    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // static members

      /*!************************************************************************
        \brief
          Initializes number of nodes alive to be 0
        **************************************************************************/
    template <typename T>
    int List_Template<T>::Node::nodes_alive = 0;

    /*!************************************************************************
        \brief
          Returns how many nodes are alive

        \return
          The number of nodes alive
        **************************************************************************/
    template <typename T>
    int List_Template<T>::node_count(void)
    {
        return Node::nodes_alive;
    }

    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // List::Node methods
    /*!**********************************************************************
          \brief
            Non-Default Constructor - sets the value within the node to given int

          \param value
            Value to be stored within the new node
          ************************************************************************/
    template <typename T>
    List_Template<T>::Node::Node(T value) : data(value)
    {
        // Sets data to the given value
        data = value;

        // Sets the next pointer to NULL (not pointing at anything)
        next = NULL;

        // Increment nodes alive
        nodes_alive++;
    }

    /*!**********************************************************************
          \brief
            Default Constructor - Destroys node
          ************************************************************************/
    template <typename T>
    List_Template<T>::Node::~Node()
    {
        // Decrement nodes alive
        nodes_alive--;
    }


    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // public methods
    /*!************************************************************************
        \brief
          Default Constructor - sets all class variables to 0
        **************************************************************************/
    template <typename T>
    List_Template<T>::List_Template()
    {
        // Set the two class pointers to NULL
        head_ = tail_ = NULL;

        // Set size to 0
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
    template <typename T>
    List_Template<T>::List_Template(const T* array, int size)
    {
        // Set the values to their default positions
        head_ = tail_ = NULL;
        size_ = 0;

        // Add each index within the array to the back of the list
        for (int i = 0; i < size; i++)
            push_back(array[i]);
    }

    /*!************************************************************************
        \brief
          Copy Constructor - copies over the values from a given linked list

        \param list
          The list that is being copied into a new list
        **************************************************************************/
    template <typename T>
    List_Template<T>::List_Template(const List_Template& list)
    {
        // Set the class values to their default positions
        head_ = tail_ = NULL;
        size_ = 0;

        // Iterate across the list
        Node* node = list.head_;

        while (node)
        {
            // copy over the values from within the given list
            push_back(node->data);
            node = node->next;
        }

        // Set the class list's size (just for safety)
        size_ = list.size_;
    }

    /*!************************************************************************
        \brief
          Default Destructor - deletes the dynamic memory associated with the class
        **************************************************************************/
    template <typename T>
    List_Template<T>::~List_Template()
    {
        clear();
    }

    /*!************************************************************************
        \brief
          Returns the address of the list than a variable is going to be set to

        \param list
          The list that is being copied to be have a variable set equal to it

        \return
          The address of the copied list
        **************************************************************************/
    template <typename T>
    List_Template<T>& List_Template<T>::operator=(const List_Template<T>& list)
    {
        // Check for self-assignment
        if (this == &list) return *this;

        // Clear existing nodes within the class list
        this->clear();

        // Set class values to their default positions
        size_ = 0;
        head_ = tail_ = NULL;

        // Append the values of teh given list to the class list
        for (int i = 0; i < list.size_; i++)
            push_back(list[i]);

        // Return the class list
        return *this;
    }

    /*!************************************************************************
        \brief
          Returns the address of the list that has had another list appended to it

        \param list
          The list that is being appended onto the current list

        \return
          The address of the larger list
        **************************************************************************/
    template <typename T>
    List_Template<T>& List_Template<T>::operator+= (const List_Template<T>& list)
    {
        // Iterate across the list and append the values of the given list
        Node* node = list.head_;

        while (node)
        {
            push_back(node->data);
            node = node->next;
        }

        return *this;
    }

    /*!************************************************************************
        \brief
          Returns a new list that contains the two list appended to each other

        \param list
          The second list that is being appended onto the new list

        \return
          The new list
        **************************************************************************/
    template <typename T>
    List_Template<T> List_Template<T>::operator+(const List_Template<T>& list) const
    {
        // Create a new list and combine the two lists together
        List_Template temp(*this);
        temp += list;
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
    template <typename T>
    const T& List_Template<T>::operator[](int index) const
    {
        // Iterate across the list until it reaches the given index
        Node* node = head_;

        for (int i = 0; i < index; i++)
            node = node->next;

        // Return the data within the node at the given index
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
    template <typename T>
    T& List_Template<T>::operator[](int index)
    {
        // Iterate across the list until it reaches the given index
        Node* node = head_;

        for (int i = 0; i < index; i++)
            node = node->next;

        // Return the data within the node at the given index
        return node->data;
    }

    /*!************************************************************************
        \brief
          Adds given value to the front of the linked list, updating the class
          head_ pointer

        \param value
          Value that is being stored in the new node
        **************************************************************************/
    template <typename T>
    void List_Template<T>::push_front(const T& value)
    {
        // Create a new node with the given value
        Node* node = new_node(value);

        // Set the new node's next pointer to point to the current head
        node->next = head_;

        // Reassign the head pointer
        head_ = node;

        // Check if tail needs to be reassigned
        if (size_ == 0)
            tail_ = node;

        // Increase the size parameter
        size_++;
    }

    /*!************************************************************************
        \brief
          Adds given value to the back of the array, updating the class tail_
          pointer

        \param value
          Value that is being stored in the new node
        **************************************************************************/
    template <typename T>
    void List_Template<T>::push_back(const T& value)
    {

        Node* head = head_;

        // Create the new node with the given value
        Node* node = new_node(value);
        node->next = NULL;

        // If the list is empty, set the tail and head to point to the new node
        if (head == NULL)
        {
            head_ = node;
            size_++;
            return;
        }

        // Set the list's tail to equal to current node
        tail_ = node;

        // Iterate to the end of the list
        while (head->next)
            head = head->next;

        // Link the new node to the list
        head->next = node;

        // Increment the size parameter
        size_++;
    }

    /*!************************************************************************
        \brief
          Removes the head_ node and updates the head_ pointer
        **************************************************************************/
    template <typename T>
    void List_Template<T>::pop_front()
    {
        if (head_ == NULL) return;

        // Set a new node to the next item from the head
        Node* node = head_->next;

        // Remove the head from the list
        delete head_;

        // Set the head to be the next value
        head_ = node;

        // Decrement the size parameter
        size_--;
    }

    /*!************************************************************************
        \brief
          Returns the value stored in the head_ node

        \return
          The value stored in the head_ node
        **************************************************************************/
    template <typename T>
    T List_Template<T>::front() const
    {
        return head_->data;
    }

    /*!************************************************************************
        \brief
          Returns how many nodes are within the list

        \return
          The number of nodes within the list
        **************************************************************************/
    template <typename T>
    int List_Template<T>::size() const
    {
        return size_;
    }

    /*!************************************************************************
        \brief
          Returns whether or not the list is empty

        \return
          Bool indicating whether or not the list is empty
        **************************************************************************/
    template <typename T>
    bool List_Template<T>::empty() const
    {
        if (size_ == 0)
            return true;

        return false;
    }

    /////////////////////////////////////////////////////////////////////////
    // Function: List::clear
    //  Purpose: Removes all of the nodes in the list.
    //   Inputs: None
    //  Outputs: None
    /////////////////////////////////////////////////////////////////////////
    /*!************************************************************************
        \brief
          Removes all of the nodes within the list
        **************************************************************************/
    template <typename T>
    void List_Template<T>::clear()
    {
        while (!empty())
            pop_front();
    }


    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // private methods

    /////////////////////////////////////////////////////////////////////////
    // Function: new_node
    //  Purpose: Allocate a node, initialize the data and next pointer
    //   Inputs: data - the data to put in the node
    //  Outputs: A pointer to the node
    /////////////////////////////////////////////////////////////////////////
    /*!************************************************************************
        \brief
          Allocates new node with the given value

        \param data
          The value to be stored within the new node

        \return
          The new node
        **************************************************************************/
    template <typename T>
    typename List_Template<T>::Node* List_Template<T>::new_node(const T& data) const
    {
        Node* node = new Node(data); // create the node
        node->next = 0;              // no next pointer yet

        return node;
    }

} // namespace CS170


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// non-members

#include <iomanip>

/*!**********************************************************************
  Outputs all of the data in a list to an output stream.

  \param os
    The stream to output the list to.

  \param list
    The list to output to the stream.

  \return
    The ouput stream (ref) that was passed in (for chaining)
************************************************************************/
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
template <typename T>
std::ostream& CS170::operator<<(std::ostream& os, const CS170::List_Template<T>& list)
{
    // Start at the beginning
    typename CS170::List_Template<T>::Node* pnode = list.head_;

    // Print each item
    while (pnode != 0)
    {
        os << std::setw(4) << pnode->data;
        pnode = pnode->next;
    }
    os << std::endl;

    return os;
}
