#pragma once
/*!****************************************************************************
\file    Vector.h
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

  - Vector
      Copy Constructor - creates a copy of the given class

  - Vector
      Non-Default Constructor - creates a vector from a given array

  - ~Vector
      Default Destructor - deletes the dynamic memory associated with the class

  - push_back
      Adds given value to the back of the array, pushing the existing values
      forward in the array

  - push_front
      Adds given value to the front of the array, pushing the existing values
      back in the array

  - pop_back
      Removes the last element, does nothing if empty

  - pop_front
      Removes the first element, shift the remaining elements to the left.
      Does nothing if empty.

  - insert
      Inserts a new element at the specified position. Causes an abort() if
      the postion is invalid.

  - remove
      Removes an elemnt with the specified value (first occurence only)

  - operator=
      Copy assignment operator

  - operator+=
      Concatenates a vector onto the end of this vector

  - operator+
      Concatenates two Vectors

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

///////////////////////////////////////////////////////////////////////////////
#ifndef VECTOR_LAB_1_H
#define VECTOR_LAB_1_H
///////////////////////////////////////////////////////////////////////////////

namespace CS170
{
    /*!**************************************************************************
      \brief
        Class that houses functions and variables necessary to create, analyize,
        and maintain an array of numbers
    ****************************************************************************/
    class Vector_Lab_1
    {
    public:

        /*!************************************************************************
        \brief
          Default Constructor - sets all class variables to 0
        **************************************************************************/
        Vector_Lab_1();

        /*!************************************************************************
        \brief
          Copy Constructor - creates a copy of the given class

        \param rhs
          The vector that is being copied
        **************************************************************************/
        Vector_Lab_1(const Vector_Lab_1& rhs);

        /*!************************************************************************
        \brief
          Non-Default Constructor - creates a vector from a given array

        \param array
          Values that will be transferred into the array

        \param size
          Size of the given array
        **************************************************************************/
        Vector_Lab_1(const int array[], unsigned int size);

        /*!************************************************************************
        \brief
          Default Destructor - deletes the dynamic memory associated with the
          class
        **************************************************************************/
        ~Vector_Lab_1();

        /*!************************************************************************
        \brief
           Adds given value to the back of the array, pushing the existing values
           forward in the array

        \param value
           The value that is being added to the array
        **************************************************************************/
        void push_back(int value);

        /*!************************************************************************
        \brief
           Adds given value to the front of the array, pushing the existing values
           back in the array

        \param value
           The value that is being added to the array
       ***************************************************************************/
        void push_front(int value);

        /*!************************************************************************
        \brief
           Removes the last element, does nothing if empty
       ***************************************************************************/
        void pop_back();

        /*!************************************************************************
        \brief
          Removes the first element, shift the remaining elements to the left.
          Does nothing if empty.
       ***************************************************************************/
        void pop_front();

        /*!************************************************************************
       \brief
         Inserts a new element at the specified position. Causes an abort() if
         the postion is invalid.

       \param value
          The value that is being inserted

       \param position
         The location that the value is being inserted at
      ***************************************************************************/
        void insert(int value, unsigned int position);

        /*!************************************************************************
       \brief
         Removes an elemnt with the specified value (first occurence only)

       \param value
          The value that is being removed
      ***************************************************************************/
        void remove(int value);

        /*!************************************************************************
    \brief
      Copy assignment operator

    \param rhs
       Vector that is being assigned

    \return
       Address to the vector that is what the other vector is being set equal to
    ***************************************************************************/
        Vector_Lab_1& operator= (const Vector_Lab_1& rhs);

        /*!************************************************************************
        \brief
            Concatenates a vector onto the end of this vector

        \param rhs
            Vector that is being concatenated onto the class Vector

        \return
          Address to the vector that has been added to
        **************************************************************************/
        Vector_Lab_1& operator+=(const Vector_Lab_1& rhs);

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
        friend Vector_Lab_1 operator+(Vector_Lab_1 lhs, const Vector_Lab_1& rhs);

        /*!************************************************************************
        \brief
           Returns the value stored at the given index

        \param index
           The index that is trying to be accessed

        \return
           The value at the given index of the array
        **************************************************************************/
        int operator[](unsigned index) const;

        /*!************************************************************************
          \brief
             Returns the address for the given index

          \param index
             The index that is trying to be accessed

          \return
             The address for the given index
        **************************************************************************/
        int& operator[](unsigned index);

        /*!************************************************************************
          \brief
             Deletes the class array and sets size and capacity to 0
        **************************************************************************/
        void clear();

        /*!************************************************************************
        \brief
           Returns true / false based on whether the size of the array is 0

        \return
           A bool notated whether the array is empty or not
        **************************************************************************/
        bool empty() const;

        /*!************************************************************************
          \brief
             Returns the size of the array

          \return
             The size of the array
        **************************************************************************/
        unsigned size() const;

        /*!************************************************************************
        \brief
           Returns the capacity of the array

        \return
           The capacity of the array
        **************************************************************************/
        unsigned capacity() const;

        /*!************************************************************************
          \brief
             Returns the allocations of the array

          \return
             The allocations of the array
        **************************************************************************/
        unsigned allocations() const;

    private:
        /*!************************************************************************
          \brief
             The dynamically allocated array
        **************************************************************************/
        int* array_;

        /*!************************************************************************
          \brief
             The number of elements in the array
        **************************************************************************/
        unsigned size_;

        /*!************************************************************************
          \brief
             The allocated size of the array
        **************************************************************************/
        unsigned capacity_;

        /*!************************************************************************
          \brief
             Number of allocations (resizes)
        **************************************************************************/
        unsigned allocs_;

        /*!************************************************************************
        \brief
           Determines if the given index is within bounds and aborts if not

        \param index
           The given index that is being checked
        **************************************************************************/
        void check_bounds(unsigned index) const;

        /*!************************************************************************
        \brief
           Doubles the capcity of the given array and copies over existing values
           within the array to the larger array
        **************************************************************************/
        void grow();
    };

} // namespace CS170

void vector_lab_1(void);

#endif // VECTOR_H