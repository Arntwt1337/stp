set(target_name ftstest)

add_executable(${target_name} ftstest.cpp)

include(GoogleTest)
include(CompileOptions)
set_compile_options(${target_name})

target_sources(
  ${target_name}
  PRIVATE
    ftstest.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
    fts
    gtest_main
    gtest
)

add_test(NAME ${target_name} COMMAND ${target_name})