#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/matrixd.h"
#include "errno.h"
#include <omp.h>  

#ifdef __k1__
    #include <mppa_power.h>
    #include <mppa_routing.h>
    #include <mppa_remote.h>
    #include <mppa_rpc.h>
    #include <mppa_async.h>
    #include <assert.h>
    #include <utask.h>// for pthread_barrier_t
    #include <HAL/hal/board/boot_args.h>
#endif

/**
 * @file matrixd.c to handle matrix operations for doubles
 * @author Nicolas Sourbier
 * @date 10/09/2018
 **/


Matrix_d * readMatrix_d_File(char * filename)
{
	if(!filename)
		return NULL;
	FILE * f = fopen(filename, "rb+");
	if(!f)
	{
		return NULL;
	}

	M_data_type dtype;
	fread(&dtype, sizeof(M_data_type),1,f);
	if(dtype != M_DOUBLE)
	{
		return NULL;
	}
	unsigned int row, column;
	fread(&row, sizeof(unsigned int),1,f);
	fread(&column, sizeof(unsigned int),1,f);

	Matrix_d * mat = matrixAllocD(row,column);
	
	if(mat == NULL)
		return NULL;

	fread(mat->data, sizeof(double), row*column, f);
	fread(&mat->prop, sizeof(M_Property), 1, f);

	if (!fclose(f))
		return mat;
	return NULL;
}



int writeMatrix_d_File(char * filename, Matrix_d * ptr)
{
	if(!filename || !ptr)
		return 0;

	FILE * f = fopen(filename, "wb+");
	if(!f)
	{
		return 0;
	}
	
	fwrite (&ptr->data_type, sizeof(M_data_type), 1, f);
	fwrite (&ptr->row, sizeof(unsigned int), 1, f);
	fwrite (&ptr->column, sizeof(unsigned int), 1, f);
	unsigned int i;
	for(i = 0; i < ptr->row*ptr->column; i++)
		fwrite (&ptr->data[i], sizeof(double), 1, f);

	fwrite (&ptr->prop, sizeof(M_Property), 1, f);

	if (!fclose(f))
		return 0;
	return 1;
}

void printMatrixD(Matrix_d *a)
{
    if(a)
    {
        unsigned int i, j;
		printf("matrix of doubles [%d, %d]\n", a->row, a->column);
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                printf("%f",matrixGetD(a, i, j));
                printf("\t");
            }
            printf("\n");
        }
    }
}

Matrix_d * matrixAllocD(unsigned int rows, unsigned int column)
{
    Matrix_d *A = (Matrix_d *) malloc(sizeof(Matrix_d));
    double *c = (double *)calloc(rows*column, sizeof(double));
    if (!A || !c)
    {
        fprintf(stderr,"Matrix Error \n in file: %s \n function %s\n line : %d\n %s \n", __FILE__,__FUNCTION__, __LINE__, strerror(errno));
        return NULL;
    }
	A->data_type = M_DOUBLE;
    A->column = column;
    A->row = rows;
    A->data = c;
    createZeroMatrixD(A);
	A->prop = M_SPARSE;
    return A;
}

Matrix_d * createRandomMatrixD(unsigned int rows, unsigned int column)
{
    Matrix_d *res = matrixAllocD(rows, column);
    unsigned int i,j;
	#pragma omp parallel for private(i,j) shared(res)
    for (i=0; i<res->row; i++)
    {
        for(j=0; j<res->column; j++)
        {
            res->data[i*res->column+j] = (double) (rand()/RAND_MAX);
        }
    }
    return res;
}

void createZeroMatrixD(Matrix_d *J)
{
    if(J)
    {
        unsigned int i,j;
		#pragma omp parallel for private(i,j) shared(J)
        for (i=0; i<J->row; i++)
        {
            for(j=0; j<J->column; j++)
            {
                J->data[i*J->column+j] = 0.f;
            }
        }
    }
}

Matrix_d *createTestVectorD(unsigned int type)
{
    Matrix_d *res = matrixAllocD(3, 1);
    double tmp;
	if(type == 1)
	{
		tmp = 0.19078;
		matrixSetD(res, 0,0,tmp);
		tmp = 0.21209;
		matrixSetD(res,1,0,tmp);
		tmp = 0.30087;
		matrixSetD(res,2,0,tmp);
	}	
	else if (type == 2)
	{
		tmp = 0.48679;
		matrixSetD(res, 0,0,tmp);
		tmp = 0.11108;
		matrixSetD(res,1,0,tmp);
		tmp = 0.61440;
		matrixSetD(res,2,0,tmp);
	} 
    return res;
}

void createIdentityD(Matrix_d *I)
{
    if(I && I->column==I->row)
    {
        unsigned int i;
		#pragma omp parallel for private(i) shared(I)
        for (i=0; i<I->row; i++)
        {
            I->data[i*I->column+i] = 1.f;
        }
		I->prop = M_DIAG;
    }
}

void freeMatrixD(Matrix_d *A)
{
    if(A)
    {
		if(A->data) {
			free(A->data);
			A->data = NULL;
		}
        free(A);
		A = NULL;
    }
}

double matrixGetD(Matrix_d *A, unsigned int i, unsigned int j)
{
    if(A)
        return  A->data[i*A->column+j];
    return 0;
}

void matrixSetD(Matrix_d *A, unsigned int i, unsigned int j, const double c)
{
    if(A)
        A->data[i*A->column+j] = c;
}

void scaleMatrixD(Matrix_d *a, const double c)
{
    if(a)
    {
        unsigned int i, j;
        double tmp;
		#pragma omp parallel for private(i,j) shared(a)
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                tmp = a->data[i*a->column+j];
                a->data[i*a->column+j] = tmp*c;
            }
        }
    }
}

void addMatrixD(Matrix_d *a, Matrix_d *b)
{
    unsigned int i, j;
    double tmpa, tmpb;
	#pragma omp parallel for private(i,j) shared(a,b)
    for (i=0; i<a->row; i++)
    {
        for(j=0; j<a->column; j++)
        {
            tmpa = a->data[i*a->column+j];
            tmpb = b->data[i*b->column+j];
            a->data[i*a->column+j] = tmpa+tmpb;
        }
    }
}

void addMatrixTD(Matrix_d *a, Matrix_d *b)
{
    if ((a->column==b->row) && (a->row==b->column))
    {
        unsigned int i, j;
        double tmpa, tmpb;
		#pragma omp parallel for private(i,j) shared(a,b)
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                tmpa = a->data[i*a->column+j];
                tmpb = b->data[j*b->column+i];
                a->data[i*a->column+j] = tmpa+tmpb;
            }
        }
    }
}

void subMatrixD(Matrix_d *a, Matrix_d *b)
{
    if (a && b)
    {
        if ((a->column==b->column) && (a->row==b->row))
        {
            unsigned int i, j;
            double tmpa, tmpb;
			#pragma omp parallel for private(i,j) shared(a,b)
            for (i=0; i<a->row; i++)
            {
                for(j=0; j<a->column; j++)
                {
                    tmpa = a->data[i*a->column+j];
                    tmpb = b->data[i*b->column+j];
                    a->data[i*a->column+j] = tmpa-tmpb;
                }
            }
        }
    }
}

void mulMatrixD(Matrix_d *a, Matrix_d *b, Matrix_d *c)
{
	if( a && b && c)
	{
		unsigned int i, j, k;
		double tmp = 0.f;
		#pragma omp parallel for private(i,j,k) shared(a,b,c)
		for (i = 0; i< a->row; i++)
		{	
			for(j = 0; j< b->column; j++)
			{
				for(k = 0; k < a->column; k++)
				{
					tmp += a->data[i*a->column+k]*b->data[k*b->column+j];
				}
				c->data[i*c->column+j] =tmp;
				tmp = 0;
			}
		}
	}
}

void matRefD(Matrix_d *A)
{
    if (A)
    {
        double c = A->data[0];
        scaleMatrixD(A, 1/c);
    }
}

void scaleLineD(Matrix_d *a, unsigned int i, double f)
{
    if(a)
    {
        if(i<a->row)
        {
            double c;
            unsigned int j;
			#pragma omp parallel for private(j) shared(a)
            for(j=0; j<a->column; j++)
            {
                c = f*a->data[i*a->column+j];
                a->data[i*a->column+j] = c;
            }
        }
    }
}

int subXLinesD(Matrix_d *a, unsigned int i, unsigned int i2, double f)
{
    int ret = 1;
    if(a)
    {
        if(i<a->row)
        {
            double c,tmp, tmp1;
            unsigned int j;
			#pragma omp parallel for private(j) shared(a)
            for(j=0; j<a->column; j++)
            {
                tmp = a->data[i*a->column+j];
                tmp1 = a->data[i2*a->column+j];
                tmp1 = f * tmp1;
                c = tmp - tmp1;
                a->data[i*a->column+j] = c;
                if(c >1e-6)
                {
                    ret = 0;
                }
            }
        }
    }
    return ret;
}

void mulAddScaleMatrixD(Matrix_d *result, Matrix_d *mul1, Matrix_d *mul2, Matrix_d * add, double l, double s)
{
	unsigned int i, j, k;
	double tmp = 0.f;
	if(result && mul1 && mul2 && !add)
	{
		#pragma omp parallel for private(i,j,k) shared(mul1,mul2,result)
		for (i = 0; i< mul1->row; i++)
		{
			for(j = 0; j< mul2->column; j++)
			{
				for(k = 0; k < mul1->column; k++)
				{
					tmp += mul1->data[i*mul1->column+k]*mul2->data[k*mul2->column+j];
				}
				tmp = s*tmp;
				result->data[i*result->column+j] = tmp;
				tmp = 0;
			}
		}
	}
	if(result && mul1 && mul2)
	{
		#pragma omp parallel for private(i,j,k) shared(mul1,mul2,result,add)
		for (i = 0; i< mul1->row; i++)
		{
			for(j = 0; j< mul2->column; j++)
			{
				for(k = 0; k < mul1->column; k++)
				{
					tmp += mul1->data[i*mul1->column+k]*mul2->data[k*mul2->column+j];
				}
				tmp = s*(tmp+ l*add->data[i*add->column+j]);
				result->data[i*result->column+j] = tmp;
				tmp = 0;
			}
		}
	}
}

static int is_null(double a, double precision)
{
	return (a<precision && a>(-precision));
}

int isDiagD(Matrix_d *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i!=j && !is_null(a->data[i*a->column+j], 10e-9))
				return 0;
		}
	}
	a->prop = M_DIAG;
	return 1;
}

int isTriUD(Matrix_d *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i<=j)
				continue;
			if(!is_null(a->data[i*a->column+j], 10e-9))
				return 0;
		}
	}
	a->prop = M_TRI_U;
	return 1;
}

int isTriLD(Matrix_d *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i>=j)
				continue;
			if(!is_null(a->data[i*a->column+j], 10e-9))
				return 0;
		}
	}
	a->prop = M_TRI_L;
	return 1;
}

int isSparseD(Matrix_d *a)
{
	unsigned int i, j;
	double count = 0.;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(is_null(a->data[i*a->column+j], 10e-9))
				++count;
		}
	}
	if((count/(a->row*a->column)) >= 0.67)
	{
		a->prop = M_SPARSE;
		return 1;
	}
	return 0;
}

