// Copyright (c) 2014 Roku Inc.  All rights reserved.

#ifndef ROKU_SFX_PCM_H
#define ROKU_SFX_PCM_H

#include <stdbool.h>
#include <stdint.h>

#include "RokuSndFxTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ROKU_SFX_DLL_EXPORTED __attribute__((__visibility__("default")))

typedef enum {
  ROKU_SFX_STATE_OPENED,
  ROKU_SFX_STATE_PREPARED,
  ROKU_SFX_STATE_PLAYING,
  ROKU_SFX_STATE_DRAINING,
  ROKU_SFX_STATE_STOPPING,
  ROKU_SFX_STATE_UNDERRUN
} RokuSndFx_PcmState;

typedef struct RokuSndFx_PcmStatus_ {
  RokuSndFx_PcmState state;
  int32_t  delay;     // approximate number of frames waiting to be mixed
  uint32_t avail;     // free space in buffer, in frames
  uint32_t avail_max; // total size of buffer, in frames
  uint32_t processed; // total samples processed by hardware, may wrap around
  int32_t  pp_delay;  // post processing delay, in frames
} RokuSndFx_PcmStatus;

typedef struct RokuSndFx_PcmFormat_ {
  RokuSndFx_Endianness endian;
  bool     sign;   // true if signed format
  uint32_t width;  // number of bits per sample
  uint32_t num_ch; // number of channels
  uint32_t rate;   // sample rate
} RokuSndFx_PcmFormat;


typedef void (*RokuSndFx_PcmCb)(void*);

// Opaque type for DRM session instance
typedef struct RokuSndFx_Pcm_ RokuSndFx_Pcm;

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmCreate
//
// DESCRIPTION: Creates a new PCM sound effects player instance. This instance
//              can be used to play PCM sounds
//
// PARAMETERS: sfx_player [in], a valid DRM session object.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmCreate(RokuSndFx_Pcm** sfx_player);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmDestroy
//
// DESCRIPTION: Destroys PCM sound effects player instance. All resources owned
//              by this player instance will be released. Callers can no longer
//              use this player instance after it has been closed.
//
// PARAMETERS: sfx_player [in], a valid DRM session object.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmDestroy(RokuSndFx_Pcm* sfx_player);


//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmSetMaxDelay
//
// DESCRIPTION: Sets the maximum delay for this PCM player. The PCM
//              player maintains a ring buffer that stores audio frames
//              before they are being rendered. This function limits how many
//              frames can be stored in that buffer and by doing so limits the
//              delay of the audio player.
//
//              The maximum delay affects how many frames are available for
//              writing (RokuSndFx_PcmStatus.avail) as well as how long
//              RokuSndFx_PcmWaitBufferAvailable waits before returning.
//
//              Setting a delay value of 0 disables any maximum delay checks
//              in the player.
//
//              The max_delay value is in units of frames.
//
//              The maximum delay value can be translated to seconds by
//              dividing the value by the sample rate. For example a delay of
//              2400 frames for a 48KHz sample rate stream translates to a
//              delay of 0.05 seconds or 50 milliseconds.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             max_delay [in], maximum delay in number of frames.
//
// RETURN: ROKU_SFX_ERR_OK, ring buffer size has been set to new upper limit.
//         ROKU_SFX_ERR_INVALID_ARG, the requested delay was too small and
//                                   could not be set.
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmSetMaxDelay(RokuSndFx_Pcm* sfx_player,
                         int32_t max_delay);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmSetFormat
//
// DESCRIPTION: Tells the PCM sound effects player everything there is to know
//              about the PCM audio stream. This includes information like
//              sample width, sample rate, signed/unsigned, endianness and more.
//
//              This function must be called before any samples are pushed into
//              the player (see RokuSndFx_PcmWrite, RokuSndFx_PcmGetBuffer,
//              RokuSndFx_PcmPutBuffer) and before RokuSndFx_PcmStart.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             stream_prop [in], caller provided stream property structure.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmSetFormat(RokuSndFx_Pcm* sfx_player,
                       RokuSndFx_PcmFormat* pcm_format);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmGetStatus
//
// DESCRIPTION: Returns the current status of the PCM sound effects player.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             status [out], caller provided pointer to a status structure.
//                           The contents of this structure will be update to
//                           reflect the current status.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmGetStatus(RokuSndFx_Pcm* sfx_player,
                       RokuSndFx_PcmStatus* status);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmPrepare
//
// DESCRIPTION: Prepares the player for playback. This function must be
//              called before any data is fed into the player or
//              RokuSndFx_PcmStart is called.
//
//              Note, it's important to call this function before restarting
//              playback after stop has been called. RokuSndFx_PcmPrepare
//              clears any old data that may still be pending.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             timeout_ms [in], timeout value in milliseconds. Specifies
//                              the maximum amount of time to wait until the
//                              player is ready. When the timeout value is 0
//                              the timeout is ignored and
//                              RokuSndFx_PcmPrepare waits until the player
//                              is ready.
//
// RETURN VALUES:
//     ROKU_SFX_ERR_OK: The player is ready to start playback.
//     ROKU_SFX_ERR_INVALID_ARG: The player is in an invalid state.
//     ROKU_SFX_ERR_TIMEOUT: A timeout occurred.
//     ROKU_SFX_ERR_URUN: There was and underrun, caller needs to call
//                        RokuSndFx_PcmResume to resume playback.
//
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmPrepare(RokuSndFx_Pcm* sfx_player, uint32_t timeout_ms);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmWrite
//
// DESCRIPTION: Writes new data into the PCM audio buffer.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             buffer [in], buffer containing PCM audio samples that will be
//                          copied into the player's PCM audio buffer.
//             num_frames [in], number of audio frames that need to be copied.
//                              The provided buffer should contain at least
//                              this amount of frames for each channel.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmWrite(RokuSndFx_Pcm* sfx_player,
                   uint8_t const* buffer,
                   uint32_t num_frames);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmGetBuffer
//
// DESCRIPTION: Get a buffer that audio data can be written to.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             buffer [out], buffer that PCM audio samples can be written to.
//             frames_available [out], number of audio frames that can be
//                                     written to buffer.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmGetBuffer(RokuSndFx_Pcm* sfx_player,
                       uint8_t** buffer,
                       uint32_t* frames_available);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmPutBuffer
//
// DESCRIPTION: Complete write of data to buffer from RokuSndFx_PcmGetBuffer.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             frames_written [in], the number of audio frames that have been
//                                  written to the buffer.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmPutBuffer(RokuSndFx_Pcm* sfx_player,
                       uint32_t frames_written);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmStart
//
// DESCRIPTION: Starts audio playback.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             start_threshold [in], the start threshold in number of frames.
//             The player will wait until at least this amount of frames are
//             received before starting playback.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmStart(RokuSndFx_Pcm* sfx_player, uint32_t start_threshold);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmResume
//
// DESCRIPTION: Resumes audio playback after an underrun has been detected.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             resume_threshold [in], the resume threshold in number of frames.
//             The player will wait until at least this amount of frames are
//             received before resuming playback.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmResume(RokuSndFx_Pcm* sfx_player, uint32_t resume_threshold);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmStop
//
// DESCRIPTION: Stops audio playback.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             drain [in], If true the player will play out all the data it
//             has received so far and stop after that. If false the player
//             will flush all data and stop immediately.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmStop(RokuSndFx_Pcm* sfx_player,
                bool drain);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmSetVolume
//
// DESCRIPTION: Sets the sound PCM volume.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             volume_per_mil [in], Volume level between 0 and 1000. A
//             volume level of 0 effectively mutes audio, and 1000 is the
//             maximum sound level.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmSetVolume(RokuSndFx_Pcm* sfx_player, uint32_t volume_per_mil);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmWaitBufferAvailable
//
// DESCRIPTION: Blocks until the PCM player has sufficient space to store the
//              requested amount of frames in its internal buffer.
//              Once the buffer has sufficient space the call will block until
//              the delay drops low enough such that the buffer delay does not
//              exceed the value set by RokuSndFx_PcmSetMaxDelay.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             num_frames [in], number of audio frames that need to fit insides
//                              the players sample buffer before the function
//                              returns.
//             timeout_ms [in], timeout value in milliseconds. When the timeout
//                              value is 0 the timeout is ignored and
//                              RokuSndFx_PcmWaitBufferAvailable waits until
//                              num_frames are availble.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmWaitBufferAvailable(RokuSndFx_Pcm* sfx_player,
                               uint32_t num_frames,
                               uint32_t timeout_ms);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmSetBufferAvailCb
//
// DESCRIPTION: Installs a space available callback. This callback will be
//              called when the player's sample buffer has sufficient space to
//              store 'num_frames' or more samples. The callback will be called
//              once each time the available space crosses the 'num_frames'
//              threshold or after the caller has added new samples and there
//              is still sufficient space to store 'num_frames' samples.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             num_frames [in], number of audio frames that need to fit insides
//                              the players sample buffer before the callback
//                              is called.
//             callback [in], the callback function that will be called.
//             context [in], a context that is passed to the callback function
//                           as argument.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmSetBufferAvailCb(RokuSndFx_Pcm* sfx_player,
                            uint32_t num_frames,
                            RokuSndFx_PcmCb callback,
                            void* context);

//-----------------------------------------------------------------------------
// FUNCTION: RokuSndFx_PcmSetPeriodCb
//
// DESCRIPTION: Installs a period callback. This callback will be called each
//              time period has passed.
//
// PARAMETERS: sfx_player [in], a valid PCM sound effects player object.
//             callback [in], the callback function that will be called.
//             context [in], a context that is passed to the callback function
//                           as argument.
//
ROKU_SFX_DLL_EXPORTED RokuSndFx_Err
RokuSndFx_PcmSetPeriodCb(RokuSndFx_Pcm* sfx_player,
                         RokuSndFx_PcmCb callback,
                         void* context);

#ifdef __cplusplus
}
#endif

#endif // ROKU_SFX_PCM_H
