#include <p18cxxx.h>
#include <spi.h>

/********************************************************************
*     Function Name:    WriteSPI                                    *
*     Return Value:     Status byte for WCOL detection.             *
*     Parameters:       Single data byte for SPI bus.               *
*     Description:      This routine writes a single byte to the    * 
*                       SPI bus.                                    *
********************************************************************/
#if defined (SPI_V1)

unsigned char WriteSPI( unsigned char data_out )
{
  SSPBUF = data_out;           // write byte to SSPBUF register
  if ( SSPCON1 & 0x80 )        // test if write collision occurred
   return ( -1 );              // if WCOL bit is set return negative #
  else
  {
    while( !SSPSTATbits.BF );  // wait until bus cycle complete 
  }
  return ( 0 );                // if WCOL bit is not set return non-negative#
}

#endif

#if defined (SPI_V4)

unsigned char WriteSPI( unsigned char data_out )
{
  SSPBUF = data_out;           // write byte to SSPBUF register
  if ( SSPCON & 0x80 )        // test if write collision occurred
   return ( -1 );              // if WCOL bit is set return negative #
  else
  {
    while( !SSPSTATbits.BF );  // wait until bus cycle complete 
  }
  return ( 0 );                // if WCOL bit is not set return non-negative#
}

#endif
