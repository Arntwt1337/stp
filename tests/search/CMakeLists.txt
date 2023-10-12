set(target_name searchtest)

add_executable(${target_name} searchtest.cpp)

include(GoogleTest)
include(CompileOptions)
set_compile_options(${target_name})

target_sources(
  ${target_name}
  PRIVATE
  searchtest.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
    search
    index
    gtest_main
    gtest
)

add_test(NAME ${target_name} COMMAND ${target_name})