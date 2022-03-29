// Copyright (c) 2014 Roku Inc.  All rights reserved.

#ifndef ROKU_DRM_ENGINE_H
#define ROKU_DRM_ENGINE_H

#include <stdint.h>

#include "RokuDrmSession.h"
#include "RokuDrmTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Opaque type for DRM engine object.
typedef struct RokuDrm_Eng_ RokuDrm_Eng;

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngCreate
//
// DESCRIPTION: Creates a new decryption engine for a specific DRM system.
//
// PARAMETERS: drm_system [in], a string that identifies the DRM system that
//                 should be used for this DRM session. Supported DRM systems
//                 are: PlayReady.
//             custom_data [in], DRM system specific initialization data.
//             custom_data_size [in], number of initialization data bytes.
//             engine [out], upon successful completion this will point
//                 to a valid DRM engine object.
//
RokuDrm_Error
RokuDrm_EngCreate(char const* drm_system,
                  uint8_t const* custom_data,
                  uint32_t custom_data_size,
                  RokuDrm_Eng** engine);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngDestroy
//
// DESCRIPTION: Destroys a DRM engine object. After calling this function the
//              DRM engine object is no longer valid and can no longer be
//              used as input argument to any of the NDK API's.
//
// PARAMETERS: engine [in], DRM engine object that will be destroyed.
//
RokuDrm_Error
RokuDrm_EngDestroy(RokuDrm_Eng* engine);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngCreateSession
//
// DESCRIPTION: Creates a DRM session that can be used to decrypt data. The
//              DRM session will use the same DRM system as the engine that
//              created the session.
//
// PARAMETERS: engine [in], DRM engine object that will be destroyed.
//             init_data [in], Session specific initialization data.
//             int_data_size [in], Number of initialization data bytes.
//             session [out], upon successful completion this will point
//                 to a valid DRM session object.
//
RokuDrm_Error
RokuDrm_EngCreateSession(RokuDrm_Eng* engine,
                         uint8_t const* init_data,
                         uint32_t int_data_size,
                         RokuDrm_Ses** session);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngEnableSecureStop
//
// DESCRIPTION: Enables or disables secure stop.
//
// PARAMETERS: engine [in], DRM engine object.
//             enable [in], boolean, if true secure stop will be enabled, else
//                          secure stop will be disabled.
//
RokuDrm_Error
RokuDrm_EngEnableSecureStop(RokuDrm_Eng* engine,
                            bool enable);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngGetSecureStop
//
// DESCRIPTION: Returns verification data for the given secure stop ID.
//
// PARAMETERS: engine [in], DRM engine object.
//             secure_stop [in], Secure stop ID.
//             buffer_size [in], Verification data buffer size.
//             verify_data [in], Pointer to verification data buffer. Upon
//                 successful return this buffer will contain the secure stop
//                 verification data.
//             verify_size [out], Actual verification data size.
//
RokuDrm_Error
RokuDrm_EngGetSecureStop(RokuDrm_Eng* engine,
                         uint8_t* secure_stop,
                         uint32_t secure_stop_size,
                         uint32_t verify_buffer_size,
                         uint8_t* verify_data,
                         uint32_t* verify_size);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngReleaseSecureStop
//
// DESCRIPTION: Releases the secure stop data for the given ID. The caller would
//              typically call this function after the secure stop data has been
//              verified by the server.
//
// PARAMETERS: engine [in], DRM engine object.
//             secure_stop [in], Secure stop ID.
//             buffer_size [in], Verification data buffer size.
//
RokuDrm_Error
RokuDrm_EngReleaseSecureStop(RokuDrm_Eng* engine,
                             uint8_t* secure_stop,
                             uint32_t secure_stop_size);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngReleaseSecureStopEx
//
// DESCRIPTION: Releases the secure stop data for the given ID. Passes a full server response
//              in addition to just securestopid. Can be used for Playready 3.x
//
// PARAMETERS: engine [in], DRM engine object.
//             secure_stop [in], Secure stop ID.
//             secure_stop_size [in], Verification data buffer size.
//             secure_stop_body [in], complete server response.
//             secure_stop_body_size [in], server response buffer size.
//
RokuDrm_Error
RokuDrm_EngReleaseSecureStopEx(RokuDrm_Eng* engine,
                             uint8_t* secure_stop,
                             uint32_t secure_stop_size,
                             uint8_t* secure_stop_body,
                             uint32_t secure_stop_body_size);


//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngGetSecureStopIds
//
// DESCRIPTION: Returns all the secure stop ids currently pending
//
// PARAMETERS: engine [in], DRM engine object.
//
//    @sessionIds[out] secureStopIds vector of session ids (each a 16 byte vector)
//    @sessionIds_size [in] max number of @sessionIds to fit into buffer
//                    [out] actual number of @sessionIds placed in the buffer

RokuDrm_Error
RokuDrm_EngGetSecureStopIds(RokuDrm_Eng* engine, uint8_t * sessionIds, uint32_t * sessionIds_size);


//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngGetSystemTime
//
// DESCRIPTION: Returns the current DRM system time.
//
// PARAMETERS: engine [in], DRM engine object.
//             drm_time [out], Returned DRM system time.
//
RokuDrm_Error
RokuDrm_EngGetSystemTime(RokuDrm_Eng* engine,
                         RokuDrm_Time* drm_time);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngGetSecureStoreHash
//
// DESCRIPTION: Returns the PlayReady secure store hash
//
// PARAMETERS: engine [in], DRM engine object.
//             uint8_t* [out], Returned secure store hash.
//
RokuDrm_Error
RokuDrm_EngGetSecureStoreHash(RokuDrm_Eng* engine,
                         uint8_t* hash);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngGetKeyStoreHash
//
// DESCRIPTION: Returns the PlayReady secure store hash
//
// PARAMETERS: engine [in], DRM engine object.
//             uint8_t* [out], Returned key store hash.
//
RokuDrm_Error
RokuDrm_EngGetKeyStoreHash(RokuDrm_Eng* engine,
                         uint8_t* hash);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngDeleteSecureStore
//
// DESCRIPTION: Deletes the PlayReady secure store
//
// PARAMETERS: engine [in], DRM engine object.
//
RokuDrm_Error
RokuDrm_EngDeleteSecureStore(RokuDrm_Eng* engine);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngDeleteKeyStore
//
// DESCRIPTION: Deletes the PlayReady key store
//
// PARAMETERS: engine [in], DRM engine object.
//
RokuDrm_Error
RokuDrm_EngDeleteKeyStore(RokuDrm_Eng* engine);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_Version
//
// DESCRIPTION: Returns the DRM-system-specific version string. Not available for all
//  DRM types.
// format: <drm-system> <major>.<minor> ex; "playready 2.5"
// PARAMETERS: engine [in], DRM engine object.
//             version [in] an valid, allocated buffer of at least ** 32 bytes **,
//                     [out] version value copied into the buffer
//

RokuDrm_Error RokuDrm_EngVersion(RokuDrm_Eng* engine, char * version);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_EngSetDrmParams
//
// DESCRIPTION: Sets DRM system specific parameters. Not available for all
//  DRM types.
//
// PARAMETERS: engine [in], DRM engine object
//             command_id [in], Command ID
//             input_data [in], optional input data
//             input_data_size[in] size of input data
//             output_data [out], optional output data
//             output_data_size [in/out] size of output data
//             result_code [out] optional result code
//
RokuDrm_Error
RokuDrm_EngSetDrmParams(RokuDrm_Eng* engine,
                        uint32_t command_id,
                        uint8_t const* input_data,
                        uint32_t input_data_size,
                        uint8_t* output_data,
                        uint32_t* output_data_size,
                        uint32_t* result_code);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #define ROKU_DRM_ENGINE_H

