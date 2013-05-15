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
/// Author:       Nivis LLC, Mihaela Goloman
/// Date:         February 2009
/// Description:  This file holds the definitions of the device provisioning object of the DMAP
/// Changes:      Created
/// Revisions:    1.0
/// @endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _NIVIS_DMAP_DPO_H_
#define _NIVIS_DMAP_DPO_H_

#include "typedef.h"
#include "config.h"
#include "dmap_utils.h"

#define MAX_PUBLISH_ITEMS     10

#define CERTIFICATE_SIZE      8 //TODO check which is the size of a cert.
#define MAX_CERTIFICATES_NO   4 //TODO set which value is fitted

#define UAP_CO_VERSION        2   //same as for DAQ Concentrator version

typedef enum{
  DPO_ATTR_ZERO = 0,
  DPO_DEFAULT_NWK_ID = 1,
  DPO_DEFAULT_SYM_JOIN_KEY = 2,
  DPO_OPEN_SYM_JOIN_KEY = 3,
  DPO_DEFAULT_FREQ_LIST = 4,
  DPO_JOIN_METH_CAPABILITY = 5,
  DPO_ALLOW_PROVISIONING = 6,
  DPO_ALLOW_OVER_THE_AIR_PROVISIONING = 7,
  DPO_ALLOW_OOB_PROVISIONING = 8,
  DPO_ALLOW_RESET_TO_FACTORY_DEFAULTS = 9,
  DPO_ALLOW_DEFAULT_JOIN = 10,
  DPO_TARGET_NWK_ID = 11,
  DPO_TARGET_NWK_BITMASK = 12,
  DPO_TARGET_JOIN_METHOD = 13,
  DPO_TARGET_SECURITY_MNGR_EUI = 14,
  DPO_TARGET_SYSTEM_MNGR_ADDR = 15,
  DPO_TARGET_FREQUENCY_LIST = 16,
  DPO_TARGET_DL_CONFIG = 17,
  DPO_PKI_ROOT_CERTIFICATE = 18,
  DPO_NOF_PKI_CERTIFICATES = 19,
  DPO_PKI_CERTIFICATE = 20,
  DPO_CURRENT_UTC_ADJUSTMENT = 21,
  DPO_DAQ_PUB_COMM_ENDPOINT = 22,
  DPO_DAQ_PUB_OBJ_ATTR = 23,
  DPO_DEVICE_ROLE = 24,
  DPO_UAP_CO_VERSION = 25,
  DPO_ATTR_NO = 26
} DPO_ATTRIBUTES;

typedef enum{
  DPO_RESET_TO_DEFAULT = 1,
  DPO_WRITE_SYMMETRIC_JOIN_KEY = 2
}DPO_METHODS;

typedef enum{
  DEFAULT_JOIN_ONLY = 0,
  SYM_KEY_JOIN_ONLY = 1,
  PKI_JOIN_ONLY = 2,
  PKY_OR_SYM_KEY_JOIN
}DMAP_DPO_JOIN_CAPABILITY;

typedef struct{
  IPV6_ADDR m_aRemoteAddr128;
  uint16    m_unRemoteTLPort;
  uint16    m_unRemoteObjID;  
  int16     m_nPubPeriod;  
  uint8     m_ucIdealPhase;   //same as the Contract's "Assigned_Period" - percent from the publishing period
  uint8     m_ucStaleDataLimit;
  uint8     m_ucPubAutoRetransmit;
  uint8     m_ucCfgStatus; //0=not configured; 1=configured;
}COMM_ASSOC_ENDP;

typedef struct{
  uint16  m_unObjID;
  uint16  m_unAttrID;
  uint16  m_unAttrIdx;
  uint16  m_unSize;
}OBJ_ATTR_IDX_AND_SIZE;


typedef struct{
    uint8   m_ucStructSignature; 
    uint8   m_ucJoinMethodCapability; 
    uint8   m_ucAllowProvisioning;
    uint8   m_ucAllowOverTheAirProvisioning;
    uint8   m_ucAllowOOBProvisioning;
    uint8   m_ucAllowResetToFactoryDefaults;
    uint8   m_ucAllowDefaultJoin;
    uint8   m_ucTargetJoinMethod; //0 = symmetric key, 1 = public key
    uint16  m_unTargetNwkBitMask; // aligned to 4
    uint16  m_unTargetNwkID; 
    uint8   m_aJoinKey[16];        // aligned to 4
    uint8   m_aTargetSecurityMngrEUI[8];
//    uint16  m_aTargetSystemMngrAddr[16]; // g_stDMO.m_aucSysMng128BitAddr
    uint16  m_unTargetFreqList; //frequencies for advertisments which are to be used
//    uint8   m_aDlConfigInfo[MAX_PARAM_SIZE]; //1st octet = nof attributes; then a suite of (attr. number(may be id)- one octet and the octet string of the attribute)
//    uint8   m_aPKIRootCertificate[CERTIFICATE_SIZE];                // not supported (optional on ISA100) 
//    uint8   m_ucNofPKICertificates;                                 // not supported (optional on ISA100)
//    uint8   m_aPKICertificate[CERTIFICATE_SIZE*MAX_CERTIFICATES_NO];// not supported (optional on ISA100)
    int16   m_nCurrentUTCAdjustment;
    COMM_ASSOC_ENDP m_stDAQPubCommEndpoint;
    OBJ_ATTR_IDX_AND_SIZE m_aAttrDescriptor[MAX_PUBLISH_ITEMS];
    uint16 m_unDeviceRole;
    uint8  m_ucUAPCORevision;
}DPO_STRUCT;

extern const DMAP_FCT_STRUCT c_aDPOFct[DPO_ATTR_NO];
extern DPO_STRUCT  g_stDPO;  

#define g_stFilterBitMask  g_stDPO.m_unTargetNwkBitMask
#define g_stFilterTargetID g_stDPO.m_unTargetNwkID

void DPO_Init(void);

#define DPO_Read(p_unAttrID,p_punBufferSize,p_pucRspBuffer) \
            DMAP_ReadAttr(p_unAttrID,p_punBufferSize,p_pucRspBuffer,c_aDPOFct,DPO_ATTR_NO)

  #define DPO_Write(...)   SFC_READ_ONLY_ATTRIBUTE
//  #define DEVICE_ROLE 0x04 // BBR
  #define DEVICE_ROLE 0x08 //GW

uint8 DPO_Execute(uint8   p_ucMethID,
                   uint16  p_unReqSize, 
                   uint8*  p_pReqBuf,
                   uint16* p_pRspSize,
                   uint8*  p_pRspBuf);

uint8 DPO_ResetToDefault(uint16* p_pRspSize, uint8*  p_pRspBuf);

#endif //_NIVIS_DMAP_DPO_H_
