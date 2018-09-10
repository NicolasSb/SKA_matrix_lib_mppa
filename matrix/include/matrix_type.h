#ifndef MATRIX_H
#define MATRIX_H

#include "complex_f.h"
#include "complex_d.h"


/****************************************************************************
*
*	File writen by Nicolas SOURBIER during his internship in summer 2018 
*
*	For any question contact me : nicolas.sourbier1@gmail.com OR 0678545220
*
*****************************************************************************/

typedef enum M_data_type
{
	M_INT,
	M_FLOAT,
	M_DOUBLE,
	M_COMPLEX_F,
	M_COMPLEX_D
}M_data_type;


typedef enum M_Property
{
	M_DIAG,
	M_TRI_U,
	M_TRI_L,
	M_SPARSE,
	M_DENSE
}M_Property;

/**
*	\struct Matrix_f matrix_type.h
*	\brief floating point matrix type
*/
typedef struct Matrix_f //done
{
	M_data_type data_type;  /*!< type of data stored in the matrix - used for verification */
	unsigned int row;		/*!< number of rows of the matrix*/
	unsigned int column;	/*!< number of columns of the matrix*/
	float * data;			/*!< array containing the data*/
	M_Property prop;		/*!< property of the matrix (for optimizations)*/
}Matrix_f;

/**
*	\struct Matrix_d matrix_type.h
*	\brief double precision matrix type
*/
typedef struct Matrix_d //done
{
	M_data_type data_type;	/*!< type of data stored in the matrix - used for verification */
	unsigned int row;		/*!< number of rows of the matrix*/
	unsigned int column;	/*!< number of columns of the matrix*/
	double * data;			/*!< array containing the data*/
	M_Property prop;		/*!< property of the matrix (for optimizations)*/
}Matrix_d;


/**
*	\struct Matrix_i matrix_type.h
*	\brief integer matrix type
*/
typedef struct Matrix_i
{
	M_data_type data_type;	/*!< type of data stored in the matrix - used for verification */
	unsigned int row;		/*!< number of rows of the matrix*/
	unsigned int column;	/*!< number of columns of the matrix*/
	int * data;				/*!< array containing the data*/
	M_Property prop;		/*!< property of the matrix (for optimizations)*/
}Matrix_i;


/**
*	\struct Matrix_cf matrix_type.h
*	\brief complex float matrix type
*/
typedef struct Matrix_cf //done
{
	M_data_type data_type;	/*!< type of data stored in the matrix - used for verification */
	unsigned int row;		/*!< number of rows of the matrix*/
	unsigned int column;	/*!< number of columns of the matrix*/
	Complex_f * data;		/*!< array containing the data*/
	M_Property prop;		/*!< property of the matrix (for optimizations)*/
}Matrix_cf;


/**
*	\struct Matrix_cd matrix_type.h
*	\brief complex double matrix type
*/
typedef struct Matrix_cd //done
{
	M_data_type data_type;	/*!< type of data stored in the matrix - used for verification */
	unsigned int row;		/*!< number of rows of the matrix*/
	unsigned int column;	/*!< number of columns of the matrix*/
	Complex_d * data;		/*!< array containing the data*/
	M_Property prop;		/*!< property of the matrix (for optimizations)*/
}Matrix_cd;



#endif //MATRIX_H
