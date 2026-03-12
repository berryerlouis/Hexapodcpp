add_library(ServiceMessage
    MessageListener.cpp
)

target_include_directories(ServiceMessage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)