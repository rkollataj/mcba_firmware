/*
 *      limits.h
 *
 *      Defines implementation specific limits on type values.
 *
 *      Copyright (c) 1997, Microchip Technology Inc.
 *      2355 W.Chandler Blvd., Chandler, AZ, 85224
 *      All Rights Reserved.
 */

#ifndef __LIMITS_H
#define __LIMITS_H

#define CHAR_BIT        8               /* number of bits in a char */
#define MB_LEN_MAX      2               /* max. # bytes in multibyte char */

#define SCHAR_MIN     (-128)            /* minimum signed   char value */
#define SCHAR_MAX       127             /* maximum signed   char value */
#define UCHAR_MAX       255             /* maximum unsigned char value */

/* changed '\x80'  to 0x80 on the next line */
#if 0x80 < 0       /* signed chars ? */
#define CHAR_MIN        SCHAR_MIN       /* minimum char value */
#define CHAR_MAX        SCHAR_MAX       /* maximum char value */
#else              /* unsigned chars */
#define CHAR_MIN        0
#define CHAR_MAX        UCHAR_MAX
#endif

#define SHRT_MIN      (-32767-1)        /* minimum signed   short value */
#define SHRT_MAX        32767           /* maximum signed   short value */
#define USHRT_MAX       65535U          /* maximum unsigned short value */

#define LONG_MIN      0x80000000
#define LONG_MAX      0x7fffffff
#define ULONG_MAX     0xffffffff

#define SLONG_MIN     0x800000         /* minimum signed short long value */
#define SLONG_MAX     0x7fffff         /* maximum signed short long value */
#define USLONG_MAX    0xffffff         /* maximum unsigned short long value */

/* Our ints are 16-bit just like our shorts */
#define INT_MIN         SHRT_MIN        /* minimum signed   int value */
#define INT_MAX         SHRT_MAX        /* maximum signed   int value */
#define UINT_MAX        USHRT_MAX       /* maximum unsigned int value */

/* alternate names for INT_MAX and INT_MIN */
#define MIN_INT         INT_MIN
#define MAX_INT         INT_MAX

#endif  /* __LIMITS_H */











