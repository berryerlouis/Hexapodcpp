add_library(App 
    App.cpp
)

target_include_directories(App
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(App
    PUBLIC
        tiny_websockets_lib
)

if (${TARGET} STREQUAL "RPI")
    target_include_directories(App PUBLIC ${wiringpi_SOURCE_DIR})
    target_link_libraries(App
        PUBLIC
            wiringPi
    )
endif ()