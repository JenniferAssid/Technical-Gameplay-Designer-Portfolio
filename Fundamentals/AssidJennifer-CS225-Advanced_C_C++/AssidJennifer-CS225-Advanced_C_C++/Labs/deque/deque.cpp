/*****************************************************************
 * \file   deque.cpp
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Unknown
 * \date   Fall 2021
 * \brief
 *    Contains all the functions for a deque list
 *********************************************************************/
#include "deque.h"
#include <iostream>
#include <iomanip>

namespace DigiPen {

    // implemented
    Deque::Deque() : b(0), e(0), size(0), capacity(0), array(nullptr) {
    }

    // implemented
    // trim capacity to size when coping
    Deque::Deque(const Deque& rhs) :
        b(0), e(rhs.size),
        size(e), capacity(size),
        array(size ? new int[size] : nullptr) // size == 0 => array = nullptr in ctor
    {
        for (int i = 0; i < e; ++i) array[i] = rhs[i];
    }

    Deque& Deque::operator=(Deque rhs)
    {
        b = 0; e = rhs.size; size = e; capacity = size;

        if (size)
        {
            if (array != NULL) delete[] array;
            array = new int[size];

            for (int i = 0; i < size; i++) array[i] = rhs[i];
        }
        else array = nullptr;

        return *this;
    }

    Deque::~Deque()
    {
        delete[] array;
    }

    // implemented
    Deque::Deque(int* array_, unsigned int size_) : b(0), e(size_), size(e), capacity(size), array(new int[size]) {
        for (int i = 0; i < e; ++i) array[i] = array_[i];
    }

    // implemented
    int Deque::Size() const { return size; }

    // implemented
    bool Deque::Empty() const { return (size == 0); }

    // implemented
    int Deque::Capacity() const { return capacity; }

    void Deque::Push_back(int val) {
        if (size == capacity) { // double the array size, or change size=0 to size=1
            reallocate(capacity ? capacity * 2 : 1);
        }

        size++;

        array[e] = val;

        if (e + 1 > capacity - 1) e = 0;
        else e += 1;
    }

    int Deque::Pop_back() {
        if (size == capacity / 4) { // half the array
            reallocate(capacity / 2);
        }

        size--;

        if (e - 1 < 0) e = capacity - 1;
        else e -= 1;

        int result = array[e];
        array[e] = 0;

        return result;
    }

    void Deque::Push_front(int val) {
        if (size == capacity) { // double the array size, or change size=0 to size=1
            reallocate(capacity ? capacity * 2 : 1);
        }

        size++;

        if (b == 0) b = capacity - 1;
        else b--;

        array[b] = val;
    }

    int Deque::Pop_front() {
        if (size == capacity / 4) { // half the array
            reallocate(capacity / 2);
        }

        size--;

        int result = array[b];

        if (b == capacity - 1) b = 0;
        else b++;

        return result;
    }

    int& Deque::operator[](unsigned int pos)
    {
        return array[(b + pos) % capacity];
    }

    int Deque::operator[](unsigned int pos) const
    {
        return array[(b + pos) % capacity];
    }

    void Deque::swap(Deque& other)
    {
        Deque temp(other);
        other = *this;
        *this = temp;
    }

    // if current capacity is not enough for the combined:
    // reallocate and trim, else do not reallocate at all
    Deque& Deque::operator+=(const Deque& op2) {
        if (size + op2.size > capacity) {
            reallocate(size + op2.size);
        }
        //....

        for (int i = 0; i < op2.size; i++)
        {
            Push_back(op2[i]);
        }

        return *this;
    }

    Deque Deque::operator+(const Deque& rhs) const
    {
        Deque result(*this);
        result += rhs;
        return result;
    }

    Deque& Deque::reverse()
    {
        for (int i = 0; i < size / 2; i++)
        {
            int temp = array[(b + i) % capacity];
            array[(b + i) % capacity] = array[(e - 1 - i) % capacity];
            array[(e - 1 - i) % capacity] = temp;
        }

        return *this;
    }

    Deque Deque::operator~() const
    {
        Deque result(*this);
        result.reverse();

        return result;
    }

    // implemented
    void Deque::Print() const {
        std::cout << "size = " << size << " capacity = " << capacity << std::endl;

        if (array == nullptr) {
            std::cout << "array=nullptr" << std::endl;
            return;
        }

        // deque may still be empty, but array is allocated, so print some data
        int width = 3;
        // line of indices
        for (int i = 0; i < capacity; ++i) {
            std::cout << std::setw(width) << i;
        }
        std::cout << " indices" << std::endl;
        // line of data
        for (int i = 0; i < capacity; ++i) {
            if ((b <= i and i < e)                         //  ....b.....e.....
                or (e < b and (b <= i or i < e))        //  ....e.....b.....
                or (b == e and size > 0))            //  ....be.......... either empty or full - look at size
                std::cout << std::setw(width) << array[i];
            else
                std::cout << std::setw(width) << ".";
        }
        std::cout << " data" << std::endl;

        char ch1 = 'b', ch2 = 'e';
        int dist1 = b + 1, dist2 = e - b;
        if (e < b) { ch1 = 'e'; ch2 = 'b'; dist1 = e + 1; dist2 = b - e; }
        std::cout << std::setw(width * dist1) << ch1;
        std::cout << std::setw(width * dist2) << ch2;
        std::cout << std::endl;
    }

    // implemented
    void Deque::reallocate(int new_capacity) {

        // clean - dtor uses this
        if (new_capacity == 0) {
            delete[] array;
            array = nullptr;
            b = 0;
            e = 0;
            size = 0;
            capacity = 0;
        }
        else {

            // do not check whether new_capacity makes sense etc - it is caller's responsibility
            int* new_array = new int[new_capacity];
            for (int i = 0; i < size; ++i) {
                new_array[i] = array[(b + i) % capacity]; // new b is 0
            }
            //for (int i=size;i<capacity;++i) new_array[i]=0;
            delete[] array;
            array = new_array;
            b = 0;
            e = size;
            // same size
            capacity = new_capacity;
        }
    }

    // implemented
    std::ostream& operator<<(std::ostream& os, const Deque& d) {
        for (int i = 0; i < d.Size(); ++i) os << d[i] << " "; // this is public Deque::operator[], operator<< does not need to be friend
        return os;
    }
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <iostream>
#include <cstdlib>

////////////////////////////////////////////////////////////
// Push_back / Pop_back
// correctness of logic
void deque_test0() {
    std::cout << __func__ << std::endl;
    DigiPen::Deque v;

    // v.Print(); 
    // std::cout << "--------------------\n";

    //grow 
    for (int i = 0; i < 13; ++i) {
        v.Push_back(i);
        //v.Print();  // debugging
        std::cout << "-->" << v << "<--" << std::endl;
    }
    std::cout << "--------------------\n";

    //shrink back, remember shrink capacity when size == capacity/4
    while (!v.Empty()) {
        v.Pop_back();
        //v.Print(); 
        std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

    //grow back
    for (int i = 0; i < 13; ++i) {
        v.Push_back(i);
        //v.Print(); 
        std::cout << v << std::endl;
    }
}

////////////////////////////////////////////////////////////
// Push_back / Pop_back
// correctness of implementation
void deque_test1() {
    std::cout << __func__ << std::endl;
    DigiPen::Deque v;

    v.Print();
    std::cout << "--------------------\n";

    //grow 
    for (int i = 0; i < 13; ++i) {
        v.Push_back(i);
        v.Print();  // debugging
        //std::cout << "-->" << v << "<--" << std::endl;
    }
    std::cout << "--------------------\n";

    //shrink back, remember shrink capacity when size == capacity/4
    while (!v.Empty()) {
        v.Pop_back();
        v.Print();
        //std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

    //grow back
    for (int i = 0; i < 13; ++i) {
        v.Push_back(i);
        v.Print();
        //std::cout << v << std::endl;
    }
}


////////////////////////////////////////////////////////////
// Push_front / Pop_front
// correctness of logic
void deque_test2() {
    DigiPen::Deque v;

    //grow front
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        //v.Print(); 
        std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

    //shrink front
    while (!v.Empty()) {
        v.Pop_front();
        //v.Print(); 
        std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

    //grow front
    for (int i = 0; i < 33; ++i) {
        v.Push_front(i);
        //v.Print(); 
        std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

}

////////////////////////////////////////////////////////////
// Push_front / Pop_front
// correctness of implementation
void deque_test3() {
    DigiPen::Deque v;

    //grow front
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        v.Print();
        //std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

    //shrink front
    while (!v.Empty()) {
        v.Pop_front();
        v.Print();
        //std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

    //grow front
    for (int i = 0; i < 33; ++i) {
        v.Push_front(i);
        v.Print();
        //std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

}


////////////////////////////////////////////////////////////
// Push_back / Pop_front
// correctness of logic
void deque_test4() {
    DigiPen::Deque v;

    // grow front
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        std::cout << v << std::endl;
    }

    // imitate queue
    for (int i = 0; i < 50; ++i) {
        v.Push_back(v.Pop_front());
        std::cout << v << std::endl;
        std::cout << "--------------------\n";
    }
}

////////////////////////////////////////////////////////////
// Push_back / Pop_front
// correctness of implementation
void deque_test5() {
    DigiPen::Deque v;

    // grow front
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        v.Print();
    }

    // imitate queue
    for (int i = 0; i < 50; ++i) {
        v.Push_back(v.Pop_front());
        v.Print();
        std::cout << "--------------------\n";
    }
}

////////////////////////////////////////////////////////////
// Push_front / Pop_back
// correctness of logic
void deque_test6() {
    DigiPen::Deque v;

    //grow front
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        std::cout << v << std::endl;
    }

    // imitate queue - different direction
    for (int i = 0; i < 50; ++i) {
        v.Push_front(v.Pop_back());
        std::cout << v << std::endl;
        std::cout << "--------------------\n";
    }
}

////////////////////////////////////////////////////////////
// Push_front / Pop_back
// correctness of implementation
void deque_test7() {
    DigiPen::Deque v;

    //grow front
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        std::cout << v << std::endl;
    }

    // imitate queue - different direction
    for (int i = 0; i < 50; ++i) {
        v.Push_front(v.Pop_back());
        v.Print();
        std::cout << "--------------------\n";
    }
}

////////////////////////////////////////////////////////////
// grow with Push_front and Push_back simultaneously
// correctness of logic
void deque_test8() {
    DigiPen::Deque v;

    //grow front and back
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        v.Push_back(i + 1);
        //v.Print(); 
        std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";
    while (!v.Empty()) {
        v.Pop_back();
        std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";

    //grow front and back
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        v.Push_back(i + 1);
        //v.Print(); 
        std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";
    while (!v.Empty()) {
        v.Pop_front();
        std::cout << v << std::endl;
    }
    std::cout << "--------------------\n";
}

////////////////////////////////////////////////////////////
// grow with Push_front and Push_back simultaneously
// correctness of implementation
void deque_test9() {
    DigiPen::Deque v;

    //grow front and back
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        v.Push_back(i + 1);
        v.Print();
    }
    std::cout << "--------------------\n";
    while (!v.Empty()) {
        v.Pop_back();
        v.Print();
    }
    std::cout << "--------------------\n";

    //grow front and back
    for (int i = 0; i < 10; ++i) {
        v.Push_front(i + 1);
        v.Push_back(i + 1);
        v.Print();
    }
    std::cout << "--------------------\n";
    while (!v.Empty()) {
        v.Pop_front();
        v.Print();
    }
    std::cout << "--------------------\n";
}

////////////////////////////////////////////////////////////
// grow / shrink randomly
// correctness of logic
#include <random>
#include <deque>
void deque_test10_aux() {
    bool const print_extra = false; // true for debugging

    int push_front_prob = 30;
    int push_back_prob = 30;
    int pop_front_prob = 20;
    int pop_back_prob = 20;
    int num_iter = 10000;

    std::random_device                      rd;
    std::mt19937                            gen(rd());
    std::uniform_int_distribution<int>      dis_action(0, push_front_prob + push_back_prob + pop_front_prob + pop_back_prob);  // random out of 4: Push_front, Push_back, Pop_front, Pop_back
    std::uniform_int_distribution<int>      dis_push(0, push_front_prob + push_back_prob);    // random out of 2: Push_front, Push_back
    std::uniform_int_distribution<int>      dis_val(0, 999);    // just a random value

    DigiPen::Deque v;
    std::deque<int> deq; // for checking correctness
    for (int i = 0; i < num_iter; ++i) {
        int rnd = 0;
        if (v.Size() > 0) { // all 4
            rnd = dis_action(gen);
        }
        else { // only pushes
            rnd = dis_push(gen);
        }

        if ((rnd -= push_front_prob) <= 0) { // push_front is chosen
            int value = dis_val(gen);
            if (print_extra) std::cout << "Push_front " << value << std::endl;
            v.Push_front(value);
            deq.push_front(value);
        }
        else if ((rnd -= push_back_prob) <= 0) { // push_back  is chosen
            int value = dis_val(gen);
            if (print_extra) std::cout << "Push_back " << value << std::endl;
            v.Push_back(value);
            deq.push_back(value);
        }
        else if ((rnd -= pop_front_prob) <= 0) { // pop_front  is chosen
            if (print_extra) std::cout << "Pop_front " << std::endl;
            v.Pop_front();
            deq.pop_front();
        }
        else { // pop_back   is chosen
            if (print_extra) std::cout << "Pop_back " << std::endl;
            v.Pop_back();
            deq.pop_back();
        }

        // check v and deq are same
        if (v.Size() == static_cast<int>(deq.size())) {
            for (int j = 0; j < v.Size(); ++j) {
                if (v[j] != deq[j]) {
                    std::cout << "iteration " << i << " error: v[" << j << "]=" << v[j] << ", deq[" << j << "]=" << deq[j] << std::endl;
                }
            }
        }
        else {
            std::cout << "iteration " << i << " error: v.Size()=" << v.Size() << ", deq.size()=" << deq.size() << std::endl;
            return;
        }
        if (print_extra) std::cout << "Size = " << v.Size() << std::endl;
    }
    std::cout << "OK" << std::endl;
}

void deque_test10() {
    deque_test10_aux();
    deque_test10_aux();
    deque_test10_aux();
    deque_test10_aux();
    deque_test10_aux();
}

////////////////////////////////////////////////////////////
// other ctors
void deque_test11() {
    int a1[] = { 1,2,3,4,5,6 };
    int a2[] = { 11,12,13,14,15,16 };
    DigiPen::Deque v1(a1, sizeof(a1) / sizeof(a1[0]));
    DigiPen::Deque v2(a2, sizeof(a2) / sizeof(a2[0]));
    std::cout << "v1: size = " << v1.Size() << " capacity = " << v1.Capacity() << std::endl;
    std::cout << v1 << std::endl;
    std::cout << "v2: size = " << v2.Size() << " capacity = " << v2.Capacity() << std::endl;
    std::cout << v2 << std::endl;
}

////////////////////////////////////////////////////////////
// copy ctor
void deque_test12() {
    DigiPen::Deque v;

    for (int i = 0; i < 20; ++i) v.Push_front(i);
    // create copy
    DigiPen::Deque v_copy(v);

    std::cout << "v: size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "v_copy: size = " << v_copy.Size() << " capacity = " << v_copy.Capacity() << std::endl;
    std::cout << v_copy << std::endl;

    // remove all elements from copy
    while (!v_copy.Empty()) {
        v_copy.Pop_back();
    }

    std::cout << "v: size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "v_copy: size = " << v_copy.Size() << " capacity = " << v_copy.Capacity() << std::endl;
    std::cout << v_copy << std::endl;

    // create another copy
    DigiPen::Deque v_copy2(v);

    std::cout << "v: size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "v_copy2: size = " << v_copy2.Size() << " capacity = " << v_copy2.Capacity() << std::endl;
    std::cout << v_copy2 << std::endl;

    // remove all elements from original
    while (!v.Empty()) {
        v.Pop_back();
    }

    std::cout << "v: size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "v_copy2: size = " << v_copy2.Size() << " capacity = " << v_copy2.Capacity() << std::endl;
    std::cout << v_copy2 << std::endl;

}
////////////////////////////////////////////////////////////
// assignment operator
void deque_test13() {
    DigiPen::Deque v;
    DigiPen::Deque v2;
    DigiPen::Deque v3;

    for (int i = 0; i < 20; ++i) v.Push_front(i);
    for (int i = 0; i < 30; ++i) v2.Push_front(50 - i);
    for (int i = 0; i < 40; ++i) v3.Push_front(30 - i);

    // assign
    v2 = v;

    std::cout << "v: size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "v2: size = " << v2.Size() << " capacity = " << v2.Capacity() << std::endl;
    std::cout << v2 << std::endl;
    std::cout << "--------------------\n";

    // remove all elements from copy
    while (!v2.Empty()) {
        v2.Pop_back();
    }

    std::cout << "v: size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "v2: size = " << v2.Size() << " capacity = " << v2.Capacity() << std::endl;
    std::cout << v2 << std::endl;
    std::cout << "--------------------\n";

    // create another copy
    v3 = v;

    std::cout << "v: size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "v3: size = " << v3.Size() << " capacity = " << v3.Capacity() << std::endl;
    std::cout << v3 << std::endl;
    std::cout << "--------------------\n";

    // remove all elements from original
    while (!v.Empty()) {
        v.Pop_back();
    }

    std::cout << "v: size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "v3: size = " << v3.Size() << " capacity = " << v3.Capacity() << std::endl;
    std::cout << v3 << std::endl;
    std::cout << "--------------------\n";

}

////////////////////////////////////////////////////////////
void deque_test14() {
    int a1[] = { 1,2,3,4,5,6 };
    int a2[] = { 11,12,13,14,15,16 };

    DigiPen::Deque v1(a1, sizeof(a1) / sizeof(a1[0]));
    DigiPen::Deque v2(a2, sizeof(a2) / sizeof(a2[0]));

    DigiPen::Deque v_sum = v1 + v2;

    std::cout << "v1: size = " << v1.Size() << " capacity = " << v1.Capacity() << std::endl;
    std::cout << v1 << std::endl;
    std::cout << "v2: size = " << v2.Size() << " capacity = " << v2.Capacity() << std::endl;
    std::cout << v2 << std::endl;
    std::cout << "v_sum: size = " << v_sum.Size() << " capacity = " << v_sum.Capacity() << std::endl;
    std::cout << v_sum << std::endl;
    std::cout << "--------------------\n";

    // quick check of v_sum
    int c = 0;
    while (!v_sum.Empty()) {
        if (++c % 2) {
            v_sum.Pop_back();
        }
        else {
            v_sum.Pop_front();
        }
        std::cout << v_sum << std::endl;
    }

}

////////////////////////////////////////////////////////////
// const correctness - const deque supports read index 
void deque_test15() {
    DigiPen::Deque v;
    for (int i = 0; i < 20; ++i) v.Push_back(i);
    DigiPen::Deque const const_v = v;
    for (int i = 0; i < 20; ++i) std::cout << const_v[i] << std::endl;
}

////////////////////////////////////////////////////////////
// reversing
void deque_test16() {
    DigiPen::Deque v;

    //grow
    for (int i = 0; i < 20; ++i) {
        v.Push_back(i);
    }
    std::cout << v << std::endl;
    std::cout << "--------------------\n";

    // reverse 
    DigiPen::Deque v2(~v);
    std::cout << v2 << std::endl;
    std::cout << "--------------------\n";
    return;

    //shrink
    for (int i = 0; i < 20; ++i) {
        v2.Pop_front();
    }

    //reverse self
    v.reverse();
    std::cout << "size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
    std::cout << v << std::endl;
    std::cout << "--------------------\n";

    //shrink
    for (int i = 0; i < 20; ++i) {
        v.Pop_back();
        std::cout << "size = " << v.Size() << " capacity = " << v.Capacity() << std::endl;
        std::cout << v << std::endl;
    }
}

////////////////////////////////////////////////////////////
// reverse many times - time this
// using pops and pushs is about 4 times slower than in place (swaps)
// on pontus 3.8 second vs 0.9
void deque_test17() {
    int const num_iter = 98;
    int const size = 400000;
    DigiPen::Deque v;
    for (int i = 0; i < size; ++i) v.Push_back(i);

    v.reverse();

    for (int i = 0; i < size; ++i) {
        if (v[i] != size - i - 1) { // check is reversed
            std::cout << "E1";
            return;
        }
    }

    for (int i = 0; i < num_iter; ++i) {
        v.reverse();
    }

    // make it even (we also did one extra)
    if (num_iter % 2 == 0) {
        v.reverse();
    }

    // should be back to normal
    for (int i = 0; i < size; ++i) {
        if (v[i] != i) {
            std::cout << "E2";
            return;
        }
    }
    std::cout << "OK";
}

////////////////////////////////////////////////////////////
// deque_testing on empty 
void deque_test18() {
    DigiPen::Deque empty;
    DigiPen::Deque copy_empty(empty);
    std::cout << "-->" << empty << "<--" << std::endl;
    std::cout << "1-------------------\n";
    std::cout << "-->" << copy_empty << "<--" << std::endl;
    std::cout << "2-------------------\n";
    DigiPen::Deque c(~empty);
    std::cout << "-->" << c << "<--" << std::endl;
    std::cout << "3-------------------\n";
    empty.reverse();
    std::cout << "-->" << empty << "<--" << std::endl;
    std::cout << "4-------------------\n";
    DigiPen::Deque empty2;
    empty += empty2;
    std::cout << "-->" << empty << "<--" << std::endl;
    std::cout << "5-------------------\n";

}

////////////////////////////////////////////////////////////
void deque_test19() {
    DigiPen::Deque v1, v2, v3, v4;
    for (int i = 0; i < 10; ++i) v1.Push_back(3 + i);
    for (int i = 0; i < 20; ++i) v2.Push_back(13 - i);
    for (int i = 0; i < 80; ++i) v3.Push_back(i + 7);
    for (int i = 0; i < 5; ++i)  v4.Push_back(i);

    // some meaningless long sequence
    DigiPen::Deque v = v1 + ~v1 + ~v2 + v3 + v4 + ~v3 + ~v3 + ~v2 + ~v1;
    std::cout << v << std::endl;
}

////////////////////////////////////////////////////////////
void deque_test20() {
    DigiPen::Deque v1, v2, v3, v4;
    for (int i = 0; i < 10; ++i) v1.Push_back(3 + i);
    for (int i = 0; i < 20; ++i) v2.Push_back(13 - i);
    for (int i = 0; i < 80; ++i) v3.Push_back(i + 7);
    for (int i = 0; i < 5; ++i)  v4.Push_back(i);

    // some meaningless swaps - hopefully you have already used swap in
    // assignment and operator~
    v1.swap(v2);
    v2.swap(v3);
    v3.swap(v4);
    v4.swap(v1);
    std::cout << v1 << v2 << v3 << v4 << std::endl;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void deque() {
    deque_test0();
    deque_test1();
    deque_test2();
    deque_test3();
    deque_test4();
    deque_test5();
    deque_test6();
    deque_test7();
    deque_test8();
    deque_test9();
    deque_test10();
    deque_test11();
    deque_test12();
    deque_test13();
    deque_test14();
    deque_test15();
    deque_test16();
    deque_test17();
    deque_test18();
    deque_test19();
    deque_test20();
}
