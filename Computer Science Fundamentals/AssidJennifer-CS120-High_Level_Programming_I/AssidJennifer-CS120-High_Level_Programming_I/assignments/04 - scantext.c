/***************************************
filename    scantext.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    11
due date    3 December 2020

Brief Description: analyzes and manipulate given strings based on input

***************************************/

#include "assignments.h"

/******************************************************************************
   Function: mystrlenStudent

Description: calculates the length of given string

     Inputs:  *string - string to analyze

    Outputs: length of string as an integer
******************************************************************************/
int mystrlenStudent(const char* string)
{
    int i;       /* loop counter    */
    int len = 0; /* length variable */

    /* iterates through given string */
    for (i = 0; string[i] != 0; i++)
    {
        /* increments length */
        len++;
    }

    /* return length */
    return len;
}

/******************************************************************************
   Function: count_tabs

Description: counts how many tabs there are in a given string

     Inputs: *string - pointer to given string

    Outputs: number of tabs are an integer value
******************************************************************************/
int count_tabs(const char* string)
{
    int i;       /* loop counter   */
    int total = 0; /* total variable */

    /* iterates through the string */
    for (i = 0; i < mystrlenStudent(string); i++)
    {
        /* checks if element in string is a tab*/
        if (*(string + i) == 9)
            /* increments total */
            total++;
    }

    /* returns total */
    return total;
}

/******************************************************************************
   Function: substitute_char

Description: changes out any occurrence of old_char and replaces it with
             new_char in given string

     Inputs:  *string - string to modify
             old_char - designated character to be replaced
             new_char - designated character to be used to replace

    Outputs: number of adjustments as an integer
******************************************************************************/
int substitute_char(char* string, char old_char, char new_char)
{
    int i;         /* loop counter   */
    int total = 0; /* total variable */

    /* iterates through the string */
    for (i = 0; i < mystrlenStudent(string); i++)
    {
        /* if current element in string is old_char*/
        if (*(string + i) == old_char)
        {
            /* replace with new_char*/
            *(string + i) = new_char;

            /* increment the total*/
            total++;
        }
    }

    /* return the total */
    return total;
}

/******************************************************************************
   Function: calculate_lengths

Description: calculates the initial length of a given string and the adjusted
             length based off of changes to possible tabs.

     Inputs:         *string - string to analyze
                     tabsize - value applied to modify any tabs in string
              *string_length - initial length of string
             *display_length - length of string with modified tabs

    Outputs: N/A
******************************************************************************/
void calculate_lengths(const char* string, int tabsize, int* string_length, int* display_length)
{
    int tabCount = 0; /* running total */

    /* calculates initial length of string */
    *string_length = mystrlenStudent(string);

    /* calculates how many tabs are present in string */
    tabCount = count_tabs(string);

    /* calculates modified length by multiplying the tabs by tabsize */
    *display_length = (*string_length - tabCount) + (tabCount * tabsize);
}

/******************************************************************************
   Function: count_words

Description: counts how many words are in a sentence based off of whitespace

     Inputs: *string - string to analyze

    Outputs: number of words in sentence as an integer
******************************************************************************/
int count_words(const char* string)
{
    int i;                   /* loop counter       */
    char lastChar = *string; /* previous character */
    int total = 0;           /* running total      */

    /* iterates through the string */
    for (i = 1; i < mystrlenStudent(string); i++)
    {
        /* checks if last character was something other than whitespace */
        if (lastChar != '\n' &&
            lastChar != '\t' &&
            lastChar != ' ')
        {
            /* checks if current character is whitespace */
            if (*(string + i) == '\n' ||
                *(string + i) == '\t' ||
                *(string + i) == ' ')
                /* increments total */
                total++;
            /* checks if the end of the line is reached */
            else if (i == (mystrlenStudent(string) - 1))
                /* adds to word total when line doesn't end in whitespace*/
                total++;
        }

        /* updates previous character variable */
        lastChar = *(string + i);
    }

    /* returns total */
    return total;
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/30/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
// Original author: Artie Fufkin
#include <stdio.h>    /* printf */

int mystrlen(const char* string)
{
    int length = 0;
    while (*string++)
        length++;

    printf("This should not be called.\n");
    return length;
}

static void scantext_test1(void)
{
    char string[] = "This\tstring\thas\tfive\twords.";
    int tabs;

    printf("\n== scantext_test1 ==========================================================\n");
    printf("|%s|\n", string);
    tabs = count_tabs(string);
    printf("Number of tabs is %i.\n", tabs);
}

static void scantext_test2(void)
{
    char string[] = "This\tstring\thas\tfive\twords.";
    int char_count;
    int print_length;
    int tabsize = 2;

    calculate_lengths(string, tabsize, &char_count, &print_length);

    printf("\n== scantext_test2 ==========================================================\n");
    printf("|%s|\n", string);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);

    tabsize = 3;
    calculate_lengths(string, tabsize, &char_count, &print_length);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);

    tabsize = 6;
    calculate_lengths(string, tabsize, &char_count, &print_length);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);

    tabsize = 8;
    calculate_lengths(string, tabsize, &char_count, &print_length);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);
}

static void scantext_test3(void)
{
    char string[] = "This\tstring\thas\tfive\twords.";
    int count;
    int tabs;
    int tabsize;
    int char_count;
    int print_length;

    printf("\n== scantext_test3 ==========================================================\n");
    printf("|%s|\n", string);
    tabs = count_tabs(string);
    printf("Number of tabs is %i.\n", tabs);

    tabsize = 4;
    calculate_lengths(string, tabsize, &char_count, &print_length);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);

    count = substitute_char(string, '\t', ' ');
    printf("Replacements made: %i. After replacing tabs with spaces:\n", count);
    printf("|%s|\n", string);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("Number of tabs is %i\n", count_tabs(string));

    tabsize = 4;
    calculate_lengths(string, tabsize, &char_count, &print_length);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);
}

static void scantext_test4(void)
{
    char string[] = "\t\t\t\t\t\t";
    int count;
    int tabs;
    int tabsize;
    int char_count;
    int print_length;

    printf("\n== scantext_test4 ==========================================================\n");
    printf("|%s|\n", string);
    tabs = count_tabs(string);
    printf("Number of tabs is %i.\n", tabs);

    tabsize = 4;
    calculate_lengths(string, tabsize, &char_count, &print_length);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);

    count = substitute_char(string, '\t', ' ');
    printf("Replacements made: %i. After replacing tabs with spaces:\n", count);
    printf("|%s|\n", string);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("Number of tabs is %i\n", count_tabs(string));

    tabsize = 4;
    calculate_lengths(string, tabsize, &char_count, &print_length);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);
}


static void scantext_test5(void)
{
    char string[] = "This is a simple string.";
    int count;

    printf("\n== scantext_test5 ==========================================================\n");
    printf("|%s|\n", string);
    count = count_words(string);
    printf("Word count is %2i\n", count);
}

static void scantext_test6(void)
{
    char string[] = "These\twords\tare\tseparated\tby\ttabs.";
    int count;

    printf("\n== scantext_test6 ==========================================================\n");
    printf("|%s|\n", string);
    count = count_words(string);
    printf("Word count is %2i\n", count);
}

static void scantext_test7(void)
{
    char string[] = "  This string has leading and trailing spaces.  ";
    int count;

    printf("\n== scantext_test7 ==========================================================\n");
    printf("|%s|\n", string);
    count = count_words(string);
    printf("Word count is %2i\n", count);
}

static void scantext_test8(void)
{
    char string[] = "  \t This\t  string  \t \t\t  has \t  \tfive\t\n  words.\n";
    int count;

    printf("\n== scantext_test8 ==========================================================\n");
    printf("|%s|\n", string);
    count = count_words(string);
    printf("Word count is %2i\n", count);
    printf("Number of tabs is %i\n", count_tabs(string));
}

static void scantext_test9(void)
{
    char string[] = "  \t This\t  string\n  \t \t\t  has\n \t\n  \t\"more\" than five \n\t\n  words.\n\n";
    int count;

    printf("\n== scantext_test9 ==========================================================\n");
    printf("|%s|\n", string);
    count = count_words(string);
    printf("Word count is %2i\n", count);
    printf("Number of tabs is %i\n", count_tabs(string));
}

static void scantext_test10(void)
{
    char string[] = "Four\nscore\tand\nseven\t"
        "years\nago\nour fathers\tbrought\n"
        "forth\ton\nthis\ncontinent a\n"
        "new\tnation.";

    int count;
    int tabs;
    int tabsize;
    int char_count;
    int print_length;

    printf("\n== scantext_test10 ==========================================================\n");
    printf("|%s|\n", string);
    tabs = count_tabs(string);
    printf("Number of tabs is %i.\n", tabs);

    tabsize = 4;
    calculate_lengths(string, tabsize, &char_count, &print_length);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("With tab size of %i, the print length is %i.\n", tabsize, print_length);

    count = substitute_char(string, '\n', ' ');
    printf("Replacements made: %i. After replacing newlines with spaces:\n", count);
    printf("|%s|\n", string);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("Number of tabs is %i\n", count_tabs(string));

    count = substitute_char(string, '\t', ' ');
    printf("Replacements made: %i. After replacing tabs with spaces:\n", count);
    printf("|%s|\n", string);
    printf("Number of characters in the string is %i.\n", char_count);
    printf("Number of tabs is %i\n", count_tabs(string));
}


void scantext(void)
{
    scantext_test1();
    scantext_test2();
    scantext_test3();
    scantext_test4();
    scantext_test5();
    scantext_test6();
    scantext_test7();
    scantext_test8();
    scantext_test9();
    scantext_test10();
}
