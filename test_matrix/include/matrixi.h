#ifndef MATRIXI_H
#define MATRIXI_H

#include <pthread.h>
#include "matrix_type.h"


/****************************************************************************
*
*	Iile writen by Nicolas SOURBIER during his internship in summer 2018 
*
*	Ior any question contact me : nicolas.sourbier1@gmail.com OR 0678545220
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
 *    Prototypes to handle matrix of int
 *
 * **************************************************/

/**
*	\brief reads a matrix from a binary file
*	\param filename	the name of the file where the matrix is writen
*	\return the writen matrix NULL if there was an error
*/
Matrix_i * readMatrix_i_File(char * filename);


/**
*	\brief writes a matrix in a binary file
*	\param filename	the name of the file where the matrix is writen
*	\param ptr the matrix to write
*	\return 1 on success 0 otherwise
*/
int writeMatrix_i_File(char * filename, Matrix_i * ptr);

/**
     * @brief printMatrixI display a matrix of ints (usefull for testing)
     * @param a the matrix to display
     */
void printMatrixI(Matrix_i *a);

/**
     * @brief matrixAllocI allocate memory for a matrix and initialize it to zero
     * @param rows number of rows
     * @param column number of columns
     * @return the allocated matrix if no errors occured, NULL if not
     */
Matrix_i * matrixAllocI(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
Matrix_i * createRandomMatrixI(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
void createZeroMatrixI(Matrix_i *J);

/**
     * @brief createTestVector initialize a known vector of size 3:1
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
Matrix_i *createTestVectorI(unsigned int type);

/**
     * @brief createIdentityI compute the identoty matrix
     * @param I A pointer to store the result
     */
void createIdentityI(Matrix_i *);

/**
     * @brief freeMatrixI free a  int matrix
     * @param A the matrix to free
     */
void freeMatrixI(Matrix_i *A);

/**
     * @brief matrixGetI access a specified data of a matrix
     * @param A the matrix
     * @param i the index of rows
     * @param j the index on columns
     * @return the data A(i,j)
	 * @warning if the matrix in parameter is NULL, returns 0
     */
int matrixGetI(Matrix_i *A, unsigned int i, unsigned int j);

/**
     * @brief matrixSetI set the value of a specified data
     * @param A the matrix
     * @param i the index on rows
     * @param j the index on columns
     * @param c the  to set at A(i,j)
     */
void matrixSetI(Matrix_i *A, unsigned int i, unsigned int j, const int c);

/**
     * @brief scaleMatrixI multiply each data by a constant ()
     * @param a the matrix to scale
     * @param c the 
     * @todo Parallel programming
     */
void scaleMatrixI(Matrix_i *a, const int c);

/**
     * @brief addMatrixI compute A = A+B
     * @param a  matrix
     * @param b  matrix
     * @todo Parallel programming
     */
void addMatrixI(Matrix_i *a, Matrix_i *b);

/**
     * @brief addMatrixTI compute A = A+BT
	 * BT being the transposed matrix of B
     * @param a  matrix
     * @param b  matrix
     * @todo Parallel programming
     */
void addMatrixTI(Matrix_i *a, Matrix_i *b);

/**
     * @brief subMatrixI compute A = A-B
     * @param a  matrix
     * @param b  matrix
     * @todo Parallel programming
     */
void subMatrixI(Matrix_i *a, Matrix_i *b);

/**
     * @brief compute C = A*B
     * @param a  matrix
     * @param b  matrix
     * @param c a matrix to store the result
     */
void mulMatrixI(Matrix_i *a, Matrix_i *b, Matrix_i *c);

/**
     * @brief compute result = s*(mul1*mul2 + l*add)
     * @param result a matrix to store the result
     * @param mul1 & mul2 matrices to multiply
     * @param add a matrix to add to the previous product
	 * @param l a int to scale the add matrix
	 * @param s a int to scale the result
	 * doesn't use optimisations related to the matrix properties
     */
void mulAddScaleMatrixI(Matrix_i *result, Matrix_i *mul1, Matrix_i *mul2, Matrix_i * add, int l, int s);

/**
     * @brief matNormI take the first matrix A value as a reference (ie: 1) and scale the other value accoarding to that
     * @param A the matrix that we want to "norm"
     */
void matRefI(Matrix_i *A);

/**
     * @brief scaleLineI multiply a line with a 
     * @param a the matrix
     * @param i the index of the line we want to scale
     * @param f the  that will scale the line
     */
void scaleLineI(Matrix_i *a, unsigned int i, int f);

/**
     * @brief subXLinesI computes line i - f*line i2 and store it into line i
     * @param a the matrix
     * @param i the index of the line to update
     * @param i2 the index of the "reference" line
     * @param f a  to scale the matrix
     */
int subXLinesI(Matrix_i *a, unsigned int i, unsigned int i2, int f);

/**
     * @brief checks if the matrix is diagonal and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isDiagI(Matrix_i *a);

/**
     * @brief checks if the matrix is triangular UP and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriUI(Matrix_i *a);

/**
     * @brief checks if the matrix is triangular Low and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriLI(Matrix_i *a);

/**
     * @brief checks if the sparcity of the matrix is superior to 2/3 (allows compression)
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isSparseI(Matrix_i *a);


#endif //MATRIXI_H
