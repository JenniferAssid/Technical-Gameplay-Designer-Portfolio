/***************************************
filename    array.c
author      Jennifer Assid
DP email    jennifer.assid@digipen.edu
course      CS 120
section     A
lab week    6
due date    10/19/2020

Brief Description:

***************************************/

#include <stdio.h> /* printf */
#include "assignments.h"

/* print all elements of an array */
void print_array(const int a[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%5i", a[i]);
    printf("\n");
}

/******************************************************************************
   Function: reverse_array
Description: prints out initial elements within an array, reverses the order of
             the elements, and prints out the new order.

     Inputs: int a[] - the array that is being printed
             int size - how large the array is

    Outputs: the elements of the array in a single line with space in-between as
             well as the elements of the reversed array.
******************************************************************************/
void reverse_array(int a[], int size)
{
    /*defines the counter variable and storage array*/
    int array_counter = 0;
    int last_element = size - 1;
    int temp_hold;

    /*prints the initial a array*/
    printf("Array a: \n");
    print_array(a, size);

    while (array_counter < last_element)
    {
        /*places the first element in a temporary hold, places the last element
        in first slot, places first element in last slot*/
        temp_hold = a[array_counter];
        a[array_counter] = a[last_element];
        a[last_element] = temp_hold;

        /*adjusts counters to slowly converge on the middle of the array*/
        array_counter++;
        last_element--;
    }

    /*prints out reverse array a*/
    printf("Array a reversed:\n");
    print_array(a, size);
}

/******************************************************************************
   Function: add_arrays
Description: prints out the sums of two arrays in a new array

     Inputs: int a[] - array begin added
             int b[] - array being added
             int c[] - array where sums are being stored
             int size - how large the array is

    Outputs: the elements of array b and array c printed out
******************************************************************************/
void add_arrays(const int a[], const int b[], int c[], int size)
{
    /*defines counter variable*/
    int array_counter = 0;

    /*prints out array b*/
    printf("\nArray b:\n");
    print_array(b, size);

    /*adds the values of array a and array b and stores sums in array c*/
    for (array_counter = 0; array_counter < size; array_counter++)
    {
        c[array_counter] = a[array_counter] + b[array_counter];
    }

    /*prints out array c*/
    printf("Array c (sum of array a and array b):\n");
    print_array(c, size);

}

/******************************************************************************
   Function: scalar_multiply
Description: prints out the product of an array times a scalar

     Inputs: int a[] - array where sums are being stored
             int size - how large the array is
             int multiplier - number that array is multiplied by

    Outputs: the elements of array c after multiplication printed out
******************************************************************************/
void scalar_multiply(int a[], int size, int multiplier)
{
    /*defines counter variable*/
    int array_counter = 0;

    /*multiplies the elements of a by scalar*/
    for (array_counter = 0; array_counter < size; array_counter++)
    {
        a[array_counter] = a[array_counter] * multiplier;
    }

    /*prints out array a*/
    printf("\nAll values of c multiplied by %i:\n", multiplier);
    print_array(a, size);

}

/******************************************************************************
   Function: dot_product
Description: prints out the dot product between two arrays (vectors)

     Inputs: const int a[] - first array in equation
             const int b[] - second array in equation
             int size - how large the array is

    Outputs: the dot product of the two arrays (vectors)
******************************************************************************/
int dot_product(const int a[], const int b[], int size)
{
    /*defines the counter variable and initializes dot product*/
    int array_counter = 0;
    int dot_product = 0;

    /*prints out the two arrays (vectors)*/
    printf("\nArray v1:");
    print_array(a, size);

    printf("Array v2:");
    print_array(b, size);

    /*adds the product of the two array elements to a common variable*/
    for (array_counter = 0; array_counter < size; array_counter++)
    {
        dot_product = dot_product + (a[array_counter] * b[array_counter]);
    }

    /*prints out the dot product*/
    printf("The dot product of v1 * v2 is %i\n", dot_product);

    /*returns the dot product*/
    return dot_product;
}

/******************************************************************************
   Function: cross_product
Description: prints out the cross product between two arrays (vectors)

     Inputs: const int a[] - first array in equation
             const int b[] - second array in equation
             int size - how large the array is

    Outputs: the cross product of the two arrays (vectors)
******************************************************************************/
void cross_product(const int a[], const int b[], int c[])
{
    /*prints out the two arrays (vectors)*/
    printf("\nArray v1:");
    print_array(a, 3);

    printf("Array v2:");
    print_array(b, 3);

    /*determining the cross product of the two vectors and placing the value in
    a separate array*/
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];

    /*prints out the cross product*/
    printf("The cross product of v1 x v2 is:");
    print_array(c, 3);
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/30/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
// Original author: Artie Fufkin
#include "PRNG.h"  /* ut_srand, RandomInt */

/* generate random numbers between low and high */
/*
int RandomInt(int low, int high)
{
  int number;
  number = rand() % (high - low + 1) + low;
  return number;
}
*/

/* more multiply tests */
void array_test1(void)
{
#define SIZE1 10

    int a[SIZE1];
    int b[SIZE1];
    int i;
    int product;

    ut_srand(0, 1);
    for (i = 0; i < SIZE1; i++)
    {
        a[i] = RandomInt(-5, 5);
        b[i] = RandomInt(-5, 5);
    }
    printf("Array a:");
    print_array(a, SIZE1);
    printf("Array b:");
    print_array(b, SIZE1);

    product = dot_product(a, b, SIZE1);
    printf("The dot product of a * b is %i", product);
    printf("\n");
}

/* a bigger multiply test */
void array_test2(void)
{
#define SIZE2 1000

    int a[SIZE2];
    int b[SIZE2];
    int i;
    int product;

    ut_srand(0, 1);
    for (i = 0; i < SIZE2; i++)
    {
        a[i] = RandomInt(-10, 10);
        b[i] = RandomInt(-10, 10);
    }

    /* Too much to print out */
    /*
    printf("Array a:\n");
    print_array(a, SIZE2);
    printf("Array b:\n");
    print_array(b, SIZE2);
    */

    product = dot_product(a, b, SIZE2);
    printf("The dot product of a * b is %i", product);
    printf("\n");
}

void arrays(void)
{
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int b[] = { 4, 6, 8, 2, 4, 3, 5, 8 };
    int c[8];
    int size = sizeof(a) / sizeof(a[0]);

    int v1[] = { 1, 2, 3 };
    int v2[] = { 3, 4, 7 };
    int vsize = sizeof(v1) / sizeof(v1[0]);
    int v3[sizeof(v1) / sizeof(v1[0])];
    int product;

    /* reverse an array */
    printf("Array a:\n");
    print_array(a, size);
    reverse_array(a, size);
    printf("Array a reversed:\n");
    print_array(a, size);
    printf("\n");

    /* add two arrays into a third array */
    printf("Array b:\n");
    print_array(b, size);
    add_arrays(a, b, c, size);
    printf("Array c (sum of array a and array b):\n");
    print_array(c, size);
    printf("\n");

    /* multiply each element by 10 */
    scalar_multiply(c, size, 10);
    printf("All values of c multiplied by 10:\n");
    print_array(c, size);
    printf("\n");

    /* multiply two arrays (dot product) */
    printf("Array v1:");
    print_array(v1, vsize);
    printf("Array v2:");
    print_array(v2, vsize);
    product = dot_product(v1, v2, vsize);
    printf("The dot product of v1 * v2 is %i\n", product);
    printf("\n");

    /* multiply two arrays (cross product) */
    printf("Array v1:");
    print_array(v1, vsize);
    printf("Array v2:");
    print_array(v2, vsize);
    cross_product(v1, v2, v3);
    printf("The cross product of v1 x v2 is:");
    print_array(v3, 3);
    printf("\n");


    /* more tests */
    array_test1();
    array_test2();
    printf("\n");
}