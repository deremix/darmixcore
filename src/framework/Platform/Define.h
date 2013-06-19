/*
 * BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_DEFINE_H
#define BLIZZLIKE_DEFINE_H

#include <sys/types.h>

#include <ace/Basic_Types.h>
#include <ace/ACE_export.h>

#include "Platform/CompilerDefs.h"

#define BLIZZLIKE_LITTLEENDIAN 0
#define BLIZZLIKE_BIGENDIAN    1

#if !defined(BLIZZLIKE_ENDIAN)
#  if defined (ACE_BIG_ENDIAN)
#    define BLIZZLIKE_ENDIAN BLIZZLIKE_BIGENDIAN
#  else //ACE_BYTE_ORDER != ACE_BIG_ENDIAN
#    define BLIZZLIKE_ENDIAN BLIZZLIKE_LITTLEENDIAN
#  endif //ACE_BYTE_ORDER
#endif //BLIZZLIKE_ENDIAN

#if PLATFORM == PLATFORM_WINDOWS
#  define BLIZZLIKE_PATH_MAX MAX_PATH
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#else //PLATFORM != PLATFORM_WINDOWS
#  define BLIZZLIKE_PATH_MAX PATH_MAX
#  define DECLSPEC_NORETURN
#endif //PLATFORM

#if !defined(COREDEBUG)
#  define BLIZZLIKE_INLINE inline
#else //COREDEBUG
#  if !defined(BLIZZLIKE_DEBUG)
#    define BLIZZLIKE_DEBUG
#  endif //BLIZZLIKE_DEBUG
#  define BLIZZLIKE_INLINE
#endif //!COREDEBUG

#if COMPILER == COMPILER_GNU
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F,V) __attribute__ ((format (printf, F, V)))
#else //COMPILER != COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F,V)
#endif //COMPILER == COMPILER_GNU

typedef ACE_INT64 int64;
typedef ACE_INT32 int32;
typedef ACE_INT16 int16;
typedef ACE_INT8 int8;
typedef ACE_UINT64 uint64;
typedef ACE_UINT32 uint32;
typedef ACE_UINT16 uint16;
typedef ACE_UINT8 uint8;

#if COMPILER != COMPILER_MICROSOFT
typedef uint16      WORD;
typedef uint32      DWORD;
#endif //COMPILER

typedef uint64 OBJECT_HANDLE;

#endif //BLIZZLIKE_DEFINE_H

