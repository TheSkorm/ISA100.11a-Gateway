/*
* Copyright (C) 2013 Nivis LLC.
* Email:   opensource@nivis.com
* Website: http://www.nivis.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
* 
* Redistribution and use in source and binary forms must retain this
* copyright notice.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @verbatim
/// Author:       Nivis LLC, Mircea Vlasin
/// Date:         December 2007
/// Description:  MAC Extension Layer Data Entity
/// Changes:      Rev 1.0 - created
/// Revisions:    1.0 - by author
/// @endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _NIVIS_MLDE_H_
#define _NIVIS_MLDE_H_

#include <string.h>

#include "typedef.h"
#include "global.h"


#define MAX_DHDR_LENGTH                   4    //maximum length of DHDR when the "Extended Frame Control" byte is necessary
#define MAX_PHY_PAYLOAD_LEN               125  //maximum length of the PHY level payload

#define INVALID_IDX                       0xff

#define SHORT_ADDRESS_MODE                0x02 // 16 bit short address
#define EXTENDED_ADDRESS_MODE             0x03 // 64 bit extended address

//#define  MULTIPLE_16_BYTES_AES_PARAMS

#define   DLL_MAX_DAUX_HDR_LEN            30
#define   DLL_MAX_DMXHR_HDR_LEN           11

#define   DLL_ADVERTISMENT_DAUX           0
#define   DLL_SOLICITATION_DAUX           1
#define   DLL_ACTIVATE_IDLE_SF_DAUX       2
#define   DLL_LQI_RSSI_REPORT_DAUX        3

#define   DLL_EXT_FRAME_CTRL_MASK         0x10

#define   DLL_ADV_SF_INFO_MIN_LEN         4     //minimum length in bytes for the advertisment superframe info
#define   DLL_ADV_MAX_SUPERFRAME_LEN      4096  //maximum length in timeslots for an advertisment superframe

#define   DLL_MHR_SEQ_NO_OFFSET           2
#define   DLL_MHR_SUBNET_ID_OFFSET        3
#define   DLL_MHR_DEST_ADDR_OFFSET        5     //offset of the destination address inside the MHR header

#define   DLL_MHR_FC_DEST_ADDR_MASK       0x0C
#define   DLL_MHR_FC_DEST_ADDR_OFFSET     0x02
#define   DLL_MHR_FC_SRC_ADDR_MASK        0xC0
#define   DLL_MHR_FC_SRC_ADDR_OFFSET      0x06


#define   DLL_DHDR_DHR_LEN_OFFSET         1     //offset of the information related to the length in bytes
                                                //of the ISA100 header
#define   DLL_DHDR_BASE_LINE_OFFSET       2
#define   DLL_DHDR_EXT_FRAME_OFFSET       3


//masks of the advertisment options flags
#define   DLL_MASK_DAUX_TYPE              0x07
#define   DLL_ADV_OPT_SKIP_CH             0x08
#define   DLL_ADV_OPT_SF_SLOW_HOP         0x10
#define   DLL_ADV_OPT_SF_RAND             0x20
#define   DLL_ADV_OPT_CH_RAND             0x40
#define   DLL_ADV_RESERVED                0x80

//incoming frame response
#define DLL_DISCARD_MSG                   0
#define DLL_NO_ACK_SEND_MSG_BROADCAST     1
#define DLL_NO_ACK_SEND_MSG_DAUX          2
#define DLL_ACK_SEND_MSG                  3
#define DLL_NAK_SEND_MSG_DAUX             4
#define DLL_NAK_DISCARD_MSG               5
#define DLL_NO_ACK_SEND_MSG_BROAD_DAUX    6
#define DLL_DISCARD_MSG_NOT_FOR_ME        7


enum
{
  DEVICE_DISCOVERY     = 0, // device in discovery state: wait for an advertisement
  DEVICE_RECEIVED_ADVERTISE,
  DEVICE_FIND_ROUTER_EUI64,
  DEVICE_WAIT_ROUTER_EUI64,
  DEVICE_SECURITY_JOIN_REQ_SENT,
  
  DEVICE_SEND_SM_JOIN_REQ,
  DEVICE_SM_JOIN_REQ_SENT,
  
  DEVICE_SEND_SM_CONTR_REQ,
  DEVICE_SM_CONTR_REQ_SENT,     // a join contract request was sent through the data path by DMAP
    
  DEVICE_JOIN_TIMEOUT,      //
  DEVICE_JOIN_BACKOFF,      // join timeout. backoff and retry after a delay
  DEVICE_SEND_SEC_CONFIRM_REQ, 
  DEVICE_SEC_CONFIRM_REQ_SENT,

  DEVICE_JOINED,            // device already joined

  DEVICE_STATES_NO
};

/*
typedef struct
{
  uint32 m_ulDllTaiSecond;     //current TAI time in seconds
  uint16 m_unDllTaiFraction;   //fractional TAI seconds in units of 2^(-15) seconds(one clock tick)
  uint16 m_unDllSlotLen;       //timeslot length in 2^(-20) seconds (~usec)
} DLL_ADV_TIME_SYNC;
*/
extern uint8 g_ucDiscoveryState;

extern uint8 g_ucDllECNStatus;

//global variables related to security sublayer
#define DEFAULT_ISA100_SEC_LAYER      SECURITY_CTRL_ENC_MIC_32


#define PHY_LAYER                     0
#define DLL_LAYER                     1
#define NWK_LAYER                     2
#define TRANS_LAYER                   3
#define APP_LAYER                     4

extern uint8 g_ucMsgSendCnt;        //counter of the send messages during last second
//end security sublayer

extern const uint8 c_aucInvalidEUI64Addr[8];

extern uint8 g_aucDllRxTxBuf[MAX_PHY_PAYLOAD_LEN+3];

extern uint8 g_ucExtFrameControl;

/* Tx ACK PDU item */
#define MAX_ACK_LEN                     26
extern uint8 g_oAckPdu[MAX_ACK_LEN];
extern uint8 g_ucAckLen;
extern uint8 g_ucMsgForMe;
//extern DLL_RX_MHR   g_stDllRxHdrExt;

#define DLL_MASK_NEED_DAUX           0x01
#define DLL_MASK_NEED_EUI64          0x02
#define DLL_MASK_NEED_LQI            0x04
#define DLL_MASK_NO_NEED_ACK         0x08

//global variables related to the solicitation transmit within DAUX header
//extern uint16 g_unDauxTgtInterval;    //target time interval in 2^(-20) seconds(~usec)
//extern uint16 g_unDauxSubnetMask;     //subnet mask to be validated by the active scanning host


//extern uint8 MLDE_GenerateSolicitDauxHdr ( uint8 p_ucTargetCount, uint16 p_unTargetInterval, uint16 p_unSubnetMask, uint8* p_pDauxHdr );
//extern uint8 MLDE_GenerateActSfDauxHdr ( uint8 p_ucSuperframeId, uint16 p_unActivateSfTime, uint8* p_pDauxHdr );
//extern uint8 MLDE_GenerateLqiRssiDauxHdr ( uint8 p_ucLqiIndicator, uint8 p_ucRssiIndicator, uint8* p_pDauxHdr );

uint8 MLDE_OutgoingFrame(void);
uint8 MLDE_IncomingFrame( uint8 * p_pRxBuff   );
uint8 MLDE_IncomingHeader( uint8 * p_pRxBuff );
void MLDE_ProcessIncomingFrame( uint8 p_ucStatus, uint8 * p_pRxBuff);
const uint8* MLDE_InterpretDauxHdr( const uint8* p_pucDauxHdr);

void MLDE_Init();

#endif // _NIVIS_MLDE_H_

