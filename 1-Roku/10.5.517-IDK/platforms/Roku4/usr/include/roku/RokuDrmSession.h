// Copyright (c) 2014 Roku Inc.  All rights reserved.

#ifndef ROKU_DRM_SESSION_H
#define ROKU_DRM_SESSION_H

#include <stdint.h>

#include "RokuDrmTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Opaque type for DRM session instance
typedef struct RokuDrm_Ses_ RokuDrm_Ses;
typedef void (*RokuDrm_EventHandler)( RokuDrm_Ses*, const char*, const uint8_t*, uint32_t, void* callback_data);


//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesGetHandle
//
// DESCRIPTION: Returns a handle for the given DRM session.
//
//              Each DRM session has a unique handle can be used to decrypt
//              samples via the RokuAv2_DecoderWriteSample function. The
//              handle should be assigned to the crypto field in the
//              RokuAv2_DrmInfo structure.
//
// PARAMETERS: session [in], DRM session object.
//
uint32_t
RokuDrm_SesGetHandle(RokuDrm_Ses* session);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesDestroy
//
// DESCRIPTION: Destroys a DRM session object. After calling this function the
//              DRM session object is no longer valid and can no longer be
//              used as input argument to any of the NDK API's.
//
//              An internal copy of the DRM session object is kept alive
//              as long as it is used by another component. For example, if the
//              DRM session is used by an AV player to decrypt an audio/video
//              stream the internal DRM session object will stay alive until
//              the AV player switches to a different DRM session or the player
//              is destroyed.
//
// PARAMETERS: session [in], DRM session object that will be destroyed.
//
RokuDrm_Error
RokuDrm_SesDestroy(RokuDrm_Ses* session);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesGenerateChallenge
//
// DESCRIPTION: Creates a challenge that is required by the license server
//              before it can generate a license.
//
// PARAMETERS: session [in], a valid DRM session object.
//             custom_data [in], optional user data that needs to be included in
//                 the challenge. This can be a data specific for the
//                 content that needs to be decrypted or other information the
//                 license server requires.
//             custom_data_size [in], number of bytes of user data.
//             challenge_buffer [out], pointer to a valid buffer of size
//                 'challenge_buffer_size'. Upon successful completion this
//                 buffer will contain the challenge.
//             challenge_buffer_size [in], size of the challenge buffer in bytes.
//             challenge_size [out], actual size of the challenge.
//
RokuDrm_Error
RokuDrm_SesGenerateChallenge(RokuDrm_Ses* session,
                             uint8_t const* custom_data,
                             uint32_t custom_data_size,
                             uint32_t challenge_buffer_size,
                             uint8_t* challenge_buffer,
                             uint32_t* challenge_size);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesSetLicense
//
// DESCRIPTION: Sets the license data. Typically, the license data contains
//              one or a set of keys that the DRM session can use to decrypt a
//              piece of content.
//
// PARAMETERS: session [in], a valid DRM session object.
//             license_data [in], buffer containing the license data.
//             license_size [in], size of license data in bytes.
//
RokuDrm_Error
RokuDrm_SesSetLicense(RokuDrm_Ses* session,
                      uint8_t const* license_data,
                      uint32_t license_size);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesGetSecureStopId
//
// DESCRIPTION: Returns secure stop id for this session.
//
// PARAMETERS: session [in], DRM session object that will be destroyed.
//             secure_stop_buffer [out], pointer to a valid buffer of size
//                 'secure_stop_buffer_size'. Upon successful completion this
//                 buffer will contain the secure stop ID.
//             secure_stop_buffer_size [in], size of the secure stop buffer in
//                 bytes.
//             secure_stop_size [out], actual size of the secure stop ID.
RokuDrm_Error
RokuDrm_SesGetSecureStopId(RokuDrm_Ses* session,
                           uint32_t secure_stop_buffer_size,
                           uint8_t* secure_stop_buffer,
                           uint32_t* secure_stop_size);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesCreateLicenseFromKeyLadder
//
// DESCRIPTION: Creates a license .
//
// PARAMETERS: session [in], a valid DRM session object.
//             license_data [in], buffer containing the license data.
//             license_size [in], size of license data in bytes.
//
RokuDrm_Error
RokuDrm_SesCreateLicenseFromKeyLadder(uint32_t nr_keys,
                                      RokuDrm_KeyConfig const* key_config,
                                      RokuDrm_CipherAlgo    cipher_algo,
                                      RokuDrm_CipherMode    cipher_mode,
                                      RokuDrm_CipherPadding cipher_padding,
                                      uint32_t license_buffer_size,
                                      uint8_t* license_buffer,
                                      uint32_t* license_size);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesInstallMessageCallback
//
// DESCRIPTION: Enable the app to receive events/messages back from the DRM subsystem
//
// PARAMETERS: session [in], a valid DRM session object.
//             callback [in], a pointer to a function where the message is going to be delivered.
//             NOTE: the message delivery is going to happen on a different thread from what invokes
//             this function call.

RokuDrm_Error
RokuDrm_SesInstallMessageCallback(RokuDrm_Ses* session, RokuDrm_EventHandler callback, void* callback_data);


//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesHasLicense
//
// DESCRIPTION: Asks whether a license given by key ID (KID) is owned by this session.
//
// PARAMETERS: session [in]: a valid DRM session object.
//             keyid_buffer_size[in]   : size of keyid buffer
//             keyid_buffer[in]        : keyId GUID that identifies a particular license
//             has_license [out]       : 1 - if this session owns the license identified by this key,
//                                       0 - if otherwise

RokuDrm_Error
RokuDrm_SesHasLicense(RokuDrm_Ses* session, uint32_t keyid_buffer_size, uint8_t const* keyid_buffer, uint32_t *has_license);

//-----------------------------------------------------------------------------
// FUNCTION: RokuDrm_SesGetCloneLicense
//
// DESCRIPTION: Gets a clone license for a session
//
// PARAMETERS: session [in]: a valid DRM session object.
//             clone_license_size       [in/out] : size of buffer to store clone license
//             clone_license_buffer     [in/out] : buffer to store clone license

RokuDrm_Error
RokuDrm_SesGetCloneLicense(RokuDrm_Ses* session, uint32_t* clone_license_size, char* clone_license_buffer);


#ifdef __cplusplus
}
#endif

#endif // ROKU_DRM_SESSION_H

