
set(LEAP_CPP_TEST_SRCS
  ApiPropertyTest.cpp
  IteratorTest.cpp
)

add_executable(LeapC++Test ${LEAP_CPP_TEST_SRCS})

target_link_libraries(LeapC++Test LeapC++ gtest)
target_compile_definitions(LeapC++Test PUBLIC _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING=1)
