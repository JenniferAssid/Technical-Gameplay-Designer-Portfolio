/*!****************************************************************************
\file    List.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab 08
\date    13-03-2021

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

  - Merge
      Merges two sorted lists together into one list that is then made into
      the class list

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
      This function deletes all occurences of the values in the given list in
      the class list

  - Insert (value)
      This function places the given value into the linked list in the order
      of smallest to largest

  - MakeNode
      This function dynamically allocates a Node and initializes the
      associated values (number / next) to 0 / NULL.

******************************************************************************/
#include <iostream> // ostream
#include "04_list_lab.h"

namespace CS170
{
    namespace ListLab_4_NM
    {
        /*!************************************************************************
          \brief
            This function assigns initial value to the head_ pointer for the list
        **************************************************************************/
        ListLab_4::ListLab_4()
        {
            head_ = 0;
        }

        /*!************************************************************************
          \brief
            This function frees the memory associated with each node within the
            given list
        **************************************************************************/
        ListLab_4::~ListLab_4()
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
            Takes the given sorted list and removes all the duplicates of data

          \return
            The number of duplicates that were removed from the given list

        **************************************************************************/
        int ListLab_4::RemoveDuplicates()
        {
            int count = 0;

            Node* curr = head_;

            Node* next;

            if (curr == NULL) return 0;

            while (curr->next != NULL)
            {
                if (curr->number == curr->next->number)
                {
                    next = curr->next->next;
                    delete curr->next;
                    curr->next = next;

                    count++;
                }
                else
                    curr = curr->next;
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
        void ListLab_4::AddToEnd(int value)
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
            Merges two sorted lists together into one list that is then made into
            the class list

          \param rhs
            The list that will be sorted into the class list
        **************************************************************************/
        void ListLab_4::Merge(ListLab_4& rhs)
        {
            // if the class list is empty
            if (head_ == NULL)
            {
                // Go through the rhs list and add it to the class list
                head_ = rhs.head_;
                rhs.head_ = rhs.head_->next;
                while (rhs.head_)
                {
                    rhs.head_ = rhs.head_->next;
                }

                return;
            }

            // If the rhs list is empty, return out of function
            if (rhs.head_ == NULL)
                return;


            // Determine which list's head node will be the head node of the new list
            Node* mergedHead = NULL;
            if (head_->number <= rhs.head_->number)
            {
                mergedHead = head_;
                head_ = head_->next;
            }
            else
            {
                mergedHead = rhs.head_;
                rhs.head_ = rhs.head_->next;
            }

            Node* mergedTail = mergedHead;

            // While both lists are not empty
            while (head_ && rhs.head_)
            {
                Node* temp = NULL;
                // Determine which node has the lowest value and increment the list
                if (head_->number <= rhs.head_->number)
                {
                    temp = head_;
                    head_ = head_->next;
                }
                else
                {
                    temp = rhs.head_;
                    rhs.head_ = rhs.head_->next;
                }

                // Set the node with lowest amount in the new list
                mergedTail->next = temp;
                mergedTail = temp;
            }

            // If the class list is not empty
            if (head_)
            {
                // Add the remaining nodes to the new list
                while (head_)
                {
                    mergedTail->next = head_;
                    mergedTail = mergedTail->next;
                    head_ = head_->next;
                }
            }

            // If the rhs list is not empty
            if (rhs.head_)
            {
                // Add the remaining nodes to the new list
                while (rhs.head_)
                {
                    mergedTail->next = rhs.head_;
                    mergedTail = mergedTail->next;
                    rhs.head_ = rhs.head_->next;
                }
            }

            // Set the class head to the new sorted list
            head_ = mergedHead;
        }

        /*!************************************************************************
         \brief
           This function adds a node with the given data to the head of the
           given list

         \param value
           Data to be stored in the new node
       ***************************************************************************/
        void ListLab_4::AddToFront(int value)
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
        int ListLab_4::Count() const
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
        Node* ListLab_4::FindItem(int value) const
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
        void ListLab_4::Insert(int value, int position)
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
        void ListLab_4::Delete(int value)
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
        void ListLab_4::Concat(const ListLab_4& Source)
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
        void ListLab_4::Delete(const ListLab_4& Items)
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
        void ListLab_4::Insert(int value)
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
        std::ostream& operator<<(std::ostream& os, const ListLab_4& rhs)
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
        Node* ListLab_4::MakeNode(int value) const
        {
            // The Node constructor sets the value, 
            // and sets next to 0.
            return new Node(value);
        }
    } // ListLab
} // CS170

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <iomanip>

int RandomInt(unsigned low, unsigned high)
{
    return static_cast<int>(rand() % (high - low + 1) + low);
}

void Swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void Shuffle(int* array, unsigned count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        int r = RandomInt(i, count - 1);
        Swap(array[i], array[r]);
    }
}

void TestInsert2_4(void)
{
    std::cout << "\n******************** TestInsert2 ********************\n";

    CS170::ListLab_4_NM::ListLab_4 list1;
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

void TestDuplicates(const int numbers1[], int size1)
{
    CS170::ListLab_4_NM::ListLab_4 list1;

    for (int i = 0; i < size1; i++)
        list1.Insert(numbers1[i]);

    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;

    std::cout << "Duplicates removed: ";
    std::cout << list1.RemoveDuplicates() << std::endl;
    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;
}

void TestDuplicates1(void)
{
    std::cout << "\n******************** TestDuplicates1 ********************\n";

    const int numbers1[] = { 12, 34, 12, 24, 12, 12, 27, 34, 12, 12, 88, 27, 59, 12, 27, 12 };
    int size1 = static_cast<int>(sizeof(numbers1) / sizeof(*numbers1));

    TestDuplicates(numbers1, size1);
}

void TestDuplicates2(void)
{
    std::cout << "\n******************** TestDuplicates2 ********************\n";

    const int numbers1[] = { 12, 12, 12, 12, 12, 12, 12, 12 };
    int size1 = static_cast<int>(sizeof(numbers1) / sizeof(*numbers1));

    TestDuplicates(numbers1, size1);
}

void TestDuplicates3(void)
{
    std::cout << "\n******************** TestDuplicates3 ********************\n";

    const int numbers1[] = { 12, 17, 5, 10, 4, 11, 9, 8 };
    int size1 = static_cast<int>(sizeof(numbers1) / sizeof(*numbers1));

    TestDuplicates(numbers1, size1);
}

void TestDuplicates4(void)
{
    std::cout << "\n******************** TestDuplicates4 ********************\n";

    const int numbers1[] = { 5 };
    int size1 = static_cast<int>(sizeof(numbers1) / sizeof(*numbers1));

    TestDuplicates(numbers1, size1);
}

void TestDuplicates5(void)
{
    std::cout << "\n******************** TestDuplicates5 ********************\n";

    const int numbers1[] = { 5, 1 };
    int size1 = static_cast<int>(sizeof(numbers1) / sizeof(*numbers1));

    TestDuplicates(numbers1, size1);
}

void TestDuplicates6(void)
{
    std::cout << "\n******************** TestDuplicates6 ********************\n";

    const int numbers1[] = { 5, 1 };

    TestDuplicates(numbers1, 0);
}

void TestDuplicates7(void)
{
    std::cout << "\n******************** TestDuplicates7 ********************\n";

    CS170::ListLab_4_NM::ListLab_4 list1;
    const int size = 10000;

    for (int i = 0; i < size; i++)
        list1.Insert(RandomInt(1, 9));

    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): \n";
    //std::cout << list1 << std::endl;

    std::cout << "Duplicates removed: ";
    std::cout << list1.RemoveDuplicates() << std::endl;
    std::cout << "List1 (" << std::setw(2) << list1.Count() << " nodes): ";
    std::cout << list1 << std::endl;
}

void list_lab_4(void)
{
    TestDuplicates1();
    TestDuplicates2();
    TestDuplicates3();
    TestDuplicates4();
    TestDuplicates5();
    TestDuplicates6();
    TestDuplicates7();
}