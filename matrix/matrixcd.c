#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrixcd.h"
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

void printMatrixCD(Matrix_cd *a)
{
    if(a)
    {
        unsigned int i, j;
	printf("matrix of size : %d, %d\n", a->row, a->column);
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                printComplexD(matrixGetCD(a, i, j));
                printf("\t");
            }
            printf("\n");
        }
    }
}

Matrix_cd * matrixAllocCD(unsigned int rows, unsigned int column)
{
    Matrix_cd *A = (Matrix_cd *)calloc(1,sizeof(Matrix_cd));
    Complex_d *c=(Complex_d *)calloc(rows*column, sizeof(Complex_d));
    if (!A || !c)
    {
        fprintf(stderr,"Matrix Error \n in file: %s \n function %s\n line : %d\n Unable to allocate memory \n", __FILE__,__FUNCTION__, __LINE__);
        return NULL;
    }
    A->column = column;
    A->row = rows;
    A->data = c;
    createZeroMatrixD(A);
	A->prop = M_SPARSE;
    return A;
}

Matrix_cd * createRandomMatrixCD(unsigned int rows, unsigned int column)
{
    Matrix_cd *res = matrixAllocCD(rows, column);
    unsigned int i,j;
    for (i=0; i<res->row; i++)
    {
        for(j=0; j<res->column; j++)
        {
            matrixSetCD(res, i, j, createRandomComplexD());
        }
    }
    return res;
}

void createZeroMatrixCD(Matrix_cd *J)
{
    if(J)
    {
        unsigned int i,j;
        for (i=0; i<J->row; i++)
        {
            for(j=0; j<J->column; j++)
            {
                matrixSetCD(J, i, j, createNulComplexD());
            }
        }
    }
}

Matrix_cd *createTestVectorCD(unsigned int type)
{
    Matrix_cd *res = matrixAllocCD(3, 1);

    Complex_d tmp;
    
	if(type == 1)
	{
		tmp. re = 0.19078;
		tmp.im = 0.90151;
		matrixSetCD(res, 0,0,tmp);
		tmp. re = 0.21209;
		tmp.im = 0.54550;
		matrixSetCD(res,1,0,tmp);
		tmp. re = 0.30087;
		tmp.im = 0.14163;
		matrixSetCD(res,2,0,tmp);
	}	
	else if (type == 2)
	{
		tmp. re = 0.48679;
		tmp.im = 0.05623;
		matrixSetCD(res, 0,0,tmp);
		tmp. re = 0.11108;
		tmp.im = 0.02649;
		matrixSetCD(res,1,0,tmp);
		tmp. re = 0.61440;
		tmp.im = 0.29708;
		matrixSetCD(res,2,0,tmp);
	}

	tmp.im = tmp.re = 1.0f;
    
    return res;
}

void createIdentityCD(Matrix_cd *I)
{
    if(I && I->column==I->row)
    {
        unsigned int i;
        Complex_d a;
        a.re =1;
        a.im =0;
        for (i=0; i<I->row; i++)
        {
            matrixSetCD(I, i, i, a);
        }
    }
}


void freeMatrixCD(Matrix_cd *A)
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

Complex_d matrixGetCD(Matrix_cd *A, unsigned int i, unsigned int j)
{
    if(A)
        return A->data[i*A->column+j];
    return createNulComplexD();
}

void matrixSetCD(Matrix_cd *A, unsigned int i, unsigned int j, const Complex_d c)
{
    if(A)
        A->data[i*A->column+j] = c;
}

void scaleMatrixCD(Matrix_cd *a, const Complex_d c)
{
    if(a)
    {
        unsigned int i, j;
        Complex_d tmp;
		#pragma omp parallel for private(i,j) shared(a)
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                tmp = a->data[i*a->column+j];
                matrixSetCD(a, i, j, multiplyD(tmp, c));
            }
        }
    }
}

void addMatrixCD(Matrix_cd *a, Matrix_cd *b)
{
    unsigned int i, j;
    Complex_d tmpa, tmpb;
	#pragma omp parallel for private(i,j) shared(a,b)
    for (i=0; i<a->row; i++)
    {
        for(j=0; j<a->column; j++)
        {
            tmpa = a->data[i*a->column+j];
            tmpb = b->data[i*b->column+j];
            matrixSetCD(a, i, j, addD(tmpa, tmpb));
        }
    }
}

void addMatrixTCD(Matrix_cd *a, Matrix_cd *b)
{
    if ((a->column==b->row) && (a->row==b->column))
    {
        unsigned int i, j;
        Complex_d tmpa, tmpb;
		#pragma omp parallel for private(i,j) shared(a,b)
        for (i=0; i<a->row; i++)
        {
            for(j=0; j<a->column; j++)
            {
                tmpa = a->data[i*a->column+j];
                tmpb = b->data[j*b->column+i];
                matrixSetCD(a, i, j, addD(tmpa, tmpb));
            }
        }
    }
}

void subMatrixCD(Matrix_cd *a, Matrix_cd *b)
{
    if (a && b)
    {
        if ((a->column==b->column) && (a->row==b->row))
        {
            unsigned int i, j;
            Complex_d tmpa, tmpb;
			#pragma omp parallel for private(i,j) shared(a,b)
            for (i=0; i<a->row; i++)
            {
                for(j=0; j<a->column; j++)
                {
                    tmpa = a->data[i*a->column+j];
                    tmpb = b->data[i*b->column+j];
                    matrixSetCD(a, i, j, subD(tmpa, tmpb));
                }
            }
        }
    }
}

void mulMatrixD(Matrix_cd *a, Matrix_cd *b, Matrix_cd *c)
{
	if( a && b && c)
	{
		unsigned int i, j, k;
		Complex_d tmp = createNulComplexD();
		#pragma omp parallel for private(i,j,k) shared(a,b,c)
		for (i = 0; i< a->row; i++)
		{	
			for(j = 0; j< b->column; j++)
			{
				for(k = 0; k < a->column; k++)
				{
					addD(tmp,multiplyD(a->data[i*a->column+k],b->data[k*b->column+j]));
				}
				matrixSetCD(c,i,j,tmp);
				tmp.re = tmp.im = 0.f;
			}
		}
	}
}

void matRefCD(Matrix_cd *A)
{
    if (A)
    {
        Complex_d c = matrixGetCD(A, 0, 0);
        Complex_d cconj = conjugateD(c);
        double gain  =0.;
        scaleMatrixCD(A, cconj);
        c = matrixGetCD(A, 0, 0);
        gain = gainD(c);
        gain = 1/gain;
        c.re = gain;
        c.im = 0;
        scaleMatrixCD(A, c);
    }
}

void scaleLineCD(Matrix_cd *a, unsigned int i, Complex_d f)
{
    if(a)
    {
        if(i<a->row)
        {
            Complex_d c;
            unsigned int j;
			#pragma omp parallel for private(j) shared(a)
            for(j=0; j<a->column; j++)
            {
                c = multiplyD(f,a->data[i*a->column+j]);
                matrixSetCD(a, i, j, c);
            }
        }
    }
}

int subXLinesD(Matrix_cd *a, unsigned int i, unsigned int i2, Complex_d f)
{
    int ret = 1;
    if(a)
    {
        if(i<a->row)
        {
            Complex_d c,tmp, tmp1;
            unsigned int j;
			#pragma omp parallel for private(j) shared(a)
            for(j=0; j<a->column; j++)
            {
                tmp = a->data[i*a->column+j];
                tmp1 = a->data[i2*a->column+j];
                tmp1 = multiplyD(f,tmp1);
                c = subD(tmp,tmp1);
                matrixSetCD(a, i, j, c);
                if(c.re >1e-6 || c.im >1e-6)
                {
                    ret = 0;
                }
            }
        }
    }
    return ret;
}

void mulAddScaleMatrixD(Matrix_cd *result, Matrix_cd *mul1, Matrix_cd *mul2, Matrix_cd * add, Complex_d l, Complex_d s)
{
	unsigned int i, j, k;
	Complex_d tmp = createNulComplexD();
	if(result && mul1 && mul2 && !add)
	{
		#pragma omp parallel for private(i,j,k) shared(mul1,mul2,result)
		for (i = 0; i< mul1->row; i++)
		{
			for(j = 0; j< mul2->column; j++)
			{
				for(k = 0; k < mul1->column; k++)
				{
					tmp = addD(tmp, multiplyD(matrixGetCD(mul1,i,k),matrixGetCD(mul2,k,j)));
				}
				tmp = multiplyD(s,tmp);
				matrixSetCD(result,i,j, tmp);
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
					tmp = addD(tmp, multiplyD(matrixGetCD(mul1,i,k),matrixGetCD(mul2,k,j)));
				}
				tmp = multiplyD(s,addD(tmp,multiplyD(l, matrixGetCD(add,i,j)))); //tmp = s*(tmp+ l*matrixGetCD(add,i,j));
				matrixSetCD(result,i,j, tmp);
				tmp.re = tmp.im = 0;
			}
		}
	}
}


int isDiagD(Matrix_cd *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i!=j && !isNull_d(a->data[i*a->column+j]))
				return 0;
		}
	}
	a->prop = M_DIAG;
	return 1;
}

int isTriUD(Matrix_cd *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i<=j)
				continue;
			if(!isNull_d(a->data[i*a->column+j]))
				return 0;
		}
	}
	a->prop = M_TRI_U;
	return 1;
}

int isTriLD(Matrix_cd *a)
{
	unsigned int i, j;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(i>=j)
				continue;
			if(!isNull_d(a->data[i*a->column+j]))
				return 0;
		}
	}
	a->prop = M_TRI_L;
	return 1;
}

int isSparseD(Matrix_cd *a)
{
	unsigned int i, j;
	double count = 0.;
	for(i = 0; i<a->column; i++)
	{
		for(j=0; j<a->row; j++)
		{
			if(isNull_d(a->data[i*a->column+j]))
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








void compute_data_vectorCD(Matrix_cd *jac, Matrix_cd *A,  Matrix_cd *res, int na)
{
    int i, j;
    Complex_d tmp;
	for(i=0; i<na; i++)
    {
		for(j=0; j < na-1; j++)
		{
			tmp = multiplyD(matrixGetCD(jac,i*(na-1)+j,0),matrixGetCD(A,i,0));
			matrixSetCD(res, i*(na-1)+j, 0, tmp);
		}
	}
}

void giveJfD(Matrix_cd *A, Matrix_i * lbl, Matrix_cd *jac, int na)
{
    if(A && jac)
    {
        int i,j;
        int b;
        Complex_d insert;
        for(i=0; i<na; i++)
        {
            for(j=0; j<(na-1); j++)
            {
                b = (int) matrixGetI(lbl, 2*i+j, 1);
                insert = conjugateD(matrixGetCD(A,b,0));
                matrixSetCD(jac,i*(na-1) + j,0, insert);
            }
        }
    }
}
