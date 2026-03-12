add_library(Software
    Software.cpp
)

target_include_directories(Software
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)