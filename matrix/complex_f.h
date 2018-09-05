#ifndef COMPLEXF_H
#define COMPLEXF_H



typedef struct Complex_f
{
    float re;
    float im;
}Complex_f;


/**********************************************************************
 *          ---- Useful functions for float complexes ----
 * *******************************************************************/

/*
 *	@brief test the equality of two floats relatively to a precision
 *	@param a and b, the floats to compare
 *	@return 1 if equal, 0 otherwise
 */
int equals_f(float a, float b, float precis);

/*
 *	@brief test if a complex is NULL relatively to a precision
 *	@return 1 if null, 0 otherwise
 */
int isNull_f(Complex_f a);

/**
 * @brief conjugateF computes the conjugate of a complex
 * @param c the complex we want to know the conjugate
 * @return a new complex containing the conjugate of the one given in param
 */
Complex_f conjugateF(Complex_f c);


/**
 * @brief addF add two complexes
 * @param a the first operand
 * @param b the complex to add to the first one
 * @return  the sum of the two complexes
 */
Complex_f addF(Complex_f a, Complex_f b);

/**
 * @brief subF substract two complexes (a-b)
 * @param a the fist operand
 * @param b the complex we want to substract
 * @return the difference of the two
 */
Complex_f subF(Complex_f a, Complex_f b);

/**
 * @brief scaleF multiply a complex by a constant
 * @return the product of a and c
 */
Complex_f scaleF(Complex_f a, float c);

/**
 * @brief multiplyF multiply two complexes
 * @return the product of a and b
 */
Complex_f multiplyF(Complex_f a, Complex_f b);

/**
 * @brief divF divide two complexes (a/b)
 * @return the quotient of the two
 */
Complex_f divF(Complex_f a, Complex_f b);

/**
 * @brief invF compute the inverse of a complex
 * @param a the complex to invert
 * @return the inverted complex
 */
Complex_f invF(Complex_f a);

/**
 * @brief multiplyByConjF multiply a complex by it's conjugate
 * @param a the complex to multiply
 * @return the product of the complex and it's conjugate
 */
Complex_f multiplyByConjF(Complex_f a);

/**
 * @brief gainF compute the gain of a complex
 * @param a the complex we want to know the gain
 * @return the gain of the complex
 */
float gainF(Complex_f a);

/**
 * @brief createNulComplexF create a complex 0 + 0*i
 * @return a nul complex
 */
Complex_f createNulComplexF(void);

/**
 * @brief createUnitaryComplexF create a complex 1 + 1*i
 * @return a complex with a real an imaginary part of 1
 */
Complex_f createUnitaryComplexF(void);

/**
 * @brief createRandomComplexF create a random complex
 * @return a random complex
 */
Complex_f createRandomComplexF(void);

/**
 * @brief creatComplexF create a complex by receiving real and imaginary part
 * @return a random complex
 */
Complex_f createComplexF(float re, float im);

/**
 * @brief printf a complex
 */
void printComplexF(Complex_f a);


#endif /* COMPLEX_H*/

