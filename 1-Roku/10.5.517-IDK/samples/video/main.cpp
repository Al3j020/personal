// Copyright 2021 Roku, Inc.  All rights reserved.
#include "buffer.h"
#include "screen.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <roku/RokuAv2.h>
#include <roku/RokuAv2VideoSink.h>
#include <string>
#include <thread>
using namespace std::chrono_literals;

std::atomic<bool> bRunning = { true };
void
handler(int signal, siginfo_t*, void*)
{
  switch (signal) {
    case SIGTERM:
      fprintf(stderr, "received SIGTERM\n");
      bRunning = false; // User hit HOME button on remote.
      break;
    default:
      break;
  }
}

std::atomic<bool> bEndOfStream = { false };
void
VideoEnded(void*, const RokuAv2_DecoderPlaybackEndedParams*)
{
  bEndOfStream = true; // Received EOS, note event so we can loop video.
}

void
RunStreamLoop(char** argv)
{
  // Initially video plane is blocked by opaque black graphics plane.  This
  // call will clear graphics (see screen.cpp) so we can see video.
  MakeGraphicsTransparentToShowVideo();

  // Set the desired Roku AV2 API version.
  RokuAv2_SetApiVersion(ROKU_AV2_THIS_API_VERSION);

  // Creating decoders.  I already know what codecs were used to create the
  // elementary streams I am playing back.
  const RokuAv2_DecoderId decoderID = 1;

  RokuAv2_Decoder* decoderAudio;
  RokuAv2_DecoderCreateParams paramsAudio;
  paramsAudio.size = sizeof(paramsAudio);
  paramsAudio.decoder_id = decoderID;
  paramsAudio.es = ROKU_AV2_ES_TYPE_AUDIO;
  RokuAv2_DecoderCreate2(&decoderAudio, &paramsAudio);

  RokuAv2_DecoderCreateParams paramsVideo;
  RokuAv2_Decoder* decoderVideo;
  paramsVideo.size = sizeof(paramsVideo);
  paramsVideo.decoder_id = decoderID;
  paramsVideo.es = ROKU_AV2_ES_TYPE_VIDEO;
  RokuAv2_DecoderCreate2(&decoderVideo, &paramsVideo);

  // Notify EOS.
  RokuAv2_DecoderSetPlaybackEndedListener2(decoderVideo, VideoEnded, nullptr);

  // Set up decoders.
  RokuAv2_AudioFormat formatAudio;
  formatAudio.size = sizeof(formatAudio);
  formatAudio.codec = ROKU_AV2_CA_AAC_ADTS;
  formatAudio.flags = 0;       // Used by
  formatAudio.sample_size = 0; // PCM
  formatAudio.channels = 0;    // playback.

  RokuAv2_EsFormat esAudio;
  esAudio.size = sizeof(esAudio);
  esAudio.es = ROKU_AV2_ES_TYPE_AUDIO;
  esAudio.format.a = &formatAudio;
  RokuAv2_DecoderConfigure2(decoderAudio, &esAudio);

  RokuAv2_VideoFormat formatVideo;
  formatVideo.size = sizeof(formatVideo);
  formatVideo.codec = ROKU_AV2_CV_MPEG4_10B;
  formatVideo.dvmode = ROKU_AV2_DV_OFF;
  formatVideo.flags = 0;        // These parameters
  formatVideo.frame_rate = 0;   // are detected
  formatVideo.par = { 0, 0 };   // automatically
  formatVideo.image = { 0, 0 }; // from bitstream.

  RokuAv2_EsFormat esVideo;
  esVideo.size = sizeof(esVideo);
  esVideo.es = ROKU_AV2_ES_TYPE_VIDEO;
  esVideo.format.v = &formatVideo;
  RokuAv2_DecoderConfigure2(decoderVideo, &esVideo);

  // Control location of video output; don't need to keep video sink pointer
  // around once we are done.
  RokuAv2_VSink* videoSink;
  RokuAv2_VSinkQuery(&videoSink, decoderID);

  RokuAv2_NormRect windowDst;
  windowDst.x = 0.0f; // These are normalized coordinates.
  windowDst.y = 0.0f; // For example, setting all fields
  windowDst.w = 1.0f; // to 0.5 will show video in lower-right
  windowDst.h = 1.0f; // corner, 50% scaled in each dimension.

  RokuAv2_NormRect windowSrc;
  windowSrc.x = 0.0f;
  windowSrc.y = 0.0f;
  windowSrc.w = 1.0f;
  windowSrc.h = 1.0f;

  RokuAv2_VideoScale scaleVideo;
  scaleVideo.size = sizeof(scaleVideo);
  scaleVideo.mode = ROKU_AV2_PRESERVE_ASPECT;
  scaleVideo.src = windowSrc;
  scaleVideo.dst = windowDst;
  RokuAv2_VSinkSetScale2(videoSink, &scaleVideo);
  RokuAv2_VSinkRelease(videoSink);

  // StreamBuffer will mmap() the bitstream, demux and write samples to the
  // decoders I specify.  Create separate audio and video elementary streams.
  auto video = StreamBuffer::CreateFromFile(formatVideo.codec, argv[1]);
  auto audio = StreamBuffer::CreateFromFile(formatAudio.codec, argv[2]);

  // Start decoding samples.
  RokuAv2_DecoderSetSpeed(decoderVideo, 1.0f);
  RokuAv2_DecoderSetSpeed(decoderAudio, 1.0f);

  // Send samples to decoderVideo and decoderAudio.  See buffer.cpp for more
  // implementation details.
  video->startBuffering(decoderVideo);
  audio->startBuffering(decoderAudio);

  // Loop video until user hit HOME button.
  while (bRunning) {
    if (bEndOfStream) { // Reached end of stream...
      bEndOfStream = false;

      // Stop threads feeding ES to decoders.
      video->exit();
      audio->exit();

      // Stop decoders.
      RokuAv2_DecoderStop(decoderAudio);
      RokuAv2_DecoderStop(decoderVideo);
      RokuAv2_DecoderFlush(decoderAudio);
      RokuAv2_DecoderFlush(decoderVideo);

      // Show decoder statistics.
      RokuAv2_DecoderStats s;
      RokuAv2_DecoderGetDecoderStats(decoderVideo, &s);
      fprintf(stderr,
              "\nrestarting stream; statistics: render %d repeat %d drop %d "
              "error %d\n",
              s.frame_render_cnt,
              s.frame_repeat_cnt,
              s.frame_repeat_cnt,
              s.stream_err_cnt);
      fflush(stderr);

      // Restart player and threads feeding ES.
      RokuAv2_DecoderSetSpeed(decoderVideo, 1.0f);
      RokuAv2_DecoderSetSpeed(decoderAudio, 1.0f);
      video->startBuffering(decoderVideo);
      audio->startBuffering(decoderAudio);
    } else {
      std::this_thread::sleep_for(1s);
    }
  }

  // Clean up, we are exiting sample channel.
  RokuAv2_DecoderStop(decoderAudio);
  RokuAv2_DecoderStop(decoderVideo);
  RokuAv2_DecoderFlush(decoderAudio);
  RokuAv2_DecoderFlush(decoderVideo);
  RokuAv2_DecoderDestroy(decoderAudio);
  RokuAv2_DecoderDestroy(decoderVideo);
}

int
main(int argc, char** argv)
{
  if (argc < 3) {
    fprintf(stderr, "usage: %s video-es-file audio-es-file\n", argv[0]);
    fprintf(stderr, "   ex: %s video.mp4 audio.ts\n", argv[0]);
  } else {
    // Setup signal handler
    struct sigaction old_handler;
    struct sigaction new_handler;
    memset(&new_handler, 0, sizeof(new_handler));
    new_handler.sa_sigaction = handler;
    sigemptyset(&new_handler.sa_mask);
    new_handler.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGTERM, &new_handler, &old_handler);

    // Play and loop video.
    RunStreamLoop(argv);

    sigaction(SIGTERM, &old_handler, nullptr);
    fprintf(stderr, "exiting...\n");
    fflush(stderr);
  }
  return 0;
}
