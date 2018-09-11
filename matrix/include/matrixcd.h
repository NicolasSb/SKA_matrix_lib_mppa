#ifndef MATRIXCD_H
#define MATRIXCD_H

#include <pthread.h>
#include "matrix_type.h"


/****************************************************************************
*
*	CDile writen by Nicolas SOURBIER during his internship in summer 2018 
*
*	CDor any question contact me : nicolas.sourbier1@gmail.com OR 0678545220
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
 *    Prototypes to handle matrix of Complex_d
 *
 * **************************************************/

/**
*	\brief reads a matrix from a binary file
*	\param filename	the name of the file where the matrix is writen
*	\return the writen matrix NULL if there was an error
*/ 
Matrix_cd * readMatrix_cd_File(char * filename);


/**
*	\brief writes a matrix in a binary file
*	\param filename	the name of the file where the matrix is writen
*	\param ptr the matrix to write
*	\return 1 on success 0 otherwise
*/
int writeMatrix_cd_File(char * filename, Matrix_cd * ptr);

/**
     * @brief printMatrixCD display a matrix of Complex_d (usefull for testing)
     * @param a the matrix to display
     */
void printMatrixCD(Matrix_cd *a);

/**
     * @brief matrixAllocCD allocate memory for a matrix and initialize it to zero
     * @param rows number of rows
     * @param column number of columns
     * @return the allocated matrix if no errors occured, NULL if not
     */
Matrix_cd * matrixAllocCD(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param rows number of rows
     * @param column number of columns
     * @return the random matrix
     */
Matrix_cd * createRandomMatrixCD(unsigned int rows, unsigned int column);

/**
     * @brief createRandomMatrix initialize a random Matrix
     * @param J a matrix to fill with zeros
     * @return the random matrix
     */
void createZeroMatrixCD(Matrix_cd *J);

/**
     * @brief createTestVector initialize a known vector of size 3:1
     * @param type (1 or 2) you've got choice between two test vectors
     * @return the random matrix
     */
Matrix_cd *createTestVectorCD(unsigned int type);

/**
     * @brief createIdentityCD compute the identoty matrix
     * @param id A pointer to store the result
     */
void createIdentityCD(Matrix_cd *id);

/**
     * @brief freeMatrixCD free a  Complex_d matrix
     * @param A the matrix to free
     */
void freeMatrixCD(Matrix_cd *A);

/**
     * @brief matrixGetCD access a specified data of a matrix
     * @param A the matrix
     * @param i the index of rows
     * @param j the index on columns
     * @return the data A(i,j)
	 * @warning if the matrix in parameter is NULL, returns 0
     */
Complex_d matrixGetCD(Matrix_cd *A, unsigned int i, unsigned int j);

/**
     * @brief matrixSetCD set the value of a specified data
     * @param A the matrix
     * @param i the index on rows
     * @param j the index on columns
     * @param c the  to set at A(i,j)
     */
void matrixSetCD(Matrix_cd *A, unsigned int i, unsigned int j, const Complex_d c);

/**
     * @brief scaleMatrixCD multiply each data by a constant ()
     * @param a the matrix to scale
     * @param c the 

     */
void scaleMatrixCD(Matrix_cd *a, const Complex_d c);

/**
     * @brief addMatrixCD compute A = A+B
     * @param a  matrix
     * @param b  matrix

     */
void addMatrixCD(Matrix_cd *a, Matrix_cd *b);

/**
     * @brief addMatrixTCD compute A = A+BT
	 * BT being the transposed matrix of B
     * @param a  matrix
     * @param b  matrix

     */
void addMatrixTCD(Matrix_cd *a, Matrix_cd *b);

/**
     * @brief subMatrixCD compute A = A-B
     * @param a  matrix
     * @param b  matrix

     */
void subMatrixCD(Matrix_cd *a, Matrix_cd *b);

/**
     * @brief compute C = A*B
     * @param a  matrix
     * @param b  matrix
     * @param c a matrix to store the result
     */
void mulMatrixCD(Matrix_cd *a, Matrix_cd *b, Matrix_cd *c);

/**
     * @brief compute result = s*(mul1*mul2 + l*add)
     * @param result a matrix to store the result
     * @param mul1 & @param mul2 matrices to multiply
     * @param add a matrix to add to the previous product
	 * @param l a Complex_d to scale the add matrix
	 * @param s a Complex_d to scale the result
	 * doesn't use optimisations related to the matrix properties
     */
void mulAddScaleMatrixCD(Matrix_cd *result, Matrix_cd *mul1, Matrix_cd *mul2, Matrix_cd * add, Complex_d l, Complex_d s);

/**
     * @brief matNormCD take the first matrix A value as a reference (ie: 1) and scale the other value accoarding to that
     * @param A the matrix that we want to "norm"
     */
void matRefCD(Matrix_cd *A);

/**
     * @brief scaleLineCD multiply a line with a 
     * @param a the matrix
     * @param i the index of the line we want to scale
     * @param f the  that will scale the line
     */
void scaleLineCD(Matrix_cd *a, unsigned int i, Complex_d f);

/**
     * @brief subXLinesCD computes line i - f*line i2 and store it into line i
     * @param a the matrix
     * @param i the index of the line to update
     * @param i2 the index of the "reference" line
     * @param f a  to scale the matrix
     */
int subXLinesCD(Matrix_cd *a, unsigned int i, unsigned int i2, Complex_d f);

/**
     * @brief checks if the matrix is diagonal and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isDiagCD(Matrix_cd *a);

/**
     * @brief checks if the matrix is triangular UP and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriUCD(Matrix_cd *a);

/**
     * @brief checks if the matrix is triangular Low and sets its flag if necessary 
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isTriLCD(Matrix_cd *a);

/**
     * @brief checks if the sparcity of the matrix is superior to 2/3 (allows compression)
     * @param a the matrix to test
	 * returns 1 if diag, 0 otherwise
     */
int isSparseCD(Matrix_cd *a);


#endif //MATRIXCD_H
