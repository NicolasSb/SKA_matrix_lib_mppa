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


typedef struct Matrix_f //done
{
	M_data_type data_type;
	unsigned int row;
	unsigned int column;
	float * data;
	M_Property prop;
}Matrix_f;

typedef struct Matrix_d //done
{
	M_data_type data_type;
	unsigned int row;
	unsigned int column;
	double * data;
	M_Property prop;
}Matrix_d;

typedef struct Matrix_i
{
	M_data_type data_type;
	unsigned int row;
	unsigned int column;
	int * data;
	M_Property prop;
}Matrix_i;

typedef struct Matrix_cf //done
{
	M_data_type data_type;
	unsigned int row;
	unsigned int column;
	Complex_f * data;
	M_Property prop;
}Matrix_cf;

typedef struct Matrix_cd //done
{
	M_data_type data_type;
	unsigned int row;
	unsigned int column;
	Complex_d * data;
	M_Property prop;
}Matrix_cd;



#endif //MATRIX_H
