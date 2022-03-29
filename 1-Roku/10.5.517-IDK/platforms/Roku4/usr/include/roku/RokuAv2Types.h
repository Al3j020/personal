// Copyright (c) 2012 Roku Inc.  All rights reserved.

#ifndef ROKU_AV2_DECODER_TYPES_H
#define ROKU_AV2_DECODER_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//============================================================================
// Types and Defines
//

//----------------------------------------------------------------------------
// TYPE RokuAv2_DecoderId:
//
// DESCRIPTION: Identifies the primary, secondary, and other decoders.
//
//----------------------------------------------------------------------------
//
typedef uint32_t RokuAv2_DecoderId;

//----------------------------------------------------------------------------
// TYPE RokuAv2_Version:
//
// DESCRIPTION: Stores the API version
//
//----------------------------------------------------------------------------
//
typedef uint32_t RokuAv2_Version;

//----------------------------------------------------------------------------
// ENUM RokuAv2_Err:
//
// DESCRIPTION: List of error codes.
//
//----------------------------------------------------------------------------
//
typedef enum {
    ROKUAV2_OK,
    ROKUAV2_ERR_NOT_FOUND,
    ROKUAV2_ERR_NOT_SUPORTED,
    ROKUAV2_ERR_PENDING,
    ROKUAV2_ERR_INTERRUPTED,
    ROKUAV2_ERR_INVALID_HANDLE,
    ROKUAV2_ERR_INTERNAL,
    ROKUAV2_ERR_DRM_FAILED,
    ROKUAV2_ERR_DECRYPT_FAILED,
    ROKUAV2_ERR_DRM_NO_LICENSE,
    ROKUAV2_ERR_DECODE_FAILED,
    ROKUAV2_ERR_INVALID_VERSION
} RokuAv2_Err;

//----------------------------------------------------------------------------
// TYPE RokuAv2_Time
//
// DESCRIPTION: Stores time, depending on the function this type is used to
//              store milliseconds since start of playback or a
//              presentation timestamp value in Mhz.
//
//----------------------------------------------------------------------------
//
typedef int64_t RokuAv2_Time;

//----------------------------------------------------------------------------
// ENUM RokuAv2_AudioSupplementary:
//
// DESCRIPTION: Gives the source of supplementary audio data
//
//----------------------------------------------------------------------------
//
typedef enum {
    ROKU_AV2_AUDIO_SUPPLEMENTARY_NONE, // no supplementary audio
    ROKU_AV2_AUDIO_SUPPLEMENTARY_ES,   // delivered in separate stream
    ROKU_AV2_AUDIO_SUPPLEMENTARY_EAC3_SUBSTREAM1, // delivered in EAC3 substream
    ROKU_AV2_AUDIO_SUPPLEMENTARY_EAC3_SUBSTREAM2, // delivered in EAC3 substream
    ROKU_AV2_AUDIO_SUPPLEMENTARY_EAC3_SUBSTREAM3, // delivered in EAC3 substream
} RokuAv2_AudioSupplementary;

typedef enum {
    ROKU_AV2_AP_NONE,
} RokuAv2_AudioParam;

typedef enum {
    ROKU_AV2_VP_NONE,
    ROKU_AV2_VP_PROFILE, // char const *
    ROKU_AV2_VP_LEVEL,   // char const *
} RokuAv2_VideoParam;

typedef enum {
    ROKU_AV2_CGMS_NO_RESTRICTION = 0,
    ROKU_AV2_CGMS_COPY_NO_MORE = 1,
    ROKU_AV2_CGMS_COPY_ONCE = 2,
    ROKU_AV2_CGMS_COPY_NEVER = 3
} RokuAv2_CGMSProtectionFlag;

typedef enum {
    ROKU_AV2_ES_TYPE_AUDIO = 0,
    ROKU_AV2_ES_TYPE_VIDEO,
    ROKU_AV2_ES_TYPE_AUDIO_EOS,
    ROKU_AV2_ES_TYPE_VIDEO_EOS
} RokuAv2_EsType;

typedef struct {
    uint8_t* begin;
    uint8_t* end;
} RokuAv2_Uint8Range;

typedef struct {
    uint32_t size;
    // Number of frames that have been rendered since playback was started.
    // frame_render_cnt is increment each time a new decoded frame is rendered.
    uint32_t frame_render_cnt;

    // Number of frames that have been repeated since playback was started.
    // frame_repeat_cnt is incremented each time a new frame is not available
    // in time and the current frame is rendered an additional frame period.
    uint32_t frame_repeat_cnt;

    // number of frames that have been dropped since playback was started.
    // frame_drop_cnt is incremented each time the presentation time of a
    // decoded frame is too old to be rendered and the next frame is rendered
    // instead.
    uint32_t frame_drop_cnt;

    // number of bit stream errors since playback was started.
    // stream_err_cnt is incremented each time the decoder detects a bit
    // stream error.
    uint32_t stream_err_cnt;
} RokuAv2_DecoderStats;

typedef struct {
    uint32_t size;
    int64_t matrix_coefficients;
    int64_t bits_per_channel;
    int64_t chroma_subsampling_horz;
    int64_t chroma_subsampling_vert;
    int64_t cb_subsampling_horz;
    int64_t cb_subsampling_vert;
    int64_t chroma_siting_horz;
    int64_t chroma_siting_vert;
    int64_t range;
    int64_t transfer_characteristics;
    int64_t primaries;
    int64_t max_cll;
    int64_t max_fall;
    double primary_r_chromaticity_x;
    double primary_r_chromaticity_y;
    double primary_g_chromaticity_x;
    double primary_g_chromaticity_y;
    double primary_b_chromaticity_x;
    double primary_b_chromaticity_y;
    double whitepoint_chromaticity_x;
    double whitepoint_chromaticity_y;
    double luminance_max;
    double luminance_min;
} RokuAv2_ColorMatrix;

//----------------------------------------------------------------------------
// ENUM RokuAv2_DolbyVisionMode:
//
// DESCRIPTION: List of possible Dolby Vision modes.
//              See also RokuAvPlayer_SetDolbyVisionMode.
//
//
//----------------------------------------------------------------------------
//
typedef enum {
    ROKU_AV2_DV_OFF,
    ROKU_AV2_DV_PROFILE_0,
    ROKU_AV2_DV_PROFILE_1,
    ROKU_AV2_DV_PROFILE_2,
    ROKU_AV2_DV_PROFILE_3,
    ROKU_AV2_DV_PROFILE_4,
    ROKU_AV2_DV_PROFILE_5,
    ROKU_AV2_DV_PROFILE_6,
} RokuAv2_DolbyVisionMode;

/**
 * Video output pixel encodings.
 */
typedef enum
{
    ROKU_AV2_PIXENC_NULL,
    ROKU_AV2_PIXENC_RGB_444,
    ROKU_AV2_PIXENC_YCBCR_444,
    ROKU_AV2_PIXENC_YCBCR_422,
    ROKU_AV2_PIXENC_YCBCR_420,
 } RokuAv2_PixelEncoding;

/**
 * Quantization range.
 */
typedef enum
{
    ROKU_AV2_QUANTRANGE_NULL,
    ROKU_AV2_QUANTRANGE_LIMITED,
    ROKU_AV2_QUANTRANGE_FULL
} RokuAv2_QuantizationRange;

/**
 * Colorimetry information.
 */
typedef enum
{
    ROKU_AV2_COLORIMETRY_NULL,
    ROKU_AV2_COLORIMETRY_SMPTE170M,
    ROKU_AV2_COLORIMETRY_BT709,
    ROKU_AV2_COLORIMETRY_XVYCC601,
    ROKU_AV2_COLORIMETRY_XVYCC709,
    ROKU_AV2_COLORIMETRY_SYCC601,
    ROKU_AV2_COLORIMETRY_OPYCC601,
    ROKU_AV2_COLORIMETRY_OPRGB,
    ROKU_AV2_COLORIMETRY_BT2020YCCBCBRC,
    ROKU_AV2_COLORIMETRY_BT2020RGB_YCBCR
} RokuAv2_Colorimetry;

/**
 * Electro-optical transfer function.
 */
typedef enum
{
    ROKU_AV2_EOTF_NULL,
    ROKU_AV2_EOTF_BT1886,  // Traditional gamma for HDTV
    ROKU_AV2_EOTF_BT2100,
    ROKU_AV2_EOTF_SMPTE_ST_2084
} RokuAv2_Eotf;

typedef struct {
    uint32_t                  size;           // Struct size
    RokuAv2_PixelEncoding     pix_enc;
    int32_t                   frame_rate_mhz; // frame rate in millihertz
    int32_t                   color_depth;
    RokuAv2_QuantizationRange quantization_range;
    RokuAv2_Colorimetry       colorimetry;
    RokuAv2_Eotf              eotf;
} VideoOutputFormat;

typedef enum {
    ROKU_AV2_EASINGFN_NULL,
    ROKU_AV2_EASINGFN_LINEAR,    // linear easing
    ROKU_AV2_EASINGFN_INCUBIC,   // in-cubic easing
    ROKU_AV2_EASINGFN_OUTCUBIC   // out-cubic easing
} RokuAv2_EasingFunc;


#ifdef __cplusplus
}
#endif

#endif // ROKU_AV2_DECODER_TYPES_H
