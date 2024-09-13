/*******************************************************************************
filename    palindrome1.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    8
due date    2 November 2020

Brief Description: Determines if an array of characters is a palindrome based
                   off of array notation.

*******************************************************************************/

#include <stdio.h>
#include "assignments.h"

/******************************************************************************
   Function: is_palindrome1

Description: Determines if an array of characters is a palindrome based off of
             array notation.

     Inputs: phrase[] - array being analyzed
             length   - how large the array is

    Outputs: TRUE (1) or FALSE (0)
******************************************************************************/
int is_palindrome1(const char phrase[], int length)
{
    int i = 0;            /*loop counter                                  */
    int end = length - 1; /*calculates the right most element of the array*/
    int result = 1;           /*stores the function calculations              */

    /*returns TRUE (1) if array is only one character long*/
    if (length == 1)
        return 1;

    /*iterates through half the array's length*/
    while (i < end)
    {
        /*compares the outermost elements to each other*/
        if (phrase[i] == phrase[end])
            /*sets result to TRUE (1) and iterates through loop again*/
            result = 1;
        else
        {
            /*sets results to FALSE (0) breaks out of loop*/
            result = 0;

            break;
        }

        /*adjusts counters to converge on the middle of the array*/
        i++;
        end--;
    }

    /*returns either TRUE (1) or FALSE (0)*/
    return result;

}

/*******************************************************************************
filename    palindrome2.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    8
due date    2 November 2020

Brief Description: Determines if an array of characters is a palindrome based
                   off of pointer notation.

*******************************************************************************/

/******************************************************************************
   Function: is_palindrome2

Description: Determines if an array of characters is a palindrome based off of
             pointer notation.

     Inputs: *phrase  - array being analyzed
             length   - how large the array is

    Outputs: TRUE (1) or FALSE (0)
******************************************************************************/
int is_palindrome2(const char* phrase, int length)
{
    int i = 0;            /*loop counter                                  */
    int end = length - 1; /*calculates the right most element of the array*/
    int result = 1;           /*stores the function calculations              */

    /*returns TRUE (1) if array is only one character long*/
    if (length == 1)
        return 1;

    /*iterates through half the array's length*/
    while (i < end)
    {
        /*compares the outermost elements to each other*/
        if (*(phrase + i) == *(phrase + end))
            /*sets result to TRUE (1) and iterates through loop again*/
            result = 1;
        else
        {
            /*sets results to FALSE (0) breaks out of loop*/
            result = 0;

            break;
        }

        /*adjusts counters to converge on the middle of the array*/
        i++;
        end--;
    }

    /*returns either TRUE (1) or FALSE (0)*/
    return result;

}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/30/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
// Original author: Artie Fufkin
/* print all elements of an array */
//void print_array(const char a[], int size)
//{
//    int i;
//    for (i = 0; i < size; i++)
//        printf("%c", a[i]);
//}

void test_palindrome(const char sentence[], int length, int which_one)
{
    int pal;
    if (which_one == 1)
    {
        printf("Testing #1: ");
        pal = is_palindrome1(sentence, length);
    }
    else
    {
        printf("Testing #2: ");
        pal = is_palindrome2(sentence, length);
    }

    print_array(sentence, length);
    if (pal == 1)
        printf(" is a palindrome\n");
    else
        printf(" is not a palindrome\n");
}

void palindrome_test1(void)
{
    const char a[] = { 'a' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test2(void)
{
    const char a[] = { 'a', 'b' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test3(void)
{
    const char a[] = { 'a', 'a' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test4(void)
{
    const char a[] = { 'a', 'b', 'c', 'd', 'c', 'b', 'a' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}


void palindrome_test5(void)
{
    const char a[] = { 'm', 'a', 'd', 'a', 'm', 'I', 'm', 'a', 'd', 'a', 'm' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test6(void)
{
    const char a[] = { 'm', 'a', 'd', 'a', 'm', 'I', 'a', 'm', 'a', 'd', 'a', 'm' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test7(void)
{
    const char a[] = { 'a', 'B', 'c', 'D', 'E', 'D', 'c', 'B', 'a' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test8(void)
{
    const char a[] = { 'a', 'B', 'c', 'D', 'E', 'E', 'D', 'c', 'B', 'a' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test9(void)
{
    const char a[] = { 'a', 'B', 'c', 'D', 'E', 'e', 'D', 'c', 'B', 'a' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test10(void)
{
    const char a[] = { 'h', 'e', 'l', 'i', 'v', 'e', 'd', 'a', 's', 'a',
                      'd', 'e', 'v', 'i', 'l', 'e', 'h' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test11(void)
{
    const char a[] = { 'b', 'o', 't', 't', 'o', 'm' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome_test12(void)
{
    const char a[] = { '1', '2', '3', '2', '1' };
    test_palindrome(a, sizeof(a), 1);
    test_palindrome(a, sizeof(a), 2);
    printf("\n");
}

void palindrome(void)
{
    palindrome_test1();
    palindrome_test2();
    palindrome_test3();
    palindrome_test4();
    palindrome_test5();
    palindrome_test6();
    palindrome_test7();
    palindrome_test8();
    palindrome_test9();
    palindrome_test10();
    palindrome_test11();
    palindrome_test12();
}