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

/** INCLUDES *******************************************************/
#include "usb.h"
#include "usb_function_generic.h"

#include "HardwareProfile.h"
#include "spi.h"
#include "queue.h"

#pragma udata
unsigned char DummyVar;
#define ImprovedNop()   {DummyVar++;}

#define dUSB_FW_VERSION_MAJOR 0xff  //use 0b1nnn mmmm oooo ppppfor any Beta release
#define dUSB_FW_VERSION_MINOR 0x00

#define dTRUE 1
#define dFALSE !dTRUE

    #define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

/** CONFIGURATION **************************************************/
//#if defined(PICDEM_FS_USB)      // Configuration bits for PICDEM FS USB Demo Board (based on PIC18F4550)
//        #pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
//        #pragma config CPUDIV   = OSC1_PLL2   
//        #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
//        #pragma config FOSC     = HSPLL_HS
//        #pragma config FCMEN    = OFF
//        #pragma config IESO     = OFF
//        #pragma config PWRT     = OFF
//        #pragma config BOR      = ON
//        #pragma config BORV     = 3
//        #pragma config VREGEN   = ON      //USB Voltage Regulator
//        #pragma config WDT      = OFF
//        #pragma config WDTPS    = 32768
//        #pragma config MCLRE    = ON
//        #pragma config LPT1OSC  = OFF
//        #pragma config PBADEN   = OFF
////      #pragma config CCP2MX   = ON
//        #pragma config STVREN   = ON
//        #pragma config LVP      = OFF
////      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
//        #pragma config XINST    = OFF       // Extended Instruction Set
//        #pragma config CP0      = OFF
//        #pragma config CP1      = OFF
////      #pragma config CP2      = OFF
////      #pragma config CP3      = OFF
//        #pragma config CPB      = OFF
////      #pragma config CPD      = OFF
//        #pragma config WRT0     = OFF
//        #pragma config WRT1     = OFF
////      #pragma config WRT2     = OFF
////      #pragma config WRT3     = OFF
//        #pragma config WRTB     = OFF       // Boot Block Write Protection
//        #pragma config WRTC     = OFF
////      #pragma config WRTD     = OFF
//        #pragma config EBTR0    = OFF
//        #pragma config EBTR1    = OFF
////      #pragma config EBTR2    = OFF
////      #pragma config EBTR3    = OFF
//        #pragma config EBTRB    = OFF
//
//
//#elif defined(PIC18F87J50_PIM)				// Configuration bits for PIC18F87J50 FS USB Plug-In Module board
//        #pragma config XINST    = OFF   	// Extended instruction set
//        #pragma config STVREN   = ON      	// Stack overflow reset
//        #pragma config PLLDIV   = 3         // (12 MHz crystal used on this board)
//        #pragma config WDTEN    = OFF      	// Watch Dog Timer (WDT)
//        #pragma config CP0      = OFF      	// Code protect
//        #pragma config CPUDIV   = OSC1      // OSC1 = divide by 1 mode
//        #pragma config IESO     = OFF      	// Internal External (clock) Switchover
//        #pragma config FCMEN    = OFF      	// Fail Safe Clock Monitor
//        #pragma config FOSC     = HSPLL     // Firmware must also set OSCTUNE<PLLEN> to start PLL!
//        #pragma config WDTPS    = 32768
////      #pragma config WAIT     = OFF      	// Commented choices are
////      #pragma config BW       = 16      	// only available on the
////      #pragma config MODE     = MM      	// 80 pin devices in the 
////      #pragma config EASHFT   = OFF      	// family.
//        #pragma config MSSPMSK  = MSK5
////      #pragma config PMPMX    = DEFAULT
////      #pragma config ECCPMX   = DEFAULT
//        #pragma config CCP2MX   = DEFAULT   
//
//#elif defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
//     #pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
//     #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
//     #pragma config STVREN = ON          //stack overflow/underflow reset enabled
//     #pragma config XINST = OFF          //Extended instruction set disabled
//     #pragma config CPUDIV = OSC1        //No CPU system clock divide
//     #pragma config CP0 = OFF            //Program memory is not code-protected
//     #pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
//     #pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
//     #pragma config IESO = OFF           //Two-Speed Start-up disabled
//     #pragma config WDTPS = 32768        //1:32768
//     #pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
//     #pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
//     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
//     #pragma config DSWDTEN = OFF        //Disabled
//     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
//     #pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
//     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
//     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
//     #pragma config WPEND = PAGE_0       //Start protection at page 0
//     #pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
//     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored 
//     #if defined(PIC18F47J53_PIM)
//        #pragma config CFGPLLEN = OFF
//     #else
//        #pragma config T1DIG = ON           //Sec Osc clock source may be selected
//        #pragma config LPT1OSC = OFF        //high power Timer1 mode
//     #endif
//#elif defined(LOW_PIN_COUNT_USB_DEVELOPMENT_KIT)
//	// PIC18F14K50
//        #pragma config CPUDIV = NOCLKDIV
//        #pragma config USBDIV = OFF
//        #pragma config FOSC   = HS
//        #pragma config PLLEN  = ON
//        #pragma config FCMEN  = OFF
//        #pragma config IESO   = OFF
//        #pragma config PWRTEN = OFF
//        #pragma config BOREN  = OFF
//        #pragma config BORV   = 30
//        #pragma config WDTEN  = OFF
//        #pragma config WDTPS  = 32768
//        #pragma config MCLRE  = OFF
//        #pragma config HFOFST = OFF
//        #pragma config STVREN = ON
//        #pragma config LVP    = OFF
//        #pragma config XINST  = OFF
//        #pragma config BBSIZ  = OFF
//        #pragma config CP0    = OFF
//        #pragma config CP1    = OFF
//        #pragma config CPB    = OFF
//        #pragma config WRT0   = OFF
//        #pragma config WRT1   = OFF
//        #pragma config WRTB   = OFF
//        #pragma config WRTC   = OFF
//        #pragma config EBTR0  = OFF
//        #pragma config EBTR1  = OFF
//        #pragma config EBTRB  = OFF        
//
//#elif defined(EXPLORER_16)
//    #if defined(__PIC24FJ256GB110__)
//        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
//        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_ON)
//    #elif defined(PIC24FJ256GB210_PIM)
//        _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
//        _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
//    #elif defined(__PIC24FJ64GB004__)
//        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
//        _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_ON)
//        _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
//        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
//    #elif defined(__32MX460F512L__) || defined(__32MX795F512L__)
//        #pragma config UPLLEN   = ON        // USB PLL Enabled
//        #pragma config FPLLMUL  = MUL_15        // PLL Multiplier
//        #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
//        #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
//        #pragma config FPLLODIV = DIV_1         // PLL Output Divider
//        #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
//        #pragma config FWDTEN   = OFF           // Watchdog Timer
//        #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
//        #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
//        #pragma config OSCIOFNC = OFF           // CLKO Enable
//        #pragma config POSCMOD  = HS            // Primary Oscillator
//        #pragma config IESO     = OFF           // Internal/External Switch-over
//        #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
//        #pragma config FNOSC    = PRIPLL        // Oscillator Selection
//        #pragma config CP       = OFF           // Code Protect
//        #pragma config BWP      = OFF           // Boot Flash Write Protect
//        #pragma config PWP      = OFF           // Program Flash Write Protect
//        #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
//        #pragma config DEBUG    = ON            // Background Debugger Enable
//  #elif defined(__dsPIC33EP512MU810__)
//        _FOSCSEL(FNOSC_FRC);
//        _FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//        _FWDT(FWDTEN_OFF);
//    #else
//        #error No hardware board defined, see "HardwareProfile.h" and __FILE__
//    #endif
//#elif defined(PIC24F_STARTER_KIT)
//    _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
//    _CONFIG2( 0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
//#elif defined(PIC24FJ256DA210_DEV_BOARD)
//    _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
//    _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
//#elif defined(PIC32_USB_STARTER_KIT)
//    #pragma config UPLLEN   = ON        // USB PLL Enabled
//    #pragma config FPLLMUL  = MUL_15        // PLL Multiplier
//    #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
//    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
//    #pragma config FPLLODIV = DIV_1         // PLL Output Divider
//    #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
//    #pragma config FWDTEN   = OFF           // Watchdog Timer
//    #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
//    #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
//    #pragma config OSCIOFNC = OFF           // CLKO Enable
//    #pragma config POSCMOD  = HS            // Primary Oscillator
//    #pragma config IESO     = OFF           // Internal/External Switch-over
//    #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
//    #pragma config FNOSC    = PRIPLL        // Oscillator Selection
//    #pragma config CP       = OFF           // Code Protect
//    #pragma config BWP      = OFF           // Boot Flash Write Protect
//    #pragma config PWP      = OFF           // Program Flash Write Protect
//    #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
//    #pragma config DEBUG    = ON            // Background Debugger Enable
//#else
//    #error No hardware board defined, see "HardwareProfile.h"

        #pragma config PLLDIV   = 3 //5         // (20 MHz crystal on PICDEM FS USB board)
        #pragma config CPUDIV   = OSC1_PLL2   
        #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
        #pragma config FOSC     = HSPLL_HS
        #pragma config FCMEN    = OFF
        #pragma config IESO     = OFF
        #pragma config PWRT     = OFF
        #pragma config BOR      = OFF
        #pragma config BORV     = 3
        #pragma config VREGEN   = ON      //USB Voltage Regulator
        #pragma config WDT      = OFF
        #pragma config WDTPS    = 32768
        #pragma config MCLRE    = OFF
        #pragma config LPT1OSC  = OFF
        #pragma config PBADEN   = OFF
//      #pragma config CCP2MX   = ON
        #pragma config STVREN   = ON
        #pragma config LVP      = OFF
//      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
        #pragma config XINST    = OFF       // Extended Instruction Set
        #pragma config CP0      = OFF
        #pragma config CP1      = OFF
//      #pragma config CP2      = OFF
//      #pragma config CP3      = OFF
        #pragma config CPB      = OFF
//      #pragma config CPD      = OFF
        #pragma config WRT0     = OFF
        #pragma config WRT1     = OFF
//      #pragma config WRT2     = OFF
//      #pragma config WRT3     = OFF
        #pragma config WRTB     = OFF       // Boot Block Write Protection
        #pragma config WRTC     = OFF
//      #pragma config WRTD     = OFF
        #pragma config EBTR0    = OFF
        #pragma config EBTR1    = OFF
//      #pragma config EBTR2    = OFF
//      #pragma config EBTR3    = OFF
        #pragma config EBTRB    = OFF
//#endif



/** VARIABLES ******************************************************/
//#if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
//    #pragma udata usbram2
//#elif defined(__18F2455) || defined(__18F2550) || defined(__18F4455) || defined(__18F4550)\
//    || defined(__18F2458) || defined(__18F2453) || defined(__18F4558) || defined(__18F4553)
    #pragma udata USB_VARIABLES=0x500

//#elif defined(__18F4450) || defined(__18F2450)
//    #pragma udata USB_VARIABLES=0x480
//#else
//    #pragma udata
//#endif

unsigned char OUTPacket[64];	//User application buffer for receiving and holding OUT packets sent from the host
unsigned char INPacket[64];		//User application buffer for sending IN packets to the host

#define MAX_CAN_MESSAGES_BUFFERABLE 15u
#define SPI_CAN_MESSAGE_SIZE  19u
#define dCHECKSUM_LOCATION (SPI_CAN_MESSAGE_SIZE-1)

#define USB_IN_BUF_SIZE (WORD)((WORD)SPI_CAN_MESSAGE_SIZE * (WORD)MAX_CAN_MESSAGES_BUFFERABLE)


#define USB_IN_BUFFER_START_ADDRESS 0x580
#pragma udata USB_BUFFER_REGION = 0x580
unsigned char USBINBuffer[USB_IN_BUF_SIZE];
#define USB_IN_BUFFER_LAST_USED_ADDRESS (unsigned int)((unsigned int)USB_IN_BUFFER_START_ADDRESS + (unsigned int)USB_IN_BUF_SIZE)
#define USB_IN_BUFFER_LAST_USED_ADDRESS_LB  (unsigned char)USB_IN_BUFFER_LAST_USED_ADDRESS
#define USB_IN_BUFFER_LAST_USED_ADDRESS_UB  (unsigned char)(USB_IN_BUFFER_LAST_USED_ADDRESS >> 8)

#pragma udata SOME_VAR_LABEL1
unsigned char* USBINBufWritePointer;
unsigned char* USBINBufReadPointer;
WORD USBINBufByteCount;

USB_HANDLE USBGenericOutHandle;
USB_HANDLE USBGenericInHandle;
#pragma udata SOME_VAR_LABEL2
unsigned char fakeSpiMessage[SPI_CAN_MESSAGE_SIZE];
unsigned char tempCheckSum;
unsigned char request2SendIamAlive;
unsigned char currentTerminationSetting;
unsigned long sendIamAliveCounter;
unsigned char debugMode;

/** PRIVATE PROTOTYPES *********************************************/
static void InitializeSystem(void);
void USBDeviceTasks(void);
void YourHighPriorityISRCode(void);
void YourLowPriorityISRCode(void);
void USBCBSendResume(void);
void UserInit(void);
void ProcessIO(void);
void BlinkUSBStatus(void);
void SendReceiveSPIPacketRealFunction(BOOL SendDataAvailable);  //Don't actually call this function directly.  Use the SendReceiveSPIPacket() macro instead.
#define SendReceiveSPIPacket(a) {LATAbits.LATA5 = 0; LATBbits.LATB2 = 1; SendReceiveSPIPacketRealFunction(a);}

/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
	//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
	//the reset, high priority interrupt, and low priority interrupt
	//vectors.  However, the current Microchip USB bootloader 
	//examples are intended to occupy addresses 0x00-0x7FF or
	//0x00-0xFFF depending on which bootloader is used.  Therefore,
	//the bootloader code remaps these vectors to new locations
	//as indicated below.  This remapping is only necessary if you
	//wish to program the hex file generated from this project with
	//the USB bootloader.  If no bootloader is used, edit the
	//usb_config.h file and comment out the following defines:
	//#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
	//#define PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x1000
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018
	#elif defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)	
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x800
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x808
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x818
	#else	
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x00
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x08
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x18
	#endif
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}
	#endif
	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	//Note: If this project is built while one of the bootloaders has
	//been defined, but then the output hex file is not programmed with
	//the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
	//As a result, if an actual interrupt was enabled and occured, the PC would jump
	//to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
	//executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
	//(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
	//would effective reset the application.
	
	//To fix this situation, we should always deliberately place a 
	//"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
	//"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
	//hex file of this project is programmed with the bootloader, these sections do not
	//get bootloaded (as they overlap the bootloader space).  If the output hex file is not
	//programmed using the bootloader, then the below goto instructions do get programmed,
	//and the hex file still works like normal.  The below section is only required to fix this
	//scenario.
	#pragma code HIGH_INTERRUPT_VECTOR = 0x08
	void High_ISR (void)
	{
	     _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#pragma code LOW_INTERRUPT_VECTOR = 0x18
	void Low_ISR (void)
	{
	     _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#endif	//end of "#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER)"

	#pragma code
	
	
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
        #if defined(USB_INTERRUPT)
	        USBDeviceTasks();
        #endif
	
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.

//    SPI_isr()
	
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 
#endif




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
    ADCON1 |= 0x0F;                 // Default all pins to digital



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
    //tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
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
    //tris_self_power = INPUT_PIN;	// See HardwareProfile.h
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
//#define dI_AM_ALIVE 0xF5
#define dI_AM_ALIVE_FROM_CAN 0xF5
#define dI_AM_ALIVE_FROM_USB 0xF7

//Recieve from USB USB host, send down to CAN micro
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
unsigned char writeEEPROM(unsigned char address,unsigned char data);
unsigned char readEEPROM(unsigned char address);

#define dSPI_HEADER_BYTE1 0x55
#define dSPI_HEADER_BYTE2 0x46
#define dSPI_HEADER_BYTE1_STATE 0
#define dSPI_HEADER_BYTE2_STATE 1
#define dSPI_GET_THE_COMMAND_STATE 2
#define dSPI_GET_DATA_CHECK_MSG_CHECKSUM_STATE 3
#define dSPI_CLEANUP_QUEUE 4
#define dFINISHED_STATE 0x55
#define dMAX_SPI_MESSAGE_SIZE 20

#define dEEPROM_IN_USE_ADDRESS 0
#define dEEPROM_CAN_TERMINATION_ADDRESS 1


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
    
    //    mInitAllLEDs();
    //    mInitAllSwitches();
    
    TRISCbits.TRISC0 = 0; //Yellow
    TRISCbits.TRISC1 = 0; //green
    LATCbits.LATC0 = 0;
    LATCbits.LATC1 = 0;
    
    
    TRISAbits.TRISA1 = 1;  //used to sense USB power on / off 
    TRISAbits.TRISA2 = 1;  //used to sense 9v power on / off 
    


    tempUSBdataIndex = 0;
    usbWaitingFor60BytesTimer = 0;
    messageTransmitFlag = 0;
    //SPI
    TRISCbits.TRISC7 = 0; //SD0
    TRISBbits.TRISB0 = 1; //SDI
    TRISBbits.TRISB1 = 0; //SCK
    LATAbits.LATA5 = 1;
    TRISAbits.TRISA5 = 0; //SS
    
    TRISBbits.TRISB3 = 1; //Input from CAN
    TRISBbits.TRISB2 = 0; //Output to CAN
    
    //All GPIO pins to digital mode
    ADCON1 = 0x0F;
    
    //Init SPI Hardware for Master mode operation
    SSPSTAT = 0x00; //Sample in middle of bit, xmit on idle to active edge
    SSPCON1 = 0x00; //Idle level of clock is logic low, Fosc/4 mode
    SSPCON1bits.SSPEN = 1;      //Enable module
    
    //OpenSPI(SPI_FOSC_4, MODE_01, SMPMID); //With PLL
    //  PIR1bits.SSPIF = 0;
    //  PIE1bits.SSPIE = 1;
    
    //Setup Termination
    TRISCbits.TRISC6 = 0;  //used for the termination resistor

//Do some error checking...
	currentTerminationSetting = readEEPROM(dEEPROM_CAN_TERMINATION_ADDRESS);

	if (currentTerminationSetting == 0)
	{
	}
	else if (currentTerminationSetting == 1)
	{
	}
	else //Should catch all other cases on power up
	{
//Force the termination on... and write the eeprom
	currentTerminationSetting = 0; //Termination is on
	writeEEPROM(dEEPROM_CAN_TERMINATION_ADDRESS ,0x00);
	}

	LATCbits.LATC6 = currentTerminationSetting;

//Should send this message at powerup so the GUI get this to display in the "About" window
	request2SendIamAlive = dTRUE;
    debugMode = 0;
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

#define dUSB_MICRO 1
#define dCAN_MICRO 2
#define dBOTH_PICMICROS 0x55

#define dLED_ALL 0x55
#define dLED_USB_ONE 1
#define dLED_USB_TWO 2
#define dLED_CAN_RGB1 3
#define dLED_CAN_RGB2 4
#define dLED_CAN_RGB3 5

#define dLED_STATE_ON 1
#define dLED_STATE_OFF 2
#define dLED_STATE_GREEN 3
#define dLED_STATE_YELLOW 4
#define dLED_STATE_RED 5


void ProcessIO(void)
{
    static unsigned char i;
    static unsigned char NumMessages = 0;
    static unsigned char BytesToXmit = 0;
    static unsigned char j;
    static unsigned char DummyVariable;
    static unsigned int FSR1Save;
    static unsigned int FSR2Save;
    static unsigned char PRODLSave;


    //User Application USB tasks below.
    //Note: The user application should not begin attempting to read/write over the USB
    //until after the device has been fully enumerated.  After the device is fully
    //enumerated, the USBDeviceState will be set to "CONFIGURED_STATE".
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1))
		{
			if (debugMode == 0)
			{
			mLED_1_Off();
			mLED_2_Off();
			}
		return;
		}

		if (debugMode == 0)
		{
//We are operating... turn on the Green LED
    	mLED_2_On();
		}



    //--------------------------------------------------------------------------
    //Check if the CAN device has any messages it wants to send us.  If so, it
    //drives RB1 (CAN device) attached to RB3 (this USB device) logic high.
    //Also make sure we have enough RAM buffer available to receive a message.
    //--------------------------------------------------------------------------
    if((PORTBbits.RB3 == 1) && (USBINBufByteCount < (USB_IN_BUF_SIZE - SPI_CAN_MESSAGE_SIZE)))
    {
        //This takes care of getting a CAN message from the SPI slave device.
        //It pushes the 19 byte received message onto the circular FIFO.
        SendReceiveSPIPacket(FALSE);
    }


//////////////////////////////
    if (sendIamAliveCounter < 1000000)
	{
	sendIamAliveCounter++;
	}
	else
	{
	request2SendIamAlive = dTRUE;
	}

	if (request2SendIamAlive == dTRUE) 
	{
//I need to fake a SPI message so I can send data up to the host PC
		if(USBINBufByteCount < (USB_IN_BUF_SIZE - SPI_CAN_MESSAGE_SIZE))
		{
			sendIamAliveCounter = 0;
			request2SendIamAlive = dFALSE;
			
			//Clear buffer
			for(i = 0; i < SPI_CAN_MESSAGE_SIZE; i++)
			{
				fakeSpiMessage[i] = 0;
			}
			
			//Set Data
			fakeSpiMessage[0] = dI_AM_ALIVE_FROM_USB;
			fakeSpiMessage[1] = currentTerminationSetting;
			fakeSpiMessage[2] = dUSB_FW_VERSION_MAJOR;
			fakeSpiMessage[3] = dUSB_FW_VERSION_MINOR;
			
			//Calculate CheckSum
			tempCheckSum = 0;
			for(i = 1; i < (dCHECKSUM_LOCATION); i++)
			{
				tempCheckSum = tempCheckSum + fakeSpiMessage[i];
			}
			fakeSpiMessage[dCHECKSUM_LOCATION] = tempCheckSum;
			
			//Receive Fake SPI message
			INTCONbits.GIEH = 0;
			for(i = 0; i < SPI_CAN_MESSAGE_SIZE; i++)
			{
			//Form on fake SPI message
				*USBINBufWritePointer++ = fakeSpiMessage[i];
			}
			//Increment byte count of bufferred data now that we received a packet.
			USBINBufByteCount += SPI_CAN_MESSAGE_SIZE;
			//Check if time for write pointer wrap around
			if(USBINBufWritePointer >= ((unsigned char*)&USBINBuffer[0] + USB_IN_BUF_SIZE))
			{
				USBINBufWritePointer = (unsigned char*)&USBINBuffer[0];
			}
            INTCONbits.GIEH = 1;
		}
	}
//////////////////////////////







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
            //We will send a short packet (ex: 19, 38, or 58 bytes in the USB packet)
            BytesToXmit = USBINBufByteCount;
        }         
        
        //-------------------------------------------------------------------------------------------------------------
        //Below loop is what we want, but it executes too slow.  Going to replace with asm.
        //---------------------------------------------------------------------------------
        //Copy a packet worth of data from the circular FIFO and place it in the USB INPacket[] buffer
        //in preperation for sending it to the USB host. 
  
////2315 cycles with BytesToXmit = 64; instruction prepended
//        for(i = 0; i < BytesToXmit; i++)
//        {
//            //Increment read location pointer and check for pointer wrap around
//            INPacket[i] = *USBINBufReadPointer++;
//            if(USBINBufReadPointer >= ((unsigned char*)&USBINBuffer[0] + USB_IN_BUF_SIZE))
//            {
//                USBINBufReadPointer = (unsigned char*)&USBINBuffer[0];
//            }               
//        }     
        
		if(USBINBufReadPointer + BytesToXmit >= USBINBuffer + sizeof(USBINBuffer))
		{	// 926 for this path
			i = USBINBuffer + sizeof(USBINBuffer) - USBINBufReadPointer;
			memcpy(INPacket, USBINBufReadPointer, i);
			j = BytesToXmit-i;
			memcpy(&INPacket[i], USBINBuffer, j);
			USBINBufReadPointer = USBINBuffer + j;
		}
		else
		{	// 848 for this path
			memcpy(INPacket, USBINBufReadPointer, BytesToXmit);
			USBINBufReadPointer += BytesToXmit;
		}

 
        
        
        //------------- ASM Equivalent ----------------------------------------------------
//        INTCONbits.GIEH = 0;    //Disable interrupts for sensitive inline asm that uses FSRs
//        
//        //Save C compiler managed registers that we are about to use in inline asm
//        FSR1Save = FSR1;
//        FSR2Save = FSR2;
//        PRODLSave = PRODL;
//
//        //Load registers for inline asm copy operation        
//        FSR1 = (unsigned int)USBINBufReadPointer;
//        FSR2 = (unsigned int)&INPacket[0];
//        PRODL = BytesToXmit;
//        
//        //Now perform asm loop copy of one RAM location to another RAM location using pointers.
//        _asm
//FAST_ASM_COPY_LOOP:
//            //Do this: INPacket[i++] = *USBINBufReadPointer++; 
//            movff POSTINC1, POSTINC2
//            
//            //Now check for USBINBufReadPointer wraparound, ex: if(FSR1 >= USB_IN_BUFFER_LAST_USED_ADDRESS)
//            movlw USB_IN_BUFFER_LAST_USED_ADDRESS_LB
//            subwf FSR1L, 0, ACCESS
//            movlw USB_IN_BUFFER_LAST_USED_ADDRESS_UB
//            subwfb FSR1L, 0, ACCESS
//            bnc    END_OF_LOOP_CHECK         //Skip over code inside if() statement
//            //The below line resets pointer to start of buffer.  Below line is skipped if no wraparound needed.
//            lfsr    1, USB_IN_BUFFER_START_ADDRESS  //Wrap around to start of USBINBuffer[]
//END_OF_LOOP_CHECK:
//            decfsz  PRODL, 1, ACCESS    
//            bra     FAST_ASM_COPY_LOOP
//        _endasm
//
//        //Save the updated USBINBufReadPointer
//        USBINBufReadPointer = (unsigned char*)FSR1;
//
//        //Restore C compiler managed resources
//        FSR1 = FSR1Save;
//        FSR2 = FSR2Save;
//        PRODL = PRODLSave;
//        
//        INTCONbits.GIEH = 1;    //Re-enable interrupts
        //-------------------------------------------------------------------------------------------------------------
        
        //Prepare USB module to send a IN packet to the USB host.  Note: size of packet is variable (21, 42, or 63 bytes).
        USBGenericInHandle = USBGenWrite(USBGEN_EP_NUM, (BYTE*)&INPacket[0], BytesToXmit);
        
        //Remove the previously sent message(s) from the buffer message count, so
        //we "free up" the RAM associated with the buffer space.
        USBINBufByteCount -= BytesToXmit;
		if (debugMode == 0)
		{
		mLED_1_Toggle();
		}
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
            ////Recieve from USB USB host, send down to CAN micro
            //#define dCHANGE_BIT_RATE 0xA1
            //#define dTRANSMIT_MESSAGE_PD 0xA2
            //#define dTRANSMIT_MESSAGE_EV 0xA3
            //#define dSETUP_TRIGGER 0xA4
            //#define dWRITE_REGISTER_DIRECTLY 0xA5
            //#define dREAD_REGISTER_DIRECTLY 0xA6
            //#define dTOGGLE_LED 0xA7
            //#define dSETUP_TERMINATION_RESISTANCE 0xA8
            //#define dREAD_FW_VERSION 0xA9
            //#define dCAN_RESET 0xAA
            //#define dCHANGE_CAN_MODE 0xAB
            //#define dSETUP_HW_FILTER 0xAC
            //#define dDEBUG_MODE 0xB0

            case dTRANSMIT_MESSAGE_EV:
                //Send CAN message from USB buffer received from GUI, out the SPI and to
                //the CAN micro.  This function call will simultaneously send and
                //possibly receive a CAN message from the CAN micro.
                SendReceiveSPIPacket(TRUE);
				ImprovedNop();
				ImprovedNop();
				ImprovedNop();
                break;

            case dCHANGE_BIT_RATE:
                SendReceiveSPIPacket(TRUE);
				ImprovedNop();
                break;

            case dCHANGE_CAN_MODE:
                SendReceiveSPIPacket(TRUE);
				ImprovedNop();
                break;

            case dSETUP_TERMINATION_RESISTANCE:
                //Setup termination resistance... do not need to forward this message on
				request2SendIamAlive = dTRUE;
				if (OUTPacket[1] == 1)
				{
				//Turn off Termination only if it is ON
					if (currentTerminationSetting != 1)
					{
					currentTerminationSetting = 1;
					LATCbits.LATC6 = 1;
					writeEEPROM(dEEPROM_CAN_TERMINATION_ADDRESS ,0x01);
					}
				}
				else
				{
				//Turn on Termination only if it is ON
					if (currentTerminationSetting != 0)
					{
					currentTerminationSetting = 0;
					LATCbits.LATC6 = 0;
					writeEEPROM(dEEPROM_CAN_TERMINATION_ADDRESS ,0x00);
					}
				}
                break;

            case dTOGGLE_LED:
//Check to see if we need to pass this on
//OUTPacket[0] Command byted TOGGLE_LED
//OUTPacket[1] USB 1 or CAN 2
//OUTPacket[2] What LED to change
//OUTPacket[3] What State to Change it too...
				if ((OUTPacket[1] == dUSB_MICRO) || (OUTPacket[1] == dBOTH_PICMICROS))
				{
				//If it is for us we do not need to pass it on
					if(OUTPacket[2] == dLED_USB_ONE)
					{
						if(OUTPacket[3] == dLED_STATE_ON)
						{
						mLED_1_On();
						}
						else
						{
						mLED_1_Off();
						}
					}
					if(OUTPacket[2] == dLED_USB_TWO)
					{
						if(OUTPacket[3] == dLED_STATE_ON)
						{
						mLED_2_On();
						}
						else
						{
						mLED_2_Off();
						}
					}
					if(OUTPacket[2] == dLED_ALL)
					{
						if(OUTPacket[3] == dLED_STATE_ON)
						{
						mLED_1_On();
						mLED_2_On();
						}
						else
						{
						mLED_1_Off();
						mLED_2_Off();
						}
     				SendReceiveSPIPacket(TRUE);
					}
				}
				else
				{
				//Pass it on to the CAN micro
                SendReceiveSPIPacket(TRUE);
				}
            	break;

            case dREAD_FW_VERSION:
				if (OUTPacket[1] == dUSB_MICRO)
				{
				request2SendIamAlive = dTRUE;
				}
				else
				{
                SendReceiveSPIPacket(TRUE);
				}
				break;

			case dDEBUG_MODE:
				debugMode = OUTPacket[1];
                SendReceiveSPIPacket(TRUE);
				break;

            default:
                //Currently do nothing for unimplemented commands
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

    INTCONbits.GIEH = 0;

    LATBbits.LATB2 = 0;
    //Now wait some delay for the interrupt latency on the SPI slave, and to allow
    //enough time for the slave firmware to prepare to receive a SPI transmission packet.
    for(i = 0; i < 20; i++);
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
            for(j = 0; j < 5u; j++);
            ImprovedNop();            
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
            for(j = 0; j < 5u; j++);
            ImprovedNop();              
        }        
    }    
    for(j = 0; j < 5u; j++);        //Give slave enough time to save off the last received byte
    LATAbits.LATA5 = 1;//De-assert SPI slave select
    
    //Increment byte count of bufferred data now that we received a packet.
    USBINBufByteCount += SPI_CAN_MESSAGE_SIZE;
    //Check if time for write pointer wrap around
    if(USBINBufWritePointer >= ((unsigned char*)&USBINBuffer[0] + USB_IN_BUF_SIZE))
    {
        USBINBufWritePointer = (unsigned char*)&USBINBuffer[0];
    } 
    
    INTCONbits.GIEH = 1;
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
/** EOF main.c ***************************************************************/
