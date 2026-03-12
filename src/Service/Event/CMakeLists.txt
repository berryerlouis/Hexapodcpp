add_library(ServiceEvent 
    EventDispatcher.cpp
)

target_include_directories(ServiceEvent
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)