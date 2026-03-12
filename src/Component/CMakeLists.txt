add_subdirectory(Adc)
add_subdirectory(Barometer)
add_subdirectory(Battery)
add_subdirectory(Button)
add_subdirectory(Communication)
add_subdirectory(Display)
add_subdirectory(Imu)
add_subdirectory(Led)
add_subdirectory(LedPwm)
add_subdirectory(Proximity)
add_subdirectory(Servo)
add_subdirectory(Servos)
add_subdirectory(ServosController)
add_subdirectory(Software)
add_subdirectory(Sound)

add_library(Component INTERFACE)

target_include_directories(Component 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Component 
    INTERFACE
        Adc
        Barometer
        Battery
        Button
        Communication
        Display
        Imu
        Led
        LedPwm
        Proximity
        Servo
        Servos
        ServosController
        Software
        Sound          
)