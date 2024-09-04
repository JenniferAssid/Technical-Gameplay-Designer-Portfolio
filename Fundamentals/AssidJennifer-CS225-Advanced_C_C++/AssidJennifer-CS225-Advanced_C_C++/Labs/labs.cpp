/*
*
* Filename: labs.cpp
* Author: Jennifer Assid
* Date: 14 July 2024
*
* Description: Contains the lab class calls
*
*/

#include "labs.h"
#include "readline/readline.h"
#include "replace/replace.h"
#include "cipher/cipher.h"
#include "tddaa/tddaa.h"
#include "matrix/matrix.h"
#include "rational/rational.h"
#include "deque/deque.h"
#include "deque_template/deque_template.h"
#include "functions/functions.h"
#include "ray_intersection/shapes.h"
#include "polynomial/polynomial.h"

Labs::Labs() : SubMenu(_Labs,
    "Labs",
    "Run lab programs.")
{
    AddProgram("Readline", "Reads all characters before a new line (/n), places them in a buffer, and returns the result", &readline);
    AddProgram("Replace", "Iterates through a given string to find a given subscript and replace that subscript with a given replacement.If the modified string is too large for the buffer, false is returned", &replace);
    AddProgram("Cipher", "Takes encrypted text and decodes it as wellas vice versa", &cipher);
    AddProgram("tddaa", "Allocates and deallocates", &tddaa);
    AddProgram("Matrix", "Contains all the functions needed to create, manipulate, and analyze matrices", &matrix);
    AddProgram("Ray Intersection", "Contains various shape variablest to run ray intersections against", &shapes);
    AddProgram("Rational", "Demonstrates operator overloads", &rational);
    AddProgram("Deque", "Contains all the functions for a deque list", &deque);
    AddProgram("Deque Template", "Contains all the functions for a deque list - templatized", &deque_template);
    AddProgram("Functions", "Various functions that deal with values - templatized", &functions);
    AddProgram("Polynomial", "Different polynomial functions - operator overloads and templates", &polynomial);
}