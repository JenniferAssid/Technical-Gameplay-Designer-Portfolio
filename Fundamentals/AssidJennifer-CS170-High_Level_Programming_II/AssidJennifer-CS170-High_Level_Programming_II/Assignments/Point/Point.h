/*!****************************************************************************
\file    Point.h
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

////////////////////////////////////////////////////////////////////////////////
#ifndef POINT_H
#define POINT_H
////////////////////////////////////////////////////////////////////////////////

#include <iostream> // istream, ostream

namespace CS170
{
  class Point
  {
    public:
      // Constructors (2)
      Point();
      Point(double x, double y);

      // Overloaded operators (14 member functions)
      // Rotation
      Point operator%(double angle) const;

      // Distance
      double operator-(Point& pt2) const;

      // Midpoint
      Point operator^(Point& pt2) const;

      // Translation (Point to point)
      Point& operator+=(Point& pt);
      // Translation (Point to double)
      Point& operator+=(double d);

      // Translation (Subtraction)
      Point operator-(double d) const;

      // Pre-Increment
      Point& operator++();
      // Post-Increment
      Point operator++(int);

      // Pre-Decrement
      Point& operator--();
      // Post-Decrement
      Point operator--(int);

      // Unary negation
      Point operator-() const;

      // Translation
      Point operator+(const Point& pt) const;

      // Scale
      Point operator*(double d) const;
        
      // Overloaded operators (2 friend functions)
      // Output
      friend std::ostream& operator<<(std::ostream& os, const Point& pt);
      // Input
      friend std::istream& operator>>(std::istream& is, Point& pt);
        
    private:
      double x_; // The x-coordinate of a Point
      double y_; // The y-coordinate of a Point

      // Helper functions
      double DegreesToRadians(double degrees) const;
      double RadiansToDegrees(double radians) const;
  };
  
  // Overloaded operators (2 non-member, non-friend functions)
  // Translation
  Point operator+(double d,Point& pt1);
  Point operator+(Point& pt1, double d);

  // Scale
  Point operator*(double d, const Point& pt1);
    
} // namespace CS170

#endif
////////////////////////////////////////////////////////////////////////////////
