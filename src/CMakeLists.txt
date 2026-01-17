cmake_minimum_required(VERSION 3.22)

# Build information
if (CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling ${PROJECT_NAME} for ${TARGET} in ${CMAKE_BUILD_TYPE} mode")
else ()
    message(STATUS "Native compilation ${PROJECT_NAME} for ${TARGET} in ${CMAKE_BUILD_TYPE} mode")
endif ()


# Platform-specific dependencies
include(FetchContent)

if (${TARGET} STREQUAL "X64")
    # TinyWebsockets for X64
    FetchContent_Declare(
            TinyWebsockets
            GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
            GIT_TAG master
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(TinyWebsockets)

    add_compile_definitions(X64)
    message(STATUS "Configured for X64 platform")

elseif (${TARGET} STREQUAL "RPI")
    # WiringPi for Raspberry Pi
    FetchContent_Declare(
            wiringPi
            GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
            GIT_TAG master
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(wiringPi)

    # TinyWebsockets for RPI
    FetchContent_Declare(
            TinyWebsockets
            GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
            GIT_TAG master
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(TinyWebsockets)

    add_compile_definitions(RPI)
    message(STATUS "Configured for Raspberry Pi platform")
else ()
    message(FATAL_ERROR "Unknown TARGET: ${TARGET}. Must be X64 or RPI")
endif ()

# Source files organization
set(SRC_FILES_MAIN Hexapodcpp.cpp)

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

# Collect all source files
set(SRC_FILES
        ${SRC_FILES_MAIN}
        ${SRC_FILES_CORE}
        ${SRC_FILES_DRIVER}
        ${SRC_FILES_MISC}
        ${SRC_FILES_COMPONENT}
        ${SRC_FILES_CLUSTER}
        ${SRC_FILES_SERVICES}
        ${SRC_FILES_BOT}
        ${SRC_FILES_MOVE}
        ${SRC_FILES_BUILDER}
)

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

# Debug symbols
if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_options(-O0 -g3)
else ()
    add_compile_options(-O2)
endif ()

# Create executable
# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        tiny_websockets_lib
)

if (${TARGET} STREQUAL "RPI")
    target_include_directories(${PROJECT_NAME} PRIVATE ${wiringpi_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            wiringPi
    )
endif ()
# Linker options for size optimization
target_link_options(${PROJECT_NAME} PRIVATE
        -Wl,--gc-sections
)