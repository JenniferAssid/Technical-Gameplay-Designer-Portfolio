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
#include "Fibonacci/fibonacci.h"
#include "00 List/00_list_lab.h"
#include "01 List/01_list_lab.h"
#include "02 List/02_list_lab.h"
#include "03 List/03_list_lab.h"
#include "04 List/04_list_lab.h"
#include "00 Vector/00_vector_lab.h"
#include "01 Vector/01_vector_lab.h"
#include "02 Vector/02_vector_lab.h"
#include "03 Vector/03_vector_lab.h"

Labs::Labs() : SubMenu(_Labs,
    "Labs",
    "Run lab programs.")
{
    AddProgram("Fibonacci", 
        "This program takes a number and creates a table containing every Fibonacci number and quotient from 0 to the number given.", 
        &fibonacci_test);
    AddProgram("List Lab - v0",
        "This program houses functions necessary to create, analyize, and maintain linked lists",
        &list_lab_0);
    AddProgram("List Lab - v1",
        "This program houses functions necessary to create, analyize, and maintain linked lists contained in a List class",
        &list_lab_1);
    AddProgram("List Lab - v2",
        "This program houses functions necessary to create, analyize, and maintain linked lists contained in a List class - now with additional functionality",
        &list_lab_2);
    AddProgram("List Lab - v3",
        "This program houses functions necessary to create, analyize, and maintain linked lists contained in a List class - now with merging",
        &list_lab_3);
    AddProgram("List Lab - v4",
        "This program houses functions necessary to create, analyize, and maintain linked lists contained in a List class - now with duplicate testing",
        &list_lab_4);
    AddProgram("Vector Lab - v0",
        "This program houses functions necessary to create, analyize, and maintain an array of numbers within a class Vector",
        &vector_lab_0);
    AddProgram("Vector Lab - v1",
        "This program houses functions necessary to create, analyize, and maintain an array of numbers within a class Vector - now with operator overloads",
        &vector_lab_1);
    AddProgram("Vector Lab - v2",
        "This program houses functions necessary to create, analyize, and maintain an array of numbers within a class Vector - additional functionality",
        &vector_lab_2);
    AddProgram("Vector Lab - v3",
        "This program houses functions necessary to create, analyize, and maintain an array of numbers within a class Vector - binary search",
        &vector_lab_3);
}