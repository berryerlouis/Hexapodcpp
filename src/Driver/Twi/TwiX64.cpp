#include "Twi.h"


namespace Driver
{
    namespace Twi
    {
        Twi::Twi(const EI2cFreq &freq) {
            (void) freq;
        }

        Core::Status Twi::Initialize() {
            LOG_DRIVER_DEBUG("Twi", "Initialized.");
            return (Core::Status::CORE_OK);
        }

        void Twi::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        bool Twi::SendCommand(const uint8_t address, const uint8_t command) {
            (void) address;
            (void) command;
            return (true);
        }


        bool Twi::ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data) {
            (void) address;
            (void) reg;
            (void) data;
            return (true);
        }
        bool Twi::Read16Register(const uint8_t address, const uint16_t reg, uint8_t &data) {
            (void) address;
            (void) reg;
            (void) data;
            return (true);
        }

        bool Twi::ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            (void) address;
            (void) reg;
            (void) data;
            return (true);
        }

        bool Twi::ReadRegister24Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            (void) address;
            (void) reg;
            (void) data;
            return true;
        }

        bool Twi::ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            (void) address;
            (void) reg;
            (void) data;
            return true;
        }

        bool Twi::ReadRegisters(const uint8_t address,
                                const uint8_t reg,
                                uint8_t      *data,
                                const uint8_t length) {
            (void) address;
            (void) reg;
            (void) data;
            (void) length;
            return (true);
        }

        bool Twi::Read16Registers(const uint8_t  address,
                                  const uint16_t reg,
                                  uint8_t       *data,
                                  const uint16_t length) {
            (void) address;
            (void) reg;
            (void) data;
            (void) length;
            return (true);
        }

        bool Twi::Write(const uint8_t address, const uint8_t reg) {
            (void) address;
            (void) reg;
            return (true);
        }

        bool Twi::WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) {
            (void) address;
            (void) reg;
            (void) data;
            return (true);
        }
        bool Twi::Write16Register(const uint8_t address, const uint16_t reg, uint8_t data) {
            (void) address;
            (void) reg;
            (void) data;
            return (true);
        }

        bool Twi::WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            (void) address;
            (void) reg;
            (void) data;
            return (true);
        }

        bool Twi::WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            (void) address;
            (void) reg;
            (void) data;
            return (true);
        }

        bool Twi::WriteRegisters(const uint8_t address,
                                 const uint8_t reg,
                                 uint8_t      *data,
                                 const uint8_t length) {
            (void) address;
            (void) reg;
            (void) data;
            (void) length;
            return (true);
        }
        bool Twi::Write16Registers(const uint8_t  address,
                                   const uint16_t reg,
                                   uint8_t       *data,
                                   const uint16_t length) {
            (void) address;
            (void) reg;
            (void) data;
            (void) length;
            return (true);
        }
    } // namespace Twi
} // namespace Driver
