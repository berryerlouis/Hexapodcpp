add_library(Gait 
    GaitCycle.cpp
    GaitParams.cpp
    GaitStrategy.cpp
)

target_include_directories(Gait
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Gait
    INTERFACE
        Legs
)