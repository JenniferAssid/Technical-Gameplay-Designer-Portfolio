#pragma once
/*****************************************************************
 * \file   polynomial.h
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Unknown
 * \date   Fall 2021
 * \brief
 *    Different polynomial functions - operator overloads and templates
 *********************************************************************/
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
#include <fstream>

void polynomial(void);

template<typename T, int N> class Polynomial;

template<typename T, int N>
std::ostream& operator<<(std::ostream& out, const Polynomial<T, N>& pol);


template<typename T, int N>
class Polynomial
{
public:
    Polynomial();

    template<typename T2>
    Polynomial(const Polynomial<T2, N>& other);

    friend std::ostream& operator<< <> (std::ostream& out, Polynomial<T, N> const& pol);
    T operator()(T value) const;
    template<int N2>
    Polynomial<T, (N > N2) ? N : N2> operator+(const Polynomial<T, N2>& rhs) const;
    template<int N2>
    Polynomial<T, N + N2> operator*(const Polynomial<T, N2>& rhs) const;
    Polynomial<T, N>& operator=(const Polynomial<T, N>& rhs);
    template<typename T2>
    Polynomial<T, N>& operator=(const Polynomial<T2, N>& rhs);

    T& operator[](int index) { return coeff[index]; }
    T operator[](int index) const { return coeff[index]; }

private:
    T coeff[N + 1];
};
#endif