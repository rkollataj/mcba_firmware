/* $Id: stdarg.h,v 1.2 2004/08/04 18:46:13 GrosbaJ Exp $ */

#ifndef __STDARG_H
#define __STDARG_H

typedef void* va_list;
#define va_start(ap,l) {(ap)=(void*)((char*)&(l));}
#define va_end(ap)
#define va_arg(ap,t) (*((t*)((ap)=((t*)ap)-1)))
#define va_copy(dst, src) ((dst) = (src))
#endif
