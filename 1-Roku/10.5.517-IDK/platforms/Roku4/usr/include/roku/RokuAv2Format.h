#ifndef ROKU_AV2_FORMAT_H
#define ROKU_AV2_FORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "RokuAv2Types.h"

//----------------------------------------------------------------------------
// ENUM RokuAv2_AudioFormatFlags:
//
// DESCRIPTION: Audio format flags.
//
//----------------------------------------------------------------------------
//
typedef enum {
    ROKU_AV2_AF_SIGNED     = 1 << 0,
    ROKU_AV2_AF_BIG_ENDIAN = 1 << 1
} RokuAv2_AudioFormatFlags;

//----------------------------------------------------------------------------
// ENUM RokuAv2_CodecAudio:
//
// DESCRIPTION: Lists all audio codec formats.
//
//----------------------------------------------------------------------------
//
typedef enum {
  ROKU_AV2_CA_NULL,     // not applicable to specified stream
  ROKU_AV2_CA_UNKNOWN,  // not yet known/still being determined
  ROKU_AV2_CA_AAC_ADTS, // ISO/IEC 14496-3, Advanced Audio Coding, ADTS
  ROKU_AV2_CA_AAC_ADIF, // ISO/IEC 14496-3, Advanced Audio Coding, ADIF
  ROKU_AV2_CA_AC3,      // Dolby Digital
  ROKU_AV2_CA_EAC3,     // Dolby Digital Plus
  ROKU_AV2_CA_AC4,      // Dolby AC-4
  ROKU_AV2_CA_DTS,      // DTS Coherent Acoustics
  ROKU_AV2_CA_MP2,      // ISO/IEC 11172-3, MPEG Audio Layer II
  ROKU_AV2_CA_MP3,      // ISO/IEC 11172-3, MPEG Audio Layer III
  ROKU_AV2_CA_PCM,      // linear PCM (use configuration to further specify)
  ROKU_AV2_CA_WMA,      // Microsoft Windows Media Audio
  ROKU_AV2_CA_WMAPRO,   // Microsoft Windows Media Pro Audio
  ROKU_AV2_CA_FLAC,     // Free Lossless Audio Codec
  ROKU_AV2_CA_ALAC,     // Apple Lossless Audio Codec
  ROKU_AV2_CA_VORBIS,   // Ogg Vorbis
  ROKU_AV2_CA_OPUS,     // Xiph Opus
} RokuAv2_CodecAudio;

//----------------------------------------------------------------------------
// STRUCT RokuAv2_AudioFormat:
//
// DESCRIPTION: Audio stream format.
//
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t           size;           // size, sizeof(RokuAv2_AudioFormat)
    RokuAv2_CodecAudio codec;          // Audio codec
    uint16_t           flags;          // Sample encoding flags
    uint8_t            sample_size;    // Sample size in bits
    uint8_t            channels;       // Number of audio channels
} RokuAv2_AudioFormat;

//----------------------------------------------------------------------------
// ENUM RokuAv2_VideoFormatFlags:
//
// DESCRIPTION: Video format flags.
//
//----------------------------------------------------------------------------
//

typedef enum {
    ROKU_AV2_VF_INTERLACED  = 1 << 0,
    ROKU_AV2_VF_STILL_IMAGE = 1 << 1
} RokuAv2_VideoFormatFlags;

//----------------------------------------------------------------------------
// ENUM RokuAv2_CodecVideo:
//
// DESCRIPTION: Lists all video codec formats.
//
//----------------------------------------------------------------------------
//
typedef enum {
  ROKU_AV2_CV_NULL,      // not applicable to specified stream
  ROKU_AV2_CV_UNKNOWN,   // not yet known/still being determined
  ROKU_AV2_CV_MPEG2,     // ISO/IEC 13818-2, MPEG-2 part 2, H.262
  ROKU_AV2_CV_MPEG4_10B, // ISO/IEC 14496-10, MPEG-4 part 10, H.264, AVC
  ROKU_AV2_CV_MPEG4_15,  // ISO/IEC 14496-2, MPEG-4 part 2, H.263
  ROKU_AV2_CV_HEVC,      // High Efficiency Video Coding
  ROKU_AV2_CV_HEVC_B,    // High Efficiency Video Coding
  ROKU_AV2_CV_VP8,       // VP8 Video Codec
  ROKU_AV2_CV_VP9,       // VP9 Video Codec
  ROKU_AV2_CV_AV1        // AOMedia Video 1 codec
} RokuAv2_CodecVideo;

//----------------------------------------------------------------------------
// STRUCT RokuAv2_VideoFormat:
//
// DESCRIPTION: Video stream format.
//
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t                  size;           // size, sizeof(RokuAv2_VideoFormat)
    RokuAv2_CodecVideo        codec;          // Video codec
    RokuAv2_DolbyVisionMode   dvmode;         // Dolby Vision mode
    uint16_t                  flags;          // Sample encoding flags
    double                    frame_rate;     // Frames per second
    struct { uint32_t x, y; } par;            // Pixel aspect ratio
    struct { uint32_t w, h; } image;          // Source image size
} RokuAv2_VideoFormat;

//----------------------------------------------------------------------------
// STRUCT RokuAv2_EsFormat:
//
// DESCRIPTION: Media stream format.
//
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t       size;           // size, sizeof(RokuAv2_EsFormat)
    RokuAv2_EsType es;             // Sample ES type
    union {
        RokuAv2_AudioFormat* a;     // Audio ES format
        RokuAv2_VideoFormat* v;     // Video ES format
    } format;         // Audio or video ES format
} RokuAv2_EsFormat;

#ifdef __cplusplus
}
#endif

#endif // ROKU_AV2_FORMAT_H
