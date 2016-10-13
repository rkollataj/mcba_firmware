#include "queue.h"
#include "cpu.h"

void delay (void);

void initMsgQueue(tSPI_QUEUE *q)
{
unsigned char counter = 0;

while (counter < dNUMBER_OF_SPI_MSGS)
  {
  q->spiMsg[counter].status.empty = dTRUE;
  q->spiMsg[counter].status.full = dFALSE;
  counter++;
  }

  q->head = 0;
  q->tail = 0;
  q->numberOfMsgs = 0;
}

void addMsg2Queue(tSPI_QUEUE *q,unsigned char *b)
{
unsigned char counter;
unsigned char msgNumber;

counter = 0;
msgNumber = (unsigned char)q->tail;
while (counter < dSIZE_OF_SPI_MSG)
  {
  q->spiMsg[msgNumber].data[counter] = b[counter];
  counter++;
  }

//increment Msg Tail by one
  if (q->tail < (dNUMBER_OF_SPI_MSGS-1))
    {
    q->tail++;
    }
  else
    {
    q->tail=0;
    }

  q->numberOfMsgs++;
}


void removeMsgfromQueue(tSPI_QUEUE *q, unsigned char *data)
{
unsigned char counter;
unsigned char msgNumber;

counter = 0;
msgNumber = (unsigned char)q->head;

while (counter < dSIZE_OF_SPI_MSG)
    {
    data[counter] = q->spiMsg[msgNumber].data[counter];
    counter++;
    }

//increment Msg Head by one
  if (q->head < (dNUMBER_OF_SPI_MSGS-1))
    {
    q->head++;
    }
  else
    {
    q->head=0;
    }

  q->numberOfMsgs--;
}


//Used only for Master
//returns true for successful reception



unsigned char spiTransmitandReceiveMsg(unsigned char *txdata,unsigned char *rxdata)
{
  unsigned char counter;
  unsigned char SPIack;
  unsigned char spiState;

//This lets the CAN part know we are in the middle of transmitting

  SPIack = dSPI_MSG_NG;
  spiState = dRECEIVE_SPI_MESSAGE;

  LATAbits.LATA5 = 0;//Set slave select
  delay();
  LATBbits.LATB2 = 1; //This will cause a ECAN PINx interrupt
  delay();
  LATBbits.LATB2 = 0;
  delay();
  delay();
  delay();
  delay();

  while(spiState != dFINISHED)
    {
    switch (spiState)
      {
      case dRECEIVE_SPI_MESSAGE:
//        SSPBUF = 0x25; //header
//        while(!PIR1bits.SSPIF);
//        counter = SSPBUF;


//        SSPBUF = 0x11; //initate read
        counter = 0;
        while (counter < dMAX_SPI_PACKET_SIZE)
          {
          PIR1bits.SSPIF = 0;
          if (counter < dSIZE_OF_SPI_MSG)
            {
            SSPBUF = *txdata++;
            }
          else
            {
            SSPBUF = 0xFF;
            }
          while(!PIR1bits.SSPIF);
          *rxdata++ = SSPBUF;
          Nop();         
          Nop();
          Nop();
          Nop();
          Nop();
          counter++;
          }
        spiState = dFINISHED;
        delay();
        LATAbits.LATA5 = 1;
        spiState = dFINISHED;
        SPIack = dSPI_MSG_OK;
      break;



      default:
//This will release the CAN part from ISR
//        LATBbits.LATB2 = 1;
        LATAbits.LATA5 = 1;
        spiState = dFINISHED;
        SPIack = dSPI_MSG_NG; 
      break;
      }
    }

//This will release the CAN part from ISR
LATAbits.LATA5 = 1; //Clear slave select

return (SPIack);
}



void delay (void)
{
unsigned char count;

count = 0;
while (count < 5)
//while (count < 250)
  {
  count++;
  }
}
