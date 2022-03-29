// Copyright 2021 Roku, Inc.  All rights reserved.
#include "buffer.h"
#include "bento4.h"
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
using namespace std::chrono_literals;

extern "C" RokuAv2_CryptoSession
RokuDrm_SesPrivGetHandle(RokuDrm_Ses*);

std::string
PrintBytes(std::vector<uint8_t> buf);

namespace {
struct FileBuffer : StreamBuffer
{
  FileBuffer(RokuAv2_CodecAudio, const char*);
  FileBuffer(RokuAv2_CodecVideo, const char*);
  virtual ~FileBuffer();

  void startBuffering(RokuAv2_Decoder*, RokuDrm_Eng*, RokuDrm_Ses*) override;
  void exit() override;

private:
  std::thread threadParse;
  std::atomic<bool> bParsing;
  const std::string strURL;
};

FileBuffer::FileBuffer(RokuAv2_CodecAudio codec, const char* path)
  : StreamBuffer(codec)
  , strURL(path)
{}

FileBuffer::FileBuffer(RokuAv2_CodecVideo codec, const char* path)
  : StreamBuffer(codec)
  , strURL(path)
{}

FileBuffer::~FileBuffer()
{
  exit();
}

void
FileBuffer::exit()
{
  bParsing = false;
  if (threadParse.joinable()) {
    threadParse.join();
  }
}

void
FileBuffer::startBuffering(RokuAv2_Decoder* pDecoder,
                           RokuDrm_Eng* pDRM,
                           RokuDrm_Ses* pDRMSession)
{
  if (!pDecoder) {
    fprintf(stderr, "no decoder to write to for: %s\n", strURL.c_str());
    return;
  }
  if (!pDRM || !pDRMSession) { // DRM addition
    fprintf(stderr, "missing DRM engine or session\n");
    return;
  }

  exit(); // Join existing thread if exists; in case of playing stream again.

  threadParse = std::thread([=] {
    bParsing = true;
    fprintf(stderr, "opening %s\n", strURL.c_str());

    if (auto parser = std::make_shared<MP4Parser>(strURL.c_str(), this)) {
      std::vector<uint8_t> iv;
      RokuAv2_Time ptsLast = 0;

      // DRM additions, this data structure describe which part of the
      // elementary stream is encrypted, since it is likely for part of video
      // sample of be clear.  Part of this data structure is filled out by
      // MP4Parser::getNextSample().  Here we fill out information not
      // available from bitstream and supply default values.
      RokuAv2_DrmInfo drm_info;
      RokuAv2_Uint8Range dummy = { nullptr, nullptr };
      drm_info.size = sizeof(drm_info);
      drm_info.crypto = RokuDrm_SesGetHandle(pDRMSession);
      drm_info.crypto_ranges = { 0, &dummy };

      // Patterns are defined in ISO 23001-7:2016 specification, but not
      // applicable to the streams prepared for this sample channel.
      drm_info.enc_pattern = { nullptr, nullptr };

      while (bParsing) {
        AP4_Sample packet;
        AP4_DataBuffer db;
        RokuAv2_Uint8Range* to_free = nullptr;

        if (parser->getNextSample(packet, db, drm_info, this, to_free)) {
          auto p = (uint8_t*)db.GetData();
          ptsLast = parser->convertPTS(packet.GetCts());

          RokuAv2_Sample sample;
          sample.size = db.GetDataSize();
          sample.es =
            IsVideo() ? ROKU_AV2_ES_TYPE_VIDEO : ROKU_AV2_ES_TYPE_AUDIO;
          sample.pts = ptsLast;

          RokuAv2_Uint8Range es[1] = { { p, p + db.GetDataSize() } };
          sample.data_ranges.num = 1;
          sample.data_ranges.data = es;
          sample.drm = &drm_info; // DRM addition.
#if 0 // Set to 1 to see first 8 bytes of each sample and CTS translation.
          fprintf(stderr,
                  "sending %s sample %02x%02x%02x%02x %02x%02x%02x%02x "
                  "%d %d %" PRId64 " %" PRId64 "\n",
                  IsVideo() ? "video" : "audio",
                  p[0],
                  p[1],
                  p[2],
                  p[3],
                  p[4],
                  p[5],
                  p[6],
                  p[7],
                  drm_info.crypto_ranges.num,
                  sample.size,
                  packet.GetCts(),
                  ptsLast);
#endif

          // This API will block if there is no space in FIFO to write sample.
          RokuAv2_DecoderWriteSample2(pDecoder, &sample);

          // Liberate data structure used by drm_info.crypto_ranges.
          if (to_free) {
            delete[] to_free;
          }
        } else {
          // End of stream.
          bParsing = false;
          RokuAv2_Sample sample;
          sample.size = 0;
          sample.es =
            IsVideo() ? ROKU_AV2_ES_TYPE_VIDEO_EOS : ROKU_AV2_ES_TYPE_AUDIO_EOS;
          sample.pts = 0;
          sample.data_ranges.num = 0;
          sample.data_ranges.data = nullptr;
          sample.drm = nullptr;

          // Inform decoder that no more samples will be coming.
          RokuAv2_DecoderWriteSample2(pDecoder, &sample);
        }
      } // while (bParsing)
    }   // if (auto parser = std::make_shared<MP4Parser>(strURL.c_str(), this))
  });
}
} // anonymous namespace

StreamBufferRef
StreamBuffer::CreateFromFile(RokuAv2_CodecAudio codec, const char* path)
{
  return std::make_shared<FileBuffer>(codec, path);
}

StreamBufferRef
StreamBuffer::CreateFromFile(RokuAv2_CodecVideo codec, const char* path)
{
  return std::make_shared<FileBuffer>(codec, path);
}

StreamBuffer::StreamBuffer(RokuAv2_CodecAudio codec)
  : codecAudio(codec)
  , codecVideo(ROKU_AV2_CV_NULL)
{}

StreamBuffer::StreamBuffer(RokuAv2_CodecVideo codec)
  : codecAudio(ROKU_AV2_CA_NULL)
  , codecVideo(codec)
{}
