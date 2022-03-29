// Copyright 2021 Roku, Inc.  All rights reserved.
#include "buffer.h"
extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/timestamp.h>
}
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
using namespace std::chrono_literals;
namespace {
struct FileBuffer : StreamBuffer
{
  FileBuffer(RokuAv2_CodecAudio, const char*);
  FileBuffer(RokuAv2_CodecVideo, const char*);
  virtual ~FileBuffer();

  void startBuffering(RokuAv2_Decoder*) override;
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
FileBuffer::startBuffering(RokuAv2_Decoder* pDecoder)
{
  if (!pDecoder) {
    fprintf(stderr, "no decoder to write to for: %s\n", strURL.c_str());
    return;
  }

  exit(); // Join existing thread if exists; in case of playing stream again.

  threadParse = std::thread([=] {
    bParsing = true;
    AVFormatContext* pFormat = nullptr;
    fprintf(stderr, "opening %s\n", strURL.c_str());

    // Using libavformat to parse stream so that each AVPacket will contain an
    // an ES sample and PTS.  Then use hardware to decode the samples.
    int r = avformat_open_input(&pFormat, strURL.c_str(), nullptr, nullptr);
    if (r >= 0) {
      int64_t ptsLast = 0;
      while (bParsing) {
        AVPacket packet;
        if (av_read_frame(pFormat, &packet) == 0) {
          if (packet.pts != AV_NOPTS_VALUE) { // PTS is valid
            ptsLast = av_q2d(pFormat->streams[packet.stream_index]->time_base) *
                      packet.pts * 1000000;
          }

          RokuAv2_Sample sample;
          sample.size = packet.size;
          sample.es = codecAudio == ROKU_AV2_CA_NULL ? ROKU_AV2_ES_TYPE_VIDEO
                                                     : ROKU_AV2_ES_TYPE_AUDIO;
          sample.pts = ptsLast;
          RokuAv2_Uint8Range es[1] = { { packet.data,
                                         packet.data + packet.size } };
          sample.data_ranges.num = 1;
          sample.data_ranges.data = es;
          sample.drm = nullptr;

          // This API will block if there is no space in FIFO to write sample.
          RokuAv2_DecoderWriteSample2(pDecoder, &sample);
        } else {
          // Error or end of file
          bParsing = false;
          RokuAv2_Sample sample;
          sample.size = 0;
          sample.es = codecAudio == ROKU_AV2_CA_NULL
                        ? ROKU_AV2_ES_TYPE_VIDEO_EOS
                        : ROKU_AV2_ES_TYPE_AUDIO_EOS;
          sample.pts = 0;
          sample.data_ranges.num = 0;
          sample.data_ranges.data = nullptr;
          sample.drm = nullptr;

          // Inform decoder that no more samples will be coming.
          RokuAv2_DecoderWriteSample2(pDecoder, &sample);
        }
        av_packet_unref(&packet);
      }
    } else {
      char error[AV_ERROR_MAX_STRING_SIZE];
      av_make_error_string(error, AV_ERROR_MAX_STRING_SIZE, r);
      fprintf(stderr, "unable to open %s: %s\n", strURL.c_str(), error);
    }
    avformat_close_input(&pFormat);
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
