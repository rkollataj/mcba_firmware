#include "p18f2680.h"
#include "spi.h"
#include "queue.h"
#include "CAN_to_SPI.h"
#include "config_bits.h"



//------------------------------------------------------------------------------
//Function prototypes
//------------------------------------------------------------------------------
//ISR funtions
void Low_ISR(void);
void High_ISR(void);
//Other functions
void InitializeCAN(void);
void TransmitTX0(void);
void TransmitTX1(void);
void TransmitTX2(void);
void PopCANFIFO(void);
unsigned char CallReadRXwithEWIN(void);
void EnterSleep(void);
void delay(unsigned int count);
void delay_us(unsigned int count);
void InitializeSPI(void);
void InitializeCCP(void);
void InitializeTimer0(void);
void handleSPI(void);
void transmitIAmAlive(void);
void incrementIndex(unsigned char* index);
unsigned char writeEEPROM(unsigned char address,unsigned char data);
unsigned char readEEPROM(unsigned char address);
void UpdateCANBitRate(void);
void PrepareIAmAliveMessage(void);
void CheckForTXTrasnmitEcho(void);
void UpdateCANMode(void);
void UpdateLEDToggle(void);
void autoTriggerTest(void);


//------------------------------------------------------------------------------
//RAM Variables
//------------------------------------------------------------------------------
#pragma udata SECTION1
#define MAX_MESSAGES_BUFFERABLE 12u
CAN_MESSAGE CANMessageBuffer[MAX_MESSAGES_BUFFERABLE];
unsigned char DummyArray[20];
#pragma udata SECTION2
unsigned char* WritePointer;
unsigned char* ReadPointer;
unsigned char MessagesInBuffer;

#pragma udata SECTION3
#define MAX_USB_OUT_MESSAGES_BUFFERRABLE 13u
#define USB_MESSAGE_BUFFER_SIZE (unsigned int)((unsigned int)dSIZE_OF_SPI_MSG * (unsigned int)MAX_USB_OUT_MESSAGES_BUFFERRABLE)
unsigned char* USBMessageWritePointer;
unsigned char* USBMessageReadPointer;
unsigned char USBBytesInBuffer;
unsigned char USBMessageBuffer[USB_MESSAGE_BUFFER_SIZE];
unsigned char DummyArray2[20];

unsigned char transmitSPIState = 0;
unsigned char SetError10Flag = dFALSE;
unsigned char SetError15Flag = dFALSE;
unsigned int waitingOnMessage = 0;
unsigned char receivedData;
unsigned char transmitData;
unsigned char SPI_checkSumTX;
unsigned char SPI_checkSumRX;
unsigned char SPI_BuildRXMessage;
unsigned char SPI_RXmessageComplete;
unsigned int transmitCANcounter = 0;
unsigned int receiveCANcounter = 0;
unsigned int transmitSPIcounter = 0;
unsigned char globalCANMode;
unsigned int CANRxPacketsLostCount;


unsigned char tempTXB0_EIDH;
unsigned char tempTXB0_EIDL;
unsigned char tempTXB0_SIDH;
unsigned char tempTXB0_SIDL;
unsigned char tempTXB0_DLC;
unsigned char tempTXB0_D0;
unsigned char tempTXB0_D1;
unsigned char tempTXB0_D2;
unsigned char tempTXB0_D3;
unsigned char tempTXB0_D4;
unsigned char tempTXB0_D5;
unsigned char tempTXB0_D6;
unsigned char tempTXB0_D7;
unsigned char tempTxMsgPeriod;

unsigned char SPICommand1 = 0;
unsigned char SPICommand2 = 0;
unsigned char SPIDummy = 0;

//unsigned int tempTime3and2;

unsigned char tempTime_3;
unsigned char tempTime_2;
unsigned char tempTime_1;
unsigned char tempTime_0;
unsigned char tempTime_2_rx;
unsigned char tempTime_3_rx;
unsigned char periodicMsgIndex;
unsigned int TMR0Tick = 0;
unsigned int TXperiod_WIP;
unsigned int TXperiodCounter_WIP;
unsigned char CurrentPERCANID = 0;
unsigned char tempCounter1;
unsigned char tempCounter2;
unsigned char tempCounter3;
unsigned int ImAliveCounter1;
unsigned int ImAliveCounter2;

unsigned char DigitalInTestResult;
unsigned char UseHWFilter;
unsigned char HWFilterInclude;
unsigned char numberOfIdsInHWFilter;
unsigned char TXB0_TXID = 0xFF;
unsigned char TXB1_TXID = 0xFF;
unsigned char TXB2_TXID = 0xFF;
unsigned char B0_TXID = 0xFF;
unsigned char B1_TXID = 0xFF;
unsigned char B2_TXID = 0xFF;
unsigned char B3_TXID = 0xFF;
unsigned char B4_TXID = 0xFF;
unsigned char B5_TXID = 0xFF;    






// Trigger

unsigned char TriggerIndex;
unsigned char TriggerStatus1;
unsigned char TriggerStatus2;
unsigned char TempTriggerStatus;
unsigned char TriggerCount1;
unsigned char TriggerCount2;
unsigned char TriggerType1;
unsigned char TriggerType2;
unsigned char Trigger1_EIDH;
unsigned char Trigger1_EIDL;
unsigned char Trigger1_SIDH;
unsigned char Trigger1_SIDL;
unsigned char Trigger2_EIDH;
unsigned char Trigger2_EIDL;
unsigned char Trigger2_SIDH;
unsigned char Trigger2_SIDL;
unsigned int TriggerLEDCount1;
unsigned int TriggerLEDCount2;
unsigned char wasTrigger1Flag;
unsigned char wasTrigger2Flag;
unsigned char Trigger1isextended = dFALSE;
unsigned char Trigger2isextended = dFALSE;

unsigned char WriteRegAddressHigh;
unsigned char WriteRegAddressLow;
unsigned char WriteRegData;
unsigned int WriteRegAddress;
unsigned char* ptrAddress;

unsigned char transmitNewSPI = dFALSE;
unsigned char newSpiMessage = dFALSE;
unsigned char transmitNumberOfNewSPI = 0;

unsigned char processSpiBufferTX[dMAX_SPI_PACKET_SIZE];
unsigned char processSpiBufferRX[dSIZE_OF_SPI_MSG];

static unsigned char spiRXBufferISR[dSIZE_OF_SPI_MSG];
static unsigned char spiTXBufferISR[dSIZE_OF_SPI_MSG];


unsigned char wipArrayCounterISR;

unsigned char IAmAliveMessageBuffer[dSIZE_OF_SPI_MSG];

static unsigned char newSpiBytes;
unsigned char toggleCheck;

static tSPI_QUEUE spiTxQueue = {{(unsigned char*)0}};
static tSPI_QUEUE spiRxQueue = {{(unsigned char*)0}};

unsigned char tempSpiBuffer[dSIZE_OF_SPI_MSG];

unsigned char tempCounter;
unsigned char tempCheckSum;
unsigned char sizeOfPacket;

static unsigned char counter;
static unsigned char spiState;
unsigned int timeSinceLastSPITransmit;


#define dSIZE_OF_WIPARRAY 250u
#pragma udata WIP_MEM_1
unsigned char wipArrayCounter;
unsigned char wipArrayTail = 0;
unsigned char wipArrayHead = 0;
static unsigned char wipArray[dSIZE_OF_WIPARRAY];
#pragma udata WIP_MEM_2
unsigned char checkForLeadingFFs;
static unsigned char wipArrayISR[dSIZE_OF_WIPARRAY];


unsigned char NeedToSendAliveMessageBool;
unsigned char Counter1ms;
unsigned int TimeStampIntSafe;
unsigned long TimeStampLongSafe;
unsigned long TimeStampLongISR;

unsigned int CANBitRate;
unsigned char CANBitRateHi;
unsigned char CANBitRateLo;
unsigned char debugMode;
unsigned char newTriggerTestResult;
unsigned char triggerTestStepComplete;
unsigned char triggerTestStepResult;


#define KEEP_ALIVE_INTERVAL_MS  100u        //Value is in milliseconds
#define WAKE_UP_DELAY_MS        250u        //Value is in milliseconds

#define CheckForAndCorrectPointerWrapAround(pointer, startPoint, wrapPoint) {if(pointer >= wrapPoint){pointer = startPoint;}}
//WES SOME LOSS ...#define TMR_VALUE_1MS   (unsigned int)((unsigned long int)0x10000 - (unsigned int)10000u + 4u)   //Timer value to add to current count to get 1ms TMRxIF rate

#define TMR_VALUE_1MS   (unsigned int)((unsigned long int)0x10000 - (unsigned int)9949u)   //Timer value to add to current count to get 1ms TMRxIF rate







// interrupts
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
_asm GOTO High_ISR _endasm
}

#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
_asm GOTO Low_ISR _endasm
}


#pragma code
#pragma interruptlow Low_ISR
void Low_ISR(void)
{
    // not used
}




#pragma interruptlow High_ISR
void High_ISR(void)
{
    static unsigned char DummyVar;
    static unsigned char ByteCounter;
    static unsigned int TimerSnapshot;
    static unsigned char ReceivedByte;


    
    //Check for the RB0 PINxx interrupt
    if ((INTCONbits.INT0IE == 1)&&(INTCONbits.INT0IF == 1))
    {
        //Clear the bit we used to notify the USB we need to transmit
        LATBbits.LATB1 = 0;

        DummyVar = SSPBUF;  //Makes certain BF is clear.
        ByteCounter = 1;
        PIR1bits.SSPIF = 0;
       
        //Check if we have any data to send to the SPI Master device.
        if(NeedToSendAliveMessageBool == dTRUE)
        {
            //We need to send a "I am alive" packet to the USB host.
            SSPBUF = IAmAliveMessageBuffer[0];    //Load SSPBUF so the first byte is ready to be transmitted

            //Stay in ISR as long as USB is transmitting data
            while(PORTAbits.RA5 == 0)   //Loop while master has our SS asserted
            {                
                if(PIR1bits.SSPIF == 1)
                {
                    PIR1bits.SSPIF = 0;
                    if(ByteCounter < dSIZE_OF_SPI_MSG)
                    {
                        *USBMessageWritePointer++ = SSPBUF;
                        SSPBUF = IAmAliveMessageBuffer[ByteCounter];
                        USBBytesInBuffer++;
                        ByteCounter++;
                    }
                    else if(ByteCounter == dSIZE_OF_SPI_MSG)
                    {
                        *USBMessageWritePointer++ = SSPBUF;                       
                        USBBytesInBuffer++;
                        ByteCounter++;
                    }        
                    
                    //Make sure we don't receive way more data than expected...
                    //TO DO: Need to fix problem somewhere believed to exist around here...
                    if(USBBytesInBuffer >= USB_MESSAGE_BUFFER_SIZE)
                    {
                        CANRxPacketsLostCount++;
                        break;
                    }    
                }
            }//while(PORTAbits.RA5 == 0)            
            
            NeedToSendAliveMessageBool = dFALSE;          
        }    
        else if(MessagesInBuffer != 0)
        {
            //We need to send a regular message packet (ex: a CAN message)
            SSPBUF = *ReadPointer++;    //Load SSPBUF so the first byte is ready to be transmitted

            //Stay in ISR as long as USB is transmitting data
            while(PORTAbits.RA5 == 0)   //Loop while master has our SS asserted
            {                
                if(PIR1bits.SSPIF == 1)
                {
                    PIR1bits.SSPIF = 0;
                    //TO DO: Need to fix problem somewhere believed to exist around here...
                    if(ByteCounter < dSIZE_OF_SPI_MSG)
                    {
                        *USBMessageWritePointer++ = SSPBUF;
                        SSPBUF = *ReadPointer++;
                        USBBytesInBuffer++;
                        ByteCounter++;
                    }
                    else if(ByteCounter == dSIZE_OF_SPI_MSG)
                    {
                        *USBMessageWritePointer++ = SSPBUF;                       
                        USBBytesInBuffer++;
                        ByteCounter++;
                    }        
                    
                    //Make sure we don't receive way more data than expected...
                    if(USBBytesInBuffer >= USB_MESSAGE_BUFFER_SIZE)
                    {
                        CANRxPacketsLostCount++;
                        //In case we hit the break, make sure the read pointer advances by
                        //the proper amount for the whole message.
                        ReadPointer += dSIZE_OF_SPI_MSG - ByteCounter;                        
                        break;
                    }    
                }
            }//while(PORTAbits.RA5 == 0)
            MessagesInBuffer--;
        }
        else
        {
            //We don't have any messages to send to the USB host, but the host
            //still wants to send us something, so we had better receive it and
            //just respond back with pure 0xFF (letting the host know we had nothing
            //to send).
            SSPBUF = 0xFF;  //Load SSPBUF so the first byte is ready to be transmitted
            //We have no data to transmit to the SPI master.  We are only receiving data.
            while ((PORTAbits.RA5 == 0)) // && (SPI_timeCounter_ISR < (dMAX_SPI_MESSAGE_SIZE + dSPI_SIZE_BUFFER)))
            {
                if (SSPSTATbits.BF)
                {
                    *USBMessageWritePointer++ = SSPBUF; //Clears BF
                    if(ByteCounter < dSIZE_OF_SPI_MSG)
                    {
                        SSPBUF = 0xFF;
                        ByteCounter++;
                    }    
                    USBBytesInBuffer++;
                    
                    //Make sure we don't receive way more data than expected...
                    if(USBBytesInBuffer >= USB_MESSAGE_BUFFER_SIZE)
                    {
                        break;
                    }    
                }
            }
        }    
        
        //Check for pointer wraparound events
        CheckForAndCorrectPointerWrapAround(ReadPointer, (unsigned char*)&CANMessageBuffer[0], ((unsigned char*)&CANMessageBuffer[MAX_MESSAGES_BUFFERABLE] + sizeof(CAN_MESSAGE)));
        CheckForAndCorrectPointerWrapAround(USBMessageWritePointer, (unsigned char*)&USBMessageBuffer[0], (unsigned char*)&USBMessageBuffer[USB_MESSAGE_BUFFER_SIZE]);


        INTCONbits.INT0IF = 0;
    }//if ((INTCONbits.INT0IE == 1)&&(INTCONbits.INT0IF == 1))


    //Update Timestamp information
    //Just Poll this Timer 1
    if (PIR1bits.TMR1IF == 1)
    {
        //Update Timer for 1ms interrupt rate
        TimerSnapshot = TMR1L;
        *((unsigned char*)&TimerSnapshot + 1) = TMR1H;  //Ugly syntax method of quickly writing to MSB of timerSnapShot
        TimerSnapshot =+ (unsigned int)TMR_VALUE_1MS;
        TMR1H = (TimerSnapshot >> 8);
        TMR1L = (unsigned char)TimerSnapshot;
        
        //Add the msec to the counter
        PIR1bits.TMR1IF = 0;
		TimeStampLongISR++;
		if (TimeStampLongISR > 16777215) //16777.215 seconds or 4.6603375 hours
			{
			TimeStampLongISR = 0;
			}
        
        if(Counter1ms != 0xFF)
        {
            Counter1ms++;
        }    
    }
}


void main(void)
{
    static unsigned char i;
    static unsigned char SPIHeaderByte;
    static unsigned char MainLoopCounter;

	//Initialize production Test vars
	debugMode = dFALSE;
	newTriggerTestResult = 0;
	triggerTestStepComplete = 0;
	triggerTestStepResult = 0;

    
    //Initialize circular RAM FIFO pointers and variables for incoming CAN messages
    ReadPointer = (unsigned char*)&CANMessageBuffer[0];
    WritePointer = (unsigned char*)&CANMessageBuffer[0];
    MessagesInBuffer = 0;

    //Initialize circular RAM FIFO pointers and variables for incoming messages from USB host
    USBMessageWritePointer = (unsigned char*)&USBMessageBuffer;
    USBMessageReadPointer = (unsigned char*)&USBMessageBuffer;
    USBBytesInBuffer = 0;
    
    NeedToSendAliveMessageBool = dFALSE;
    Counter1ms = 0;
    TimeStampIntSafe = 0;
    MainLoopCounter = 0;
    CANRxPacketsLostCount = 0;
    
    //LEDs
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    
    TRISAbits.TRISA4 = 0;
    TRISCbits.TRISC0 = 0;
    
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    
    //?
    PORTCbits.RC2 = 1;
    TRISCbits.TRISC2 = 1;

    //Trigger LEDs
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    TRISCbits.TRISC0 = 0;
    TRISAbits.TRISA4 = 0;
    
    LATAbits.LATA2 = 0;
    LATAbits.LATA3 = 0;
    LATCbits.LATC0 = 0;
    LATAbits.LATA4 = 0;


    //Trigger 
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    TRISAbits.TRISA0 = 0; //Output
    TRISAbits.TRISA1 = 0; //Output

    //Trigger Init
    TriggerStatus1 = dTRIGGER_OFF;
    TriggerStatus2 = dTRIGGER_OFF;
    TriggerType1 = 0;
    TriggerCount1 = 0;
    TriggerLEDCount1 = 0;
    TriggerType2 = 0;
    TriggerCount2 = 0;
    TriggerLEDCount2 = 0;
    wasTrigger1Flag = dFALSE;
    wasTrigger2Flag = dFALSE;

    TRISBbits.TRISB0 = 1;

    //Bus Error LED
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;

    receiveCANcounter = 0;
    transmitSPIcounter = 0;

    tempTime_3 = 0;
    tempTime_2 = 0;
    tempTime_1 = 0;
    tempTime_0 = 0;
    tempTime_2_rx = 0;
    tempTime_3_rx = 0;
TimeStampLongISR = 0;
TimeStampLongSafe = 0;

    
    TriggerIndex = 0;
    TriggerStatus1 = 0;
    TriggerStatus2 = 0;
    TempTriggerStatus = 0;
    TriggerCount1 = 0;
    TriggerCount2 = 0;
    TriggerType1 = 0;
    TriggerType2 = 0;
    Trigger1_EIDH = 0;
    Trigger1_EIDL = 0;
    Trigger1_SIDH = 0;
    Trigger1_SIDL = 0;
    Trigger2_EIDH = 0;
    Trigger2_EIDL = 0;
    Trigger2_SIDH = 0;
    Trigger2_SIDL = 0;
    TriggerLEDCount1 = 0;
    TriggerLEDCount2 = 0;
    wasTrigger1Flag = 0;
    wasTrigger2Flag = 0;
    Trigger1isextended = dFALSE;
    Trigger2isextended = dFALSE;
    timeSinceLastSPITransmit = 0;

    InitializeSPI();
    TRISBbits.TRISB1 = 0;
    LATBbits.LATB1 = 0;
    
    
    InitializeCAN();
    InitializeCCP();
    
    newSpiBytes = dFALSE;
    wipArrayCounterISR = 0;
    wipArrayCounter = 0;
    

    //power up delay
    TimeStampLongSafe=0;
	while(TimeStampLongSafe < 50000)
    {
    TimeStampLongSafe++;
    }
	TimeStampLongSafe = 0;

    TXBIEbits.TXB0IE = 1;
    //Sets the interrupt to be triggered on a rising edge
    INTCON2bits.INTEDG0 = 1;
    //INTCONbits.RBIE = 1;
    INTCONbits.INT0IE = 1;
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;


    while(1)
    {
        //----------------------------------------------------------------------
        //Check if we have any CAN messages sitting in the hardware FIFO.  If so
        //read it and buffer it to RAM.
        //----------------------------------------------------------------------
        PopCANFIFO();
        
        //----------------------------------------------------------------------
        //Check if it is time to prepare to send a keep alive message to the USB host
        //----------------------------------------------------------------------
        if(Counter1ms >= KEEP_ALIVE_INTERVAL_MS)
        {
            Counter1ms = 0;
            PrepareIAmAliveMessage();
            INTCONbits.GIE = 0; //disable ISR
            NeedToSendAliveMessageBool = dTRUE; //Don't set this until after 
                                                //PrepareIAmAliveMessage() has been called
            LATBbits.LATB1 = 1; //Let USB micro know we have data waiting to send to it.          
            INTCONbits.GIE = 1; //enable ISR

//light up the LEDs
			if (debugMode == dFALSE)
			{
				//Bus Status
				if(COMSTATbits.TXBO == 0x01)
				{ //Red
					LATBbits.LATB4 = 1;
					LATBbits.LATB5 = 0;
				}
				else if ((RXERRCNT > 127) || (TXERRCNT > 127))
				{ //Yellow
					LATBbits.LATB4 = 1;
					LATBbits.LATB5 = 1;
				}
				else
				{ //Green
					LATBbits.LATB4 = 0;
					LATBbits.LATB5 = 1;
				}
			}


        }           
        
        //----------------------------------------------------------------------
        //Check if we need to prepare any echo back packets for when we send 
        //CAN TX messages.
        //----------------------------------------------------------------------
        CheckForTXTrasnmitEcho();

        //----------------------------------------------------------------------
        //Check if we have any received messages from the USB host that we
        //need to process
        //----------------------------------------------------------------------
        if(USBBytesInBuffer > 0)
        {
            //Fetch the first byte to determine the type of message we have to
            //process.
            SPIHeaderByte = *USBMessageReadPointer;
            
            
            //Check if the first byte is 0x55 (indicating a CAN message to transmit onto CAN bus
            switch(SPIHeaderByte)
            {
                //Recieve from USB
				//NOTE: dREAD_FW_VERSION is handled in the I'm alive message


                case 0xFF:
                    //In this case, we have a "non-packet" worth of data.  This is just
                    //a default value we get when the USB host has nothing to send us.
                    //Skip a packet worth of bytes
                    USBMessageReadPointer += dSIZE_OF_SPI_MSG;
                    INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                    USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                    INTCONbits.GIEH = 1;    //Re-enable interrupts
                    break;
                case dTRANSMIT_MESSAGE_EV:
                    if (globalCANMode == 0x00) //dMODE_NORMAL
                    {
    	                if(TXB0CONbits.TXREQ == 0)
	                    {
                        TransmitTX0();
                        INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                        USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                        INTCONbits.GIEH = 1;    //Re-enable interrupts
						}
						else if (TXB1CONbits.TXREQ == 0)
						{
                        TransmitTX1();
                        INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                        USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                        INTCONbits.GIEH = 1;    //Re-enable interrupts
						}
						else if (TXB2CONbits.TXREQ == 0)
						{
                        TransmitTX2();
                        INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                        USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                        INTCONbits.GIEH = 1;    //Re-enable interrupts
						}
						else
						{
                        INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                        USBMessageReadPointer += dSIZE_OF_SPI_MSG;
                        USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                        INTCONbits.GIEH = 1;    //Re-enable interrupts
						}
                    }
                    else if(USBBytesInBuffer >= (USB_MESSAGE_BUFFER_SIZE - dSIZE_OF_SPI_MSG))
                    {
                        //Even though we didn't transmit the packet, we should waste
                        //the data, so we can receive a new packet of data.  If the CAN
                        //bus gets locked due to error frames, we might not ever be able
                        //to transmit.  However, we still need to receive new packets from
                        //the host, so we better gobble up the data anyway.
                        INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                        USBMessageReadPointer += dSIZE_OF_SPI_MSG;
                        USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                        INTCONbits.GIEH = 1;    //Re-enable interrupts
                    }
                    break; 
                case dCHANGE_BIT_RATE:
                    //The user wants us to update the CAN bit rate
                    UpdateCANBitRate();
                    INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                    USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                    INTCONbits.GIEH = 1;    //Re-enable interrupts
                    break;
				case dCHANGE_CAN_MODE:
                    //The user wants us to change the CAN mode
                    UpdateCANMode();

                    INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                    USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                    INTCONbits.GIEH = 1;    //Re-enable interrupts
					break;

				case dDEBUG_MODE:
                    //The user wants us to enter Debug mode
					//This allows the user to control LEDs and TRIGGER input/output
					USBMessageReadPointer++; //Bounce out the Commmand byte
    				debugMode = *USBMessageReadPointer++;
                    if (debugMode == 0)
                    {
					//Just turn off MODE
					}
					else if (debugMode == 1)
					{
					//Just turn on MODE
					}
					else if (debugMode == 0xA)
					{
					debugMode = 1;
					//Run Trigger test
					autoTriggerTest();
					}
					else
					{
					}

                    INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                    USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                    INTCONbits.GIEH = 1;    //Re-enable interrupts
					break;



				case dTOGGLE_LED:
                    UpdateLEDToggle();

                    INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                    USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                    INTCONbits.GIEH = 1;    //Re-enable interrupts
					break;

                default:
                    USBMessageReadPointer += dSIZE_OF_SPI_MSG;
                    INTCONbits.GIEH = 0;    //Tempoarily disable interrupts for safe access to variable
                    USBBytesInBuffer -= dSIZE_OF_SPI_MSG;
                    INTCONbits.GIEH = 1;    //Re-enable interrupts                
                    break;                   
            }    
            
            CheckForAndCorrectPointerWrapAround(USBMessageReadPointer, (unsigned char*)&USBMessageBuffer[0], (unsigned char*)&USBMessageBuffer[USB_MESSAGE_BUFFER_SIZE])
        }    
        

        //----------------------------------------------------------------------
        //Check if we have any CAN messages sitting in RAM that are awaiting 
        //transmission over SPI to the USB host.
        //----------------------------------------------------------------------
        MainLoopCounter++;
        if(MainLoopCounter >= 2)
        {
            MainLoopCounter = 0;
            INTCONbits.GIE = 0; //disable ISR
            if(MessagesInBuffer != 0u)
            {
                LATBbits.LATB1 = 1; //Let USB micro know we have data waiting to send to it.
            }    
            INTCONbits.GIE = 1; //enable ISR
        }

    }//while(1)
}//void main(void)

void PrepareIAmAliveMessage(void)
{
    static unsigned char i;
    static unsigned char Checksum;
    static unsigned char TimeStampByte;
    
    IAmAliveMessageBuffer[0] = dI_AM_ALIVE;
    IAmAliveMessageBuffer[1] = TXERRCNT;
    IAmAliveMessageBuffer[2] = RXERRCNT;
    IAmAliveMessageBuffer[3] = COMSTATbits.RXBnOVFL; //overflow
    IAmAliveMessageBuffer[4] = COMSTATbits.TXBO; //busoff
    IAmAliveMessageBuffer[5] = CANBitRateHi;
    IAmAliveMessageBuffer[6] = CANBitRateLo;
    IAmAliveMessageBuffer[7] = CANRxPacketsLostCount;       //LSB 
    IAmAliveMessageBuffer[8] = CANRxPacketsLostCount >> 8;  //MSB
    IAmAliveMessageBuffer[9] = CANSTAT;

	IAmAliveMessageBuffer[10] = dCAN_FW_VERSION_MAJOR;
	IAmAliveMessageBuffer[11] = dCAN_FW_VERSION_MINOR;


//Variable data
	IAmAliveMessageBuffer[12] = debugMode; //maybe share this with Bus OFF
	IAmAliveMessageBuffer[13] = 0;
	IAmAliveMessageBuffer[14] = 0;


    if (newTriggerTestResult > 0)
	{
		IAmAliveMessageBuffer[12] = 0x81; //debugMode + TRIGGER Test
		IAmAliveMessageBuffer[13] = triggerTestStepComplete;
		IAmAliveMessageBuffer[14] = triggerTestStepResult;
		newTriggerTestResult--;
	}

//Unused Data
	IAmAliveMessageBuffer[15] = 0;
	IAmAliveMessageBuffer[16] = 0;
	IAmAliveMessageBuffer[17] = 0;

    //Compute checksum
    Checksum = 0;
    for(i = 1; i < (dSIZE_OF_SPI_MSG - 1); i++)   //Compute checksum on bytes 1-18 (inclusive of limits)
    {
        Checksum += IAmAliveMessageBuffer[i];
    }
    IAmAliveMessageBuffer[(dSIZE_OF_SPI_MSG - 1)] = Checksum;

        
}


void InitializeTimer0(void)
{
    //#define SYMBOLS_TO_TICKS(a) ((CLOCK_FREQ/1000 * a) / (SYMBOL_TO_TICK_RATE / 1000))
    
    T0CON = 0b01000010;
    
    //40 MHz 10 MIPS, each instruction executes in 100 nanoseconds
    
    INTCON2bits.TMR0IP = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    T0CONbits.TMR0ON = 1;
}


void InitializeCCP(void)
{
    TMR1H = TMR_VALUE_1MS >> 8;
    TMR1L = (unsigned char)TMR_VALUE_1MS;
    CCP1CON = 0x04;
    T1CON = 0x85;
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    
}

void InitializeSPI(void)
{
    ADCON1 = 0x0F;
    //SPI
    TRISCbits.TRISC4 = 1;  //SDI
    TRISCbits.TRISC5 = 0;  //SDO
    TRISCbits.TRISC3 = 1;  //SCK
    
    LATAbits.LATA5 = 1;
    TRISAbits.TRISA5 = 1;  //SS
    
    
    
    //CAN to USB > RB2 - RB0 (CAN)
    //USB to CAN > RB3 - RB1 (CAN)
    //  LATBbits.LATB1 = 1;
    //  TRISBbits.TRISB0 = 1;
    //  TRISBbits.TRISB1 = 1;
    
    //Works great No PLL        OpenSPI(SPI_FOSC_4,MODE_01,SMPMID);
    //OpenSPI(SPI_FOSC_4, MODE_01, SMPMID); //With PLL
    //OpenSPI(SLV_SSON, MODE_01, SMPMID); //With PLL
    
    //Slave Setup   OpenSPI(SLV_SSON, MODE_01, SMPMID);
    //        SSPCON1 |= SPI_FOSC_64;
    //OpenSPI(SPI_FOSC_64, MODE_00, SMPMID);
    
    //Slave mode, idle state of clock is low, xmit on idle to active transition.
    SSPSTAT = 0x00;
    SSPCON1 = 0x04;
    SSPCON1bits.SSPEN = 1;  //Enable SPI module
    
    PIR1bits.SSPIF = 0;
    //  PIE1bits.SSPIE = 1;
}   





//****************************************************************************
//
// Configure the CAN Module
//
//****************************************************************************
void InitializeCAN(void)
{
    ECANCON = 0x00;
    
    CANCON = 0x80;
    while(0x80 != (CANSTAT & 0xE0)); // wait until ECAN is in config mode
    
//FIFO Mode    ECANCON = 0x80; //Mode 2 FIFO mode
    ECANCON = 0x40; //Mode 1 Enhanced Legacy mode (Single Buffer used to get exact Timestamp)

    CIOCON = 0x30;
    
    //****************************************************************************
    // Initialize CAN Timing
    //****************************************************************************
   
    //Read EEPROM
    CANBitRateHi = readEEPROM(dEEPROM_CAN_BITRATE_HI_ADDRESS);
    CANBitRateLo = readEEPROM(dEEPROM_CAN_BITRATE_LO_ADDRESS);
    CANBitRate = 0;
    CANBitRate = CANBitRateHi;
    CANBitRate = (CANBitRate << 8);
	CANBitRate = (CANBitRate + CANBitRateLo);

    //CANBitRate = dCAN_1000KBPS_40MHZ;
    //CANBitRate = dCAN_100KBPS_40MHZ;
    
    switch(CANBitRate)
    {
 
        case dCAN_20_KBPS_40MHZ:
        BRGCON1 = 0x31;
        BRGCON2 = 0xBC;
        BRGCON3 = 0x05;
        break;

        case dCAN_33_3KBPS_40MHZ:
        BRGCON1 = 0x17;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_50KBPS_40MHZ:
        BRGCON1 = 0x13;
        BRGCON2 = 0xB7;
        BRGCON3 = 0x03;
        break;
        
        case dCAN_80KBPS_40MHZ:
        BRGCON1 = 0x09;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_83_3KBPS_40MHZ:
        BRGCON1 = 0x09;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x06;
        break;
        
        case dCAN_100KBPS_40MHZ:
        BRGCON1 = 0x13;
        BRGCON2 = 0xA0;
        BRGCON3 = 0x02;
        break;
        
        case dCAN_125KBPS_40MHZ:
        BRGCON1 = 0x07;
        BRGCON2 = 0xBA;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_150KBPS_40MHZ:
        BRGCON1 = 0x06;
        BRGCON2 = 0xAF;
        BRGCON3 = 0x03;
        break;
        
        case dCAN_175KBPS_40MHZ:
        BRGCON1 = 0x05;
        BRGCON2 = 0xAF;
        BRGCON3 = 0x03;
        break;
        
        case dCAN_200KBPS_40MHZ:
        BRGCON1 = 0x03;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_225KBPS_40MHZ:
        BRGCON1 = 0x03;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x04;
        break;
        
        case dCAN_250KBPS_40MHZ:
        BRGCON1 = 0x03;
        BRGCON2 = 0xBA;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_275KBPS_40MHZ:
        BRGCON1 = 0x02;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x06;
        break;
        
        case dCAN_300KBPS_40MHZ:
        BRGCON1 = 0x02;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x04;
        break;
        
        case dCAN_500KBPS_40MHZ:
        BRGCON1 = 0x01;
        BRGCON2 = 0xBA;
        BRGCON3 = 0x07;
        break;

        case dCAN_625KBPS_40MHZ:
        BRGCON1 = 0x03;
        BRGCON2 = 0x98;
        BRGCON3 = 0x01;
        break;

        case dCAN_800KBPS_40MHZ:
        BRGCON1 = 0x00;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_1000KBPS_40MHZ:
        BRGCON1 = 0x00;
        BRGCON2 = 0xBA;
        BRGCON3 = 0x07;
        break;
        
        default:
        BRGCON1 = 0x13;
        BRGCON2 = 0xA0;
        BRGCON3 = 0x02;
        CANBitRateHi = 0;    //dCAN_100KBPS_40MHZ
        CANBitRateLo = 0x64; //dCAN_100KBPS_40MHZ
        
        writeEEPROM(dEEPROM_CAN_BITRATE_HI_ADDRESS,CANBitRateHi);
        writeEEPROM(dEEPROM_CAN_BITRATE_LO_ADDRESS,CANBitRateLo);
        break;
    }
    
    
    //*****************************************************************************
    // Setup Programmable buffers
    //*****************************************************************************
    BSEL0 = 0x00;		//B0 - B5 are receive buffers
    
    //****************************************************************************
    // Initialize Receive Masks
    //****************************************************************************
    RXM0EIDH = 0x0;
    RXM0EIDL = 0x0;
    RXM0SIDH = 0x0;
    RXM0SIDL = 0x0;
    
    RXM1EIDH = 0x0;
    RXM1EIDL = 0x0;
    RXM1SIDH = 0x0;
    RXM1SIDL = 0x0;
    
    //****************************************************************************
    // Initialize Receive Filters
    //****************************************************************************
    RXF0EIDH = 0x0;
    RXF0EIDL = 0x0;
    RXF0SIDH = 0x0;
    RXF0SIDL = 0x0;
    
    RXF1EIDH = 0x0;
    RXF1EIDL = 0x0;
    RXF1SIDH = 0x0;
    RXF1SIDL = 0x0;
    
    RXF2EIDH = 0x0;
    RXF2EIDL = 0x0;
    RXF2SIDH = 0x0;
    RXF2SIDL = 0x0;
    
    RXF3EIDH = 0x0;
    RXF3EIDL = 0x0;
    RXF3SIDH = 0x0;
    RXF3SIDL = 0x0;
    
    RXF4EIDH = 0x0;
    RXF4EIDL = 0x0;
    RXF4SIDH = 0x0;
    RXF4SIDL = 0x0;
    
    RXF5EIDH = 0x0;
    RXF5EIDL = 0x0;
    RXF5SIDH = 0x0;
    RXF5SIDL = 0x0;
    
    //****************************************************************************
    // Enable Filters
    //****************************************************************************
//    RXFCON0 = 0x3F;     //Enable Filters 0-7
	RXFCON0 = 0x01;     //Enable Filter 0
    RXFCON1 = 0x00;     //Enable Filters 8-15
    //  RXFCON0 = 0x00;     //Enable Filters 0,1,2
    //  RXFCON1 = 0x00;     //Disable all others
    
    //****************************************************************************
    // Assign Filters to Masks
    //****************************************************************************
    MSEL0 = 0x00;     //Assign Filters 0-3 to Mask 0
    MSEL1 = 0x00;     //Assign Filters 4-7 to Mask 0
    MSEL2 = 0x00;     //Assign Filters 8-11 to Mask 1
    MSEL3 = 0x00;     //Assign Filters 12-15 to Mask 1

    //****************************************************************************
    //
    // Assign Filters to Buffers
    //
    //****************************************************************************
    RXFBCON0 = 0x00;     //Assign Filters 0 and 1 to RXB0
    RXFBCON1 = 0x00;     //Assign Filters 2 and 3 to RXB0
    RXFBCON2 = 0x00;     //Assign Filters 4 and 5 to RXB0
    RXFBCON3 = 0x00;     //Assign Filters 6 and 7 to RXB0
    RXFBCON4 = 0x00;     //Assign Filters 8 and 9 to RXB0
    RXFBCON5 = 0x00;     //Assign Filters 10 and 11 to RXB0
    RXFBCON6 = 0x00;     //Assign Filters 12 and 13 to RXB0
    RXFBCON7 = 0x00;     //Assign Filters 14 and 15 to RXB0
    
    
    // initialize I/O
    TRISB = 0xC9;		//RB3 = CANRX, RB4 & RB5 = push button
    PORTB = 0x04;
    TRISBbits.TRISB2 = 0;  //CAN TX
    //LEDs
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    //	INTCON2bits.NOT_RBPU = 0; //enable pullups on PORTB
    
    CANCON = 0x00;
    while(0x00 != (CANSTAT & 0xE0)); // wait until ECAN is in normal mode
    globalCANMode = 0x00; //Normal
    //****************************************************************************
    // Set Receive Mode
    //****************************************************************************
    
    RXB0CON = 0x00;
    RXB1CON = 0x00;
}


unsigned char writeEEPROM(unsigned char address,unsigned char data)
{
    unsigned char save_int;
    
    EEADRH = 0;
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
    
    while(EECON1bits.WR);
    
    EECON1bits.WREN = 0;                /* Disable writes on write complete (EEIF set) */
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



#define dTRIGGER_TEST_ONE 1
#define dTRIGGER_TEST_TWO 2
#define dTRIGGER_TEST_THREE 3
#define dTRIGGER_TEST_FOUR 4

void autoTriggerTest(void)
{
//Test Trigger input/output hardware
	unsigned char localTriggerTest;
    unsigned char didTestPass;

//    USBMessageReadPointer++;    //toss out the command byte
	localTriggerTest = *USBMessageReadPointer++;


	didTestPass = dFALSE;
    switch(localTriggerTest)
    {
		//Test 1 (Test Trigger One first as input looking for high)
	case dTRIGGER_TEST_ONE:
		//Setup
		LATAbits.LATA1 = 1;
		TRISAbits.TRISA0 = 1; //input
		TRISAbits.TRISA1 = 0; //output
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		//check
		if (PORTAbits.RA0 == 1)
		{
		didTestPass = dTRUE;
		}
		break;

	case dTRIGGER_TEST_TWO:
		//Setup
		LATAbits.LATA1 = 0;
		LATAbits.LATA0 = 0;
		TRISAbits.TRISA0 = 0; //output //drain what is there...
		TRISAbits.TRISA0 = 1; //input
		TRISAbits.TRISA1 = 0; //output
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		//check
		if (PORTAbits.RA0 == 0)
		{
		didTestPass = dTRUE;
		}
		break;

	case dTRIGGER_TEST_THREE:
		//Setup
		LATAbits.LATA0 = 1;
		TRISAbits.TRISA0 = 0; //output
		TRISAbits.TRISA1 = 1; //input
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		//check
		if (PORTAbits.RA1 == 1)
		{
		didTestPass = dTRUE;
		}
		break;

	case dTRIGGER_TEST_FOUR:
		//Setup
		LATAbits.LATA1 = 0;
		LATAbits.LATA0 = 0;
		LATAbits.LATA1 = 0;
		TRISAbits.TRISA1 = 0; //output //drain what is there...
		TRISAbits.TRISA1 = 1; //input
		TRISAbits.TRISA0 = 0; //output

		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		//check
		if (PORTAbits.RA1 == 0)
		{
		didTestPass = dTRUE;
		}
		break;

	default:
		didTestPass = dFALSE;
		break;
	}

newTriggerTestResult = 10; //This will send this result at least five times...
triggerTestStepComplete = localTriggerTest;
triggerTestStepResult = didTestPass;
}


#define dLED_ALL 0x55
#define dLED_CAN_RGB1 3
#define dLED_CAN_RGB2 4
#define dLED_CAN_RGB3 5

#define LED_STATE_ON 1
#define LED_STATE_OFF 2
#define LED_STATE_GREEN 3
#define LED_STATE_YELLOW 4
#define LED_STATE_RED 5


void UpdateLEDToggle(void)
{
	unsigned char localLED;
	unsigned char localLEDState;

    USBMessageReadPointer++;    //toss out the command byte
	USBMessageReadPointer++;    //toss out the PICmicro Selection

	localLED = *USBMessageReadPointer++;
	localLEDState = *USBMessageReadPointer++;


    switch(localLED)
    {
	case dLED_ALL:
		if (localLEDState == LED_STATE_ON)
		{
			//Green 1
			LATAbits.LATA3 = 1;
			LATAbits.LATA2 = 0;

			//Green 2
			LATCbits.LATC0 = 1;
			LATAbits.LATA4 = 0;

			//Green 3
			LATBbits.LATB4 = 0;
			LATBbits.LATB5 = 1;
		}
		else if (localLEDState == LED_STATE_OFF)
		{
		//Turn OFF ALL LEDs
		LATAbits.LATA3 = 0;
		LATAbits.LATA2 = 0;
		LATCbits.LATC0 = 0;
		LATAbits.LATA4 = 0;
		LATBbits.LATB4 = 0;
		LATBbits.LATB5 = 0;
		}
		else
		{
		}
		break;

	case dLED_CAN_RGB1:
		if (localLEDState == LED_STATE_GREEN)
		{
			//Green
			LATAbits.LATA3 = 1;
			LATAbits.LATA2 = 0;
		}
		else if (localLEDState == LED_STATE_YELLOW)
		{
			//Yellow
			LATAbits.LATA3 = 1;
			LATAbits.LATA2 = 1;
		}
		else if (localLEDState == LED_STATE_RED)
		{
			//Red
			LATAbits.LATA3 = 0;
			LATAbits.LATA2 = 1;
		}
		break;


	case dLED_CAN_RGB2:
		if (localLEDState == LED_STATE_GREEN)
		{
			//Green
			LATCbits.LATC0 = 1;
			LATAbits.LATA4 = 0;
		}
		else if (localLEDState == LED_STATE_YELLOW)
		{
			//Yellow
			LATCbits.LATC0 = 1;
			LATAbits.LATA4 = 1;
		}
		else if (localLEDState == LED_STATE_RED)
		{
			//Red
			LATCbits.LATC0 = 0;
			LATAbits.LATA4 = 1;
		}
		break;


	case dLED_CAN_RGB3:
		if (localLEDState == LED_STATE_GREEN)
		{
			//Green
			LATBbits.LATB4 = 0;
			LATBbits.LATB5 = 1;
		}
		else if (localLEDState == LED_STATE_YELLOW)
		{
			//Yellow
			LATBbits.LATB4 = 1;
			LATBbits.LATB5 = 1;
		}
		else if (localLEDState == LED_STATE_RED)
		{
			//Red
			LATBbits.LATB4 = 1;
			LATBbits.LATB5 = 0;
		}
		break;
	default:
		//Turn OFF ALL LEDs
		LATAbits.LATA3 = 0;
		LATAbits.LATA2 = 0;
		LATCbits.LATC0 = 0;
		LATAbits.LATA4 = 0;
		LATBbits.LATB4 = 0;
		LATBbits.LATB5 = 0;
		break;
	}
}


//------------------------------------------------------------------------------
//Update CAN Mode
//------------------------------------------------------------------------------
#define CAN_MODE_CONFIG 1
#define CAN_MODE_NORMAL 2
#define CAN_MODE_LISTEN_ONLY 3

void UpdateCANMode(void)
{
	unsigned char localCANmode;
    //toss out the command byte
    USBMessageReadPointer++;
    
    localCANmode = *USBMessageReadPointer++;

    switch(localCANmode)
    {
	case CAN_MODE_CONFIG:
		TXB0CONbits.TXREQ = 0;
		globalCANMode = 0x80;
    	CANCON = globalCANMode;
      	while(globalCANMode != (CANSTAT & 0xE0)); // wait until ECAN is in listen only mode
		break;

	case CAN_MODE_NORMAL:
		TXB0CONbits.TXREQ = 0;
		globalCANMode = 0x00;
    	CANCON = globalCANMode;
      	while(globalCANMode != (CANSTAT & 0xE0)); // wait until ECAN is in listen only mode
		break;

	case CAN_MODE_LISTEN_ONLY:
		TXB0CONbits.TXREQ = 0;
		globalCANMode = 0x60;
    	CANCON = globalCANMode;
      	while(globalCANMode != (CANSTAT & 0xE0)); // wait until ECAN is in listen only mode
		break;

	default:
		TXB0CONbits.TXREQ = 0;
		globalCANMode = 0x80;
    	CANCON = globalCANMode;
    	while(globalCANMode != (CANSTAT & 0xE0)); // wait until ECAN is in config mode
		break;
	}
}



//------------------------------------------------------------------------------
//Update CAN bit rate
//------------------------------------------------------------------------------
void UpdateCANBitRate(void)
{
    //toss out the command byte
    USBMessageReadPointer++;
    
    CANBitRateHi = *USBMessageReadPointer++;
	CANBitRateLo = *USBMessageReadPointer++;

    CANBitRate = 0;
    CANBitRate = CANBitRateHi;
    CANBitRate = (CANBitRate << 8);
	CANBitRate = (CANBitRate + CANBitRateLo);

    CANCON = 0x80;
    while(0x80 != (CANSTAT & 0xE0)); // wait until ECAN is in config mode

    switch(CANBitRate)
    {
        case dCAN_20_KBPS_40MHZ:
        BRGCON1 = 0x31;
        BRGCON2 = 0xBC;
        BRGCON3 = 0x05;
        break;

        case dCAN_33_3KBPS_40MHZ:
        BRGCON1 = 0x17;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_50KBPS_40MHZ:
        BRGCON1 = 0x13;
        BRGCON2 = 0xB7;
        BRGCON3 = 0x03;
        break;
        
        case dCAN_80KBPS_40MHZ:
        BRGCON1 = 0x09;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_83_3KBPS_40MHZ:
        BRGCON1 = 0x09;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x06;
        break;
        
        case dCAN_100KBPS_40MHZ:
        BRGCON1 = 0x13;
        BRGCON2 = 0xA0;
        BRGCON3 = 0x02;
        break;
        
        case dCAN_125KBPS_40MHZ:
        BRGCON1 = 0x07;
        BRGCON2 = 0xBA;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_150KBPS_40MHZ:
        BRGCON1 = 0x06;
        BRGCON2 = 0xAF;
        BRGCON3 = 0x03;
        break;
        
        case dCAN_175KBPS_40MHZ:
        BRGCON1 = 0x05;
        BRGCON2 = 0xAF;
        BRGCON3 = 0x03;
        break;
        
        case dCAN_200KBPS_40MHZ:
        BRGCON1 = 0x03;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_225KBPS_40MHZ:
        BRGCON1 = 0x03;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x04;
        break;
        
        case dCAN_250KBPS_40MHZ:
        BRGCON1 = 0x03;
        BRGCON2 = 0xBA;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_275KBPS_40MHZ:
        BRGCON1 = 0x02;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x06;
        break;
        
        case dCAN_300KBPS_40MHZ:
        BRGCON1 = 0x02;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x04;
        break;
        
        case dCAN_500KBPS_40MHZ:
        BRGCON1 = 0x01;
        BRGCON2 = 0xBA;
        BRGCON3 = 0x07;
        break;

        case dCAN_625KBPS_40MHZ:
        BRGCON1 = 0x03;
        BRGCON2 = 0x98;
        BRGCON3 = 0x01;
        break;

        case dCAN_800KBPS_40MHZ:
        BRGCON1 = 0x00;
        BRGCON2 = 0xBF;
        BRGCON3 = 0x07;
        break;
        
        case dCAN_1000KBPS_40MHZ:
        BRGCON1 = 0x00;
        BRGCON2 = 0xBA;
        BRGCON3 = 0x07;
        break;
        
        default:
        //dCAN_100KBPS_40MHZ;
        BRGCON1 = 0x13;
        BRGCON2 = 0xA0;
        BRGCON3 = 0x02;

        CANBitRateHi = 0;
        CANBitRateLo = 0x64;
        CANBitRate = 0x64;
        break;
    }


    writeEEPROM(dEEPROM_CAN_BITRATE_HI_ADDRESS,CANBitRateHi);
    writeEEPROM(dEEPROM_CAN_BITRATE_LO_ADDRESS,CANBitRateLo);

    CANCON = globalCANMode;
    while(globalCANMode != (CANSTAT & 0xE0)); // wait until ECAN is in listen only mode
    
    USBMessageReadPointer += 17u; //Increment past dummy bytes not used in the packet
}    


//****************************************************************************
//
// Transmit a CAN message
//
//****************************************************************************
void TransmitTX0(void)
{
    //Increment past messageID header byte.
    USBMessageReadPointer++;
    
    //Request Node 1 0x551 total Transmitted (10101010 001)
    TXB0EIDH = *USBMessageReadPointer++;     //EIDH
    TXB0EIDL = *USBMessageReadPointer++;     //EIDL
    TXB0SIDH = *USBMessageReadPointer++;     //SIDH
    TXB0SIDL = *USBMessageReadPointer++;     //SIDL
    TXB0DLC = *USBMessageReadPointer++;      //DLC

    TXB0D0 = *USBMessageReadPointer++;       //D0
    TXB0D1 = *USBMessageReadPointer++;
    TXB0D2 = *USBMessageReadPointer++;
    TXB0D3 = *USBMessageReadPointer++;       //D3
    TXB0D4 = *USBMessageReadPointer++;
    TXB0D5 = *USBMessageReadPointer++;
    TXB0D6 = *USBMessageReadPointer++;
    TXB0D7 = *USBMessageReadPointer++;       //D7
    
    TXB0CONbits.TXREQ = 1; //Set the buffer to transmit
    
    USBMessageReadPointer += 5u; //Just increment past 4 byte timestamp and 1 byte checksum for now.
}


//****************************************************************************
//
// Transmit a CAN message
//
//****************************************************************************
void TransmitTX1(void)
{
    //Increment past messageID header byte.
    USBMessageReadPointer++;
    
    //Request Node 1 0x551 total Transmitted (10101010 001)
    TXB1EIDH = *USBMessageReadPointer++;     //EIDH
    TXB1EIDL = *USBMessageReadPointer++;     //EIDL
    TXB1SIDH = *USBMessageReadPointer++;     //SIDH
    TXB1SIDL = *USBMessageReadPointer++;     //SIDL
    TXB1DLC = *USBMessageReadPointer++;      //DLC

    TXB1D0 = *USBMessageReadPointer++;       //D0
    TXB1D1 = *USBMessageReadPointer++;
    TXB1D2 = *USBMessageReadPointer++;
    TXB1D3 = *USBMessageReadPointer++;       //D3
    TXB1D4 = *USBMessageReadPointer++;
    TXB1D5 = *USBMessageReadPointer++;
    TXB1D6 = *USBMessageReadPointer++;
    TXB1D7 = *USBMessageReadPointer++;       //D7
    
    TXB1CONbits.TXREQ = 1; //Set the buffer to transmit
    
    USBMessageReadPointer += 5u; //Just increment past 4 byte timestamp and 1 byte checksum for now.
}

//****************************************************************************
//
// Transmit a CAN message
//
//****************************************************************************
void TransmitTX2(void)
{
    //Increment past messageID header byte.
    USBMessageReadPointer++;
    
    //Request Node 1 0x551 total Transmitted (10101010 001)
    TXB2EIDH = *USBMessageReadPointer++;     //EIDH
    TXB2EIDL = *USBMessageReadPointer++;     //EIDL
    TXB2SIDH = *USBMessageReadPointer++;     //SIDH
    TXB2SIDL = *USBMessageReadPointer++;     //SIDL
    TXB2DLC = *USBMessageReadPointer++;      //DLC

    TXB2D0 = *USBMessageReadPointer++;       //D0
    TXB2D1 = *USBMessageReadPointer++;
    TXB2D2 = *USBMessageReadPointer++;
    TXB2D3 = *USBMessageReadPointer++;       //D3
    TXB2D4 = *USBMessageReadPointer++;
    TXB2D5 = *USBMessageReadPointer++;
    TXB2D6 = *USBMessageReadPointer++;
    TXB2D7 = *USBMessageReadPointer++;       //D7

    TXB2CONbits.TXREQ = 1; //Set the buffer to transmit
    
    USBMessageReadPointer += 5u; //Just increment past 4 byte timestamp and 1 byte checksum for now.
}






//****************************************************************************
//
// Receive a CAN message
//
//****************************************************************************
//------------------------------------------------------------------------------------
//Function: void PopCANFIFO(void)
//Reads a CAN message from the FIFO and pushes it onto the circular FIFO RAM buffer.
//------------------------------------------------------------------------------------
void PopCANFIFO(void)
{
    static unsigned char tempReg;
    static unsigned char tempECANCON;
    static unsigned char RunningChecksum;
    static unsigned char tempByteHi;
    static unsigned char tempByteLo;

    
    //Check if we have any room remaining in the circular RAM FIFO
    if(MessagesInBuffer >= (MAX_MESSAGES_BUFFERABLE - 1))
    {
        return;
    } 

    tempReg = (CANCON & 0x0F);
    tempECANCON = ECANCON; //Backup
    ECANCON |= (tempReg + 0x10);    
  
    //Check if a CAN message is waiting for us in the hardware.  If so, grab it.  
    if (RXB0CONbits.RXFUL != 0) //CheckRXB0
    {
    tempByteHi = CCPR1H;
    tempByteLo = CCPR1L;

        //----------------------------------------------------------------------
        //Read the TimeStamp count from the ISR
        //----------------------------------------------------------------------
        //Need to temporarily disable interrupts to make sure we don't get interrupted
        //in between reads of the high and low byte, and the value changes.
        INTCONbits.GIE = 0; //disable ISR
//        TimeStampIntSafe = tempTime3and2;
        TimeStampLongSafe = TimeStampLongISR;//tempTime3and2;
        INTCONbits.GIE = 1; //enable ISR

        //There is a new CAN message... put the data into the RAM FIFO buffer
        *WritePointer++ = dRECEIVE_MESSAGE; //Store the messageID byte
        *WritePointer++ = RXB0EIDH;
        *WritePointer++ = RXB0EIDL;
        *WritePointer++ = RXB0SIDH;
        *WritePointer++ = RXB0SIDL;
        *WritePointer++ = RXB0DLC;
        *WritePointer++ = RXB0D0;
        *WritePointer++ = RXB0D1;
        *WritePointer++ = RXB0D2;
        *WritePointer++ = RXB0D3;
        *WritePointer++ = RXB0D4;
        *WritePointer++ = RXB0D5;
        *WritePointer++ = RXB0D6;
        *WritePointer++ = RXB0D7;
        //Timestamp bytes are next


RunningChecksum = 0;

*WritePointer++ =(unsigned char)(TimeStampLongSafe >> 16);
RunningChecksum +=(unsigned char)(TimeStampLongSafe >> 16);

*WritePointer++ =(unsigned char)(TimeStampLongSafe >> 8);
RunningChecksum +=(unsigned char)(TimeStampLongSafe >> 8);

*WritePointer++ =(unsigned char)TimeStampLongSafe;
RunningChecksum +=(unsigned char)TimeStampLongSafe;

*WritePointer++ =(unsigned char)tempByteHi;
RunningChecksum +=(unsigned char)tempByteHi;

//We don't need this resolution *WritePointer++ =(unsigned char)tempByteLo;
//We don't need this resolution RunningChecksum +=(unsigned char)tempByteLo;

        //Checksum byte is next
        *WritePointer++ = (unsigned char)RXB0EIDH + RXB0EIDL + RXB0SIDH + RXB0SIDL + RXB0DLC + RXB0D0 + RXB0D1 + RXB0D2 + RXB0D3 + RXB0D4 + RXB0D5 + RXB0D6 + RXB0D7 + RunningChecksum;
        RXB0CONbits.RXFUL = 0;

        //We added a message to the circular FIFO.
        INTCONbits.GIEH = 0;
        MessagesInBuffer++;
        LATBbits.LATB1 = 1; //Let USB micro know we have data waiting to send to it.
        INTCONbits.GIEH = 1;
        
        //Check if it is time for write pointer wraparound
        if(WritePointer >= ((unsigned char*)&CANMessageBuffer[MAX_MESSAGES_BUFFERABLE] + sizeof(CAN_MESSAGE)))
        {
            //Wrap back to beginning address.
            WritePointer = (unsigned char*)&CANMessageBuffer[0];
        }    
    }   
    
    ECANCON = tempECANCON;  //Restore register
}    


void CheckForTXTrasnmitEcho(void)
{
    static unsigned char RunningChecksum;
    
    if(PIR3bits.TXBnIF == 1) //;check to see if the Global TX Interrupt Flag is set
    {
        //Check if we have any room remaining in the circular RAM FIFO
        if(MessagesInBuffer >= (MAX_MESSAGES_BUFFERABLE - 1))
        {
            return;
        } 
        
        if (TXB0CONbits.TXBIF == 1) //CheckTXB0
        {
            //Need to compose a packet to echo back to the USB PC host
            *WritePointer++ = dTRANSMIT_MESSAGE_RSP; //Store the messageID byte
            *WritePointer++ = TXB0EIDH;
            *WritePointer++ = TXB0EIDL;
            *WritePointer++ = TXB0SIDH;
            *WritePointer++ = TXB0SIDL;
            *WritePointer++ = TXB0DLC;
            *WritePointer++ = TXB0D0;
            *WritePointer++ = TXB0D1;
            *WritePointer++ = TXB0D2;
            *WritePointer++ = TXB0D3;
            *WritePointer++ = TXB0D4;
            *WritePointer++ = TXB0D5;
            *WritePointer++ = TXB0D6;
            *WritePointer++ = TXB0D7;
            //Timestamp bytes are next


*WritePointer++ =(unsigned char)(TimeStampLongSafe >> 24);
RunningChecksum=(unsigned char)(TimeStampLongSafe >> 24);

*WritePointer++ =(unsigned char)(TimeStampLongSafe >> 16);
RunningChecksum +=(unsigned char)(TimeStampLongSafe >> 16);

*WritePointer++ =(unsigned char)(TimeStampLongSafe >> 8);
RunningChecksum +=(unsigned char)(TimeStampLongSafe >> 8);

*WritePointer++ =(unsigned char)(TimeStampLongSafe);
RunningChecksum +=(unsigned char)(TimeStampLongSafe);

//            //Append sub-1ms counter Timer register snapshot timestamp.
//            //But also take slight detour and begin computing checksum.
//            RunningChecksum = TMR1L; //Get snapshot of Timer1H and Timer1L by reading TMR1L
//            *WritePointer++ = RunningChecksum;    
//            *WritePointer++ = TMR1H;    
//            RunningChecksum += TMR1H;
//            //Append 1ms counter value
//            RunningChecksum += (unsigned char)(TimeStampIntSafe >> 8);
//            *WritePointer++ = (unsigned char)(TimeStampIntSafe >> 8);
//            RunningChecksum += (unsigned char)TimeStampIntSafe; 
//            *WritePointer++ = (unsigned char)TimeStampIntSafe; 
                
            //Checksum byte is next
            *WritePointer++ = (unsigned char)TXB0EIDH + TXB0EIDL + TXB0SIDH + TXB0SIDL + TXB0DLC + TXB0D0 + TXB0D1 + TXB0D2 + TXB0D3 + TXB0D4 + TXB0D5 + TXB0D6 + TXB0D7 + RunningChecksum;
    
            //We added a message to the circular FIFO.
            INTCONbits.GIEH = 0;
            MessagesInBuffer++;
            LATBbits.LATB1 = 1; //Let USB micro know we have data waiting to send to it.
            INTCONbits.GIEH = 1;
            
            //Check if it is time for write pointer wraparound
            if(WritePointer >= ((unsigned char*)&CANMessageBuffer[MAX_MESSAGES_BUFFERABLE] + sizeof(CAN_MESSAGE)))
            {
                //Wrap back to beginning address.
                WritePointer = (unsigned char*)&CANMessageBuffer[0];
            }
                           
            TXB0CONbits.TXBIF = 0;
            PIR3bits.TXB0IF = 0;
        }
    }
    
}




//------------------------------------------------------------------------------
//End of file main.c
