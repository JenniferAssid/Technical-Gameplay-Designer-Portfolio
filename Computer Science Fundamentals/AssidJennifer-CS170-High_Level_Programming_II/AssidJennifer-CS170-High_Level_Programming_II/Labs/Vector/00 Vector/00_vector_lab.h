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

///////////////////////////////////////////////////////////////////////////////
#ifndef VECTOR_LAB_0_H
#define VECTO_LAB_0R_H
///////////////////////////////////////////////////////////////////////////////

namespace CS170
{
    /*!**************************************************************************
      \brief
        Class that houses functions and variables necessary to create, analyize,
        and maintain an array of numbers
    ****************************************************************************/
    class Vector_Lab_0
    {
    public:

        /*!************************************************************************
        \brief
          Default Constructor - sets all class variables to 0
        **************************************************************************/
        Vector_Lab_0();

        /*!************************************************************************
        \brief
          Default Destructor - deletes the dynamic memory associated with the
          class
        **************************************************************************/
        ~Vector_Lab_0();

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

        /*!**************************************************************************
        \brief
           Determines if the given index is within bounds and aborts if not

        \param index
           The given index that is being checked
        ****************************************************************************/
        void check_bounds(unsigned index) const;

        /*!************************************************************************
        \brief
           Doubles the capcity of the given array and copies over existing values
           within the array to the larger array
        **************************************************************************/
        void grow();
    };

} // namespace CS170

void vector_lab_0(void);

#endif // VECTOR_H