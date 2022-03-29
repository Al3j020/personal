// Copyright 2021 Roku, Inc.  All rights reserved.
#include "bento4.h"
#include "buffer.h"
#include <string>

std::string
PrintBytes(std::vector<uint8_t> buf)
{
  char hex[3];
  std::stringstream ss;
  for (auto&& byte : buf) {
    snprintf(hex, sizeof(hex), "%02x", byte);
    ss << hex;
  }
  return ss.str();
}

MP4Parser::MP4Parser(std::string path, StreamBuffer* buffer)
  : input(nullptr)
  , movie(nullptr)
  , track(nullptr)
  , tableSampleInfos(nullptr)
  , tableSamples(nullptr)
{
  if (AP4_FileByteStream::Create(path.c_str(),
                                 AP4_FileByteStream::STREAM_MODE_READ,
                                 input) == AP4_SUCCESS) {
    AP4_File file(*input);
    movie = file.GetMovie();
    track = movie->GetTrack(buffer->IsVideo() ? AP4_Track::TYPE_VIDEO
                                              : AP4_Track::TYPE_AUDIO);
    auto description = track->GetSampleDescription(0);

    AP4_ProtectedSampleDescription* protected_description = nullptr;
    if (protected_description =
          AP4_DYNAMIC_CAST(AP4_ProtectedSampleDescription, description)) {
      AP4_UI32 st = protected_description->GetSchemeType();
      char coding[5];
      AP4_FormatFourChars(coding, protected_description->GetFormat());
      fprintf(stderr,
              "coding: %s scheme: %c%c%c%c\n",
              coding,
              (char)((st >> 24) & 0xff),
              (char)((st >> 16) & 0xff),
              (char)((st >> 8) & 0xff),
              (char)(st & 0xff));

      if (auto scheme = protected_description->GetSchemeInfo()) {
        if (auto schi = scheme->GetSchiAtom()) {
          if (auto tenc =
                AP4_DYNAMIC_CAST(AP4_TencAtom, schi->FindChild("tenc"))) {
            auto p = tenc->GetDefaultKid();
            kid.insert(kid.end(), p, p + 16);
            fprintf(stderr,
                    "%s KID: %s\n",
                    buffer->IsVideo() ? "video" : "audio",
                    PrintBytes(kid).c_str());
          }
        }
      }

      // Take description from TYPE_PROTECTED to TYPE_MPEG (audio) or
      // TYPE_AVC (video).
      description = protected_description->GetOriginalSampleDescription();
    }

    AP4_String codec;
    description->GetCodecString(codec);
    fprintf(stderr,
            "%s codec: %s %s\n",
            buffer->IsVideo() ? "video" : "audio",
            codec.GetChars(),
            movie->HasFragments() ? "fragments" : "");

    switch (description->GetType()) {
      case AP4_SampleDescription::TYPE_MPEG:
        if (auto m4a =
              AP4_DYNAMIC_CAST(AP4_MpegAudioSampleDescription, description)) {
          auto dsi = m4a->GetDecoderInfo();
          AP4_Mp4AudioDecoderConfig config;
          config.Parse(dsi.GetData(), dsi.GetDataSize());
          idxSampling = config.m_SamplingFrequencyIndex;
          chAudio = config.m_ChannelCount;
          fprintf(stderr,
                  "channels: %d sample rate: %d %d\n",
                  chAudio,
                  config.m_SamplingFrequency,
                  idxSampling);

          ratio = 1000000.0f / config.m_SamplingFrequency;
        }
        break;
      case AP4_SampleDescription::TYPE_AVC:
        if (auto avc_description =
              AP4_DYNAMIC_CAST(AP4_AvcSampleDescription, description)) {
          zNALU = avc_description->GetNaluLengthSize();

          auto sps_db = avc_description->GetSequenceParameters()[0];
          auto p = sps_db.GetData();
          sps.insert(sps.end(), p, p + sps_db.GetDataSize());
          fprintf(stderr, "SPS: %s\n", PrintBytes(sps).c_str());

          auto pps_db = avc_description->GetPictureParameters()[0];
          p = pps_db.GetData();
          pps.insert(pps.end(), p, p + pps_db.GetDataSize());
          fprintf(stderr, "PPS: %s\n", PrintBytes(pps).c_str());

          auto time_scale = track->GetMediaTimeScale();
          fprintf(stderr, "zNALU: %d fps: %d\n", zNALU, time_scale);
          ratio = 1000000.0f / time_scale;
        }
        break;
      default:
        // Not handling other types because I know the types of streams
        // I prepared for this sample channel.
        break;
    }

    // Searching for table of samples moof -> traf -> trun
    if (movie->HasFragments()) {
      input->Seek(0);
      AP4_DefaultAtomFactory factory;
      // while (searching && tableSamples == nullptr) {
      AP4_Position pos = 0;
      AP4_LargeSize fsize = 0;
      input->GetSize(fsize);
      while (pos < fsize) {
        AP4_Atom* atom = nullptr;
        auto ok = factory.CreateAtomFromStream(*input, atom);
        if (ok == AP4_SUCCESS) {
          input->Tell(pos);
          if (atom->GetType() == AP4_ATOM_TYPE_MOOF) {
            auto moof_offset = pos - atom->GetSize();
            auto mdat_payload_offset = pos + 8;
            if (auto moof = AP4_DYNAMIC_CAST(AP4_ContainerAtom, atom)) {
              if (auto fragment = std::make_shared<AP4_MovieFragment>(moof)) {
                AP4_Array<AP4_UI32> ids;
                fragment->GetTrackIds(ids);

                AP4_ContainerAtom* traf = nullptr;
                for (AP4_Cardinal i = 0; i < ids.ItemCount(); ++i) {
                  if (fragment->GetTrafAtom(ids[i], traf) == AP4_SUCCESS) {
                    if (fragment->CreateSampleTable(movie,
                                                    ids[i],
                                                    input,
                                                    moof_offset,
                                                    mdat_payload_offset,
                                                    0,
                                                    tableSamples) ==
                        AP4_SUCCESS) {
                      fprintf(stderr,
                              "got sample tables for %s with %d samples\n",
                              buffer->IsVideo() ? "video" : "audio",
                              tableSamples->GetSampleCount());
                      indexSample = 0;
                    }
                  }
                }

                AP4_SaioAtom* saio;
                AP4_SaizAtom* saiz;
                AP4_CencSampleEncryption* senc;
                AP4_UI32 cipher_type = 0;
                bool reset_iv_for_each_subsample = false;
                if (AP4_CencSampleInfoTable::Create(protected_description,
                                                    traf,
                                                    saio,
                                                    saiz,
                                                    senc,
                                                    cipher_type,
                                                    reset_iv_for_each_subsample,
                                                    *input,
                                                    moof_offset,
                                                    tableSampleInfos) ==
                    AP4_SUCCESS) {
                  fprintf(stderr,
                          "got crypto ranges for %d %s samples\n",
                          tableSampleInfos->GetSampleCount(),
                          buffer->IsVideo() ? "video" : "audio");
                }

                if (tableSamples && tableSampleInfos) {
                  vecTableSampleInfos.push_back(tableSampleInfos);
                  vecTableSamples.push_back(tableSamples);
                }
              }
            }
          } else {
            auto t = atom->GetType();
            fprintf(stderr,
                    "skipping %c%c%c%c @%llu\n",
                    (char)((t >> 24) & 0xff),
                    (char)((t >> 16) & 0xff),
                    (char)((t >> 8) & 0xff),
                    (char)(t & 0xff),
                    pos);
          }
        }
      }
    }

    if (vecTableSampleInfos.size() > 0) {
      tableSampleInfos = vecTableSampleInfos[0];
    }
    if (vecTableSamples.size() > 0) {
      tableSamples = vecTableSamples[0];
    }
    indexSample = 0;
    indexTable = 0;
  }
}

MP4Parser::~MP4Parser()
{
  if (tableSampleInfos) {
    delete tableSampleInfos;
  }
  if (tableSamples) {
    delete tableSamples;
  }
  if (input) {
    input->Release();
  }
}

bool
MP4Parser::getNextSample(AP4_Sample& sample,
                         AP4_DataBuffer& db,
                         RokuAv2_DrmInfo& info,
                         StreamBuffer* buffer,
                         RokuAv2_Uint8Range*& liberator)
{
  if (tableSamples && indexSample < tableSamples->GetSampleCount()) {
    if (tableSamples->GetSample(indexSample, sample) == AP4_SUCCESS) {
      if (tableSampleInfos) {
        auto p = (uint8_t*)tableSampleInfos->GetIv(indexSample);
        info.iv = { p, p + tableSampleInfos->GetIvSize() };
        info.key = { kid.data(), kid.data() + kid.size() };

        // Bento cannot read from stream and append to end of data buffer.
        // Boooo.  Your code will be more efficient than this sample channel. =)
        AP4_DataBuffer tmp;
        sample.ReadData(tmp);

        AP4_Size header_size = 0;
        std::vector<Padding> adjustments;
        if (buffer->IsAudio()) {
          header_size = generateADTSPacket(db, sample, tmp);
        } else {
          adjustments = generateVideoPacket(db, sample, tmp);
        }

        unsigned int subsamples = 0;
        const AP4_UI16* clear_data = nullptr;
        const AP4_UI32* encrypted_data = nullptr;
        auto e = tableSampleInfos->GetSampleInfo(
          indexSample, subsamples, clear_data, encrypted_data);
        if (e == AP4_SUCCESS) {
          int ranges = 0;
          for (unsigned int i = 0; i < subsamples; ++i) {
            if (encrypted_data[i] > 0) {
              ++ranges;
            }
#if 0 // Set to 1 to see clear/encrypted subsample length.
            fprintf(stderr,
                    "%s %d %d %d\n",
                    buffer->IsVideo() ? "video" : "audio",
                    indexSample,
                    clear_data[i],
                    encrypted_data[i]);
#endif
          }
          if (ranges > 0) {
            // Liberated after we write samples to decoder in buffer.cpp.
            liberator = new RokuAv2_Uint8Range[ranges];
          }

          // Because we added framing headers, we need to shift all crypto
          // ranges by header_size or adjustments.
          if (subsamples > 0) {
            if (ranges > 0) {
              int32_t offset = 0;
              for (int i = 0; i < ranges; ++i) {
                liberator[i].begin =
                  (uint8_t*)(getPaddingBeforeOffset(offset, adjustments) +
                             clear_data[i]);
                liberator[i].end = liberator[i].begin + encrypted_data[i];
                offset += clear_data[i] + encrypted_data[i];
              }

              info.crypto_ranges.num = ranges;
              info.crypto_ranges.data = liberator;
#if 0 // Set to 1 to see reformated crypto ranges
              for (int i = 0; i < ranges; ++i) {
                fprintf(stderr,
                        "%s %d:%d:%d %p %p %d\n",
                        buffer->IsVideo() ? "video" : "audio",
                        indexTable,
                        indexSample,
                        i,
                        liberator[i].begin,
                        liberator[i].end,
                        liberator[i].end - liberator[i].begin);
              }
#endif

              // Add base address so the pointer is valid.  Above print really
              // is just the offset to help debugging.
              auto base = (uint32_t)db.GetData();
              for (int i = 0; i < ranges; ++i) {
                liberator[i].begin += base;
                liberator[i].end += base;
              }
            } else {
              info.crypto_ranges.num = 0;
#if 0 // Sample is not encrypted.
              fprintf(stderr,
                      "%s %d:%d %d not encrypted\n",
                      buffer->IsVideo() ? "video" : "audio",
                      indexTable,
                      indexSample,
                      db.GetDataSize());
#endif
            }
          } else {
            info.crypto_ranges.num = 1;

            // I know info.crypto_ranges.data has a dummy data structure
            // assigned to it in buffer.cpp, write to it avoids an allocation.
            auto crange = info.crypto_ranges.data;
            crange[0].begin = (uint8_t*)header_size;
            crange[0].end = (uint8_t*)sample.GetSize() + header_size;
#if 0 // No subsamples, everything is encrypted.
            fprintf(stderr,
                    "no subsample for %s %d:%d %d %p %p %d\n",
                    buffer->IsAudio() ? "audio" : "video",
                    indexTable,
                    indexSample,
                    info.crypto_ranges.num,
                    crange[0].begin,
                    crange[0].end,
                    crange[0].end - crange[0].begin);
#endif

            // Add base address so the pointer is valid.  Above print really
            // is just the offset to help debugging.
            auto base = (uint32_t)db.GetData();
            crange[0].begin += base;
            crange[0].end += base;
          }
          ++indexSample;
          return true;
        }
      }
    }
  } else {
    ++indexTable; // Move on to next set of tables for more samples/subsamples.
    if (indexTable < vecTableSampleInfos.size() &&
        indexTable < vecTableSamples.size()) {
      tableSampleInfos = vecTableSampleInfos[indexTable];
      tableSamples = vecTableSamples[indexTable];
      indexSample = 0;
      return getNextSample(sample, db, info, buffer, liberator);
    }
  }

  // No more sample, we've reached end of stream.
  return false;
}

RokuAv2_Time
MP4Parser::convertPTS(AP4_UI64 cts)
{
  return cts * ratio; // Calculated ratio in constructor.
}

// Because we configured our audio and video decoders to take in AAC in ADTS and
// H.264 ES, Roku's decoders need some framing information.  The next functions
// will generate packets by adding appropriate headers.
AP4_Size
MP4Parser::generateADTSPacket(AP4_DataBuffer& data,
                              AP4_Sample& sample,
                              AP4_DataBuffer& bitstream)
{
  // We parsed idxSampling and chAudio in constructor.
  const auto frame_size = sample.GetSize() + 7;
  AP4_UI08 header[7];
  header[0] = 0xff;
  header[1] = 0xf1;
  header[2] = (AP4_UI08)(0x40 | (idxSampling << 2) | (chAudio >> 2));
  header[3] = (AP4_UI08)(((chAudio & 0x3) << 6) | (frame_size >> 11));
  header[4] = (frame_size >> 3) & 0xff;
  header[5] = ((frame_size << 5) & 0xff) | 0x1f;
  header[6] = 0xfc;

  data.AppendData(header, sizeof(header));
  data.AppendData(bitstream.GetData(), bitstream.GetDataSize());

  return sizeof(header);
}

std::vector<Padding>
MP4Parser::generateVideoPacket(AP4_DataBuffer& data,
                               AP4_Sample& sample,
                               AP4_DataBuffer& bitstream)
{
  // For extra bits we add for each NALU, remember how much padding we added
  // fore NALU and its offset in original bitstream.  In getNextSample() we'll
  // recalculate crypto ranges to include the extra padding we added.
  std::vector<Padding> adjustments;

  bool first_nalu = true;
  auto p = bitstream.GetData();
  auto z = bitstream.GetDataSize();
  auto end = p + z;

  // If true, this sample is a random access point and we want to insert SPS
  // and PPS.  We parsed zNALU, SPS, and PPS in constructor.
  bool emit_prefix = sample.IsSync();

  int32_t padding = 0; // How many bytes got added before NALU.
  auto add_prefix = [&] {
    // This lambda can get called before adding NALU or after.
    if (emit_prefix) {
      emit_prefix = false;
      const static AP4_UI08 prefix[] = { 0, 0, 0, 1 };
      data.AppendData(prefix, sizeof(prefix));
      data.AppendData(sps.data(), sps.size());
      data.AppendData(prefix, sizeof(prefix));
      data.AppendData(pps.data(), pps.size());
      padding += sizeof(prefix) + sps.size() + sizeof(prefix) + pps.size();
    }
  };

  while (z > 0) {
    if (z < zNALU) {
      break;
    }

    AP4_UI32 nalu_size;
    switch (zNALU) {
      case 1:
        nalu_size = *p;
        break;
      case 2:
        nalu_size = AP4_BytesToInt16BE(p);
        break;
      case 4:
        nalu_size = AP4_BytesToInt32BE(p);
        break;
      default:
        break;
    }
    p += zNALU;
    z -= zNALU;
    padding -= zNALU;

    if (nalu_size > z) {
      break;
    }

    if (first_nalu) {
      first_nalu = false;
      if (z < 1 || p >= end) {
        break;
      } else if ((p[0] & 0x1f) != AP4_AVC_NAL_UNIT_TYPE_ACCESS_UNIT_DELIMITER) {
        // The first NALU is not an access unit delimiter, we need to add one.
        const static AP4_UI08 delimiter[] = { 0, 0, 0, 1, 9, 0xf0 };
        data.AppendData(delimiter, sizeof(delimiter));
        padding += sizeof(delimiter);
      }
      add_prefix();
    }

    // Add start code before NALU for byte-stream format.
    const static AP4_UI08 start_code[] = { 0, 0, 1 };
    data.AppendData(start_code, sizeof(start_code));
    padding += sizeof(start_code);

    // Add NALU data.
    data.AppendData(p, nalu_size);

    // Record amount of padding added for this NALU
    adjustments.emplace_back(padding, p - bitstream.GetData());

    // For samples that do start with a NALU delimiter, we need to add prefix
    // now.  If we add prefix now it'll bump the next NALU; reset padding now
    // to include the bump.
    padding = 0;
    add_prefix();

    // Skip over to next NALU.
    p += nalu_size;
    z -= nalu_size;

    if (p > end) {
      break;
    }
  } // while (z > 0)

  return adjustments;
}

int32_t
MP4Parser::getPaddingBeforeOffset(size_t offset, std::vector<Padding>& paddings)
{
  int32_t padding = 0;
  for (auto&& record : paddings) {
    if (record.offset > offset) {
      padding += record.padding;
      record.padding = 0;
    } else {
      break;
    }
  }
  return padding;
}
