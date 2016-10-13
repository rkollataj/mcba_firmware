/********************************************************************
 FileName:		main.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18, PIC24, and PIC32 USB Microcontrollers
 Hardware:		This demo is natively intended to be used on Microchip USB demo
 				boards supported by the MCHPFSUSB stack.  See release notes for
 				support matrix.  This demo can be modified for use on other hardware
 				platforms.
 Complier:  	Microchip C18 (for PIC18), C30 (for PIC24), C32 (for PIC32)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Initial release

  2.1   Updated for simplicity and to use common
        coding style

  2.6   Added support for PIC32MX795F512L & PIC24FJ256DA210

  2.6a  Added support for PIC24FJ256GB210

  2.7   No change

  2.7b  Improvements to USBCBSendResume(), to make it easier to use.
********************************************************************/



#include <p18cxxx.inc>


/** DECLARATIONS ***************************************************/
#pragma code

/******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/

#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{   
    InitializeSystem();

    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif

    while(1)
    {
//        #if defined(USB_POLLING)
//		// Check bus status and service USB interrupts.
//        USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
//        				  // this function periodically.  This function will take care
//        				  // of processing and responding to SETUP transactions 
//        				  // (such as during the enumeration process when you first
//        				  // plug in).  USB hosts require that USB devices should accept
//        				  // and process SETUP packets in a timely fashion.  Therefore,
//        				  // when using polling, this function should be called 
//        				  // regularly (such as once every 1.8ms or faster** [see 
//        				  // inline code comments in usb_device.c for explanation when
//        				  // "or faster" applies])  In most cases, the USBDeviceTasks() 
//        				  // function does not take very long to execute (ex: <100 
//        				  // instruction cycles) before it returns.
//        #endif
    				  

		// Application-specific tasks.
		// Application related code may be added here, or in the ProcessIO() function.
        ProcessIO();        
    }//end while
}//end main


/********************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.                  
 *
 * Note:            None
 *******************************************************************/
static void InitializeSystem(void)
{
//    ADCON1 |= 0x0F;                 // Default all pins to digital



//	The USB specifications require that USB peripheral devices must never source
//	current onto the Vbus pin.  Additionally, USB peripherals should not source
//	current on D+ or D- when the host/hub is not actively powering the Vbus line.
//	When designing a self powered (as opposed to bus powered) USB peripheral
//	device, the firmware should make sure not to turn on the USB module and D+
//	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
//	firmware needs some means to detect when Vbus is being powered by the host.
//	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
// 	can be used to detect when Vbus is high (host actively powering), or low
//	(host is shut down or otherwise not supplying power).  The USB firmware
// 	can then periodically poll this I/O pin to know when it is okay to turn on
//	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
//	peripheral device, it is not possible to source current on D+ or D- when the
//	host is not actively providing power on Vbus. Therefore, implementing this
//	bus sense feature is optional.  This firmware can be made to use this bus
//	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
//	HardwareProfile.h file.    
    #if defined(USE_USB_BUS_SENSE_IO)
//WES    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
    #endif

//	If the host PC sends a GetStatus (device) request, the firmware must respond
//	and let the host know if the USB peripheral device is currently bus powered
//	or self powered.  See chapter 9 in the official USB specifications for details
//	regarding this request.  If the peripheral device is capable of being both
//	self and bus powered, it should not return a hard coded value for this request.
//	Instead, firmware should check if it is currently self or bus powered, and
//	respond accordingly.  If the hardware has been configured like demonstrated
//	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
//	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
//	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
//	has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
//	to it in HardwareProfile.h.
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;	// See HardwareProfile.h
    #endif
   
	USBGenericOutHandle = 0;	
	USBGenericInHandle = 0;		
    UserInit();			//Application related initialization.  See user.c
    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    					//variables to known states.
}//end InitializeSystem


static unsigned char timeCounter = 0;
unsigned char tempCharCounter = 0;

#pragma udata USB_CAN_ONE_VAR

unsigned char spiState;
unsigned char tempCommand;
unsigned char numberOfBytesFromMessage;
unsigned char sizeOfPacket;
unsigned char tempCheckSum;


#pragma udata USB_CAN_TWO_VAR




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
#define dI_AM_ALIVE 0xF5

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


void incrementIndex(unsigned char* index);

#define dSPI_HEADER_BYTE1 0x55
#define dSPI_HEADER_BYTE2 0x46
#define dSPI_HEADER_BYTE1_STATE 0
#define dSPI_HEADER_BYTE2_STATE 1
#define dSPI_GET_THE_COMMAND_STATE 2
#define dSPI_GET_DATA_CHECK_MSG_CHECKSUM_STATE 3
#define dSPI_CLEANUP_QUEUE 4
#define dFINISHED_STATE 0x55
#define dMAX_SPI_MESSAGE_SIZE 20



unsigned char messageTransmitFlag;
unsigned char tempUSBdata[64];
unsigned char tempUSBdataIndex;
unsigned int usbWaitingFor60BytesTimer;

#pragma idata USB_CAN_IDATA_VARS1
unsigned char usbNeeds2TransmitSPI = dFALSE;


void UserInit(void)
{
    //Make sure CAN microcontroller is powered up and turned on.
    LATBbits.LATB5 = 0;
    TRISBbits.TRISB5 = 0;
    
    USBINBufWritePointer = (unsigned char*)&USBINBuffer[0];
    USBINBufReadPointer = (unsigned char*)&USBINBuffer[0];
    USBINBufByteCount = 0;
    
    mInitAllLEDs();
    //    mInitAllSwitches();
    
    TRISCbits.TRISC0 = 0; //Yellow
    TRISCbits.TRISC1 = 0; //green
    LATCbits.LATC0 = 0;
    LATCbits.LATC1 = 0;
    
    
    TRISAbits.TRISA1 = 1;  //used to sense USB power on / off 
    TRISAbits.TRISA2 = 1;  //used to sense 9v power on / off 
    TRISCbits.TRISC6 = 0;  //used for the termination resistor
    
    tempUSBdataIndex = 0;
    usbWaitingFor60BytesTimer = 0;
    messageTransmitFlag = 0;
    //SPI
    TRISCbits.TRISC7 = 0; //SD0
    TRISBbits.TRISB0 = 1; //SDI
    TRISBbits.TRISB1 = 0; //SCK
    TRISAbits.TRISA5 = 0; //SS
    LATAbits.LATA5 = 1;
    
    TRISBbits.TRISB3 = 1; //Input from CAN
    TRISBbits.TRISB2 = 0; //Output to CAN
    
    

    

    
    ADCON1 = 0x0F;
    
    //Init SPI Hardware for Master mode operation
    OpenSPI(SPI_FOSC_4, MODE_01, SMPMID); //With PLL
    //  PIR1bits.SSPIF = 0;
    //  PIE1bits.SSPIE = 1;
    
    //Setup Termination
    TRISCbits.TRISC6 = 0;  //used for the termination resistor
    LATCbits.LATC6 = 0; //Termination is on
    

}//end UserInit



/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/

#define dMAX_SPI_TX_ATTEMPTS 3

void ProcessIO(void)
{
    static unsigned char i;
    static unsigned char NumMessages = 0;
    static unsigned char BytesToXmit = 0;
    static unsigned char j;
    static unsigned char DummyVariable;
    static unsigned int FSR1Save;
    static unsigned int FSR2Save;
    static unsigned int PRODSave;


    //User Application USB tasks below.
    //Note: The user application should not begin attempting to read/write over the USB
    //until after the device has been fully enumerated.  After the device is fully
    //enumerated, the USBDeviceState will be set to "CONFIGURED_STATE".
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
    
    


    //--------------------------------------------------------------------------
    //Check if the CAN device has any messages it wants to send us.  If so, it
    //drives RB1 (CAN device) attached to RB3 (this USB device) logic high.
    //Also make sure we have enough RAM buffer available to receive a message.
    //--------------------------------------------------------------------------
    if((PORTBbits.RB3 == 1) && (USBINBufByteCount < (USB_IN_BUF_SIZE - SPI_CAN_MESSAGE_SIZE)))
    {
        //This takes care of getting a CAN message from the SPI slave device.
        //It pushes the 21 byte received message onto the circular FIFO.
        SendReceiveSPIPacket(FALSE);  
    }
    
    
 


    //--------------------------------------------------------------------------
    //Check if we can send a CAN message to the USB host
    //--------------------------------------------------------------------------
    if((USBINBufByteCount > 0u) && (!USBHandleBusy(USBGenericInHandle)))
    {
        //Figure out how many bytes we will want to send to the USB host in the next transaction
        if(USBINBufByteCount >= USBGEN_EP_SIZE)
        {
            //We will send a full 64-byte USB packet.  
            BytesToXmit = USBGEN_EP_SIZE;
        }    
        else
        {
            //We will send a short packet (ex: 21, 42, or 63 bytes in the USB packet)
            BytesToXmit = USBINBufByteCount;
        }         
        
        //Below loop is what we want, but it executes too slow.  Going to replace with asm.
//        //Copy a packet worth of data from the circular FIFO and place it in the USB INPacket[] buffer
//        //in preperation for sending it to the USB host.
//        for(i = 0; i < BytesToXmit; i++)
//        {
//            //Increment read location pointer and check for pointer wrap around
//            INPacket[i] = *USBINBufReadPointer++;
//            if(USBINBufReadPointer >= ((unsigned char*)&USBINBuffer[0] + USB_IN_BUF_SIZE))
//            {
//                USBINBufReadPointer = (unsigned char*)&USBINBuffer[0];
//            }               
//        }    

        INTCONbits.GIEH = 0;    //Disable interrupts for sensitive inline asm that uses FSRs
        
        FSR1Save = FSR1;
        FSR2Save = FSR2;
        PRODSave = PROD;
        
        PROD = (unsigned int)((unsigned char*)&USBINBuffer[0] + USB_IN_BUF_SIZE);
        
        
        FSR1 = (unsigned int)USBINBufReadPointer;
        FSR2 = (unsigned int)&INPacket;
        WREG = BytesToXmit;
        
            if(FSR1 >= PROD)
            {
                Nop();
            }

 
        _asm
            movff POSTINC1, POSTINC2
            
            //This is a check: if(FSR1 >= PROD)
            MOVF 0xff3, 0, ACCESS
            SUBWF 0xfe1, 0, ACCESS
            MOVF 0xff4, 0, ACCESS
            SUBWFB 0xfe2, 0, ACCESS
            BNC 0x2388
            
            movlw   &USBINBuffer[0]
                       
        
        _endasm
        
        INTCONbits.GIEH = 1;    //Re-enable interrupts
        
        //Prepare USB module to send a IN packet to the USB host.  Note: size of packet is variable (21, 42, or 63 bytes).
        USBGenericInHandle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&INPacket[0], BytesToXmit);
        
        //Remove the previously sent message(s) from the buffer message count, so
        //we "free up" the RAM associated with the buffer space.
        USBINBufByteCount -= BytesToXmit;
        
    }




    //--------------------------------------------------------------------------
    //Check if we have received any OUT messages from the USB host
    //--------------------------------------------------------------------------
    //Check if the OUT endpoint has received any data from the host.  Also make 
    //sure we have enough RAM available to receive a CAN message over SPI from the 
    //slave.  We need to ensure this in case we send a CAN packet to the SPI slave
    //(and receive one back in response).
    if(!USBHandleBusy(USBGenericOutHandle) && (USBINBufByteCount < (USB_IN_BUF_SIZE - SPI_CAN_MESSAGE_SIZE))) 
    {   
        switch(OUTPacket[0])					//Data arrived, check what kind of command might be in the packet of data.
        {
            case 0x55:
                //Send CAN message from USB buffer received from GUI, out the SPI and to
                //the CAN micro.  This function call will simultaneously send and
                //possibly receive a CAN message from the CAN micro.
                //SendReceiveSPIPacket(TRUE);  

                break;

            case 0x80:  //Toggle LED(s) command from PC application.
                if(mGetLED_1() == mGetLED_2())
                {
                    mLED_1_Toggle();
                    mLED_2_Toggle();
                }
                else
                {
                    mLED_1_On();
                    mLED_2_On();
                }
                break;

            case 0x81:
                //Currently do nothing
                break;
        }//switch(OUTPacket[0])

        //Make USB module ready to receive the next USB OUT packet from the host.
        USBGenericOutHandle = USBGenRead(USBGEN_EP_NUM,(BYTE*)&OUTPacket,USBGEN_EP_SIZE);
    }//if(!USBHandleBusy(USBGenericOutHandle))	


}//end ProcessIO


void SendReceiveSPIPacketRealFunction(BOOL SendDataAvailable)
{
    static unsigned char i;
    static unsigned char j;
    static unsigned char DummyVariable;
    
    //Now lets use SPI to receive a message
    //Below two lines commented out, since we do this in a macro before calling the 
    //function, so as to improve latency requirement.
    //LATAbits.LATA5 = 0;//Set slave select
    //LATBbits.LATB2 = 1; //This will cause a ECAN PINx interrupt

    LATBbits.LATB2 = 0;
    //Now wait some delay for the interrupt latency on the SPI slave, and to allow
    //enough time for the slave firmware to prepare to receive a SPI transmission packet.
    for(i = 0; i < 6; i++);
    ImprovedNop();
    ImprovedNop();
    ImprovedNop();

    DummyVariable = SSPBUF; //Make sure BF is clear so we can receive the next byte
    
    //Now commence with the SPI packet transmission.  Send either all 0xFF bytes, or
    //legit USB OUT packet data.
    if(SendDataAvailable == TRUE)
    {
        //We have a message we want to send to the CAN micro
        for(i = 0; i < SPI_CAN_MESSAGE_SIZE; i++)
        {
            PIR1bits.SSPIF = 0;
            SSPBUF = OUTPacket[i];
            while(PIR1bits.SSPIF == 0);
            *USBINBufWritePointer++ = SSPBUF;
            for(j = 0; j < 4u; j++);
        }
    }
    else
    {
        //No data needs sending, just send a block of 0xFF traffic.
        for(i = 0; i < SPI_CAN_MESSAGE_SIZE; i++)
        {
            PIR1bits.SSPIF = 0;
            SSPBUF = 0xFF;
            while(PIR1bits.SSPIF == 0);
            *USBINBufWritePointer++ = SSPBUF;
            for(j = 0; j < 4u; j++);
        }        
    }    
    LATAbits.LATA5 = 1;//De-assert SPI slave select
    
    //Increment byte count of bufferred data now that we received a packet.
    USBINBufByteCount += SPI_CAN_MESSAGE_SIZE;
    //Check if time for write pointer wrap around
    if(USBINBufWritePointer >= ((unsigned char*)&USBINBuffer[0] + USB_IN_BUF_SIZE))
    {
        USBINBufWritePointer = (unsigned char*)&USBINBuffer[0];
    }     
}    

/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs 
 *                  corresponding to the USB device state.
 *
 * Note:            mLED macros can be found in HardwareProfile.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/
void BlinkUSBStatus(void)
{
    static WORD led_count=0;
    
    if(led_count == 0)led_count = 10000U;
    led_count--;

    if(USBSuspendControl == 1)
    {
        if(led_count==0)
        {
            mLED_1_Toggle();
            if(mGetLED_1())
            {
                mLED_2_On();
            }
            else
            {
                mLED_2_Off();
            }
        }//end if
    }
    else
    {
        if(USBDeviceState == DETACHED_STATE)
        {
            mLED_Both_Off();
        }
        else if(USBDeviceState == ATTACHED_STATE)
        {
            mLED_Both_On();
        }
        else if(USBDeviceState == POWERED_STATE)
        {
            mLED_Only_1_On();
        }
        else if(USBDeviceState == DEFAULT_STATE)
        {
            mLED_Only_2_On();
        }
        else if(USBDeviceState == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                mLED_1_Toggle();
                mLED_2_Off();
            }//end if
        }
        else if(USBDeviceState == CONFIGURED_STATE)
        {
            if(led_count==0)
            {
                mLED_1_Toggle();
                if(mGetLED_1())
                {
                    mLED_2_Off();
                }
                else
                {
                    mLED_2_On();
                }
            }//end if
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus




// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:
	
	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is 
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause 
	//things to not work as intended.	
	

    #if defined(__C30__)
    #if 0
        U1EIR = 0xFFFF;
        U1IR = 0xFFFF;
        U1OTGIR = 0xFFFF;
        IFS5bits.USB1IF = 0;
        IEC5bits.USB1IE = 1;
        U1OTGIEbits.ACTVIE = 1;
        U1OTGIRbits.ACTVIF = 1;
        Sleep();
    #endif
    #endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
    #if !defined(self_powered)
        if(U1OTGIRbits.ACTVIF)
        {
            IEC5bits.USB1IE = 0;
            U1OTGIEbits.ACTVIE = 0;
            IFS5bits.USB1IF = 0;
        
            //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
            USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
            //USBSuspendControl = 0;
        }
    #endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// a few milliseconds of wakeup time, after which the device must be 
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.
	
	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *****************************************************************************/
void USBCBCheckOtherReq(void)
{
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *****************************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/******************************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *****************************************************************************/
void USBCBInitEP(void)
{
    USBEnableEndpoint(USBGEN_EP_NUM,USB_OUT_ENABLED|USB_IN_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    USBGenericOutHandle = USBGenRead(USBGEN_EP_NUM,(BYTE*)&OUTPacket,USBGEN_EP_SIZE);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior, 
 *                  as a USB device that has not been armed to perform remote 
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *                  
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are 
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex: 
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup. 
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in 
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    //First verify that the host has armed us to perform remote wakeup.
    //It does this by sending a SET_FEATURE request to enable remote wakeup,
    //usually just before the host goes to standby mode (note: it will only
    //send this SET_FEATURE request if the configuration descriptor declares
    //the device as remote wakeup capable, AND, if the feature is enabled
    //on the host (ex: on Windows based hosts, in the device manager 
    //properties page for the USB device, power management tab, the 
    //"Allow this device to bring the computer out of standby." checkbox 
    //should be checked).
    if(USBGetRemoteWakeupStatus() == TRUE) 
    {
        //Verify that the USB bus is in fact suspended, before we send
        //remote wakeup signalling.
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();
            
            //Clock switch to settings consistent with normal USB operation.
            USBCBWakeFromSuspend();
            USBSuspendControl = 0; 
            USBBusIsSuspended = FALSE;  //So we don't execute this code again, 
                                        //until a new suspend condition is detected.

            //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            //device must continuously see 5ms+ of idle on the bus, before it sends
            //remote wakeup signalling.  One way to be certain that this parameter
            //gets met, is to add a 2ms+ blocking delay here (2ms plus at 
            //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
            //5ms+ total delay since start of idle).
            delay_count = 3600U;        
            do
            {
                delay_count--;
            }while(delay_count);
            
            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1;       // Start RESUME signaling
            delay_count = 1800U;        // Set RESUME line for 1-13 ms
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0;       //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}


/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was 
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was 
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT 
            //      endpoints).
            break;            
        default:
            break;
    }      
    return TRUE; 
}
/** EOF main.c ***************************************************************/
