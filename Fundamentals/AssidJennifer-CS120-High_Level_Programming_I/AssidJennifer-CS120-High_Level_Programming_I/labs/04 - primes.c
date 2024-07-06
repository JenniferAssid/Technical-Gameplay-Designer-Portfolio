/***************************************
filename    prime1.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    6
due date    10/15/2020

Brief Description: Determining is a number is prime as well as displaying that
                   all even numbers within a range are the sum of two prime #s.

***************************************/

#include <stdio.h> /* printf */
#include "labs.h"

/******************************************************************************
   Function: is_prime

Description: determines whether a number is prime

     Inputs: int number - number that is being evaluated
    Outputs: True / False determined by whether the number is prime or not.
******************************************************************************/
int is_prime(int number)
{
    /*defines TRUE and FALSE values*/
    int TRUE = 1;
    int FALSE = 0;
    /*initializes counter variable*/
    int prime_counter;

    /*determines whether the number is 1 or 2 out outputs necessary output*/
    if (number == 1)
    {
        return FALSE;
    }

    if (number == 2)
    {
        return TRUE;
    }

    /*determines whether number is even and outputs necessary output*/
    if (number % 2 == 0)
    {
        return FALSE;
    }

    /*goes through all values between 3 and half of the number, divides the
    number by them, determines whether quotient is 0 or not and outputs
    necessary output*/
    for (prime_counter = 3; prime_counter < number * 0.5f; prime_counter++)
    {
        if (number % prime_counter == 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************
   Function: conjecture

Description: shows that every even number within a range is the sum of two
             prime numbers

     Inputs: int low - the lower bound of the range
             int high - the upper bound of the range
    Outputs: A printed out table showing every even within the range and the
             addition equation with the two prime numbers that prove the theory.
******************************************************************************/
void conjecture(int low, int high)
{
    /*initializes the three counters for the three nested for loops*/
    int range_counter, firstprime_counter, secondprime_counter;

    /*initializes the stop loop variable*/
    int finish_loop;

    /*goes through the range specified by inputs*/
    for (range_counter = low; range_counter <= high; range_counter++)
    {
        finish_loop = 0; /*defines the stop loop variable*/
        if (range_counter % 2 == 0) /*checks if value is even*/
        {
            for (firstprime_counter = 2;         /*goes through the values   */
                firstprime_counter < range_counter; /*between 2 and range to see*/
                firstprime_counter++)               /*if there are prime numbers*/
            {
                if (is_prime(firstprime_counter) == 1) /*checks if counter is*/
                {                                     /*prime               */
                    for (secondprime_counter = 2;
                        secondprime_counter < range_counter;
                        secondprime_counter++)
                    {
                        if (is_prime(secondprime_counter) == 1) /*check prime*/
                        {
                            if (secondprime_counter + firstprime_counter
                                == range_counter) /*checks if sum is equal to*/
                            {                    /* original value          */
                                printf("%3i = %3i + %3i\n",
                                    range_counter,
                                    firstprime_counter,
                                    secondprime_counter);
                                finish_loop = 1;
                            }
                        }

                        if (finish_loop == 1)
                            break;

                    }
                    if (finish_loop == 1)
                        break;
                }
                if (finish_loop == 1)
                    break;
            }

        }
    }
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/29 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE

void check_primes(int count)
{
    int primes = 0; /* number of primes found */
    int i;          /* loop counter           */

    /* Check all values from 1 to count */
    for (i = 1; i <= count; i++)
    {
        /* If it's prime, count it and print it */
        if (is_prime(i))
        {
            primes++;
            printf("#%3i: %3i is prime.\n", primes, i);
        }
    }
}

void primes(void)
{
    check_primes(500);
    conjecture(2, 200);
    return 0;
}