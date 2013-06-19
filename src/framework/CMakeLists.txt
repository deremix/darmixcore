# Darmix-Core Coryright (C) 2013  Deremix
# Integrated Files: CREDITS.md and LICENSE.md

SET(darmixframework_STAT_SRCS
   Policies/ObjectLifeTime.cpp
   Utilities/EventProcessor.cpp
)

include_directories(
  ${ACE_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/src/framework
)

add_library(darmixframework STATIC ${darmixframework_STAT_SRCS})
