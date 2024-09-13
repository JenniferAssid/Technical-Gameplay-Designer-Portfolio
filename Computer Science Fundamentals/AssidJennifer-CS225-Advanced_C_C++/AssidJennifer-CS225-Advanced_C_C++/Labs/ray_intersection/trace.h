#pragma once
// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#ifndef TRACE_H
#define TRACE_H
#include "shapes.h"

//one and only global function (not counting main)
std::pair<bool, float>
Intersect(Shape const* const* shapes, int num_shapes, Ray const& ray);

#endif
