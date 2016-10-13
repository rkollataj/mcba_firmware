/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        user.h
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
 * Rawin Rojvanit       11/19/04     Original.
 ********************************************************************/

#ifndef PICDEM_FS_DEMO_H
#define PICDEM_FS_DEMO_H

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"

/** D E F I N I T I O N S ****************************************************/
/* PICDEM FS USB Demo Version */
#define MINOR_VERSION   0x00    //Demo Version 1.00
#define MAJOR_VERSION   0x01

/* Temperature Mode */
#define TEMP_REAL_TIME  0x00
#define TEMP_LOGGING    0x01

/** S T R U C T U R E S ******************************************************/
typedef union DATA_PACKET
{
    byte _byte[USBGEN_EP_SIZE];  //For byte access
    word _word[USBGEN_EP_SIZE/2];//For word access(USBGEN_EP_SIZE msut be even)
    struct
    {
        enum
        {
            READ_VERSION    = 0x00,
            /*
            READ_FLASH      = 0x01,
            WRITE_FLASH     = 0x02,
            ERASE_FLASH     = 0x03,
            READ_EEDATA     = 0x04,
            WRITE_EEDATA    = 0x05,
            READ_CONFIG     = 0x06,
            WRITE_CONFIG    = 0x07,
            */
            ID_BOARD        = 0x31,
            UPDATE_LED      = 0x32,
            SET_TEMP_REAL   = 0x33,
            RD_TEMP         = 0x34,
            SET_TEMP_LOGGING= 0x35,
            RD_TEMP_LOGGING = 0x36,
            RD_POT          = 0x37,
			RD_DATA			= 0x38,
			RD_DATA_CONT	= 0x39,
			RD_DATA_CONT_END= 0x40,
			RD_DEV_NAME		= 0x41,


			LED_TOGGLE		= 0x51,  //LED Toggle Command
            WR_TO_CAN_TX_REQ = 0x52,   //WES added for lab
        	WR_TO_CAN_TX_PERIODIC_REQ = 0x53,
            RD_FROM_CAN_TX_REQ = 0x54,    //WES Is this needed
            SET_CAN_BITRATE_REQ = 0x55,    //WES added for lab
            SETUP_TRIGGER = 0x56,
            WRITE_REGISTER_DIRECTLY =  0x57,
            READ_REGISTER_DIRECTLY =  0x58,
			SETUP_TERMINATION_RESISTANCE =  0x59,
  			READ_FW_VER = 0x60,
  			DEBUG_MODE = 0x61,
            RESET_CAN = 0x62,
            SET_CAN_MODE_REQ = 0x63,
            SETUP_HW_FILTER = 0x64,
            RESET = 0xFF

//	#define dCHANGE_BIT_RATE 0xA1
//#define dTRANSMIT_MESSAGE_PD 0xA2
//#define dTRANSMIT_MESSAGE_EV 0xA3
//	#define dSETUP_TRIGGER 0xA4
//	#define dWRITE_REGISTER_DIRECTLY 0xA5
//	#define dREAD_REGISTER_DIRECTLY 0xA6
//	#define dTOGGLE_LED 0xA7
//	#define dSETUP_TERMINATION_RESISTANCE 0xA8
//	#define dREAD_FW_VERSION 0xA9
//	#define dDEBUG_MODE 0xB0
        }CMD;
        byte len;
    };
    struct
    {
        unsigned :8;
        byte ID;
    };
    struct
    {
        unsigned :8;
        byte led_num;
        byte led_status;
    };
    struct
    {
        unsigned :8;
        word word_data;
    };
} DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/
void UserInit(void);
void ProcessIO(void);
void highISR (void);

#endif //PICDEM_FS_DEMO_H
