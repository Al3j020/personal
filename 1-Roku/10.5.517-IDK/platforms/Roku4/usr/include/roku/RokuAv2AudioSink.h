/*
 * Copyright (c) 2020-2021 Roku, Inc. All rights reserved.
 * This software and any compilation or derivative thereof is,
 * and shall remain, the proprietary information of Roku, Inc. and
 * is highly confidential in nature. Reproduction in whole or in part
 * is prohibited without the prior written consent of Roku, Inc.
 *
 * Under no circumstance may this software or any derivative thereof be
 * combined with any third party software, including open source software,
 * without the written permission of the Copyright owner.
 */
#ifndef ROKU_AV2_AUDIO_SINK_H
#define ROKU_AV2_AUDIO_SINK_H

#ifdef __cplusplus
extern "C" {
#endif
#include "RokuAv2Types.h"

namespace Roku {
namespace NDK {
namespace AV2 {
class Fader;
} // namespace AV2
} // namespace NDK
} // namespace Roku

//----------------------------------------------------------------------------
// DATA TYPES
//----------------------------------------------------------------------------

typedef struct RokuAv2_ASinkHandle RokuAv2_ASink;

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkQuery:
//
// DESCRIPTION: Queries audio sink handle associated with given decoder.
//
//              The reference count of the audio sink object is incremented
//              each time this function is called.
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink [out]: Opaque pointer to audio sink object, if this call succeeds
//               sink points to RokuAv2_ASink object.
//   decoder_id [in]: Decoder connected to audio sink.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, audio sink object created.  
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_ASinkQuery(
    RokuAv2_ASink **sink,
    RokuAv2_DecoderId decoder_id
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkRelease:
//
// DESCRIPTION: Releases the audio sink object and frees all allocated
//              resources.
//              Note, the audio sink object is ref counted, resources are only
//              freed after all references have been released.
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to audio sink object.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, audio sink object released.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_ASinkRelease(
    RokuAv2_ASink *sink
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkStartVolumeFade
//
// DESCRIPTION: Starts fading the media audio volume until the given volume
//              is reached. The easing function and duration control the fade
//              operation.
//              If another fade process is ongoing, it is interrupted and a new
//              one is started.
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink   [in]: Pointer to opaque audio sink object.
//   params [in]: Volume fade parameters
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, volume fade set.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t size;             // sizeof(RokuAv2_ASinkStartVolumeFadeParams)
    int      volume;           // desired playback volume
    uint32_t duration_ms;      // fade duration in ms
    RokuAv2_EasingFunc easing_func; // easing function
} RokuAv2_ASinkStartVolumeFadeParams;

RokuAv2_Err RokuAv2_ASinkStartVolumeFade(
    RokuAv2_ASink* sink,
    RokuAv2_ASinkStartVolumeFadeParams const* params
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkQueueVolumeFade
//
// DESCRIPTION: Queue a volume fade that's linked to a playback
//              position. The volume fade will start at a given playback
//              position.
//
//              Available since API version 2.
//              
// PARAMETERS:
//   sink [in]:    Pointer to opaque audio sink object.
//   params [in]:  Volume fade request parameters.
//
// RETURN VALUES:
//   ROKU_OK: Call succeeded and locked
//   ROKUAV_ERR_INTERNAL: The call failed due to an unexpected error.
//   ROKUAV_ERR_INVALID_HANDLE: Call failed
//
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t     size;            // sizeof(RokuAv2_ASinkQueueVolumeFadeParams)
    RokuAv2_Time fade_out;        // playback position at which fade out starts
    RokuAv2_Time fade_in;         // playback position at which fade in starts
    uint32_t     out_duration_ms; // fade out duration in ms
    uint32_t     in_duration_ms;  // fade in duration in ms
    RokuAv2_EasingFunc easing_func; // easing function
} RokuAv2_ASinkQueueVolumeFadeParams;

RokuAv2_Err RokuAv2_ASinkQueueVolumeFade(
        RokuAv2_ASink *sink,
        RokuAv2_ASinkQueueVolumeFadeParams const* params);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkGetVolumeFade
//
// DESCRIPTION: Returns the current volume fade scale.
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink   [in]:  Pointer to opaque audio sink object.
//   volume [out]: Current volume fade scale
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, volume fade scale was returned.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_ASinkGetVolumeFade(
    RokuAv2_ASink* sink,
    int* volume
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkPauseVolumeFade
//
// DESCRIPTION: Pause the active volume fade
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink  [in]:    Pointer to opaque audio sink object.
//   pause [in]:    pause or resume.
//
// RETURN VALUES:
//   ROKU_OK: Call succeeded and fade has been paused/resumed.
//   ROKUAV_ERR_INTERNAL: The call failed due to an unexpected error.
//   ROKUAV_ERR_INVALID_HANDLE: The provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_ASinkPauseVolumeFade (
    RokuAv2_ASink* sink,
    bool pause);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkMuteAudio
//
// DESCRIPTION: Mutes or un-mutes media audio
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Pointer to opaque audio sink object.
//   mute [in]: true of audio should be muted, false otherwise
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, audio mute set.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_ASinkMuteAudio(
        RokuAv2_ASink* sink,
        bool mute
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkDisableAtmosFormatOverlay
//
//
// DESCRIPTION: Disables/enables rendering of the Dolby Atmos logo. Channels
//              may temporary disable the logo if it interferes with other UI
//              elements.
//              The logo is enabled by default.
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink    [in]: Pointer to opaque audio sink object.
//   disable [in]: True if the atmos overlay should be disabled,
//                 false otherwise.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_ASinkDisableAtmosFormatOverlay(
        RokuAv2_ASink* sink,
        bool disable
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_ASinkEnableContAudOutAtmosLock
//
// DESCRIPTION: Lock the output to Dolby Atmos (for no pops between episodes).
//
//              Available since API version 2.
//              
// PARAMETERS:
//   sink [in]:    Pointer to opaque audio sink object.
//   enable [in]:  Tue to enable continuous atmos lock, false to disable lock.
//
// RETURN VALUES:
//   ROKU_OK: Call succeeded and locked
//   ROKUAV_ERR_INTERNAL: The call failed due to an unexpected error.
//   ROKUAV_ERR_INVALID_HANDLE: Call failed
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_ASinkEnableContAudOutAtmosLock (
        RokuAv2_ASink* sink,
        bool enable
);
        
#ifdef __cplusplus
}
#endif

#endif // ROKU_AV2_AUDIO_SINK_H
