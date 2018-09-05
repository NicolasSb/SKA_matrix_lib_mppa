#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex_d.h"


int equals_d(double a, double b, double precis)
{
  if (fabs(a-b) < precis) return 1;
  return 0;
}

int isNull_d(Complex_d a)
{
	return (equals_d(a.re, 0.f, 1E-9) && equals_d(a.re, 0.f, 1E-9));
}

Complex_d conjugateD(Complex_d c)
{
    Complex_d res = c;
    res.im *= -1;
    return res;
}

Complex_d addD(Complex_d a, Complex_d b)
{
    Complex_d res;
    res.re = a.re + b.re;
    res.im = a.im + b.im;
    return res;
}

Complex_d subD(Complex_d a, Complex_d b)
{
    Complex_d res;
    res.re = a.re - b.re;
    res.im = a.im - b.im;
    return res;
}

Complex_d scaleD(Complex_d a, double c)
{
    Complex_d res;
    res.re = a.re*c;
    res.im = a.im*c;
    return res;
}

Complex_d multiplyD(Complex_d a, Complex_d b)
{
    Complex_d res;
    res.re = a.re*b.re -a.im*b.im;
    res.im = a.re*b.im +a.im*b.re;
    return res;
}

Complex_d divD(Complex_d a, Complex_d b)
{
    Complex_d res, tmp;
    if(!equals(b.im, 0.0f, 1e-9))
    {
        tmp = conjugateD(b);
        a = multiplyD(a, tmp);
        b = multiplyD(b, tmp);
        res.re = a.re/b.re;
        res.im = a.im/b.re;
    }
    else{
        res.re = a.re/b.re;
        res.im = a.im/b.re;
    }
    return res;
}

Complex_d invD(Complex_d a)
{
    Complex_d tmp;
    tmp.re = 1.0f;
    tmp.im = 0.0f;
    return divD(tmp, a);
}

Complex_d multiplyByConjD(Complex_d a)
{
    Complex_d res;
    res.im=0;
    res.re=2*a.re + 2*a.im;
    return res;
}

double gainD(Complex_d a)
{
    return sqrt(pow(a.im,2)+pow(a.re,2));
}

Complex_d createNulComplexD(void)
{
    Complex_d res;
    res.re = res.im =0;
    return res;
}

Complex_d createUnitaryComplexD(void)
{
    Complex_d res;
    res.re = res.im =1;
    return res;
}

Complex_d createRandomComplexD(void)
{
    Complex_d res;
    res.re = (double) rand()/RAND_MAX;
    res.im = (double) rand()/RAND_MAX;
    return res;
}

Complex_d createComplexD(double re, double im)
{
    Complex_d res;
    res.re = re;
    res.im = im;
    return res;
}

void printComplexD(Complex_d a)
{
    if(a.re >=0 && a.im >=0)
        printf(" %f +%f*j", a.re, a.im);
    else if (a.im>0 && a.re <0)
        printf("%f +%f*j", a.re, a.im);
    else if (a.im<0 && a.re >0)
        printf(" %f %f*j", a.re, a.im);
    else printf("%f %f*j", a.re, a.im);
}
