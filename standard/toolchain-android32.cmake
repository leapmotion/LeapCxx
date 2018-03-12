set(CMAKE_ANDROID_ARCH_ABI armeabi)
set(CMAKE_ANDROID_ARM_MODE ON)

#Normally we would use   set(CMAKE_ANDROID_ARM_NEON ON), but it's not generally compatible with armv5te)
set(CMAKE_CXX_FLAGS_INIT "-mfpu=neon")

find_path(CMAKE_ANDROID_STANDALONE_TOOLCHAIN sysroot/usr/lib/libandroid.so PATHS
  /opt/android-standalone-toolchain
)

include(${CMAKE_CURRENT_LIST_DIR}/toolchain-android.cmake)
