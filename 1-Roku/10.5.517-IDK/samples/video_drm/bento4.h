// Copyright 2021 Roku, Inc.  All rights reserved.
#ifndef IDK_SAMPLE_BENTO4_HELPER
#define IDK_SAMPLE_BENTO4_HELPER
#include <Ap4.h>
#include <Ap4Mp4AudioInfo.h>
#include <Ap4SaioAtom.h>
#include <Ap4SaizAtom.h>
#include <Ap4SencAtom.h>
#include <cinttypes>
#include <roku/RokuAv2Decoder.h>
#include <sstream>
#include <vector>
struct Padding
{
  int32_t padding;
  const size_t offset;

  Padding(int32_t p, size_t o)
    : padding(p)
    , offset(o)
  {}
};

struct StreamBuffer;
struct MP4Parser
{
  MP4Parser(std::string path, StreamBuffer*);
  virtual ~MP4Parser();

  // Extract sample and subsample tables from MOOF.
  bool updateTables(StreamBuffer*);

  // Call this to get the next sample in bitstream.
  bool getNextSample(AP4_Sample&,           // Sample.
                     AP4_DataBuffer&,       // Packet for sample.
                     RokuAv2_DrmInfo&,      // Updated crypto ranges.
                     StreamBuffer*,         // For debug prints.
                     RokuAv2_Uint8Range*&); // Free array after use.

  // This API converts frame number to MHz resolution we need.
  RokuAv2_Time convertPTS(AP4_UI64);

  // These functions will format the sample and add headers or delimiters.
  AP4_Size generateADTSPacket(AP4_DataBuffer&, AP4_Sample&, AP4_DataBuffer&);
  std::vector<Padding> generateVideoPacket(AP4_DataBuffer&,
                                           AP4_Sample&,
                                           AP4_DataBuffer&);
  int32_t getPaddingBeforeOffset(size_t offset, std::vector<Padding>&);

  AP4_ByteStream* input;
  AP4_Movie* movie;
  AP4_Track* track;
  AP4_CencSampleInfoTable* tableSampleInfos;
  std::vector<AP4_CencSampleInfoTable*> vecTableSampleInfos;
  AP4_FragmentSampleTable* tableSamples;
  std::vector<AP4_FragmentSampleTable*> vecTableSamples;
  AP4_Cardinal indexSample; // Bitstream may have multiple MOOF, each with its
  AP4_Cardinal indexTable;  // own tables.
  AP4_ProtectedSampleDescription* descProtected; // For making subsamples table.
  std::vector<uint8_t> kid;                      // Key ID.
  std::vector<uint8_t> sps;                      // Sequence parameter set.
  std::vector<uint8_t> pps;                      // Picture parameter set.
  float ratio;                                   // Used for PTS conversion.
  unsigned int idxSampling;                      // ADTS sample rate index.
  unsigned int chAudio;                          // Number of audio channels.
  unsigned int zNALU; // NALU length size for parsing.
};
#endif //#ifndef IDK_SAMPLE_BENTO4_HELPER
