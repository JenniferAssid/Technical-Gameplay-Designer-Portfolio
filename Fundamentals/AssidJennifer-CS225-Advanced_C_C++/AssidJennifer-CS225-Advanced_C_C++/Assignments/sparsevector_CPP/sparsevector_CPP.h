#pragma once
/*****************************************************************
* \file   sparsevector_CPP.h
* \author Jennifer Assid
* \par    email: jennifer.assid@digipen.edu
* \par    DigiPen Login: jennifer.assid
* \par    Course: CS 225
* \par    Section A
* \par    Assignment 03
* \date   23/10/2021
* \brief
*
*      Contains functions needed to create the SparseVector and
*      ElementProxy classes.
*
*********************************************************************/

#include <iostream>

namespace CS225
{
    /**
    * The elements of the sparsevector.
    */
    struct ElementNode
    {
        int data;
        int pos;
        struct ElementNode* next;
    };

    //forward declaration
    class ElementProxy;

    /**
    * Maintains a "linked list" where 0 values aren't permitted.
    */
    class SparseVector
    {
    public:
        /**
        * Default constructor for the SparseVector class.
        *
        */
        SparseVector() : pHead(0), dimension(0) {}
        /**
         * Copy constructor for the SparseVector class.
         *
         * \param rhs SparseVector class being copied
         */
        SparseVector(const SparseVector& rhs);
        /**
         * Default destructor for the SparseVector class.
         *
         */
        ~SparseVector();
        /**
         * Assignment operator for the SparseVector class.
         *
         * \param rhs SparseVector which current class is being set to
         * \return The current class now being a copy of the given SparseVector
         */
        SparseVector& operator=(const SparseVector& rhs);
        /**
         * Get the value from the SparseVector list at the given postion.
         *
         * \param pos Given position within the SparseVector
         * \return The value stored at the given position
         */
        int Get(long pos) const;
        /**
         * Insert the given value into the given position within the SparseVector.
         *
         * \param val The given value being added to the list
         * \param pos The position at which the value will be added
         */
        void Insert(int val, long pos);
        /**
         * Delete the value at the given postion.
         *
         * \param pos The given position which is being deleted
         */
        void Delete(long pos);
        /**
         * Prints the raw version of the SparseVector.
         *
         */
        void PrintRaw() const
        { //used for grading
            ElementNode* curr = pHead;
            std::cout << "Raw vector: ";
            while (curr)
            {
                std::cout << "(" << curr->data << ", " << curr->pos << ")";
                curr = curr->next;
            }
            std::cout << std::endl;
        }
        friend std::ostream& operator<<(std::ostream& out, const SparseVector
            & v);//implemented
        /**
         * Add the given SparseVector to the current SparseVector.
         *
         * \param rhs Given class being concatenated onto the current class
         * \return The current class which has been added with the given class
         */
        SparseVector& operator+=(const SparseVector& rhs);
        /**
         * Add the given SparseVector and the current SparseVector together.
         *
         * \param rhs Given class being added onto the current class
         * \return A new SparseVector which is the sum of the two classes
         */
        SparseVector operator+(const SparseVector& rhs);
        /**
         * Subtract the given class from the current class.
         *
         * \param rhs Given class being subtracted from the current class
         * \return The current class which is now the difference
         */
        SparseVector& operator-=(const SparseVector& rhs);
        /**
         * Multiply two SparseVector classes together to gain the dot product.
         *
         * \param rhs The second class being multiplied
         * \return Dot product of the two classes
         */
        int operator*(SparseVector& rhs);
        /**
         * Multiply the SparseVector by the given scalar.
         *
         * \param scalar What the SparseVector is getting multiplied by
         * \return A new SparseVector which has been scaled by the given scalar value
         */
        SparseVector operator*(int scalar);
        /**
         * Get the ElementProxy class from the given position in the SparseVector.
         * Allows the stored value to be modified.
         *
         * \param pos The given position
         * \return The ElementProxy class associated with the give position
         */
        ElementProxy operator[](long pos);
        /**
         * Get the value from the given position in the SparseVector.
         * Doesn't allow the stored value to be modified.
         *
         * \param pos The given position
         * \return The value stored at the position
         */
        int operator[](long pos) const;
    private:
        /**
         * The head of the sparsevector.
         */
        ElementNode* pHead;
        /**
         * The dimension of the SparseVector.
         */
        long dimension;
    };

    /**
     * Multiply a SparseVector by a scalar.
     */
    SparseVector operator*(int scalar, SparseVector rhs);
    /**
     * Multiply two SparseVectors together to get the dot product.
     */
    int operator*(const SparseVector& lhs, const SparseVector& rhs);

    //uncomment when done with basic class functionality and ready to implement Proxy
    class ElementProxy
    {
    public:
        /**
         * Non-Default Contructor. Sets the private variables as needed.
         *
         * \param v The SparseVector the class is tied too
         * \param pos The postiion which the class is associated with
         */
        ElementProxy(SparseVector& v, long pos) : v(v), pos(pos) {}
        /**
         * Assignment operator for the ElementProxy class.
         *
         * \param rhs ElementProxy class the current class is being set equal to
         * \return The current class (which is now a copy of the given class)
         */
        ElementProxy& operator=(const ElementProxy& rhs) { v.Insert(rhs.v.Get(rhs.pos), pos); return *this; }
        /**
         * Assignment operator for the ElementProxy class.
         *
         * \param data The data that is position is being set too
         * \return The ElementProxy class associated with the sparsevector
         */
        ElementProxy& operator=(int data) { v.Insert(data, pos); return *this; }
        /**
         * Subtracts the value in the ElementProxy class by the given data.
         *
         * \param data What the given value will be subracted by
         * \return The ElementProxy who's data is the difference
         */
        ElementProxy& operator-=(int data) { v.Insert(v.Get(pos) - data, pos); return *this; }
        /**
         * Adds the value in the ElementProxy class by the given data.
         *
         * \param data What the given value will be added by
         * \return The ElementProxy who's data is the sum
         */
        ElementProxy& operator+=(int data) { v.Insert(data + v.Get(pos), pos); return *this; }
        /**
         * Changes the sign of the value of the ElementProxy class.
         *
         * \return The changed value at the position
         */
        int operator-() { return -v.Get(pos); }
        /**
         * Automatically switches the data to an int.
         *
         * \return The value stored within the ElementProxy
         */
        operator int() const { return v.Get(pos); }
        // .......
    private:
        /**
         * Associated SparseVector class.
         */
        SparseVector& v;
        /**
         * The position that the class is associated with within the SparseVector.
         */
        long pos;
    };
}

void sparsevector_CPP_test0(void);
void sparsevector_CPP_test1(void);
void sparsevector_CPP_test2(void);
void sparsevector_CPP_test3(void);
void sparsevector_CPP_test4(void);
void sparsevector_CPP_test5(void);
void sparsevector_CPP_test6(void);
void sparsevector_CPP_test7(void);
void sparsevector_CPP_test8(void);
void sparsevector_CPP_test9(void);
void sparsevector_CPP_test10(void);
void sparsevector_CPP_test11(void);
void sparsevector_CPP_test12(void);
void sparsevector_CPP_test13(void);
void sparsevector_CPP_test14(void);
void sparsevector_CPP_test15(void);
void sparsevector_CPP_test16(void);
void sparsevector_CPP_test17(void);