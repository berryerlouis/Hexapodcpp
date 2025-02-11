#include "Twi.h"

#include <string.h>
#include "wiringPi/wiringPiI2C.h"

namespace Driver
{
    namespace Twi
    {
        Twi::Twi(const EI2cFreq &freq) {
            (void) freq;
        }

        Core::Status Twi::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void Twi::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        bool Twi::SendCommand(const uint8_t address, const uint8_t command) {
            wiringPiI2CWrite(address, command);
            return (true);
        }


        bool Twi::ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data) {
            data = wiringPiI2CReadReg8(address, reg);
            return (true);
        }

        bool Twi::ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            data = wiringPiI2CReadReg16(address, reg);
            return (true);
        }

        bool Twi::ReadRegister24Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            data = wiringPiI2CReadReg16(address, reg);
            data |= (wiringPiI2CReadReg8(address, reg + 2U) << 16U);
            data &= 0xFFFFFFU;
            return true;
        }

        bool Twi::ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            data = wiringPiI2CReadReg16(address, reg);
            data |= (wiringPiI2CReadReg16(address, reg + 2U) << 16U);
            return true;
        }

        bool Twi::ReadRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
            wiringPiI2CReadBlockData(address, reg, data, length);
            return (true);
        }

        bool Twi::WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) {
            wiringPiI2CWriteReg8(address, reg, data);
            return (true);
        }

        bool Twi::WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            wiringPiI2CWriteReg16(address, reg, data);
            return (true);
        }

        bool Twi::WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            wiringPiI2CWriteReg16(address, reg, (data & 0x0000FFFF));
            wiringPiI2CWriteReg16(address, reg, (data & 0xFFFF0000) >> 16U);
            return (true);
        }

        bool Twi::WriteRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
            uint8_t buffer[length + 1U] = {reg};
            memcpy(&buffer[1U], data, length);
            wiringPiI2CRawWrite(address, buffer, length + 1U);
            return (true);
        }

    } // namespace Twi
} // namespace Driver
