set(target_name indextest)

add_executable(${target_name} indextest.cpp)

include(GoogleTest)
include(CompileOptions)
set_compile_options(${target_name})

target_sources(
  ${target_name}
  PRIVATE
  indextest.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
    index
    gtest_main
    gtest
)

add_test(NAME ${target_name} COMMAND ${target_name})