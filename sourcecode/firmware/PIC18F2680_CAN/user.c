/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        user.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original.
 * William Stuart		3/16/09
 ********************************************************************/

/** I N C L U D E S **********************************************************/

#include <string.h>

#include <p18cxxx.h>
#include <usart.h>
#include "spi.h"
#include "system\typedefs.h"

#include "system\usb\usb.h"

#include "io_cfg.h"             // I/O pin mapping
#include "user\user.h"
#include "user\temperature.h"
extern rom const unsigned char *rom USB_SD_Ptr[];

#pragma idata

/** V A R I A B L E S ********************************************************/
#pragma udata


#define dUSB_FW_VERSION_MAJOR 0x81
#define dUSB_FW_VERSION_MINOR 0x03



#define dGARBAGE_DATA 1
#define dHEADER1 2
#define dHEADER2 3
#define dMESSAGE_WIP 4




////////////////////////////////
//Send to USB
#define dCHANGE_BIT_RATE_RSP 0xE1
#define dTRANSMIT_MESSAGE_RSP 0xE2
#define dRECEIVE_MESSAGE 0xE3
#define dTRIGGER_EVENT 0xE4
#define dSETUP_TRIGGER_RSP 0xE5
#define dERROR_COUNT_STATUS 0xE6
#define dREAD_REGISTER_DIRECTLY_RSP 0xE7
#define dREAD_FW_VERSION_RSP 0xE8
#define dDEBUG_MODE_RSP 0xF0

//Recieve from USB
#define dCHANGE_BIT_RATE 0xA1
#define dTRANSMIT_MESSAGE_PD 0xA2
#define dTRANSMIT_MESSAGE_EV 0xA3
#define dSETUP_TRIGGER 0xA4
#define dWRITE_REGISTER_DIRECTLY 0xA5
#define dREAD_REGISTER_DIRECTLY 0xA6
#define dTOGGLE_LED 0xA7
#define dSETUP_TERMINATION_RESISTANCE 0xA8
#define dREAD_FW_VERSION 0xA9
#define dCAN_RESET 0xAA
#define dCHANGE_CAN_MODE 0xAB
#define dSETUP_HW_FILTER 0xAC
#define dDEBUG_MODE 0xB0

#define dNO_SPI_COMMAND 0x0

//Header to send to the CAN Part
#define dFIRST_HEADER 0xA5
#define dSECOND_HEADER 0xA1

//Header to send to the USB Part
#define dFIRST_HEADER_2USB 0xB6
#define dSECOND_HEADER_2USB 0xA2

#define dSIZEOF_READ_FW_VERSION_RSP 5
#define dSIZEOF_CHANGE_BIT_RATE_RSP	9
#define dSIZEOF_TRANSMIT_MESSAGE_RSP 7
#define dSIZEOF_RECEIVE_MESSAGE	19
#define dSIZEOF_TRIGGER_EVENT	25
#define dSIZEOF_SETUP_TRIGGER_RSP 4
#define dSIZEOF_ERROR_COUNT_STATUS	25
#define dSIZEOF_READ_REGISTER_DIRECTLY_RSP 19
#define dSIZEOF_DEBUG_MODE_RSP 4

#define dSIZEOF_READ_FW_VERSION 3
#define dSIZEOF_CHANGE_BIT_RATE	3
#define dSIZEOF_CHANGE_CAN_MODE 3
#define dSIZEOF_TOGGLE_LED 4 //3
#define dSIZEOF_SETUP_TERMINATION_RESISTANCE 3
#define dSIZEOF_TRANSMIT_MESSAGE_PD	19
#define dSIZEOF_TRANSMIT_MESSAGE_EV	19
#define dSIZEOF_SETUP_TRIGGER 9
#define dSIZEOF_SETUP_HW_FILTER 7
#define dSIZEOF_WRITE_REGISTER_DIRECTLY 5
#define dSIZEOF_READ_REGISTER_DIRECTLY 5
#define dSIZEOF_CAN_RESET 3
#define dSIZEOF_DEBUG_MODE	25
#define dSIZEOF_MAX_SIZE 25 
////////////////////////////////


unsigned char counter;
unsigned char trf_state;

DATA_PACKET dataPacket;

unsigned char receivedData;
unsigned char transmitData;
unsigned char SPI_checkSumTX;
unsigned char SPI_checkSumRX;
unsigned char SPI_BuildRXMessage;
unsigned char SPI_RXmessageComplete;

  unsigned char SPICommand1 = 0;
  unsigned char SPICommand2 = 0;
  unsigned char SPIDummy = 0;
  unsigned char transmitSPIState = 0;

//#define SPI_CS  PORTCbits.RC2 

#define dSIZEOF_SPI_MSG 20
#define dNUMBEROF_TX_SPI_MSG 4
#define dNUMBEROF_RX_SPI_MSG 6

//Not need now  unsigned char maxSize;
#define dALMOST_HALF_FULL_RXSSPBUF 60
#define dALMOST_HALF_FULL_RXSSPBUF_Minus2 50
#define dALMOST_FULL_RXSSPBUF 100

#define dSIZE_OF_RXSSPBUF 120
  unsigned char appRXSSPBUF[dSIZE_OF_RXSSPBUF];
  unsigned char tempRXSSPBUF;
  unsigned char numberOFBytesInSSPUF;
  unsigned char HeadIndexRX;
  unsigned char TailIndexRX;
  unsigned char newSPIBYTE;

#define dSIZE_OF_TXSSPBUF 25 //Maybe could be 21???
  unsigned char appTXSSPBUF[dSIZE_OF_TXSSPBUF];
  unsigned char numberOFBytesInTXmsg;
  unsigned char HeadIndexTX;
  unsigned char TailIndexTX;
  unsigned char newSPITXMsg;

unsigned char imAliveCounter;
unsigned char tempIMALIVE0;
unsigned char tempIMALIVE1;
unsigned char tempIMALIVE2;
unsigned char tempIMALIVE3;
unsigned char tempIMALIVE4;
unsigned char tempIMALIVE5;
unsigned char tempIMALIVE6;
unsigned char tempIMALIVE7;


////////////////// USED for SPI RX and TX
typedef struct {
//    unsigned char PENDING;
  union {
    unsigned char byte[dSIZEOF_SPI_MSG];
    struct {
    unsigned char SPI_DLC;
    unsigned char SPI_CMD;
    unsigned char EIDH;
    unsigned char EIDL;
    unsigned char SIDH;
    unsigned char SIDL;
    unsigned char DLC;
    unsigned char D0;
    unsigned char D1;
    unsigned char D2;
    unsigned char D3;
    unsigned char D4;
    unsigned char D5;
    unsigned char D6;
    unsigned char D7;
    unsigned char Time_3; //For RX Message Timestamp //For TX ID
    unsigned char Time_2; //For RX Message Timestamp //For TX Period
    unsigned char Time_1; //For RX Message Timestamp //For TX Period
    unsigned char Time_0; //For RX Message Timestamp //For TX Period Repeat
    unsigned char SPI_CHECKSUM;
    }st1;
    struct {
    unsigned char SPI_DLC;
    unsigned char SPI_CMD;
    unsigned char CAN_BUS_BITRATE;
    unsigned char TX_CAN_ERRORCNT;
    unsigned char RX_CAN_ERRORCNT;
    unsigned char CAN_BUS_OFF;
    unsigned char SPI_CHECKSUM;
    unsigned char dummy0;
    unsigned char dummy1;
    unsigned char dummy2;
    unsigned char dummy3;
    unsigned char dummy4;
    unsigned char dummy5;
    unsigned char dummy6;
    unsigned char dummy7;
    unsigned char dummy8;
    unsigned char dummy9;
    unsigned char dummy10;
    unsigned char dummy11;
    unsigned char dummy12;
    }st2;
    struct {
    unsigned char SPI_DLC;
    unsigned char SPI_CMD;
    unsigned char TRIGGER1_a;
    unsigned char TRIGGER1_b;
    unsigned char TRIGGER1_c;
    unsigned char TRIGGER1_d;
    unsigned char TRIGGER2_a;
    unsigned char TRIGGER2_b;
    unsigned char TRIGGER2_c;
    unsigned char TRIGGER2_d;
    unsigned char SPI_CHECKSUM;
    unsigned char dummy0;
    unsigned char dummy1;
    unsigned char dummy2;
    unsigned char dummy3;
    unsigned char dummy4;
    unsigned char dummy5;
    unsigned char dummy6;
    unsigned char dummy7;
    unsigned char dummy8;
    }st3;
    struct { //Toggle LED or Bitrate
    unsigned char SPI_DLC;
    unsigned char SPI_CMD;
    unsigned char DATA;
    unsigned char SPI_CHECKSUM;
    unsigned char dummy0;
    unsigned char dummy1;
    unsigned char dummy2;
    unsigned char dummy3;
    unsigned char dummy4;
    unsigned char dummy5;
    unsigned char dummy6;
    unsigned char dummy7;
    unsigned char dummy8;
    unsigned char dummy9;
    unsigned char dummy10;
    unsigned char dummy11;
    unsigned char dummy12;
    unsigned char dummy13;
    unsigned char dummy14;
    unsigned char dummy15;
    }st4;
    struct {
    unsigned char SPI_DLC;
    unsigned char SPI_CMD;
    unsigned int  TXIDtrackingNumber;
    unsigned char Time_3;
    unsigned char Time_2;
    unsigned char Time_1;
    unsigned char Time_0;
    unsigned char SPI_CHECKSUM;
    unsigned char dummy0;
    unsigned char dummy1;
    unsigned char dummy2;
    unsigned char dummy3;
    unsigned char dummy4;
    unsigned char dummy5;
    unsigned char dummy6;
    unsigned char dummy7;
    unsigned char dummy8;
    unsigned char dummy9;
    unsigned char dummy10;
    unsigned char dummy11;
    }st5;
    struct { //Toggle LED
    unsigned char SPI_DLC;
    unsigned char SPI_CMD;
    unsigned char DATA1;
    unsigned char DATA2;
    unsigned char SPI_CHECKSUM;
    unsigned char dummy0;
    unsigned char dummy1;
    unsigned char dummy2;
    unsigned char dummy3;
    unsigned char dummy4;
    unsigned char dummy5;
    unsigned char dummy6;
    unsigned char dummy7;
    unsigned char dummy8;
    unsigned char dummy9;
    unsigned char dummy10;
    unsigned char dummy11;
    unsigned char dummy12;
    unsigned char dummy13;
    unsigned char dummy14;
    }st6;
  }un;
} sSPIMESSAGE;


//First byte in array gives the 
sSPIMESSAGE SPI_generalTransmit[dNUMBEROF_TX_SPI_MSG];
sSPIMESSAGE SPI_generalReceive[dNUMBEROF_RX_SPI_MSG];

unsigned char SPI_WIP_RXCommandBuffer[dSIZEOF_SPI_MSG]; //The plus one is for the SPI_DLC
unsigned char SPI_WIP_TXCommandBuffer[dSIZEOF_SPI_MSG]; //The plus one is for the SPI_DLC
unsigned char USBDATA[64]; //dSIZEOF_SPI_MSG_WITH_USB_WRAPPER    #define dSIZEOF_SPI_MSG_WITH_USB_WRAPPER (dSIZEOF_SPI_MSG+2)

unsigned char SPI_NumberOfRXMsg;
unsigned char SPI_FirstLocationOfRXMsg;
unsigned char SPI_FirstFreeLocationOfRXMsg;

unsigned char SPI_NumberOfTXMsg;
unsigned char SPI_FirstLocationOfTXMsg;
unsigned char SPI_FirstFreeLocationOfTXMsg;

unsigned char SPI_TXByteCnt;
unsigned char SPI_RXByteCnt;

unsigned char DATAFORUSB = 0;
unsigned char DATAFORCAN = 0;
unsigned char InternalUSBSlot = 0;
unsigned char tempOffset = 0;

unsigned char ledTestOngoing;

unsigned char wakeUpCounter;

/** P R I V A T E  P R O T O T Y P E S ***************************************/

void handleSPI(void);
unsigned char SPI_messageTooTransmit(void);
void SPI_transmitLastestByte(void);
void SPI_transmitDummyByte(void);
void SPI_addReceiveToSPIQueue(void);
void SPI_addTransmitToSPIQueue(void);
unsigned char SPI_checkRXChecksum(void);
unsigned char SPI_messageReceived(void);
void SPI_BuildReceiveMessage(void);
void SPI_ProcessSPIFromReceiveQueue(void);
void ServiceRequests(void);
void updateInternalUSBSlot(void);

unsigned char writeEEPROM(unsigned char address,unsigned char data);
unsigned char readEEPROM(unsigned char address);

#define dEEPROM_IN_USE_ADDRESS 0
#define dEEPROM_CAN_TERMINATION_ADDRESS 1


//unsigned char tempCounter;
void highISR (void)
{
  if ((PIR1bits.SSPIF == 1) && (PIE1bits.SSPIE == 1))
    {
  while (PORTAbits.RA5 == 0)
  {


    if (SSPSTATbits.BF)
      {
if (ledTestOngoing == FALSE)
    LATCbits.LATC0 = 1;


      if (numberOFBytesInSSPUF < (dSIZE_OF_RXSSPBUF))
        {

        appRXSSPBUF[HeadIndexRX] = SSPBUF;
        numberOFBytesInSSPUF++;

        if (HeadIndexRX < (dSIZE_OF_RXSSPBUF-1))
          {
          HeadIndexRX++;
          }
        else
          {
          HeadIndexRX = 0;
          }
        }

      if (newSPITXMsg == TRUE)
        {
        SSPBUF = appTXSSPBUF[HeadIndexTX];

        if (HeadIndexTX < numberOFBytesInTXmsg)
          {
          HeadIndexTX++;
          }
        else
          {
          HeadIndexTX = 0;
          newSPITXMsg = FALSE;
          }
        }
      else
        {
        SSPBUF = 0x7F;
        }
      }

if (ledTestOngoing == FALSE)
      LATCbits.LATC0 = 0;

    }//end of while
  }

    PIR1bits.SSPIF = 0;
}

void UserInit(void)
{
unsigned char tempCount = 0;

   TRISCbits.TRISC0 = 0;
   TRISCbits.TRISC1 = 0;


   TRISAbits.TRISA5 = 1; //Slave Select in

   TRISAbits.TRISA1 = 1;  //used to sense USB power on / off 
   TRISAbits.TRISA2 = 1;  //used to sense 9v power on / off 
   TRISBbits.TRISB5 = 0;  //used to turn on the power the CAN from the USB

   TRISBbits.TRISB2 = 0;
   TRISBbits.TRISB3 = 0;

//For older Rev Boards (Before the mod)
//TRISAbits.TRISA5 = 0;  //used for the termination resistor
//LATAbits.LATA5 = 0; //OL


//For Newer Rev Boards
TRISCbits.TRISC6 = 0;  //used for the termination resistor

LATCbits.LATC6 = readEEPROM(dEEPROM_CAN_TERMINATION_ADDRESS);

//LATCbits.LATC6 = 0; //180 ohms
////LATCbits.LATC6 = 1; //OL

//Should Clear out array  unsigned char appRXSSPBUF[dSIZE_OF_RXSSPBUF];
 tempRXSSPBUF = 0;
 numberOFBytesInSSPUF = 0;
 HeadIndexRX = 0;
 TailIndexRX = 0;
 newSPIBYTE = FALSE;
//Should Clear out array  unsigned char appTXSSPBUF[dSIZE_OF_TXSSPBUF];
 numberOFBytesInTXmsg = 0;
 HeadIndexTX = 0;
 newSPITXMsg = FALSE;

  tempCount = 0;
  while (tempCount < 64)
    {
	USBDATA[tempCount] = 0;
    dataPacket._byte[tempCount] = 0;
	tempCount++;
	}

  tempCount = 0;
  while (tempCount < dSIZE_OF_RXSSPBUF)
    {
	appRXSSPBUF[tempCount] = 0;
	tempCount++;
	}

  imAliveCounter = 0;
  tempIMALIVE0 = 0;
  tempIMALIVE1 = 0;
  tempIMALIVE2 = 0;
  tempIMALIVE3 = 0;
  tempIMALIVE4 = 0;
  tempIMALIVE5 = 0;
  tempIMALIVE6 = 0;
  tempIMALIVE7 = 0;


  wakeUpCounter = 0;

   ADCON1 = 0x0F;
   OpenSPI(SLV_SSON, MODE_01, SMPMID);
//OpenSPI(SLV_SSON, MODE_01, SMPEND);

PIR1bits.SSPIF = 0;
PIE1bits.SSPIE = 1;

DATAFORUSB = 0;
DATAFORCAN = 0;
InternalUSBSlot = 0;

SPI_NumberOfTXMsg = 0;
SPI_NumberOfRXMsg = 0;
SPI_TXByteCnt = 0;
SPI_RXByteCnt = 0;

ledTestOngoing = FALSE;

  transmitData = 0;

  SPI_NumberOfRXMsg = 0;
  SPI_FirstLocationOfRXMsg = 0;
  SPI_FirstFreeLocationOfRXMsg = 0;

  SPI_NumberOfTXMsg = 0;
  SPI_FirstLocationOfTXMsg = 0;
  SPI_FirstFreeLocationOfTXMsg = 0;

  SPI_RXmessageComplete = FALSE;

  SPI_checkSumTX = 0;
  SPI_checkSumRX = 0;
  SPI_TXByteCnt = 0;
  SPI_RXByteCnt = 0;
  transmitSPIState = 0;

  SPI_BuildRXMessage = dGARBAGE_DATA;

SPI_generalTransmit[0].un.st1.SPI_CMD = dNO_SPI_COMMAND;
SPI_generalTransmit[1].un.st1.SPI_CMD = dNO_SPI_COMMAND;
SPI_generalReceive[0].un.st1.SPI_CMD = dNO_SPI_COMMAND;
SPI_generalReceive[1].un.st1.SPI_CMD = dNO_SPI_COMMAND;
SPI_generalReceive[2].un.st1.SPI_CMD = dNO_SPI_COMMAND;


}//end UserInit


void ProcessIO(void)
{
//Check 9 volts powered ... if not then power the ECAN part or not
if (PORTAbits.RA2 == 0)
  {
  if (PORTAbits.RA1 == 0)
    {
    //The CAN part is not being powered by 9v ... need to power it by USB
    LATBbits.LATB5 = 1;
    }
  else
    {
    LATBbits.LATB5 = 0; //Turn on the Power
    }
  }
else
  {
//the 9 volts is powered ... so turn off the USB power to the ECAN
  LATBbits.LATB5 = 1;
  }


   if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1))
     {
	 INTCONbits.GIE = 0;
	 INTCONbits.PEIE = 0;
     wakeUpCounter = 0;
     LATCbits.LATC0 = 0;
     LATCbits.LATC1 = 0;
     return;
     }


   handleSPI();
   INTCONbits.PEIE = 1;
   INTCONbits.GIE = 1;

   ServiceRequests();

//
////On the 50th time through turn on SPI in ISR
//if (wakeUpCounter > 50)
//  {
//
//   
//  }
//else
//  {
//  LATCbits.LATC0 = 0;
//  wakeUpCounter++;
//  }
}//end ProcessIO


rom const unsigned char DEV_NAME[]="CAN BUS ANALYZER";
void ServiceRequests(void)
{
    unsigned char index;
    unsigned char tempCounter;
	unsigned char i;
	unsigned char dtLen;
	unsigned char rndStart;
	static unsigned char ContDataSend=0;
	static unsigned char DataCounter=0;
	static unsigned int WESDataCounter=0;


    
    if(USBGenRead((unsigned char*)&dataPacket,sizeof(dataPacket)))
	{
if (ledTestOngoing == FALSE)
LATCbits.LATC1 = 1;
        counter = 0;
        switch(dataPacket.CMD)
        {
            case READ_FW_VER:


if (dataPacket._byte[1] == 1)
  {
  if (DATAFORUSB != 1)
    {
	tempOffset = InternalUSBSlot*20;
    USBDATA[0+tempOffset] = 0x20; //Debug Response;
    USBDATA[1+tempOffset] = 4;    //This is USB DLC ... means next n bytes belongs to this control message
    USBDATA[2+tempOffset] = 1; //Denotes data from the USB part
    USBDATA[3+tempOffset] = dUSB_FW_VERSION_MAJOR;
    USBDATA[4+tempOffset] = dUSB_FW_VERSION_MINOR;
    updateInternalUSBSlot();
    }
//  else
//    {
//If we get here that is because the USB write is busy...
//We should schedule this for process later
//    }
  }
else if (dataPacket._byte[1] == 2)
  {
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_READ_FW_VERSION;
SPI_WIP_TXCommandBuffer[1] = dREAD_FW_VERSION;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2];

SPI_WIP_TXCommandBuffer[dSIZEOF_READ_FW_VERSION] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
  }
                break;

//            case UPDATE_LED:
//                // LED1 & LED2 are used as USB event indicators.
//                if(dataPacket.led_num == 3)
//                {
////WES                    mLED_3 = dataPacket.led_status;
//                    counter = 0x01;
//                }//end if
//                else if(dataPacket.led_num == 4)
//                {
////WES                    mLED_4 = dataPacket.led_status;
//                    counter = 0x01;
//                }//end if else
//                break;
              
            case RESET:

//				EnterBootLoader=0x77;
				//Turn Off USB Module				
//				UCFG=0x00;
				//Delay ~300ms before Reset
//				TMR0H=0;
//				TMR0L=0;
//				while(TMR0L<200 || TMR0H<50)
//				{
					//mLED_4_Toggle();
//				}
//                Reset();
                break;
			
			case RD_DATA:
				dtLen=dataPacket._byte[1];
				dataPacket._byte[1]=TMR0H;
				dataPacket._byte[2]=TMR0L;
				rndStart=TMR0L;
				for(i=3;i<dtLen;i++)//Started @1
				{
					dataPacket._byte[i]=rndStart;
					rndStart++;
				}
				counter=dtLen;
				break;
            case RD_DATA_CONT:
				ContDataSend=1;
				break;
            case RD_DATA_CONT_END:
				ContDataSend=0;
				break;
            case RD_DEV_NAME:
				strcpypgm2ram(&(dataPacket._byte[1]),DEV_NAME);
				counter=sizeof(DEV_NAME);
				break;



			case READ_REGISTER_DIRECTLY:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_READ_REGISTER_DIRECTLY;
SPI_WIP_TXCommandBuffer[1] = dREAD_REGISTER_DIRECTLY;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1]; //Address High
SPI_WIP_TXCommandBuffer[3] = dataPacket._byte[2]; //Address Low
SPI_WIP_TXCommandBuffer[4] = dataPacket._byte[3]; //Read Flag

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2] +
                 SPI_WIP_TXCommandBuffer[3] +
                 SPI_WIP_TXCommandBuffer[4];

SPI_WIP_TXCommandBuffer[dSIZEOF_READ_REGISTER_DIRECTLY] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
				break;

			case WRITE_REGISTER_DIRECTLY:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_WRITE_REGISTER_DIRECTLY;
SPI_WIP_TXCommandBuffer[1] = dWRITE_REGISTER_DIRECTLY;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1]; //Address High
SPI_WIP_TXCommandBuffer[3] = dataPacket._byte[2]; //Address Low
SPI_WIP_TXCommandBuffer[4] = dataPacket._byte[3]; //Data

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2] +
                 SPI_WIP_TXCommandBuffer[3] +
                 SPI_WIP_TXCommandBuffer[4];

SPI_WIP_TXCommandBuffer[dSIZEOF_WRITE_REGISTER_DIRECTLY] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
				break;

			case LED_TOGGLE:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_TOGGLE_LED;
SPI_WIP_TXCommandBuffer[1] = dTOGGLE_LED;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];
SPI_WIP_TXCommandBuffer[3] = dataPacket._byte[2];

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2] +
                 SPI_WIP_TXCommandBuffer[3];

SPI_WIP_TXCommandBuffer[dSIZEOF_TOGGLE_LED] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
				break;

            case SETUP_TERMINATION_RESISTANCE:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_SETUP_TERMINATION_RESISTANCE;
SPI_WIP_TXCommandBuffer[1] = dSETUP_TERMINATION_RESISTANCE;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];;

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2];

SPI_WIP_TXCommandBuffer[dSIZEOF_SETUP_TERMINATION_RESISTANCE] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
				break;

            case RESET_CAN:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_CAN_RESET;
SPI_WIP_TXCommandBuffer[1] = dCAN_RESET;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2];

SPI_WIP_TXCommandBuffer[dSIZEOF_CAN_RESET] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
                break;


            case SET_CAN_BITRATE_REQ:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_CHANGE_BIT_RATE;
SPI_WIP_TXCommandBuffer[1] = dCHANGE_BIT_RATE;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2];

SPI_WIP_TXCommandBuffer[dSIZEOF_CHANGE_BIT_RATE] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
                break;

            case WR_TO_CAN_TX_PERIODIC_REQ:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_TRANSMIT_MESSAGE_PD;
SPI_WIP_TXCommandBuffer[1] = dTRANSMIT_MESSAGE_PD;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];
SPI_WIP_TXCommandBuffer[3] = dataPacket._byte[2];
SPI_WIP_TXCommandBuffer[4] = dataPacket._byte[3];
SPI_WIP_TXCommandBuffer[5] = dataPacket._byte[4];
SPI_WIP_TXCommandBuffer[6] = dataPacket._byte[5];
SPI_WIP_TXCommandBuffer[7] = dataPacket._byte[6];
SPI_WIP_TXCommandBuffer[8] = dataPacket._byte[7];
SPI_WIP_TXCommandBuffer[9] = dataPacket._byte[8];
SPI_WIP_TXCommandBuffer[10] = dataPacket._byte[9];
SPI_WIP_TXCommandBuffer[11] = dataPacket._byte[10];
SPI_WIP_TXCommandBuffer[12] = dataPacket._byte[11];
SPI_WIP_TXCommandBuffer[13] = dataPacket._byte[12];
SPI_WIP_TXCommandBuffer[14] = dataPacket._byte[13];
SPI_WIP_TXCommandBuffer[15] = dataPacket._byte[14];
SPI_WIP_TXCommandBuffer[16] = dataPacket._byte[15];
SPI_WIP_TXCommandBuffer[17] = dataPacket._byte[16];
SPI_WIP_TXCommandBuffer[18] = dataPacket._byte[17];

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2] +
                 SPI_WIP_TXCommandBuffer[3] +
                 SPI_WIP_TXCommandBuffer[4] +
                 SPI_WIP_TXCommandBuffer[5] +
                 SPI_WIP_TXCommandBuffer[6] +
                 SPI_WIP_TXCommandBuffer[7] +
                 SPI_WIP_TXCommandBuffer[8] +
                 SPI_WIP_TXCommandBuffer[9] +
                 SPI_WIP_TXCommandBuffer[10] +
                 SPI_WIP_TXCommandBuffer[11] +
                 SPI_WIP_TXCommandBuffer[12] +
                 SPI_WIP_TXCommandBuffer[13] +
                 SPI_WIP_TXCommandBuffer[14] +
                 SPI_WIP_TXCommandBuffer[15] +
                 SPI_WIP_TXCommandBuffer[16] +
                 SPI_WIP_TXCommandBuffer[17] +
                 SPI_WIP_TXCommandBuffer[18];

SPI_WIP_TXCommandBuffer[dSIZEOF_TRANSMIT_MESSAGE_PD] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
                break;

            case WR_TO_CAN_TX_REQ:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_TRANSMIT_MESSAGE_EV;
SPI_WIP_TXCommandBuffer[1] = dTRANSMIT_MESSAGE_EV;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];
SPI_WIP_TXCommandBuffer[3] = dataPacket._byte[2];
SPI_WIP_TXCommandBuffer[4] = dataPacket._byte[3];
SPI_WIP_TXCommandBuffer[5] = dataPacket._byte[4];
SPI_WIP_TXCommandBuffer[6] = dataPacket._byte[5];
SPI_WIP_TXCommandBuffer[7] = dataPacket._byte[6];
SPI_WIP_TXCommandBuffer[8] = dataPacket._byte[7];
SPI_WIP_TXCommandBuffer[9] = dataPacket._byte[8];
SPI_WIP_TXCommandBuffer[10] = dataPacket._byte[9];
SPI_WIP_TXCommandBuffer[11] = dataPacket._byte[10];
SPI_WIP_TXCommandBuffer[12] = dataPacket._byte[11];
SPI_WIP_TXCommandBuffer[13] = dataPacket._byte[12];
SPI_WIP_TXCommandBuffer[14] = dataPacket._byte[13];
SPI_WIP_TXCommandBuffer[15] = dataPacket._byte[14];
SPI_WIP_TXCommandBuffer[16] = dataPacket._byte[15];
SPI_WIP_TXCommandBuffer[17] = dataPacket._byte[16];
SPI_WIP_TXCommandBuffer[18] = dataPacket._byte[17];

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2] +
                 SPI_WIP_TXCommandBuffer[3] +
                 SPI_WIP_TXCommandBuffer[4] +
                 SPI_WIP_TXCommandBuffer[5] +
                 SPI_WIP_TXCommandBuffer[6] +
                 SPI_WIP_TXCommandBuffer[7] +
                 SPI_WIP_TXCommandBuffer[8] +
                 SPI_WIP_TXCommandBuffer[9] +
                 SPI_WIP_TXCommandBuffer[10] +
                 SPI_WIP_TXCommandBuffer[11] +
                 SPI_WIP_TXCommandBuffer[12] +
                 SPI_WIP_TXCommandBuffer[13] +
                 SPI_WIP_TXCommandBuffer[14] +
                 SPI_WIP_TXCommandBuffer[15] +
                 SPI_WIP_TXCommandBuffer[16] +
                 SPI_WIP_TXCommandBuffer[17] +
                 SPI_WIP_TXCommandBuffer[18];

SPI_WIP_TXCommandBuffer[dSIZEOF_TRANSMIT_MESSAGE_EV] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();

                break;

            case SETUP_TRIGGER:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_SETUP_TRIGGER;
SPI_WIP_TXCommandBuffer[1] = dSETUP_TRIGGER;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];
SPI_WIP_TXCommandBuffer[3] = dataPacket._byte[2];
SPI_WIP_TXCommandBuffer[4] = dataPacket._byte[3];
SPI_WIP_TXCommandBuffer[5] = dataPacket._byte[4];
SPI_WIP_TXCommandBuffer[6] = dataPacket._byte[5];
SPI_WIP_TXCommandBuffer[7] = dataPacket._byte[6];
SPI_WIP_TXCommandBuffer[8] = dataPacket._byte[7];

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2] +
                 SPI_WIP_TXCommandBuffer[3] +
                 SPI_WIP_TXCommandBuffer[4] +
                 SPI_WIP_TXCommandBuffer[5] +
                 SPI_WIP_TXCommandBuffer[6] +
                 SPI_WIP_TXCommandBuffer[7] +
                 SPI_WIP_TXCommandBuffer[8];

SPI_WIP_TXCommandBuffer[dSIZEOF_SETUP_TRIGGER] = SPI_checkSumTX;
SPI_addTransmitToSPIQueue();

                break;

            case SET_CAN_MODE_REQ:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_CHANGE_CAN_MODE;
SPI_WIP_TXCommandBuffer[1] = dCHANGE_CAN_MODE;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1];

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2];

SPI_WIP_TXCommandBuffer[dSIZEOF_CHANGE_CAN_MODE] = SPI_checkSumTX;

SPI_addTransmitToSPIQueue();
                break;



            case SETUP_HW_FILTER:
SPI_WIP_TXCommandBuffer[0] = dSIZEOF_SETUP_HW_FILTER;
SPI_WIP_TXCommandBuffer[1] = dSETUP_HW_FILTER;
SPI_WIP_TXCommandBuffer[2] = dataPacket._byte[1]; //add / delete
SPI_WIP_TXCommandBuffer[3] = dataPacket._byte[2]; //
SPI_WIP_TXCommandBuffer[4] = dataPacket._byte[3]; //
SPI_WIP_TXCommandBuffer[5] = dataPacket._byte[4]; //
SPI_WIP_TXCommandBuffer[6] = dataPacket._byte[5]; //

SPI_checkSumTX = SPI_WIP_TXCommandBuffer[0] +
                 SPI_WIP_TXCommandBuffer[1] +
                 SPI_WIP_TXCommandBuffer[2] +
                 SPI_WIP_TXCommandBuffer[3] +
                 SPI_WIP_TXCommandBuffer[4] +
                 SPI_WIP_TXCommandBuffer[5] +
                 SPI_WIP_TXCommandBuffer[6];

SPI_WIP_TXCommandBuffer[dSIZEOF_SETUP_HW_FILTER] = SPI_checkSumTX;
SPI_addTransmitToSPIQueue();

                break;



            default:
                break;
        }//end switch()
if (ledTestOngoing == FALSE)
LATCbits.LATC1 = 0;
        if(counter != 0)
        {
            if(!mUSBGenTxIsBusy())
			{
                USBGenWrite((unsigned char*)&dataPacket,counter);
			}
        }//end if
    }//end if
	else
	{

    if(!mUSBGenTxIsBusy())
	  {

      if (ledTestOngoing == FALSE)
        LATCbits.LATC1 = 1;

      if (DATAFORUSB == 1)
        {
        INTCONbits.GIE = 0;
        InternalUSBSlot = 4;

        DATAFORUSB = 0;


//USB HEADER
        dataPacket._byte[0]=USBDATA[0];
        dataPacket._byte[1]=USBDATA[1];
//CAN HEADER
        dataPacket._byte[2]= USBDATA[2];
        dataPacket._byte[3]= USBDATA[3];
        dataPacket._byte[4]= USBDATA[4];
        dataPacket._byte[5]= USBDATA[5];
        dataPacket._byte[6]= USBDATA[6];
        dataPacket._byte[7]= USBDATA[7];
        dataPacket._byte[8]= USBDATA[8];
        dataPacket._byte[9]= USBDATA[9];
        dataPacket._byte[10]= USBDATA[10];
        dataPacket._byte[11]= USBDATA[11];
        dataPacket._byte[12]= USBDATA[12];
        dataPacket._byte[13]= USBDATA[13];
        dataPacket._byte[14]= USBDATA[14];
        dataPacket._byte[15]= USBDATA[15];
        dataPacket._byte[16]= USBDATA[16];
        dataPacket._byte[17]= USBDATA[17];
        dataPacket._byte[18]= USBDATA[18];
        dataPacket._byte[19]= USBDATA[19];

//USB HEADER
        dataPacket._byte[20]= USBDATA[20];
        dataPacket._byte[21]= USBDATA[21];
//CAN HEADER
        dataPacket._byte[22]= USBDATA[22];
        dataPacket._byte[23]= USBDATA[23];
        dataPacket._byte[24]= USBDATA[24];
        dataPacket._byte[25]= USBDATA[25];
        dataPacket._byte[26]= USBDATA[26];
        dataPacket._byte[27]= USBDATA[27];
        dataPacket._byte[28]= USBDATA[28];
        dataPacket._byte[29]= USBDATA[29];
        dataPacket._byte[30]= USBDATA[30];
        dataPacket._byte[31]= USBDATA[31];
        dataPacket._byte[32]= USBDATA[32];
        dataPacket._byte[33]= USBDATA[33];
        dataPacket._byte[34]= USBDATA[34];
        dataPacket._byte[35]= USBDATA[35];
        dataPacket._byte[36]= USBDATA[36];
        dataPacket._byte[37]= USBDATA[37];
        dataPacket._byte[38]= USBDATA[38];
        dataPacket._byte[39]= USBDATA[39];

//USB HEADER
        dataPacket._byte[40]= USBDATA[40];
        dataPacket._byte[41]= USBDATA[41];
//CAN HEADER
        dataPacket._byte[42]= USBDATA[42];
        dataPacket._byte[43]= USBDATA[43];
        dataPacket._byte[44]= USBDATA[44];
        dataPacket._byte[45]= USBDATA[45];
        dataPacket._byte[46]= USBDATA[46];
        dataPacket._byte[47]= USBDATA[47];
        dataPacket._byte[48]= USBDATA[48];
        dataPacket._byte[49]= USBDATA[49];
        dataPacket._byte[50]= USBDATA[50];
        dataPacket._byte[51]= USBDATA[51];
        dataPacket._byte[52]= USBDATA[52];
        dataPacket._byte[53]= USBDATA[53];
        dataPacket._byte[54]= USBDATA[54];
        dataPacket._byte[55]= USBDATA[55];
        dataPacket._byte[56]= USBDATA[56];
        dataPacket._byte[57]= USBDATA[57];
        dataPacket._byte[58]= USBDATA[58];
        dataPacket._byte[59]= USBDATA[59];

        dataPacket._byte[60]= 0;
        dataPacket._byte[61]= 0;
        dataPacket._byte[62]= 0;
        dataPacket._byte[63]= 0;
        counter = 64;

        USBGenWrite((unsigned char*)&dataPacket,counter);

        counter = 0;
        while (counter < 64)
	      {
	      USBDATA[counter] = 0;
          counter++;
	      }

	    INTCONbits.GIE = 1;
        InternalUSBSlot = 0;

        }
      else
        {
        WESDataCounter++;
        }

        counter=0;
        if (ledTestOngoing == FALSE)
          LATCbits.LATC1 = 0;
      }
	}

}//end ServiceRequests



void handleSPI(void)
{

if (numberOFBytesInSSPUF > dALMOST_FULL_RXSSPBUF)
  {
//Set RB3 low
  PORTBbits.RB3 = 0;

//Pulse RB0
  PORTBbits.RB0 = 0;
  Nop();
  Nop();
  PORTBbits.RB0 = 1;
  Nop();
  Nop();
  PORTBbits.RB0 = 0;
  }
else if ((numberOFBytesInSSPUF > dALMOST_HALF_FULL_RXSSPBUF_Minus2) &&
         (numberOFBytesInSSPUF < dALMOST_HALF_FULL_RXSSPBUF))
  {
  PORTBbits.RB3 = 1;

//Pulse RB0
  PORTBbits.RB0 = 0;
  Nop();
  Nop();
  PORTBbits.RB0 = 1;
  Nop();
  Nop();
  PORTBbits.RB0 = 0;
  }
else
  {
  PORTBbits.RB3 = 1;
  }

INTCONbits.GIE = 0;
//Pull from the Head...
if (numberOFBytesInSSPUF > 0)
  {
//  if (SPI_BuildRXMessage == dGARBAGE_DATA)
//    {
////Try and consume as many 0x7F as possible
//     if(numberOFBytesInSSPUF > 0)
//      {
//      tempRXSSPBUF = appRXSSPBUF[TailIndexRX];
//        numberOFBytesInSSPUF--;
//        if (TailIndexRX < (dSIZE_OF_RXSSPBUF-1))
//          {
//          TailIndexRX++;
//          }
//        else
//          {
//          TailIndexRX = 0;
//          }
//      if (tempRXSSPBUF == 0x7F)
//        {
//
//        }
//      else
//        {
//        break;
//        }
//      }
//    }
//  else
//    {
//Get one byte
    tempRXSSPBUF = appRXSSPBUF[TailIndexRX];
    numberOFBytesInSSPUF--;
    if (TailIndexRX < (dSIZE_OF_RXSSPBUF-1))
      {
      TailIndexRX++;
      }
    else
      {
      TailIndexRX = 0;
      }
//    }

  newSPIBYTE = TRUE;
  }
else if (numberOFBytesInSSPUF > dSIZE_OF_RXSSPBUF)
  {
numberOFBytesInSSPUF = 0;
TailIndexRX = 0;
HeadIndexRX = 0;
  }
INTCONbits.GIE = 1;

      if ((SPI_messageTooTransmit() == TRUE) && (newSPITXMsg == FALSE))
        {
        //Add SPI msg to ISR
        SPI_transmitLastestByte();
        }


//////////////////////////////////////////////
//// Receive SPI message
//////////////////////////////////////////////
if (newSPIBYTE == TRUE)
  {
  newSPIBYTE = FALSE;
  SPI_BuildReceiveMessage();
  if (SPI_RXmessageComplete == TRUE)
    {
    SPI_RXmessageComplete = FALSE;
    SPI_BuildRXMessage = dGARBAGE_DATA;

//Check to see if this is a "Good" Spi message
        if (SPI_checkRXChecksum() == TRUE)
        {
//        if ((SPI_NumberOfRXMsg == 0) &&
//            (DATAFORUSB != 1) &&
//             (SPI_WIP_RXCommandBuffer[1] == dRECEIVE_MESSAGE))
//          {
////dRECEIVE_MESSAGE:
//
//  tempOffset = InternalUSBSlot*20;
//
//  USBDATA[0+tempOffset] = 0x02; //Received Memory Read Response
//  USBDATA[1+tempOffset] = 18; //This is USB DLC ... means next n bytes belongs to this control message
//  USBDATA[2+tempOffset] = SPI_WIP_RXCommandBuffer[2];//EIDH;
//  USBDATA[3+tempOffset] = SPI_WIP_RXCommandBuffer[3];//EIDL;
//  USBDATA[4+tempOffset] = SPI_WIP_RXCommandBuffer[4];//SIDH;
//  USBDATA[5+tempOffset] = SPI_WIP_RXCommandBuffer[5];//SIDL;
//  USBDATA[6+tempOffset] = SPI_WIP_RXCommandBuffer[6];//DLC;
//  USBDATA[7+tempOffset] = SPI_WIP_RXCommandBuffer[7];//D0;
//  USBDATA[8+tempOffset] = SPI_WIP_RXCommandBuffer[8];//D1;
//  USBDATA[9+tempOffset] = SPI_WIP_RXCommandBuffer[9];//D2;
//  USBDATA[10+tempOffset] = SPI_WIP_RXCommandBuffer[10];//D3;
//  USBDATA[11+tempOffset] = SPI_WIP_RXCommandBuffer[11];//D4;
//  USBDATA[12+tempOffset] = SPI_WIP_RXCommandBuffer[12];//D5;
//  USBDATA[13+tempOffset] = SPI_WIP_RXCommandBuffer[13];//D6;
//  USBDATA[14+tempOffset] = SPI_WIP_RXCommandBuffer[14];//D7;
//  USBDATA[15+tempOffset] = SPI_WIP_RXCommandBuffer[15];//Time_3;
//  USBDATA[16+tempOffset] = SPI_WIP_RXCommandBuffer[16];//Time_2;
//  USBDATA[17+tempOffset] = SPI_WIP_RXCommandBuffer[17];//st1.Time_1;
//  USBDATA[18+tempOffset] = SPI_WIP_RXCommandBuffer[18];//st1.Time_0;
//
//  updateInternalUSBSlot();
//
//////Dump this data quickly straight into the USB array
//          }
//        else
//          {
          SPI_addReceiveToSPIQueue();
//          }
        }
      }
  }  


INTCONbits.GIE = 0;
if (SPI_messageReceived() == TRUE)
  {
//Just after a Good message consume at least dSIZE_OF_RXSSPBUF bytes

   while(numberOFBytesInSSPUF > 0)
    {
    if (appRXSSPBUF[TailIndexRX] == 0x7F)
      {
      numberOFBytesInSSPUF--;

      if (TailIndexRX < (dSIZE_OF_RXSSPBUF-1))
        {
        TailIndexRX++;
        }
      else
        {
        TailIndexRX = 0;
        }
      }
    else
      {
      break;
      }
    }
  SPI_ProcessSPIFromReceiveQueue();
  }
INTCONbits.GIE = 1;
}


////////////////////////////////////

void SPI_BuildReceiveMessage(void)
{
//We are currently receiving a command

switch (SPI_BuildRXMessage)
  {
  case dGARBAGE_DATA:
    SPI_RXByteCnt = 0; //Force to zero to store first header
    SPI_WIP_RXCommandBuffer[SPI_RXByteCnt] = 0;//tempRXSSPBUF;
    if (tempRXSSPBUF == dFIRST_HEADER_2USB)
      {
      SPI_BuildRXMessage = dHEADER1;
      }
  break;

  case dHEADER1:
    SPI_RXByteCnt = 0; //Force to zero to store first header
    SPI_WIP_RXCommandBuffer[SPI_RXByteCnt] = 0;//tempRXSSPBUF;
  if (tempRXSSPBUF == dSECOND_HEADER_2USB)
    {
//We received the 2nd header Successfully
    SPI_BuildRXMessage = dHEADER2;
    }
  else
    {
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }
  break;

  case dHEADER2:

//This for first databyte
  SPI_BuildRXMessage = dMESSAGE_WIP;
  case dMESSAGE_WIP:

  SPI_WIP_RXCommandBuffer[SPI_RXByteCnt] = tempRXSSPBUF;
  SPI_RXByteCnt++;


  if ((SPI_WIP_RXCommandBuffer[1] == dCAN_RESET) && (SPI_RXByteCnt > dSIZEOF_CAN_RESET))
    {
    //Message should be done... so check the checksum.
    SPI_RXmessageComplete = TRUE;
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }
  else if ((SPI_WIP_RXCommandBuffer[1] == dRECEIVE_MESSAGE) && (SPI_RXByteCnt > dSIZEOF_RECEIVE_MESSAGE))
    {
    //Message should be done... so check the checksum.
    SPI_RXmessageComplete = TRUE;
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }
  else if ((SPI_WIP_RXCommandBuffer[1] == dREAD_FW_VERSION_RSP) && (SPI_RXByteCnt > dSIZEOF_READ_FW_VERSION_RSP))
    {
    //Message should be done... so check the checksum.
    SPI_RXmessageComplete = TRUE;
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }
  else if ((SPI_WIP_RXCommandBuffer[1] == dDEBUG_MODE_RSP) && (SPI_RXByteCnt > dSIZEOF_DEBUG_MODE_RSP))
    {
    //Message should be done... so check the checksum.
    SPI_RXmessageComplete = TRUE;
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }
  else if ((SPI_WIP_RXCommandBuffer[1] == dREAD_REGISTER_DIRECTLY_RSP) && (SPI_RXByteCnt > dSIZEOF_READ_REGISTER_DIRECTLY_RSP))
    {
    //Message should be done... so check the checksum.
    SPI_RXmessageComplete = TRUE;
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }
  else if ((SPI_WIP_RXCommandBuffer[1] == dCHANGE_BIT_RATE_RSP) && (SPI_RXByteCnt > dSIZEOF_CHANGE_BIT_RATE_RSP))
    {
    //Message should be done... so check the checksum.
    SPI_RXmessageComplete = TRUE;
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }
  else if ((SPI_WIP_RXCommandBuffer[1] == dTRANSMIT_MESSAGE_RSP) && (SPI_RXByteCnt > dSIZEOF_TRANSMIT_MESSAGE_RSP))
    {
    //Message should be done... so check the checksum.
    SPI_RXmessageComplete = TRUE;
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }

  else
    {
    SPI_RXmessageComplete = FALSE;
    }



  if (SPI_RXByteCnt > dSIZEOF_MAX_SIZE)
    {
    SPI_RXmessageComplete = FALSE;
    SPI_BuildRXMessage = dGARBAGE_DATA;
    }

  break;
  default:
  //This is garage data
  SPI_BuildRXMessage = dGARBAGE_DATA;
  break;
  }
}


unsigned char SPI_messageReceived(void)
  {
  if (SPI_NumberOfRXMsg > 0)
    {
    return TRUE;
    }
  else
    {
    return FALSE;
    }
  }

/////////////////////////////////////


unsigned char SPI_messageTooTransmit(void)
  {
  if (SPI_NumberOfTXMsg > 0)
    {
    return TRUE;
    }
  else
    {
    return FALSE;
    }
  }

/////////////////////////////////////

void SPI_transmitLastestByte(void)
  {
  appTXSSPBUF[0] = dFIRST_HEADER;
  appTXSSPBUF[1] = dSECOND_HEADER;
   
  SPI_TXByteCnt = 0;
  
  while (SPI_TXByteCnt <= SPI_generalTransmit[SPI_FirstLocationOfTXMsg].un.byte[0]) //SPI DLC
	    {
	    appTXSSPBUF[SPI_TXByteCnt+2] = SPI_generalTransmit[SPI_FirstLocationOfTXMsg].un.byte[SPI_TXByteCnt];
	    SPI_TXByteCnt++;
	    }

  SPI_NumberOfTXMsg--;

        
        if (SPI_FirstLocationOfTXMsg < (dNUMBEROF_TX_SPI_MSG - 1))
          {
          SPI_FirstLocationOfTXMsg++;
          }
        else
          {
          SPI_FirstLocationOfTXMsg = 0;
          }

        numberOFBytesInTXmsg = SPI_TXByteCnt+1;
	    SPI_TXByteCnt = 0;
        //TXHeadIndex = 0;
        newSPITXMsg = TRUE;
  }   



////Check to see if current message is still PENDING if so finish transmission
//  if (SPI_generalTransmit[SPI_FirstLocationOfTXMsg].un.st1.SPI_CMD != dNO_SPI_COMMAND)
//    {
////transmit next unsigned char
//     switch(transmitSPIState)
//     {
//     case 0: //first header
//       SSPBUF = dFIRST_HEADER;
//       transmitSPIState = 1;
//     break;
//
//     case 1: //second header
//       SSPBUF = dSECOND_HEADER;
//       transmitSPIState = 2;
//     break;
//
//     case 2: //second header
//	 if(SPI_TXByteCnt <= SPI_generalTransmit[SPI_FirstLocationOfTXMsg].un.byte[0]) //SPI DLC
//	    {
//	    SSPBUF = SPI_generalTransmit[SPI_FirstLocationOfTXMsg].un.byte[SPI_TXByteCnt];
//	    SPI_TXByteCnt++;
//	    }
//	  else
//	    {
//        SPI_generalTransmit[SPI_FirstLocationOfTXMsg].un.st1.SPI_CMD = dNO_SPI_COMMAND;
//	    SPI_NumberOfTXMsg--;
//	    SPI_TXByteCnt = 0;
//	    transmitSPIState = 0;
//        
//        if (SPI_FirstLocationOfTXMsg < (dNUMBEROF_TX_SPI_MSG - 1))
//          {
//          SPI_FirstLocationOfTXMsg++;
//          }
//        else
//          {
//          SPI_FirstLocationOfTXMsg = 0;
//          }
//
//
//	    }
//     break;
//     }
//  }
//else
//  {
////should get here
////TEST 2
//  }

/////////////////////////////////////

void SPI_transmitDummyByte(void)
  {
//Transmit a dummy SPI Byte
  SSPBUF = 0x7E;
  }



///////////////////////
// USB to SPI TX to CAN
///////////////////////

void SPI_addTransmitToSPIQueue(void)
  {
  unsigned char tempCounter;
//Check to see if this message was only for the USB part... if so do not add to SPI queue

/////////////////////////////////////////////
  unsigned char addUSB2SPI = FALSE;



  //Parce through the received data only pass it onto the CAN part if needed
  switch (SPI_WIP_TXCommandBuffer[1])
  {
  case dCAN_RESET:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  case dCHANGE_BIT_RATE:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  case dCHANGE_CAN_MODE:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  case dTRANSMIT_MESSAGE_PD:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  case dTRANSMIT_MESSAGE_EV:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  case dSETUP_TRIGGER:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  case dWRITE_REGISTER_DIRECTLY:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  case dREAD_REGISTER_DIRECTLY:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  case dREAD_FW_VERSION:
                //Check to see which LED we want to toggle
  if (SPI_WIP_TXCommandBuffer[2] == 2)
    {
    DATAFORCAN = 1;
    addUSB2SPI = TRUE;
    }
  break;

  case dTOGGLE_LED:
                //Check to see which LED we want to toggle


  if (SPI_WIP_TXCommandBuffer[2] == 0)
    {
    LATCbits.LATC0 = 0; //LED1
    LATCbits.LATC1 = 0; //LED2

    DATAFORCAN = 1;
    addUSB2SPI = TRUE;
    ledTestOngoing = TRUE;

    }
  else if (SPI_WIP_TXCommandBuffer[2] == 1)
    {
    if (SPI_WIP_TXCommandBuffer[3] == 0)
      {
      LATCbits.LATC0 = 0; //LED1
      }
    else if (SPI_WIP_TXCommandBuffer[3] == 1)
      {
      LATCbits.LATC0 = 1; //LED1
      }
    }
  else if (SPI_WIP_TXCommandBuffer[2] == 2)
    {
    if (SPI_WIP_TXCommandBuffer[3] == 0)
      {
      LATCbits.LATC1 = 0; //LED2
      }
    else if (SPI_WIP_TXCommandBuffer[3] == 1)
      {
      LATCbits.LATC1 = 1; //LED2
      }
    }
  else if (SPI_WIP_TXCommandBuffer[2] == 10)
    {
    DATAFORCAN = 1;
    addUSB2SPI = TRUE;
    ledTestOngoing = FALSE;
    }
  else
    {
//                case 3: //LED6 RED
//                case 4: //LED6 GREEN
//                case 5: //LED6 BLUE
//                case 6: //LED7 RED
//                case 7: //LED7 GREEN
//                case 8: //LED7 BLUE

    DATAFORCAN = 1;
    addUSB2SPI = TRUE;
    }
  break;

  case dSETUP_TERMINATION_RESISTANCE:
//Internal to USB Part
    if (SPI_WIP_TXCommandBuffer[2] == 1)//OFF
      {
      LATCbits.LATC6 = 1;
      writeEEPROM(dEEPROM_CAN_TERMINATION_ADDRESS ,0x01);
      }
    if (SPI_WIP_TXCommandBuffer[2] == 0)//ON
      {
      LATCbits.LATC6 = 0;
      writeEEPROM(dEEPROM_CAN_TERMINATION_ADDRESS ,0x00);
      }
  DATAFORCAN = 0;
  addUSB2SPI = FALSE;
  break;

  case dSETUP_HW_FILTER:
  DATAFORCAN = 1;
  addUSB2SPI = TRUE;
  break;

  default:
//Shouldn't get here... flag the user
  DATAFORCAN = 0;
  addUSB2SPI = FALSE;
  break;
  }



  if (addUSB2SPI == TRUE)
  {
//Check to see if there is room for this new SPI message
  if (SPI_NumberOfTXMsg > (dNUMBEROF_TX_SPI_MSG - 1))
    {
//TEST 1
//overrun condition... should do something.  This means that there are 
//more SPI messages coming in faster then we can handle
    }
  else
    {

//copy the WIP to the Working, but don't sent the PENDING just yet
    tempCounter = 0;
    while (tempCounter <= SPI_WIP_TXCommandBuffer[0])
      {
          SPI_generalTransmit[SPI_FirstFreeLocationOfTXMsg].un.byte[tempCounter] = SPI_WIP_TXCommandBuffer[tempCounter];
          tempCounter++;
      } 

//adjust "SPI_FirstFreeLocationOfTXMsg" for next message
      SPI_NumberOfTXMsg++;

      if (SPI_FirstFreeLocationOfTXMsg < (dNUMBEROF_TX_SPI_MSG - 1))
        {
        SPI_FirstFreeLocationOfTXMsg++;
        }
      else
        {
        SPI_FirstFreeLocationOfTXMsg = 0; //wrap around to position zero
        }
    }
  }
}

void SPI_addReceiveToSPIQueue(void)
  {
  unsigned char tempCounter;

//Check to see if there is room for this new SPI message
//  if (SPI_NumberOfRXMsg < dNUMBEROF_RX_SPI_MSG)
  if (SPI_NumberOfRXMsg > (dNUMBEROF_RX_SPI_MSG - 1))
    {
//TEST 3
//overrun condition... should do something.  This means that there are 
//more SPI messages coming in faster then we can handle
    }
  else
    {
//copy the WIP to the Working, but don't sent the PENDING just yet
    tempCounter = 0;
    while (tempCounter <= SPI_WIP_RXCommandBuffer[0])
      {
      SPI_generalReceive[SPI_FirstFreeLocationOfRXMsg].un.byte[tempCounter] = SPI_WIP_RXCommandBuffer[tempCounter];
      tempCounter++;
      } 

//adjust "SPI_FirstFreeLocationOfTXMsg" for next message
//    SPI_FirstFreeLocationOfRXMsg++;
    SPI_NumberOfRXMsg++;
//    if (SPI_FirstFreeLocationOfRXMsg > (dNUMBEROF_RX_SPI_MSG - 1))
//      {
//      SPI_FirstFreeLocationOfRXMsg = 0;
//      }
      if (SPI_FirstFreeLocationOfRXMsg < (dNUMBEROF_RX_SPI_MSG - 1))
        {
        SPI_FirstFreeLocationOfRXMsg++;
        }
      else
        {
        SPI_FirstFreeLocationOfRXMsg = 0; //wrap around to position zero
        }
    }
  }


//////////////////////////////////////////

///////////////////////
//CAN to SPI RX to USB
///////////////////////
void SPI_ProcessSPIFromReceiveQueue(void)
  {
  if (DATAFORUSB != 1)
    {
  switch (SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.st1.SPI_CMD)
  {
  case dRECEIVE_MESSAGE:
  tempOffset = InternalUSBSlot*20;

  USBDATA[0+tempOffset] = 0x02; //Received Memory Read Response
  USBDATA[1+tempOffset] = 18; //This is USB DLC ... means next n bytes belongs to this control message
  USBDATA[2+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[2];
  USBDATA[3+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[3];
  USBDATA[4+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[4];
  USBDATA[5+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[5];
  USBDATA[6+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[6];
  USBDATA[7+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[7];
  USBDATA[8+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[8];
  USBDATA[9+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[9];
  USBDATA[10+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[10];
  USBDATA[11+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[11];
  USBDATA[12+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[12];
  USBDATA[13+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[13];
  USBDATA[14+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[14];
  USBDATA[15+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[15];
  USBDATA[16+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[16];
  USBDATA[17+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[17];
  USBDATA[18+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[18];

  updateInternalUSBSlot();
  break;


  case dSETUP_TRIGGER_RSP:
  tempOffset = InternalUSBSlot*20;

  USBDATA[0+tempOffset] = 0x55;
  USBDATA[1+tempOffset] = 3; //This is USB DLC ... means next n bytes belongs to this control message
  USBDATA[2+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[2];
  USBDATA[3+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[3];

  updateInternalUSBSlot();
  break;


  case dCHANGE_BIT_RATE_RSP: //Also called the I'm Alive Message
  tempOffset = InternalUSBSlot*20;

  USBDATA[0+tempOffset] = 0x45;
  USBDATA[1+tempOffset] = 9; //This is USB DLC ... means next n bytes belongs to this control message
  USBDATA[2+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[2];
  USBDATA[3+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[3];
  USBDATA[4+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[4];
  USBDATA[5+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[5];
  USBDATA[6+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[6];
  USBDATA[7+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[7];
  USBDATA[8+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[8];
  if (LATCbits.LATC6 == 1)
    {
    USBDATA[9+tempOffset] = 0x01; //Off
    }
  else
    {
    USBDATA[9+tempOffset] = 0x00; //On
    }

  updateInternalUSBSlot();

//  imAliveCounter = 0;
//  tempIMALIVE2 = USBDATA[2+tempOffset];
//  tempIMALIVE3 = USBDATA[3+tempOffset];
//  tempIMALIVE4 = USBDATA[4+tempOffset];
//  tempIMALIVE5 = USBDATA[5+tempOffset];
//  tempIMALIVE6 = USBDATA[6+tempOffset];
//  tempIMALIVE7 = USBDATA[7+tempOffset];
//  tempIMALIVE7 = USBDATA[8+tempOffset];
//  tempIMALIVE7 = USBDATA[9+tempOffset];
  break;

  case dTRANSMIT_MESSAGE_RSP:
  tempOffset = InternalUSBSlot*20;

  USBDATA[0+tempOffset] = 0x40;
  USBDATA[1+tempOffset] = 6; //This is USB DLC ... means next n bytes belongs to this control message
  USBDATA[2+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[2];
  USBDATA[3+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[3];
  USBDATA[4+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[4];
  USBDATA[5+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[5];
  USBDATA[6+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[6];

  updateInternalUSBSlot();
  break;

  case dDEBUG_MODE_RSP:
  tempOffset = InternalUSBSlot*20;

  USBDATA[0+tempOffset] = 0x30;  //Debug Response
  USBDATA[1+tempOffset] = 3; //This is USB DLC ... means next n bytes belongs to this control message
  USBDATA[2+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[2];
  USBDATA[3+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[3];

  updateInternalUSBSlot();
  break;

  case dREAD_FW_VERSION_RSP:
  tempOffset = InternalUSBSlot*20;

  USBDATA[0+tempOffset] = 0x20;  //Debug Response
  USBDATA[1+tempOffset] = 4; //This is USB DLC ... means next n bytes belongs to this control message
  USBDATA[2+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[2];
  USBDATA[3+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[3];
  USBDATA[4+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[4];

  updateInternalUSBSlot();
  break;

  case dREAD_REGISTER_DIRECTLY_RSP:
  tempOffset = InternalUSBSlot*20;

  USBDATA[0+tempOffset] = 0x0A; //Received Memory Read Response
  USBDATA[1+tempOffset] = 18; //This is USB DLC ... means next n bytes belongs to this control message
  USBDATA[2+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[2];
  USBDATA[3+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[3];
  USBDATA[4+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[4];
  USBDATA[5+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[5];
  USBDATA[6+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[6];
  USBDATA[7+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[7];
  USBDATA[8+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[8];
  USBDATA[9+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[9];
  USBDATA[10+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[10];
  USBDATA[11+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[11];
  USBDATA[12+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[12];
  USBDATA[13+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[13];
  USBDATA[14+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[14];
  USBDATA[15+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[15];
  USBDATA[16+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[16];
  USBDATA[17+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[17];
  USBDATA[18+tempOffset] = SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.byte[18];

  updateInternalUSBSlot();
  break;

  default:
//Shouldn't get here... flag the user
  break;
  }



//Done Processing message
//so increment pointer
//Clear command
//and decrement 
    SPI_generalReceive[SPI_FirstLocationOfRXMsg].un.st1.SPI_CMD = dNO_SPI_COMMAND;
    SPI_NumberOfRXMsg--;
    if (SPI_FirstLocationOfRXMsg < (dNUMBEROF_RX_SPI_MSG - 1))
      {
      SPI_FirstLocationOfRXMsg++;
      }
    else
      {
      SPI_FirstLocationOfRXMsg = 0; //wrap around to position zero
      }
    }//USB == 1
  }


/////////////////////////////////////

// 1,6,6,6,6,19
unsigned char SPI_checkRXChecksum(void)
  {
  unsigned char tempCount;
  unsigned char returnValue;
  unsigned char numberOfBytesToCheck;

  numberOfBytesToCheck = SPI_WIP_RXCommandBuffer[0];
  returnValue = FALSE;
  tempCount = 0;
  if (numberOfBytesToCheck > 0)
    {

    SPI_checkSumRX = 0;
    while (tempCount < numberOfBytesToCheck)
      {
      SPI_checkSumRX = SPI_checkSumRX + SPI_WIP_RXCommandBuffer[tempCount];
      tempCount++;
      }

//If checksum is ok then return TRUE
    if (SPI_checkSumRX == SPI_WIP_RXCommandBuffer[numberOfBytesToCheck])
      {
      returnValue = TRUE;
      }
    }

  return(returnValue);
  }

void updateInternalUSBSlot(void)
  {
  if (InternalUSBSlot == 0)
    {
    InternalUSBSlot = 1;
    }
  else if (InternalUSBSlot == 1)
    {
    InternalUSBSlot = 2;
    }
  else if (InternalUSBSlot == 2)
    {
    InternalUSBSlot = 0;
    DATAFORUSB = 1; //All 64 bytes are ready...
    }
  else
    {
  //Should only get here if USB is busy
    InternalUSBSlot = 0; //if so reset
    }
  }



unsigned char writeEEPROM(unsigned char address,unsigned char data)
{
  unsigned char save_int;

  EEADR = 0;

  EEADR = address;
  EEDATA = data;

  EECON1bits.EEPGD=0;                /* Point to DATA memory */
  EECON1bits.CFGS=0;                 /* Access EEPROM */
  EECON1bits.WREN=1;                 /* Enable writes */
  
  save_int = INTCONbits.GIE;
  INTCONbits.GIE  = 0;
  
    _asm
    MOVLW 	0x55                 /* Required */
    MOVWF 	EECON2,0             /* Write 55h Sequence */
    MOVLW 	0xAA
    MOVWF 	EECON2,0             /* Write 0AAh */
    _endasm

  EECON1bits.WR=1;                   /* Set WR bit to begin write */
  INTCONbits.GIE = save_int;


  while(EECON1bits.WR)
    {
    }

  EECON1bits.WREN =0;                /* Disable writes on write complete (EEIF set) */
  return (EECON1bits.WRERR);
}

unsigned char readEEPROM(unsigned char address)
{
  unsigned char temp;
	
//	TriggerWatchdog();

  EEADR	 = address;
  EECON1bits.EEPGD=0;		/* Point to DATA memory */
  EECON1bits.CFGS=0;		/* Access EEPROM */
  EECON1bits.RD=1; 		/* EEPROM Read */

  Nop();

  temp = EEDATA;
  EECON1bits.EEPGD=1;

  return (temp);
}