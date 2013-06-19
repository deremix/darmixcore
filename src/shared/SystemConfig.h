/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

// THIS FILE IS DEPRECATED

#ifndef BLIZZLIKE_SYSTEMCONFIG_H
#define BLIZZLIKE_SYSTEMCONFIG_H

#include "Platform/Define.h"
#include "revision.h"


#define _PACKAGENAME "Darmix-Core "
#define _CODENAME "VELOCITY"

#if BLIZZLIKE_ENDIAN == BLIZZLIKE_BIGENDIAN
# define _ENDIAN_STRING "big-endian"
#else
# define _ENDIAN_STRING "little-endian"
#endif

#if PLATFORM == PLATFORM_WINDOWS
# ifdef _WIN64
#  define _FULLVERSION _PACKAGENAME "Rev: " _REVISION  " Hash: " _HASH " (Win64," _ENDIAN_STRING ")"
# else
#  define _FULLVERSION _PACKAGENAME "Rev: " _REVISION  " Hash: " _HASH " (Win32," _ENDIAN_STRING ")"
# endif
#else
#  define _FULLVERSION _PACKAGENAME "Rev: " _REVISION  " Hash: " _HASH " (Unix," _ENDIAN_STRING ")"
#endif

#define DEFAULT_PLAYER_LIMIT 100
#define DEFAULT_WORLDSERVER_PORT 8085                       //8129
#define DEFAULT_AUTHSERVER_PORT 3724
#define DEFAULT_SOCKET_SELECT_TIME 10000
#endif

