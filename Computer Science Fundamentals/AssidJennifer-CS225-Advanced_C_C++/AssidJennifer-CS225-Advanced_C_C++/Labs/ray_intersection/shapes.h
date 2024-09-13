#pragma once
/*****************************************************************
 * \file   shapes.h
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Unknown
 * \date   Fall 2021
 * \brief
 *    Contains various shape variablest to run ray intersections against
 *********************************************************************/
#ifndef SHAPES_H
#define SHAPES_H

#include <utility> //pair<T1,T2>, make_pair, p.first, p.second
#include <fstream>

class Shape;

void shapes(void);

class Ray {
private:
    float  origin_x;
    float  origin_y;
    float  dir_x;
    float  dir_y;
public:
    // compiler generated copy, assignment, destructor are OK
    Ray(float o_x, float o_y, float d_x, float d_y);

    const float& Origin_x() const;
    const float& Origin_y() const;
    const float& Direction_x() const;
    const float& Direction_y() const;

    std::pair<bool, float> IntersectSegment(float p1x, float p1y, float p2x, float p2y) const;

    friend std::ostream& operator<<(std::ostream& os, Ray const& ray);

};

class BoundingCircle {
private:
    float  center_x;
    float  center_y;
    float  radius;
public:
    BoundingCircle(float c_x, float c_y, float r);
    bool IntersectTest(Ray const& ray) const;

    friend std::ostream& operator<< (std::ostream& os, BoundingCircle const& bs);

};

class Shape {
public:
    virtual ~Shape();
    virtual void Print() const = 0;
    virtual BoundingCircle GetBoundingCircle() const = 0;
    virtual std::pair<bool, float> Intersect(const Ray& ray) const = 0;

};

class Box : public Shape {
private:
    float center_x;
    float center_y;
    float side_x;
    float side_y;
public:
    // compiler generated copy, assignment, destructor are OK
    Box(float c_x, float c_y, float s_x, float s_y);
    BoundingCircle GetBoundingCircle() const;
    std::pair<bool, float> Intersect(Ray const& ray) const;

    friend std::ostream& operator<< (std::ostream& os, Box const& box);

    void Print() const;
};

class Rhombus : public Shape {
private:
    float center_x;
    float center_y;
    float diag_x;
    float diag_y;
public:
    // compiler generated copy, assignment, destructor are OK
    Rhombus(float c_x, float c_y, float dia_x, float dia_y);
    BoundingCircle GetBoundingCircle() const;
    std::pair <bool, float> Intersect(Ray const& ray) const;
    friend std::ostream& operator<< (std::ostream& os, Rhombus const& rhombus);
    void Print() const;
};


#endif
