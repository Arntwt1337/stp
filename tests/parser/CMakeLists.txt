set(target_name parsertest)

add_executable(${target_name} parsertest.cpp)

include(GoogleTest)
include(CompileOptions)
set_compile_options(${target_name})

target_sources(
  ${target_name}
  PRIVATE
    parsertest.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
    parser
    gtest_main
    gtest
)

add_test(NAME ${target_name} COMMAND ${target_name})