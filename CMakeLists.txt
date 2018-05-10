cmake_minimum_required(VERSION 3.4)
include(version.cmake)
include("standard/Standard.cmake")

project(LeapC++ VERSION ${LeapC++_VERSION})
parse_version(LeapC++ ${LeapC++_VERSION})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

if(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_LIBRARY_ARCHITECTURE x64)
  else()
    set(CMAKE_LIBRARY_ARCHITECTURE x86)
  endif()
endif()

find_package(jni 8.0)
find_package(swig 3.0.12)
find_package(LeapSDK 4.0.0)

# Recurse through source directories
add_subdirectory(contrib)
add_subdirectory(src)

option(BUILD_SAMPLES "Build the samples" ON)
if(BUILD_SAMPLES)
  add_subdirectory(samples)
endif()

# Now we can generate the version and install stuff
#generate_version()
combined_installer(
  VENDOR "Leap Motion"
  CONTACT "cmercenary@gmail.com"
)
