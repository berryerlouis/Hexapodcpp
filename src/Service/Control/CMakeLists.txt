add_library(ServiceControl 
    ServiceControl.cpp
)

target_include_directories(ServiceControl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)