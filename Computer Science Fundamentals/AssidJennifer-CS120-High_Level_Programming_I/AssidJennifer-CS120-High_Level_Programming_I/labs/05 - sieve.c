/***************************************
filename    sieve.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    7
due date    10/29/2020

Brief Description: holds functions that
                   do different things
                   with the same array
                   and range of numbers.

***************************************/

#include <stdio.h> /* printf */
#include <math.h>  /*square root*/

#include "labs.h"

#define FALSE 0
#define TRUE 1
#define SIZE 1001

/******************************************************************************
   Function: sieve

Description: Determines which values between 0 and N are primes and returns an
             array of 1s (primes) and 0s (not primes).

     Inputs: int array[] - array that is going to store the values
             int size - size of the array
    Outputs: Array with 1 (TRUE) for each prime within the range and 0 (FALSE)
             for non-primes
******************************************************************************/
void sieve(int array[], int size)
{
    int i, j;

    array[0] = FALSE; /* sets values 0 and 1 of the array to 0*/
    array[1] = FALSE; /* in keeping with the rules of primes*/

    for (i = 2; i <= size; i++)
    {
        array[i] = TRUE; /*initializes every index to TRUE (1)*/
    }

    /*goes through all values from 2 to square root of size for base factors*/
    for (i = 2; i <= sqrt(size); i++)
    {
        /*goes through all values from base factor to size*/
        for (j = i + 1; j <= size; j++)
        {
            /*checks if second value is a multiple of the base factor*/
            if (j % i == FALSE)
            {
                /*sets index to FALSE*/
                array[j] = FALSE;
            }
        }
    }
}

/******************************************************************************
   Function: twin_primes

Description: Determines the twin primes within a range.

     Inputs: const int primes[] - array that is being reviewed
             int size - how large the array is
    Outputs: The twin primes within a range
******************************************************************************/
int twin_primes(const int primes[], int size)
{
    /*initializes the count variable and loop variable*/
    int twin_count = 0;
    int i;

    /*goes through values from 3 to size to keep conventions*/
    for (i = 3; i <= size; i++)
    {
        /*check if initial value and it's possible twin are actually twins*/
        if (primes[i] == TRUE && primes[i + 2] == TRUE)
        {
            /*increments twin count and prints the twin primes*/
            twin_count++;
            printf("twin prime #%4i: %4i and %4i\n", twin_count, i, i + 2);
        }
    }
    return twin_count; /*returns the final twin count*/
}

/******************************************************************************
   Function: brun_constant

Description: Determines Brun's Constant of the twin primes within a range.

     Inputs: const int primes[] - array that is being reviewed
             int size - how large the array is
    Outputs: The Brun's Constant of a range
******************************************************************************/
double brun_constant(const int primes[], int size)
{
    /*initializes Brun's Constant and looper variables*/
    double brunConst = 0.0;
    int i;

    /*goes through values from 3 to size to keep conventions*/
    for (i = 3; i <= size; i++)
    {
        /*check if initial value and it's possible twin are actually twins*/
        if (primes[i] == TRUE && primes[i + 2] == TRUE)
        {
            /*calculates the Brun's Constant*/
            brunConst = brunConst + ((1.0 / i) + (1.0 / (i + 2)));
        }
    }

    return brunConst; /*returns Brun's Constant*/
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/29/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
#define SIZE 1001   /* The size of the array        */

void sieve_cum(void)
{
    int array[SIZE] = { 0 }; /* The array of 0's and 1's     */
    int count = 0;         /* Number of primes/twins found */
    double bruns;          /* Brun's Constant              */
    int i;                 /* Loop counter                 */

    /* Mark prime/non-prime numbers using the array */
    sieve(array, SIZE - 1);

    /* Count the number of primes and print each one */
    for (i = 0; i < SIZE; i++)
    {
        /* If TRUE, this marks a prime number */
        if (array[i] == TRUE)
        {
            count++;
#if 1
            printf("Prime #%04i:  %4i\n", count, i);
#endif
        }
    }

    /* Print the total number of primes that were found */
    printf("%i primes found between 0 and %i.\n", count, SIZE - 1);

    /* Find the number of twin primes and print it */
    count = twin_primes(array, SIZE - 1);
    printf("%i twin primes found between 0 and %i.\n", count, SIZE - 1);

    /* Get Brun's Constant and print it */
    bruns = brun_constant(array, SIZE - 1);
    printf("Brun's constant with %i twin primes is %1.12f.\n", count, bruns);

    return 0;
}