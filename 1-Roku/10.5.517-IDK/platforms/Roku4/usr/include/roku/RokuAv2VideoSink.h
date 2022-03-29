#ifndef ROKU_AV2_VIDEO_SINK_H
#define ROKU_AV2_VIDEO_SINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RokuAv2Types.h"

//----------------------------------------------------------------------------
// DATA TYPES
//----------------------------------------------------------------------------

typedef struct RokuAv2_VSinkHandle RokuAv2_VSink;

typedef enum {
    ROKU_AV2_PRESERVE_ASPECT,
    ROKU_AV2_IGNORE_ASPECT
} RokuAv2_VideoScaleMode;

typedef struct {
    float x, y, w, h;
} RokuAv2_NormRect;

typedef struct {
    uint32_t               size;   // Struct size
    RokuAv2_VideoScaleMode mode;
    RokuAv2_NormRect       src;    // normalized source rect
    RokuAv2_NormRect       dst;    // normalized destination rect
} RokuAv2_VideoScale;

typedef void (*RokuAv2_VSinkOutputFormatCb) (
    void* context, VideoOutputFormat const* format);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkQuery:
//
// DESCRIPTION: Queries video sink handle associated with given decoder.
//
//              The reference count of the audio sink object is incremented
//              each time this function is called.
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink [out]: Opaque pointer to video sink object, if this call succeeds
//               sink points to RokuAvVideoSink object.
//   decoder_id [in]: Decoder connected to video sink.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, video sink object created.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkQuery(
    RokuAv2_VSink **sink,
    RokuAv2_DecoderId decoder_id
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkRelease:
//
// DESCRIPTION: Releases video sink object and frees all allocated resources.
//              Note, the audio sink object is ref counted, resources are only
//              freed after all references have been released.
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to video sink object.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, video sink object released.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkRelease(
    RokuAv2_VSink *sink
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkSetScale2:
//
// DESCRIPTION: Sets video output scaling, cropping and positioning. By
//              default, video us rendered full screen.
//
//              Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to video sink object.
//   dst [in]: Destination video rectangle in normalized coordinates. Values
//             range is [0..1] where (0, 0) is top left corner of the screen
//             and (1, 1) is bottom right conner. Rectangle {0; 0; 1; 1}
//             exactly matches the whole screen.
//   src [in]: Video cropping rectangle in normalized coordinates with value
//             range [0..1]. (0, 0) is the top left corner of the video frame,
//             (1, 1) is bottom right one. Rectangle {0; 0; 1; 1} represents
//             the whole video frame.
//   mode [in]: Video scaling mode - preserving aspect ratio or not.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, video output configured.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkSetScale2(
    RokuAv2_VSink* sink,
    RokuAv2_VideoScale const* scale_params
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkSetRefreshRate
//
// DESCRIPTION: Sets video output refresh rate if the resolution switcher is
//   active. Otherwise, it just sets the rate which will be set to when the
//   resolution switcher becomes active.
//
//   Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to the video sink object
//   rate [in]: The desired vertical refresh rate in Hz.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded.
//   ROKUAV2_ERR_PENDING - Refresh rate change deferred until
//     RokuAv2_VSinkEnableRefreshRateChange(sink, true) is called;
//   ROKUAV2_ERR_NOT_SUPPORTED: Refresh rate cannot be changed, probably due
//     to "Auto-adjust display refresh rate" set to "off".
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//   ROKUAV2_ERR_INTERNAL: internal error.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkSetRefreshRate(
    RokuAv2_VSink *sink,
    float rate
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkEnableRefreshRateChange
//
// DESCRIPTION: Enables or disables the resolution switcher
//   If refresh rate changing is enabled then the HDMI output verticalrefresh
//   rate will automatically be changed as much as possible to the input frame
//   rate. It also enables the RokuAv2_VSinkSetRefreshRate API to explicitly
//   change the frame rate. Refresh-rate changing can only be enabled if the
//   "Auto-adjust display refresh rate" user preference is turned on.
//   Otherwise, this function will return ROKUAV2_ERR_NOT_SUPPORTED.
//   This function allows refresh rate chaning to be explicitly enabled or
//   disabled regardless of the number of instances of the video sink. The
//   refresh rate changing is disabled by default but it can be changed using
//   this function by any instance and that state remains active untill all
//   instances are destroyed. When refresh rate changing is disabled, the
//   refresh rate is set back what it was before refresh rate changing was
//   enabled.
//
//   Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to the video sink object
//   enabled [in]: True to enable, false to disable
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded, video refresh rate set to the last call
//     to RokuAv2_VSinkSetRefreshRate if necessary.
//   ROKUAV2_ERR_NOT_SUPPORTED: Resolution switcher cannot be enabled, probably
//     due to "Auto-adjust display refresh rate" set to "off".
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//   ROKUAV2_ERR_INTERNAL: Could not create the resolution switcher object.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkEnableRefreshRateChange(
    RokuAv2_VSink *sink,
    bool enable
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkIsRefreshRateChangeEnabled2
//
// DESCRIPTION: returns whether or not the resolution switcher is enabled or
//   can be enabled
//
//   Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to the video sink object
//
// RETURN VALUES:
//   ROKUAV2_OK: Resolution switcher is enabled
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//   ROKUAV2_ERR_NOT_SUPPORTED: Resolution switcher cannot be enabled
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkIsRefreshRateChangeEnabled2(
    RokuAv2_VSink* sink,
    bool* enabled
);

//----------------------------------------------------------------------------
// FUNCTION RokuAvVideoSink_HideVideo
//
// DESCRIPTION: Hides or un-hides the video. If video is hidden, black pixels
//   will be rendered where the graphics layer is transparant.
//
//   Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to the video sink object
//   bool [in]: true if the video plane needs to be hidden, false otherwise.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkHideVideo(
    RokuAv2_VSink *sink,
    bool hide
);

//----------------------------------------------------------------------------
// FUNCTION RokuAvVideoSink_SetColorMatrix
//
// DESCRIPTION: Sets color matrix.
//
//   Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to the video sink object
//   color [in]: Color matrix structure.
//
// RETURN VALUES:
//   None.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkSetColorMatrix(
    RokuAv2_VSink* sink,
    RokuAv2_ColorMatrix const* color
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkDisableHDRFormatOverlay
//
// DESCRIPTION: Disables/enables rendering of the HDR logo. Channels may
//   temporary disable the logo if it interferes with other UI elements.
//   The logo is enabled by default.
//
//   Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to the video sink object
//   bool [in]: true if the video plane needs to be hidden, false otherwise.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkDisableHDRFormatOverlay(
    RokuAv2_VSink *sink,
    bool disable
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkGetOutputFormat
//
// DESCRIPTION: Returns properties of the video output format for video
//              source devices. This function does not apply to TVs since
//              they do not output a video signal.
//
//   Available since API version 2.
//
// PARAMETERS:
//   sink [in]: Handle to the video sink object
//   output_format [out]: Pointer to a VideoOutputFormat object that will
//                        contain information about the current video output
//                        format upon return.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//   ROKUAV2_ERR_NOT_SUPORTED:   This call is not supported on this platform.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkGetOutputFormat(
    RokuAv2_VSink *sink,
    VideoOutputFormat *output_format
);

//----------------------------------------------------------------------------
// FUNCTION RokuAv2_VSinkSetOutputFormatListener
//
// DESCRIPTION: Configures a callback so that the framework may inform
//              a listener that the configuration of the video sink
//              has changed, due to either framework or external action.
//              This may include changes to: color format, frame rate,
//              color depth, quantization, colorimetry, or
//              electro-optical transfer function (EOTF).
//
// PARAMETERS:
//   sink [in]: Handle to the video sink object
//   callback [in]: Pointer to a function that the framework will call
//                  when the configuration of the video output sink changes.
//   user_data [in]: Pointer to user data for this call. Currently unused.
//
// RETURN VALUES:
//   ROKUAV2_OK: Call succeeded.
//   ROKUAV2_ERR_INVALID_HANDLE: Provided sink handle is invalid.
//
//----------------------------------------------------------------------------
//
RokuAv2_Err RokuAv2_VSinkSetOutputFormatListener(
    RokuAv2_VSink *sink,
    RokuAv2_VSinkOutputFormatCb callback,
    void* user_data
);

#ifdef __cplusplus
}
#endif

#endif // ROKU_AV2_VIDEO_SINK_H
