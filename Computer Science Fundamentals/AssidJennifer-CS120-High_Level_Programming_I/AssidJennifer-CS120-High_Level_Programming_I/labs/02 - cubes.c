/***************************************
filename    triple_cube.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    5
due date    10/08/2020

Brief Description:
    contains two functions: print_cubes and print_pythagorean_triples

    print_cubes goes through all three digit numbers (100 - 999) and determines
     which three-digit numbers have the cube property but cubing each
     of the number's digits, adding their cubes together, and comparing
     their sum to the number.

    print_pythagorean_triples goes through a series of three digits ranging from
     any two inputted integers, determines whether the three digits make up a
     Pythagorean Triple by seeing if the sum of the squares of the first two
     digits equal the square of the second, then print out the digits and
     squares in a chart.

***************************************/

#include <stdio.h> /* printf */
#include "labs.h"

/******************************************************************************
   Function: print_cubes

Description: Goes through all three digit numbers (100 - 999) and determines
             which three-digit numbers have the cube property but cubing each
             of the number's digits, adding their cubes together, and comparing
             their sum to the number.

     Inputs: N/A

    Outputs: The numbers that have cube property as well as the digit cubes set
             in an addition formula with one another.
******************************************************************************/

void print_cubes()
{
    int i; /* sets a counter variable */

    for (i = 100; i < 1000; i++) /* iterates through all numbers 100-999 */
    {
        /* initializes the variables to hold the separated digits and cubes*/
        int first_digit = 0;
        int second_digit = 0;
        int third_digit = 0;
        int first_cube = 0;
        int second_cube = 0;
        int third_cube = 0;

        first_digit = i / 100; /* separates the first digit */
        third_digit = i % 10; /* separates the second digit */

        /* determines second digit from the previous two digits */
        second_digit = (i - first_digit * 100 - third_digit) / 10;

        /* cubes each digit */
        first_cube = first_digit * first_digit * first_digit;
        second_cube = second_digit * second_digit * second_digit;
        third_cube = third_digit * third_digit * third_digit;

        /* determines if number has cube property by comparing the sum of the
        cubed digits against the number's value */
        if (first_cube + second_cube + third_cube == i)
        {
            /* prints output when number has cube property */
            printf("%i has the cube property. (%i + %i + %i)\n",
                i, first_cube, second_cube, third_cube);
        }
    }
}

/******************************************************************************
   Function: print_pythagorean_triples

Description: Goes through a series of three digits ranging from any two inputted
             integers, determines whether the three digits make up a
             Pythagorean Triple by seeing if the sum of the squares of the first
             two digits equal the square of the second, then print out the
             digits and squares in a chart.

     Inputs: int low: sets the first number of iteration
             int high: sets the ceiling for the loop

    Outputs: The Triple count, the three digits that form a Pythagorean Triple,
             and the Pythagorean Equation with the squares of the three digits.
******************************************************************************/

void print_pythagorean_triples(int low, int high)
{
    int i, j, k; /* defines the counter variables */
    int tripleNUM = 1; /* initializes the variable tracking number of triples */

    /* goes through low - high for all three digits */
    for (i = low; i < high; i++)
    {
        for (j = low; j < high; j++)
        {
            for (k = low; k < high; k++)
            {
                if ((i < j) && (j < k))
                {
                    /* initializes variables storing squares of the digits */
                    int first_square = i * i;
                    int second_square = j * j;
                    int third_square = k * k;

                    /* Determines whether the digits make up a Triple */
                    if (first_square + second_square == third_square)
                    {
                        /* prints Triple's digits and squares in chart */
                        printf("Triple #%3i: %3i,%3i,%3i -->",
                            tripleNUM, i, j, k);
                        printf(" %4i + %4i = %4i\n",
                            first_square, second_square, third_square);
                        /* updates number of triples */
                        tripleNUM += 1;
                    }
                }
            }
        }
    }
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/29/24 ******/
void cubes()
{
    print_cubes();

    print_pythagorean_triples(1, 100);
}