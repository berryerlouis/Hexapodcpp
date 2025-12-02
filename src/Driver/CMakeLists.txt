# Driver module source files (platform-specific)
set(SRC_FILES_DRIVER
        Driver/Adc/Adc${TARGET}.cpp
        Driver/Gpio/Gpio${TARGET}.cpp
        Driver/InputCapture/InputCapture${TARGET}.cpp
        Driver/Socket/Socket${TARGET}.cpp
        Driver/Tick/Tick${TARGET}.cpp
        Driver/Twi/Twi${TARGET}.cpp
        Driver/Uart/Uart${TARGET}.cpp
        PARENT_SCOPE
)