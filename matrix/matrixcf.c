#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrixcf.h"
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
 * @file matrixcf.c to handle matrix operations for complex float
 * @author Nicolas Sourbier
 * @date 10/09/2018
 **/

void printMatrixCF(Matrix_cf *a)
{
    if(a)
    {
        unsigned int i, j;
	printf("matrix of size : %d, %d\n", a->row, a->column);
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                printComplexF(matrixGetCF(a, i, j));
                printf("\t");
            }
            printf("\n");
        }
    }
}

Matrix_cf * matrixAllocCF(unsigned int rows, unsigned int column)
{
    Matrix_cf *A = (Matrix_cf *)calloc(1,sizeof(Matrix_cf));
    Complex_f *c=(Complex_f *)calloc(rows*column, sizeof(Complex_f));
    if (!A || !c)
    {
        fprintf(stderr,"Matrix Error \n in file: %s \n function %s\n line : %d\n Unable to allocate memory \n", __FILE__,__FUNCTION__, __LINE__);
        return NULL;
    }
    A->column = column;
    A->row = rows;
    A->data = c;
    createZeroMatrixF(A);
	A->prop = M_SPARSE;
    return A;
}

Matrix_cf * createRandomMatrixCF(unsigned int rows, unsigned int column)
{
    Matrix_cf *res = matrixAllocCF(rows, column);
    unsigned int i,j;
    for (i=0; i<res->row; i++)
    {
        for(j=0; j<res->column; j++)
        {
            matrixSetCF(res, i, j, createRandomComplexF());
        }
    }
    return res;
}

void createZeroMatrixCF(Matrix_cf *J)
{
    if(J)
    {
        unsigned int i,j;
        for (i=0; i<J->row; i++)
        {
            for(j=0; j<J->column; j++)
            {
                matrixSetCF(J, i, j, createNulComplexF());
            }
        }
    }
}

Matrix_cf *createTestVectorCF(unsigned int type)
{
    Matrix_cf *res = matrixAllocCF(3, 1);

    Complex_f tmp;
    
	if(type == 1)
	{
		tmp. re = 0.19078;
		tmp.im = 0.90151;
		matrixSetCF(res, 0,0,tmp);
		tmp. re = 0.21209;
		tmp.im = 0.54550;
		matrixSetCF(res,1,0,tmp);
		tmp. re = 0.30087;
		tmp.im = 0.14163;
		matrixSetCF(res,2,0,tmp);
	}	
	else if (type == 2)
	{
		tmp. re = 0.48679;
		tmp.im = 0.05623;
		matrixSetCF(res, 0,0,tmp);
		tmp. re = 0.11108;
		tmp.im = 0.02649;
		matrixSetCF(res,1,0,tmp);
		tmp. re = 0.61440;
		tmp.im = 0.29708;
		matrixSetCF(res,2,0,tmp);
	}

	tmp.im = tmp.re = 1.0f;
    
    return res;
}

void createIdentityCF(Matrix_cf *I)
{
    if(I && I->column==I->row)
    {
        unsigned int i;
        Complex_f a;
        a.re =1;
        a.im =0;
        for (i=0; i<I->row; i++)
        {
            matrixSetCF(I, i, i, a);
        }
    }
}


void freeMatrixCF(Matrix_cf *A)
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

Complex_f matrixGetCF(Matrix_cf *A, unsigned int i, unsigned int j)
{
    if(A)
        return A->data[i*A->column+j];
    return createNulComplexF();
}

void matrixSetCF(Matrix_cf *A, unsigned int i, unsigned int j, const Complex_f c)
{
    if(A)
        A->data[i*A->column+j] = c;
}

void scaleMatrixCF(Matrix_cf *a, const Complex_f c)
{
    if(a)
    {
        unsigned int i, j;
        Complex_f tmp;
		#pragma omp parallel for private(i,j) shared(a)
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                tmp = a->data[i*a->column+j];
                matrixSetCF(a, i, j, multiplyF(tmp, c));
            }
        }
    }
}

void addMatrixCF(Matrix_cf *a, Matrix_cf *b)
{
    unsigned int i, j;
    Complex_f tmpa, tmpb;
	#pragma omp parallel for private(i,j) shared(a,b)
    for (i=0; i<a->row; i++)
    {
        for(j=0; j<a->column; j++)
        {
            tmpa = a->data[i*a->column+j];
            tmpb = b->data[i*b->column+j];
            matrixSetCF(a, i, j, addF(tmpa, tmpb));
        }
    }
}

void addMatrixTCF(Matrix_cf *a, Matrix_cf *b)
{
    if ((a->column==b->row) && (a->row==b->column))
    {
        unsigned int i, j;
        Complex_f tmpa, tmpb;
		#pragma omp parallel for private(i,j) shared(a,b)
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                tmpa = a->data[i*a->column+j];
                tmpb = b->data[j*b->column+i];
                matrixSetCF(a, i, j, addF(tmpa, tmpb));
            }
        }
    }
}

void subMatrixCF(Matrix_cf *a, Matrix_cf *b)
{
    if (a && b)
    {
        if ((a->column==b->column) && (a->row==b->row))
        {
            unsigned int i, j;
            Complex_f tmpa, tmpb;
			#pragma omp parallel for private(i,j) shared(a,b)
            for (i=0; i<a->row; i++)
            {
                for(j=0; j<a->column; j++)
                {
                    tmpa = a->data[i*a->column+j];
                    tmpb = b->data[i*b->column+j];
                    matrixSetCF(a, i, j, subF(tmpa, tmpb));
                }
            }
        }
    }
}

void mulMatrixCF(Matrix_cf *a, Matrix_cf *b, Matrix_cf *c)
{
	if( a && b && c)
	{
		unsigned int i, j, k;
		Complex_f tmp = createNulComplexF();
		#pragma omp parallel for private(i,j,k) shared(a,b,c)
		for (i = 0; i< a->row; i++)
		{	
			for(j = 0; j< b->column; j++)
			{
				for(k = 0; k < a->column; k++)
				{
					addF(tmp,multiplyF(a->data[i*a->column+k],b->data[k*b->column+j]));
				}
				matrixSetCF(c,i,j,tmp);
				tmp.re = tmp.im = 0.f;
			}
		}
	}
}

void matRefCF(Matrix_cf *A)
{
    if (A)
    {
        Complex_f c = matrixGetCF(A, 0, 0);
        Complex_f cconj = conjugateF(c);
        float gain  =0.;
        scaleMatrixCF(A, cconj);
        c = matrixGetCF(A, 0, 0);
        gain = gainF(c);
        gain = 1/gain;
        c.re = gain;
        c.im = 0;
        scaleMatrixCF(A, c);
    }
}

void scaleLineCF(Matrix_cf *a, unsigned int i, Complex_f f)
{
    if(a)
    {
        if(i<a->row)
        {
            Complex_f c;
            unsigned int j;
			#pragma omp parallel for private(j) shared(a)
            for(j=0; j<a->column; j++)
            {
                c = multiplyF(f,a->data[i*a->column+j]);
                matrixSetCF(a, i, j, c);
            }
        }
    }
}

int subXLinesCF(Matrix_cf *a, unsigned int i, unsigned int i2, Complex_f f)
{
    int ret = 1;
    if(a)
    {
        if(i<a->row)
        {
            Complex_f c,tmp, tmp1;
            unsigned int j;
			#pragma omp parallel for private(j) shared(a)
            for(j=0; j<a->column; j++)
            {
                tmp = a->data[i*a->column+j];
                tmp1 = a->data[i2*a->column+j];
                tmp1 = multiplyF(f,tmp1);
                c = subF(tmp,tmp1);
                matrixSetCF(a, i, j, c);
                if(c.re >1e-6 || c.im >1e-6)
                {
                    ret = 0;
                }
            }
        }
    }
    return ret;
}

void mulAddScaleMatrixCF(Matrix_cf *result, Matrix_cf *mul1, Matrix_cf *mul2, Matrix_cf * add, Complex_f l, Complex_f s)
{
	unsigned int i, j, k;
	Complex_f tmp = createNulComplexF();
	if(result && mul1 && mul2 && !add)
	{
		#pragma omp parallel for private(i,j,k) shared(mul1,mul2,result)
		for (i = 0; i< mul1->row; i++)
		{
			for(j = 0; j< mul2->column; j++)
			{
				for(k = 0; k < mul1->column; k++)
				{
					tmp = addF(tmp, multiplyF(matrixGetCF(mul1,i,k),matrixGetCF(mul2,k,j)));
				}
				tmp = multiplyF(s,tmp);
				matrixSetCF(result,i,j, tmp);
				tmp.re = tmp.im = 0;
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
					tmp = addF(tmp, multiplyF(matrixGetCF(mul1,i,k),matrixGetCF(mul2,k,j)));
				}
				tmp = multiplyF(s,addF(tmp,multiplyF(l, matrixGetCF(add,i,j)))); //tmp = s*(tmp+ l*matrixGetCF(add,i,j));
				matrixSetCF(result,i,j, tmp);
				tmp.re = tmp.im = 0;
			}
		}
	}
}


int isDiagCF(Matrix_cf *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i!=j && !isNull_f(a->data[i*a->column+j]))
				return 0;
		}
	}
	a->prop = M_DIAG;
	return 1;
}

int isTriUCF(Matrix_cf *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i<=j)
				continue;
			if(!isNull_f(a->data[i*a->column+j]))
				return 0;
		}
	}
	a->prop = M_TRI_U;
	return 1;
}

int isTriLCF(Matrix_cf *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i>=j)
				continue;
			if(!isNull_f(a->data[i*a->column+j]))
				return 0;
		}
	}
	a->prop = M_TRI_L;
	return 1;
}

int isSparseCF(Matrix_cf *a)
{
	unsigned int i, j;
	float count = 0.;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(isNull_f(a->data[i*a->column+j]))
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





/*


void compute_data_vectorCF(Matrix_cf *jac, Matrix_cf *A,  Matrix_cf *res, int na)
{
    int i, j;
    Complex_f tmp;
	for(i=0; i<na; i++)
    {
		for(j=0; j < na-1; j++)
		{
			tmp = multiplyF(matrixGetCF(jac,i*(na-1)+j,0),matrixGetCF(A,i,0));
			matrixSetCF(res, i*(na-1)+j, 0, tmp);
		}
	}
}

void giveJCF(Matrix_cf *A, Matrix_i * lbl, Matrix_cf *jac, int na)
{
    if(A && jac)
    {
        int i,j;
        int b;
        Complex_f insert;
        for(i=0; i<na; i++)
        {
            for(j=0; j<(na-1); j++)
            {
                b = (int) matrixGetI(lbl, 2*i+j, 1);
                insert = conjugateF(matrixGetCF(A,b,0));
                matrixSetCF(jac,i*(na-1) + j,0, insert);
            }
        }
    }
}*/
