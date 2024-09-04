/*****************************************************************
 * \file   shapes.cpp
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
#include <limits> // std::numeric_limits<T>::max(), std::numeric_limits<T>::min()
#include <cmath>  //std::sqrt
#include <iostream>
#include <algorithm> // std::max
#include "shapes.h"

 ////////////////////////////////////////////////////////////////////////////////
 //Ray class implementation
Ray::Ray(float o_x, float o_y, float d_x, float d_y) : origin_x(o_x), origin_y(o_y), dir_x(d_x), dir_y(d_y) {}

std::ostream& operator<<(std::ostream& os, Ray const& ray) {
    os << " origin(" << ray.origin_x << "," << ray.origin_y << "), direction ( " << ray.dir_x << "," << ray.dir_y << ")";
    return os;
}

float const& Ray::Origin_x()    const { return origin_x; }
float const& Ray::Origin_y()    const { return origin_y; }
float const& Ray::Direction_x() const { return dir_x; }
float const& Ray::Direction_y() const { return dir_y; }

std::pair<bool, float>  Ray::IntersectSegment(float p1x, float p1y, float p2x, float p2y) const {
    float ox = origin_x;
    float oy = origin_y;
    float dx = dir_x;
    float dy = dir_y;

    //std::cout << "Segment " << p1x << "," << p1y << " - " << p2x << "," << p2y << " with ray " << ray << std::endl;

    //ray parametric:      XR = ox + dx*t, YR = oy + dy*t
    //segment parametric:  XI = p1x + (p2x-p1x)*s, YI = p1y + (p2y-p1y)*s
    //solve for t, make sure 0<s<1 (strict inequality means corner point 
    //does not reflect the ray)

    //solve system:
    //ox + dx*t = p1x + (p2x-p1x)*s
    //oy + dy*t = p1y + (p2y-p1y)*s
    //-----------------------------
    //dx*t = p1x + (p2x-p1x)*s - ox
    //dy*t = p1y + (p2y-p1y)*s - oy
    //-----------------------------
    //dy*dx*t = dy*( p1x + (p2x-p1x)*s - ox )
    //dx*dy*t = dx*( p1y + (p2y-p1y)*s - oy )
    //-----------------------------
    //dy*( p1x + (p2x-p1x)*s - ox ) = dx*( p1y + (p2y-p1y)*s - oy )
    //-----------------------------
    //dy*p1x + dy*(p2x-p1x)*s - dy*ox = dx*p1y + dx*(p2y-p1y)*s - dx*oy
    //-----------------------------
    //dy*(p2x-p1x)*s - dx*(p2y-p1y)*s = dx*p1y - dx*oy + dy*ox - dy*p1x
    //-----------------------------
    //[ dy*(p2x-p1x) - dx*(p2y-p1y) ]*s = dx*p1y - dx*oy + dy*ox - dy*p1x

    if (dy * (p2x - p1x) - dx * (p2y - p1y) == 0) { // parallel lines
        //std::cout << "parallel\n";
        return std::make_pair(false, -1);
    }

    float s = (dx * p1y - dx * oy + dy * ox - dy * p1x) / (dy * (p2x - p1x) - dx * (p2y - p1y));
    if (s < 0 || s> 1) {
        //std::cout << "c<=0 or c>=1\n";
        return std::make_pair(false, -1);
    }

    //to find t use any of the previous equations, for example 
    //ox + dx*t = p1x + (p2x-p1x)*s
    //t = [ p1x - ox + ( (p2x-p1x)*s ) ] / dx
    float t;
    if (dx == 0) { //vertical segment
        //use 
        //oy + dy*t = p1y + (p2y-p1y)*s 
        //t = [ p1y - oy  + (p2y-p1y)*s ] / dy
        //dx and dy cannot be both 0
        t = (p1y - oy + (p2y - p1y) * s) / dy;
    }
    else {
        t = (p1x - ox + ((p2x - p1x) * s)) / dx;
    }

    //only if t is positive
    //std::cout << "t = " << t << "\n";
    return t >= 0 ? std::make_pair(true, t) : std::make_pair(false, -1.0f);
}


////////////////////////////////////////////////////////////////////////////////
//BoundingCircle implementation
BoundingCircle::BoundingCircle(float c_x, float c_y, float r) : center_x(c_x), center_y(c_y), radius(r) {}

bool BoundingCircle::IntersectTest(Ray const& ray) const {
    float ox = ray.Origin_x();
    float oy = ray.Origin_y();
    float dx = ray.Direction_x();
    float dy = ray.Direction_y();
    //to test - find distance from circle center to  the ray, 
    //if <=r - there is intersection
    //use ray equation (X-ox)*dy - (Y-oy)*dx = 0
    //then distance is      [ (cx-ox)*dy - (cy-oy)*dx ] / sqrt( dx^2+dy^2 ) 
    //to eliminate sqrt:    [ (cx-ox)*dy - (cy-oy)*dx ]^2 / ( dx^2+dy^2 ) < r^2
    float temp = ((center_x - ox) * dy - (center_y - oy) * dx);
    return ((temp * temp) / (dx * dx + dy * dy) <= radius * radius);
}

std::ostream& operator<< (std::ostream& os, BoundingCircle const& bs) {
    os << "center (" << bs.center_x << "," << bs.center_y << "), radius " << bs.radius;
    return os;
}


////////////////////////////////////////////////////////////////////////////////
//Shape class implementation
Shape::~Shape() {}


////////////////////////////////////////////////////////////////////////////////
//Box class implementation
Box::Box(float c_x, float c_y, float s_x, float s_y) : center_x(c_x), center_y(c_y), side_x(s_x), side_y(s_y) {}

BoundingCircle Box::GetBoundingCircle() const {
    return BoundingCircle(center_x, center_y, std::sqrt(side_x * side_x + side_y * side_y) / 2);
}

std::pair<bool, float> Box::Intersect(Ray const& ray) const {
    //for each side (segment) solve segment-ray intersection and find minimum
    /*
    // +--------------+/
    // |              +
    // |             /| may be 2 intersection points
    // |            / |
    // +-----------+--+
    //            / ray
    */

    bool intersect = false;
    float t = std::numeric_limits<float>::max();
    std::pair<bool, float> t1 = ray.IntersectSegment(
        center_x - side_x / 2, center_y - side_y / 2,
        center_x + side_x / 2, center_y - side_y / 2);
    if (t1.first) { t = ((t1.second < t) ? t1.second : t); intersect = true; }
    std::pair<bool, float>  t2 = ray.IntersectSegment(
        center_x - side_x / 2, center_y - side_y / 2,
        center_x - side_x / 2, center_y + side_y / 2);
    if (t2.first) { t = ((t2.second < t) ? t2.second : t); intersect = true; }
    std::pair<bool, float>  t3 = ray.IntersectSegment(
        center_x + side_x / 2, center_y + side_y / 2,
        center_x - side_x / 2, center_y + side_y / 2);
    if (t3.first) { t = ((t3.second < t) ? t3.second : t); intersect = true; }
    std::pair<bool, float>  t4 = ray.IntersectSegment(
        center_x + side_x / 2, center_y + side_y / 2,
        center_x + side_x / 2, center_y - side_y / 2);
    if (t4.first) { t = ((t4.second < t) ? t4.second : t); intersect = true; }

    return std::make_pair(intersect, t);

}

std::ostream& operator<< (std::ostream& os, Box const& box) {
    os << "Box( center (" << box.center_x << "," << box.center_y << "), sides ("
        << box.side_x << "," << box.side_y << ") )";
    return os;
}

void Box::Print() const {
    std::cout << *this;
}

////////////////////////////////////////////////////////////////////////////////
//Rhombus class implementation
Rhombus::Rhombus(float c_x, float c_y, float dia_x, float dia_y) : center_x(c_x), center_y(c_y), diag_x(dia_x), diag_y(dia_y) {}

BoundingCircle Rhombus::GetBoundingCircle() const {
    return BoundingCircle(center_x, center_y, std::max(diag_x, diag_y) / 2);
}

std::pair<bool, float> Rhombus::Intersect(Ray const& ray) const {
    //for each side (segment) solve segment-ray intersection and find minimum
    /*           X
    //          / \     may be 2 intersection points.
    //     ----+---+---<--------ray
    //        /     \
    //       X       X
    //        \     /
    //         \   /
    //          \ /
    //           X
    */
    bool intersect = false;
    float t = std::numeric_limits<float>::max();
    std::pair<bool, float> t1 = ray.IntersectSegment(
        center_x - diag_x / 2, center_y,
        center_x, center_y - diag_y / 2);
    if (t1.first) { t = ((t1.second < t) ? t1.second : t); intersect = true; }
    std::pair<bool, float>  t2 = ray.IntersectSegment(
        center_x - diag_x / 2, center_y,
        center_x, center_y + diag_y / 2);
    if (t2.first) { t = ((t2.second < t) ? t2.second : t); intersect = true; }
    std::pair<bool, float>  t3 = ray.IntersectSegment(
        center_x + diag_x / 2, center_y,
        center_x, center_y + diag_y / 2);
    if (t3.first) { t = ((t3.second < t) ? t3.second : t); intersect = true; }
    std::pair<bool, float>  t4 = ray.IntersectSegment(
        center_x + diag_x / 2, center_y,
        center_x, center_y - diag_y / 2);
    if (t4.first) { t = ((t4.second < t) ? t4.second : t); intersect = true; }

    return std::make_pair(intersect, t);

}

std::ostream& operator<< (std::ostream& os, Rhombus const& rhombus) {
    os << "Rhombus( center (" << rhombus.center_x << "," << rhombus.center_y << "), diagonals ("
        << rhombus.diag_x << "," << rhombus.diag_y << ") )";
    return os;
}

void Rhombus::Print() const {
    std::cout << *this;
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY]
#include "shapes.h" //Shape,Box,Rhombus,BoundingCircle,Ray
#include <iostream>
#include <cstdio>   //sscanf
#include "trace.h"  //global Intersect

////////////////////////////////////////////////////////////////////////////////
void ray_intersection_test0() {
    Box box1(3, 2, 2, 2);
    Box box2(6, 0, 2, 2);
    Ray ray(0, 0, 1, 0);

    std::pair<bool, float> res1 = box1.Intersect(ray);
    std::pair<bool, float> res2 = box2.Intersect(ray);

    std::cout << "intersect shape " << box1 << " and ray " << ray << " - ";
    if (res1.first) {
        std::cout << "true, t = " << res1.second << std::endl;
    }
    else {
        std::cout << "false" << std::endl;
    }

    std::cout << "Intersect shape " << box2 << " and ray " << ray << " - ";
    if (res2.first) {
        std::cout << "true, t = " << res2.second << std::endl;
    }
    else {
        std::cout << "false" << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
void ray_intersection_test1() {
    Box box1(3, 2, 2, 2);
    Box box2(6, 0, 2, 2);
    Ray ray(0, 0, 1, 0);

    BoundingCircle bc1 = box1.GetBoundingCircle();
    BoundingCircle bc2 = box2.GetBoundingCircle();

    bool bc_int1 = bc1.IntersectTest(ray);
    bool bc_int2 = bc2.IntersectTest(ray);

    std::cout << "Intersect bounding circle " << bc1 << " and ray " << ray;
    std::cout << " - " << std::boolalpha << bc_int1 << std::endl;

    std::cout << "Intersect bounding circle " << bc2 << " and ray " << ray;
    std::cout << " - " << std::boolalpha << bc_int2 << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void ray_intersection_test2() {
    Box box1(4, 4, 6, 6);
    Box box2(10, 0, 4, 4);
    Ray ray(0, 0, 1, 0);

    BoundingCircle bc1 = box1.GetBoundingCircle();
    BoundingCircle bc2 = box2.GetBoundingCircle();

    bool bc_int1 = bc1.IntersectTest(ray);
    bool bc_int2 = bc2.IntersectTest(ray);

    std::cout << "Intersect bounding circle " << bc1 << " and ray " << ray;
    std::cout << " - " << std::boolalpha << bc_int1 << std::endl;
    if (bc_int1) { // if intersects circle, perform full intersection ray_intersection_test
        std::pair<bool, float> res1 = box1.Intersect(ray);
        std::cout << "intersect shape " << box1 << " and ray " << ray << " - ";
        if (res1.first) {
            std::cout << "true, t = " << res1.second << std::endl;
        }
        else {
            std::cout << "false" << std::endl;
        }
    }

    std::cout << "Intersect bounding circle " << bc2 << " and ray " << ray;
    std::cout << " - " << std::boolalpha << bc_int2 << std::endl;
    if (bc_int2) { // if intersects circle, perform full intersection ray_intersection_test
        std::pair<bool, float> res2 = box2.Intersect(ray);
        std::cout << "intersect shape " << box2 << " and ray " << ray << " - ";
        if (res2.first) {
            std::cout << "true, t = " << res2.second << std::endl;
        }
        else {
            std::cout << "false" << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ray_intersection_test3() {
    Shape* shapes[] = { new Box(3,2, 2,2), new Box(6,0, 2,2), new Box(5,5, 2,2) };
    Ray ray(0, 0, 1, 1);

    int num_shapes = sizeof(shapes) / sizeof(*shapes);

    std::cout << "Ray " << ray << "\n================\n";
    for (int i = 0; i < num_shapes; ++i) {
        shapes[i]->Print();
        std::cout << ": first check intersection with a bounding circle";
        if (shapes[i]->GetBoundingCircle().IntersectTest(ray)) { //bounding circle ray_intersection_test
            std::cout << " - true, make shape-intersection ray_intersection_test";
            std::pair<bool, float> res = shapes[i]->Intersect(ray);
            if (res.first) {
                std::cout << " - true, t = " << res.second << std::endl;
            }
            else {
                std::cout << " - false\n";
            }
        }
        else {
            std::cout << " - false, skip shape-intersection ray_intersection_test\n";
        }
    }

    for (int i = 0; i < num_shapes; ++i) {
        delete shapes[i];
    }
}

////////////////////////////////////////////////////////////////////////////////
void ray_intersection_test4() {
    Shape* shapes[] = { new Box(3,2, 2,2), new Box(6,-4, 6,6), new Rhombus(12,-2, 4,4), new Box(10,3, 4,4), new Rhombus(20,5, 12,12) };
    // see pic.pdf for above arrangement
    Ray ray(0, 0, 1, 0);

    int num_shapes = sizeof(shapes) / sizeof(*shapes);

    //    for ( int i=0; i<num_shapes; ++i) {
    //        std::cout << "BoundingCircle " << shapes[i]->GetBoundingCircle() << std::endl;
    //    }

    std::cout << "Ray " << ray << "\n================\n";
    for (int i = 0; i < num_shapes; ++i) {
        shapes[i]->Print();
        std::cout << ": first check intersection with a bounding circle";
        if (shapes[i]->GetBoundingCircle().IntersectTest(ray)) { //bounding circle ray_intersection_test
            std::cout << " - true, make shape-intersection ray_intersection_test";
            std::pair<bool, float> res = shapes[i]->Intersect(ray);
            if (res.first) {
                std::cout << " - true, t = " << res.second << std::endl;
            }
            else {
                std::cout << " - false\n";
            }
        }
        else {
            std::cout << " - false, skip shape-intersection ray_intersection_test\n";
        }
    }

    for (int i = 0; i < num_shapes; ++i) {
        delete shapes[i];
    }
}

////////////////////////////////////////////////////////////////////////////////
void ray_intersection_test5() {
    Shape* shapes[] = { new Box(3,2, 2,2), new Box(6,-4, 6,6), new Rhombus(12,-2, 4,4), new Box(10,3, 4,4), new Rhombus(20,5, 12,12) };
    int num_shapes = sizeof(shapes) / sizeof(*shapes);

    Ray rays[] = { Ray(0,0, 1,0), Ray(1,1, 1,0), Ray(8,8, 0,-1), Ray(24,13, -1,-1) };
    int num_rays = sizeof(rays) / sizeof(*rays);

    for (int i = 0; i < num_rays; ++i) {
        std::cout << "Ray " << rays[i] << "  ";
        std::pair<bool, float> res = Intersect(shapes, num_shapes, rays[i]); // trace.h
        if (res.first) { std::cout << "The first intersection is at t = " << res.second << std::endl; }
        else { std::cout << "There is no intersection\n"; }
    }


    for (int i = 0; i < num_shapes; ++i) { delete shapes[i]; }
}


void (*pray_intersection_tests[])() = { ray_intersection_test0,ray_intersection_test1,ray_intersection_test2,ray_intersection_test3,ray_intersection_test4,ray_intersection_test5};

void shapes() {
    for (size_t i = 0; i < sizeof(pray_intersection_tests) / sizeof(pray_intersection_tests[0]); ++i)
        pray_intersection_tests[i]();
}
