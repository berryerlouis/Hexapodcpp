cmake_minimum_required(VERSION 3.22)

# Explicitly build all internal libraries as static to avoid runtime .so dependencies.
# This keeps the deployed Hexapodcpp executable self-contained (no libApp.so/libBot.so/etc).
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries as static" FORCE)

# Build information
if (CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling ${PROJECT_NAME} for ${TARGET} in ${CMAKE_BUILD_TYPE} mode")
else ()
    message(STATUS "Native compilation ${PROJECT_NAME} for ${TARGET} in ${CMAKE_BUILD_TYPE} mode")
endif ()

# Platform-specific dependencies
include(FetchContent)

# Some fetched dependencies declare cmake_minimum_required < 3.5, which CMake >= 4.0 rejects.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# TinyWebsockets for RPI
FetchContent_Declare(
        TinyWebsockets
        GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
        GIT_TAG master
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(TinyWebsockets)
# TinyWebsockets forces -Werror; don't fail our build on third-party warnings (e.g. GCC 15 -Wmaybe-uninitialized at -O3)
target_compile_options(tiny_websockets_lib PRIVATE -Wno-error)

if (${TARGET} STREQUAL "X64")
    add_compile_definitions(X64)
    message(STATUS "Configured for X64 platform")

elseif (${TARGET} STREQUAL "RPI")
    add_compile_definitions(RPI)
    message(STATUS "Configured for Raspberry Pi platform")

    # WiringPi for Raspberry Pi
    FetchContent_Declare(
            wiringPi
            GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
            GIT_TAG master
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(wiringPi)

    # pigpio
    FetchContent_Declare(
            pigpio
            GIT_REPOSITORY https://github.com/joan2937/pigpio.git
            GIT_TAG "v79"
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(pigpio)
else ()
    message(FATAL_ERROR "Unknown TARGET: ${TARGET}. Must be X64 or RPI")
endif ()

# Compiler options
add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wno-psabi
        -Wno-cast-align
        -fno-exceptions
        -ffunction-sections
        -fdata-sections
        -fno-threadsafe-statics
)

# Add subdirectories
add_subdirectory(Core)
add_subdirectory(Driver)
add_subdirectory(Misc)
add_subdirectory(Component)
add_subdirectory(Cluster)
add_subdirectory(Service)
add_subdirectory(Bot)
add_subdirectory(Move)
add_subdirectory(App)

# Create executable
add_executable(${PROJECT_NAME} Hexapodcpp.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        App
        Bot
        Cluster
        Component
        Core
        Driver
        Move
        Service
        Misc
)

# Linker options for size optimization
target_link_options(${PROJECT_NAME} PRIVATE
        -Wl,--gc-sections
)
