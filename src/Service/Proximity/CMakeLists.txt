add_library(ServiceProximity 
    ServiceProximity.cpp
)

target_include_directories(ServiceProximity
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)