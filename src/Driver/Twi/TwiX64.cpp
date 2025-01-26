#include "Twi.h"


namespace Driver
{
    namespace Twi
    {
        Twi::Twi(const EI2cFreq &freq) {
        }

        Core::Status Twi::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void Twi::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        bool Twi::SendCommand(const uint8_t address, const uint8_t command) {
            return (true);
        }


        bool Twi::ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data) {
            return (true);
        }

        bool Twi::ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            return (true);
        }

        bool Twi::ReadRegister24Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            return true;
        }

        bool Twi::ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            return true;
        }

        bool Twi::ReadRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
            return (true);
        }

        bool Twi::WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) {
            return (true);
        }

        bool Twi::WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            return (true);
        }

        bool Twi::WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            return (true);
        }

        bool Twi::WriteRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
            return (true);
        }

    } // namespace Twi
} // namespace Driver
