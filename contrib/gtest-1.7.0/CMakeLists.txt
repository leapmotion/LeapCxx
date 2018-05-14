add_library(gtest STATIC
  fused-src/gtest/gtest-all.cc
)
target_include_directories(gtest PUBLIC fused-src)
target_sources(gtest INTERFACE ${CMAKE_CURRENT_LIST_DIR}/fused-src/gtest/gtest_main.cc)
target_compile_definitions(gtest PUBLIC _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
