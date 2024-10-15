#pragma once

#include "TwiInterface.h"

namespace Driver {
    namespace Twi {
        class Twi : public TwiInterface {
        public:
            Twi(const EI2cFreq &freq) {
                (void) freq;
            }

            ~Twi() = default;

            virtual Core::CoreStatus Initialize(void) final override {
                return (Core::CoreStatus::CORE_OK);
            }

            virtual void Update(const uint64_t currentTime) final override {
                (void) currentTime;
            }

            bool SendCommand(const uint8_t address, const uint8_t command) final override {
                (void) address;
                (void) command;
                return (true);
            }

            virtual bool ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data) final override {
                (void) address;
                (void) reg;
                (void) data;
                return true;
            }

            virtual bool ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) final override {
                (void) address;
                (void) reg;
                (void) data;
                return true;
            }

            virtual bool ReadRegister24Bits(const uint8_t address, const uint8_t reg, uint32_t &data) final override {
                (void) address;
                (void) reg;
                (void) data;
                return true;
            }

            virtual bool ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) final override {
                (void) address;
                (void) reg;
                (void) data;
                return true;
            }

            virtual bool ReadRegisters(const uint8_t address, const uint8_t reg, uint8_t *data,
                                       const uint8_t length) final override {
                (void) address;
                (void) reg;
                (void) data;
                (void) length;
                return true;
            }

            virtual bool WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data) final override {
                (void) address;
                (void) reg;
                (void) data;
                return true;
            }

            virtual bool WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data) final override {
                (void) address;
                (void) reg;
                (void) data;
                return true;
            }

            virtual bool WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data) final override {
                (void) address;
                (void) reg;
                (void) data;
                return true;
            }

            virtual bool WriteRegisters(const uint8_t address, const uint8_t reg, uint8_t *data,
                                        const uint8_t length) final override {
                (void) address;
                (void) reg;
                (void) data;
                (void) length;
                return true;
            }

        private:
        };
    }
}
