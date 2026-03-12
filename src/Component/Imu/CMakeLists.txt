add_library(Imu
    Ahrs.cpp
    Mpu9150.cpp
)

target_include_directories(Imu
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)