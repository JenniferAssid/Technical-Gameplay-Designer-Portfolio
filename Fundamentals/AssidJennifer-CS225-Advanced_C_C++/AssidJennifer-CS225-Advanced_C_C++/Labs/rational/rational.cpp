/*****************************************************************
 * \file   rational.cpp
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
#include "rational.h"
#include <iomanip>  // std::setw

int Rational::width = 0; // no padding by default

////////////////////////////////////////////////////////////////////////
Rational::Rational() : n(0), d(1) {}
////////////////////////////////////////////////////////////////////////
Rational::Rational(int i) : n(i), d(1) {}
////////////////////////////////////////////////////////////////////////
Rational::Rational(int _n, int _d) : n(_n), d(_d) { CheckSign(); CheckGCD(); }
////////////////////////////////////////////////////////////////////////
Rational Rational::operator- () const { return Rational(-n, d); }
////////////////////////////////////////////////////////////////////////
Rational::Rational(const Rational& other) : n(other.n), d(other.d) { CheckSign(); CheckGCD(); }

Rational& Rational::operator= (const Rational& other)
{
    n = other.n;
    d = other.d;
    width = other.width;

    return *this;
}

Rational& Rational::operator+= (const Rational& op2)
{
    int op2_n = op2.n;
    int op2_d = op2.d;

    n *= op2_d;
    op2_n *= d;
    op2_d *= d;
    d = op2_d;

    n += op2_n;

    CheckGCD();

    return *this;
}
Rational& Rational::operator*= (const Rational& op2)
{
    n *= op2.n;
    d *= op2.d;

    CheckGCD();

    return *this;
}

Rational Rational::operator* (const Rational& op2)
{
    Rational result(n, d);
    result *= op2;
    result.CheckGCD();

    return result;
}

Rational operator*(int i, Rational op2)
{
    return op2.operator*(i);
}

Rational Rational::operator+ (const Rational& op2)
{
    Rational result;
    result += *this;
    result += op2;

    return result;
}

Rational operator+(int i, Rational op2)
{
    return op2.operator+(i);
}

Rational Rational::operator- (const Rational& op2)
{
    Rational result(n, d);
    int op2_n = op2.n;
    int op2_d = op2.d;

    result.n *= op2_d;
    op2_n *= result.d;
    result.d *= op2_d;

    result.n -= op2_n;

    return result;
}

Rational Rational::operator-(int i)
{
    Rational result(n, d);

    result.n -= i * d;

    result.CheckGCD();

    return result;
}

Rational Rational::operator/(const int i)
{
    Rational result(n, d);

    result.d *= i;
    result.CheckGCD();

    return result;
}

Rational Rational::operator/(const Rational& other)
{
    Rational result(n, d);

    result.n *= other.d;
    result.d *= other.n;

    result.CheckGCD();

    return result;
}

Rational operator/(int i, Rational other)
{
    return Rational(i) / other;
}

////////////////////////////////////////////////////////////////////////
// comparisons
bool Rational::operator< (Rational const& op2) const { return (n * op2.d < d * op2.n); }
bool Rational::operator> (Rational const& op2) const { return (n * op2.d > d * op2.n); }
bool Rational::operator!= (Rational const& op2) const
{
    if (n == op2.n && d == op2.d) return false;

    return true;
}
bool Rational::operator<=(Rational const& op2) const
{
    if (this->operator<(op2) || this->operator==(op2)) return true;

    return false;
}
bool Rational::operator>= (Rational const& op2) const
{
    if (this->operator>(op2) || this->operator==(op2)) return true;

    return false;
}
bool Rational::operator== (Rational const& op2) const
{
    return !(this->operator!=(op2));
}


////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, Rational const& r) {
    os << std::setw(r.width) << r.n;
    if (r.d != 1) { os << "/" << std::setw(r.width - 1) << r.d; }
    else { os << std::setw(r.width) << " "; }
    return os;
}
////////////////////////////////////////////////////////////////////////
void Rational::CheckSign() {
    if (d < 0) { n *= -1; d *= -1; }
}
////////////////////////////////////////////////////////////////////////
void Rational::CheckGCD() {
    int gcd = GCD();
    n = n / gcd;
    d = d / gcd;
}
////////////////////////////////////////////////////////////////////////
int Rational::GCD() const {
    int a = std::abs(n), b = std::abs(d);
    while (a && b) {
        if (a < b) { b = b % a; }
        else { a = a % b; }
    }
    if (a == 0) return b;
    else        return a;
}

// when first operand is not Rational, conversion does not kick in, so swap
Rational operator- (int op1, Rational const& op2) { return Rational(op1) - op2; }

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <iostream>
#include <iomanip> // setprecision


void rational_test0()
{
    Rational r1(1, 2);
    std::cout << r1 << std::endl;

    Rational r2(-1, 2);
    std::cout << r2 << std::endl;

    Rational r3(1, -2);
    std::cout << r3 << std::endl;
}

void rational_test1()
{
    Rational r1(4, 8);
    std::cout << r1 << std::endl;

    Rational r2(-6, 8);
    std::cout << r2 << std::endl;

    Rational r3(3, -9);
    std::cout << r3 << std::endl;
}

void rational_test2()
{
    Rational r1(6);
    std::cout << r1 << std::endl;

    Rational r2(-3);
    std::cout << r2 << std::endl;
}

void rational_test3()
{
    Rational r1(4, 8);

    Rational r2(r1);

    Rational r3(3, -9);
    std::cout << r1 << std::endl;
    std::cout << r2 << std::endl;
    std::cout << r3 << std::endl;

    r3 = r2;
    std::cout << r1 << std::endl;
    std::cout << r2 << std::endl;
    std::cout << r3 << std::endl;
}

void rational_test4()
{
    Rational pi_1(16, 5);     // approximation of pi (see "Indiana Pi Bill" of 1897)
    Rational pi_2(22, 7);     // approximation of pi (Ancient Egypt)
    Rational pi_3(355, 113);  // approximation of pi (Milu approximation by Chiniese astronomer Zu Chongzhi)

    // pi_1 > pi_2 > pi_3

    std::cout << "pi_1 and pi_2 are " << (pi_1 != pi_2 ? "not " : "") << "equal\n";
    std::cout << "pi_2 and pi_3 are " << (pi_2 != pi_3 ? "not " : "") << "equal\n";
    std::cout << "pi_1 and pi_3 are " << (pi_1 != pi_3 ? "not " : "") << "equal\n";
    std::cout << "pi_1 and pi_2 are " << (pi_1 == pi_2 ? "" : "not ") << "equal\n";
    std::cout << "pi_2 and pi_3 are " << (pi_2 == pi_3 ? "" : "not ") << "equal\n";
    std::cout << "pi_1 and pi_3 are " << (pi_1 == pi_3 ? "" : "not ") << "equal\n";

    std::cout << "pi_1 is " << (pi_1 < pi_2 ? "" : "not ") << "less than pi_2\n";
    std::cout << "pi_1 is " << (pi_1 < pi_3 ? "" : "not ") << "less than pi_3\n";
    std::cout << "pi_2 is " << (pi_2 < pi_3 ? "" : "not ") << "less than pi_3\n";

    std::cout << "pi_1 is " << (pi_1 > pi_2 ? "" : "not ") << "greater than pi_2\n";
    std::cout << "pi_1 is " << (pi_1 > pi_3 ? "" : "not ") << "greater than pi_3\n";
    std::cout << "pi_2 is " << (pi_2 > pi_3 ? "" : "not ") << "greater than pi_3\n";

    std::cout << "pi_1 is " << (pi_1 <= pi_2 ? "" : "not ") << "less or equal than pi_2\n";
    std::cout << "pi_1 is " << (pi_1 <= pi_3 ? "" : "not ") << "less or equal than pi_3\n";
    std::cout << "pi_2 is " << (pi_2 <= pi_3 ? "" : "not ") << "less or equal than pi_3\n";

    std::cout << "pi_1 is " << (pi_1 >= pi_2 ? "" : "not ") << "greater or equal than pi_2\n";
    std::cout << "pi_1 is " << (pi_1 >= pi_3 ? "" : "not ") << "greater or equal than pi_3\n";
    std::cout << "pi_2 is " << (pi_2 >= pi_3 ? "" : "not ") << "greater or equal than pi_3\n";
}

void rational_test5()
{
    Rational r1(-1, 2);
    Rational r2(1, -2);
    Rational r3(10, -20);

    std::cout << "r1 and r2 are " << (r1 == r2 ? "" : "not ") << "equal\n";
    std::cout << "r2 and r3 are " << (r2 == r3 ? "" : "not ") << "equal\n";
    std::cout << "r1 and r3 are " << (r1 == r3 ? "" : "not ") << "equal\n";
}

void rational_test6()
{
    Rational one_sixth(1, 6);
    Rational r; // default = zero (0/1)

    for (int i = 0; i < 60; ++i) {
        r += one_sixth;
        std::cout << r << " ";
    }
    std::cout << std::endl;
}

void rational_test7()
{
    Rational one_sixth(1, 6);

    for (int i = 0; i < 60; ++i) {
        Rational r(i);
        r *= one_sixth;
        std::cout << r << " ";
    }
    std::cout << std::endl;
}

void rational_test8()
{
    Rational one_sixth(1, 6);

    for (int i = 0; i < 60; ++i) {
        Rational r(one_sixth);
        r *= i;                     // conversion int -> Rational in the second argument will be done by compiler by calling Rational(int)
        std::cout << r << " ";
    }
    std::cout << std::endl;
}

void rational_test9()
{
    Rational one_sixth(1, 6);
    Rational r(-one_sixth);   // -1/6, requires unary negation operator

    for (int i = 0; i < 60; ++i) {
        Rational m(one_sixth);
        std::cout << m * i << " ";
        std::cout << i * m << " "; // conversion int->Rational in the first argument is NOT automatic. You will need a global overload for operator* with ( int, Rational ) arguments. Same for the 3 other operations
        std::cout << (r + one_sixth) << " "; // try removing parentheses 
        r = r + one_sixth;
        std::cout << std::endl;
    }
}

void rational_test10()
{
    Rational one_third(1, 3);
    int      two = 2;
    std::cout << two - one_third << std::endl;
    std::cout << two + one_third << std::endl;
    std::cout << two / one_third << std::endl;
}

void rational_test11()
{
    // Leibniz formula for pi
    // pi = 4/1 - 4/3 + 4/5 - 4/7 + 4/9 - 4/11 ...
    // very slow convergence
    int len = 10;
    Rational pi;
    for (int i = 0; i < len; ++i) {
        pi += (i % 2 ? Rational(-4) : Rational(4)) / (2 * i + 1);
        std::cout << pi << " appr " << std::setprecision(8) << static_cast<double>(pi.numerator()) / pi.denominator() << std::endl;
    }
}

void rational_test12()
{
    // John Wallis formula for pi
    //         inf
    //         +--+  4* i^2
    // pi = 2 *|  | ---------  (product)
    //         |  | 4* i^2 -1
    //         n=1
    int len = 6;
    Rational pi(2);
    for (int i = 1; i <= len; ++i) {
        pi *= Rational(4 * i * i, 4 * i * i - 1);
        std::cout << pi << " appr " << std::setprecision(8) << static_cast<double>(pi.numerator()) / pi.denominator() << std::endl;
    }
}

void rational(void)
{
    rational_test0();
    rational_test1();
    rational_test2();
    rational_test3();
    rational_test4();
    rational_test5();
    rational_test6();
    rational_test7();
    rational_test8();
    rational_test9();
    rational_test10();
    rational_test11();
    rational_test12();
}