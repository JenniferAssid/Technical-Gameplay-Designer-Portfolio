/*******************************************************************************
filename    bytes.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    8
due date    16 November 2020

Brief Description: Includes functions that analyze and manipulate strings.

*******************************************************************************/

#include <stddef.h> /* NULL */

#include "assignments.h"

/******************************************************************************
   Function: *find_byte

Description: finds the first occurrence of a constant value between two values
             given by pointers

     Inputs: *from - lower-bound of range
             *to   - upper-bound of range
             byte  - value that is being compared against
    Outputs:
******************************************************************************/
const char* find_byte(const char* from, const char* to, char byte)
{
    /*iterates through the range*/
    for (from = from; from <= to; from++)
    {
        /*compares the current value to byte*/
        if (*from == byte)
            /*returns the current value if it is equal to byte*/
            return from;
    }

    /*returns NULL if byte is not found within range*/
    return NULL;
}

/******************************************************************************
   Function: *find_any_bytes

Description: finds the first occurrence of a value within a list between two
             values given by pointers

     Inputs: *from  - lower-bound of range
             *to    - upper-bound of range
             *bytes - array of values (characters)
             count  - length of the array
    Outputs:
******************************************************************************/
const char* find_any_byte(const char* from, const char* to, const char* bytes,
    int count)
{
    int i; /*loop counter*/

    /*iterates through indexes in bytes array*/
    for (i = 0; i < count; i++)
    {
        /*determines if current value of bytes is equal to current value of string*/
        if (find_byte(from, to, *(bytes + i)))
            return find_byte(from, to, *(bytes + i));
    }

    /*returns NULL if none of the characters match*/
    return NULL;
}

/******************************************************************************
   Function: count_bytes

Description: counts the occurrence of a constant value between two values given
             by pointers

     Inputs: *from - lower-bound of range
             *to   - upper-bound of range
             byte  - value that is being compared against

    Outputs: pointer to the count of occurrences
******************************************************************************/
int count_bytes(const char* from, const char* to, char byte)
{
    int total = 0;        /*number of occurrences            */
    int* result = &total; /*pointer to number of occurrences*/

    /*iterates through the range*/
    for (from = from; from <= to; from++)
    {
        /*compares the current value to byte*/
        if (*from == byte)
            /*adds to count if values are equal*/
            total++;
    }

    /*returns the pointer to the count of occurences*/
    return *result;

}

/******************************************************************************
   Function: count_any_bytes

Description: counts the occurrence of a value within a list between two values
             given by pointers

     Inputs: *from  - lower-bound of range
             *to    - upper-bound of range
             *bytes - array of values (characters)
             count  - length of the array

    Outputs: pointer to the total occurrences of a value within a list between
             two values given by pointers
******************************************************************************/
int count_any_bytes(const char* from, const char* to, char* bytes, int count)
{
    int i;                /*loop counter                              */
    int total = 0;        /*total amount of occurrences               */
    int* result = &total; /*pointer to the total amount of occurrences*/

    /*iterates through indexes in bytes array*/
    for (i = 0; i < count; i++)
    {
        /*adds the amount from count_bytes to running total*/
        total += count_bytes(from, to, *(bytes + i));
    }

    /*returns NULL if none of the characters match*/
    return *result;
}

/******************************************************************************
   Function: compare_bytes

Description: compares the sizes of two strings (determined by two pointers
             and a count variable).

     Inputs: *location1 - location of string (pointer)
             *location2 - location of string (pointer)
             count      - length of strings

    Outputs: SAME (0); location1 less than location2 (-1); location1 greater
             than location2 (1)
******************************************************************************/
int compare_bytes(const char* location1, const char* location2, int count)
{
    int i;          /*loop counter*/

    /*iterates from 0 to count*/
    for (i = 0; i < count; i++)
    {
        /*returns -1 if element in location1 is less than the element in location2*/
        if (*(location1 + i) < *(location2 + i))
            return -1;

        /*returns 1 if element in location1 is greater than element in location2*/
        if (*(location1 + i) > *(location2 + i))
            return 1;
    }

    /*returns 0 if all elements in location1 and location2 are equal*/
    return 0;
}

/******************************************************************************
   Function: exchange_bytes

Description: switches the elements in a range determined by two pointers and a
             length variable

     Inputs: *p1    - location within string (pointer)
             *p2    - location within string (pointer)
             length - size of range

    Outputs: the string holding the switch elements
******************************************************************************/
void exchange_bytes(char* p1, char* p2, int length)
{
    int i;        /*loop counter                          */
    int temp = 0; /*variable to temporarily hold an elemnt*/

    /*iterates through the range*/
    for (i = 0; i < length; i++)
    {
        /*places the element in location1 into temp*/
        temp = *(p1 + i);

        /*places the element in location2 into corresponding element in location1*/
        *(p1 + i) = *(p2 + i);

        /*places the element in temp (location1's initial element) in location2*/
        *(p2 + i) = temp;
    }
}

/******************************************************************************
   Function: copy_bytes

Description: copies values in a string determined by length from one pointer to
             another pointer

     Inputs: *from  - lower-bound of range
             *to    - upper-bound of range
             length - length of string
    Outputs:
******************************************************************************/
void copy_bytes(char* from, char* to, int length)
{
    int i; /*loop counter*/

    /*determines with pointer is larger to determine direction of switching*/
    if (*from < *to)
    {
        /*iterates through the length*/
        for (i = 1; i <= length; i++)
        {
            /*copies the values backwards*/
            *(to + length - i) = *(from + length - i);
        }
    }
    else
    {
        /*iterates through the length*/
        for (i = 1; i <= length; i++)
        {
            /*copies the values forwards*/
            *(from + length - i) = *(to + length - i);
        }
    }

}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/30/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
#include <stdio.h>  /* printf              */
#include <string.h> /* strlen              */
#include <stdlib.h> /* atoi                */
#include <time.h>   /* time                */
#include "PRNG.h"   /* RandomInt, ut_srand */

void bytes_test1(void)
{
    const char* string = "abcdefghijklmnopqrstuvwxyz";
    int length = strlen(string);
    const char* bytes = "aimzX1";
    const char* found;

    printf("============ bytes_test1 ============\n");
    printf("01234567890123456789012345\n");
    printf("%s\n", string);

    while (*bytes)
    {
        found = find_byte(string, string + length, *bytes);
        if (found)
            printf("Found '%c' at offset %li\n", *bytes, found - string);
        else
            printf("Didn't find byte: %c\n", *bytes);

        bytes++;
    }

    found = find_byte(string + 10, string + 20, 'z');
    if (found)
        printf("Found z at offset %li\n", found - string);
    else
        printf("Didn't find z\n");

    found = find_byte(string + 10, string + 20, 'p');
    if (found)
        printf("Found p at offset %li\n", found - string);
    else
        printf("Didn't find p\n");

    found = find_byte(string, string, 'a');
    if (found)
        printf("Found a at offset %li\n", found - string);
    else
        printf("Didn't find a\n");

    found = find_byte(string, string, 'b');
    if (found)
        printf("Found b at offset %li\n", found - string);
    else
        printf("Didn't find b\n");
}

void bytes_test2(void)
{
    const char* string = "There's a fine line between clever and stupid.";
    int length = strlen(string);
    const char* bytes = "gjkqxyz";
    const char* found;

    printf("============ bytes_test2 ============\n");
    printf("012345678901234567890123456789012345678901234567890\n");
    printf("%s\n", string);

    found = find_any_byte(string, string + length, bytes, strlen(bytes));
    if (found)
        printf("Found '%c' at offset %li\n", *found, found - string);
    else
        printf("Didn't find any bytes: %s\n", bytes);

    bytes = "gjkqxybz";
    found = find_any_byte(string, string + length, bytes, strlen(bytes));
    if (found)
        printf("Found '%c' at offset %li\n", *found, found - string);
    else
        printf("Didn't find any bytes: %s\n", bytes);

}

void bytes_test3(void)
{
    const char* string = "There's a fine line between clever and stupid.";
    int length = strlen(string);
    const char* bytes = "aeiou";

    printf("============ bytes_test3 ============\n");
    printf("012345678901234567890123456789012345678901234567890\n");
    printf("%s\n", string);

    while (*bytes)
    {
        int count = count_bytes(string, string + length, *bytes);
        printf("Found %i occurrences of '%c'\n", count, *bytes);
        bytes++;
    }
}

void bytes_test4(void)
{
    const char* string = "There's a fine line between clever and stupid.";
    int length = strlen(string);
    const char* bytes = "aeiou";
    int count;

    printf("============ bytes_test4 ============\n");
    printf("012345678901234567890123456789012345678901234567890\n");
    printf("%s\n", string);

    count = count_any_bytes(string, string + length, bytes, strlen(bytes));
    printf("Found %i occurrences from the set '%s'\n", count, bytes);
}

void scanner(const char* loc1, const char* loc2, int count)
{
    int result;
    int i;
    const char* bytes;

    bytes = loc1;
    for (i = 0; i < count; i++)
        printf("%c", *bytes++);
    printf("\n");

    bytes = loc2;
    for (i = 0; i < count; i++)
        printf("%c", *bytes++);
    printf("\n");

    result = compare_bytes(loc1, loc2, count);

    if (result == 0)
        printf("The bytes are the same.\n");
    else if (result < 0)
        printf("The first bytes are less than the second bytes.\n");
    else
        printf("The first bytes are greater than the second bytes.\n");
}

void bytes_test5(void)
{
    /*                    0123456789012345678901234567890 */
    const char* bytes1 = "abccefghijklmnopqrstuvwxyzxxxxx";
    const char* bytes2 = "abcdefxxxxxghijklpqwxyzabcdefxx";

    printf("============ bytes_test5 ============\n");

    scanner(bytes1, bytes2, 3);
    scanner(bytes1, bytes2, 4);
    scanner(bytes1 + 12, bytes2 + 11, 6);
    scanner(bytes1 + 22, bytes2 + 14, 4);
    scanner(bytes1 + 10, bytes2 + 15, 1);
    scanner(bytes1 + 6, bytes2 + 11, 6);

    while (*bytes1 && *bytes2)
        scanner(bytes1++, bytes2++, 1);
}

void bytes_test6(void)
{
    /*               0123456789.123456789.123456789.  */
    /*                         1         2            */
    char string[] = "Four score and seven years ago.";

    printf("============ bytes_test6 ============\n");

    printf("BEFORE: %s\n", string);
    exchange_bytes(string + 5, string + 15, 5);
    printf(" AFTER: %s\n", string);
    exchange_bytes(string + 0, string + 27, 4);
    printf(" AFTER: %s\n", string);
    exchange_bytes(string + 7, string + 19, 9);
    printf(" AFTER: %s\n", string);
}

void bytes_test7(void)
{
    char string[] = "Any sufficiently advanced technology is indistinguishable from magic.";
    int length = strlen(string);
    int maxsize = length / 3;
    int count = 100;
    int i;

#define MAX_EXCHANGES 1000
    int sizes[MAX_EXCHANGES];
    int loffsets[MAX_EXCHANGES];
    int roffsets[MAX_EXCHANGES];

    printf("============ bytes_test7 ============\n");
    printf("%s\n", string);
    for (i = 0; i < count; i++)
    {
        int size = RandomInt(1, maxsize);
        int loffset = RandomInt(0, length - 2 * maxsize);

        int roffset = RandomInt(loffset + size, length - maxsize);
        exchange_bytes(string + loffset, string + roffset, size);
        printf("%s\n", string);
        sizes[i] = size;
        loffsets[i] = loffset;
        roffsets[i] = roffset;
    }

    printf("*********************************************************************\n");

    for (i = count - 1; i >= 0; i--)
    {
        exchange_bytes(string + loffsets[i], string + roffsets[i], sizes[i]);
        printf("%s\n", string);
    }
}

void bytes_test8(void)
{
    /*               01234567890123456789012345 */
    char string1[] = "abcdefghijklmnopqrstuvwxyz";

    char string2[] =
        /*           1         2         3         4         5         6        */
        /* 012345678901234567890123456789012345678901234567890123456789012345678*/
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    printf("============ bytes_test8 ============\n");

    printf("01234567890123456789012345\n");
    printf("%s\n", string1);

    copy_bytes(string1 + 6, string1 + 11, 10);
    printf("%s\n", string1);

    copy_bytes(string1 + 2, string1 + 5, 15);
    printf("%s\n", string1);


    printf("          1         2         3         4         5         6        \n");
    printf("012345678901234567890123456789012345678901234567890123456789012345678\n");
    printf("%s\n", string2);

    copy_bytes(string2 + 6, string2 + 11, 9);
    printf("%s\n", string2);

    copy_bytes(string2 + 3, string2 + 19, 30);
    printf("%s\n", string2);

    copy_bytes(string2 + 30, string2 + 35, 22);
    printf("%s\n", string2);

    copy_bytes(string2 + 1, string2 + 2, 3);
    printf("%s\n", string2);
}

void bytes_testX(void)
{
    char string1[] = "There's a fine line between clever and stupid.";
    const char* string2 = "abcdefghijklm";
    const char* string3 = "abcdehijklmno";
    int length = strlen(string1);
    const char* found;
    const char* bytes;
    int count;

    /* Finds 'b' in the range 'fine line between' and returns a pointer to it */
    found = find_byte(string1 + 10, string1 + 26, 'b');
    printf("Found %c at offset %li\n", *found, found - string1);

    /* Doesn't find 'z' in the range 'There's a fi' and returns NULL */
    found = find_byte(string1, string1 + 11, 'z');

    /*
     * Finds the 'l' in the entire range of characters and returns pointer to it.
     * It first looks for x, then y, then z, and then l. It stops after finding
     * the l, and doesn't look for a, b, or c.
     */
    bytes = "xyzlabc";
    found = find_any_byte(string1, string1 + length, bytes, 7);
    printf("Found %c at offset %li\n", *found, found - string1);

    /* Returns 3, the number of time 'e' is in range 'There's a fine '  */
    count = count_bytes(string1, string1 + 15, 'e');
    printf("count is %i\n", count);

    /* Returns 15, the number of times a vowel appears in the entire string  */
    bytes = "aeiou";
    count = count_any_bytes(string1, string1 + length, bytes, 5);
    printf("count is %i\n", count);

    /* Returns 0 as the ranges 'abcde' and 'abcde' are the same */
    count = compare_bytes(string2, string3, 5);
    printf("count is %i\n", count);

    /* Returns negative number as 'abcdef' is less than 'abcdeh' */
    count = compare_bytes(string2, string3, 6);
    printf("count is %i\n", count);

    /* Returns positive number as 'bcd' is greater than 'abc' */
    count = compare_bytes(string2 + 1, string3, 3);
    printf("count is %i\n", count);

    /* Returns 0 as the ranges 'jklm' and 'jklm' are the same */
    count = compare_bytes(string2 + 9, string3 + 7, 4);
    printf("count is %i\n", count);

    /* Changes string to "There's a line fine between clever and stupid." */
    exchange_bytes(string1 + 10, string1 + 15, 4);
    printf("%s\n", string1);

    /* Changes string to "There's a line fine between stupid and clever." */
    exchange_bytes(string1 + 28, string1 + 39, 6);
    printf("%s\n", string1);

    /* Changes string to "There's a line line fine between stand clever." */
    copy_bytes(string1 + 10, string1 + 15, 20);
    printf("%s\n", string1);
}

void bytes(void)
{
    bytes_test1();
    bytes_test2();
    bytes_test3();
    bytes_test4();
    bytes_test5();
    bytes_test6();
    bytes_test7();
    bytes_test8();
    bytes_testX();
}