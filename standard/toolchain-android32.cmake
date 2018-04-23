if(NOT DEFINED CMAKE_ANDROID_ARCH_ABI)
  set(CMAKE_ANDROID_ARCH_ABI armeabi)
endif()
set(CMAKE_ANDROID_ARM_MODE ON)

find_path(CMAKE_ANDROID_STANDALONE_TOOLCHAIN sysroot/usr/lib/libandroid.so PATHS
  /opt/android-standalone-toolchain
)

include(${CMAKE_CURRENT_LIST_DIR}/toolchain-android.cmake)
