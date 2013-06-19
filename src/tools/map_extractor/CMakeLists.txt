# BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
# Integrated Files: CREDITS.md and LICENSE.md

file(GLOB_RECURSE sources *.cpp *.h)

if( UNIX )
  include_directories (
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/dep/libmpq
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/loadlib
    ${CMAKE_SOURCE_DIR}/dep/zlib
  )
elseif( WIN32 )
  include_directories (
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/dep/libmpq
    ${CMAKE_SOURCE_DIR}/dep/libmpq/win
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/loadlib
    ${CMAKE_SOURCE_DIR}/dep/zlib
  )
endif()

add_executable(map_extractor
  ${sources}
)

target_link_libraries(map_extractor
  mpq
  ${BZIP2_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

add_dependencies(map_extractor mpq)

if( UNIX )
  install(TARGETS map_extractor DESTINATION bin)
elseif( WIN32 )
  install(TARGETS map_extractor DESTINATION "${CMAKE_INSTALL_PREFIX}") 
endif()
