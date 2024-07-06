/***************************************
filename	marathon.c
author		Jennifer Assid
DP email	jennifer.assid@digipen.edu
course		CS 120
section		A
lab week	2
due date	09/15/2020

Brief Description:
    This program calculates how many yards are within
    a marathon.

***************************************/

#include <stdio.h> /* printf */
#include "labs.h"

/* Convenient definitions */
#define YARDS_PER_MILE 1760
#define KILOS_PER_MILE 1.609

/* A marathon is 26 miles, 385 yards               */
/* Prints the distance of a marathon in kilometers */
void marathon(void)
{
    int miles = 26;    /* Miles in a marathon                 */
    int yards = 385;   /* Yards in a marathon                 */
    double kilometers; /* Calculated kilometers in a marathon */

    /* Convert miles and yards into kilometers */
    kilometers = (miles + (double)yards / YARDS_PER_MILE) * KILOS_PER_MILE;

    /* Display the result on the screen */
    printf("A marathon is %f kilometers\n", kilometers);
}