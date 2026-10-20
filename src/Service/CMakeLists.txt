cmake_minimum_required(VERSION 3.22)
add_subdirectory(Battery)
add_subdirectory(Body)
add_subdirectory(Button)
add_subdirectory(Communication)
add_subdirectory(Control)
add_subdirectory(Display)
add_subdirectory(Event)
add_subdirectory(Gamepad)
add_subdirectory(General)
add_subdirectory(Message)
add_subdirectory(Orientation)
add_subdirectory(Proximity)
add_subdirectory(Services)
add_subdirectory(Sound)

add_library(Service INTERFACE)

target_include_directories(Service
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Service
    INTERFACE
        ServiceBattery
        ServiceBody
        ServiceButton
        ServiceCommunication
        ServiceControl
        ServiceDisplay
        ServiceEvent
        ServiceGamepad
        ServiceGeneral
        ServiceMessage
        ServiceOrientation
        ServiceProximity
        Services
        ServiceSound
)