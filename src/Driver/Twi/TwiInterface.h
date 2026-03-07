#pragma once

#include "../DriverInterface.h"
#include "Constants.h"

namespace Driver
{
    namespace Twi
    {
        class TwiInterface : public DriverInterface {
        public:
            virtual bool SendCommand(const uint8_t address, const uint8_t command) = 0;

            virtual bool ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data) = 0;

            virtual bool
            ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) = 0;

            virtual bool
            ReadRegister24Bits(const uint8_t address, const uint8_t reg, uint32_t &data) = 0;

            virtual bool
            ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) = 0;

            virtual bool ReadRegisters(const uint8_t address,
                                       const uint8_t reg,
                                       uint8_t      *data,
                                       const uint8_t length) = 0;

            virtual bool Write(const uint8_t address, const uint8_t reg) = 0;

            virtual bool WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) = 0;

            virtual bool
            WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) = 0;

            virtual bool
            WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) = 0;

            virtual bool WriteRegisters(const uint8_t address,
                                        const uint8_t reg,
                                        uint8_t      *data,
                                        const uint8_t length) = 0;
        };
    } // namespace Twi
} // namespace Driver
