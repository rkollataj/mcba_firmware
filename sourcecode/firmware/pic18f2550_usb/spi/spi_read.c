#include <p18cxxx.h>
#include <spi.h>

/********************************************************************
*     Function Name:    ReadSPI                                     *
*     Return Value:     contents of SSPBUF register                 *
*     Parameters:       void                                        *
*     Description:      Read single byte from SPI bus.              *
********************************************************************/
#if defined (SPI_V1) || defined (SPI_V4)

unsigned char ReadSPI( void )
{
  SSPBUF = 0x00;                  // initiate bus cycle
  while ( !SSPSTATbits.BF );      // wait until cycle complete
  return ( SSPBUF );              // return with byte read 
}

#endif
