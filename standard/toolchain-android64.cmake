set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)

find_path(CMAKE_ANDROID_STANDALONE_TOOLCHAIN sysroot/usr/lib/libandroid.so PATHS
  /opt/android-standalone-toolchain-4.9-aarch64
)

include(${CMAKE_CURRENT_LIST_DIR}/toolchain-android.cmake)
