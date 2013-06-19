# Darmix-Core Copyright (C) 2013  Deremix
# Integrated Files: CREDITS.md and LICENSE.md

file(GLOB sources_localdir *.cpp *.h)

set(world-server_SRCS
  ${world-server_SRCS}
  ${sources_localdir}
)

if( WIN32 )
  set(world-server_SRCS
    ${world-server_SRCS}
    ${sources_Debugging}
    world-server.rc
  )
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/gsoap
  ${CMAKE_SOURCE_DIR}/dep/sockets/include
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/dep/mersennetwister
  ${CMAKE_SOURCE_DIR}/src/collision
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_SOURCE_DIR}/src/game
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

set(world-server_LINK_FLAGS "")

add_executable(world-server ${world-server_SRCS})

if( NOT WIN32 )
  add_definitions(-D_Darmix_WORLD_CONFIG="${CONF_DIR}/world-server.conf")
endif()

add_dependencies(world-server revision.h)

if( UNIX )
  set(world-server_LINK_FLAGS "-pthread ${world-server_LINK_FLAGS}")
endif()

set_target_properties(world-server PROPERTIES LINK_FLAGS "${world-server_LINK_FLAGS}")

target_link_libraries(world-server
  game
  shared
  scripts
  darmixframework
  collision
  g3dlib
  gsoap
  Recast
  Detour
  ${JEMALLOC_LIBRARY}
  ${READLINE_LIBRARY}
  ${TERMCAP_LIBRARY}
  ${ACE_LIBRARY}
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${OSX_LIBS}
)

if( WIN32 )
  add_custom_command(TARGET world-server
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/world-server.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS world-server DESTINATION bin)
  install(FILES world-server.conf.dist DESTINATION etc)
elseif( WIN32 )
  install(TARGETS world-server DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES world-server.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
