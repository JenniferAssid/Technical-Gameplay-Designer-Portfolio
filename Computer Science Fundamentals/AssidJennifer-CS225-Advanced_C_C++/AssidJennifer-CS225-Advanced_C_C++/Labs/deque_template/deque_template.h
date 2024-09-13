/*****************************************************************
 * \file   deque_template.h
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Unknown
 * \date   Fall 2021
 * \brief
 *    Contains all the functions for adeque_template list - utilizes templates
 *********************************************************************/
#ifndef Deque_Template_H
#define Deque_Template_H
#include <fstream>

namespace DigiPen {

    template< typename T>
    class Deque_Template {
    public:
        // big 4
        Deque_Template();                        // note that inside class declaration "Deque_Template" automatically refers to Deque_Template<T>
        Deque_Template(Deque_Template const& rhs);      // copy from exactly same Deque_Template only
        Deque_Template& operator=(Deque_Template rhs);    // assign from exactly same Deque_Template only
        ~Deque_Template();

        // from array - exactly same type
        Deque_Template(T* array_, unsigned int size_);

        // simple stuff
        int   Size() const;
        bool  Empty() const;
        void  Clear();
        int   Capacity() const;

        // real stuff
        void  Push_front(T val);
        T     Pop_front();
        void  Push_back(T val);
        T     Pop_back();
        T& operator[](unsigned int pos);
        T     operator[](unsigned int pos) const;

        // usefull stuff
        void swap(Deque_Template& other);

        // non-standard stuff
        Deque_Template& operator+=(const Deque_Template& rhs);       // concatenate 2 Deque_Templates
        Deque_Template     operator+(const Deque_Template& rhs) const;  // concatenate 2 Deque_Templates
        Deque_Template& reverse();                // reverse in-place
        Deque_Template     operator~() const;                 // return new reverse

        // implemented
        void Print() const;
    private:
        // implemented
        // NOP if new_capacity <= current size
        void reallocate(int new_capacity);
    private:
        int b;    // index where Deque_Template's data starts
        int e;    // index where Deque_Template's data ends
        int size;
        int capacity; // size of the array
        T* array;  // data array
    };

    // non friend - but has to be declared in the header
    template< typename T>
    std::ostream& operator<<(std::ostream& os, Deque_Template<T> const& d);

}

void deque_template(void);

#endif
