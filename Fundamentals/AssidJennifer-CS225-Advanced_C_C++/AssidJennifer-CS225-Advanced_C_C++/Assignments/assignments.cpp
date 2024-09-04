/*
*
* Filename: assignments.cpp
* Author: Jennifer Assid
* Date: 10 July 2024
*
* Description: Contains the function calls for each assignment (tests)
*
*/

#include "assignments.h"
#include "splitter/splitter.h"
#include "sparsevector/sparsevector.h"
#include "sparsevector_CPP/sparsevector_CPP.h"
#include "lazycopy/lazycopy.h"

// Create each assignment menu and add their respective testing programs to be called by the user

Splitter_Assignment::Splitter_Assignment() : SubMenu(_Splitter,
    "Splitter",
    "This program holds functions necessary to split, join, and analayze files",
    true)
{
    AddProgram("Test 0", "Testing the split file functionality - file is 120 bytes so output expected is 2 chunks: 100 and 100 bytes", &splitter_test0);
    AddProgram("Test 1", "Testing the split file functionality - file is 120 bytes so output expected is 2 chunks: 100 and 100 bytes", &splitter_test1);
    AddProgram("Test 2", "Testing the split file functionality - file is 19999 bytes so output expected is 2 chunks: 10000 and 999 bytes", &splitter_test2);
    AddProgram("Test 3", "Testing the combine file functionality.", &splitter_test3);
}

Sparsevector_Assignment::Sparsevector_Assignment() : SubMenu(_Sparsevector,
    "Sparsevector",
    "This program holds functions necessary to manipulate and maintained a sparsevector",
    true)
{
    AddProgram("Test 0", "Insert Back - v1", &sparsevector_test0);
    AddProgram("Test 1", "Insert Back - v2", &sparsevector_test1);
    AddProgram("Test 2", "Insert Middle", &sparsevector_test2);
    AddProgram("Test 3", "Delete Front", &sparsevector_test3);
    AddProgram("Test 4", "Delete Back", &sparsevector_test4);
    AddProgram("Test 5", "Delete Middle", &sparsevector_test5);
    AddProgram("Test 6", "Scalar Manipulation - v1", &sparsevector_test6);
    AddProgram("Test 7", "Scalar Manipulation - v2", &sparsevector_test7);
    AddProgram("Test 8", "Vector Addition - v1", &sparsevector_test8);
    AddProgram("Test 9", "Vector Addition - v2", &sparsevector_test9);
    AddProgram("Test 10", "Vector Addition - Zeroes", &sparsevector_test10);
    AddProgram("Test 11", "Find - v1", &sparsevector_test11);
    AddProgram("Test 12", "Find - v2", &sparsevector_test12);
    AddProgram("Test 13", "Insert Row Front", &sparsevector_test13);
    AddProgram("Test 14", "Insert Row Back", &sparsevector_test14);
    AddProgram("Test 15", "Insert Row Middle", &sparsevector_test15);
    AddProgram("Test 16", "Insert Element in Row List", &sparsevector_test16);
    AddProgram("Test 17", "Insert Element in Row List - Zeroes", &sparsevector_test17);
    AddProgram("Test 18", "Insert Element in Row List - Zeroes and Overwrites", &sparsevector_test18);
    AddProgram("Test 19", "Overwrite Entire Row 0 with Zeroes", &sparsevector_test19);
    AddProgram("Test 20", "Overwrite Entire Row 1 with Zeroes", &sparsevector_test20);
    AddProgram("Test 21", "Overwrite Entire Row 9 with Zeroes", &sparsevector_test21);
    AddProgram("Test 22", "Transposition", &sparsevector_test22);
    AddProgram("Test 23", "Multiplication", &sparsevector_test23);
    AddProgram("Test 24", "Determinant", &sparsevector_test24);
}

Sparsevector_CPP_Assignment::Sparsevector_CPP_Assignment() : SubMenu(_Sparsevector_CPP,
    "Sparsevector CPP",
    "This program holds functions necessary to manipulate and maintained a sparsevector - now in C++",
    true)
{
    AddProgram("Test 0", "Insert Back - v1", &sparsevector_CPP_test0);
    AddProgram("Test 1", "Insert Back - v2", &sparsevector_CPP_test1);
    AddProgram("Test 2", "Copy - v1", &sparsevector_CPP_test2);
    AddProgram("Test 3", "Copy - v2", &sparsevector_CPP_test3);
    AddProgram("Test 4", "Copy and Delete", &sparsevector_CPP_test4);
    AddProgram("Test 5", "Copy and Addition", &sparsevector_CPP_test5);
    AddProgram("Test 6", "Copy and Addition and Compare", &sparsevector_CPP_test6);
    AddProgram("Test 7", "Scalar Multiplication", &sparsevector_CPP_test7);
    AddProgram("Test 8", "Scalar Multiplication and Addition", &sparsevector_CPP_test8);
    AddProgram("Test 9", "Multiplication", &sparsevector_CPP_test9);
    AddProgram("Test 10", "Index Setting - v1", &sparsevector_CPP_test10);
    AddProgram("Test 11", "Index Setting - v2", &sparsevector_CPP_test11);
    AddProgram("Test 12", "Testing += and -=", &sparsevector_CPP_test12);
    AddProgram("Test 13", "Testing +=", &sparsevector_CPP_test13);
    AddProgram("Test 14", "Testing with Indexes Set to Zero", &sparsevector_CPP_test14);
    AddProgram("Test 15", "Copy and Multiplication and Scalar Multiplication", &sparsevector_CPP_test15);
    AddProgram("Test 16", "Zero Multiplication", &sparsevector_CPP_test16);
    AddProgram("Test 17", "Copy constructor", &sparsevector_CPP_test17);
}

Lazycopy_Assignment::Lazycopy_Assignment() : SubMenu(_Lazycopy,
    "Lazycopy",
    "This program holds functions necessary to manipulate and maintained an array",
    true)
{
    AddProgram("Test 0", "Set", &lazycopy_test0);
    AddProgram("Test 1", "Copy Constructor", &lazycopy_test1);
    AddProgram("Test 2", "Copy Constructor and Set", &lazycopy_test2);
    AddProgram("Test 3", "Assignment - v1", &lazycopy_test3);
    AddProgram("Test 4", "Assignment - v2", &lazycopy_test4);
    AddProgram("Test 5", "Copy Element and Modify - v1", &lazycopy_test5);
    AddProgram("Test 6", "Copy Element and Modify - v2", &lazycopy_test6);
    AddProgram("Test 7", "Assignment with Element and Modify - v1", &lazycopy_test7);
    AddProgram("Test 8", "Assignment with Element and Modify - v1", &lazycopy_test8);
    AddProgram("Test 9", "Accept by Value", &lazycopy_test9);
    AddProgram("Test 10", "Copy and Delete", &lazycopy_test10);
    AddProgram("Test 11", "Assignment and Delete", &lazycopy_test11);
    AddProgram("Test 12", "Chaining", &lazycopy_test12);
    AddProgram("Test 13", "Assignment - v3", &lazycopy_test13);
    AddProgram("Test 14", "give-my-array", &lazycopy_test14);
    AddProgram("Test 15", "Copy Element and Modify with Virtual Constructor - v1", &lazycopy_test15);
    AddProgram("Test 16", "Copy Element and Modify with Virtual Constructor - v2", &lazycopy_test16);
}