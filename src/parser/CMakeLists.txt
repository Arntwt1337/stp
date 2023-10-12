set(target_name parser)
set(HEADER_LIST "${CMAKE_SOURCE_DIR}/include/parser/parser.hpp")

add_library(${target_name} STATIC
  parser.cpp
  ${HEADER_LIST})

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
  ${target_name}
  PRIVATE
    nlohmann_json
)