add_library(
  gtest
  fused-src/gtest/gtest-all.cc
)
target_include_directories(
  gtest
  PUBLIC
  fused-src
)
target_sources(gtest INTERFACE ${CMAKE_CURRENT_LIST_DIR}/fused-src/gtest/gtest_main.cc)
