// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_STREAM_BUFFER
#define IDK_SAMPLE_STREAM_BUFFER
#include <memory>
#include <roku/RokuAv2Decoder.h>
struct StreamBuffer
{
  using Ref = std::shared_ptr<StreamBuffer>;
  static Ref CreateFromFile(RokuAv2_CodecAudio, const char*);
  static Ref CreateFromFile(RokuAv2_CodecVideo, const char*);

  virtual void startBuffering(RokuAv2_Decoder*) = 0;
  virtual void exit() = 0;

  const RokuAv2_CodecAudio codecAudio;
  const RokuAv2_CodecVideo codecVideo;

  StreamBuffer(RokuAv2_CodecAudio);
  StreamBuffer(RokuAv2_CodecVideo);
};
using StreamBufferRef = StreamBuffer::Ref;
#endif //#ifndef IDK_SAMPLE_STREAM_BUFFER
