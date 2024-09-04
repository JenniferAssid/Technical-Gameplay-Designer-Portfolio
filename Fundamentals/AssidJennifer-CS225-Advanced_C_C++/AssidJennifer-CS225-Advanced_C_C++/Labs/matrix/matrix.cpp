/*****************************************************************
 * \file   matrix.cpp
 * \author Jennifer Assid
 * \par    email: jennifer.assid@digipen.edu
 * \par    DigiPen Login: jennifer.assid
 * \par    Course: CS 225
 * \par    Section A
 * \par    Unknown
 * \date   Fall 2021
 * \brief
 *    Contains all the functions needed to create, manipulate, and analyze matrices
 *********************************************************************/
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

typedef int** Matrix;

void  matrix_print(Matrix m, int num_rows, int num_columns) {
    int i, j;
    for (i = 0; i < num_rows; ++i) {
        for (j = 0; j < num_columns; ++j) {
            printf("%4i ", m[i][j]);
        }
        printf("\n");
    }
}

/* allocate 2-dimentional array with the specified dimensions */
Matrix matrix_create(int num_rows, int num_columns)
{
    int i = 0;
    Matrix mat = (Matrix)calloc((size_t)num_rows, sizeof(int*));
    for (i = 0; i < num_rows; i++)
        mat[i] = (int*)calloc((size_t)num_columns, sizeof(int));

    return mat;
}
/* matrix addition, assume result matrix is already allocated */
void matrix_add(Matrix m1, Matrix m2, Matrix result, int num_rows, int num_columns)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_columns; j++)
            result[i][j] = m1[i][j] + m2[i][j];
    }
}
/* matrix transposition, returns a new matrix, does not delete the original */
Matrix matrix_transpose(Matrix m, int num_rows, int num_columns)
{
    Matrix result = matrix_create(num_columns, num_rows);
    int i = 0;
    int j = 0;

    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_columns; j++)
        {
            result[j][i] = m[i][j];
        }
    }

    return result;
}
/* deallocate matrix */
void   matrix_delete(Matrix m, int num_rows)
{
    int i = 0;
    for (i = 0; i < num_rows; i++)
        free(m[i]);

    free(m);
}
/* delete one row (index r) from matrix */
void   matrix_delete_row(Matrix m, int r, int num_rows)
{
    int i = r;

    free(m[r]);
    for (i = r; i < num_rows - 1; i++)
        m[i] = m[i + 1];
}
/* delete one column (index c) from matrix */
void   matrix_delete_column(Matrix m, int c, int num_rows, int num_columns)
{
    int i = 0;
    int j = c;

    for (i = 0; i < num_rows; i++)
    {
        for (j = c; j < num_columns - 1; j++)
        {
            m[i][j] = m[i][j + 1];
        }
    }
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY
#include <stdio.h>

void matrix_test0() {
    Matrix m1 = matrix_create(3, 3);
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = i + j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    matrix_delete(m1, 3);
}

void matrix_test1() {
    Matrix m1 = matrix_create(3, 3);
    Matrix m2 = matrix_create(3, 3);
    Matrix m3 = matrix_create(3, 3);
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = i + j; } }
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m2[i][j] = 5; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);
    printf("m2 =\n");
    matrix_print(m2, 3, 3);

    matrix_add(m1, m2, m3, 3, 3);
    printf("m1 + m2 =\n");
    matrix_print(m3, 3, 3);

    matrix_delete(m1, 3);
    matrix_delete(m2, 3);
    matrix_delete(m3, 3);
}

void matrix_test2() {
    Matrix m1 = matrix_create(3, 3);
    Matrix m1_t;
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = i + 2 * j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    printf("transpose m1 =\n");
    m1_t = matrix_transpose(m1, 3, 3);
    matrix_print(m1_t, 3, 3);

    matrix_delete(m1, 3);
    matrix_delete(m1_t, 3);
}

void matrix_test3() {
    Matrix m1 = matrix_create(3, 3);
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = i + 2 * j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    printf("delete row 0 from m1 =\n");
    matrix_delete_row(m1, 0, 3);
    matrix_print(m1, 2, 3);

    matrix_delete(m1, 2);
}

void matrix_test4() {
    Matrix m1 = matrix_create(3, 3);
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = i + 2 * j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    printf("delete column 1 from m1 =\n");
    matrix_delete_column(m1, 1, 3, 3);
    matrix_print(m1, 3, 2);

    matrix_delete(m1, 3);
}

void matrix_test5() {
    Matrix m1 = matrix_create(3, 3);
    Matrix m1_t;
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = i + 2 * j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    printf("transpose m1 =\n");
    m1_t = matrix_transpose(m1, 3, 3);
    matrix_print(m1_t, 3, 3);

    printf("delete row 0 =\n");
    matrix_delete_row(m1_t, 0, 3);
    matrix_print(m1_t, 2, 3);

    matrix_delete(m1, 3);
    matrix_delete(m1_t, 2);
}

void matrix_test6() {
    Matrix m1 = matrix_create(3, 3);
    Matrix m1_t;
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = 3 * i - j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    printf("delete row 1 =\n");
    matrix_delete_row(m1, 1, 3);
    matrix_print(m1, 2, 3);

    printf("transpose =\n");
    m1_t = matrix_transpose(m1, 2, 3);
    matrix_print(m1_t, 3, 2);

    matrix_delete(m1, 2);
    matrix_delete(m1_t, 3);
}

void matrix_test7() {
    Matrix m1 = matrix_create(3, 3);
    Matrix m1_t;
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = i + 2 * j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    printf("transpose m1 =\n");
    m1_t = matrix_transpose(m1, 3, 3);
    matrix_print(m1_t, 3, 3);

    printf("delete column 1 =\n");
    matrix_delete_column(m1_t, 1, 3, 3);
    matrix_print(m1_t, 3, 2);

    matrix_delete(m1, 3);
    matrix_delete(m1_t, 3);
}

void matrix_test8() {
    Matrix m1 = matrix_create(3, 3);
    Matrix m1_t;
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = 3 * i - j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    printf("delete column 2 =\n");
    matrix_delete_column(m1, 2, 3, 3);
    matrix_print(m1, 3, 2);

    printf("transpose =\n");
    m1_t = matrix_transpose(m1, 3, 2);
    matrix_print(m1_t, 2, 3);

    matrix_delete(m1, 3);
    matrix_delete(m1_t, 2);
}

void matrix_test9() {
    Matrix m1 = matrix_create(3, 3);
    int i, j;
    for (i = 0; i < 3; ++i) { for (j = 0; j < 3; ++j) { m1[i][j] = 3 * i - j; } }

    printf("m1 =\n");
    matrix_print(m1, 3, 3);

    printf("delete column 2 =\n");
    matrix_delete_column(m1, 2, 3, 3);
    matrix_print(m1, 3, 2);

    printf("delete row 1 =\n");
    matrix_delete_row(m1, 1, 3);
    matrix_print(m1, 2, 2);

    printf("delete column 1 =\n");
    matrix_delete_column(m1, 1, 2, 2);
    matrix_print(m1, 2, 1);

    printf("delete row 0 =\n");
    matrix_delete_row(m1, 0, 2);
    matrix_print(m1, 1, 1);

    matrix_delete(m1, 1);
}

void matrix_test10() {
    int dim = 5;
    Matrix m1 = matrix_create(dim, dim);
    Matrix m2;
    int i, j;
    for (i = 0; i < dim; ++i) { for (j = 0; j < dim; ++j) { m1[i][j] = 3 * i - j; } }


    printf("m1 =\n");
    matrix_print(m1, dim, dim);

    while (dim > 1) {
        printf("delete row 0 =\n");
        matrix_delete_row(m1, 0, dim);
        matrix_print(m1, dim - 1, dim);

        printf("transpose =\n");
        m2 = matrix_transpose(m1, dim - 1, dim);
        matrix_print(m2, dim, dim - 1);
        matrix_delete(m1, dim - 1);

        printf("delete row 0 =\n");
        matrix_delete_row(m2, 0, dim);
        matrix_print(m2, dim - 1, dim - 1);

        printf("transpose =\n");
        m1 = matrix_transpose(m2, dim - 1, dim - 1);
        matrix_print(m1, dim - 1, dim - 1);
        matrix_delete(m2, dim - 1);
        --dim;
    }

    matrix_delete(m1, dim);
}

void matrix_test11() {
    int dim = 5;
    Matrix m1 = matrix_create(dim, dim);
    Matrix m2;
    int i, j;
    for (i = 0; i < dim; ++i) { for (j = 0; j < dim; ++j) { m1[i][j] = 3 * i - j; } }


    printf("m1 =\n");
    matrix_print(m1, dim, dim);

    while (dim > 1) {
        printf("delete row %d =\n", dim - 1);
        matrix_delete_row(m1, dim - 1, dim);
        matrix_print(m1, dim - 1, dim);

        printf("transpose =\n");
        m2 = matrix_transpose(m1, dim - 1, dim);
        matrix_print(m2, dim, dim - 1);
        matrix_delete(m1, dim - 1);

        printf("delete row %d =\n", dim - 1);
        matrix_delete_row(m2, dim - 1, dim);
        matrix_print(m2, dim - 1, dim - 1);

        printf("transpose =\n");
        m1 = matrix_transpose(m2, dim - 1, dim - 1);
        matrix_print(m1, dim - 1, dim - 1);
        matrix_delete(m2, dim - 1);
        --dim;
    }

    matrix_delete(m1, dim);
}

void (*pmatrix_tests[])() = {
    matrix_test0,matrix_test1,matrix_test2,matrix_test3,matrix_test4,matrix_test5,matrix_test6,
    matrix_test7,matrix_test8,matrix_test9,matrix_test10,matrix_test11};

void matrix() {
    unsigned i;
    for (i = 0; i < sizeof(pmatrix_tests) / sizeof(pmatrix_tests[0]); ++i)
        pmatrix_tests[i]();
}
