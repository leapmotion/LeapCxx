add_executable(Sample Sample.cpp)
set_property(TARGET Sample PROPERTY FOLDER "Samples")
target_link_libraries(Sample PUBLIC LeapC++)
target_compile_features(Sample PRIVATE cxx_std_11)

if(NOT BUILD_SWIG)
  return()
endif()

#TODO: Compile the python & Java samples if possible
