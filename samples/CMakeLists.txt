add_executable(SampleCxx Sample.cpp)
set_property(TARGET SampleCxx PROPERTY FOLDER "Samples")
target_link_libraries(SampleCxx PUBLIC LeapC++)
if(UNIX)
  target_link_libraries(SampleCxx PRIVATE -lpthread -lm)
endif()
target_compile_features(SampleCxx PRIVATE cxx_std_11)
add_custom_command(TARGET SampleCxx POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PROPERTY:LeapSDK::LeapC,IMPORTED_LOCATION_RELEASE> $<TARGET_FILE_DIR:SampleCxx>)

if(BUILD_SHARED_LIBS)
  add_custom_command(TARGET SampleCxx POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:LeapC++> $<TARGET_FILE_DIR:SampleCxx>)
endif()

if(NOT BUILD_SWIG)
  return()
endif()

#Compile Python Sample
add_custom_target(SamplePy
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_LIST_DIR}/Sample.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/Sample.py
        DEPENDS LeapPython
        WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}"
        VERBATIM
        SOURCES Sample.py)
set_property(TARGET SamplePy PROPERTY FOLDER "Samples")

#Compile Java Sample
add_custom_target(SampleJava
        COMMAND ${Java_JAVAC_EXECUTABLE} -cp "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/*" -d "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}" ${CMAKE_CURRENT_LIST_DIR}/Sample.java
        DEPENDS LeapJava
        VERBATIM
        SOURCES Sample.java)
set_property(TARGET SampleJava PROPERTY FOLDER "Samples")
