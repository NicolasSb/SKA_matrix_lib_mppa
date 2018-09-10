#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/matrixi.h"
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
 * @file matrixd.c to handle matrix operations for ints
 * @author Nicolas Sourbier
 * @date 10/09/2018
 **/

void printMatrixI(Matrix_i *a)
{
    if(a)
    {
        unsigned int i, j;
		printf("matrix of ints [%d, %d]\n", a->row, a->column);
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                printf("%f",matrixGetI(a, i, j));
                printf("\t");
            }
            printf("\n");
        }
    }
}

Matrix_i * matrixAllocI(unsigned int rows, unsigned int column)
{
    Matrix_i *A = (Matrix_i *) malloc(sizeof(Matrix_i));
    int *c = (int *)calloc(rows*column, sizeof(int));
    if (!A || !c)
    {
        fprintf(stderr,"Matrix Error \n in file: %s \n function %s\n line : %d\n %s \n", __FILE__,__FUNCTION__, __LINE__, strerror(errno));
        return NULL;
    }
	A->data_type = M_INT;
    A->column = column;
    A->row = rows;
    A->data = c;
    createZeroMatrixI(A);
	A->prop = M_SPARSE;
    return A;
}

Matrix_i * createRandomMatrixI(unsigned int rows, unsigned int column)
{
    Matrix_i *res = matrixAllocI(rows, column);
    unsigned int i,j;
	#pragma omp parallel for private(i,j) shared(res)
    for (i=0; i<res->row; i++)
    {
        for(j=0; j<res->column; j++)
        {
            res->data[i*res->column+j] = (int) (rand()/RAND_MAX);
        }
    }
    return res;
}

void createZeroMatrixI(Matrix_i *J)
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

Matrix_i *createTestVectorI(unsigned int type)
{
    Matrix_i *res = matrixAllocI(3, 1);
    int tmp;
	if(type == 1)
	{
		tmp = 0.19078;
		matrixSetI(res, 0,0,tmp);
		tmp = 0.21209;
		matrixSetI(res,1,0,tmp);
		tmp = 0.30087;
		matrixSetI(res,2,0,tmp);
	}	
	else if (type == 2)
	{
		tmp = 0.48679;
		matrixSetI(res, 0,0,tmp);
		tmp = 0.11108;
		matrixSetI(res,1,0,tmp);
		tmp = 0.61440;
		matrixSetI(res,2,0,tmp);
	} 
    return res;
}

void createIdentityI(Matrix_i *I)
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

void freeMatrixI(Matrix_i *A)
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

int matrixGetI(Matrix_i *A, unsigned int i, unsigned int j)
{
    if(A)
        return  A->data[i*A->column+j];
    return 0;
}

void matrixSetI(Matrix_i *A, unsigned int i, unsigned int j, const int c)
{
    if(A)
        A->data[i*A->column+j] = c;
}

void scaleMatrixI(Matrix_i *a, const int c)
{
    if(a)
    {
        unsigned int i, j;
        int tmp;
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

void addMatrixI(Matrix_i *a, Matrix_i *b)
{
    unsigned int i, j;
    int tmpa, tmpb;
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

void addMatrixTI(Matrix_i *a, Matrix_i *b)
{
    if ((a->column==b->row) && (a->row==b->column))
    {
        unsigned int i, j;
        int tmpa, tmpb;
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

void subMatrixI(Matrix_i *a, Matrix_i *b)
{
    if (a && b)
    {
        if ((a->column==b->column) && (a->row==b->row))
        {
            unsigned int i, j;
            int tmpa, tmpb;
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

void mulMatrixI(Matrix_i *a, Matrix_i *b, Matrix_i *c)
{
	if( a && b && c)
	{
		unsigned int i, j, k;
		int tmp = 0.f;
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

void matRefI(Matrix_i *A)
{
    if (A)
    {
        int c = A->data[0];
        scaleMatrixI(A, 1/c);
    }
}

void scaleLineI(Matrix_i *a, unsigned int i, int f)
{
    if(a)
    {
        if(i<a->row)
        {
            int c;
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

int subXLinesI(Matrix_i *a, unsigned int i, unsigned int i2, int f)
{
    int ret = 1;
    if(a)
    {
        if(i<a->row)
        {
            int c,tmp, tmp1;
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

void mulAddScaleMatrixI(Matrix_i *result, Matrix_i *mul1, Matrix_i *mul2, Matrix_i * add, int l, int s)
{
	unsigned int i, j, k;
	int tmp = 0.f;
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


int isDiagI(Matrix_i *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i!=j && a->data[i*a->column+j] !=0)
				return 0;
		}
	}
	a->prop = M_DIAG;
	return 1;
}

int isTriUI(Matrix_i *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i<=j)
				continue;
			if(a->data[i*a->column+j] !=0)
				return 0;
		}
	}
	a->prop = M_TRI_U;
	return 1;
}

int isTriLI(Matrix_i *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i>=j)
				continue;
			if(a->data[i*a->column+j] !=0)
				return 0;
		}
	}
	a->prop = M_TRI_L;
	return 1;
}

int isSparseI(Matrix_i *a)
{
	unsigned int i, j;
	int count = 0.;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(a->data[i*a->column+j] == 0)
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

