#include "queue.h"
#include "cpu.h"

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


