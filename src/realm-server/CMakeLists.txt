# Darmix-Core Copyright (C) 2013  Deremix
# Integrated Files: CREDITS.md and LICENSE.md

file(GLOB sources_localdir *.cpp *.h)

set(realm-server_SRCS
  ${realm-server_SRCS}
  ${sources_localdir}
)

if( WIN32 )
  set(realm-server_SRCS
    ${realm-server_SRCS}
    ${sources_Debugging}
    realm-server.rc
  )
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_SOURCE_DIR}/src/framework
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

set(realm-erver_LINK_FLAGS "")

add_executable(realm-server
  ${realm-server_SRCS}
)

add_dependencies(realm-server revision.h)

if( NOT WIN32 )
  add_definitions(-D_DARMIX_AUTH_CONFIG="${CONF_DIR}/realm-server.conf")
endif()

if( UNIX )
  set(realm-server_LINK_FLAGS "-pthread ${realm-server_LINK_FLAGS}")
endif()

if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
  set(realm-server_LINK_FLAGS "-framework Carbon ${realm-server_LINK_FLAGS}")
endif()

set_target_properties(realm-server PROPERTIES LINK_FLAGS "${realm-server_LINK_FLAGS}")

target_link_libraries(realm-server
  shared
  darmixframework
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${OSX_LIBS}
)

if( WIN32 )
  add_custom_command(TARGET realm-server
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/realm-server.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS realm-server DESTINATION bin)
  install(FILES realm-server.conf.dist DESTINATION etc)
elseif( WIN32 )
  install(TARGETS realm-server DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES realm-server.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
