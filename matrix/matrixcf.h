#ifndef MATRIXCF_H
#define MATRIXCF_H

#include <pthread.h>
#include "matrix_type.h"


/****************************************************************************
*
*	CFile writen by Nicolas SOURBIER during his internship in summer 2018 
*
*	CFor any question contact me : nicolas.sourbier1@gmail.com OR 0678545220
*
*****************************************************************************/



/*	Previously defined property matrix enum (view matrix_type.h)

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
 *    Prototypes to handle matrix of Complex_f
 *
 * **************************************************/

/**
     * @brief printMatrixCF display a matrix of Complex_f (usefull for testing)
     * @param a the matrix to display
     */
void printMatrixCF(Matrix_cf *a);

/**
     * @brief matrixAllocCF allocate memory for a matrix and initialize it to zero
     * @param rows number of rows
     * @param column number of columns
     * @return the allocated matrix if no errors occured, NULL if not
     */
Matrix_cf * matrixAllocCF(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
Matrix_cf * createRandomMatrixCF(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
void createZeroMatrixCF(Matrix_cf *J);

/**
     * @brief createTestVector initialize a known vector of size 3:1
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
Matrix_cf *createTestVectorCF(unsigned int type);

/**
     * @brief createIdentityCF compute the identoty matrix
     * @param I A pointer to store the result
     */
void createIdentityCF(Matrix_cf *id);

/**
     * @brief freeMatrixCF free a  Complex_f matrix
     * @param A the matrix to free
     */
void freeMatrixCF(Matrix_cf *A);

/**
     * @brief matrixGetCF access a specified data of a matrix
     * @param A the matrix
     * @param i the index of rows
     * @param j the index on columns
     * @return the data A(i,j)
	 * @warning if the matrix in parameter is NULL, returns 0
     */
Complex_f matrixGetCF(Matrix_cf *A, unsigned int i, unsigned int j);

/**
     * @brief matrixSetCF set the value of a specified data
     * @param A the matrix
     * @param i the index on rows
     * @param j the index on columns
     * @param c the  to set at A(i,j)
     */
void matrixSetCF(Matrix_cf *A, unsigned int i, unsigned int j, const Complex_f c);

/**
     * @brief scaleMatrixCF multiply each data by a constant ()
     * @param a the matrix to scale
     * @param c the 
     * @todo Parallel programming
     */
void scaleMatrixCF(Matrix_cf *a, const Complex_f c);

/**
     * @brief addMatrixCF compute A = A+B
     * @param a  matrix
     * @param b  matrix
     * @todo Parallel programming
     */
void addMatrixCF(Matrix_cf *a, Matrix_cf *b);

/**
     * @brief addMatrixTCF compute A = A+BT
	 * BT being the transposed matrix of B
     * @param a  matrix
     * @param b  matrix
     * @todo Parallel programming
     */
void addMatrixTCF(Matrix_cf *a, Matrix_cf *b);

/**
     * @brief subMatrixCF compute A = A-B
     * @param a  matrix
     * @param b  matrix
     * @todo Parallel programming
     */
void subMatrixCF(Matrix_cf *a, Matrix_cf *b);

/**
     * @brief compute C = A*B
     * @param a  matrix
     * @param b  matrix
     * @param c a matrix to store the result
     */
void mulMatrixCF(Matrix_cf *a, Matrix_cf *b, Matrix_cf *c);

/**
     * @brief compute result = s*(mul1*mul2 + l*add)
     * @param result a matrix to store the result
     * @param mul1 & mul2 matrices to multiply
     * @param add a matrix to add to the previous product
	 * @param l a Complex_f to scale the add matrix
	 * @param s a Complex_f to scale the result
	 * doesn't use optimisations related to the matrix properties
     */
void mulAddScaleMatrixCF(Matrix_cf *result, Matrix_cf *mul1, Matrix_cf *mul2, Matrix_cf * add, Complex_f l, Complex_f s);

/**
     * @brief matNormCF take the first matrix A value as a reference (ie: 1) and scale the other value accoarding to that
     * @param A the matrix that we want to "norm"
     */
void matRefCF(Matrix_cf *A);

/**
     * @brief scaleLineCF multiply a line with a 
     * @param a the matrix
     * @param i the index of the line we want to scale
     * @param f the  that will scale the line
     */
void scaleLineCF(Matrix_cf *a, unsigned int i, Complex_f f);

/**
     * @brief subXLinesCF computes line i - f*line i2 and store it into line i
     * @param a the matrix
     * @param i the index of the line to update
     * @param i2 the index of the "reference" line
     * @param f a  to scale the matrix
     */
int subXLinesCF(Matrix_cf *a, unsigned int i, unsigned int i2, Complex_f f);

/**
     * @brief checks if the matrix is diagonal and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isDiagCF(Matrix_cf *a);

/**
     * @brief checks if the matrix is triangular UP and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriUCF(Matrix_cf *a);

/**
     * @brief checks if the matrix is triangular Low and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriLCF(Matrix_cf *a);

/**
     * @brief checks if the sparcity of the matrix is superior to 2/3 (allows compression)
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isSparseCF(Matrix_cf *a);


#endif //MATRIXCF_H
