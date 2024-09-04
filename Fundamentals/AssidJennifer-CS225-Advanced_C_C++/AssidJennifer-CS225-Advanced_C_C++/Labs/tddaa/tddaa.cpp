/*****************************************************************
 * \file   tddaa.c
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Unknown
 * \date   Fall 2021
 * \brief
 *    Allocates and deallocates
 *********************************************************************/

/*
2x3x4
+----------------------------------------+
|+------------------++------------------+|
||+----++----++----+||+----++----++----+||
|||1234||1234||1234||||1234||1234||1234|||
||+----++----++----+||+----++----++----+||
|+------------------++------------------+|
+----------------------------------------+

+------+
|abcdef| 6 pointers to 1's ( array of row pointers )
+------+

+--+
|mn| 2 pointers to a and d above ( array of pointers to slices )
+--+

a[i][j][k]
i chooses which slice
j chooses which row within the slice
k chooses within element with the row
*/

#include "tddaa.h"
#include <stdlib.h>
#include <stdio.h>

int*** allocate(int d1, int d2, int d3)
{
    int i = 0;

    int*** ppp_1 = (int***)calloc(d1, sizeof(int**));
    int** ppp_2 = (int**)calloc(d1 * d2, sizeof(int*));
    int* ppp_3 = (int*)calloc(d1 * d2 * d3, sizeof(int));

    for (i = 0; i < d1; i++)
    {
        ppp_1[i] = &ppp_2[i * d2];
    }

    for (i = 0; i < d1 * d2; i++)
    {
        ppp_2[i] = &ppp_3[i * d3];
    }

    return ppp_1;
}

void deallocate(int*** ppp)
{
    free(ppp[0][0]);
    free(ppp[0]);
    free(ppp);
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <stdio.h>

void run(int, int, int);
void tddaa_test0(void);
void tddaa_test1(void);

void run(int d1, int d2, int d3)
{
    /* printf( "%dx%dx%d\n", d1,d2,d3 ); */
    int i, j, k;
    char ok = 1;

    int*** ppp = allocate(d1, d2, d3);

    for (i = 0; i < d1; ++i) {
        for (j = 0; j < d2; ++j) {
            for (k = 0; k < d3; ++k) {
                ppp[i][j][k] = 100 * i + 10 * j + k; /* 3-digit number ijk if i,j,k are single digits */
            }
        }
    }

    for (i = 0; i < d1; ++i) {
        for (j = 0; j < d2; ++j) {
            for (k = 0; k < d3; ++k) {
                if (ppp[i][j][k] != 100 * i + 10 * j + k) {
                    printf("E");
                    ok = 0;
                }
            }
        }
    }

    if (ok) { printf("OK\n"); }

    deallocate(ppp);
}

void tddaa_test0(void)
{
    run(2, 3, 4);
}

#include <time.h> /* time */
#include <stdlib.h> /* rand/srand */
void tddaa_test1(void)
{
    srand(time(0));
    run(rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
}

void tddaa(void)
{
    tddaa_test0();
    tddaa_test1();
}
