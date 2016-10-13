#ifndef __CPU_H
#define __CPU_H

#include <p18f2550.h>

#define NOP() Nop()

#define dTRUE 1
#define dFALSE !dTRUE

#define SETATOMIC(n) do {INTCONbits.GIEH = n;} while(0)
#define ATOMICSTATE() INTCONbits.GIEH
#define ATOMIC()  do {INTCONbits.GIEH = 0;} while(0)
#define nATOMIC() do {INTCONbits.GIEH = 1;} while(0)
#define ATOMICSECTION_SAVE(a) do{char b = INTCONbits.GIEH; INTCONbits.GIEH = 0; do{a}while(0); INTCONbits.GIEH = b;} while(0)
#define ATOMICSECTION_NOSAVE(a) do{ INTCONbits.GIEH = 0; do{a}while(0); INTCONbits.GIEH = 1;} while(0)
#endif //__CPU_H
