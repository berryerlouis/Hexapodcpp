add_library(ServiceSound 
    ServiceSound.cpp
)

target_include_directories(ServiceSound
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)