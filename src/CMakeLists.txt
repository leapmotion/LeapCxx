set(PROJECT_EXECUTABLE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})

set(LEAP_CPP_SRCS
  LeapC++.cpp
  LeapC++.h
  LeapImplementationC++.cpp
  LeapImplementationC++.h
  LeapMath.h
)

set(ORIGINAL_FILE_NAME "LeapC++.dll")
configure_file(LeapC++.rc.in ${CMAKE_CURRENT_BINARY_DIR}/LeapC++.rc @ONLY)
add_library(LeapC++ ${LEAP_CPP_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/LeapC++.rc)
set_property(TARGET LeapC++ PROPERTY FOLDER "API")
target_include_directories(
  LeapC++
  PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

if(WIN32)
  target_compile_options(LeapC++ PUBLIC "/wd4996")
endif()

if(APPLE)
  set_target_properties(LeapC++ PROPERTIES LINK_FLAGS "${LINK_FLAGS} -compatibility_version ${LEAP_COMPATIBILITY_VERSION_MAJOR}.${LEAP_COMPATIBILITY_VERSION_MINOR}.${LEAP_COMPATIBILITY_VERSION_PATCH} -current_version ${LEAP_VERSION_MAJOR}.${LEAP_VERSION_MINOR}.${LEAP_VERSION_PATCH}")
endif()

target_link_libraries(LeapC++ LeapSDK::LeapC)

include(CMakeDependentOption)

install_headers(TARGET LeapC++ DESTINATION include/leapc++ COMPONENT LeapC++)
install(TARGETS LeapC++ EXPORT LeapC++Targets
  DESTINATION lib
  COMPONENT LeapC++
  CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
)

option(BUILD_TESTING "Build Tests" OFF)
if(BUILD_TESTING)
  add_subdirectory(testing)
endif()

option(BUILD_SWIG "Build Java & Python Swig bindings" OFF)
if(NOT BUILD_SWIG)
  return()
endif()

set(LEAP_API_SWIG_INTERFACES
  LeapC++.i
)

set(LEAP_API_SWIG_INTERFACES_DEPENDENCIES
  LeapC++.h
  LeapMath.h
)

set(ORIGINAL_FILE_NAME "LeapJava.dll")
configure_file(LeapC++.rc.in ${CMAKE_CURRENT_BINARY_DIR}/LeapJava.rc @ONLY)
set(ORIGINAL_FILE_NAME "LeapPython.dll")
configure_file(LeapC++.rc.in ${CMAKE_CURRENT_BINARY_DIR}/LeapPython.rc @ONLY)

set(LEAP_JAVA_API_SRCS ${CMAKE_CURRENT_BINARY_DIR}/LeapJava.rc)
set(LEAP_PYTHON_API_SRCS ${CMAKE_CURRENT_BINARY_DIR}/LeapPython.rc)

if(ANDROID)
  # workaround as Bionic loader does not respect rpath
  set(LEAP_JAVA_API_SRCS ${LEAP_API_SRCS} ${LEAP_JAVA_API_SRCS})
  set(LEAP_PYTHON_API_SRCS ${LEAP_API_SRCS} ${LEAP_PYTHON_API_SRCS})
endif()
include(UseSWIG)
set(SWIG_CXX_EXTENSION "cpp")
set_source_files_properties(${LEAP_API_SWIG_INTERFACES} PROPERTIES CPLUSPLUS ON)

##############################################################################
# Java Bindings
##############################################################################
find_package(Java COMPONENTS Development)
if(Java_JAVAC_EXECUTABLE AND NOT Java_JAVA_EXECUTABLE MATCHES "NOTFOUND")
  # OS X: Java_JAVAC_EXECUTABLE always set due to /usr/bin/javac placeholder
  #       Using FindJava.cmake's check for "No Java runtime present" string
  set(LEAP_API_SWIG_JAVA ${CMAKE_CURRENT_BINARY_DIR}/leap_api_swig_Java)
  if(WIN32)
    add_custom_command(OUTPUT ${LEAP_API_SWIG_JAVA}
                       COMMAND ${CMAKE_COMMAND} -E touch ${LEAP_API_SWIG_JAVA}
                       COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/Java
                       COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/LeapJava.jar.dir/com
                       DEPENDS ${LEAP_API_SWIG_INTERFACES} ${LEAP_API_SWIG_INTERFACES_DEPENDENCIES}
                       COMMENT "Creating Java SWIG bindings")
  else()
    add_custom_command(OUTPUT ${LEAP_API_SWIG_JAVA}
                       COMMAND ${CMAKE_COMMAND} -E touch ${LEAP_API_SWIG_JAVA}
                       COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_BINARY_DIR}/Java/*.java
                       COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_BINARY_DIR}/Java/LeapC++JAVA_wrap.cpp
                       COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/LeapJava.jar.dir/com
                       DEPENDS ${LEAP_API_SWIG_INTERFACES} ${LEAP_API_SWIG_INTERFACES_DEPENDENCIES}
                       COMMENT "Creating Java SWIG bindings")
  endif()

  set(CMAKE_SWIG_FLAGS -package;com.leapmotion.leap)
  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/Java)
  set(SWIG_MODULE_LeapJava_EXTRA_DEPS ${LEAP_API_SWIG_JAVA})
  if(CMAKE_VERSION VERSION_LESS 3.8)
    swig_add_module(LeapJava java ${LEAP_API_SWIG_INTERFACES} ${LEAP_JAVA_API_SRCS})
  else()
    swig_add_library(LeapJava TYPE MODULE LANGUAGE java SOURCES ${LEAP_API_SWIG_INTERFACES} ${LEAP_JAVA_API_SRCS})
  endif()
  set_property(TARGET LeapJava PROPERTY FOLDER "API")
    set_target_properties(LeapJava PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})

  if(APPLE)
    set_target_properties(LeapJava PROPERTIES SUFFIX .dylib)
  endif()

  if(NOT WIN32)
    # Suppress compiler warnings from auto-generated wrapper file
    if(ANDROID)
      # gcc makes it nigh impossible to use -Werror with void** conversions
      set(_fpermissive_war "-Wno-error")
    endif()
    set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/Java/LeapC++JAVA_wrap.cpp PROPERTY COMPILE_FLAGS "-Wno-unused-function ${_fpermissive_war}")
  endif()

  target_include_directories(LeapJava PUBLIC ${JAVA_INCLUDE_PATH})
  target_include_directories(LeapJava PUBLIC ${JAVA_INCLUDE_PATH2})

  if(ANDROID)
    set_target_properties(LeapJava PROPERTIES COMPILE_FLAGS "-DLEAP_API_IMPLEMENTATION=1")
    target_link_libraries(LeapJava Autowiring::Autowiring Calibration Networking SystemUtility)
    set_target_properties(LeapJava PROPERTIES LINK_FLAGS_RELEASE "-Xlinker -s")
  endif()
  target_link_libraries(LeapJava LeapC++)

  set(LEAP_JNI_SOURCE_FILE_LIST ${CMAKE_CURRENT_BINARY_DIR}/leap_jni_sources)

  # Create list of Java source files
  add_custom_command(OUTPUT ${LEAP_JNI_SOURCE_FILE_LIST}
                     DEPENDS LeapJava
                     COMMAND ${CMAKE_COMMAND}
                       -DJAVA_SOURCE_DIR_PATHS=${CMAKE_SWIG_OUTDIR}
                       -DJAVA_SOURCE_FILE_LIST=${LEAP_JNI_SOURCE_FILE_LIST}
                       -P ${CMAKE_SOURCE_DIR}/cmake-modules/UseJavaSourceFileList.cmake)

  list(APPEND SDK_COMPONENTS LeapJava)

  include(UseJavaEx)
  set(CMAKE_JAVA_COMPILE_FLAGS -d ${CMAKE_SWIG_OUTDIR} -source 6 -target 6)
  add_jar_ex(LeapJava.jar FILELIST ${LEAP_JNI_SOURCE_FILE_LIST} OUTPUT_NAME LeapJava OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
  set_property(TARGET LeapJava.jar PROPERTY FOLDER "API")
  add_dependencies(LeapJava.jar LeapJava)
  add_custom_command(TARGET LeapJava.jar POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_CURRENT_BINARY_DIR}/LeapJava.jar
                       ${PROJECT_EXECUTABLE_PATH}/LeapJava.jar)

  list(APPEND SDK_COMPONENTS LeapJava.jar)
endif()

##############################################################################
# Python Bindings
##############################################################################
if(PYTHONLIBS_FOUND)
  set(LEAP_API_SWIG_PYTHON ${CMAKE_CURRENT_BINARY_DIR}/leap_api_swig_Python)
  add_custom_command(OUTPUT ${LEAP_API_SWIG_PYTHON}
                     COMMAND ${CMAKE_COMMAND} -E touch ${LEAP_API_SWIG_PYTHON}
                     COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_BINARY_DIR}/Python/LeapPYTHON_wrap.cpp
                     DEPENDS ${LEAP_API_SWIG_INTERFACES} ${LEAP_API_SWIG_INTERFACES_DEPENDENCIES}
                     COMMENT "Creating Python SWIG bindings")

  set(SWIG_MODULE_LeapPython_NOPROXY TRUE)
  set(CMAKE_SWIG_FLAGS -interface;LeapPython)
  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/Python)
  set(SWIG_MODULE_LeapPython_EXTRA_DEPS ${LEAP_API_SWIG_PYTHON})
  if(CMAKE_VERSION VERSION_LESS 3.8)
    swig_add_module(LeapPython python ${LEAP_API_SWIG_INTERFACES} ${LEAP_PYTHON_API_SRCS})
  else()
    set(swig_extra_generated_files "")
    swig_add_library(LeapPython TYPE MODULE LANGUAGE python SOURCES ${LEAP_API_SWIG_INTERFACES} ${LEAP_PYTHON_API_SRCS})
    #otherwise LeapC++.i isn't added to the sources list
    target_sources(LeapPython PRIVATE ${LEAP_API_SWIG_INTERFACES})
  endif()

  set_property(TARGET LeapPython PROPERTY FOLDER "API")
  set_target_properties(LeapPython PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})

  target_link_libraries(LeapPython ${PYTHON_LIBRARY})
  target_include_directories(LeapPython PUBLIC ${PYTHON_INCLUDE_DIR})

  if(WIN32)
    set_target_properties(LeapPython PROPERTIES COMPILE_FLAGS "-DHAVE_ROUND=1")
  else()
    if(APPLE)
      if(NOT USE_DEBUG_SYMBOLS)
        set_target_properties(LeapPython PROPERTIES LINK_FLAGS_RELEASE "-Xlinker -S -Xlinker -x -Xlinker -exported_symbol -Xlinker _PyInit_LeapPython -Xlinker -dead_strip -Xlinker -dead_strip_dylibs")
      endif()
      # Suppress compiler warnings from auto-generated wrapper file
      set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/Python/LeapPYTHON_wrap.cpp PROPERTY COMPILE_FLAGS "-Wno-parentheses-equality")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT ANDROID)
      target_link_libraries(LeapPython -lutil)
    endif()
  endif()

  if(ANDROID)
    set_target_properties(LeapPython PROPERTIES COMPILE_FLAGS "-DLEAP_API_IMPLEMENTATION=1")
    target_link_libraries(LeapPython Autowiring::Autowiring Calibration Networking SystemUtility)
    set_target_properties(LeapPython PROPERTIES LINK_FLAGS_RELEASE "-Xlinker -s")
  endif()
  target_link_libraries(LeapPython LeapC++)
  add_custom_command(TARGET LeapPython POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${CMAKE_CURRENT_BINARY_DIR}/Python/Leap.py
                       ${PROJECT_EXECUTABLE_PATH}/Leap.py)

  list(APPEND SDK_COMPONENTS LeapPython)
endif()
