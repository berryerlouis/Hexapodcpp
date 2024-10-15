#include "Twi.h"

namespace Driver
{
    namespace Twi
    {
        Twi::Twi(const EI2cFreq &freq)
            : mFreq(freq)
              , mTimeout(0U) {
        }

        Core::CoreStatus Twi::Initialize(void) {
            return (Core::CoreStatus::CORE_OK);
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
            return (true);
        }

        bool Twi::ReadRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
            (void) address;
            (void) reg;
            (void) data;
            (void) length;
            return (true);
        }

        bool Twi::WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) {
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

        bool Twi::WriteRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
            (void) address;
            (void) reg;
            (void) data;
            (void) length;
            return (true);
        }

        Twi::EI2cStatus Twi::Start(const uint8_t slave) {
            (void) slave;
            return (EI2cStatus::ACK);
        }

        Twi::EI2cStatus Twi::RepeatedStart(const uint8_t slave) {
            (void) slave;
            return (EI2cStatus::ACK);
        }

        Twi::EI2cStatus Twi::Write(const uint8_t data) {
            (void) data;
            return (EI2cStatus::ACK);
        }

        uint8_t Twi::ReadAck(void) {
            return (0U);
        }

        uint8_t Twi::ReadNack(void) {
            return (0U);
        }

        Twi::EI2cStatus Twi::Stop(void) {
            return (EI2cStatus::ACK);
        }
    }
}
