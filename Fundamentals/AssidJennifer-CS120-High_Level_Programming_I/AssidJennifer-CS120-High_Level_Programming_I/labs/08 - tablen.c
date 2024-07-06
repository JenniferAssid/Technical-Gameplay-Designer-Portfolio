/***************************************
filename    tablen
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    10
due date    25 November 2020

Brief Description: analyzes a file to check for tabs and long lines

***************************************/

#include <stdio.h>  /* fopen, feof, fgets, fclose */
#include <string.h> /* strlen, strcspn            */
#include "labs.h"

#define MAX_LINE_LENGTH 1000 /* max length of any strings */

/******************************************************************************
   Function: DetectingLongLines

Description: Goes through a file and determines if a file is longer than 80 char

     Inputs: *filename - the file being analyzed
    Outputs: printing lines that are longer than 80 char with extra information
******************************************************************************/
void DetectingLongLines(const char* filename)
{
    char buffer[MAX_LINE_LENGTH];   /* string holding file strings   */
    FILE* file;                     /* pointer to the file           */

    int len;                        /* length of the string          */

    int lineCount = 0;              /* line numbers                  */
    int longLineCount = 0;          /* total of lines over 80 char   */

    int longestLineNumber = 0;      /* line number of longest line   */
    int longestLineCharacters = 0;  /* total of char in longest line */

    /* opens file */
    file = fopen(filename, "r");

    /* checks if file can be opened */
    if (file)
    {
        /* while code isn't at the end of the file */
        while (!feof(file))
        {
            /* if the line isn't NULL*/
            if (fgets(buffer, MAX_LINE_LENGTH, file))
            {
                /* remove the /n */
                buffer[strcspn(buffer, "\n")] = 0;

                /* increment the line count */
                lineCount++;

                /* length of the string */
                len = strlen(buffer);

                /* checks if the string is above the threshold */
                if (len > 80)
                {
                    /* prints out long line information */
                    printf("#%3i: (%3i) |%s|\n", lineCount, len, buffer);

                    /* increments the total of long lines */
                    longLineCount++;

                    /* checks if current length is above the current longest */
                    if (len > longestLineCharacters)
                    {
                        /* updates the longest length */
                        longestLineCharacters = strlen(buffer);

                        /* updates longest line number */
                        longestLineNumber = lineCount;
                    }
                }
            }
        }

        /* closes file*/
        fclose(file);

        /* prints overall long line information */
        printf("***** LINE TOO LONG *****:");
        printf(" %d lines longer than 80. Longest line is %d at %d chars.\n",
            longLineCount, longestLineNumber, longestLineCharacters);
    }
    /* checks if file is not able to be opened */
    else
        /* prints error message */
        printf("Cannot open %s for read.\n", filename);
}

/******************************************************************************
   Function: DetectingTabs

Description: analyzes a file for any tabs and prints extra information

     Inputs: *filename - file being analyzed
    Outputs: printing lines that have tabs with extra information
******************************************************************************/
void DetectingTabs(const char* filename)
{
    char buffer[MAX_LINE_LENGTH]; /* string holding file strings            */
    FILE* file;                   /* pointer to file                        */

    int i;                        /* loop counter                           */
    int len;                      /* length of string                       */
    int isThereTabs;              /* bool for is the string has tabs        */
    int tabTotalCount = 0;        /* running total of tabs in document      */
    int tabLineCount = 0;         /* running total of tabs in a single line */
    int lineCount = 0;            /* number of lines                        */

    /* opens file */
    file = fopen(filename, "r");

    /* checks if file is able to be opened */
    if (file)
    {
        /* while the code is not at the end of the file */
        while (!feof(file))
        {
            /* checks if file is not NULL */
            if (fgets(buffer, MAX_LINE_LENGTH, file))
            {
                /* removes the /n */
                buffer[strcspn(buffer, "\n")] = 0;

                /* increments line count */
                lineCount++;
                /* restarts the total of tabs in line */
                tabLineCount = 0;
                /* restarts bool for if there are tabs */
                isThereTabs = 0;

                /* length of the string */
                len = strlen(buffer);

                /* iterates through the string */
                for (i = 0; i < len; i++)
                {
                    /* sees if the index is a TAB */
                    if (buffer[i] == 9)
                    {
                        /* switches on the bool for tabs */
                        isThereTabs = 1;
                        /* increments number of tabs in a line*/
                        tabLineCount++;
                        /* increments number of tabs in the document*/
                        tabTotalCount++;
                    }
                }

                /* checks if there are tabs */
                if (isThereTabs)
                    /* prints the tab information */
                    printf("#%3i: (tabs:%2i) |%s|\n",
                        lineCount, tabLineCount, buffer);
            }
        }
        /* closes the file*/
        fclose(file);

        /* prints extra tab information */
        printf("*****  TABS IN USE  *****: Number of tabs in the file is %d.\n",
            tabTotalCount);
        DetectingLongLines(filename);
    }
    /* checks if file is not able to be opened */
    else
        /* prints error message */
        printf("Can't open %s for read.\n", filename);
}

/******************************************************************************
   Function: tablen

Description: analyzes a file to check for tabs and long lines

     Inputs: *filename - file being analyzed
    Outputs: printed out information of tabs and long lines if relevant
******************************************************************************/
void tablen(const char* filename)
{
    DetectingTabs(filename);
}

// ADDED TO LINK WITH PROGRAM
void tablen_test(void)
{
    tablen("input.txt");
}