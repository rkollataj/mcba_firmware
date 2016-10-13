/*
 * File:   main.c
 * Author: c10948
 *
 * Created on March 31, 2011, 11:01 AM
 */

#include <pic18.h>
#include <htc.h>
#include "uart.h"

void interrupt isr()
{

}

void interrupt low_priority low_isr()
{
    uart_isr();
}

int main(void)
{
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;

    uart_init();

    IPEN = 1;
    GIEL = 1;
    GIEH = 1;

    LATA = LATB=LATC=LATD = 0x55;

    while(1)
    {
        for(char x='a'; x <= 'z'; x ++)
        {
	        LATD0 = 1;
            uart_put(x);
            CLRWDT();
            LATD0 = 0;
            for(int x = 0 ; x < 100; x ++);
        }
        for(int x=0;x<1000;x++);
    }

    return 0;
}
