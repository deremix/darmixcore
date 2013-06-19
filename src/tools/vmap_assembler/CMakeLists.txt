# BlizzLikeCore Copyright (C) 2013  BlizzLikeGroup
# Integrated Files: CREDITS.md and LICENSE.md

include_directories(
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/collision
  ${CMAKE_SOURCE_DIR}/src/framework
  ${ACE_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}
)

add_definitions(-DNO_CORE_FUNCS)
add_executable(vmap_assembler vmap_assembler.cpp)
add_dependencies(vmap_assembler mpq)

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set_target_properties(vmap_assembler PROPERTIES LINK_FLAGS "-framework Carbon")
endif()

target_link_libraries(vmap_assembler
  collision
  g3dlib
  ${ZLIB_LIBRARIES}
)

if( UNIX )
  install(TARGETS vmap_assembler DESTINATION bin)
elseif( WIN32 )
  install(TARGETS vmap_assembler DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
