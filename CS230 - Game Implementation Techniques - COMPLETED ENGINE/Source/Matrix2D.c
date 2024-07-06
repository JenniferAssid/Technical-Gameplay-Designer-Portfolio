//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix2D.h"
#include "Trace.h"

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// This function sets the matrix equal to the identity matrix.
void Matrix2DIdentity(Matrix2D* pResult)
{
  int row, col;
  
  // Iterates over the matrix (hardcoded width / height)
  for (row = 0; row < 3; row++)
  {
    for (col = 0; col < 3; col++)
    {
      // Places a one in the diagonal positions
      if (row == col)
        Matrix2DRowCol(pResult, row, col) = 1;
      // Places zero in the remaining positions
      else
        Matrix2DRowCol(pResult, row, col) = 0;
    }
  }
}

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
void Matrix2DTranspose(Matrix2D* pResult, Matrix2D* pMtx)
{
  Matrix2D temp = { 0 };

  int i, j;
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
      temp.m[i][j] = pMtx->m[j][i];
  }
  
  *pResult = temp;
}

// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
// Result = Mtx0*Mtx1
// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
void Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1)
{
  int i, j, k;             // Declares counter variables
  float sum = 0;           // Variable to store calculated sum
  Matrix2D result = { 0 }; // Matrix to hold calculated product

  // Iterates through primary (pMtx0) matrix
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      sum = 0;
      // Iterates through secondary (pMtx1) matrix
      for (k = 0; k < 3; k++)
      {
        // Performs calculation
        sum = sum + pMtx0->m[i][k] * pMtx1->m[k][j];
      }
      // Stores product in corresponding spot
      result.m[i][j] = sum;
    }
  }

  // Iterates through the pResult matrix
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      // Transfers values
      pResult->m[i][j] = result.m[i][j];
    }
  }
}

// This function creates a translation matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DTranslate(Matrix2D* pResult, float x, float y)
{
  // Initializing the matrix values
  Matrix2DIdentity(pResult);

  // 1 0 x
  // 0 1 y
  // 0 1 1
  pResult->m[0][2] = x;
  pResult->m[1][2] = y;
}

// This function creates a scaling matrix from x & y and saves it in Result.
// (Hint: This function must initialize all matrix values.)
void Matrix2DScale(Matrix2D* pResult, float x, float y)
{
  // Initalizing the matrix values
  Matrix2DIdentity(pResult);

  // x 0 0
  // 0 y 0
  // 0 1 1
  pResult->m[0][0] = x;
  pResult->m[1][1] = y;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// (Hint: This function must initialize all matrix values.)
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Matrix2DRotDeg(Matrix2D* pResult, float angle)
{
  // Initialize the matrix value
  Matrix2DIdentity(pResult);

  // Converting the angle to radians
  float radians = (angle * (float)M_PI) / 180.0f;

  // cos() -sin() 0
  // sin()  cos() 0
  //  0      0    1
  pResult->m[0][0] = cosf(radians);
  pResult->m[0][1] = -1.0f * sinf(radians);
  pResult->m[1][0] = sinf(radians);
  pResult->m[1][1] = cosf(radians);
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// (Hint: This function must initialize all matrix values.)
void Matrix2DRotRad(Matrix2D* pResult, float angle)
{
  // Initalizes the matrix values
  Matrix2DIdentity(pResult);

  // cos() -sin() 0
  // sin()  cos() 0
  //  0      0    1
  pResult->m[0][0] = cosf(angle);
  pResult->m[0][1] = -1.0f * sinf(angle);
  pResult->m[1][0] = sinf(angle);
  pResult->m[1][1] = cosf(angle);
}

// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
// Result = Mtx * Vec.
// (NOTE: Care must be taken when pResult = pVec.)
// (NOTE: Remember to treat the implied w component as 1.)
void Matrix2DMultVec(Vector2D* pResult, Matrix2D* pMtx, Vector2D* pVec)
{
  // Converting the vector into matrix form
  Matrix2D matrix = { 0 };
  matrix.m[0][0] = pVec->x;
  matrix.m[1][0] = pVec->y;
  matrix.m[2][0] = 1.0f;

  // Multiplying matrix by given vector
  Matrix2DConcat(&matrix, pMtx, &matrix);

  // Setting the return vector the Eucledian coordinates
  pResult->x = matrix.m[0][0];// / matrix.m[2][0];
  pResult->y = matrix.m[1][0];// / matrix.m[2][0];
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

