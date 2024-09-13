/*!****************************************************************************
\file    Vector.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Lab 12
\date    04-10-2021

\brief
  This program houses functions necessary to create, analyize, and maintain
  an array of numbers within a class Vector

  The functions include:

  - swap
     Helper function - swaps the values of the given two variables

  - Subscript Error
     Throws a subscript error

  - GetSubscript
     Returns the subscript stored

  - selection_sort
     Sorts the elements using a selection sort

  - bsearch
     Searches the vector using binary search instead of linear search

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

#include "03_vector_lab.h"
#include <iostream>  // cout
#include <cstdlib>   // abort

/*!**************************************************************************
   \brief
      Helper Variable - Used for the selection sort functionality
  ****************************************************************************/
#define INT_MAX 2147483647

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
    Vector_Lab_3::Vector_Lab_3() : array_(0), size_(0), capacity_(0), allocs_(0)
    {
        this->sortResult.compares = this->sortResult.swaps = 0;
    }

    /*!************************************************************************
      \brief
        Copy Constructor - creates a copy of the given class

      \param rhs
        The vector that is being copied
      **************************************************************************/
    Vector_Lab_3::Vector_Lab_3(const Vector_Lab_3& rhs)
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
    Vector_Lab_3::Vector_Lab_3(const int array[], unsigned int size)
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
    Vector_Lab_3::~Vector_Lab_3()
    {
        delete[] array_;
    }

    /*!**************************************************************************
     \brief
        Sorts the elements using a selection sort

    \return
       The number of swaps and comparions that occured
    ****************************************************************************/
    Vector_Lab_3::SortResult Vector_Lab_3::selection_sort()
    {
        // Set the sort result values to their default positions (0)
        this->sortResult.compares = this->sortResult.swaps = 0;

        // Iterate over the array (primary)
        for (unsigned int i = 0; i < size_; i++)
        {
            // Set the tracker variables to their necessary values
            int smallestIndex = INT_MAX;
            int smallestValue = array_[i];
            // Iterate over the array (secondary)
            for (unsigned int j = i + 1; j < size_; j++)
            {
                // Increase the sort result compares
                this->sortResult.compares++;

                // Compare the current index to the stored value
                if (array_[j] < smallestValue)
                {
                    // Adjust variables as necessary
                    smallestIndex = j;
                    smallestValue = array_[smallestIndex];
                }
            }

            // If a new index was found...
            if (smallestIndex != INT_MAX)
            {
                // Swap the corresponding indexes
                this->sortResult.swaps++;
                swap(array_[i], array_[smallestIndex]);
            }
        }

        // Return the result
        return this->sortResult;
    }

    /*!**************************************************************************
     \brief
       Searches the vector using binary search instead of linear search

    \param value
       The data being searched for

    \return
       Returns the conclusion of the search (index)
    ****************************************************************************/
    int Vector_Lab_3::bsearch(int value) const
    {
        unsigned int start, end, middle;
        start = 0;
        end = size_ - 1;
        middle = end / 2;

        // Starts at the middle of the vector
        unsigned int limit = size_;
        while (limit)
        {
            if (array_[middle] == value)
                return middle;

            // Determines which direction the view box needs to go
            else if (array_[middle] > value)
                end = middle - 1;

            else if (array_[middle] < value)
                start = middle + 1;

            // Checks if the list has been iterated through
            if (middle == 0 || middle >= (size_ - 1)) return NO_INDEX;

            middle = ((end - start) / 2) + start;

            // Stops the possiblity of an infinity loop
            limit--;
        }

        return NO_INDEX;
    }

    /*!**************************************************************************
     \brief
        Adds given value to the back of the array, pushing the existing values
        forward in the array

     \param value
        The value that is being added to the array
    ****************************************************************************/
    void Vector_Lab_3::push_back(int value)
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
    void Vector_Lab_3::push_front(int value)
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
    void Vector_Lab_3::pop_back()
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
    void Vector_Lab_3::pop_front()
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
    void Vector_Lab_3::insert(int value, unsigned int position)
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
    void Vector_Lab_3::remove(int value)
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
    void Vector_Lab_3::swapv(Vector_Lab_3& other)
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
    void Vector_Lab_3::reverse()
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
    bool Vector_Lab_3::operator==(const Vector_Lab_3& rhs) const
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
    void Vector_Lab_3::shrink_to_fit()
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
    Vector_Lab_3& Vector_Lab_3::operator= (const Vector_Lab_3& rhs)
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
    Vector_Lab_3& Vector_Lab_3::operator+=(const Vector_Lab_3& rhs)
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
    Vector_Lab_3 operator+(Vector_Lab_3 lhs, const Vector_Lab_3& rhs)
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
    int Vector_Lab_3::operator[](unsigned index) const
    {
        check_bounds(index);
        return array_[index];
    }

    /*!**************************************************************************
    \brief
       Returns the address to the value stored at a given index

    \param index
       Index that is trying to be accessed by the user

    \return
       Address to the value within the index
    ****************************************************************************/
    int& Vector_Lab_3::operator[](unsigned index)
    {
        check_bounds(index);
        return array_[index];
    }

    /*!**************************************************************************
    \brief
       Deletes the class array and sets size and capacity to 0
    ****************************************************************************/
    void Vector_Lab_3::clear()
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
    bool Vector_Lab_3::empty() const
    {
        return size_ == 0;
    }

    /*!**************************************************************************
    \brief
       Returns the size of the array

    \return
       The size of the array
    ****************************************************************************/
    unsigned Vector_Lab_3::size() const
    {
        return size_;
    }

    /*!**************************************************************************
    \brief
       Returns the capacity of the array

    \return
       The capacity of the array
    ****************************************************************************/
    unsigned Vector_Lab_3::capacity() const
    {
        return capacity_;
    }

    /*!**************************************************************************
    \brief
       Returns the allocations of the array

    \return
       The allocations of the array
    ****************************************************************************/
    unsigned Vector_Lab_3::allocations() const
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
    void Vector_Lab_3::check_bounds(unsigned index) const
    {
        // Don't have to check for < 0 because index is unsigned
        if (index >= size_)
            throw SubscriptError_1(index);
    }

    /*!**************************************************************************
    \brief
       Doubles the capcity of the given array and copies over existing values
       within the array to the larger array
    ****************************************************************************/
    void Vector_Lab_3::grow()
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
// generate random numbers between low and high 
#include "PRNG.h"

bool gShowTime = false;

int RandomInt(int low, int high)
{
    int number = CS170::Utils::Random(low, high);
    return number;
}

void Shuffle(int* array, int size)
{
    for (int i = 0; i < size; i++)
    {
        int r = CS170::Utils::Random(0, 2 << 29) % size;
        int t = array[i];
        array[i] = array[r];
        array[r] = t;
    }
}

void V3_Print(const CS170::Vector_Lab_3& array, bool newline = true)
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

void PrintSortResults(const CS170::Vector_Lab_3::SortResult& results)
{
    std::cout << "Sorted: compares = " << results.compares;
    std::cout << ", swaps = " << results.swaps << std::endl;
}

void V3_TestSelectionSort1(int count)
{
    std::cout << "\n********** TestSelectionSort1 **********\n";
    CS170::Vector_Lab_3 a;

    std::cout << "push_back 10 random integers:\n";
    for (int i = 0; i < count; i++)
        a.push_back(RandomInt(1, count * 2));

    V3_Print(a);
    CS170::Vector_Lab_3::SortResult results = a.selection_sort();

    PrintSortResults(results);
    V3_Print(a);
}

void V3_TestSelectionSort2(int count)
{
    std::cout << "\n********** TestSelectionSort2 **********\n";
    CS170::Vector_Lab_3 a;

    std::cout << "push_back " << count << " integers:\n";
    for (int i = 0; i < count; i++)
        a.push_back(i);

    V3_Print(a);
    CS170::Vector_Lab_3::SortResult results = a.selection_sort();
    PrintSortResults(results);
    V3_Print(a);

    a.clear();
    std::cout << "push_back " << count << " integers:\n";
    for (int i = count; i > 0; i--)
        a.push_back(i);

    V3_Print(a);
    results = a.selection_sort();
    PrintSortResults(results);
    V3_Print(a);

    a.clear();
    std::cout << "push_back " << count << " integers:\n";
    for (int i = count; i > 0; i--)
        a.push_back(5);

    V3_Print(a);
    results = a.selection_sort();
    PrintSortResults(results);
    V3_Print(a);
}

void V3_TestSelectionSortStress(int count)
{
    std::cout << "\n********** TestSelectionSortStress **********\n";
    CS170::Vector_Lab_3 a;

    CS170::Utils::srand(6, 0);

    for (int i = 0; i < count; i++)
        a.push_back(RandomInt(1, count));

    CS170::Vector_Lab_3::SortResult results = a.selection_sort();

    PrintSortResults(results);
    for (unsigned i = 1; i < static_cast<unsigned>(count); i *= 2)
        std::cout << a[i] << "  ";
    std::cout << std::endl;
    //Print(a);
}

void V3_TestBSearch1(int count)
{
    std::cout << "\n********** TestBSearch1 **********\n";
    CS170::Vector_Lab_3 a, b;

    std::cout << "push_back 10 random integers:\n";
    for (unsigned i = 0; i < static_cast<unsigned>(count); i++)
    {
        a.push_back(RandomInt(1000, 9999));
        b.push_back(a[i]);
    }

    V3_Print(a);
    a.selection_sort();
    V3_Print(a);

    for (unsigned i = 0; i < a.size(); i++)
    {
        int index = a.bsearch(b[i]);
        if (index != CS170::Vector_Lab_3::NO_INDEX)
            std::cout << "Index of " << b[i] << " is " << index << std::endl;
        else
            std::cout << b[i] << " is not in the vector" << std::endl;
    }
    // Doesn't exist
    int index = a.bsearch(99);
    if (index != CS170::Vector_Lab_3::NO_INDEX)
        std::cout << "Index of 99 is " << index << std::endl;
    else
        std::cout << "99 is not in the vector" << std::endl;
}

void V3_TestBSearchStress(int count)
{
    std::cout << "\n********** TestBSearchStress **********\n";
    CS170::Vector_Lab_3 a;

    int* ia = new int[count * 2];

    std::cout << "Generating...\n";
    for (int i = 0; i < count * 2; i++)
        ia[i] = i;

    std::cout << "Shuffling...\n";
    Shuffle(ia, count * 2);

    std::cout << "Pushing back...\n";
    for (int i = 0; i < count; i++)
        a.push_back(ia[i]);

    //Print(a);
    a.selection_sort();
    //Print(a);

    std::cout << "Searching...\n";
    std::time_t start = std::clock();
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            int index = a.bsearch(ia[i]);
            if ((j == 0) && !(i % 1000))
                std::cout << "Index of " << ia[i] << " is " << index << std::endl;
        }
    }
    std::time_t end = std::clock();
    std::cout << "Done...\n";
    if (gShowTime)
        std::cout << "Elapsed time: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;

    delete[] ia;
}

void vector_lab_3(void)
{
    V3_TestSelectionSort1(10);
    V3_TestSelectionSort1(20);
    V3_TestSelectionSort2(10);
    V3_TestSelectionSort2(20);
    V3_TestBSearch1(10);
    V3_TestBSearch1(20);
}