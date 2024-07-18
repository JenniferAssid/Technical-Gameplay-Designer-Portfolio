/*!****************************************************************************
\file    List_lab.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab 04
\date    13-05-2021

\brief
  This program houses functions necessary to create, analyize, and maintain
  linked lists

  The functions include:

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

  - FreeList
      This function frees the memory associated with each node within the
      given list

  - Insert
      This function inserts a node with the given value at the given
      position within the given list
******************************************************************************/

#include <iostream> // cout, endl
#include "00_list_lab.h"

namespace CS170
{
    namespace ListLab
    {
        namespace
        {
            /*!**********************************************************************
            \brief
              This function dynamically allocates a Node and initializes the
              associated values (number / next) to 0 / NULL.

            \param value
              Data that will be stored within the new node

            \return
              Pointer to the newly created node
          **************************************************************************/
            struct Node* MakeNode(int value)
            {
                struct Node* pNode = new Node; // Allocate memory for the node
                pNode->number = value;         // Set data field (number)
                pNode->next = 0;               // Set next (no next yet)

                return pNode;
            }
        }

        /*!************************************************************************
          \brief
            This function prints out the data stored within the nodes in a given
            list

          \param list
            The given list whose values will be printed out
        **************************************************************************/
        void PrintList(struct Node* list)
        {
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

          \param list
            The list to count.

          \return
            The number of nodes in the list.
        ********************************************************************/
        int Count(struct Node* list)
        {
            int count = 0; // Number of nodes in the list

            while (list)
            {
                count++;
                list = list->next;
            }

            return count;
        }

        /*!************************************************************************
          \brief
            This function adds a node with the given value to the tail of the
            given list

          \param pList
            Given list where new node will be linked to

          \param value
            Data to be stored in the new node
        **************************************************************************/
        void AddToEnd(Node** pList, int value)
        {
            // Create stepper variable
            Node* stepper = *pList;

            // Allocates new Node and transfers data
            struct Node* newNode = MakeNode(value);
            newNode->number = value;

            // Set new Node to NULL to notate the end of the list
            newNode->next = NULL;

            // If the linked list is empty...
            if (*pList == NULL)
            {
                // Set new node to the head of the list
                *pList = newNode;
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

          \param pList
            Given list where new node will be linked to

          \param value
            Data to be stored in the new node
        **************************************************************************/
        void AddToFront(Node** pList, int value)
        {
            // Allocates a new Node and transfer data
            struct Node* newNode = MakeNode(value);
            newNode->number = value;

            // Set the new Nodes next pointer to current head
            newNode->next = (*pList);

            // Move the old head to point to new node
            (*pList) = newNode;
        }

        /*!************************************************************************
          \brief
            This function finds if there is a node containing the given value
            within the given list

          \param list
            Given list where new node will be linked to

          \param value
            Data to be stored in the new node

          \return
            Pointer to the node that contains the given value
        **************************************************************************/
        Node* FindItem(Node* list, int value)
        {
            // Variable to hold the node containing the value
            struct Node* result;

            // Iterates through the list
            while (list)
            {
                // If the current node contains the value
                if (list->number == value)
                {
                    // Return pointer to that node
                    result = list;
                    return result;
                }
                // Step through the list
                list = list->next;
            }
            // If value not found, return NULL
            return NULL;
        }

        /*!************************************************************************
          \brief
            This function frees the memory associated with each node within the
            given list

          \param pList
            Given list where new node will be linked to
        **************************************************************************/
        void FreeList(Node* pList)
        {
            // Variable to act as a temporary storage
            Node* iterator = pList;

            // Iterates through the linked list
            while (iterator != NULL)
            {
                // Place pointer to next node in temporary storage
                pList = iterator->next;
                // Frees memory associated with current node
                iterator->next = NULL;
                iterator->number = 0;
                delete iterator;
                // Take next node out of storage
                iterator = pList;
            }

            // Closes off the last of the pointers
            pList = NULL;
        }

        /*!************************************************************************
          \brief
            This function inserts a node with the given value at the given
            position within the given list

          \param pList
            Given list where new node will be linked to

          \param value
            Data to be stored in the new node

          \param position
            Position where the node will be stored within the list
        **************************************************************************/
        void Insert(Node** pList, int value, int position)
        {
            // Create nodes to hold the previous and current nodes of position
            struct Node* prev = NULL;
            struct Node* curr = *pList;


            int stepper = 0;

            // If the given list is empty...
            if (*pList == NULL)
            {
                struct Node* newNode = MakeNode(value);
                newNode->number = value;
                // Set the head of the list to new node
                *pList = newNode;
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
                AddToFront(pList, value);
                // Exit function
                return;
            }

            // If given position is the tail of the list
            if (curr->next == NULL && position == stepper + 1)
            {
                // Add the new node to the tail of the list
                AddToEnd(pList, value);
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

void list_lab_0(void)
{
    CS170::ListLab::Node* list1 = 0;
    CS170::ListLab::Node* list2 = 0;
    const int numbers[] = {
                           12, 34, 21, 56, 38, 94, 23, 22,
                           67, 56, 88, 19, 59, 10, 17
    };
    size_t size = sizeof(numbers) / sizeof(*numbers);

    // Create two lists
    for (size_t i = 0; i < size; i++)
    {
        CS170::ListLab::AddToEnd(&list1, numbers[i]);
        CS170::ListLab::AddToFront(&list2, numbers[i]);

        std::cout << "List1 (" << std::setw(2) << CS170::ListLab::Count(list1) << " nodes): ";
        CS170::ListLab::PrintList(list1);
        std::cout << "List2 (" << std::setw(2) << CS170::ListLab::Count(list2) << " nodes): ";
        CS170::ListLab::PrintList(list2);
        std::cout << std::endl;
    }

    // Find an existing value
    int value = 56;
    CS170::ListLab::Node* node = CS170::ListLab::FindItem(list1, value);
    if (node)
        std::cout << "Value: " << node->number << " was found.\n\n";
    else
        std::cout << "Value: " << value << " was not found.\n\n";

    // Find a non-existant value
    value = 123;
    node = CS170::ListLab::FindItem(list1, value);
    if (node)
        std::cout << "Value: " << node->number << " was found.\n\n";
    else
        std::cout << "Value: " << value << " was not found.\n\n";

    // Release the nodes
    CS170::ListLab::FreeList(list1);
    CS170::ListLab::FreeList(list2);

    // Inserting
    list1 = 0;
    for (int i = 0; i < 5; i++)
    {
        CS170::ListLab::AddToEnd(&list1, numbers[i]);
        std::cout << "List1 (" << std::setw(2) << CS170::ListLab::Count(list1) << " nodes): ";
        CS170::ListLab::PrintList(list1);
        std::cout << std::endl;
    }

    std::cout << "Inserting...\n";
    CS170::ListLab::Insert(&list1, 91, 3);
    CS170::ListLab::PrintList(list1);
    CS170::ListLab::Insert(&list1, 92, 0);
    CS170::ListLab::PrintList(list1);
    CS170::ListLab::Insert(&list1, 93, 7);
    CS170::ListLab::PrintList(list1);
    CS170::ListLab::Insert(&list1, 94, 9);
    CS170::ListLab::PrintList(list1);

    CS170::ListLab::FreeList(list1);
}