/***************************************
filename    dates.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    3
due date    01/10/2020

Brief Description:
    Takes two dates from user input and determines if the first date is
    before, after, or the same as the second date.

***************************************/

#include <stdio.h> /* printf, scanf*/

#include "labs.h"

void dates(void)
{

    /* declaring variables for first user inputted date */
    int first_date_day, first_date_month, first_date_year;

    /* declaring variables for second user inputted date */
    int second_date_day, second_date_month, second_date_year;

    /* prompting user to input first date and store values in variables */
    printf("Enter the first date: (mm/dd/yyyy) ");
    scanf("%i/%i/%i", &first_date_month, &first_date_day, &first_date_year);

    /* prompting user to input second date and store values in variables */
    printf("Enter the second date: (mm/dd/yyyy) ");
    scanf("%i/%i/%i", &second_date_month, &second_date_day, &second_date_year);


    /* checking to see if the year of the first date is more than the year of
    the second date and printing out the desired output if true */
    if (first_date_year > second_date_year)
    {
        printf("%02i/%02i/%04i comes after %02i/%02i/%04i.\n",
            first_date_month, first_date_day, first_date_year,
            second_date_month, second_date_day, second_date_year);

        /* moving from above statement if it is not true into the next statement
        checking if the year of the first date is less than the year of the
        second and printing out the desired output if true */
    }
    else if (first_date_year < second_date_year)
    {
        printf("%02i/%02i/%04i comes before %02i/%02i/%04i.\n",
            first_date_month, first_date_day, first_date_year,
            second_date_month, second_date_day, second_date_year);

        /* moving from above statement if it is not true into the next statement
        checking if the year of the first date is equal to the year of the
        second and moving on to the next set of conditionals. */
    }
    else if (first_date_year == second_date_year)
    {
        /* moving from above statement if it is true into the next statement
        checking if the month of the first date is more than the month of the
        second and printing out the desired output if true */
        if (first_date_month > second_date_month)
        {
            printf("%02i/%02i/%04i comes after %02i/%02i/%04i.\n",
                first_date_month, first_date_day, first_date_year,
                second_date_month, second_date_day, second_date_year);

            /* moving from above statement if it is not true into the next statement
            checking if the month of the first date is less than the month of the
            second and printing out the desired output if true */
        }
        else if (first_date_month < second_date_month)
        {
            printf("%02i/%02i/%04i comes before %02i/%02i/%04i.\n",
                first_date_month, first_date_day, first_date_year,
                second_date_month, second_date_day, second_date_year);

            /* moving from above statement if it is not true into the next statement
            checking if the month of the first date is equal to the month of the
            second and moving on to the next set of conditionals. */
        }
        else if (first_date_month == second_date_month)
        {
            /* moving from above statement if it is true into the next statement
            checking if the day of the first date is more than the day of the
            second and printing out the desired output if true */
            if (first_date_day > second_date_day)
            {
                printf("%02i/%02i/%04i comes after %02i/%02i/%04i.\n",
                    first_date_month, first_date_day, first_date_year,
                    second_date_month, second_date_day, second_date_year);

                /* moving from above statement if it is not true into the next
                statement checking if the day of the first date is less than the
                day of the second and printing out the desired output if true */
            }
            else if (first_date_day < second_date_day)
            {
                printf("%02i/%02i/%04i comes before %02i/%02i/%04i.\n",
                    first_date_month, first_date_day, first_date_year,
                    second_date_month, second_date_day, second_date_year);

                /* moving from above statement if it is not true into the next
                statement checking if the day of the first date is equal to the
                day of the second and printing out the desired output if true */
            }
            else if (first_date_day == second_date_day)
            {
                printf("%02i/%02i/%04i and %02i/%02i/%04i are the same date.\n",
                    first_date_month, first_date_day, first_date_year,
                    second_date_month, second_date_day, second_date_year);

                return;
            }

            return;

        }

        return;

    }

    return;

}