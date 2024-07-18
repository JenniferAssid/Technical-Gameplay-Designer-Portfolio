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

#include "01_vector_lab.h"
#include <iostream>  // cout
#include <cstdlib>   // abort

namespace CS170
{
    /*!**************************************************************************
     \brief
        Default Constructor - sets all class variables to 0
    ****************************************************************************/
    Vector_Lab_1::Vector_Lab_1() : array_(0), size_(0), capacity_(0), allocs_(0)
    {
    }

    /*!************************************************************************
      \brief
        Copy Constructor - creates a copy of the given class

      \param rhs
        The vector that is being copied
      **************************************************************************/
    Vector_Lab_1::Vector_Lab_1(const Vector_Lab_1& rhs)
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
    Vector_Lab_1::Vector_Lab_1(const int array[], unsigned int size)
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
    Vector_Lab_1::~Vector_Lab_1()
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
    void Vector_Lab_1::push_back(int value)
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
    void Vector_Lab_1::push_front(int value)
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
    void Vector_Lab_1::pop_back()
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
    void Vector_Lab_1::pop_front()
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
    void Vector_Lab_1::insert(int value, unsigned int position)
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
    void Vector_Lab_1::remove(int value)
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
    Copy assignment operator

  \param rhs
     Vector that is being assigned

  \return
     Address to the vector that is what the other vector is being set equal to
  ***************************************************************************/
    Vector_Lab_1& Vector_Lab_1::operator= (const Vector_Lab_1& rhs)
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
    Vector_Lab_1& Vector_Lab_1::operator+=(const Vector_Lab_1& rhs)
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
    Vector_Lab_1 operator+(Vector_Lab_1 lhs, const Vector_Lab_1& rhs)
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
    int Vector_Lab_1::operator[](unsigned index) const
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
    int& Vector_Lab_1::operator[](unsigned index)
    {
        check_bounds(index);
        return array_[index];
    }

    /*!**************************************************************************
    \brief
       Deletes the class array and sets size and capacity to 0
    ****************************************************************************/
    void Vector_Lab_1::clear()
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
    bool Vector_Lab_1::empty() const
    {
        return size_ == 0;
    }

    /*!**************************************************************************
    \brief
       Returns the size of the array

    \return
       The size of the array
    ****************************************************************************/
    unsigned Vector_Lab_1::size() const
    {
        return size_;
    }

    /*!**************************************************************************
    \brief
       Returns the capacity of the array

    \return
       The capacity of the array
    ****************************************************************************/
    unsigned Vector_Lab_1::capacity() const
    {
        return capacity_;
    }

    /*!**************************************************************************
    \brief
       Returns the allocations of the array

    \return
       The allocations of the array
    ****************************************************************************/
    unsigned Vector_Lab_1::allocations() const
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
    void Vector_Lab_1::check_bounds(unsigned index) const
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
    void Vector_Lab_1::grow()
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
void V1_Print(const CS170::Vector_Lab_1& array, bool newline = true)
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

void V1_TestPush()
{
    std::cout << "\n********** TestPush **********\n";
    CS170::Vector_Lab_1 a;
    std::cout << "Empty array:\n";
    V1_Print(a);

    std::cout << "push_back 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        V1_Print(a);
    }

    std::cout << "push_front 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_front(i);
        V1_Print(a);
    }

    std::cout << "pop_front until empty:\n";
    while (!a.empty())
    {
        a.pop_front();
        V1_Print(a);
    }
}

void V1_TestPop()
{
    std::cout << "\n********** TestPop **********\n";
    CS170::Vector_Lab_1 a;
    std::cout << "Empty array:\n";
    V1_Print(a);

    std::cout << "push_back 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        V1_Print(a);
    }

    std::cout << "push_front 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_front(i);
        V1_Print(a);
    }

    std::cout << "pop_back/pop_front:\n";
    a.pop_back();
    a.pop_front();
    V1_Print(a);

    std::cout << "pop_back/front until empty:\n";
    while (!a.empty())
    {
        a.pop_front();
        a.pop_back();
        V1_Print(a);
    }
}

void V1_TestRemove()
{
    std::cout << "\n********** TestRemove **********\n";
    CS170::Vector_Lab_1 a;
    std::cout << "Empty array:\n";
    V1_Print(a);

    std::cout << "push_back 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        V1_Print(a);
    }

    std::cout << "remove(0), remove(3):\n";
    a.remove(0);
    a.remove(3);
    V1_Print(a);
}

void V1_TestInsert1()
{
    std::cout << "\n********** TestInsert1 **********\n";
    CS170::Vector_Lab_1 a;
    std::cout << "Empty array:\n";
    V1_Print(a);

    std::cout << "push_back 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        V1_Print(a);
    }

    std::cout << "insert(99, 3):\n";
    a.insert(99, 3);
    V1_Print(a);
    std::cout << "insert(98, 0):\n";
    a.insert(98, 0);
    V1_Print(a);
    std::cout << "insert(97, 6):\n";
    a.insert(97, 6);
    V1_Print(a);
    std::cout << "insert(96, 3):\n";
    a.insert(96, 3);
    V1_Print(a);
}

void V1_TestSubscripts()
{
    std::cout << "\n********** TestSubscripts **********\n";
    CS170::Vector_Lab_1 a;

    std::cout << "push_back 10 even integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(2 * i);

    V1_Print(a);

    std::cout << "multiple each value by 3:\n";
    for (unsigned i = 0; i < 10; i++)
        a[i] = a[i] * 3;
    V1_Print(a);
}

void V1_TestArray()
{
    std::cout << "\n********** TestArray **********\n";
    int ia[] = { 2, 4, 6, 6, 8, 10, 6, 12, -6, 14, 16, 6, 6 };
    unsigned size = static_cast<unsigned>(sizeof(ia) / sizeof(*ia));
    std::cout << "Construct from int array:\n";
    CS170::Vector_Lab_1 a(ia, size);
    V1_Print(a);
}

void V1_TestSubscript1()
{
    std::cout << "\n********** TestSubscript1 **********\n";
    int ia[] = { 2, 4, 6, 6, 8, 10, 6, 12, -6, 14, 16, 6, 6 };
    unsigned size = static_cast<unsigned>(sizeof(ia) / sizeof(*ia));
    std::cout << "Construct from int array:\n";
    const CS170::Vector_Lab_1 a(ia, size);
    V1_Print(a);

    unsigned index = a.size() / 2;
    std::cout << "using subscript: a[" << index << "]" << std::endl;
    std::cout << "a[" << index << "] = " << a[index] << std::endl;
}

void V1_TestSubscript2()
{
    std::cout << "\n********** TestSubscript2 **********\n";
    int ia[] = { 2, 4, 6, 6, 8, 10, 6, 12, -6, 14, 16, 6, 6 };
    unsigned size = static_cast<unsigned>(sizeof(ia) / sizeof(*ia));
    std::cout << "Construct from int array:\n";
    const CS170::Vector_Lab_1 a(ia, size);
    V1_Print(a);

#if 0
    int index = a.size() * 2; // illegal
    std::cout << "using subscript: a[" << index << "]" << std::endl;
    std::cout << "a[" << index << "] = " << a[index] << std::endl;
#endif
}

void V1_TestInsert2()
{
    std::cout << "\n********** TestInsert2 **********\n";
    int ia[] = { 2, 4, 6, 6, 8, 10, 6, 12, -6, 14, 16, 6, 6 };
    unsigned size = static_cast<unsigned>(sizeof(ia) / sizeof(*ia));
    std::cout << "Construct from int array:\n";
    CS170::Vector_Lab_1 a(ia, size);
    V1_Print(a);
    std::cout << "Allocations: " << a.allocations() << std::endl;

#if 0
    int index = a.size() * 2; // illegal
    std::cout << "insert integer at index " << index << ":\n";
    a.insert(99, index);
#endif
}

void V1_TestALot1()
{
    std::cout << "\n********** TestALot1 **********\n";
    CS170::Vector_Lab_1 a;

    std::cout << "push_back 10 even integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(2 * i);
    V1_Print(a);

    std::cout << "clear:\n";
    a.clear();
    V1_Print(a);

    std::cout << "push_back 10 odd integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(2 * i + 1);
    V1_Print(a);

    std::cout << "empty/fill with 10 ints 3 times:\n";
    for (int i = 0; i < 3; i++)
    {
        while (!a.empty())
            a.pop_back();

        for (int j = 0; j < 10; j++)
            a.push_back(j);
    }
    V1_Print(a);

    std::cout << "remove all but 3 integers:\n";
    while (a.size() > 3)
        a.pop_back();
    V1_Print(a);

    std::cout << "clear:\n";
    a.clear();
    V1_Print(a);

    std::cout << "push_back 8 more integers:\n";
    for (int i = 0; i < 8; i++)
        a.push_back(i);
    V1_Print(a);
}

void V1_TestCopy()
{
    std::cout << "\n********** TestCopy **********\n";
    CS170::Vector_Lab_1 a;

    std::cout << "push_back 10 even integers:\n";
    for (int i = 0; i < 10; i++)
        a.push_back(2 * i);

    std::cout << "Copy: b(a), print a,b\n";
    CS170::Vector_Lab_1 b(a);
    V1_Print(a);
    V1_Print(b);

    std::cout << "Copy: c(b), print b,c\n";
    const CS170::Vector_Lab_1 c(b);
    V1_Print(b);
    V1_Print(c);
}

void V1_TestAssign()
{
    std::cout << "\n********** TestAssign **********\n";
    CS170::Vector_Lab_1 a, b, c;

    std::cout << "push_back 10 integers into a,b and 5 into c:\n";
    for (int i = 0; i < 10; i++)
    {
        a.push_back(2 * i);
        b.push_back(i);
        if (i % 2)
            c.push_back(i * 3);
    }

    V1_Print(a);
    V1_Print(b);
    V1_Print(c);

    std::cout << "Assign: b = a, print a,b\n";
    b = a;
    V1_Print(a);
    V1_Print(b);

    std::cout << "Assign: a = a, print a\n";
    a = a;
    V1_Print(a);

    std::cout << "Assign: a = c, print a,c\n";
    a = c;
    V1_Print(a);
    V1_Print(c);

    std::cout << "Assign: c = b, print b,c\n";
    c = b;
    V1_Print(b);
    V1_Print(c);
}

void V1_TestAdd1()
{
    std::cout << "\n********** TestAdd1 **********\n";
    CS170::Vector_Lab_1 a, b;

    std::cout << "push_back 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        V1_Print(a);
    }

    std::cout << "push_front 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        b.push_front(10 * i);
        V1_Print(b);
    }

    std::cout << "c = a + b:\n";
    CS170::Vector_Lab_1 c;
    c = a + b;

    V1_Print(a);
    V1_Print(b);
    V1_Print(c);

    std::cout << "clear a,b print c\n";
    a.clear();
    b.clear();
    V1_Print(c);
}

void V1_TestAdd2()
{
    std::cout << "\n********** TestAdd2 **********\n";
    CS170::Vector_Lab_1 a, b;

    std::cout << "push_back 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        V1_Print(a);
    }

    std::cout << "push_front 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        b.push_front(10 * i);
        V1_Print(b);
    }

    std::cout << "a += b:\n";
    a += b;
    V1_Print(a);
    V1_Print(b);

    std::cout << "clear b print a\n";
    b.clear();
    V1_Print(a);
}

void V1_TestAdd3()
{
    std::cout << "\n********** TestAdd3 **********\n";
    CS170::Vector_Lab_1 a;

    std::cout << "push_back 5 integers:\n";
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        V1_Print(a);
    }

    std::cout << "a += a:\n";
    a += a;
    V1_Print(a);
}

void vector_lab_1(void)
{
    V1_TestPush();
    V1_TestPop();
    V1_TestRemove();
    V1_TestInsert1();
    V1_TestInsert2();
    V1_TestSubscripts();
    V1_TestArray();
    V1_TestSubscript1();
    V1_TestSubscript2();
    V1_TestInsert2();
    V1_TestALot1();
    V1_TestCopy();
    V1_TestAssign();
    V1_TestAdd1();
    V1_TestAdd2();
    V1_TestAdd3();
}