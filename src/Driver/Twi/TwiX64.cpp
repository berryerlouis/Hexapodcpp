#include "TwiX64.h"
#ifdef RPI
#include "wiringPi/wiringPiI2C.h"
#endif

namespace Driver
{
    namespace Twi
    {
        Twi::Twi(const EI2cFreq &freq) {
        }

        Core::Status Twi::Initialize(void) { return (Core::Status::CORE_OK); }

        void Twi::Update(const uint64_t currentTime) { (void) currentTime; }

        bool Twi::SendCommand(const uint8_t address, const uint8_t command) {
#ifdef RPI
            wiringPiI2CWrite(address, command);
#endif
            return (true);
        }


        bool Twi::ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data) {
#ifdef RPI
            data = wiringPiI2CReadReg8(address, reg);
#endif
            return (true);
        }

        bool Twi::ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
#ifdef RPI
            data = wiringPiI2CReadReg16(address, reg);
#endif
            return (true);
        }

        bool Twi::ReadRegister24Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
#ifdef RPI
            data = wiringPiI2CReadReg16(address, reg);
            data |= (wiringPiI2CReadReg8(address, reg + 2U) << 16U);
            data &= 0xFFFFFFU;
#endif
            return true;
        }

        bool Twi::ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
#ifdef RPI
            data = wiringPiI2CReadReg16(address, reg);
            data |= (wiringPiI2CReadReg16(address, reg + 2U) << 16U);
#endif
            return true;
        }

        bool Twi::ReadRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
#ifdef RPI
            wiringPiI2CReadBlockData(address, reg, data, length);
#endif
            return (true);
        }

        bool Twi::WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) {
#ifdef RPI
            wiringPiI2CWriteReg8(address, reg, data);
#endif
            return (true);
        }

        bool Twi::WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
#ifdef RPI
            wiringPiI2CWriteReg16(address, reg, data);
#endif
            return (true);
        }

        bool Twi::WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
#ifdef RPI
            wiringPiI2CWriteReg16(address, reg, (data & 0x0000FFFF));
            wiringPiI2CWriteReg16(address, reg, (data & 0xFFFF0000) >> 16U);
#endif
            return (true);
        }

        bool Twi::WriteRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
#ifdef RPI
            wiringPiI2CWriteBlockData(address, reg, data, length);
#endif
            return (true);
        }

    } // namespace Twi
} // namespace Driver
