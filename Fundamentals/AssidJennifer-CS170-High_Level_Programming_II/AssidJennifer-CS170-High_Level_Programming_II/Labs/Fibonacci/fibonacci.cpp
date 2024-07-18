/*!****************************************************************************
\file   fibonacci.cpp
\author Jennifer Assid
\par    DP email: jennifer.assid\@digipen.edu
\par    Course: CS170
\par    Programming Assignment #1
\date   19-01-2021

\brief
  This program takes a number and creates a table containing every Fibonacci
  number and quotient from 0 to the number given. The functions include:

\par Hours spent on this assignment: 2

\par Specific portions that gave you the most trouble: formatting the output
   using setw(), fixed, and setprecision().
******************************************************************************/

#include <iostream> // cout, endl
#include <iomanip>  // setw, fixed, setprecision

/*!****************************************************************************
\brief
   Iterates through number between 0 and given value and calculates / outputs
   the corresponding Fibonacci number and quotient.

\param max
   The max number the program will iterate up to.

\return N/A
   Outputs a formatted table containing the Fibonacci numbers and quotients
   from 0 to number given (max)
******************************************************************************/
void fibonacci(int max)
{
    /// Prints out header for the output table along with the first two numbers
    std::cout << "           Fibonacci        Fibonacci" << std::endl;
    std::cout << " N           number          quotient" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << " 0             0                  N/A" << std::endl;
    std::cout << " 1             1                  N/A" << std::endl;

    /// Sets initial variables
    int count = max;
    int fib1 = 0, fib2 = 1;

    /// Iterates from 2 (the next number in the sequence) to the max number
    for (int i = 2; i <= count; i++)
    {
        /// Finds the Fibonacci Number
        int next = fib1 + fib2;

        /// Formats and prints to the N column
        std::cout << std::setw(2);
        std::cout << i;

        /// Formats and prints Fibonacci Number
        std::cout << std::setw(14);
        std::cout << next << "   ";

        /// Formats and prints Fibonacci Quotient
        std::cout << std::fixed;
        std::cout << std::setprecision(16) << (double)next / (double)fib2;
        std::cout << std::endl;

        /// Sets variables for next iteration
        fib1 = fib2;
        fib2 = next;
    }
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <string>
void fibonacci_test(void)
{
    std::cout << "Specify the amount of fibonacci numbrs to generate: ";
    std::string user_input;
    int result = 0;

    bool completed = false;

    while (completed == false)
    {
        try
        {
            std::cin >> user_input;
            result = std::stoi(user_input);
            if (result > 0) completed = true;
        }
        catch (std::exception e)
        {
            std::cout << "Input valid argument (> 0): ";
            continue;
        }

        std::cout << "Input valid argument (> 0): ";
    }

    fibonacci(result);
}