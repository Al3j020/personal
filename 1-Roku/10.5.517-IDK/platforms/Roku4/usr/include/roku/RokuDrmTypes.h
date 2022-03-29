// Copyright (c) 2014 Roku Inc.  All rights reserved.

#ifndef ROKU_DRM_TYPES_H
#define ROKU_DRM_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  ROKU_DRM_ERR_OK = 0,
  ROKU_DRM_ERR_INVALID_SESSION,
  ROKU_DRM_ERR_ARG_SIZE_TOO_LARGE,
  ROKU_DRM_ERR_INSUFFICIENT_BUFFER_SIZE,
  ROKU_DRM_ERR_INTERNAL,
  ROKU_DRM_ERR_NOT_IMPL,
  ROKU_DRM_ERR_BAD_ARG
} RokuDrm_Error;

typedef enum {
  ROKU_CALGO_NONE = 0,
  ROKU_CALGO_AES,
  ROKU_CALGO_3DES,
  ROKU_CALGO_MAX,
} RokuDrm_CipherAlgo;

typedef enum {
  ROKU_CMODE_NONE = 0,
  ROKU_CMODE_ECB,
  ROKU_CMODE_CBC,
  ROKU_CMODE_MAX,
} RokuDrm_CipherMode;

typedef enum {
  ROKU_ESCHEME_NONE,    ///< No encryption applied
  ROKU_ESCHEME_CENC,    ///< AES-CTR mode full sample and video NAL subsample enc.
  ROKU_ESCHEME_CENS,    ///< AES-CTR mode partial video NAL pattern encryption
  ROKU_ESCHEME_CBC1,    ///< AES-CBC mode full sample and video NAL subsample enc.
  ROKU_ESCHEME_CBCS,    ///< AES-CBC mode partial video NAL pattern encryption
} RokuDrm_EncScheme;

typedef enum {
  ROKU_CPAD_NONE = 0,
  ROKU_CPAD_MAX
} RokuDrm_CipherPadding;

typedef enum {
  ROKU_COP_NONE = 0,
  ROKU_COP_DECRYPT,
  ROKU_COP_ENCRYPT,
  ROKU_COP_MAX
} RokuDrm_CipherOperation;

typedef struct {
  RokuDrm_CipherAlgo cipher_algo;
  RokuDrm_CipherOperation operation; // encrypt or decrypt
  uint32_t key_size;
  uint8_t  key[32];
  uint32_t iv_size;
  uint8_t  iv[32];
} RokuDrm_KeyConfig;

// command_ids to use with RokuDrm_EngSetDrmParams
// these are DRM system specific commands used to set
// certain DrmEngine properties
// this enum must be kept in sync with corresponding
// enums in DrmEngine.h and DrmMarshalTypes.h
typedef enum {
    ROKU_INVALID_COMMAND = -1,
    // widevine
    ROKU_WV_SET_PRIVACY_MODE_CMD = 0,
    ROKU_WV_SET_SERVICE_CERT_CMD,
    ROKU_WV_SET_PERSIST_LICENSE_CMD,
    ROKU_WV_SET_APP_PARAMETERS,
    ROKU_WV_GET_METRICS
    // add commands for other DRM systems as needed here
} RokuDrm_SetDrmParamCmd;

typedef uint64_t RokuDrm_Time;

#ifdef __cplusplus
}
#endif


#endif // ROKU_DRM_TYPES_H
