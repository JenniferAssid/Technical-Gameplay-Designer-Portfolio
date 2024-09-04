// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include "shapes.h" // Shape,Box,Rhombus,BoundingCircle,Ray
#include "trace.h"  // global Intersect
#include <limits>   // std::numeric_limits<T>::max(), std::numeric_limits<T>::min()
#include <utility>  // pair<T1,T2>, make_pair, p.first, p.second

std::pair<bool, float>
Intersect(Shape const* const* shapes, int num_shapes, Ray const& ray) {
    bool intersect = false;
    float t = std::numeric_limits<float>::max(); //first intersection
    for (int i = 0; i < num_shapes; i++)
    {
        if (shapes[i]->GetBoundingCircle().IntersectTest(ray))
        {
            std::pair<bool, float> res = shapes[i]->Intersect(ray);
            if (res.first)
            {
                intersect = true;
                if (res.second < t) t = res.second;
            }
        }
    }

    return std::make_pair(intersect, t);
}
