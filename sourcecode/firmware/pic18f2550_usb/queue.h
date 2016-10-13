#ifndef __QUEUE_H
#define __QUEUE_H

#define dSIZE_OF_SPI_MSG 20
#define dNUMBER_OF_SPI_MSGS 11
#define dMAX_SPI_PACKET_SIZE 61

#define dSPI_MSG_OK 0x55
#define dSPI_MSG_NG 0XFF

#define dRECEIVE_SPI_MESSAGE 0x1
#define dCHECK_SPI_MESSAGE 0x2
#define dRECEIVE_ACK_FROM_USB 0x3
#define dRECEIVE_ACK_FROM_CAN 0x3
#define dFINISHED 0x4

typedef volatile struct
{
    unsigned char data[dSIZE_OF_SPI_MSG];
    struct status_s {
	  unsigned char empty:1;        /** Queue empty flag */
	  unsigned char full:1;         /** Queue full flag */
    unsigned :6;
  }status;
} tSPI_MSG_OBJ;

typedef volatile struct
{
  tSPI_MSG_OBJ spiMsg[dNUMBER_OF_SPI_MSGS];
  unsigned char head;
  unsigned char tail;
  unsigned char numberOfMsgs;
} tSPI_QUEUE;


void initMsgQueue(tSPI_QUEUE *q);
void addMsg2Queue(tSPI_QUEUE *q,unsigned char *b);
void removeMsgfromQueue(tSPI_QUEUE *q, unsigned char *data);
unsigned char spiTransmitandReceiveMsg(unsigned char *txdata,unsigned char *rxdata);
