cmake_minimum_required(VERSION 3.22)
add_library(DriverSocket 
    Socket.cpp
)

target_include_directories(DriverSocket
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${TinyWebsockets_SOURCE_DIR}
)

target_link_libraries(DriverSocket
    PUBLIC
        tiny_websockets_lib
)