/*
* 
* Filename: main.cpp
* Author: Jennifer Assid
* Date: 21 June 2024
* 
* Description: Creating one application to contain all of the course work completed at
*              CS120: High-Level Programming I at DigiPen Institute of Technology during
*              the Fall 2020 semester.
* 
*/

// Applicable Libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Headers for linking
#include "labs.h"
#include "assignments.h"

// Different States
typedef enum
{
    Undefined = -1,
    Main_Menu, Exit,

    Labs,
        Marathon,
        Numbers,
        Dates,
        Cubes,
        Primes,
        Sieve,
        Calendar,
        Histogram,
        Tablen,
        LinkedLists,

    Assignments,
        Pi,
        Arrays,
        Palindrome,
        Bytes,
        Scantext,
        Spellcheck

} State;

// State Strings - for user facing text
const char* States[] = 
{ 
    "Main Menu", "Exit", 

    "Labs", 
        "Marathon",
        "Numbers",
        "Dates",
        "Cubes",
        "Primes",
        "Sieve",
        "Calendar",
        "Histogram",
        "Tablen",
        "LinkedLists",

    "Assignments",
        "Pi",
        "Arrays",
        "Palindrome",
        "Bytes",
        "Scantext",
        "Spellcheck"
};

// Informing if the program is running
int program_running = 0;

// Variables needed for the State Machine
struct StateMachine
{
    State current_state;
    State next_state;

    State return_state;
    State projected_state;
};

// Clean up the code
typedef struct StateMachine StateMachine;

// Options to display on the Main Menu (along with functionality)
const int main_select[3][2] =
{
    {'l', (int)Labs},
    {'a', (int)Assignments},
    {'e', (int)Exit}
};

// Options to display on the Lab selection (along with functionality)
const int lab_select[11][2] =
{
    {'0', (int)Marathon},
    {'1', (int)Numbers},
    {'2', (int)Dates},
    {'3', (int)Cubes},
    {'4', (int)Primes},
    {'5', (int)Sieve},
    {'6', (int)Calendar},
    {'7', (int)Histogram},
    {'8', (int)Tablen},
    {'9', (int)LinkedLists},
    {'m', (int)Main_Menu}
};

// Options to display on the Assignment selection (along with functionality)
const int assignment_select[7][2] =
{
    {'0', (int)Pi},
    {'1', (int)Arrays},
    {'2', (int)Palindrome},
    {'3', (int)Bytes},
    {'4', (int)Scantext},
    {'5', (int)Spellcheck},
    {'m', (int)Main_Menu}
};

/********************          UTILITY           ********************/

// Abstraction of the user selection - checks user input against 2D array
// to determine validity of input and associated next state
void UserInput_Selection(StateMachine* sm, int* arr, unsigned int m)
{
    char s[10];
    printf("____________________________________________________________________\n");
    printf("\nType your selection: ");

    while (1 == 1)
    {
        // Memory safe way to receive/store the user input
        scanf_s("%s", &s, (unsigned)sizeof(char) * 10);
        char c = tolower(s[0]);

        for (unsigned int i = 0; i < m; i++)
        {
            if (c == tolower(*(arr + i * 2)))
            {
                sm->next_state = (State) *((arr + i * 2) + 1);
                return;
            }
        }

        // Prompt user for valid input
        printf("Input a valid selection from the list above: ");
    }
}

/*******************          MAIN MENU           *******************/
void MainMenu_Prompt(void)
{
    printf("MAIN MENU PROMPT\n");
    printf("Choose from one of the options below.\n\n");

    printf("L : Labs - View lab programs.\n");
    printf("A : Assignemnts - View assignment programs.\n");
    printf("E : Exit - Close the application.\n");
}

/*********************          LABS        *************************/
void Labs_Prompt(void)
{
    printf("LABS PROMPT\n");
    printf("Choose from one of the options below to run the associated code.\n\n");

    printf("0 : Marathon - Calculates how many yards are within a marathon.\n");
    printf("1 : Numbers - Takes three inputs, caldulates their sin/cos/tan/atan values and prints the results in a table.\n");
    printf("2 : Dates - Takes two dates from user input and determines if the first date is before, after, or the same as the second date.\n");
    printf("3 : Cubes - contains two functions: print_cubes and print_pythagorean_triples.\n");
    printf("4 : Primes - Determining if a number is prime as well as displaying that all even numbers within a range are the sum of two prime #s.\n");
    printf("5 : Sieve -  Holds functions that do different things with the same array and range of numbers.\n");
    printf("6 : Calendar - Holds functions is_leapyear; day_of_the_week; and print_calendar.\n");
    printf("7 : Histogram - Holds functions that derives stats on a provided string.\n");
    printf("8 : Tablen - Analyzes a file to check for tabs and long lines.\n");
    printf("9 : List - holds the functions that manipulate, assess, and output various linked lists.\n");
    printf("m : Main Menu - RETURN to Main Menu.\n");
}

/*******************         ASSIGNMENT        **********************/
void Assignments_Prompt(void)
{
    printf("ASSIGNMENTS PROMPT\n");
    printf("Choose from one of the options below to run the associated code.\n\n");

    printf("0 : Pi - Runs through the Riemann Sum and Leibniz equations to give an output via table\n");
    printf("1 : Arrays - Performs a variety of functions on provided arrays.\n");
    printf("2 : Palindrome - Determines if an array of characters is a palindrome based off of array notation.\n");
    printf("3 : Bytes - Includes functions that analyze and manipulate strings.\n");
    printf("4 : Scantext - Analyzes and manipulate given strings based on input.\n");
    printf("5 : Spellcheck - Analyzes strings to a given file\n");
    printf("m : Main Menu - RETURN to Main Menu.\n");
}


/****************          STATE MACHINE           ******************/
int Confirm_StateChange(StateMachine* sm)
{
    // If it is the first time entering the function - transfer to next state
    if (sm->return_state == Undefined) return 1;
    
    char s[10]; // Buffer to hold user input

    // Broadcast confirmation message showiung the previous and projected states
    printf("\n*******************************************************************\n");
    printf("\nContinue to %s from %s?\n\n", States[sm->next_state], States[sm->current_state]);
    printf("Type 'Y' to continue or 'N' to return: ");

    // Cycles until user provides a valid input
    while (1 == 1)
    {
        // Memory safe way to receive/store the user input
        scanf_s("%s", &s, (unsigned)sizeof(char) * 10);

        // Compare the user input to the valid inputs
        if (strcmp(s, "Y") == 0 || strcmp(s, "y") == 0)
        {
            // Reset the return/projected states and confirm transfer
            sm->return_state = sm->projected_state = Undefined;
            return 1;
        }
        else if (strcmp(s, "N") == 0 || strcmp(s, "n") == 0)
        {
            // Reset the return/projected states and disapprove transfer
            sm->return_state = sm->projected_state = Undefined;
            sm->next_state = sm->current_state;
            return 0;
        }

        // Prompt user for valid input
        printf("Input a valid selection (Y/N): ");
    }
}

void StateMachine_Init(StateMachine* sm)
{
    switch (sm->current_state)
    {
    case Main_Menu:
        MainMenu_Prompt();
        UserInput_Selection(sm, &main_select, 3);
        break;
    case Exit:
        program_running = 0;
        break;
    case Labs:
        Labs_Prompt();
        UserInput_Selection(sm, &lab_select, 11);
        break;
    case Assignments:
        Assignments_Prompt();
        UserInput_Selection(sm, &assignment_select, 7);
        break;
    default:
        break;
    }
}

void StateMachine_Exit(StateMachine* sm)
{
    system("cls");
}

void StateMachine_Update(StateMachine* sm)
{
    // Determine if the state needs to be changed and adjust accordingly
    if (sm->current_state != sm->next_state)
    {
        sm->return_state = sm->current_state;
        sm->projected_state = sm->next_state;

        if (Confirm_StateChange(sm) == 1)
        {
            StateMachine_Exit(sm);

            sm->current_state = sm->next_state;
        }

        StateMachine_Init(sm);
    }

    // Trigger the functions in other files
    switch (sm->current_state)
    {
    // LABS
    case Marathon:
        marathon();
        sm->next_state = Labs;
        break;
    case Numbers:
        numbers();
        sm->next_state = Labs;
        break;
    case Dates:
        dates();
        sm->next_state = Labs;
        break;
    case Cubes:
        cubes();
        sm->next_state = Labs;
        break;
    case Primes:
        primes();
        sm->next_state = Labs;
        break;
    case Sieve:
        sieve_cum();
        sm->next_state = Labs;
        break;
    case Calendar:
        calendar();
        sm->next_state = Labs;
        break;
    case Histogram:
        histogram();
        sm->next_state = Labs;
        break;
    case Tablen:
        tablen_test();
        sm->next_state = Labs;
        break;
    case LinkedLists:
        list();
        sm->next_state = Labs;
        break;

    // ASSIGNMENTS
    case Pi:
        pi();
        sm->next_state = Assignments;
        break;
    case Arrays:
        arrays();
        sm->next_state = Assignments;
        break;
    case Palindrome:
        palindrome();
        sm->next_state = Assignments;
        break;
    case Bytes:
        bytes();
        sm->next_state = Assignments;
        break;
    case Scantext:
        scantext();
        sm->next_state = Assignments;
        break;
    case Spellcheck:
        spellcheck();
        sm->next_state = Assignments;
        break;
    default:
        break;
    }
}


int main(void)
{
    // Establish the state machine with default values
    StateMachine program;
    program.current_state = Undefined;
    program.next_state = Main_Menu;
    program.return_state = Undefined;
    program.projected_state = Undefined;

    // Set the program running 'bool'
    program_running = 1;

    // Main loop for the program
    while (program_running == 1)
    {
        // Update the state machine
        StateMachine_Update(&program);
    }    

    return 0;
}