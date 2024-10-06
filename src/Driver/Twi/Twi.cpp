#include "Twi.h"
#include <util/twi.h>

namespace Driver
{
    namespace Twi
    {
        Twi::Twi(const EI2cFreq &freq)
            : mFreq(freq)
              , mTimeout(0U) {
        }

        Core::CoreStatus Twi::Initialize(void) {
            TWBR = ((F_CPU / this->mFreq) - 16) / 2;
            TWSR = 0U;
            TWCR = (1 << TWEN);

            return (Core::CoreStatus::CORE_OK);
        }

        void Twi::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        bool Twi::SendCommand(const uint8_t address, const uint8_t command) {
            Start(address);
            Write(command);
            Stop();
            return (true);
        }

        bool Twi::ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data) {
            Start(address);
            Write(reg);
            RepeatedStart(address);
            data = ReadNack();
            Stop();
            return (true);
        }

        bool Twi::ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            Start(address);
            Write(reg);
            RepeatedStart(address);
            data = (uint16_t) ((uint16_t) ReadAck() << 8U);
            data |= ReadNack();
            Stop();
            return (true);
        }

        bool Twi::ReadRegister24Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            Start(address);
            Write(reg);
            RepeatedStart(address);
            data = (uint32_t) ((uint32_t) ReadAck() << 16U);
            data |= (uint32_t) ((uint32_t) ReadAck() << 8U);
            data |= ReadNack();
            Stop();
            return (true);
        }

        bool Twi::ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            Start(address);
            Write(reg);
            RepeatedStart(address);
            data = (uint32_t) ((uint32_t) ReadAck() << 24U);
            data |= (uint32_t) ((uint32_t) ReadAck() << 16U);
            data |= (uint32_t) ((uint32_t) ReadAck() << 8U);
            data |= ReadNack();
            Stop();
            return (true);
        }

        bool Twi::ReadRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
            Start(address);
            Write(reg);
            RepeatedStart(address);

            for (size_t i = 0U; i < length - 1U; i++) {
                data[i] = ReadAck();
            }
			data[length - 1U] = ReadNack();
            Stop();
            return (true);
        }

        bool Twi::WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) {
            Start(address);
            Write(reg);
            Write(data);
            Stop();
            return (true);
        }

        bool Twi::WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) {
            Start(address);
            Write(reg);
            Write((uint8_t) (data >> 8U));
            Write(data);
            Stop();
            return (true);
        }

        bool Twi::WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) {
            Start(address);
            Write(reg);
            Write((uint8_t) (data >> 24U));
            Write((uint8_t) (data >> 16U));
            Write((uint8_t) (data >> 8U));
            Write((uint8_t) (data));
            Stop();
            return (true);
        }

        bool Twi::WriteRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length) {
            Start(address);
            Write(reg);

            for (size_t i = 0U; i < length; i++) {
                Write(data[i]);
            }

            Stop();
            return (true);
        }

        Twi::EI2cStatus Twi::Start(const uint8_t slave) {
            uint8_t status;

            TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);

            while (!(TWCR & (1 << TWINT)) && this->mTimeout < 0x1F) {
                this->mTimeout++;
            }
            this->mTimeout = 0U;
            status = TWSR & 0xF8;

            if (status != 0x08) {
                return (EI2cStatus::FAIL_START);
            }
            TWDR = slave << 1U;
            TWCR = (1 << TWEN) | (1 << TWINT);

            while (!(TWCR & (1 << TWINT)) && this->mTimeout < 0x1F) {
                this->mTimeout++;
            }
            this->mTimeout = 0U;
            status = TWSR & 0xF8;

            if (status == 0x18) {
                return (EI2cStatus::ACK);
            }

            if (status == 0x20) {
                return (EI2cStatus::NACK);
            } else {
                return (EI2cStatus::FAIL_ADDR);
            }
        }

        Twi::EI2cStatus Twi::RepeatedStart(const uint8_t slave) {
            uint8_t status;

            TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);

            while (!(TWCR & (1 << TWINT)) && this->mTimeout < 0x1F) {
                this->mTimeout++;
            }
            this->mTimeout = 0U;
            status = TWSR & 0xF8;

            if (status != 0x10) {
                return (EI2cStatus::FAIL_START);
            }
            TWDR = (slave << 1U) + 1U;
            TWCR = (1 << TWEN) | (1 << TWINT);

            while (!(TWCR & (1 << TWINT)) && this->mTimeout < 0x1F) {
                this->mTimeout++;
            }
            this->mTimeout = 0U;
            status = TWSR & 0xF8;

            if (status == 0x40) {
                return (EI2cStatus::ACK);
            }

            if (status == 0x48) {
                return (EI2cStatus::NACK);
            } else {
                return (EI2cStatus::FAIL_ADDR);
            }
        }

        Twi::EI2cStatus Twi::Write(const uint8_t data) {
            uint8_t status;

            TWDR = data;
            TWCR = (1 << TWEN) | (1 << TWINT);

            while (!(TWCR & (1 << TWINT)) && this->mTimeout < 0x1F) {
                this->mTimeout++;
            }
            this->mTimeout = 0U;
            status = TWSR & 0xF8;

            if (status == 0x28) {
                return (EI2cStatus::ACK);
            }

            if (status == 0x30) {
                return (EI2cStatus::NACK);
            } else {
                return (EI2cStatus::FAIL_WRITE);
            }
        }

        uint8_t Twi::ReadAck(void) {
            TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);

            while (!(TWCR & (1 << TWINT)) && this->mTimeout < 0x1F) {
                this->mTimeout++;
            }

            if (this->mTimeout >= 0x1F) {
                this->mTimeout++;
            }
            this->mTimeout = 0U;
            return (TWDR);
        }

        uint8_t Twi::ReadNack(void) {
            TWCR = (1 << TWEN) | (1 << TWINT);

            while (!(TWCR & (1 << TWINT)) && this->mTimeout < 0x1F) {
                this->mTimeout++;
            }

            if (this->mTimeout >= 0x1F) {
                this->mTimeout++;
            }
            this->mTimeout = 0U;
            return (TWDR);
        }

        Twi::EI2cStatus Twi::Stop(void) {
            TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);

            while (TWCR & (1 << TWSTO) && this->mTimeout < 0x1F) {
                this->mTimeout++;
            }
            this->mTimeout = 0U;
            return (EI2cStatus::ACK);
        }
    }
}
