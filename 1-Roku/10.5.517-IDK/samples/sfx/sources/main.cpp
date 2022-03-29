// Copyright 2021 Roku, Inc.  Allrights reserved.
#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <roku/RokuSndFxPcm.h>
using namespace std::chrono_literals;
bool
CheckReturnValue(RokuSndFx_Err e, int line)
{
  bool ok = false;
  switch (e) {
    case ROKU_SFX_ERR_OK:
      ok = true;
      break;
    case ROKU_SFX_ERR_AGAIN:
      fprintf(stderr, "error at line %d: ROKU_SFX_ERR_AGAIN\n", line);
      break;
    case ROKU_SFX_ERR_INTERRUPTED:
      fprintf(stderr, "error at line %d: ROKU_SFX_ERR_INTERRUPTED\n", line);
      break;
    case ROKU_SFX_ERR_BUSY:
      fprintf(stderr, "error at line %d: ROKU_SFX_ERR_BUSY\n", line);
      break;
    case ROKU_SFX_ERR_INVALID_ARG:
      fprintf(stderr, "error at line %d: ROKU_SFX_ERR_INVALID_ARG\n", line);
      break;
    case ROKU_SFX_ERR_TIMEOUT:
      fprintf(stderr, "error at line %d: ROKU_SFX_ERR_TIMEOUT\n", line);
      break;
    case ROKU_SFX_ERR_URUN:
      fprintf(stderr, "error at line %d: ROKU_SFX_ERR_URUN\n", line);
      break;
    case ROKU_SFX_ERR_INTERNAL:
      fprintf(stderr, "error at line %d: ROKU_SFX_ERR_INTERNAL\n", line);
      break;
  }
  return ok;
}

// This sample channel of course is working.  But when you are working on your
// own application this macro can help you track down location of failed call.
#define Check(e) CheckReturnValue(e, __LINE__)

int
main(int, char**)
{
  mp3dec_t decoderMP3;
  mp3dec_file_info_t infoMP3;
  if (mp3dec_load(
        &decoderMP3, "/pkg:/source/gnog.mp3", &infoMP3, nullptr, nullptr)) {
    fprintf(stderr, "unable to decode MP3\n");
    return -666;
  }

  // We have decoded our 60-second MP3 into PCM samples.  Now we'll create a
  // player to render audio output.
  RokuSndFx_Pcm* pPlayer = nullptr;
  auto ok = RokuSndFx_PcmCreate(&pPlayer);
  Check(ok);

  unsigned int rateSample = infoMP3.hz;
  unsigned int bufferFrames = 2 * rateSample / 100;
  fprintf(stderr,
          "samples: %zu ch: %d hz: %d kbps: %d delay: %d\n",
          infoMP3.samples,
          infoMP3.channels,
          infoMP3.hz,
          infoMP3.avg_bitrate_kbps,
          bufferFrames);

  ok = RokuSndFx_PcmSetMaxDelay(pPlayer, bufferFrames);
  Check(ok);

  RokuSndFx_PcmFormat formatPCM = { .endian = ROKU_SFX_ENDIAN_LIT,
                                    .sign = 1,
                                    .width = 16,
                                    .num_ch = uint32_t(infoMP3.channels),
                                    .rate = rateSample };
  ok = RokuSndFx_PcmSetFormat(pPlayer, &formatPCM);
  Check(ok);

  ok = RokuSndFx_PcmPrepare(pPlayer, 1000);
  Check(ok);

  ok = RokuSndFx_PcmStart(pPlayer, bufferFrames / 2);
  Check(ok);

  // Write every 10ms.  We want to write often enough so that we don't need to
  // allocate a big ringbuffer to hold PCM samples.  We also don't want to
  // write so often, because that generates too many interrupts.
  unsigned int frameToWrite = rateSample / 100;
  unsigned int frameTotal = infoMP3.samples / sizeof(mp3d_sample_t);
  unsigned int frameWritten = 0;
  auto timeLastReport = std::chrono::steady_clock::now();

  while (frameWritten < frameTotal) {
    const auto now = std::chrono::steady_clock::now();
    if ((now - timeLastReport) > 5s) { // print statistics every 5 seconds.
      timeLastReport = now;
      RokuSndFx_PcmStatus status;
      ok = RokuSndFx_PcmGetStatus(pPlayer, &status);
      if (Check(ok)) {
        fprintf(
          stderr,
          "processed: %u, avail: %u avail_max: %u delay: %u pp_delay %u\n",
          status.processed,
          status.avail,
          status.avail_max,
          status.delay,
          status.pp_delay);
      }
    }

    unsigned int frames =
      std::min<int>(frameToWrite, frameTotal - frameWritten);
    ok = RokuSndFx_PcmWaitBufferAvailable(pPlayer, frames, 0);
    if (ok == ROKU_SFX_ERR_URUN) {
      // Buffer underrun...
      ok = RokuSndFx_PcmResume(pPlayer, frames);
      if (!Check(ok)) {
        break; // There is no hope.  Break out of loop to clean up.
      }
    }

    ok = RokuSndFx_PcmWrite(
      pPlayer,
      (uint8_t*)&infoMP3.buffer[frameWritten * infoMP3.channels],
      frames);
    Check(ok);
    frameWritten += frames;
  }

  // Job done!  Clean up liberate memory before exiting.
  ok = RokuSndFx_PcmStop(pPlayer, true);
  Check(ok);

  ok = RokuSndFx_PcmDestroy(pPlayer);
  Check(ok);

  free(infoMP3.buffer);
  return 0;
}
