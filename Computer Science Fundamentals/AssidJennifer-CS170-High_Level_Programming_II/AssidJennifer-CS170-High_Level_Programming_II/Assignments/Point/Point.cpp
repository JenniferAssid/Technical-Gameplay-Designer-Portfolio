/*!****************************************************************************
\file    Point.cpp
\author  Jennifer Assid
\par     DP email: jennifer.assid\@digipen.edu
\par     Course: CS170
\par     Programming Assignment #3
\date    23-02-2021

\brief
  This program contains overloaded operators to manipulate class Point

  - DegreesToRadians
       Converts a given degree value to a radian value

  - RadiansToDegrees
      Converts a given radian value to a degree value

  - Point
      Default constructor for a point - sets the values to 0

  - Point
      Non-default constructor for a point - set values to specified values

  - operator%
      Rotates a Point about the origin the specified number of degrees

  - operator-
      Calculates the difference between two Points and returns the distance.

  - operator^
      Calculates the midpoint of two Points.

  - operator+=
      Adds two Points and returns a reference to the left-hand operand which
      has been modified

  - operator+=
      Adds a Point and a double and returns a reference to the left-hand
      operand which has been modified

  - operator-
      Subtracts a double from a Point

  - operator++ (post / pre)
      Increments the x/y values of the class point

  - operator-- (post / pre)
      Decrements the x/y values of the class point

  - operator-
      Negates the values of the class point

  - operator+
      Adds the given point and the class point together

  - operator*
      Multipled the class point by a scalar value

  - operator<<
      Outputs the values of the given point

  - operator>>
      Places the given values into given point

  - operator+
      Adds a point and a double together (double as the left operand)

  - operator+
      Adds a point and a double together (Point as the left operand)

  - operator-
      Multiplies a point and a double together (double as the left operand)

******************************************************************************/

#include "Point.h"  // Point members
#include <cmath>    // sqrt, atan, sin, cos

namespace CS170
{

const double PI = 3.1415926535897;
const double EPSILON = 0.00001;

///////////////////////////////////////////////////////////////////////////////
// private member functions 
/*!****************************************************************************
\brief
   Converts a given degree value to a radian value

\param degrees
   The value being converted to radians

\return
   A double of the radians calculated from the degree
******************************************************************************/
double Point::DegreesToRadians(double degrees) const
{
  return (degrees * PI / 180.0);
}

/*!****************************************************************************
\brief
   Converts a given radian value to a degree value

\param degrees
   The value being converted to degrees

\return
   A double of the degrees calculated from the radian
******************************************************************************/
double Point::RadiansToDegrees(double radians) const
{
  return (radians * 180.0 / PI);
}


///////////////////////////////////////////////////////////////////////////////
// 16 public member functions (2 constructors, 14 operators) 
// Constructors (2)
/*!****************************************************************************
\brief
   Default constructor for a point - sets the values to 0

\return
   A Point with (x, y) = (0, 0)
******************************************************************************/
Point::Point()
{
  // Sets x / y to 0 as default
  x_ = y_ = 0;
}

/*!****************************************************************************
\brief
   Non-default constructor for a point - set values to specified values

\param x
   The value that the x_ value of the Point will be set to

\param y
   The value that the y_ value of the Point will be set to

\return
   A Point containing the specified values
******************************************************************************/
Point::Point(double x, double y)
{
  // Sets x / y to specified values
  x_ = x;
  y_ = y;
}

/*!****************************************************************************
\brief
  Rotates a Point about the origin the specified number of degrees.

\param angle
  Angle that the point will be rotated by

\return
   A Point at the new rotated position
******************************************************************************/
Point Point::operator%(double angle) const
{
  // Creates new point to hold the calculation
  Point result;

  // Performs basic 2x2 matrix and vector multiplication with angle conversion
  result.x_ = (cos(DegreesToRadians(angle)) * x_) +
              (-1 * sin(DegreesToRadians(angle)) * y_);
  result.y_ = (sin(DegreesToRadians(angle)) * x_) +
              (cos(DegreesToRadians(angle)) * y_);

  // Checks for rounding error and sets values as necessary to 0
  if (result.x_ > -EPSILON && result.x_ < EPSILON)
    result.x_ = 0;

  if (result.y_ > -EPSILON && result.y_ < EPSILON)
    result.y_ = 0;

  // Returns calculated point
  return result;
}

/*!****************************************************************************
\brief
  Calculates the difference between two Points and returns the distance.

\param pt2
  The point that distance between it and the class Point will be
  determined from

\return
   A double indicating the distance between the two points
******************************************************************************/
double Point::operator-(Point& pt2) const
{
  // Calculates the distance between the two points
  return sqrt(((pt2.x_ - x_) * (pt2.x_ - x_)) +
    ((pt2.y_ - y_) * (pt2.y_ - y_)));
}

/*!****************************************************************************
\brief
  Calculates the midpoint of two Points.

\param pt2
  The point that midpoint between it and the class Point will be
  determined from

\return
   A point that is the midpoint between the two points
******************************************************************************/
Point Point::operator^(Point& pt2) const
{
  // Creates new point to hold calculation
  Point result;

  // Determines the midpoint values through basic algorithm
  result.x_ = (x_ + pt2.x_) / 2;
  result.y_ = (y_ + pt2.y_) / 2;

  // Returns point holding calculation
  return result;
}

/*!****************************************************************************
\brief
  Adds two Points and returns a reference to the left-hand operand which has 
  been modified

\param pt2
  The point that will be added to the class Point

\return
   A point at the new position of the two point sums
******************************************************************************/
Point& Point::operator+=(Point& pt)
{
  // Adds the given point to the class point
  x_ += pt.x_;
  y_ += pt.y_;

  // Returns the class point
  return *this;
}

/*!****************************************************************************
\brief
  Adds a Point and a double and returns a reference to the left-hand operand 
  which has been modified

\param d
  The double that will be added to the class Point

\return
   A point at the new position of the Point and double sum
******************************************************************************/
Point& Point::operator+=(double d)
{
  // Adds given value to class Point
  x_ += d;
  y_ += d;

  // Returns class point
  return *this;
}

/*!****************************************************************************
\brief
  Subtracts a double from a Point

\param d
  The double that will be subtracted from the class Point

\return
   A point at the new position of the Point and double difference
******************************************************************************/
Point Point::operator-(double d) const
{
  // Gets the original values of the class point
  Point result = *this;

  // Subtracts the value from the original values
  result.x_ -= d;
  result.y_ -= d;

  // Returns point holding calculation
  return result;
}

/*!****************************************************************************
\brief
  Increments the x/y values of the class point

\return
   A reference to the incremented Point
******************************************************************************/
Point& Point::operator++()
{
  // Basic pre-increment of class point
  ++x_;
  ++y_;

  // Returns class point
  return *this;
}

/*!****************************************************************************
\brief
  Increments the x/y values of the class point

\return
   A point at the position before the increment
******************************************************************************/
Point Point::operator++(int)
{
  // New point to hold the original values of the class point
  Point result = *this;

  // Basic increment
  operator++();

  // Returns the original values
  return result;
}

/*!****************************************************************************
\brief
   Decrements the x/y values of the class point

\return
   A reference to the incremented Point
******************************************************************************/
Point& Point::operator--()
{
  // Basic decrement of class point
  --x_;
  --y_;

  // Returns class point
  return *this;
}

/*!****************************************************************************
\brief
  Decrements the x/y values of the class point

\return
   A point at the position before the decrement
******************************************************************************/
Point Point::operator--(int)
{
  // New point to hold the original values of the class point
  Point result = *this;

  // Basic decrement
  operator--();

  // Return the original point
  return result;
}

/*!****************************************************************************
\brief
  Negates the values of the class point

\return
  A point holding the decremented values of the class point
******************************************************************************/
Point Point::operator-() const
{
  // New point (0, 0)
  Point result;

  // Decrements the new point by the values of the class point
  result.x_ -= x_;
  result.y_ -= y_;

  // Returns calculation
  return result;
}

/*!****************************************************************************
\brief
  Adds the given point and the class point together

\return
   A point containing the new values of the position
******************************************************************************/
Point Point::operator+(const Point& pt) const
{
  // New point to hold calculation
  Point result;

  // Performs addition between the given point and the class point
  result.x_ = x_ + pt.x_;
  result.y_ = y_ + pt.y_;

  // Return the calcualted point
  return result;
}

/*!****************************************************************************
\brief
  Multipled the class point by a scalar value

\return
   A point containing the new values of the position
******************************************************************************/
Point Point::operator*(double d) const
{
  // New point to hold calculations
  Point result;

  // Multiples the class point by the value
  result.x_ = (x_ * d);
  result.y_ = (y_ * d);

  // Returns calculation
  return result;
}

///////////////////////////////////////////////////////////////////////////////
// 2 friend functions (operators)

/*!****************************************************************************
\brief
  Outputs the values of the given point

\return
  A stream output of the given point "(x, y)"
******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Point& pt)
{
  // Output the values of the point into a stream
  os << "(" << pt.x_ << ", " << pt.y_ << ")";

  // Returns steam
  return os;
}

/*!****************************************************************************
\brief
  Places the given values into given point

\return
  A point containing the specified values
******************************************************************************/
std::istream& operator>>(std::istream& is, Point& pt)
{
  // Inputs the values from the stream into the point
  is >> pt.x_ >> pt.y_;

  // Returns stream
  return is;
}


///////////////////////////////////////////////////////////////////////////////
// 2 non-members, non-friends (operators)

/*!****************************************************************************
\brief
  Adds a point and a double together (double as the left operand)

\return
   A point containing the new values of the position
******************************************************************************/
Point operator+(double d, Point& pt1)
{
  // New point to hold calculation
  Point result = pt1;

  // Returns calculation
  return *(&result.operator+=(d));
}

/*!****************************************************************************
\brief
  Adds a point and a double together (Point as the left operand)

\return
   A point containing the new values of the position
******************************************************************************/
Point operator+(Point& pt1, double d)
{
  // New point to hold calculation
  Point result = pt1;

  // Returns calculation
  return *(&result.operator+=(d));
}

/*!****************************************************************************
\brief
  Multiplies a point and a double together (double as the left operand)

\return
   A point containing the new values of the position
******************************************************************************/
Point operator*(double d, const Point& pt1)
{
  // Returns calculation
  return pt1.operator*(d);
}

} // namespace CS170



