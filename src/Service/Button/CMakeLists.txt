add_library(ServiceButton 
    ServiceButton.cpp
)

target_include_directories(ServiceButton
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)