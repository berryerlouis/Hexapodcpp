add_subdirectory(Body)
add_subdirectory(Leg)
add_subdirectory(Legs)

add_library(Bot INTERFACE)

target_include_directories(Bot
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Bot
    INTERFACE
        Body
        Leg
        Legs
)