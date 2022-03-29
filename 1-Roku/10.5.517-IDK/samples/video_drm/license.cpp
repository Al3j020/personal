// Copyright 2021 Roku, Inc.  All rights reserved.
#include "license.h"
#include <cstring>
#include <curl/curl.h>
#include <string>
#include <vector>

extern "C" RokuDrm_Error
RokuDrm_SesSetEncryptionScheme(RokuDrm_Ses*, RokuDrm_EncScheme);

namespace {
// cURL write callback to save downloaded license.
size_t
fetchWriteCallback(char* data, size_t z, size_t n, void* pLicense)
{
  const auto count = z * n;
  auto license = (std::vector<uint8_t>*)pLicense;
  auto p = (uint8_t*)data;
  license->insert(license->end(), p, p + count);
  return count;
}

void
fetchLicense(const std::vector<uint8_t>& challenge,
             std::vector<uint8_t>& license)
{
  if (auto h = curl_easy_init()) {
    curl_easy_setopt(h, CURLOPT_URL, "https://proxy.uat.widevine.com/proxy");
    curl_easy_setopt(h, CURLOPT_VERBOSE, 0L); // Set to 1 to follow request.
    curl_easy_setopt(h, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(h, CURLOPT_CAINFO, "/common/certs/ca-bundle.crt");
    curl_easy_setopt(h, CURLOPT_POSTFIELDSIZE, (long)(challenge.size()));
    curl_easy_setopt(h, CURLOPT_POSTFIELDS, challenge.data());
    curl_easy_setopt(h, CURLOPT_POST, 1L);
    curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, fetchWriteCallback);
    curl_easy_setopt(h, CURLOPT_WRITEDATA, &license);
    curl_easy_perform(h);
    curl_easy_cleanup(h);
  }
}

// This PSSH is taken from:
//
//   https://google.github.io/shaka-packager/html/tutorials/raw_key.html
//
// Please see README for more information on how it was used to encrypt test
// bitstreams for this sample.
const std::vector<uint8_t> PSSH = {
  0x00, 0x00, 0x00, 0x31, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0xed,
  0xef, 0x8b, 0xa9, 0x79, 0xd6, 0x4a, 0xce, 0xa3, 0xc8, 0x27, 0xdc, 0xd5, 0x1d,
  0x21, 0xed, 0x00, 0x00, 0x00, 0x11, 0x22, 0x0f, 0x74, 0x65, 0x73, 0x74, 0x20,
  0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x20, 0x69, 0x64
};
} // anonymous namespace

bool
AcquireWidevineDRMLicense(RokuDrm_Ses* session)
{
  bool ok = false;
  auto bufferLicense = std::vector<uint8_t>();
  auto bufferChallenge = std::vector<uint8_t>(4096, 0);
  uint32_t zChallenge = 0;

  fprintf(stderr,
          "generating challenge (%zd) from %zd bytes of custom data\n",
          bufferChallenge.capacity(),
          PSSH.size());

  auto e = RokuDrm_SesGenerateChallenge(session,
                                        PSSH.data(),
                                        (uint32_t)PSSH.size(),
                                        (uint32_t)bufferChallenge.capacity(),
                                        bufferChallenge.data(),
                                        &zChallenge);
  if (e == ROKU_DRM_ERR_OK) {
    bufferChallenge.resize(zChallenge);
    fprintf(stderr,
            "fetching license, challenge size: %zd\n",
            bufferChallenge.size());
    fetchLicense(bufferChallenge, bufferLicense);
    fprintf(stderr, "fetched license %zd bytes\n", bufferLicense.size());

    if (bufferLicense.size() > 0) {
#if 0 // Set to 1 to see license content.
      for (size_t i = 0; i < bufferLicense.size(); ++i) {
        fprintf(stderr, "%02x", bufferLicense[i]);
      }
      fprintf(stderr, "\n");
#endif
      e = RokuDrm_SesSetLicense(
        session, bufferLicense.data(), bufferLicense.size());
      if (e == ROKU_DRM_ERR_OK) {
        fprintf(stderr, "set license data successfully\n");
        ok = true;
      } else {
        fprintf(stderr, "failed to set license data\n");
      }
#if 0
      e = RokuDrm_SesSetEncryptionScheme(session,
                                         RokuDrm_EncScheme::ROKU_ESCHEME_CENC);
#endif
    }
  } else {
    fprintf(stderr,
            "unable to generate challenge (%d), challenge size: %d\n",
            e,
            zChallenge);
  }
  return ok;
}
