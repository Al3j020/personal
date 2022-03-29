#ifndef ROKU_AV2_SAMPLE_H
#define ROKU_AV2_SAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "RokuAv2Types.h"

//----------------------------------------------------------------------------
// TYPE RokuAv2_CryptoSession:
//
// DESCRIPTION: Handle to DRM session.
//
//----------------------------------------------------------------------------
//
typedef uint32_t RokuAv2_CryptoSession;

//----------------------------------------------------------------------------
// STRUCT RokuAv2_ByteRanges:
//
// DESCRIPTION: DRM crypto ranges.
//
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t              num;  // Number of crypto ranges
    RokuAv2_Uint8Range*   data; // Crypto ranges array
} RokuAv2_ByteRanges;

//----------------------------------------------------------------------------
// STRUCT RokuAv2_DrmInfo:
//
// DESCRIPTION: Represents DRM metadata.
//
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t              size;           // Struct size
    RokuAv2_CryptoSession crypto;         // Drm session
    RokuAv2_Uint8Range    key;            // Drm license key
    RokuAv2_Uint8Range    iv;             // Initialization vector
    RokuAv2_Uint8Range    enc_pattern;    // Encryption pattern
    RokuAv2_ByteRanges    crypto_ranges;  // Crypto ranges
} RokuAv2_DrmInfo;

//----------------------------------------------------------------------------
// STRUCT RokuAv2_Sample:
//
// DESCRIPTION: Represents multimedia sample data.
//
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t           size;        // Struct size
    RokuAv2_EsType     es;          // Sample ES type
    RokuAv2_Time       pts;         // Presetnation timestamp
    RokuAv2_ByteRanges data_ranges; // Sample data ranges
    RokuAv2_DrmInfo*   drm;         // DRM meta information, should be NULL for
                                    // clear sample
} RokuAv2_Sample;

#ifdef __cplusplus
}
#endif

#endif // ROKU_AV2_SAMPLE_H
