#define SIZE 1001   /* The size of the array        */
#define TRUE 1

#include <stdio.h>
#include "labs.h"

/***************************************
filename    calendar.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    8
due date    11/05/2020

Brief Description: holds functions is_leapyear; day_of_the_week;
                   and print_calendar.

***************************************/

#include <stdio.h> /* printf */

/******************************************************************************
   Function: is_leapyear

Description: determines if a given year is a leap year

     Inputs: int year - year being analyzed
    Outputs: TRUE (1) or FALSE (0)
******************************************************************************/
int is_leapyear(int year)
{
    /*checks the exception case for leap years*/
    if (year % 100 == 0 && year % 400 != 0)
        return 0;

    if (year % 400 == 0)
        return 1;

    /*checks general case for leap years*/
    if (year % 4 == 0)
        return 1;

    return 0;
}

/******************************************************************************
   Function: day_of_the_week(int day, int month, int year)

Description: determines what day (ie "Sunday) a numerical value (ie 1) falls on

     Inputs: int day - numerical value to be analyzed
             int month - month containing the day
             int year - year containing the month
    Outputs: 1-7 to represent Sunday - Saturday
******************************************************************************/
int day_of_the_week(int day, int month, int year)
{
    /*initializes necessary variables*/
    int lastTwoDigits; int calculation;
    int months[12] = { 1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6 };

    /*Performs initial, basic arithmetic*/
    lastTwoDigits = year % 100;
    calculation = lastTwoDigits / 4;
    calculation += day;
    calculation += months[month - 1];

    /*brings year down into century key range*/
    while (year > 2099)
        year -= 400;

    /*determines and adds necessary century key*/
    if (year >= 1700 && year <= 1799)
        calculation += 4;

    if (year >= 1800 && year <= 1899)
        calculation += 2;

    if (year >= 1900 && year <= 1999)
        calculation += 0;

    if (year >= 2000 && year <= 2099)
        calculation += 6;

    /*Finalizes the basic arithmetic*/
    calculation += lastTwoDigits;
    calculation = calculation % 7;

    /*Changes "Saturday" index to be considered 7 instead of 0*/
    if (calculation == 0)
        calculation = 7;

    return calculation;
}

/******************************************************************************
   Function: print_calendar(int month, int year)

Description: prints out a formated calendar for the month notated.

     Inputs: int month - month to be printed
             int year - year containing the month
    Outputs: formated calendar
******************************************************************************/
void print_calendar(int month, int year)
{
    /*Initializes necessary variables*/
    int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    char* months[] = { "January",   "February", "March",    "April",
                      "May",       "June",     "July",     "August",
                      "September", "October",  "November", "December" };

    int day = 1; int day_counter; int week_placement = 1;

    /* Print calendar header */
    printf("%s, %i\n", months[month - 1], year);
    printf("Su Mo Tu We Th Fr Sa\n");
    printf("---------------------\n");

    /*Changes total days in February to account of leap year*/
    if (is_leapyear(year))
        days_in_month[1] = 29;

    /*adds padding if the first day of the month does not start on Sunday*/
    for (day_counter = 1;
        day_counter < day_of_the_week(1, month, year);
        day_counter++)
    {
        printf("   ");
        week_placement++;
    }
    /*prints the days out in order and jumps to next line at end of week*/
    for (week_placement = week_placement;
        week_placement <= 8 && day <= days_in_month[month - 1];
        week_placement++)
    {
        /*resets the week_placement to start new line*/
        if (week_placement == 8)
        {
            printf("\n");
            week_placement = 1;
        }

        printf("%2d ", day);
        day++;
    }

    /*adds newline to separate calendars*/
    printf("\n");

}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/30/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
#include <stdio.h>

static char* day_name[] = { "Sunday", "Monday", "Tuesday", "Wednesday",
                         "Thursday", "Friday", "Saturday" };

void test_leapyear(void)
{
    int year;
    int i;

    year = 1700;
    printf("%4i %s a leap year\n", year, is_leapyear(year) ? "is" : "is not");
    year = 1900;
    printf("%4i %s a leap year\n", year, is_leapyear(year) ? "is" : "is not");
    year = 1901;
    printf("%4i %s a leap year\n", year, is_leapyear(year) ? "is" : "is not");
    year = 1904;
    printf("%4i %s a leap year\n", year, is_leapyear(year) ? "is" : "is not");
    year = 1999;
    printf("%4i %s a leap year\n", year, is_leapyear(year) ? "is" : "is not");
    year = 2000;
    printf("%4i %s a leap year\n", year, is_leapyear(year) ? "is" : "is not");
    year = 2300;
    printf("%4i %s a leap year\n", year, is_leapyear(year) ? "is" : "is not");
    year = 2400;
    printf("%4i %s a leap year\n", year, is_leapyear(year) ? "is" : "is not");

    for (i = 2001; i < 2020; i++)
        printf("%4i %s a leap year\n", i, is_leapyear(i) ? "is" : "is not");
}

void test_day(void)
{
    int day, month, year;
    int day_of_week;
    int i;

    day = 1;
    month = 1;
    year = 2014;
    day_of_week = day_of_the_week(day, month, year);
    printf("day: %i, month: %i, year: %i is on day %i\n",
        day, month, year, day_of_week);

    day = 1;
    month = 2;
    year = 2014;
    day_of_week = day_of_the_week(day, month, year);
    printf("day: %i, month: %i, year: %i is on day %i\n",
        day, month, year, day_of_week);

    day = 1;
    month = 3;
    year = 2014;
    day_of_week = day_of_the_week(day, month, year);
    printf("day: %i, month: %i, year: %i is on day %i\n",
        day, month, year, day_of_week);

    day = 1;
    month = 4;
    year = 2014;
    day_of_week = day_of_the_week(day, month, year);
    printf("day: %i, month: %i, year: %i is on day %i\n",
        day, month, year, day_of_week);

    day = 1;
    month = 5;
    year = 2014;
    day_of_week = day_of_the_week(day, month, year);
    printf("day: %i, month: %i, year: %i is on day %i\n",
        day, month, year, day_of_week);

    for (i = 1; i <= 31; i++)
    {
        day_of_week = day_of_the_week(i, 10, 2014);
        printf("day: %i, month: %i, year: %i is on day %i (%s)\n",
            i, 10, 2014, day_of_week, day_name[day_of_week - 1]);
    }
}

void test_calendar(void)
{
    int i;

    print_calendar(10, 2014);
    printf("\n");
    print_calendar(7, 1776);
    printf("\n");
    print_calendar(7, 2776);
    printf("\n");

    for (i = 1; i <= 12; i++)
    {
        print_calendar(i, 2014);
        printf("\n");
    }
}

void calendar(void)
{
    test_leapyear();
    test_day();
    test_calendar();

    return 0;
}