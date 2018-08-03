cmake_minimum_required(VERSION 3.4)
include(version.cmake)
include("standard/Standard.cmake")

standard_project(LeapC++ VERSION ${LeapC++_VERSION})

unset(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG)
unset(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE)
unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG)
unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE)
unset(CMAKE_DEBUG_POSTFIX)
unset(CMAKE_RELEASE_POSTFIX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
parse_version(LeapC++ ${LeapC++_VERSION})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

if(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_LIBRARY_ARCHITECTURE x64)
  else()
    set(CMAKE_LIBRARY_ARCHITECTURE x86)
  endif()
endif()

if(BUILD_SWIG)
  find_package(PythonInterp 3)
  find_package(PythonLibs 3)
  if(NOT (DEFINED PYTHON_EXECUTABLE AND DEFINED PYTHON_INCLUDE_DIR AND DEFINED PYTHON_LIBRARY))
    message(SEND_ERROR "Python 3 not found.")
  endif()
  find_package(JNI 8.0)
  find_package(SWIG 3.0)
endif()

find_package(LeapSDK 4.0.0)

# Recurse through source directories
add_subdirectory(contrib)
add_subdirectory(src)

option(BUILD_SAMPLES "Build the samples" ON)
if(BUILD_SAMPLES)
  add_subdirectory(samples)
endif()

# Now we can generate the version and install stuff
generate_version()
combined_installer(
  VENDOR "Leap Motion"
  CONTACT "wgray@leapmotion.com"
)
