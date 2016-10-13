/* $Id: stdlib.h,v 1.1 2003/12/09 22:54:19 GrosbaJ Exp $ */
#ifndef __STDLIB_H
#define __STDLIB_H

#if __18CXX
/** @name String Conversion Functions
 * If the value of the result for a function cannot be represented by the
 * type of the return value of that function, the behaviour is undefined.
 */
/*@{*/
/** @name atof
 * The {\bf atof} function converts the initial portion of the string pointed
 * to by {\bf s} into a floating-point {\bf double} representation.
 * @param s pointer to the string to convert
 * @return The {\bf atof} function returns the converted value
 */
double atof (const auto char *s);
/** @name atob
 * The {\bf atob} function converts the string pointed to by {\bf s} into a 
 * {\bf signed char} integer representation. Characters are recognized in the
 * following order: An optional string of tabs and spaces, An optional sign, 
 * A string of digits. The first unrecognized character ends the conversion. 
 * Overflow results are undefined.
 * The {\bf atob} function is an MPLAB-Cxx extension to the ANSI required
 * libraries and may not be present in other implementations.
 * @param s pointer to the string to convert
 * @return The {\bf atob} function returns the converted value as a signed char
 */
signed char atob (const auto char *s);

/** @name atoi
 * The {\bf atoi} function converts the string pointed to by {\bf s} into a 
 * {\bf int} integer representation. Characters are recognized in the 
 * following order: An optional string of tabs and spaces, An optional sign, 
 * A string of digits. The first unrecognized character ends the conversion. 
 * Overflow results are undefined.
 * @param s pointer to the string to convert
 * @return The {\bf atoi} function returns the converted value as an int
 */
int atoi (const auto char *s);
/** @name atol
 * The {\bf atol} function converts the initial portion of the string pointed
 * to by {\bf s} into a {\bf long} integer representation. The initial portion 
 * of the string is assumed to be in radix 10.
 * @param s pointer to the string to convert
 * @return The {\bf atol} function returns the converted value
 */
long atol (const auto char *s);
/** @name atoul
 * The {\bf atoul} function converts the initial portion of the string pointed
 * to by {\bf s} into a {\bf unsigned long} integer representation. 
 * The initial portion  of the string is assumed to be in radix 10.
 *
 * The {\bf atoul} function is an MPLAB-Cxx extension to the ANSI required
 * libraries and may not be present in other implementations.
 * @param s pointer to the string to convert
 * @return The {\bf atoul} function returns the converted value
 */
unsigned long atoul (const auto char *s);

/** @name btoa
 * The {\bf btoa} function converts the {\bf signed char} value {\bf value} to 
 * a radix 10 string representation, storing the resultant string into
 * the location pointed to by {\bf s}.
 *
 * The {\bf btoa} function is an MPLAB-Cxx extension to the ANSI required
 * libraries and may not be present in other implementations.
 * @param value value to convert
 * @param s pointer to destination string object
 * @return The {\bf btoa} function returns the value of {\bf s}.
 */
char *btoa (auto signed char value, auto char *s);
/** @name itoa
 * The {\bf itoa} function converts the {\bf int} value {\bf value} to 
 * a radix 10 string representation, storing the resultant string into
 * the location pointed to by {\bf s}.
 *
 * The {\bf itoa} function is an MPLAB-Cxx extension to the ANSI required
 * libraries and may not be present in other implementations.
 * @param value value to convert
 * @param s pointer to destination string object
 * @return The {\bf itoa} function returns the value of {\bf s}.
 */
char *itoa (auto int value, auto char *s);
/** @name ltoa
 * The {\bf ltoa} function converts the {\bf long} value {\bf value} to 
 * a radix 10 string representation, storing the resultant string into
 * the location pointed to by {\bf s}.
 *
 * The {\bf ltoa} function is an MPLAB-Cxx extension to the ANSI required
 * libraries and may not be present in other implementations.
 * @param value value to convert
 * @param s pointer to destination string object
 * @return The {\bf ltoa} function returns the value of {\bf s}.
 */
char *ltoa (auto long value, auto char *s);
/** @name ultoa
 * The {\bf ultoa} function converts the {\bf unsigned long} value {\bf value} to 
 * a radix 10 string representation, storing the resultant string into
 * the location pointed to by {\bf s}.
 *
 * The {\bf ultoa} function is an MPLAB-Cxx extension to the ANSI required
 * libraries and may not be present in other implementations.
 * @param value value to convert
 * @param s pointer to destination string object
 * @return The {\bf ultoa} function returns the value of {\bf s}.
 */
char *ultoa (auto unsigned long value, auto char *s);
/*@}*/

/** @name Pseudo-Random Sequence Generation Functions
 */
/*@{*/
/** @name RAND_MAX
 * The maximum value which can be returned by the {\bf rand} function.
 */
#define RAND_MAX 32767

/** @name rand
 * The {\bf rand} function generates a sequence of pseudo-random integer
 * values in the range [0,RAND_MAX].
 *
 * The implementation shall behave as if no library function calls {\bf rand}.
 */
int rand (void);
/** @name srand
 * The {\bf srand} function sets the starting seed for the pseudo-random
 * number sequence generated by the {\bf rand} function. If {\bf srand} is
 * called again with the same seed value, the same sequence of value shall
 * be returned by {\bf rand}. If {\bf rand} is called without {\bf srand}
 * having first been called, the pseudo-random sequence of numbers generated
 * shall be the same as if {\bf srand} had been called with a seed value
 * of 1.
 *
 * The implementation shall behave as if no library function calls {\bf srand}.
 */
void srand (auto unsigned int seed);
/*@}*/
#else
void btoa(static char , static char *);
void ubtoa(static unsigned char ,static char *);
char atob(static char *);
unsigned char atoub(static char *);
void itoa(static int,static char *);
void uitoa(static unsigned int,static char *);
int atoi(static char *);
unsigned int atoui(static char *);
#endif

#endif
