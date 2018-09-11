#ifndef COMPLEXD_H
#define COMPLEXD_H

#include <math.h>

typedef struct Complex_d
{
    double re;
    double im;
}Complex_d;


/**********************************************************************
 *          ---- Useful functions for double complexes ----
 * *******************************************************************/

/*
 *	@brief test the equality of two doubles relatively to a precision
 *	@param a and b, the doubles to compare
 *	@return 1 if equal, 0 otherwise
 */
int equals_d(double a, double b, double precis);

/*
 *	@brief test if a complex is NULL relatively to a precision
 *	@param a and b, the doubles to compare
 *	@return 1 if equal, 0 otherwise
 */
int isNull_d(Complex_d a);

/**
 * @brief conjugateD computes the conjugate of a complex
 * @param c the complex we want to know the conjugate
 * @return a new complex containing the conjugate of the one given in param
 */
Complex_d conjugateD(Complex_d c);


/**
 * @brief addD add two complexes
 * @param a the first operand
 * @param b the complex to add to the first one
 * @return  the sum of the two complexes
 */
Complex_d addD(Complex_d a, Complex_d b);

/**
 * @brief subD substract two complexes (a-b)
 * @param a the fist operand
 * @param b the complex we want to substract
 * @return the difference of the two
 */
Complex_d subD(Complex_d a, Complex_d b);

/**
 * @brief scaleD multiply a complex by a constant
 * @return the product of a and c
 */
Complex_d scaleD(Complex_d a, double c);

/**
 * @brief multiplyD multiply two complexes
 * @return the product of a and b
 */
Complex_d multiplyD(Complex_d a, Complex_d b);

/**
 * @brief divD divide two complexes (a/b)
 * @return the quotient of the two
 */
Complex_d divD(Complex_d a, Complex_d b);

/**
 * @brief invD compute the inverse of a complex
 * @param a the complex to invert
 * @return the inverted complex
 */
Complex_d invD(Complex_d a);

/**
 * @brief multiplyByConjD multiply a complex by it's conjugate
 * @param a the complex to multiply
 * @return the product of the complex and it's conjugate
 */
Complex_d multiplyByConjD(Complex_d a);

/**
 * @brief gainD compute the gain of a complex
 * @param a the complex we want to know the gain
 * @return the gain of the complex
 */
double gainD(Complex_d a);

/**
 * @brief createNulComplexD create a complex 0 + 0*i
 * @return a nul complex
 */
Complex_d createNulComplexD(void);

/**
 * @brief createUnitaryComplexD create a complex 1 + 1*i
 * @return a complex with a real an imaginary part of 1
 */
Complex_d createUnitaryComplexD(void);

/**
 * @brief createRandomComplexD create a random complex
 * @return a random complex
 */
Complex_d createRandomComplexD(void);

/**
 * @brief creatComplexD create a complex by receiving real and imaginary part
 * @return a random complex
 */
Complex_d createComplexD(double re, double im);

/**
 * @brief printf a complex
 */
void printComplexD(Complex_d a);


#endif /* COMPLEXD_H*/

