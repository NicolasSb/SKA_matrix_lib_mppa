#ifndef MATRIXF_H
#define MATRIXF_H

#include <pthread.h>
#include "matrix_type.h"


/****************************************************************************
*
*	File writen by Nicolas SOURBIER during his internship in summer 2018 
*
*	For any question contact me : nicolas.sourbier1@gmail.com OR 0678545220
*
*****************************************************************************/



/**	Previously defined property matrix enum (view matrix_type.h)

typedef enum M_Property
{
	M_DIAG,
	M_TRI_U,
	M_TRI_L,
	M_SPARSE,
	M_DENSE
}M_Property;
*/


/*****************************************************
 *
 *    Prototypes to handle matrix of float
 *
 * **************************************************/

/**
     * @brief printMatrixF display a matrix of floats (usefull for testing)
     * @param a the matrix to display
     */
void printMatrixF(Matrix_f *a);

/**
     * @brief matrixAllocF allocate memory for a matrix and initialize it to zero
     * @param rows number of rows
     * @param column number of columns
     * @return the allocated matrix if no errors occured, NULL if not
     */
Matrix_f * matrixAllocF(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
Matrix_f * createRandomMatrixF(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
void createZeroMatrixF(Matrix_f *J);

/**
     * @brief createTestVector initialize a known vector of size 3:1
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
Matrix_f *createTestVectorF(unsigned int type);

/**
     * @brief createIdentityF compute the identoty matrix
     * @param I A pointer to store the result
     */
void createIdentityF(Matrix_f *);

/**
     * @brief freeMatrixF free a  float matrix
     * @param A the matrix to free
     */
void freeMatrixF(Matrix_f *A);

/**
     * @brief matrixGetF access a specified data of a matrix
     * @param A the matrix
     * @param i the index of rows
     * @param j the index on columns
     * @return the data A(i,j)
	 * @warning if the matrix in parameter is NULL, returns 0
     */
float matrixGetF(Matrix_f *A, unsigned int i, unsigned int j);

/**
     * @brief matrixSetF set the value of a specified data
     * @param A the matrix
     * @param i the index on rows
     * @param j the index on columns
     * @param c the  to set at A(i,j)
     */
void matrixSetF(Matrix_f *A, unsigned int i, unsigned int j, const float c);

/**
     * @brief scaleMatrixF multiply each data by a constant ()
     * @param a the matrix to scale
     * @param c the 
     */
void scaleMatrixF(Matrix_f *a, const float c);

/**
     * @brief addMatrixF compute A = A+B
     * @param a  matrix
     * @param b  matrix
     */
void addMatrixF(Matrix_f *a, Matrix_f *b);

/**
     * @brief addMatrixTF compute A = A+BT
	 * BT being the transposed matrix of B
     * @param a  matrix
     * @param b  matrix
     */
void addMatrixTF(Matrix_f *a, Matrix_f *b);

/**
     * @brief subMatrixF compute A = A-B
     * @param a  matrix
     * @param b  matrix
     */
void subMatrixF(Matrix_f *a, Matrix_f *b);

/**
     * @brief compute C = A*B
     * @param a  matrix
     * @param b  matrix
     * @param c a matrix to store the result
     */
void mulMatrixF(Matrix_f *a, Matrix_f *b, Matrix_f *c);

/**
     * @brief compute result = s*(mul1*mul2 + l*add)
     * @param result a matrix to store the result
     * @param mul1 & mul2 matrices to multiply
     * @param add a matrix to add to the previous product
	 * @param l a float to scale the add matrix
	 * @param s a float to scale the result
	 * doesn't use optimisations related to the matrix properties
     */
void mulAddScaleMatrixF(Matrix_f *result, Matrix_f *mul1, Matrix_f *mul2, Matrix_f * add, float l, float s);

/**
     * @brief matNormF take the first matrix A value as a reference (ie: 1) and scale the other value accoarding to that
     * @param A the matrix that we want to "norm"
     */
void matRefF(Matrix_f *A);

/**
     * @brief scaleLineF multiply a line with a 
     * @param a the matrix
     * @param i the index of the line we want to scale
     * @param f the  that will scale the line
     */
void scaleLineF(Matrix_f *a, unsigned int i, float f);

/**
     * @brief subXLinesF computes line i - f*line i2 and store it into line i
     * @param a the matrix
     * @param i the index of the line to update
     * @param i2 the index of the "reference" line
     * @param f a  to scale the matrix
     */
int subXLinesF(Matrix_f *a, unsigned int i, unsigned int i2, float f);

/**
     * @brief checks if the matrix is diagonal and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isDiagF(Matrix_f *a);

/**
     * @brief checks if the matrix is triangular UP and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriUF(Matrix_f *a);

/**
     * @brief checks if the matrix is triangular Low and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriLF(Matrix_f *a);

/**
     * @brief checks if the sparcity of the matrix is superior to 2/3 (allows compression)
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isSparseF(Matrix_f *a);


#endif //MATRIXF_H
