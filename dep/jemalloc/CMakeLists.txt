# BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
# Integrated Files: CREDITS.md and LICENSE.md

file(GLOB sources src/*.c)

set(jemalloc_STAT_SRC
  ${sources}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_definitions(-D_GNU_SOURCE -D_REENTRANT)

add_library(jemalloc STATIC ${sources})
