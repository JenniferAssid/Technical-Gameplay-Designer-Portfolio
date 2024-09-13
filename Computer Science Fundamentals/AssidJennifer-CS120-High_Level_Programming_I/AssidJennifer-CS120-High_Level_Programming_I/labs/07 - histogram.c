/***************************************
filename    histogram.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    9
due date    18 November 2020

Brief Description: holds the functions GetCounts(), PrintHistograms(),
                   GetStatistics() which all analyze and derive statistics such
                   as a histogram, average, and mode of a given string.

***************************************/

#include <stdio.h>     /* printf      */
#include <ctype.h>     /* toupper     */
#include <string.h>    /* strlen      */
#include "labs.h"


#define NUM_LETTERS 26
#define SPACE ' '

/******************************************************************************
   Function: GetCounts

Description: Counts how many letters, spaces, and other characters are in a
             sentence

     Inputs: *sentence - string being analyzed
             letters[] - array to store occurrences of letters
               *spaces - running total of spaces
                *other - running total of other characters

    Outputs: updated running total for spaces and other characters as well as
             a total of letters within the given string
******************************************************************************/
int GetCounts(const char* sentence, int letters[], int* spaces, int* other)
{
    int i;                /* loop counter                                 */
    int tempChar;         /* temporary holding variable                   */
    int totalLetters = 0; /* running total of amount of letters in string */
    *spaces = 0;          /* setting the running total of spaces to 0     */
    *other = 0;           /* setting the running total of other to 0      */

    /* iterates through array letters[] */
    for (i = 0; i < NUM_LETTERS; i++)
        /* setting all indexes to 0 */
        letters[i] = 0;

    /* iterates through the given string */
    for (i = 0; i < (int)strlen(sentence); i++)
    {
        /* converts current character into an uppercase letter when necessary*/
        tempChar = toupper(*(sentence + i));

        /* checks if current character is a space with space's ASCII variable*/
        if (tempChar == SPACE)
        {
            /* increments the running total for spaces */
            (*spaces)++;

            /* forces for loop to end current iteration and move on to the next*/
            continue;
        }

        /* checks if current character is the ASCII value for an uppercase letter*/
        if (tempChar >= 65 && tempChar <= 90)
        {
            /* increments the corresponding index in letters[] for the character */
            letters[tempChar - 65]++;

            /* forces for loop to end current iteration and move on to the next*/
            continue;
        }

        /* increments other total if current character is not a space or letter */
        (*other)++;
    }

    /* iterates through array letters[] */
    for (i = 0; i < NUM_LETTERS; i++)
        /* updates running sum of letter occurrences */
        totalLetters += letters[i];

    /* returns the final sum */
    return totalLetters;
}

/******************************************************************************
   Function: PrintHistograms

Description: Prints out a histogram of letter occurrences for a given string

     Inputs: letters[] - array being printed out

    Outputs: printed out histogram of letter occurrences within a given string
******************************************************************************/
void PrintHistogram(const int letters[])
{
    int i; int j; /* loop counters */

    /* iterates through array letters[] */
    for (i = 0; i < NUM_LETTERS; i++)
    {
        /* prints corresponding character from given ASCII value */
        printf("%c:", i + 65);

        /* iterates through through the index amount of letters[i] */
        for (j = 0; j <= letters[i]; j++)
        {
            /* checks if iteration is at final value */
            if (j == letters[i])
                /* prints new line */
                printf("\n");
            else
                /* prints * for each letter occurrence */
                printf("*");
        }
    }
}

/******************************************************************************
   Function: GetStatistics

Description: Evaluates a histogram and calculates average, mode, and amount
             associated with that mode.

     Inputs:   letters[] - array holding values for histogram
                *average - calculation of histogram average
             *max_letter - letter with the most occurrences (mode)
              *max_count - highest occurrence of individual letter

    Outputs: The average, letter with the most occurrences, and the value of
             letter occurrences of a given histogram
******************************************************************************/
void GetStatistics(const int letters[], double* average, char* max_letter,
    int* max_count)
{
    int i;             /* loop counter                 */
    *average = 0;      /* initialization of average    */
    *max_count = 0;    /* initialization of max_count  */
    *max_letter = 'A'; /* initialization of max_letter */

    /* iterates through array letters[] */
    for (i = 0; i < NUM_LETTERS; i++)
    {
        /* adds all letter occurrences and their associated values to average*/
        *average += (i + 1) * letters[i];

        /* checks if current index value is higher than current max_count */
        if (letters[i] > *max_count)
        {
            /* adjusts max_count to current index value */
            *max_count = letters[i];

            /* updates max_letter to the corresponding letter of index */
            *max_letter = i + 65;
        }
    }

    /* calculates the average by dividing it by NUM_LETTERS */
    *average = *average / NUM_LETTERS;
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/29/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
#include <stdio.h>  /* printf         */
#include <stdlib.h> /* atoi           */
#include <string.h> /* strcpy, strcat */


const char s0[] = "Real simple.";

const char s1[] = "This is simple - a sentence with spaces - and other "
"punctuations.";

const char s2[] =
"When in the Course of human events, it becomes necessary for one people to "
"dissolve the political bands which have connected them with another, and to "
"assume among the powers of the earth, the separate and equal station to "
"which the Laws of Nature and of Nature's God entitle them, a decent respect "
"to the opinions of mankind requires that they should declare the causes "
"which impel them to the separation.";

const char s3a[] =
"We hold these truths to be self-evident, that all men are created equal, "
"that they are endowed by their Creator with certain unalienable Rights, "
"that among these are Life, Liberty and the pursuit of Happiness. --That "
"to secure these rights, Governments are instituted among Men, deriving "
"their just powers from the consent of the governed, --That whenever any "
"Form of Government becomes destructive of these ends, it is the Right of "
"the People to alter or to abolish it, and to institute new Government, ";

const char s3b[] =
"laying its foundation on such principles and organizing its powers in "
"such form, as to them shall seem most likely to effect their Safety and "
"Happiness. Prudence, indeed, will dictate that Governments long "
"established should not be changed for light and transient causes; and "
"accordingly all experience hath shewn, that mankind are more disposed to "
"suffer, while evils are sufferable, than to right themselves by "
"abolishing the forms to which they are accustomed. But when a long train ";

const char s3c[] =
"of abuses and usurpations, pursuing invariably the same Object evinces a "
"design to reduce them under absolute Despotism, it is their right, it is "
"their duty, to throw off such Government, and to provide new Guards for "
"their future security.--Such has been the patient sufferance of these "
"Colonies; and such is now the necessity which constrains them to alter "
"their former Systems of Government. The history of the present King of "
"Great Britain is a history of repeated injuries and usurpations, all ";

const char s3d[] =
"having in direct object the establishment of an absolute Tyranny over "
"these States. To prove this, let Facts be submitted to a candid world.";

const char s4[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
"1234567890!@#$%^&*()<>?:\"{}_+ ssTT";

void GetAndPrintStatistics(const int letters[])
{
    int max;
    double average;
    char letter;

    GetStatistics(letters, &average, &letter, &max);
    printf("Average: %.5f, max letter: %c, max count: %i\n",
        average, letter, max);
}

void DoTest(const char* string)
{
    int spaces, other;        /* These will be filled in by GetCounts     */
    int letters[NUM_LETTERS]; /* This will also be filled in by GetCounts */
    int count;                /* Number of letters in the sentence        */

    count = GetCounts(string, letters, &spaces, &other);
    printf("Count of letters is: %i, spaces: %i, other: %i\n",
        count, spaces, other);

    PrintHistogram(letters);
    GetAndPrintStatistics(letters);
    printf("\n\n");
}

void histogram(void)
{
    char s3[2000];
    int test = -1;

    /* Build a larger string */
    strcpy(s3, s3a);
    strcat(s3, s3b);
    strcat(s3, s3c);
    strcat(s3, s3d);

    DoTest(s0);
    DoTest(s1);
    DoTest(s2);
    DoTest(s3);
    DoTest(s4);
}