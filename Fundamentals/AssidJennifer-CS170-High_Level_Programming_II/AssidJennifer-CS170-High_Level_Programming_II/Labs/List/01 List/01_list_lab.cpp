/*!****************************************************************************
\file    List.cpp
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

#include <iostream> // cout, endl
#include "01_list_lab.h"

/*******************************************************************/
/*******************************************************************/
namespace CS170
{
    namespace ListLab_1_NM
    {
        /*!**********************************************************************
          \brief
            Assigns initial value to the head_ pointer for the list
        **************************************************************************/
        ListLab_1::ListLab_1()
        {
            head_ = 0;
        }

        /*!************************************************************************
          \brief
            This function frees the memory associated with each node within the
            given list
        **************************************************************************/
        ListLab_1::~ListLab_1()
        {
            // Variable to act as a temporary storage
            Node* iterator = head_;

            // Iterates through the linked list
            while (iterator != NULL)
            {
                // Place pointer to next node in temporary storage
                head_ = iterator->next;
                // Frees memory associated with current node
                iterator->next = NULL;
                iterator->number = 0;
                delete iterator;
                // Take next node out of storage
                iterator = head_;
            }

            // Closes off the last of the pointers
            head_ = NULL;
        }

        /*!**********************************************************************
          \brief
            This function dynamically allocates a Node and initializes the
            associated values (number / next) to 0 / NULL.

          \param value
            Data that will be stored within the new node

          \return
            Pointer to the newly created node
        **************************************************************************/
        Node* ListLab_1::MakeNode(int value) const
        {
            // The Node constructor sets the value, and sets next to 0.
            return new Node(value);
        }

        /*!************************************************************************
          \brief
            This function prints out the data stored within the nodes in a given
            list
        **************************************************************************/
        void ListLab_1::PrintList() const
        {
            Node* list = head_;
            while (list)
            {
                std::cout << list->number << "   ";
                list = list->next;
            }
            std::cout << std::endl;
        }

        /*!******************************************************************
          \brief
            Get the number of nodes in the list.

          \return
            The number of nodes in the list.
        ********************************************************************/
        int ListLab_1::Count() const
        {
            int count = 0; // Number of nodes in the list

            Node* phead = head_;

            while (phead)
            {
                count++;
                phead = phead->next;
            }

            return count;
        }

        /*!************************************************************************
          \brief
            This function adds a node with the given value to the tail of the
            given list

          \param value
            Data to be stored in the new node
        **************************************************************************/
        void ListLab_1::AddToEnd(int value)
        {
            // Create stepper variable
            Node* stepper = head_;

            // Allocates new Node and transfers data
            struct Node* newNode = MakeNode(value);
            newNode->number = value;

            // Set new Node to NULL to notate the end of the list
            newNode->next = NULL;

            // If the linked list is empty...
            if (head_ == NULL)
            {
                // Set new node to the head of the list
                head_ = newNode;
                // Exit out of function
                return;
            }

            // Step through the list to find the end
            while (stepper->next != NULL)
                stepper = stepper->next;

            // Change the current end node to point to new end node
            stepper->next = newNode;
            return;
        }

        /*!************************************************************************
         \brief
           This function adds a node with the given data to the head of the
           given list

         \param value
           Data to be stored in the new node
       **************************************************************************/
        void ListLab_1::AddToFront(int value)
        {
            // Allocates a new Node and transfer data
            struct Node* newNode = MakeNode(value);
            newNode->number = value;

            // Set the new Nodes next pointer to current head
            newNode->next = (head_);

            // Move the old head to point to new node
            (head_) = newNode;
        }

        /*!************************************************************************
         \brief
           This function finds if there is a node containing the given value
           within the given list

         \param value
           Data to be stored in the new node

         \return
           Pointer to the node that contains the given value
       **************************************************************************/
        Node* ListLab_1::FindItem(int value)
        {
            Node* stepper = head_;

            // Iterates through the list
            while (stepper)
            {
                // If the current node contains the value
                if (stepper->number == value)
                {
                    // Return pointer to that node
                    return stepper;
                }
                // Step through the list
                stepper = stepper->next;
            }
            // If value not found, return NULL
            return NULL;
        }

        /*!************************************************************************
          \brief
            This function inserts a node with the given value at the given
            position within the given list

          \param value
            Data to be stored in the new node

          \param position
            Position where the node will be stored within the list
        **************************************************************************/
        void ListLab_1::Insert(int value, int position)
        {
            // Create nodes to hold the previous and current nodes of position
            struct Node* prev = NULL;
            struct Node* curr = head_;


            int stepper = 0;

            // If the given list is empty...
            if (head_ == NULL)
            {
                struct Node* newNode = MakeNode(value);
                newNode->number = value;
                // Set the head of the list to new node
                head_ = newNode;
                // Set the next pointer of current node to NULL
                newNode->next = NULL;
            }

            // Iterate over the list to find the given position
            while (curr->next != NULL && stepper != position)
            {
                // Sets the previous node to current node
                prev = curr;
                // Sets the current node to next node to continue iteration
                curr = curr->next;
                // Increment the stepper to continue iteration
                stepper++;
            }

            // If given position is the head of the list...
            if (position == 0)
            {
                // Add the new node to the head of the list
                AddToFront(value);
                // Exit function
                return;
            }

            // If given position is the tail of the list
            if (curr->next == NULL && position == stepper + 1)
            {
                // Add the new node to the tail of the list
                AddToEnd(value);
                // Exit function
                return;
            }

            // If given position is outside of the list bounds
            if (position > stepper + 1)
                // Exit out of the function
                return;

            struct Node* newNode = MakeNode(value);
            newNode->number = value;
            // Add the new node in specified position and correct pointers
            prev->next = newNode;
            newNode->next = curr;
        }
    } // namespace ListLab
} // namespace CS170

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <iomanip>
void TestAdd(void)
{
    std::cout << "\n******************** TestAdd ********************\n";

    CS170::ListLab_1_NM::ListLab_1 list1;
    CS170::ListLab_1_NM::ListLab_1 list2;
    const int numbers[] = {
                           12, 34, 21, 56, 38, 94, 23, 22,
                           67, 56, 88, 19, 59, 10, 17
    };
    size_t size = sizeof(numbers) / sizeof(*numbers);

    // Create two lists
    for (size_t i = 0; i < size; i++)
    {
        list1.AddToEnd(numbers[i]);
        list2.AddToFront(numbers[i]);

        std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
        list1.PrintList();
        std::cout << "List2 (" << std::setw(2) << list2.Count() << " nodes): ";
        list2.PrintList();
        std::cout << std::endl;
    }
}

void TestFind(void)
{
    std::cout << "\n******************** TestFind ********************\n";

    CS170::ListLab_1_NM::ListLab_1 list1;
    const int numbers[] = {
                           22, 23, 38, 94, 56, 34,
                           67, 56, 88, 19, 10, 59
    };
    size_t size = sizeof(numbers) / sizeof(*numbers);
    for (size_t i = 0; i < size; i++)
        list1.AddToEnd(numbers[i]);

    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    list1.PrintList();

    // Find an existing value
    int value = 56;
    CS170::ListLab_1_NM::Node* node = list1.FindItem(value);
    if (node)
        std::cout << "Value: " << node->number << " was found.\n";
    else
        std::cout << "Value: " << value << " was not found.\n";

    value = 22;
    node = list1.FindItem(value);
    if (node)
        std::cout << "Value: " << node->number << " was found.\n";
    else
        std::cout << "Value: " << value << " was not found.\n";

    value = 59;
    node = list1.FindItem(value);
    if (node)
        std::cout << "Value: " << node->number << " was found.\n";
    else
        std::cout << "Value: " << value << " was not found.\n";

    // Find a non-existant value
    value = 123;
    node = list1.FindItem(value);
    if (node)
        std::cout << "Value: " << node->number << " was found.\n";
    else
        std::cout << "Value: " << value << " was not found.\n";

    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    list1.PrintList();
}

void TestInsert1(void)
{
    std::cout << "\n******************** TestInsert1 ********************\n";

    CS170::ListLab_1_NM::ListLab_1 list1;
    const int numbers[] = {
                           12, 34, 21, 56, 38, 94, 23, 22,
                           67, 56, 88, 19, 59, 10, 17
    };
    for (int i = 0; i < 5; i++)
    {
        list1.AddToEnd(numbers[i]);
        std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
        list1.PrintList();
        std::cout << std::endl;
    }

    std::cout << "Inserting... 91 at position 3\n";
    list1.Insert(91, 3);
    list1.PrintList();
    std::cout << "Inserting... 92 at position 0\n";
    list1.Insert(92, 0);
    list1.PrintList();
    std::cout << "Inserting... 93 at position 7\n";
    list1.Insert(93, 7);
    list1.PrintList();
    std::cout << "Inserting... 94 at position 9\n";
    list1.Insert(94, 9);
    list1.PrintList();
}

void list_lab_1(void)
{
    TestAdd();
    TestFind();
    TestInsert1();
}