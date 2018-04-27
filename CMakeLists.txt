cmake_minimum_required(VERSION 3.4)
include(version.cmake)
include("standard/Standard.cmake")

project(LeapC++ VERSION ${LeapC++_VERSION})
parse_version(LeapC++ ${LeapC++_VERSION})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

# Recurse through source directories
add_subdirectory(contrib)
add_subdirectory(src)

# Now we can generate the version and install stuff
#generate_version()
combined_installer(
  VENDOR "Leap Motion"
  CONTACT "cmercenary@gmail.com"
)
