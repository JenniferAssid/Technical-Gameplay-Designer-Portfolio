/*!****************************************************************************
\file    List.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab 05
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
#include <iostream> // ostream
#include "02_list_lab.h"

namespace CS170
{
    namespace ListLab_2_NM
    {
        /*!************************************************************************
          \brief
            This function assigns initial value to the head_ pointer for the list
        **************************************************************************/
        ListLab_2::ListLab_2()
        {
            head_ = 0;
        }

        /*!************************************************************************
          \brief
            This function frees the memory associated with each node within the
            given list
        **************************************************************************/
        ListLab_2::~ListLab_2()
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

        /*!************************************************************************
          \brief
            This function adds a node with the given value to the tail of the
            given list

          \param value
            Data to be stored in the new node
        **************************************************************************/
        void ListLab_2::AddToEnd(int value)
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
       ***************************************************************************/
        void ListLab_2::AddToFront(int value)
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
            Get the number of nodes in the list.

          \return
            The number of nodes in the list.
        **************************************************************************/
        int ListLab_2::Count() const
        {
            int count = 0;
            Node* list = head_;

            while (list)
            {
                count++;
                list = list->next;
            }

            return count;
        }

        /*!************************************************************************
         \brief
           This function finds if there is a node containing the given value
           within the given list

         \param value
           Data to be stored in the new node

         \return
           Pointer to the node that contains the given value
       ***************************************************************************/
        Node* ListLab_2::FindItem(int value) const
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
        void ListLab_2::Insert(int value, int position)
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

        /*!************************************************************************
          \brief
            This function deletes the first instance of the node with the given
            data

          \param value
            Data to be deleted in the new node
        **************************************************************************/
        void ListLab_2::Delete(int value)
        {
            // Create nodes to hold the previous and current nodes of position
            struct Node* prev = NULL;
            struct Node* curr = head_;

            int stepper = 0;

            if (head_ == NULL) return;

            // Iterate over the list to find the given position
            while (curr->next != NULL && curr->number != value)
            {
                // Sets the previous node to current node
                prev = curr;
                // Sets the current node to next node to continue iteration
                curr = curr->next;
                // Increment the stepper to continue iteration
                stepper++;
            }

            // Checks if the value was actually found
            if (curr->number != value) return;

            // If given position is the head of the list...
            if (stepper == 0)
            {
                head_ = head_->next;

                delete curr; curr = NULL;

                // Exit function
                return;
            }

            // If at the tail of the list
            if (curr->next == NULL)
            {
                prev->next = NULL;
                delete curr; curr = NULL;
                return;
            }

            // Delete the current node
            prev->next = curr->next;
            delete curr; curr = NULL;
        }

        /*!************************************************************************
          \brief
            This function appends the values in the given list to the class list

          \param Source
        **************************************************************************/
        void ListLab_2::Concat(const ListLab_2& Source)
        {
            Node* stepper = Source.head_;

            while (stepper->next != NULL)
            {
                AddToEnd(stepper->number);

                stepper = stepper->next;
            }

            AddToEnd(stepper->number);
        }

        /*!************************************************************************
          \brief
            This function deletes all instances of the values in the given list
            in the class list

          \param Items
        **************************************************************************/
        void ListLab_2::Delete(const ListLab_2& Items)
        {
            Node* stepper = Items.head_;

            while (stepper)
            {
                while (FindItem(stepper->number) != NULL)
                {
                    Delete(stepper->number);
                }
                stepper = stepper->next;
            }
        }

        /*!************************************************************************
          \brief
            This function inserts a node with the given value in the list in the
            order of smallest value to largest value

          \param value
            Data to be stored in the new node
        **************************************************************************/
        void ListLab_2::Insert(int value)
        {
            // Create nodes to hold the previous and current nodes of position
            struct Node* curr = head_;

            int indexCounter = 0;

            // If the given list is empty...
            if (head_ == NULL)
            {
                struct Node* newNode = MakeNode(value);
                newNode->number = value;
                // Set the head of the list to new node
                head_ = newNode;
                // Set the next pointer of current node to NULL
                newNode->next = NULL;

                return;
            }

            // Iterate over the list to find the given position
            while (curr != NULL && curr->number < value)
            {
                // Sets the current node to next node to continue iteration
                curr = curr->next;

                indexCounter++;
            }

            Insert(value, indexCounter);
        }


        /*!************************************************************************
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
        std::ostream& operator<<(std::ostream& os, const ListLab_2& rhs)
        {
            Node* list = rhs.head_;

            while (list)
            {
                os << list->number << "   ";

                list = list->next;
            }

            return os;
        }

        /*!************************************************************************
          \brief
            This function dynamically allocates a Node and initializes the
            associated values (number / next) to 0 / NULL.

          \param value
            Data that will be stored within the new node

          \return
            Pointer to the newly created node
        **************************************************************************/
        Node* ListLab_2::MakeNode(int value) const
        {
            // The Node constructor sets the value, 
            // and sets next to 0.
            return new Node(value);
        }
    } // ListLab
} // CS170

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <iomanip>

void TestAdd_2()
{
    std::cout << "\n******************** TestAdd ********************\n";

    CS170::ListLab_2_NM::ListLab_2 list1;
    CS170::ListLab_2_NM::ListLab_2 list2;
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
        std::cout << list1 << std::endl;
        std::cout << "List2 (" << std::setw(2) << list2.Count() << " nodes): ";
        std::cout << list2 << std::endl;
        std::cout << std::endl;
    }
}

void TestFind_2()
{
    std::cout << "\n******************** TestFind ********************\n";

    CS170::ListLab_2_NM::ListLab_2 list1;
    const int numbers[] = {
                           22, 23, 38, 94, 56, 34,
                           67, 56, 88, 19, 10, 59
    };
    size_t size = sizeof(numbers) / sizeof(*numbers);
    for (size_t i = 0; i < size; i++)
        list1.AddToEnd(numbers[i]);

    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;

    // Find an existing value
    int value = 56;
    CS170::ListLab_2_NM::Node* node = list1.FindItem(value);
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
    std::cout << list1 << std::endl;
}

void TestDelete1()
{
    std::cout << "\n******************** TestDelete1 ********************\n";

    CS170::ListLab_2_NM::ListLab_2 list1;
    const int numbers[] = {
                           22, 23, 38, 94, 56, 34,
                           67, 56, 88, 19, 10, 59
    };
    size_t size = sizeof(numbers) / sizeof(*numbers);
    for (size_t i = 0; i < size; i++)
        list1.AddToEnd(numbers[i]);

    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;

    list1.Delete(56);
    std::cout << "Deleting: 56 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;

    list1.Delete(22);
    std::cout << "Deleting: 22 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;

    list1.Delete(59);
    std::cout << "Deleting: 59 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;

    list1.Delete(99);
    std::cout << "Deleting: 99 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;

    {
        const int numbers[] = {
                               23, 38, 94, 34,
                               67, 56, 88, 19, 10
        };
        size_t size = sizeof(numbers) / sizeof(*numbers);
        for (size_t i = 0; i < size; i++)
        {
            list1.Delete(numbers[i]);
            std::cout << "Deleting: " << numbers[i] << " (" << std::setw(2)
                << list1.Count() << " nodes): ";
            std::cout << list1 << std::endl;
        }
        list1.Delete(99);
        std::cout << "Deleting: 99 (" << std::setw(2) << list1.Count()
            << " nodes): ";
        std::cout << list1 << std::endl;
    }
}

void TestDelete2()
{
    std::cout << "\n******************** TestDelete2 ********************\n";

    CS170::ListLab_2_NM::ListLab_2 list1;
    const int numbers1[] = {
                           22, 23, 38, 94, 56, 34,
                           67, 56, 88, 19, 10, 59
    };
    size_t size1 = sizeof(numbers1) / sizeof(*numbers1);
    for (size_t i = 0; i < size1; i++)
        list1.AddToEnd(numbers1[i]);

    CS170::ListLab_2_NM::ListLab_2 list2;
    const int numbers2[] = {
                           94, 23, 56, 22, 56, 19,
                           67, 38, 34, 59
    };
    size_t size2 = sizeof(numbers2) / sizeof(*numbers2);
    for (size_t i = 0; i < size2; i++)
        list2.AddToEnd(numbers2[i]);

    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;
    std::cout << "List2 (" << std::setw(2) << list2.Count() << " nodes): ";
    std::cout << list2 << std::endl;

    list1.Delete(list2);
    std::cout << "List1 after deleting list2 (" << std::setw(2) << list1.Count()
        << " nodes): ";
    std::cout << list1 << std::endl;
    std::cout << "List2 (" << std::setw(2) << list2.Count() << " nodes): ";
    std::cout << list2 << std::endl;
}

void TestConcat()
{
    std::cout << "\n******************** TestConcat ********************\n";

    CS170::ListLab_2_NM::ListLab_2 list1;
    CS170::ListLab_2_NM::ListLab_2 list2;
    const int numbers1[] = { 12, 34, 21, 56, 38, 94, 23 };
    const int numbers2[] = { 67, 56, 88, 19, 59, 10, 17 };

    size_t size1 = sizeof(numbers1) / sizeof(*numbers1);
    size_t size2 = sizeof(numbers2) / sizeof(*numbers2);

    for (size_t i = 0; i < size1; i++)
        list1.AddToEnd(numbers1[i]);

    for (size_t i = 0; i < size2; i++)
        list2.AddToEnd(numbers2[i]);

    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;
    std::cout << "List2 (" << std::setw(2) << list2.Count() << " nodes): ";
    std::cout << list2 << std::endl;

    std::cout << "Adding both lists (" << std::setw(2) << list1.Count()
        << " nodes): ";
    list1.Concat(list2);

    std::cout << list1 << std::endl;
}

void TestInsert1_2()
{
    std::cout << "\n******************** TestInsert1 ********************\n";

    CS170::ListLab_2_NM::ListLab_2 list1;
    const int numbers[] = {
                           12, 34, 21, 56, 38, 94, 23, 22,
                           67, 56, 88, 19, 59, 10, 17
    };
    for (int i = 0; i < 5; i++)
    {
        list1.AddToEnd(numbers[i]);
        std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
        std::cout << list1 << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Inserting... 91 at position 3\n";
    list1.Insert(91, 3);
    std::cout << list1 << std::endl;
    std::cout << "Inserting... 92 at position 0\n";
    list1.Insert(92, 0);
    std::cout << list1 << std::endl;
    std::cout << "Inserting... 93 at position 7\n";
    list1.Insert(93, 7);
    std::cout << list1 << std::endl;
    std::cout << "Inserting... 94 at position 9\n";
    list1.Insert(94, 9);
    std::cout << list1 << std::endl;
}

void TestInsert2()
{
    std::cout << "\n******************** TestInsert2 ********************\n";

    CS170::ListLab_2_NM::ListLab_2 list1;
    const int numbers[] = {
                           12, 34, 21, 56, 38, 94, 23, 22,
                           67, 56, 88, 19, 59, 10, 17
    };
    size_t size = sizeof(numbers) / sizeof(*numbers);

    for (size_t i = 0; i < size; i++)
    {
        list1.Insert(numbers[i]);

        std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes) ";
        std::cout << "Inserting " << std::setw(2) << numbers[i] << ": ";
        std::cout << list1 << std::endl;
    }
}

void list_lab_2(void)
{
    TestAdd_2();
    TestFind_2();
    TestInsert1_2();
    TestInsert2();
    TestConcat();
    TestDelete1();
    TestDelete2();
}