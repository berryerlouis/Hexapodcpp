add_library(ServiceBattery 
    ServiceBattery.cpp
)

target_include_directories(ServiceBattery
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)