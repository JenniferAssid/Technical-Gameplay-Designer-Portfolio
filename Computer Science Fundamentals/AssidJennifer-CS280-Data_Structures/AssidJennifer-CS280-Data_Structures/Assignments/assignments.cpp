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
#include "ObjectAllocator/objectallocator.h"

// Create each assignment menu and add their respective testing programs to be called by the user

ObjectAllocator_Assignment::ObjectAllocator_Assignment() : SubMenu(_ObjectAllocator,
    "Object Allocator",
    "This program mimicks a computer's memory system.",
    true)
{
    AddProgram("DoStudents", "Check memory allocators ability to handle a mock scenario concerning 'students'.", &OA_DoStudents1);
}