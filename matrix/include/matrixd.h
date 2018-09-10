#ifndef MATRIXD_H
#define MATRIXD_H

#include <pthread.h>
#include "matrix_type.h"


/****************************************************************************
*
*	Dile writen by Nicolas SOURBIER during his internship in summer 2018 
*
*	Dor any question contact me : nicolas.sourbier1@gmail.com OR 0678545220
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
 *    Prototypes to handle matrix of double
 *
 * **************************************************/

/**
     * @brief printMatrixD display a matrix of doubles (usefull for testing)
     * @param a the matrix to display
     */
void printMatrixD(Matrix_d *a);

/**
     * @brief matrixAllocD allocate memory for a matrix and initialize it to zero
     * @param rows number of rows
     * @param column number of columns
     * @return the allocated matrix if no errors occured, NULL if not
     */
Matrix_d * matrixAllocD(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
Matrix_d * createRandomMatrixD(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param J a matrix to fill with zeros
     * @return the random matrix
     */
void createZeroMatrixD(Matrix_d *J);

/**
     * @brief createTestVector initialize a known vector of size 3:1
     * @param type (1 or 2) you can choose between two matrix
     * @return the random matrix
     */
Matrix_d *createTestVectorD(unsigned int type);

/**
     * @brief createIdentityD compute the identoty matrix
     * @param id A pointer to store the result
     */
void createIdentityD(Matrix_d *id);

/**
     * @brief freeMatrixD free a  double matrix
     * @param A the matrix to free
     */
void freeMatrixD(Matrix_d *A);

/**
     * @brief matrixGetD access a specified data of a matrix
     * @param A the matrix
     * @param i the index of rows
     * @param j the index on columns
     * @return the data A(i,j)
	 * @warning if the matrix in parameter is NULL, returns 0
     */
double matrixGetD(Matrix_d *A, unsigned int i, unsigned int j);

/**
     * @brief matrixSetD set the value of a specified data
     * @param A the matrix
     * @param i the index on rows
     * @param j the index on columns
     * @param c the  to set at A(i,j)
     */
void matrixSetD(Matrix_d *A, unsigned int i, unsigned int j, const double c);

/**
     * @brief scaleMatrixD multiply each data by a constant ()
     * @param a the matrix to scale
     * @param c the 

     */
void scaleMatrixD(Matrix_d *a, const double c);

/**
     * @brief addMatrixD compute A = A+B
     * @param a  matrix
     * @param b  matrix

     */
void addMatrixD(Matrix_d *a, Matrix_d *b);

/**
     * @brief addMatrixTD compute A = A+BT
	 * BT being the transposed matrix of B
     * @param a  matrix
     * @param b  matrix

     */
void addMatrixTD(Matrix_d *a, Matrix_d *b);

/**
     * @brief subMatrixD compute A = A-B
     * @param a  matrix
     * @param b  matrix

     */
void subMatrixD(Matrix_d *a, Matrix_d *b);

/**
     * @brief compute C = A*B
     * @param a  matrix
     * @param b  matrix
     * @param c a matrix to store the result
     */
void mulMatrixD(Matrix_d *a, Matrix_d *b, Matrix_d *c);

/**
     * @brief compute result = s*(mul1*mul2 + l*add)
     * @param result a matrix to store the result
     * @param mul1 & @param mul2 matrices to multiply
     * @param add a matrix to add to the previous product
	 * @param l a double to scale the add matrix
	 * @param s a double to scale the result
	 * doesn't use optimisations related to the matrix properties
     */
void mulAddScaleMatrixD(Matrix_d *result, Matrix_d *mul1, Matrix_d *mul2, Matrix_d * add, double l, double s);

/**
     * @brief matNormD take the first matrix A value as a reference (ie: 1) and scale the other value accoarding to that
     * @param A the matrix that we want to "norm"
     */
void matRefD(Matrix_d *A);

/**
     * @brief scaleLineD multiply a line with a 
     * @param a the matrix
     * @param i the index of the line we want to scale
     * @param f the  that will scale the line
     */
void scaleLineD(Matrix_d *a, unsigned int i, double f);

/**
     * @brief subXLinesD computes line i - f*line i2 and store it into line i
     * @param a the matrix
     * @param i the index of the line to update
     * @param i2 the index of the "reference" line
     * @param f a  to scale the matrix
     */
int subXLinesD(Matrix_d *a, unsigned int i, unsigned int i2, double f);

/**
     * @brief checks if the matrix is diagonal and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isDiagD(Matrix_d *a);

/**
     * @brief checks if the matrix is triangular UP and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriUD(Matrix_d *a);

/**
     * @brief checks if the matrix is triangular Low and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriLD(Matrix_d *a);

/**
     * @brief checks if the sparcity of the matrix is superior to 2/3 (allows compression)
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isSparseD(Matrix_d *a);


#endif //MATRIXD_H
