#pragma once

#include "TwiInterface.h"

namespace Driver
{
    namespace Twi
    {
        class Twi : public TwiInterface {
        public:
            explicit Twi(const EI2cFreq &freq);

            ~Twi() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            bool         SendCommand(const uint8_t address,
                                     const uint8_t command) final override;

            virtual bool ReadRegister(const uint8_t address,
                                      const uint8_t reg,
                                      uint8_t      &data) final override;

            virtual bool ReadRegister16Bits(const uint8_t address,
                                            const uint8_t reg,
                                            uint16_t     &data) final override;

            virtual bool ReadRegister24Bits(const uint8_t address,
                                            const uint8_t reg,
                                            uint32_t     &data) final override;

            virtual bool ReadRegister32Bits(const uint8_t address,
                                            const uint8_t reg,
                                            uint32_t     &data) final override;

            virtual bool ReadRegisters(const uint8_t address,
                                       const uint8_t reg,
                                       uint8_t      *data,
                                       const uint8_t length) final override;

            virtual bool Write(const uint8_t address,
                               const uint8_t reg) final override;

            virtual bool WriteRegister(const uint8_t address,
                                       const uint8_t reg,
                                       uint8_t       data) final override;

            virtual bool WriteRegister16Bits(const uint8_t address,
                                             const uint8_t reg,
                                             uint16_t     &data) final override;

            virtual bool WriteRegister32Bits(const uint8_t address,
                                             const uint8_t reg,
                                             uint32_t     &data) final override;

            virtual bool WriteRegisters(const uint8_t address,
                                        const uint8_t reg,
                                        uint8_t      *data,
                                        const uint8_t length) final override;

        private:
        };
    } // namespace Twi
} // namespace Driver
