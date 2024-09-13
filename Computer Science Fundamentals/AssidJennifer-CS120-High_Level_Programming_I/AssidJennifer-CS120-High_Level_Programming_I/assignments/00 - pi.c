/***************************************
filename    pi.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    4
due date    10/05/2020

Brief Description:
    Takes user input and uses it as iterations to run through the Riemann Sum
    and Leibniz equations to give an output via table.

***************************************/

#include <stdio.h> /* printf, scanf */
#include <math.h> /* sqrt */

#include "assignments.h"

/******************************************************************************
   Function: circle_pi

Description: Given number of iterations, the function applies the Riemann Sum
             to built-in circle measurements. The number of iterations shows
             the precision of the pi estimation that will be outputted.

     Inputs: int rectangles - an integer specifying how many "rectangles" are
                              to be accounted for in the equation; thus, how
                              many iterations the function must undergo.

    Outputs: The number (double) of the pi estimation gathered from the math.
******************************************************************************/
double circle_pi(int rectangles)
{
    /* Initializing the function variables */
    int cir_counter = 0;
    double radius = 2.0;
    double rect_width = radius / (double)rectangles;
    double rect_height = 0.0;
    double rect_area = 0.0;
    double rect_midpoint;

    rect_midpoint = rect_width / 2.0;

    /* making a loop to run through as many iterations as specified */
    for (cir_counter = 1; cir_counter <= rectangles; cir_counter++)
    {
        /* base equations */
        rect_height = sqrt(radius * radius - rect_midpoint * rect_midpoint);

        /* reestablishing midpoint for next iteration */
        rect_midpoint = (rect_width * cir_counter) + (rect_width * 0.5);

        /* completing iteration calculation */
        rect_area = rect_area + (rect_width * rect_height);
    }

    /* return value back to call statement */
    return rect_area;
}

/******************************************************************************
   Function: leibniz_pi

Description: Given number of iterations, the function applies the Leibniz Pi
             Formula and derives a pi estimation from a series of fractions.

     Inputs: int iterations - an integer specifying how many "objects"
                              (fractions) need to be deduced and added together.

    Outputs: The number (double) of the pi estimation gathered from the math.
******************************************************************************/
double leibniz_pi(int iterations)
{
    /* sets all necessary variables */
    int lei_counter = 1;
    double denominator = 1;
    double pi = 0;

    /* making a loop to run through as many iterations as specified */
    while (lei_counter <= iterations)
    {
        /* determines which series needs to be created based off of iteration*/
        if (lei_counter % 2 == 0)
        {
            pi = pi + (1 / denominator);
        }
        else
        {
            pi = pi - (1 / denominator);
        }
        denominator = denominator + 2;

        lei_counter++;
    }
    /* determines approximation of pi*/
    pi = pi * 4 * -1;

    return pi;
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/30/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
// Original author: Artie Fufkin
void pi(void)
{
    int iterations; /* loop counter */

    /* Print out table header */
    printf("Approximations for pi\n");
    printf("Iterations      Circle Method   Leibniz Method\n");
    printf("----------------------------------------------\n");

    /* Print out values for each set of numbers */
    for (iterations = 1; iterations <= 1000000; iterations *= 10)
    {
        /* Calculate PI with both methods */
        double pi_circle = circle_pi(iterations);
        double pi_leibniz = leibniz_pi(iterations);

        /* Print the results of the calculations */
        printf("%10i%20.12f%16.12f\n", iterations, pi_circle, pi_leibniz);
    }
}