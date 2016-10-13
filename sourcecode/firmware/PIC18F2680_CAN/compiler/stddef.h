/* $Id: stddef.h,v 1.2 2004/08/04 18:52:23 GrosbaJ Exp $ */
#ifndef __STDDEF_H
#define __STDDEF_H

typedef unsigned char wchar_t;

/* ptrdiff_t is a bit tricky for a PIC because of the Harvard architecture.
 * We'll use the data memory size for the generic and define additional types
 * for specifics.
 */
typedef signed short int ptrdiff_t;
typedef signed short int ptrdiffram_t;
typedef signed short long int ptrdiffrom_t;

/* size_t is tricky as well, for the same reason. 16 bits is sufficient
 * for data objects, but 24-bits are necessary for program memory objects.
 * This is true even for the 17Cxx architecture, as size_t is in bytes, and
 * the 17Cxx has an address range of 128Kbytes. We'll do the same thing
 * as we did for ptrdiff_t.
 */
typedef unsigned short int size_t;
typedef unsigned short int sizeram_t;
typedef unsigned short long int sizerom_t;

/* NULL is a simple one. Many compilers define NULL as ((void*)0), which
 * is not best. A pointer to void is a pointer to data and a comparison
 * to a function pointer is a type mismatch and should result in an
 * error. The ANSI/ISO standard makes no guarantees about pointers to
 * data and pointers to code being at all related to one another.
 *
 * Since the standard requires that a pointer comparison to an integral
 * constant value of zero be allowed and is equivalent to a comparison
 * to the NULL pointer constant, we define NULL very simply, as '0'.
 */
#define NULL 0

/* offsetof() is a bit trickier. We define it in the standard way. The
 * compiler should be smart enough to evaluate the expression at compile
 * time and not run-time. It has to be as offsetof() is required by
 * 4.1.5 to evaluate to an integer constant expression.
 */
#define offsetof(type, member_designator) (size_t)(&(((type *)0)->member_designator))

#endif  /* __STDDEF_H */
