cmake_minimum_required(VERSION 3.22)

if (${CMAKE_CROSSCOMPILING})
    message(INFO " => Compilation for ${TARGET} in ${CMAKE_BUILD_TYPE} mode for ${PROJECT_NAME} using Cross Compiler")
else ()
    message(INFO " => Compilation for ${TARGET} in ${CMAKE_BUILD_TYPE} mode for ${PROJECT_NAME} using native Compiler")
endif ()


if (${TARGET} STREQUAL "X64")
    include(FetchContent)
    FetchContent_Declare(
            TinyWebsockets
            GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(TinyWebsockets)

    # using GCC
    add_compile_definitions(X64)

elseif (${TARGET} STREQUAL "RPI")
    # WiringPi
    include(FetchContent)
    FetchContent_Declare(
            wiringPi
            GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(wiringPi)
    include_directories(${wiringpi_SOURCE_DIR})

    # TinyWebsockets
    FetchContent_Declare(
            TinyWebsockets
            GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(TinyWebsockets)
    add_compile_definitions(RPI)

endif ()

# All source files
set(SRC_FILES_MAIN Hexapodcpp.cpp)
add_subdirectory(App)
add_subdirectory(Bot)
add_subdirectory(Core)
add_subdirectory(Cluster)
add_subdirectory(Component)
add_subdirectory(Driver)
add_subdirectory(Misc)
add_subdirectory(Service)
add_subdirectory(Move)

set(SRC_FILES
        ${SRC_FILES_CORE}
        ${SRC_FILES_MOVE}
        ${SRC_FILES_BUILDER}
        ${SRC_FILES_BOT}
        ${SRC_FILES_CLUSTER}
        ${SRC_FILES_COMPONENT}
        ${SRC_FILES_DRIVER}
        ${SRC_FILES_MAIN}
        ${SRC_FILES_MISC}
        ${SRC_FILES_SERVICES}
)

add_compile_options(-Wall -Wextra -Wno-psabi -Werror -W -Wno-cast-align -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -O0 -g)
add_executable(${PROJECT_NAME} ${SRC_FILES})
if (${TARGET} STREQUAL "RPI")
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            wiringPi
            tiny_websockets_lib
    )
else ()
    target_link_libraries(${PROJECT_NAME} tiny_websockets_lib)
endif ()