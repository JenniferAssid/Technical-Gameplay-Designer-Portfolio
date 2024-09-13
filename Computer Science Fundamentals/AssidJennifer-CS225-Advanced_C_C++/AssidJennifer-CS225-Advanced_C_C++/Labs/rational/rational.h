#pragma once
/*****************************************************************
 * \file   rational.h
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Unknown
 * \date   Fall 2021
 * \brief
 *    Demonstrates operator overloads
 *********************************************************************/
#ifndef RATIONAL_H
#define RATIONAL_H
#include <fstream>  // std::fstream
#include <cstdlib>  // std::abs

class Rational {
private:
    int n, d;
    static int width; // used by stream insertion, do not change
    ////////////////////////////////////////////////////////////////////////
    void CheckSign();
    void CheckGCD();
    int  GCD() const;
public:
    Rational();
    Rational(const Rational& other);
    Rational(int i);
    Rational(int _n, int _d);

    int numerator() const { return n; }
    int denominator() const { return d; }

    // set width
    static void Width(int w);

    ////////////////////////////////////////////////////////////////////////
    friend std::ostream& operator<<(std::ostream& os, Rational const& r);

    Rational& operator= (const Rational& op2);

    // negate
    Rational operator- () const;
    ////////////////////////////////////////////////////////////////////////
    // multiplication
    Rational& operator*= (const Rational& op2);
    Rational operator* (const Rational& op2);

    ////////////////////////////////////////////////////////////////////////
    // division
    Rational operator/(const Rational& op2);
    Rational operator/(const int i);
    ////////////////////////////////////////////////////////////////////////
    // subtraction
    Rational operator-(const Rational& op2);
    Rational operator-(int i);
    ////////////////////////////////////////////////////////////////////////
    // addition
    Rational& operator+= (const Rational& op2);
    Rational operator+ (const Rational& op2);
    ////////////////////////////////////////////////////////////////////////
    // comparisons
    bool operator< (Rational const& op2) const;
    bool operator> (Rational const& op2) const;
    bool operator!= (Rational const& op2) const;
    bool operator >= (Rational const& op2) const;
    bool operator <= (Rational const& op2) const;
    bool operator == (Rational const& op2) const;
    ////////////////////////////////////////////////////////////////////////
    // comparisons - without class invariant
    //bool operator< ( Rational const & op2 ) const {
    //    return (n*op2.d < d*op2.n); // since one or both denominators may be `
    //    negative the above line is incorrect
    //    if ( op2.d*d > 0 )  return (n*op2.d < d*op2.n);
    //    else                return (n*op2.d > d*op2.n);
    //}

};

Rational operator*(int i, Rational op2);
Rational operator+(int i, Rational op2);
Rational operator- (int op1, Rational const& op2);
Rational operator/(int i, Rational op2);

// when first operand is not Rational, conversion does not kick in, so swap (see implementation)
//Rational operator- ( int op1, Rational const & op2 );
// ....

void rational(void);

#endif
