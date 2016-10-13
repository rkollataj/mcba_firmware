#ifndef CAN_TO_SPI__H
#define CAN_TO_SPI__H


//------------------------------------------------------------------------------
//Definitions
//------------------------------------------------------------------------------
#define dCAN_FW_VERSION_MAJOR 0xff
#define dCAN_FW_VERSION_MINOR 0x03

#define SPI_CS  PORTCbits.RC2 
////////////////// USED for CAN Periodic Transmissions
#define dNUMBER_OF_PERIODIC_MSGS 9 //up to 9 periodic messages allowed
#define dCAN_MESSAGE_DATA 19

#define dTRUE 1
#define dFALSE !dTRUE


#define dGARBAGE_DATA 1
#define dHEADER1 2
#define dHEADER2 3
#define dMESSAGE_WIP 4

#define dEEPROM_IN_USE_ADDRESS 0
#define dEEPROM_CAN_BITRATE_HI_ADDRESS 1
#define dEEPROM_CAN_BITRATE_LO_ADDRESS 2

#define dMODE_CONFIG 1
#define dMODE_NORMAL 2
#define dMODE_LISTEN_ONLY 3

#define dCAN_ID_FILTER 5
#define dNUMBER_HW_FILTER_IDS 20

#define dINIT_HW_FILTER 0x55
#define dSET_HW_FILTER_INCLUDE_IDS 0x01
#define dSET_HW_FILTER_EXCLUES_IDS 0x02

#define dHW_FILTER_OFF_STATE 0x00
#define dHW_FILTER_SETUP_STATE 0x01
#define dHW_FILTER_ON_STATE 0x02

#define dACTIVE_HW_FILTER 0x01
#define dNOT_ACTIVE_HW_FILTER 0x02

#define dTRIGGER_TRIGGERED 4  //Feedback to the GUI
#define dTRIGGER_ARMED 3 //Feedback to the GUI
#define dTRIGGER_SETUP 2 //Mainly used for the ISR
#define dTRIGGER_PAUSE 1 //Feedback to the GUI
#define dTRIGGER_OFF 0  //Feedback to the GUI

#define dTRIGGER_REPEAT 2
#define dTRIGGER_SINGLE 1


#define dCHECKSUM_LOCATION (dSIZE_OF_SPI_MSG-1)
#define dSPI_MSG_OK 0x55
#define dSPI_MSG_NG 0XFF
#define dRECEIVE_SPI_MESSAGE 0x1
#define dCHECK_SPI_MESSAGE 0x2
#define dRECEIVE_ACK_FROM_USB 0x3

#define dSPI_HEADER_BYTE1 0x55
#define dSPI_HEADER_BYTE2 0x46

#define dSPI_HEADER_BYTE1_STATE 0
#define dSPI_HEADER_BYTE2_STATE 1
#define dSPI_GET_THE_COMMAND_STATE 2
#define dSPI_GET_DATA_CHECK_MSG_CHECKSUM_STATE 3
#define dSPI_CLEANUP_QUEUE 4
#define dFINISHED_STATE 0x55

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

#define dNO_SPI_COMMAND 0x0

//Header to send to the CAN Part
#define dFIRST_HEADER 0xA5
#define dSECOND_HEADER 0xA1

//Header to send to the CAN Part
#define dFIRST_HEADER_2USB 0xB6
#define dSECOND_HEADER_2USB 0xA2

#define dSIZEOF_READ_FW_VERSION_RSP 5
#define dSIZEOF_CHANGE_BIT_RATE_RSP	9
#define dSIZEOF_TRANSMIT_MESSAGE_RSP 7
#define dSIZEOF_RECEIVE_MESSAGE	19
#define dSIZEOF_SETUP_TRIGGER_RSP 4
#define dSIZEOF_ERROR_COUNT_STATUS 25
#define dSIZEOF_READ_REGISTER_DIRECTLY_RSP 19
#define dSIZEOF_DEBUG_MODE_RSP 4
#define dSIZEOF_CAN_RESET 3

#define dSIZEOF_READ_FW_VERSION 3
#define dSIZEOF_CHANGE_BIT_RATE	3
#define dSIZEOF_CHANGE_CAN_MODE 3
#define dSIZEOF_TOGGLE_LED 4 //3
#define dSIZEOF_TRANSMIT_MESSAGE_PD	19
#define dSIZEOF_TRANSMIT_MESSAGE_EV	19
#define dSIZEOF_SETUP_TRIGGER 9
#define dSIZEOF_SETUP_HW_FILTER 7
#define dSIZEOF_WRITE_REGISTER_DIRECTLY 5
#define dSIZEOF_READ_REGISTER_DIRECTLY 5
#define dSIZEOF_DEBUG_MODE	25
#define dSIZEOF_MAX_SIZE 25 
////////////////////////////////


//The reason that these values are "spelled out" and not just 1 - n
//Is due to updates to the GUI still allowing older CAN FW to work
#define dCAN_20_KBPS_40MHZ 20
#define dCAN_33_3KBPS_40MHZ 33
#define dCAN_50KBPS_40MHZ 50
#define dCAN_80KBPS_40MHZ 80
#define dCAN_83_3KBPS_40MHZ 83
#define dCAN_100KBPS_40MHZ 100
#define dCAN_125KBPS_40MHZ 125
#define dCAN_150KBPS_40MHZ 150
#define dCAN_175KBPS_40MHZ 175
#define dCAN_200KBPS_40MHZ 200
#define dCAN_225KBPS_40MHZ 225
#define dCAN_250KBPS_40MHZ 250
#define dCAN_275KBPS_40MHZ 275
#define dCAN_300KBPS_40MHZ 300
#define dCAN_500KBPS_40MHZ 500
#define dCAN_625KBPS_40MHZ 625
#define dCAN_800KBPS_40MHZ 800
#define dCAN_1000KBPS_40MHZ 1000




typedef struct
{
    unsigned char MessageIDHeaderByte;
    unsigned char CANMessageBytes[13];
    unsigned char TimeStamp[4];
    unsigned char MessageChecksum;
}CAN_MESSAGE;    //19 byte structure




//------------------------------------------------------------------------------
#endif //#ifndef CAN_TO_SPI__H
//End of file
//------------------------------------------------------------------------------
