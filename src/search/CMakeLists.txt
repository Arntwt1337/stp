set(target_name search)
set(HEADER_LIST "${CMAKE_SOURCE_DIR}/include/search/search.hpp")

add_library(${target_name} STATIC
  search.cpp
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
    picosha2
    parser
)