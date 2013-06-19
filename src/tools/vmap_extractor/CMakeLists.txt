# BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
# Integrated Files: CREDITS.md and LICENSE.md

file(GLOB_RECURSE sources *.cpp *.h)

# uncomment next line to disable debug mode
add_definitions("-DIOMAP_DEBUG")

# build setup currently only supports libmpq 0.4.x
add_definitions("-DUSE_LIBMPQ04")
add_definitions("-Wall")

if(NOT WIN32)
  add_definitions("-ggdb")
  add_definitions("-O3")
endif(NOT WIN32)

if( UNIX )
  include_directories(
    ${CMAKE_SOURCE_DIR}/dep/libmpq
  )
elseif( WIN32 )
  include_directories(
    ${CMAKE_SOURCE_DIR}/dep/libmpq
    ${CMAKE_SOURCE_DIR}/dep/libmpq/win
  )
endif()

add_executable(vmap_extractor ${sources})

target_link_libraries(vmap_extractor
  mpq
  ${BZIP2_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

add_dependencies(vmap_extractor mpq)

if( UNIX )
  install(TARGETS vmap_extractor DESTINATION bin)
elseif( WIN32 )
  install(TARGETS vmap_extractor DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
