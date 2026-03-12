add_library(Servos
    Servos.cpp
)

target_include_directories(Servos
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Servos
    PUBLIC
        Servo
)