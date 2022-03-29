/*
 * Copyright (c) 2021 Roku, Inc. All rights reserved.
 * This software and any compilation or derivative thereof is,
 * and shall remain, the proprietary information of Roku, Inc. and
 * is highly confidential in nature. Reproduction in whole or in part
 * is prohibited without the prior written consent of Roku, Inc.
 *
 * Under no circumstance may this software or any derivative thereof be
 * combined with any third party software, including open source software,
 * without the written permission of the Copyright owner.
 */

#ifndef ROKU_AV2_H
#define ROKU_AV2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RokuAv2Types.h"

//----------------------------------------------------------------------------
// DEFINE: ROKU_AV2_THIS_API_VERSION
//
// DESCRIPTION: Defines the current interface version
//----------------------------------------------------------------------------
// 
#define ROKU_AV2_THIS_API_VERSION 2

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_GetApiVersion:
//
// DESCRIPTION: Returns the latest API version supported by RokuOS
//              on this device.
//
//              This is an optional function that channels may call to check
//              the availabilty of certain features. API version 2 is the
//              first version that was released to partners.
//
// PARAMETERS: None.
//
// RETURN VALUES:
//   RokuAv2_Version: latest supported API version.
//
//----------------------------------------------------------------------------
//
RokuAv2_Version RokuAv2_GetApiVersion();

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_SetApiVersion:
//
// DESCRIPTION: Sets the API version that the caller is using. This function
//              must be called before any other RokuAv2 interface function.
//              Callers must pass in 'ROKU_AV2_THIS_API_VERSION' to set
//              version the application was built against.
//
//                  RokuAv2_Err err = RokuAv2_SetApiVersion(
//                          ROKU_AV2_THIS_API_VERSION);
//
//              The decoder uses the API version to improve backwards
//              compatibility of the interface. For example, if future API
//              versions extend enum types, the decoder will not return newer
//              enum values to applications that don't expect them.
//
// PARAMETERS:
//   version [in]: Api version the caller is using
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_SetApiVersion(RokuAv2_Version version);

#ifdef __cplusplus
}
#endif

#endif // ROKU_AV2_H
