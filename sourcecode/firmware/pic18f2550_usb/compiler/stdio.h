#ifndef __STDIO_H
#define __STDIO_H

#include <stdarg.h>
#include <stddef.h>

/* Change this to near (or omit altogether) if building small memory model
 * versions of the libraries
 */
#define MEM_MODEL far

#define EOF ((int)-1)


typedef unsigned char FILE;

/* output destination handles */
#define _H_USER ((FILE*)((int)-1))
#define _H_USART ((FILE*)((int)-2))

extern FILE *stderr;
extern FILE *stdout;


int putc (auto char c, auto FILE *f);
int vsprintf (auto char *buf, auto const MEM_MODEL rom char *fmt, auto va_list ap);
int vprintf (auto const MEM_MODEL rom char *fmt, auto va_list ap);
int sprintf (auto char *buf, auto const MEM_MODEL rom char *fmt, ...);
int printf (auto const MEM_MODEL rom char *fmt, ...);
int fprintf (auto FILE *f, auto const MEM_MODEL rom char *fmt, ...);
int vfprintf (auto FILE *f, auto const MEM_MODEL rom char *fmt, auto va_list ap);
int puts (auto const MEM_MODEL rom char *s);
int fputs (auto const MEM_MODEL rom char *s, auto FILE *f);

#endif
