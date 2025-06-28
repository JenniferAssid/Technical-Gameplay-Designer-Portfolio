/*
*
* Filename: main.cpp
* Author: Jennifer Assid
* Date: 8 July 2024
*
* Description: Creating one application to contain all of the course work completed at
*              CS170: High-Level Programming II at DigiPen Institute of Technology during
*              the Spring 2021 semester.
*
*/

#include <iostream>
#include <string>

#include "state_manager.h"

int main(void)
{
    StateManager sm;

    while (sm.Program_Running_)
    {
        sm.Update();
    }
}