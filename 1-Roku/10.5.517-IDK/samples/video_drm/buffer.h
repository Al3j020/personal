// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_STREAM_BUFFER
#define IDK_SAMPLE_STREAM_BUFFER
#include <memory>
#include <roku/RokuAv2Decoder.h>
#include <roku/RokuDrmEngine.h> // DRM addition
struct StreamBuffer
{
  using Ref = std::shared_ptr<StreamBuffer>;
  static Ref CreateFromFile(RokuAv2_CodecAudio, const char*);
  static Ref CreateFromFile(RokuAv2_CodecVideo, const char*);

  // DRM addition: passing in DRM engine and session
  virtual void startBuffering(RokuAv2_Decoder*, RokuDrm_Eng*, RokuDrm_Ses*) = 0;
  virtual void exit() = 0;

  bool IsAudio() const { return codecVideo == ROKU_AV2_CV_NULL; }
  bool IsVideo() const { return codecAudio == ROKU_AV2_CA_NULL; }

  const RokuAv2_CodecAudio codecAudio;
  const RokuAv2_CodecVideo codecVideo;

  StreamBuffer(RokuAv2_CodecAudio);
  StreamBuffer(RokuAv2_CodecVideo);
};
using StreamBufferRef = StreamBuffer::Ref;
#endif //#ifndef IDK_SAMPLE_STREAM_BUFFER
