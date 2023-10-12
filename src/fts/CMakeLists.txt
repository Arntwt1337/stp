set(target_name fts)
set(HEADER_LIST "${CMAKE_SOURCE_DIR}/include/fts/fts.hpp")

add_library(${target_name} STATIC
  fts.cpp
  ${HEADER_LIST})

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)