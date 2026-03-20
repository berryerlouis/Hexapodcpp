cmake_minimum_required(VERSION 3.22)
add_library(ServiceDisplay 
    ServiceDisplay.cpp
)

target_include_directories(ServiceDisplay
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ServiceDisplay
    PUBLIC
        Misc
)