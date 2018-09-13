#include "../include/complex_f.h"

#include <stdint.h>
/**
 * @file complex_f.c to handle basic complexes functions
 * @author Nicolas Sourbier
 * @date 10/09/2018
 **/


static double mysqrt(double x) {
    if (x <= 0)
        return 0;
    int exp = 0;
    x = frexp(x, &exp);
    if (exp & 1) {   
        exp--;
        x *= 2;
    }
    double y = (1+x)/2;
	unsigned int i;
    for(i = 0; i< 6; i++){ 
        y = (y + x/y) / 2;
    }
    return ldexp(y, exp/2); 
}

int equals_f(float a, float b, float precis)
{
  if (fabs(a-b) < precis) return 1;
  return 0;
}

int isNull_f(Complex_f a)
{
	return (equals_f(a.re, 0.f, 1E-6) && equals_f(a.re, 0.f, 1E-6));
}

Complex_f conjugateF(Complex_f c)
{
    Complex_f res = c;
    res.im = __builtin_k1_fmulnrn(res.im,1); 
	return res;
}


Complex_f addF(Complex_f a, Complex_f b)
{
    Complex_f res;
    res.re = a.re + b.re;
    res.im = a.im + b.im;
    return res;
}

Complex_f subF(Complex_f a, Complex_f b)
{
    Complex_f res;
    res.re = a.re - b.re;
    res.im = a.im - b.im;
    return res;
}

Complex_f scaleF(Complex_f a, float c)
{
    Complex_f res;
    res.re = a.re*c;
    res.im = a.im*c;
    return res;
}

Complex_f multiplyF(Complex_f a, Complex_f b)
{
    Complex_f res;
    res.re = __builtin_k1_fdms(a.re,a.im,b.re,b.im);
    res.im = __builtin_k1_fcma(a.re,a.im,b.re,b.im);
    return res;
}

Complex_f divF(Complex_f a, Complex_f b)
{
    Complex_f res, tmp;
    if(!equals_f(b.im, 0.0f, 1e-6))// FAUX
    {
        tmp = conjugateF(b);
        a = multiplyF(a, tmp);
        b = multiplyF(b, tmp); 
		res.re = a.re/b.re;
		res.im = a.im/b.re;
    }
    else{  
        res.re = a.re/b.re;
		res.im = a.im/b.re;
    }
    return res;
}

Complex_f invF(Complex_f a)
{
    Complex_f res;
	float tmp = __builtin_k1_fcma(a.im,a.re,a.re,a.im);
	res.re = a.re/tmp;
    res.im = -a.im/tmp;
    return res;
}

Complex_f multiplyByConjF(Complex_f a)
{
    Complex_f res;
    res.im=0;
    res.re=a.re*a.re + a.im*a.im;
    return res;
}

float gainF(Complex_f a)
{
    return mysqrt((a.im*a.im)+(a.re*a.re));
}

Complex_f createNulComplexF(void)
{
    Complex_f res;
    res.re = res.im =0;
    return res;
}

Complex_f createUnitaryComplexF(void)
{
    Complex_f res;
    res.re = res.im =1;
    return res;
}

Complex_f createRandomComplexF(void)
{
    Complex_f res;
    res.re = (float) rand()/RAND_MAX;
    res.im = (float) rand()/RAND_MAX;
    return res;
}


Complex_f createComplexF(float re, float im)
{
    Complex_f res;
    res.re = re;
    res.im = im;
    return res;
}

void printComplexF(Complex_f a)
{
    if(a.re >=0 && a.im >=0)
        printf(" %f +%f*j", a.re, a.im);
    else if (a.im>0 && a.re <0)
        printf("%f +%f*j", a.re, a.im);
    else if (a.im<0 && a.re >0)
        printf(" %f %f*j", a.re, a.im);
    else printf("%f %f*j", a.re, a.im);
}
