// Matrix.cpp : Matrix class implementation.
//	 C++ implementation of Python code.
//   From Udacity CS 373 Artificial Intelliegence for Robots.
//   Taught by Sebastian Thrun.
//   C++ version by Tyler Folsom; March 6, 2012.

//#include "stdafx.h"  // required for MSVC. Remove for Arduino.

#include <math.h>
#include "Matrix.h"
#ifndef Abs
#define Abs(x) ((x)>=0?(x):-(x))
#endif



/*
	Code translated from Python to C++  March 2, 2012, Tyler Folsom
*/
matrix::matrix(int dim) // identity constructor
{
	rows = columns = dim<1? 1: dim;
#ifndef ARDUINO
	value = new REAL[rows*columns];
#endif
	for (int i = 0; i < rows*columns; i++)
	{
		value[i] = (i%(columns+1))? 0: 1.0F;
	}
	error = 0;
}   
matrix::matrix(int Rows, int Columns) // zero constructor
{
	columns = Columns<1? 1: Columns;
	rows = Rows<1? 1: Rows;
#ifndef ARDUINO
	value = new REAL[columns*rows];
#endif
	for (int i = 0; i < columns*rows; i++)
	   value[i] = 0;
	error = 0;
}   
/* matrix::matrix(matrix& Value) // constructor from a matix
{
   rows = Value.rows;
   columns = Value.columns;
   value = new REAL[rows*columns];
   for (int i = 0; i < rows*columns; i++)
   {
	   value[i] = Value.value[i];
   }
	error = 0;
} */
matrix::matrix(int Rows, int Columns, REAL* values) // contruct from list of values
{
	columns = Columns<1? 1: Columns;
	rows = Rows<1? 1: Rows;
#ifndef ARDUINO
	value = new REAL[columns*rows];
#endif
	for (int i = 0; i < columns*rows; i++)
	{
	   value[i] = values[i];
	}
	error = 0;
}
void matrix::values(REAL* dest)
{
	for (int i = 0; i < columns*rows; i++)
	{
	   dest[i] = value[i];
	}
}
void matrix::show()
{
	int i;
	for (i = 0; i < rows*columns; i++)
	{
	   if (i%columns == 0)
		   Show("\n");
           Show(value[i]);
//	   printf("%.6f, ",value[i]);
	}
   if (i%columns == 0)
	   Show("\n");
}
 
matrix matrix::__add__(matrix& other)
{
	matrix res(rows, columns);
    // check if correct dimensions
    if (columns != other.columns || rows != other.rows)
	{
        Show( "Matrices must be of equal dimensions to add\n");
		res.error = 1;
	}
    else
	{
        // add if correct dimensions
	   for (int i = 0; i < columns*rows; i++)
	   {
		   res.value[i] = value[i] + other.value[i];
	   }
	}
    return res;
}
matrix matrix::__sub__(matrix& other)
{
	matrix res(rows, columns);
    // check if correct dimensions
    if (columns != other.columns || rows != other.rows)
	{
        Show( "Matrices must be of equal dimensions to subtract\n");
		res.error = 2;
	}
    else
	{
        // add if correct dimensions
	   for (int i = 0; i < columns*rows; i++)
	   {
		   res.value[i] = value[i] - other.value[i];
	   }
	}
    return res;
}
matrix matrix::operator+(matrix& right)
{
	return __add__(right);
}
matrix matrix::operator-(matrix& right)
{
	return __sub__(right);
}
matrix matrix::operator*(matrix& right)
{
	return __mul__(right);
}
REAL matrix::element(int i, int j)
{
	return value[i*columns+j];
}
REAL* matrix::operator[](int i)
{
	return &value[i*rows];
}
matrix matrix::transpose()
{
   // compute transpose
	int ij = 0;
	matrix res(columns, rows);
    for (int i = 0; i < columns; i++)
	{
        for(int j = 0; j < rows; j++)
		{
            res.value[ij++] = this->element(j,i);
		}
	}
    return res;
}
       
matrix matrix::__mul__(matrix& other)
{
	int ij = 0;
	int ik = 0;
	int kj = 0;
	matrix res(rows, other.columns);
    // check if correct dimensions
	if (columns != other.rows)
	{
		Show( "Matrices must be m*n and n*p to multiply\n");
		res.error = 3;
	}
	else
	{
/*  [m x n] * [n x p]
    for i in range(self.dimx):  # rows = m
        for j in range(other.dimy): # columns = p
            for k in range(self.dimy): # common dimension = n
                res.value[i][j] += self.value[i][k] * other.value[k][j]
*/
		// multiply if correct dimensions
		for( int j = 0; j < rows; j++)
		{
			for (int i = 0; i < other.columns; i++,ij++)
			{
				ik = j*columns;
				kj = i;
				for (int k = 0; k <columns; k++)
				{
					res.value[ij] += value[ik++] * other.value[kj];
					kj += other.columns;
				}
			}
		}
	}            
	return res;
}
void matrix::operator=(matrix& right)
{
	rows = right.rows;
	columns = right.columns;
	error = right.error;
	for (int i = 0; i < rows*columns; i++)
		value[i] = right.value[i];
	return;
}
bool matrix::equal(matrix& other, REAL tolerance)	
{
	if (rows != other.rows || columns != other.columns)
		return false;
	for (int i = 0; i < rows*columns; i++)
		if (Abs(value[i] - other.value[i]) > tolerance)
			return false;
	return true;
}
bool matrix::operator==(matrix& right)
{
	return equal(right);
}
bool matrix::operator!=(matrix& right)
{
	return !equal(right);
}
bool matrix::symmetric(REAL tolerance)
{
	if (columns != rows)
		return false;
	for (int i = 1; i < columns; i++)
		for (int j = i; j < rows; j++)
			if (Abs(element(i,j) - element(j,i)) > tolerance)
				return false;
	return true;
}
  
    // Thanks to Ernesto P. Adorio for use of Cholesky and CholeskyInverse functions
matrix matrix::Cholesky(REAL ztol)
{
    // Computes the upper triangular Cholesky factorization of
    // a positive definite matrix.
	REAL S, d;
	int i, j, k;
	matrix res(rows, rows);
	if (!symmetric())
	{
		Show( "Matrix not symmetric\n");
		res.error = 5;
		return res;
	}   
    for (i=0; i<rows; i++)
	{
		S = 0;
		for (k=0; k<=i; k++)
		{
			S += res.value[k*rows+i] * res.value[k*rows+i];
		}
        d = value[i*rows+i] - S;
        if (Abs(d) < ztol)
            res.value[i*rows+i] = 0;
        else
		{
            if (d < 0.0)
			{
                Show( "Matrix not positive-definite\n");
				res.error = 4;
				return res;
			}
			res.value[i*rows+i] = sqrt(d);
		}
        for (j = i+1; j < rows; j++)
		{
			S = 0;
			for (k = 0; k < rows; k++)
			{
				S += res.value[k*rows+i] * res.value[k*rows+j];
			}
            if (Abs(S) < ztol)
                S = 0.0;
            res.value[i*rows+j] = (value[i*rows+j] - S) / res.value[i*rows+i];
		}
	}
//	res.show();  // triangular matrix
    return res;
} 
matrix matrix::CholeskyInverse()
{
    // Computes inverse of matrix given its Cholesky upper Triangular
    // decomposition of matrix.
	int i, j, k;
	REAL tjj;
	REAL S = 0;
	REAL Sum = 0;
	matrix res(rows, rows);
    
    // Backward step for inverse.
    for (j = rows-1; j >= 0; j--) 
	{
        tjj = value[j*rows + j];
		S = 0;
		for (k = j+1; k < rows; k++)
		{
			S += value[j*rows+k] * res.value[j*rows+k];
		}
        res.value[j*rows+j] = (1.0F/tjj - S)/tjj;
        for( i = j-1; i >= 0; i--)
		{
			Sum = 0;
			for (k = i+1; k < rows; k++)
				Sum += value[i*rows+k] * res.value[k*rows+j];
			res.value[j*rows+i] =
			res.value[i*rows+j] = -Sum / value[i*rows+i];
		}
	}   
	return res;
}
 
matrix matrix::inverse()
{
    if (rows == 1 && columns == 1)  // common for KF
    {
        REAL reciprocal = value[0];
        matrix single = matrix(1,1,&reciprocal);
        if (reciprocal == 0)
            single.error = 4;  // not positive definite
        else
            single.value[0] = 1/reciprocal;
        return single;
    }
// Cholesky method only works for square, symmetric, positive definite matrices.
//	show();  // matrix to be inverted
    matrix aux = Cholesky();
// Sometimes a problem with MSVC: Correct computation of triangular matrix,
// but returned matrix is garbage. (?)  TCF March/6/2012
//	aux.show();  // Triangular matrix
   if (aux.error)
   {
      return aux;
   }
   else
      return aux.CholeskyInverse();
}
matrix matrix::operator/(matrix& right)
{
	matrix aux = right.inverse();
	if (aux.error)
		return aux;
	return __mul__(aux);
}

