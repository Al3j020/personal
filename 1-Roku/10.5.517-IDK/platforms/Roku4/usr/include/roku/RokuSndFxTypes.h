// Copyright (c) 2014 Roku Inc.  All rights reserved.

#ifndef ROKU_SFX_TYPES_H
#define ROKU_SFX_TYPES_H

typedef enum {
  ROKU_SFX_ERR_OK,
  ROKU_SFX_ERR_AGAIN,
  ROKU_SFX_ERR_INTERRUPTED,
  ROKU_SFX_ERR_BUSY,
  ROKU_SFX_ERR_INVALID_ARG,
  ROKU_SFX_ERR_TIMEOUT,
  ROKU_SFX_ERR_URUN,
  ROKU_SFX_ERR_INTERNAL
} RokuSndFx_Err;


typedef enum {
  ROKU_SFX_ENDIAN_LIT = 0,
  ROKU_SFX_ENDIAN_BIG
} RokuSndFx_Endianness;


#endif // ROKU_SFX_TYPES_H
