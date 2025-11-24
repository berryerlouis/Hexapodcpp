

# cmake/toolchain-rpi-zero-2-w.cmake (key parts)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Prefer target paths during find_* calls (include multiarch path for libraries)
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu /usr)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Additional library search paths for cross-compiled dependencies
set(CMAKE_LIBRARY_PATH /usr/lib/aarch64-linux-gnu /usr/aarch64-linux-gnu/lib)

