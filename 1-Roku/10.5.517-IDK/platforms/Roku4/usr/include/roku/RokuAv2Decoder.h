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

#ifndef ROKU_AV2_DECODER_H
#define ROKU_AV2_DECODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RokuAv2Types.h"
#include "RokuAv2Format.h"
#include "RokuAv2Sample.h"
#include "RokuDrmSession.h"

//----------------------------------------------------------------------------
// DATA TYPES
//----------------------------------------------------------------------------

typedef struct RokuAvDecoderHandle RokuAv2_Decoder;

//----------------------------------------------------------------------------
// DECODER LIFETIME MANAGEMENT
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderCreate2:
//
// DESCRIPTION: Creates an audio or video decoder object. Users are responsible
//              of freeing any resources by calling RokuAv2_DecoderDestroy.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [out]: Opaque pointer to decoder object, if this call succeeds
//                  decoder points to newly created RokuAvDecoder object.
//   decoder_params [in]: Decoder initialization parameters.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, new decoder object created.
//   ROKUAV2_ERR_INVALID_VERSION: The API version was not set to a proper
//               value. Likely, RokuAv2_SetApiVersion was not prior to
//               RokuAv2_DecoderCreate.
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t              size;           // Struct size
    RokuAv2_DecoderId     decoder_id;     // Decoder id
    RokuAv2_EsType        es;             // Decoder ES type
} RokuAv2_DecoderCreateParams;

RokuAv2_Err RokuAv2_DecoderCreate2(
    RokuAv2_Decoder **decoder,
    RokuAv2_DecoderCreateParams const* decoder_params
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderDestroy:
//
// DESCRIPTION: Destroys decoder object and releases all allocated resources.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to previously created decoder object.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, decoder object destroyedokuAvDecoder_InitParams
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderDestroy(
    RokuAv2_Decoder* decoder
);

//----------------------------------------------------------------------------
// DECODER CONTROL
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderConfigure2:
//
// DESCRIPTION: Configure decoder for the new stream format. This call should
//              precede write RokuAv2_DecoderWriteSample calls with new sample
//              format.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   es_format [in]: Elementary stream format
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, decoder configured.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderConfigure2(
    RokuAv2_Decoder* decoder,
    RokuAv2_EsFormat const* es_format
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderStep:
//
// DESCRIPTION: Step decoder to particular stream position.
//
//              This function is typically used to implement arbitrary frame
//              start and should only be called before playback is started.
//              The proper call sequence is:
//              1. Application start feeding video samples from the IDR prior
//                 to the desired playback start.
//              2. Application calls RokuAv2_DecoderStep with the desired
//                 start position
//              3. The decoder starts decoding and does not render any frame
//                 with a PTS before the desired start. 
//              4. Application waits for the StepDone notification and calls
//                 RokuAv2_DecoderSetSpeed(1.0) when it's ready to start
//                 playback.
//
//              Available since API version 2.
//              
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   position [in]: Position in the stream to step to.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, decoder stepped.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderStep(
    RokuAv2_Decoder *decoder,
    RokuAv2_Time position
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderSetSpeed:
//
// DESCRIPTION: Sets decoder playback speed; 1.0 is regural speed and a value
//              of 0 pauses playback.
//
//              The decoder speed is set to 0 at creation time and reset to 0
//              when RokuAv2_DecoderStop is called.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   speed [in]: Decoder playback speed.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, playback started.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderSetSpeed(
    RokuAv2_Decoder *decoder,
    float speed
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderStop:
//
// DESCRIPTION: Stops the decoder and all active threads inside it. This
//              function must be called before RokuAv2_DecoderFlush
//              can be called.
//
//              Note, this function resets the speed value to 0.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, decoder stopped.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderStop(
    RokuAv2_Decoder *decoder
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderFlush:
//
// DESCRIPTION: Flush data buffer and reset decoder.
//              The decoder must be stopped and no other threads may call into
//              the decoder while RokuAv2_DecoderFlush os called.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, decoder flushed.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderFlush(
    RokuAv2_Decoder* decoder
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderWriteSample2:
//
// DESCRIPTION: Write new encoded media sample into decoder pipeline.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   sample [in]: Media sample.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, new sample pushed into decoder pipeline.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderWriteSample2(
    RokuAv2_Decoder* decoder,
    RokuAv2_Sample const* sample
);

//----------------------------------------------------------------------------
// DECODER QUERIES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderGetPosition:
//
// DESCRIPTION: Query current stream output position. 
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   position [out]: Current stream playback position.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, playback position returned.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderGetPosition (
    RokuAv2_Decoder* decoder,
    RokuAv2_Time* position
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderGetDecoderId:
//
// DESCRIPTION: Query the decoder ID
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   decoder_id [out]: ID of the decoder
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, decoder ID is returned.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle is invalid.
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderGetDecoderId(
    RokuAv2_Decoder* decoder,
    RokuAv2_DecoderId* decoder_id
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderGetDecoderStats:
//
// DESCRIPTION: Returns the current decoder statistics.
//
//              The decoder updates its statistics information as playback
//              goes on and this function retrieves the latest values. See
//              the definition of RokuAv_VideoDecoderStats for more information
//              about the decoder statistics properties.
//
//              Calling RokuAv2_DecoderStop resets all statistics to their
//              initial value (most likely 0).
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Pointer to opaque decoder object.
//   video_decoder_stats [out]: Pointer to a RokuAv_VideoDecoderStats structure.
//              GetDecoderStats copies the latest video decoder
//              statics into this caller provided memory.
//
// RETURN VALUES:
//   None.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_DecoderGetDecoderStats(
    RokuAv2_Decoder* decoder,
    RokuAv2_DecoderStats* video_decoder_stats
);

//----------------------------------------------------------------------------
// DECODER EVENTS SUBSCRIPTIONS
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderSetPlaybackStartedListener2:
//
// DESCRIPTION: Subscribe to playback started event. Decoder will generate
//              playback start event and call provided callback as soon as
//              decoder start output decoded frames. Provided user context
//              pointer will be passed to the callback. In order to cancel
//              subscription user should call this function with callback
//              set to NULL.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   callback [in]: Playback start event handler.
//   user_context [in]: User provided context pointer.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, playback statred callback updated.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle.
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t       size;    // struct size
    RokuAv2_EsType es;
} RokuAv2_DecoderPlaybackStartedParams;
typedef void (*RokuAv2_DecoderPlaybackStartedCb) (
    void* context, RokuAv2_DecoderPlaybackStartedParams const* params);
RokuAv2_Err RokuAv2_DecoderSetPlaybackStartedListener2(
    RokuAv2_Decoder *decoder,
    RokuAv2_DecoderPlaybackStartedCb callback,
    void *user_context
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderSetPlaybackEndedListener2:
//
// DESCRIPTION: Subscribe to playback ended event. Decoder will generate
//              playback ended event and call provided callback as soon as
//              it reach end of stream. Provided user context pointer will be 
//              passed to the callback. In order to cancel subscription user
//              should call this function with callback set to NULL.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   callback [in]: Playback ended event handler.
//   user_context [in]: User provided context pointer.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, playback ended callback updated.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle.
//----------------------------------------------------------------------------
// 
typedef struct {
    uint32_t       size;    // struct size
    RokuAv2_EsType es;
} RokuAv2_DecoderPlaybackEndedParams;
typedef void (*RokuAv2_DecoderPlaybackEndedCb) (
    void* context, RokuAv2_DecoderPlaybackEndedParams const* params);
RokuAv2_Err RokuAv2_DecoderSetPlaybackEndedListener2 (
    RokuAv2_Decoder *decoder,
    RokuAv2_DecoderPlaybackEndedCb callback,
    void *user_context
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderSetNewPositionListener2:
//
// DESCRIPTION: Subscribe to new position event. Decoder will generate new
//              position event periodicly and notify new playback position to 
//              user via provided callback. Provided user context pointer will
//              be passed to the callback. In order to cancel subscription
//              user should call this function with callback set to NULL.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   callback [in]: New playback position event handler.
//   user_context [in]: User provided context pointer.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, new playback position callback updated.  
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle.
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t       size;    // struct size
    RokuAv2_EsType es;
    RokuAv2_Time position;
} RokuAv2_DecoderNewPositionParams;
typedef void (*RokuAv2_DecoderNewPositionCb) (
    void* context, RokuAv2_DecoderNewPositionParams const* params);
RokuAv2_Err RokuAv2_DecoderSetNewPositionListener2(
    RokuAv2_Decoder *decoder,
    RokuAv2_DecoderNewPositionCb callback,
    void *user_context
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderSetUnderrunListener2:
//
// DESCRIPTION: Subscribe to buffer underrun event. Decoder will generate
//              buffer underrun  event and notify user via provided callback.
//              Provided user context pointer will be passed to the callback. 
//              In order to cancel subscription user should call this function 
//              with callback set to NULL.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   callback [in]: Buffer underrun event handler.
//   user_context [in]: User provided context pointer.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, buffer underrun sunbscription updated.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle.
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t       size;    // struct size
    RokuAv2_EsType es;
} RokuAv2_DecoderUnderrunParams;
typedef void (*RokuAv2_DecoderUnderrunCb) (
    void* context, RokuAv2_DecoderUnderrunParams const* params);
RokuAv2_Err RokuAv2_DecoderSetUnderrunListener2(
    RokuAv2_Decoder *decoder,
    RokuAv2_DecoderUnderrunCb callback,
    void *user_context
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderSetErrorListener2:
//
// DESCRIPTION: Subscribe to decoder error event. Decoder will generate
//              this event if generic error occured and notify user via
//              provided callback. Provided user context pointer will be
//              passed to the callback. In order to cancel subscription user
//              should call this function with callback set to NULL.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   callback [in]: Decoder error event handler.
//   user_context [in]: User provided context pointer.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, decoder error subscription updated.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle.
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t       size;    // struct size
    RokuAv2_EsType es;
    RokuAv2_Err    err;
    int64_t        drm_system_err;
} RokuAv2_DecoderErrorParams;
typedef void (*RokuAv2_DecoderErrorCb) (
    void* context, RokuAv2_DecoderErrorParams const* params);
RokuAv2_Err RokuAv2_DecoderSetErrorListener2(
    RokuAv2_Decoder *decoder,
    RokuAv2_DecoderErrorCb callback,
    void *user_context
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderSetStatsListener2:
//
// DESCRIPTION: Subscribe to decoder statistics event. Decoder will generate
//              decoder stats event and notify user via provided callback 
//              periodicaly. Provided user context pointer will be passed to
//              the callback. In order to cancel subscription user should call
//              this function with callback set to NULL.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   callback [in]: Decoder stats event handler.
//   user_context [in]: User provided context pointer.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, decoder stats sunbscription updated.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle.
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t       size;    // struct size
    RokuAv2_EsType es;
    RokuAv2_DecoderStats stats;
} RokuAv2_DecoderStatsParams;
typedef void (*RokuAv2_DecoderStatsCb) (
    void* context, RokuAv2_DecoderStatsParams const* params);
RokuAv2_Err RokuAv2_DecoderSetStatsListener2(
    RokuAv2_Decoder *decoder,
    RokuAv2_DecoderStatsCb callback,
    void *user_context
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_DecoderSetStepDoneListener2:
//
// DESCRIPTION: Subscribe to decoder step done event. Decoder will generate
//              this event when decoder releases frame in stepping mode. 
//              Provided user context pointer will be passed to the callback.
//              In order to cancel subscription user should call this function
//              with callback set to NULL.
//
//              Available since API version 2.
//
// PARAMETERS:
//   decoder [in]: Handle to decoder object.
//   callback [in]: Decoder stats event handler.
//   user_context [in]: User provided context pointer.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, step done sunbscription updated.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided decoder handle.
//----------------------------------------------------------------------------
//
typedef struct {
    uint32_t       size;    // struct size
    RokuAv2_EsType es;
    RokuAv2_Time   position;
} RokuAv2_DecoderStepDoneParams;
typedef void (*RokuAv2_DecoderStepDoneCb) (
    void* context, RokuAv2_DecoderStepDoneParams const* params);
RokuAv2_Err RokuAv2_DecoderSetStepDoneListener2(
    RokuAv2_Decoder *decoder,
    RokuAv2_DecoderStepDoneCb callback,
    void *user_context
);


#ifdef __cplusplus
}
#endif

#endif // ROKU_AV2_DECODER_H
