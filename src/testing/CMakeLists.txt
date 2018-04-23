
set(LEAP_CPP_TEST_SRCS
  ApiPropertyTest.cpp
  IteratorTest.cpp
)

add_executable(LeapC++Test ${LEAP_CPP_TEST_SRCS})

target_link_libraries(LeapC++Test LeapC++ gtest)
if(WIN32)
  target_compile_options(LeapC++Test PRIVATE "/wd4996")
endif()
