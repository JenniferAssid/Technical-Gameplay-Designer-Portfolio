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
#include "WarBoats/warboats_driver.h"
#include "WarBoatsOO/warboatsOO_driver.h"
#include "Point/point_driver.h"
#include "Table/table_driver.h"
#include "List/list_driver.h"
#include "List_Template/List-template_driver.h"

// Create each assignment menu and add their respective testing programs to be called by the user

WarBoats_Assignment::WarBoats_Assignment() : SubMenu(_WarBoats,
    "War Boats",
    "This program holds functions necessary to establish, update, and track the playing of 'Battleship'",
    true)
{
    AddProgram("Test 0", "General test for functionality.", &warboats_test0);
    AddProgram("Test 1", "General test for functionality.", &warboats_test1);
    AddProgram("Test 2", "General test for functionality.", &warboats_test2);
}

WarBoatsOO_Assignment::WarBoatsOO_Assignment() : SubMenu(_WarBoatsOO,
    "War Boats Object Oriented",
    "Similar to War Boats but all code is now contained in classes (object-oriented).",
    true)
{
    AddProgram("Test 0", "General test for functionality.", &warboatsOO_test0);
    AddProgram("Test 1", "General test for functionality.", &warboatsOO_test1);
    AddProgram("Test 2", "General test for functionality.", &warboatsOO_test2);
    AddProgram("Test 3", "General test for functionality.", &warboatsOO_test3);
}

Point_Assignment::Point_Assignment() : SubMenu(_Point,
    "Point",
    "This program contains overloaded operators to manipulate class Point",
    true)
{
    AddProgram("Constructor", "Tests the constructor functions for the Point class.", &Point_ConstructorTest);
    AddProgram("Translate", "Tests the overloaded '+' operator for the Point class.", &Point_TranslateTest);
    AddProgram("Scale", "Tests the overloaded '*' operator for the Point class.", &Point_ScaleTest);
    AddProgram("Rotate", "Tests the overloaded '%' operator for the Point class.", &Point_RotateTest);
    AddProgram("Distance", "Tests the overloaded '-' (subtraction) operator for the Point class.", &Point_DistanceTest);
    AddProgram("Midpoint", "Tests the overloaded '^' operator for the Point class.", &Point_MidpointTest);
    AddProgram("Input", "Tests the overloaded '<<' operator for the Point class.", &Point_InputTest);
    AddProgram("Assignment", "Tests the overloaded '+=' operator for the Point class.", &Point_AssignmentTest);
    AddProgram("IncDec", "Tests the overloaded '++'/'--' postfix/prefix operators for the Point class.", &Point_IncDecTest);
    AddProgram("Unary", "Tests the overloaded '-' operator for the Point class.", &Point_UnaryTest);
}

Table_Assignment::Table_Assignment() : SubMenu(_Table,
    "Table",
    "This program contains functions pertaining to the creation, maintenance, and manipulation of 2D string arrays",
    true)
{
    AddProgram("Test 0", "General test for functionality.", &table_test0);
    AddProgram("Test 1", "General test for functionality.", &table_test1);
    AddProgram("Test 2", "General test for functionality.", &table_test2);
    AddProgram("Test 3", "General test for functionality.", &table_test3);
    AddProgram("Test 4", "Copy constructor.", &table_test4);
    AddProgram("Test 5", "Copy constructor, create copy, modify original, display both, modify copy.", &table_test5);
    AddProgram("Test 6", "reverse (transpose) 4x4 table.", &table_test6);
    AddProgram("Test 7", "reverse (transpose) 5x3 table.", &table_test7);
    AddProgram("Test 8", "multiple reverses - elements, rows, tables.", &table_test8);
    AddProgram("Test 9", "operator* (tensor product).", &table_test9);
    AddProgram("Test 10", "operator* (tensot product).", &table_test10);
    AddProgram("Test 11", "operator+ - one dim is the same - v0.", &table_test11);
    AddProgram("Test 12", "operator+ - one dim is the same - v1.", &table_test12);
    AddProgram("Test 13", "operator+ - one dim is the same - v2.", &table_test13);
    AddProgram("Test 14", "operator+ - one dim is the same - v3.", &table_test14);
    AddProgram("Test 15", "operator+ - table_testing default constructed potition.", &table_test15);
    AddProgram("Test 16", "operator+ - one table is smaller in both dimensions.", &table_test16);
    AddProgram("Test 17", "Assignment operator.", &table_test17);
    AddProgram("Test 18", "Assignment operator, assign, modify original, display both, modify copy", &table_test18);
    AddProgram("Test 19", "Assignment operator - stress table_test + self assignment table_test.", &table_test19);
    AddProgram("Test 20", "Stress table test.", &table_test20);
    AddProgram("Test 21", "Stress table test - NO OUTPUT; checking for crashes (takes several seconds).", &table_test21);
}

List_Assignment::List_Assignment() : SubMenu(_List,
    "List",
    "This program houses functions necessary to create, analyize, and maintain a linked list of numbers within a class List",
    true)
{
    AddProgram("Test Push Front", "words", &List_TestPushFront);
    AddProgram("Test Push Back", "words", &List_TestPushBack);
    AddProgram("Test Push Front and Back", "words", &List_TestPushFrontBack);
    AddProgram("Test Array - v1", "words", &List_TestArray1);
    AddProgram("Test Array - v2", "words", &List_TestArray2);
    AddProgram("Test Copy Constructor - v1", "words", &List_TestCopyConstructor1);
    AddProgram("Test Copy Constructor - v2", "words", &List_TestCopyConstructor2);
    AddProgram("Test Assignment - v1", "words", &List_TestAssignment1);
    AddProgram("Test Assignment - v2", "words", &List_TestAssignment2);
    AddProgram("Test Addition - v1", "words", &List_TestAddition1);
    AddProgram("Test Addition - v2", "words", &List_TestAddition2);
    AddProgram("Test Add and Assign - v1", "words", &List_TestAddAssign1);
    AddProgram("Test Add and Assign - v2", "words", &List_TestAddAssign2);
    AddProgram("Test Add and Assign - v3", "words", &List_TestAddAssign3);
    AddProgram("Test Add and Assign - v4", "words", &List_TestAddAssign4);
    AddProgram("Test Subscript - v1", "words", &List_TestSubscript1);
    AddProgram("Test Subscript - v2", "words", &List_TestSubscript2);
}

ListTemplate_Assignment::ListTemplate_Assignment() : SubMenu(_ListTemplate,
    "List Template",
    "This program houses functions necessary to create, analyize, and maintain a linked List_Template of numbers within a class List_Template",
    true)
{
    FillPersonRecs();
    AddProgram("Test Push Front", "words", &ListTemplate_TestPushFront);
    AddProgram("Test Push Back", "words", &ListTemplate_TestPushBack);
    AddProgram("Test Push Front and Back", "words", &ListTemplate_TestPushFrontBack);
    AddProgram("Test Array - v1", "words", &ListTemplate_TestArray1);
    AddProgram("Test Array - v2", "words", &ListTemplate_TestArray2);
    AddProgram("Test Copy Constructor - v1", "words", &ListTemplate_TestCopyConstructor1);
    AddProgram("Test Copy Constructor - v2", "words", &ListTemplate_TestCopyConstructor2);
    AddProgram("Test Assignment - v1", "words", &ListTemplate_TestAssignment1);
    AddProgram("Test Assignment - v2", "words", &ListTemplate_TestAssignment2);
    AddProgram("Test Addition - v1", "words", &ListTemplate_TestAddition1);
    AddProgram("Test Addition - v2", "words", &ListTemplate_TestAddition2);
    AddProgram("Test Add and Assign - v1", "words", &ListTemplate_TestAddAssign1);
    AddProgram("Test Add and Assign - v2", "words", &ListTemplate_TestAddAssign2);
    AddProgram("Test Add and Assign - v3", "words", &ListTemplate_TestAddAssign3);
    AddProgram("Test Add and Assign - v4", "words", &ListTemplate_TestAddAssign4);
    AddProgram("Test Subscript - v1", "words", &ListTemplate_TestSubscript1);
    AddProgram("Test Subscript - v2", "words", &ListTemplate_TestSubscript2);
}

ListTemplate_Assignment::~ListTemplate_Assignment()
{
    FreePersonRecs();
}