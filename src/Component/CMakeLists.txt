cmake_minimum_required(VERSION 3.22)

set(SRC_FILES_COMPONENT
        Component/Barometer/Barometer.cpp
        Component/Battery/Battery.cpp
        Component/Sound/Sound.cpp
        Component/Adc/Ads1115.cpp
        Component/Button/Button.cpp
        Component/Communication/Communication.cpp
        Component/Display/Ssd1306.cpp
        Component/Imu/Ahrs.cpp
        Component/Imu/Mpu9150.cpp
        Component/Led/Led.cpp
        Component/LedPwm/LedPwm.cpp
        Component/Proximity/Laser/Vl53l0x.cpp
        Component/Proximity/SensorProximity.cpp
        Component/Proximity/Ultrasound/Srf05.cpp
        Component/Servo/Servo.cpp
        Component/Servos/Servos.cpp
        Component/ServosController/Pca9685.cpp
        Component/Software/Software.cpp
        PARENT_SCOPE
)