# Darmix-Core Copyright (C) 2013  Deremix
# Integrated Files: CREDITS.md and LICENSE.md

set(Detour_STAT_SRCS
    DetourAlloc.cpp
    DetourAlloc.h
    DetourCommon.cpp
    DetourNavMesh.cpp
    DetourNavMeshBuilder.cpp
    DetourNavMeshQuery.cpp
    DetourNode.cpp
)

if(WIN32)
  include_directories(
    ${CMAKE_SOURCE_DIR}/dep/zlib
  )
endif()

add_library(Detour STATIC ${Detour_STAT_SRCS})

target_link_libraries(Detour ${ZLIB_LIBRARIES})