/*!****************************************************************************
\file    Vector.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab 09
\date    19-03-2021

\brief
  This program houses functions necessary to create, analyize, and maintain
  an array of numbers within a class Vector

  The functions include:

  - Vector
      Default Constructor - sets all class variables to 0

  - ~Vector
      Default Destructor - deletes the dynamic memory associated with the class

  - push_back
      Adds given value to the back of the array, pushing the existing values
      forward in the array

  - push_front
      Adds given value to the front of the array, pushing the existing values
      back in the array

  - operator[]
      Returns the value stored at the given index

  - operator[]
      Returns the address for the given index

  - clear
      Deletes the class array and sets size and capacity to 0

  - empty
      Returns true / false based on whether the size of the array is 0

  - size
      Returns the size of the array

  - capacity
      Returns the capacity of the array

  - allocations
      Returns the allocations of the array

  - check_bounds
      Determines if the given index is within bounds and aborts if not

  - grow
      Doubles the capcity of the given array and copies over existing values
      within the array to the larger array

******************************************************************************/

#include "00_vector_lab.h"
#include <iostream>  // cout
#include <cstdlib>   // abort

namespace CS170
{
    /*!**************************************************************************
     \brief
        Default Constructor - sets all class variables to 0
    ****************************************************************************/
    Vector_Lab_0::Vector_Lab_0() : array_(0), size_(0), capacity_(0), allocs_(0)
    {
    }

    /*!**************************************************************************
     \brief
        Default Destructor - deletes the dynamic memory associated with the
        class
    ****************************************************************************/
    Vector_Lab_0::~Vector_Lab_0()
    {
        delete[] array_;
    }

    /*!**************************************************************************
     \brief
        Adds given value to the back of the array, pushing the existing values
        forward in the array

     \param value
        The value that is being added to the array
    ****************************************************************************/
    void Vector_Lab_0::push_back(int value)
    {
        // Checks if the array can contain another value
        if (size_ + 1 > capacity_)
            // If not, expand the array
            grow();

        // Set the first open index to the given value
        array_[size_] = value;

        // Increment size
        size_++;
    }

    /*!**************************************************************************
     \brief
        Adds given value to the front of the array, pushing the existing values
        back in the array

     \param value
        The value that is being added to the array
    ****************************************************************************/
    void Vector_Lab_0::push_front(int value)
    {
        // Checks if the array can contain another value
        if (size_ + 1 > capacity_)
            // If not, expand the array
            grow();

        // Iterates backwards over the array starting at the first open space (end)
        for (unsigned int i = size_; i > 0; i--)
            // Moves all of the values over to the right by one
            array_[i] = array_[i - 1];

        // Sets the first index to the given value
        array_[0] = value;

        // Increments size
        size_++;
    }

    /*!**************************************************************************
    \brief
       Returns the value stored at the given index

    \param index
       The index that is trying to be accessed

    \return
       The valud at the given index of the array
    ****************************************************************************/
    int Vector_Lab_0::operator[](unsigned index) const
    {
        check_bounds(index);
        return array_[index];
    }

    /*!**************************************************************************
    \brief
       Returns the address for the given index

    \param index
       The index that is trying to be accessed

    \return
       The address for the given index
    ****************************************************************************/
    int& Vector_Lab_0::operator[](unsigned index)
    {
        check_bounds(index);
        int* temp = &array_[index];
        return *temp;
    }

    /*!**************************************************************************
    \brief
       Deletes the class array and sets size and capacity to 0
    ****************************************************************************/
    void Vector_Lab_0::clear()
    {
        delete[] array_;
        array_ = NULL;
        size_ = capacity_ = 0;
    }

    /*!**************************************************************************
    \brief
       Returns true / false based on whether the size of the array is 0

    \return
       A bool notated whether the array is empty or not
    ****************************************************************************/
    bool Vector_Lab_0::empty() const
    {
        return size_ == 0;
    }

    /*!**************************************************************************
    \brief
       Returns the size of the array

    \return
       The size of the array
    ****************************************************************************/
    unsigned Vector_Lab_0::size() const
    {
        return size_;
    }

    /*!**************************************************************************
    \brief
       Returns the capacity of the array

    \return
       The capacity of the array
    ****************************************************************************/
    unsigned Vector_Lab_0::capacity() const
    {
        return capacity_;
    }

    /*!**************************************************************************
    \brief
       Returns the allocations of the array

    \return
       The allocations of the array
    ****************************************************************************/
    unsigned Vector_Lab_0::allocations() const
    {
        return allocs_;
    }

    /////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    // private 
    /////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    /*!**************************************************************************
    \brief
       Determines if the given index is within bounds and aborts if not

    \param index
       The given index that is being checked
    ****************************************************************************/
    void Vector_Lab_0::check_bounds(unsigned index) const
    {
        // Don't have to check for < 0 because index is unsigned
        if (index >= size_)
        {
            std::cout << "Attempting to access index " << index << ".";
            std::cout << " The size of the array is " << size_ << ". Aborting...\n";
            std::abort();
        }
    }

    /*!**************************************************************************
    \brief
       Doubles the capcity of the given array and copies over existing values
       within the array to the larger array
    ****************************************************************************/
    void Vector_Lab_0::grow()
    {
        // Double the capacity
        if (capacity_)
            capacity_ *= 2;
        else
            capacity_ = 1;

        // Dynamically allocate a new array with the double capacity
        int* temp = new int[capacity_]();

        // Iterate over current class array
        for (unsigned int i = 0; i < size_; i++)
            // Set the corresponding indexes in the new array
            temp[i] = array_[i];

        // Free the current array
        delete[] array_;

        // Set the class array to be the new array
        array_ = temp;

        // Increment the allocations
        allocs_++;
    }

} // namespace CS170

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
void V0_Print(const CS170::Vector_Lab_0& array, bool newline = true)
{
    for (unsigned i = 0; i < array.size(); i++)
    {
        std::cout << array[i] << "  ";
    }

    std::cout << "(size=" << array.size() << ", capacity=" <<
        array.capacity() << ", allocs=" <<
        array.allocations() << ")";
    if (newline)
        std::cout << std::endl;
}


void V0_TestPush()
{
    std::cout << "\n********** TestPush **********\n";
    CS170::Vector_Lab_0 a;
    std::cout << "Empty array:\n";
    V0_Print(a);

    std::cout << "push_back 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        V0_Print(a);
    }

    std::cout << "push_front 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_front(i);
        V0_Print(a);
    }
}

void V0_TestSubscripts()
{
    std::cout << "\n********** TestSubscripts **********\n";
    CS170::Vector_Lab_0 a;

    std::cout << "push_back 10 even integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(2 * i);

    V0_Print(a);

    std::cout << "multiple each value by 3:\n";
    for (unsigned i = 0; i < 10; i++)
        a[i] = a[i] * 3;
    V0_Print(a);
}

void V0_TestSubscript1()
{
    std::cout << "\n********** TestSubscript1 **********\n";
    CS170::Vector_Lab_0 a;

    int ia[] = { 2, 4, 6, 6, 8, 10, 6, 12, -6, 14, 16, 6, 6 };
    size_t size = sizeof(ia) / sizeof(*ia);
    std::cout << "push_back 10 even integers:\n";
    for (size_t i = 0; i < size; i++)
        a.push_back(ia[i]);
    V0_Print(a);

    unsigned index = a.size() / 2;
    std::cout << "using subscript: a[" << index << "]" << std::endl;
    std::cout << "a[" << index << "] = " << a[index] << std::endl;
}

void V0_TestSubscript2()
{
    std::cout << "\n********** TestSubscript2 **********\n";
    CS170::Vector_Lab_0 a;

    int ia[] = { 2, 4, 6, 6, 8, 10, 6, 12, -6, 14, 16, 6, 6 };
    size_t size = sizeof(ia) / sizeof(*ia);
    std::cout << "push_back 10 even integers:\n";
    for (size_t i = 0; i < size; i++)
        a.push_back(ia[i]);
    V0_Print(a);

#if 0
    int index = a.size() * 2;
    std::cout << "using subscript: a[" << index << "]" << std::endl;
    std::cout << "a[" << index << "] = " << a[index] << std::endl;
#endif
}

void V0_TestALot1()
{
    std::cout << "\n********** TestALot1 **********\n";
    CS170::Vector_Lab_0 a;

    std::cout << "push_back 10 even integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(2 * i);
    V0_Print(a);

    std::cout << "clear:\n";
    a.clear();
    V0_Print(a);

    std::cout << "push_back 10 odd integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(2 * i + 1);
    V0_Print(a);

    std::cout << "push_back 10 more integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(i);
    V0_Print(a);

    std::cout << "clear:\n";
    a.clear();
    V0_Print(a);
}

void vector_lab_0(void)
{
    V0_TestPush();
    V0_TestSubscripts();
    V0_TestSubscript1();
    V0_TestALot1();
}