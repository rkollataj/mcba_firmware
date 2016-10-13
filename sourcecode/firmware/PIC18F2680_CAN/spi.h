#ifndef __SPI_H
#define __SPI_H
#include <pconfig.h>

/* PIC18 SPI peripheral library header */


/* SSPSTAT REGISTER */

// Master SPI mode only

#define   SMPEND        0b10000000           // Input data sample at end of data out             
#define   SMPMID        0b00000000           // Input data sample at middle of data out


#define   MODE_00       0b00000000              // Setting for SPI bus Mode 0,0
//CKE           0x40                   // SSPSTAT register 
//CKP           0x00                   // SSPCON1 register 

#define   MODE_01       0b00000001              // Setting for SPI bus Mode 0,1
//CKE           0x00                   // SSPSTAT register 
//CKP           0x00                   // SSPCON1 register

#define   MODE_10       0b00000010              // Setting for SPI bus Mode 1,0
//CKE           0x40                   // SSPSTAT register
//CKP           0x10                   // SSPCON1 register

#define   MODE_11       0b00000011              // Setting for SPI bus Mode 1,1
//CKE           0x00                   // SSPSTAT register
//CKP           0x10                   // SSPCON1 register

/* SSPCON1 REGISTER */
#define   SSPENB        0b00100000           // Enable serial port and configures SCK, SDO, SDI

#define   SPI_FOSC_4    0b00000000              // SPI Master mode, clock = Fosc/4
#define   SPI_FOSC_16   0b00000001              // SPI Master mode, clock = Fosc/16
#define   SPI_FOSC_64   0b00000010              // SPI Master mode, clock = Fosc/64
#define   SPI_FOSC_TMR2 0b00000011              // SPI Master mode, clock = TMR2 output/2
#define   SLV_SSON      0b00000100              // SPI Slave mode, /SS pin control enabled
#define   SLV_SSOFF     0b00000101              // SPI Slave mode, /SS pin control disabled
//************************************************************************************************//

/*  25Cxxx EEPROM instruction set */
#define   SPI_WREN          6              // write enable latch
#define   SPI_WRDI          4              // reset the write enable latch
#define   SPI_RDSR          5              // read status register
#define   SPI_WRSR          1              // write status register
#define   SPI_READ          3              // read data from memory
#define   SPI_WRITE         2              // write data to memory

/*  Bits within status register of 25Cxxx */
#define   WIP           0              // write in progress status
#define   WEL           1              // write enable latch status
#define   BP0           2              // block protection bit status
#define   BP1           3              // block protection bit status


/* FUNCTION PROTOTYPES */

#define PARAM_SCLASS auto


/* These devices have two SPI modules */
#if defined (SPI_V2) || defined (SPI_V3)

/* ***** SPI1 ***** */

/* CloseSPI1
 * Disable SPI1 module
 */
#define  CloseSPI1()      (SSP1CON1 &=0xDF)
#define  CloseSPI CloseSPI1

/* DataRdySPI1
 * Test if SSP1BUF register is full
 */
#define  DataRdySPI1()    (SSP1STATbits.BF)
#define DataRdySPI DataRdySPI1

/* ReadSPI1
 * Read byte from SSP1BUF register
 */
unsigned char ReadSPI1( void );
#define ReadSPI ReadSPI1

/* getcSPI1
 * Read byte from SSP1BUF register
 */
#define  getcSPI1  ReadSPI1
#define getcSPI getcSPI1

/* OpenSPI1
 */
void OpenSPI1( PARAM_SCLASS unsigned char sync_mode,
               PARAM_SCLASS unsigned char bus_mode,
               PARAM_SCLASS unsigned char smp_phase );
#define OpenSPI OpenSPI1

/* WriteSPI1
 * Write byte to SSP1BUF register
 */
unsigned char WriteSPI1( PARAM_SCLASS unsigned char data_out );
#define WriteSPI WriteSPI1

/* putcSPI1
 * Write byte to SSP1BUF register
 */
#define  putcSPI1  WriteSPI1
#define  putcSPI putcSPI1

/* getsSPI1
 * Write string to SSP1BUF
 */
void getsSPI1( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );
#define getsSPI getsSPI1

/* putsSPI1
 * Read string from SSP1BUF
 */
void putsSPI1( PARAM_SCLASS unsigned char *wrptr );
#define putsSPI putsSPI1

/* ***** SPI2 ***** */

/* CloseSPI2
 * Disable SPI2 module
 */
#define  CloseSPI2()      (SSP2CON1 &=0xDF)


/* DataRdySPI2
 * Test if SSP2BUF register is full
 */
#define  DataRdySPI2()    (SSP2STATbits.BF)

/* ReadSPI2
 * Read byte from SSP2BUF register
 */
unsigned char ReadSPI2( void );

/* getcSPI2
 * Read byte from SSP2BUF register
 */
#define  getcSPI2  ReadSPI2

/* OpenSPI2
 */
void OpenSPI2( PARAM_SCLASS unsigned char sync_mode,
               PARAM_SCLASS unsigned char bus_mode,
               PARAM_SCLASS unsigned char smp_phase );

/* WriteSPI2
 * Write byte to SSP2BUF register
 */
unsigned char WriteSPI2( PARAM_SCLASS unsigned char data_out );

/* putcSPI2
 * Write byte to SSP2BUF register
 */
#define  putcSPI2  WriteSPI2

/* getsSPI2
 * Write string to SSP2BUF
 */
void getsSPI2( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );

/* putsSPI2
 * Read string from SSP2BUF
 */
void putsSPI2( PARAM_SCLASS unsigned char *wrptr );

#endif // v2 and v3

#if defined (SPI_V1) || defined (SPI_V4)

/* ***** SPI ***** */


/* DataRdySPI
 * Test if SSPBUF register is full
 */
#define  DataRdySPI()    (SSPSTATbits.BF)

/* ReadSPI
 * Read byte from SSPBUF register
 */
unsigned char ReadSPI( void );

/* getcSPI
 * Read byte from SSPBUF register
 */
#define  getcSPI  ReadSPI

/* OpenSPI
 */
void OpenSPI( PARAM_SCLASS unsigned char sync_mode,
              PARAM_SCLASS unsigned char bus_mode,
              PARAM_SCLASS unsigned char smp_phase );

/* WriteSPI
 * Write byte to SSPBUF register
 */
unsigned char WriteSPI( PARAM_SCLASS unsigned char data_out );

/* putcSPI
 * Write byte to SSPBUF register
 */
#define  putcSPI  WriteSPI

/* getsSPI
 * Write string to SSPBUF
 */
void getsSPI( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );


/* putsSPI
 * Read string from SSPBUF
 */
void putsSPI( PARAM_SCLASS unsigned char *wrptr );

#endif 

#if defined ( __18C242 ) || defined ( __18C252 ) || defined ( __18C442 ) || defined ( __18C452 ) || defined ( __18F242 ) || defined ( __18F252 ) || defined ( __18F442 ) || defined ( __18F452 ) || defined ( __18F248 ) || defined ( __18F258 ) || defined ( __18F448 ) || defined ( __18F458 ) || defined ( __18F2439 ) || defined ( __18F2539 ) || defined ( __18F4439 ) || defined ( __18F4539 ) || defined ( __18C601 ) || defined ( __18C801 ) || defined ( __18C658 ) || defined ( __18C858 ) || defined ( __18F2220 ) || defined ( __18F2320 ) || defined ( __18F4220 ) || defined ( __18F4320 ) || defined ( __18F2420 ) || defined ( __18F2520 ) || defined ( __18F4420 ) || defined ( __18F4520 ) || defined ( __18F2423 ) || defined ( __18F2523 ) || defined ( __18F4423 ) || defined ( __18F4523 ) || defined ( __18F2480 ) || defined ( __18F2580 ) || defined ( __18F4480 ) || defined ( __18F4580 ) || defined ( __18F2410 ) || defined ( __18F2510 ) || defined ( __18F2515 ) || defined ( __18F2610 ) || defined ( __18F4410 ) || defined ( __18F4510 ) || defined ( __18F4515 ) || defined ( __18F4610 ) || defined ( __18F2525 ) || defined ( __18F2620 ) || defined ( __18F4525 ) || defined ( __18F4620 ) || defined ( __18F2585 ) || defined ( __18F2680 ) || defined ( __18F4585 ) || defined ( __18F4680 ) || defined ( __18F2682 ) || defined ( __18F2685 ) || defined ( __18F4682 ) || defined ( __18F4685 ) || defined ( __18F2221 ) || defined ( __18F2321 ) || defined ( __18F4221 ) || defined ( __18F4321 ) || defined ( __18F24J10 ) || defined ( __18F25J10 ) || defined ( __18F44J10 ) || defined ( __18F45J10 )
#define SPI_IO_V1
#endif

#if defined ( __18F6620 ) || defined ( __18F6720 ) || defined ( __18F8620 ) || defined ( __18F8720 ) || defined ( __18F6520 ) || defined ( __18F8520 ) || defined ( __18F6310 ) || defined ( __18F6410 ) || defined ( __18F8310 ) || defined ( __18F8410 ) || defined ( __18F6390 ) || defined ( __18F6490 ) || defined ( __18F8390 ) || defined ( __18F8490 ) || defined ( __18F6527 ) || defined ( __18F6622 ) || defined ( __18F6627 ) || defined ( __18F6722 ) || defined ( __18F8527 ) || defined ( __18F8622 ) || defined ( __18F8627 ) || defined ( __18F8722 ) || defined ( __18F6585 ) || defined ( __18F6680 ) || defined ( __18F8585 ) || defined ( __18F8680 ) || defined ( __18F6525 ) || defined ( __18F6621 ) || defined ( __18F8525 ) || defined ( __18F8621 ) || defined ( __18F63J90 ) || defined ( __18F64J90 ) || defined ( __18F65J90 ) || defined ( __18F83J90 ) || defined ( __18F84J90 ) || defined ( __18F85J90 ) || defined ( __18F65J10 ) || defined ( __18F65J15 ) || defined ( __18F66J10 ) || defined ( __18F66J15 ) || defined ( __18F67J10 ) || defined ( __18F85J10 ) || defined ( __18F85J15 ) || defined ( __18F86J10 ) || defined ( __18F86J15 ) || defined ( __18F87J10 ) || defined ( __18F66J60 ) || defined ( __18F66J65 ) || defined ( __18F67J60 ) || defined ( __18F86J60 ) || defined ( __18F86J65 ) || defined ( __18F87J60 ) || defined ( __18F96J60 ) || defined ( __18F96J65 ) || defined ( __18F97J60 ) || defined ( __18F66J11 ) || defined ( __18F66J16 ) || defined ( __18F67J11 ) || defined ( __18F86J11 ) || defined ( __18F86J16 ) || defined ( __18F87J11 ) || defined ( __18F65J50 ) || defined ( __18F66J50 ) || defined ( __18F66J55 ) || defined ( __18F67J50 ) || defined ( __18F85J50 ) || defined ( __18F86J50 ) || defined ( __18F86J55 ) || defined ( __18F87J50 )
#define SPI_IO_V2
#endif

#if defined ( __18F2455 ) || defined ( __18F2550 ) || defined ( __18F4455 ) || defined ( __18F4550 ) ||\
	defined ( __18F2458 ) || defined ( __18F2553 ) || defined ( __18F4458 ) || defined ( __18F4553 )
#define SPI_IO_V3
#endif

#if defined ( __18F63J11 ) || defined ( __18F64J11 ) || defined ( __18F65J11 ) || defined ( __18F83J11 ) || defined ( __18F84J11 ) || defined ( __18F85J11 )
#define SPI_IO_V4
#endif

#if defined ( __18F2331 ) || defined ( __18F2431 ) || defined ( __18F4331 ) || defined ( __18F4431 )
#define SPI_IO_V5
#endif

#if defined ( __18F23K20 ) || defined ( __18F24K20 ) || defined ( __18F25K20 ) || defined ( __18F26K20 ) || defined ( __18F43K20 ) || defined ( __18F44K20 ) || defined ( __18F45K20 ) || defined ( __18F46K20 )
#define SPI_IO_V6
#endif

#if defined ( __18F6527 ) || defined ( __18F6622 ) || defined ( __18F6627 ) || defined ( __18F6722 ) || defined ( __18F8527 ) || defined ( __18F8622 ) || defined ( __18F8627 ) || defined ( __18F8722 ) || defined ( __18F65J10 ) || defined ( __18F65J15 ) || defined ( __18F66J10 ) || defined ( __18F66J15 ) || defined ( __18F67J10 ) || defined ( __18F85J10 ) || defined ( __18F85J15 ) || defined ( __18F86J10 ) || defined ( __18F86J15 ) || defined ( __18F87J10 ) || defined ( __18F96J60 ) || defined ( __18F96J65 ) || defined ( __18F97J60 ) || defined ( __18F66J11 ) || defined ( __18F66J16 ) || defined ( __18F67J11 ) || defined ( __18F86J11 ) || defined ( __18F86J16 ) || defined ( __18F87J11 ) || defined ( __18F65J50 ) || defined ( __18F66J50 ) || defined ( __18F66J55 ) || defined ( __18F67J50 ) || defined ( __18F85J50 ) || defined ( __18F86J50 ) || defined ( __18F86J55 ) || defined ( __18F87J50 )
#define SPI_IO_V7
#endif

#if defined ( __18F44J10 ) || defined ( __18F45J10 )
#define SPI_IO_V8
#endif

#if defined ( __18F13K50 ) || defined ( __18F14K50 )||\
    defined ( __18L13K50 ) || defined ( __18L14K50 ) ||\
    defined ( __18F13K20 ) || defined ( __18F14K20 )
#define SPI_IO_V9
#endif


#endif  /* __SPI_H */

