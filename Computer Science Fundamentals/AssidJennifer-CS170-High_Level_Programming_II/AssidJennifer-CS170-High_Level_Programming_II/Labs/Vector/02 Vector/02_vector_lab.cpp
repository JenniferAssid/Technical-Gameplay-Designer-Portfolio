/*!****************************************************************************
\file    Vector.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab 11
\date    04-03-2021

\brief
  This program houses functions necessary to create, analyize, and maintain
  an array of numbers within a class Vector

  The functions include:

  - Subscript Error
     Throws a subscript error

  - GetSubscript
     Returns the subscript stored

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

  - insert
      Inserts a new element at the specified positions. Causes an abort() is
      the position is invalid.

  - remove
      Removes an element with the specified value (first occurence only)

  - swapv
      Swaps the contents of two vector

  - reverse
      Reverses the values within the class vector

  - operator==
      Checks if two vectors are the same

  - shrink_to_fit
      Shrinks the array to fit the size of values

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

#include "02_vector_lab.h"
#include "02_vector_lab.h"
#include <iostream>  // cout
#include <cstdlib>   // abort

/*!**************************************************************************
   \brief
      Helper function - swaps the values of the given two variables

   \param a
      First value that is getting swapped

   \param b
      Second value that is getting swapped
  ****************************************************************************/
template <typename T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

namespace CS170
{
    /*!**************************************************************************
     \brief
        Default Constructor - sets all class variables to 0
    ****************************************************************************/
    Vector_Lab_2::Vector_Lab_2() : array_(0), size_(0), capacity_(0), allocs_(0)
    {
    }

    /*!************************************************************************
      \brief
        Copy Constructor - creates a copy of the given class

      \param rhs
        The vector that is being copied
      **************************************************************************/
    Vector_Lab_2::Vector_Lab_2(const Vector_Lab_2& rhs)
    {
        array_ = new int[rhs.size_]();
        size_ = 0;
        capacity_ = rhs.size_;
        allocs_ = 1;

        for (unsigned int i = 0; i < rhs.size_; i++)
            push_back(rhs[i]);
    }

    /*!************************************************************************
    \brief
      Non-Default Constructor - creates a vector from a given array

    \param array
      Values that will be transferred into the array

    \param size
      Size of the given array
    **************************************************************************/
    Vector_Lab_2::Vector_Lab_2(const int array[], unsigned int size)
    {
        array_ = new int[size]();
        size_ = 0;
        capacity_ = size;
        allocs_ = 1;

        for (unsigned int i = 0; i < size; i++)
            push_back(array[i]);
    }

    /*!**************************************************************************
     \brief
        Default Destructor - deletes the dynamic memory associated with the
        class
    ****************************************************************************/
    Vector_Lab_2::~Vector_Lab_2()
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
    void Vector_Lab_2::push_back(int value)
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
    void Vector_Lab_2::push_front(int value)
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

    /*!************************************************************************
      \brief
         Removes the last element, does nothing if empty
     ***************************************************************************/
    void Vector_Lab_2::pop_back()
    {
        if (size_ == 0) return;

        this->array_[size_ - 1] = 0;
        size_--;
    }

    /*!************************************************************************
    \brief
      Removes the first element, shift the remaining elements to the left.
      Does nothing if empty.
   ***************************************************************************/
    void Vector_Lab_2::pop_front()
    {
        if (size_ == 0) return;

        for (unsigned int i = 0; i < (size_ - 1); i++)
            this->array_[i] = this->array_[i + 1];

        this->array_[size_] = 0;
        size_--;
    }

    /*!************************************************************************
   \brief
     Inserts a new element at the specified position. Causes an abort() if
     the postion is invalid.

   \param value
      The value that is being inserted

   \param position
     The location that the value is being inserted at
  ***************************************************************************/
    void Vector_Lab_2::insert(int value, unsigned int position)
    {
        check_bounds(position);

        if (size_ + 1 > capacity_) grow();

        if (position == 0)
        {
            push_front(value);
            return;
        }

        if (position == size_)
        {
            push_back(value);
            return;
        }

        for (unsigned int i = size_; i > position; i--)
            this->array_[i] = this->array_[i - 1];

        this->array_[position] = value;
        size_++;
    }

    /*!************************************************************************
   \brief
     Removes an elemnt with the specified value (first occurence only)

   \param value
      The value that is being removed
  ***************************************************************************/
    void Vector_Lab_2::remove(int value)
    {
        for (unsigned int i = 0; i < size_; i++)
        {
            if (this->array_[i] == value)
            {
                for (unsigned int j = i; j < size_; j++)
                {
                    this->array_[j] = this->array_[j + 1];
                }

                this->array_[size_] = 0;
                size_--;

                break;
            }
        }
    }

    /*!************************************************************************
     \brief
       Swaps the contents of other with this vector

     \param other
        The other vector whose values are being swapped with the class
    ***************************************************************************/
    void Vector_Lab_2::swapv(Vector_Lab_2& other)
    {
        swap(array_, other.array_);
        swap(capacity_, other.capacity_);
        swap(size_, other.size_);
        swap(allocs_, other.allocs_);
    }


    /*!************************************************************************
   \brief
     Reverses the order of the elements
  ***************************************************************************/
    void Vector_Lab_2::reverse()
    {
        if (size_ == 0) return;

        unsigned int start, end;
        start = 0;
        end = size_ - 1;

        while (start < end)
        {
            std::swap((*this)[start], (*this)[end]);

            start++;
            end--;
        }
    }

    /*!************************************************************************
   \brief
     Checks if the vectors are the same (they must be the same sizes)

    \param rhs
       Vector that is being compared to the class

   \return
      Bool as to whether the lists are the same
  ***************************************************************************/
    bool Vector_Lab_2::operator==(const Vector_Lab_2& rhs) const
    {
        if (rhs.size_ != size_) return false;

        for (unsigned int i = 0; i < size_; i++)
        {
            if (array_[i] != rhs.array_[i]) return false;
        }

        return true;
    }

    /*!************************************************************************
   \brief
     Reallocates an array to match the size of the number of elements.
  ***************************************************************************/
    void Vector_Lab_2::shrink_to_fit()
    {
        if (size_ == capacity_) return;

        int* temp = new int[size_]();

        for (unsigned int i = 0; i < size_; i++)
            temp[i] = array_[i];

        delete[] array_;

        array_ = temp;
        capacity_ = size_;
        allocs_++;
    }

    /*!************************************************************************
  \brief
    Copy assignment operator

  \param rhs
     Vector that is being assigned

  \return
     Address to the vector that is what the other vector is being set equal to
  ***************************************************************************/
    Vector_Lab_2& Vector_Lab_2::operator= (const Vector_Lab_2& rhs)
    {
        if (this == &rhs) return *this;

        for (unsigned int i = 0; i < size_; i++)
            array_[i] = 0;

        if (capacity_ < rhs.size_)
        {
            delete[] array_;
            array_ = new int[rhs.size_]();
            capacity_ = rhs.size_;
            allocs_++;
        }


        for (unsigned int i = 0; i < rhs.size_; i++)
            array_[i] = rhs.array_[i];

        size_ = rhs.size_;

        return *this;
    }

    /*!************************************************************************
    \brief
        Concatenates a vector onto the end of this vector

    \param rhs
        Vector that is being concatenated onto the class Vector

    \return
        Address to the vector that has been added to
    **************************************************************************/
    Vector_Lab_2& Vector_Lab_2::operator+=(const Vector_Lab_2& rhs)
    {
        unsigned int limit = rhs.size_;

        for (unsigned int i = 0; i < limit; i++)
            push_back(rhs.array_[i]);

        return *this;
    }

    /*!************************************************************************
    \brief
        Concatenates two vectors

    \param rhs
        Vector that is being concatenated onto the class Vector

    \param lhs
        Vector that is being concatenated on to

    \return
        The vector that is the contatenation of the two given vectors
    **************************************************************************/
    Vector_Lab_2 operator+(Vector_Lab_2 lhs, const Vector_Lab_2& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    /*!**************************************************************************
    \brief
       Returns the value stored at the given index

    \param index
       The index that is trying to be accessed

    \return
       The valud at the given index of the array
    ****************************************************************************/
    int Vector_Lab_2::operator[](unsigned index) const
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
    int& Vector_Lab_2::operator[](unsigned index)
    {
        check_bounds(index);
        return array_[index];
    }

    /*!**************************************************************************
    \brief
       Deletes the class array and sets size and capacity to 0
    ****************************************************************************/
    void Vector_Lab_2::clear()
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
    bool Vector_Lab_2::empty() const
    {
        return size_ == 0;
    }

    /*!**************************************************************************
    \brief
       Returns the size of the array

    \return
       The size of the array
    ****************************************************************************/
    unsigned Vector_Lab_2::size() const
    {
        return size_;
    }

    /*!**************************************************************************
    \brief
       Returns the capacity of the array

    \return
       The capacity of the array
    ****************************************************************************/
    unsigned Vector_Lab_2::capacity() const
    {
        return capacity_;
    }

    /*!**************************************************************************
    \brief
       Returns the allocations of the array

    \return
       The allocations of the array
    ****************************************************************************/
    unsigned Vector_Lab_2::allocations() const
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
    void Vector_Lab_2::check_bounds(unsigned index) const
    {
        // Don't have to check for < 0 because index is unsigned
        if (index >= size_)
            throw SubscriptError(index);
    }

    /*!**************************************************************************
    \brief
       Doubles the capcity of the given array and copies over existing values
       within the array to the larger array
    ****************************************************************************/
    void Vector_Lab_2::grow()
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
void Print(const CS170::Vector_Lab_2& array, bool newline = true)
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

void PrintPartial(const CS170::Vector_Lab_2& array, bool newline = true)
{
    for (unsigned i = 1; i < array.size(); i *= 2)
    {
        std::cout << array[i - 1] << "  ";
    }

    std::cout << "(size=" << array.size() << ", capacity=" <<
        array.capacity() << ", allocs=" <<
        array.allocations() << ")";
    if (newline)
        std::cout << std::endl;
}

void V2_TestSwap1(void)
{
    std::cout << "\n********** TestSwap1 **********\n";
    CS170::Vector_Lab_2 a, b, c;

    std::cout << "push_back integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(i + 1);
    for (int i = 0; i < 5; i++)
        b.push_back(10 * (i + 1));

    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);

    std::cout << "swapv a,b:\n";
    a.swapv(b);
    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);

    std::cout << "swapv a,c:\n";
    a.swapv(c);
    std::cout << "a: ";
    Print(a);
    std::cout << "c: ";
    Print(c);

    std::cout << "swapv b,b:\n";
    b.swapv(b);
    std::cout << "b: ";
    Print(b);
}

void V2_TestReverse1(void)
{
    int count = 10;
    std::cout << "\n********** TestReverse1 **********\n";
    CS170::Vector_Lab_2 a;

    std::cout << "push_back integers:\n";
    for (int i = 0; i < count; i++)
        a.push_back(i + 1);

    Print(a);
    a.reverse();
    std::cout << "Reversed:\n";
    Print(a);

    std::cout << "Remove last element:\n";
    a.pop_back();
    Print(a);
    a.reverse();
    std::cout << "Reversed:\n";
    Print(a);
}

void V2_TestReverse2(void)
{
    int count = 10;
    std::cout << "\n********** TestReverse2 **********\n";
    CS170::Vector_Lab_2 a;

    std::cout << "push_back integers:\n";
    for (int i = 0; i < count; i++)
        a.push_back(i + 1);

    Print(a);
    a.reverse();
    std::cout << "Reversed:\n";
    Print(a);

    while (!a.empty())
    {
        if (a.size() % 2) // odd
        {
            std::cout << "Remove last element:\n";
            a.pop_back();
        }
        else // even
        {
            std::cout << "Remove first element:\n";
            a.pop_front();
        }
        Print(a);
        a.reverse();
        std::cout << "Reversed:\n";
        Print(a);
    }
}

void V2_TestEqual1(void)
{
    std::cout << "\n********** TestEqual1 **********\n";
    CS170::Vector_Lab_2 a, b, c;

    std::cout << "push_back integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(i + 1);
    for (int i = 0; i < 10; i++)
        b.push_back(i + 1);

    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);

    if (a == b)
        std::cout << "a is equal to b\n";
    else
        std::cout << "a is NOT equal to b\n";

    std::cout << "remove last element of a:\n";
    a.pop_back();
    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);
    if (a == b)
        std::cout << "a is equal to b\n";
    else
        std::cout << "a is NOT equal to b\n";

    std::cout << "remove last element of b:\n";
    b.pop_back();
    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);
    if (a == b)
        std::cout << "a is equal to b\n";
    else
        std::cout << "a is NOT equal to b\n";

    std::cout << "change last element of b to 100:\n";
    b[b.size() - 1] = 100;
    std::cout << "a: ";
    Print(a);
    std::cout << "b: ";
    Print(b);
    if (a == b)
        std::cout << "a is equal to b\n";
    else
        std::cout << "a is NOT equal to b\n";
}

void V2_TestSubscriptEx(void)
{
    std::cout << "\n********** TestSubscriptEx **********\n";
    int ia[] = { 2, 4, 6, 6, 8, 10, 6, 12, -6, 14, 16, 6, 6 };
    unsigned size = static_cast<unsigned>(sizeof(ia) / sizeof(*ia));
    std::cout << "Construct from int array:\n";

    const CS170::Vector_Lab_2 a(ia, size); // const
    CS170::Vector_Lab_2 b(ia, size);       // non-const
    Print(a);
    Print(b);

    try
    {
        unsigned index = a.size() * 2; // illegal
        std::cout << "accessing subscript on const vector: a[" << index << "]" << std::endl;
        std::cout << "a[" << index << "] = " << a[index] << std::endl;
    }
    catch (const CS170::SubscriptError& se)
    {
        std::cout << "Bad subscript: " << se.GetSubscript() << std::endl;
    }

    try
    {
        unsigned index = b.size() * 2; // illegal
        std::cout << "accessing subscript on non-const vector: b[" << index << "]" << std::endl;
        std::cout << "b[" << index << "] = " << b[index] << std::endl;
    }
    catch (const CS170::SubscriptError& se)
    {
        std::cout << "Bad subscript: " << se.GetSubscript() << std::endl;
    }

}

void V2_TestInsertEx(void)
{
    std::cout << "\n********** TestInsertEx **********\n";
    int ia[] = { 2, 4, 6, 6, 8, 10, 6, 12, -6, 14, 16, 6, 6 };
    unsigned size = static_cast<unsigned>(sizeof(ia) / sizeof(*ia));
    std::cout << "Construct from int array:\n";
    CS170::Vector_Lab_2 a(ia, size);
    Print(a);

    try
    {
        unsigned index = a.size() * 3; // illegal
        std::cout << "insert integer at index " << index << ":\n";
        a.insert(99, index);
    }
    catch (const CS170::SubscriptError& se)
    {
        std::cout << "Bad subscript: " << se.GetSubscript() << std::endl;
    }
}

void V2_TestSwapStress(void)
{
    std::cout << "\n********** TestSwapStress **********\n";
    CS170::Vector_Lab_2 a, b, c;

    int count = 1000000;

    std::cout << "Pushing back...\n";
    for (int i = 0; i < count; i++)
    {
        a.push_back(i);
        b.push_back(i * 2);
        c.push_back(i * 3);
    }

    std::cout << "Swapping...\n";
    CS170::Vector_Lab_2 x;
    for (int i = 0; i < 1000001; i++)
    {
        a.swapv(b);
        b.swapv(c);
        c.swapv(a);
    }

    PrintPartial(a);
    PrintPartial(b);
    PrintPartial(c);

    std::cout << "Done...\n";
}

void V2_TestShrink1(void)
{
    std::cout << "\n********** TestShrink1 **********\n";
    CS170::Vector_Lab_2 a;

    std::cout << "push_back 8 integers:\n";
    for (int i = 0; i < 8; i++)
        a.push_back(i);
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);

    std::cout << "pop last 3:\n";
    a.pop_back();
    a.pop_back();
    a.pop_back();
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);

    std::cout << "push_back one integer:\n";
    a.push_back(100);
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);

    std::cout << "clear:\n";
    a.clear();
    Print(a);

    std::cout << "shrink:\n";
    a.shrink_to_fit();
    Print(a);
}

void vector_lab_2(void)
{
    V2_TestSwap1();
    V2_TestReverse1();
    V2_TestReverse2();
    V2_TestEqual1();
    V2_TestSubscriptEx();
    V2_TestInsertEx();
    V2_TestSwapStress();
    V2_TestShrink1();
}