/***************************************
filename    numbers.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    3
due date    09/24/2020

Brief Description:
    This program takes three inputs, calculates their sin/cos/tan/atan
    values, and prints the results in a table.

***************************************/

#include <stdio.h>
#include <math.h>
#include "labs.h"

void numbers(void)
{
    /* Define the variables input1, input2, input3 */
    float input1 = 0;
    float input2 = 0;
    float input3 = 0;

    /* Define subheading */
    char subheading[100] = "-------------------------------------------";

    /* Asking user for specific input */
    printf("Enter 3 numbers between 0 and 9.999: \n");

    /* Taking multiple inputs from user */
    scanf("%f%f%f", &input1, &input2, &input3);

    /* Print heading and subheading */
    printf("Number      sin      cos      tan      atan\n");
    printf("%s\n", subheading);

    /* Print sin/cos/tan/atan values of all three inputs */
    printf("%6.5f   % 5.3f   % 5.3f   % 5.3f   % 5.3f\n", input1, sin(input1),
        cos(input1), tan(input1), atan(input1));
    printf("%6.5f   % 5.3f   % 5.3f   % 5.3f   % 5.3f\n", input2, sin(input2),
        cos(input2), tan(input2), atan(input2));
    printf("%6.5f   % 5.3f   % 5.3f   % 5.3f   % 5.3f\n", input3, sin(input3),
        cos(input3), tan(input3), atan(input3));
}