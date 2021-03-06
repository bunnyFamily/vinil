/** 
 *  @file       crossplatform.h
 *  @brief      Defines some cross-platiform functions
 *  @author     Igor Bonadio
 *  @copyright  MIT License. See Copyright Notice in LICENSE.txt
 */

#ifndef VINIL_CROSSPLATFORM_H_
#define VINIL_CROSSPLATFORM_H_

#ifndef _WIN32
  #define VINILAPI
#else
  #ifdef __cplusplus
    #define VINILAPI extern "C" __declspec(dllexport)
  #else
    #define VINILAPI __declspec(dllexport)
  #endif
#endif

#ifdef _WIN32
  #include <windows.h>
  #include <io.h>
#else
  #include <uuid/uuid.h>
#endif

#include <stdio.h>
#include <stdint.h>

#include "util.h"

typedef uuid_t vinil_uuid;

VINILAPI void vinil_uuid_generate(vinil_uuid* uuid);
VINILAPI int vinil_fseek(FILE *fd, int64_t offset, int origin);
VINILAPI int64_t vinil_ftell(FILE *fd);
VINILAPI int vinil_truncate(FILE *fd, int64_t new_length);


#endif