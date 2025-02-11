#pragma once

#include "../../Driver/Twi/TwiInterface.h"
#include "Ads1115Interface.h"

namespace Component
{
    namespace Adc
    {
        static constexpr uint8_t ADS1115_I2C_ADDRESS = 0x48U;

        static constexpr uint8_t ADS_CONF_GAIN = 0x10U;

        static constexpr uint16_t ADS1X15_PGA_6_144V = 0x0000U; //  voltage
        static constexpr uint16_t ADS1X15_PGA_4_096V = 0x0200U; //
        static constexpr uint16_t ADS1X15_PGA_2_048V = 0x0400U; //  default
        static constexpr uint16_t ADS1X15_PGA_1_024V = 0x0600U;
        static constexpr uint16_t ADS1X15_PGA_0_512V = 0x0800U;
        static constexpr uint16_t ADS1X15_PGA_0_256V = 0x0A00U;

        static constexpr uint16_t ADS1X15_MODE_CONTINUE = 0x0000U;
        static constexpr uint16_t ADS1X15_MODE_SINGLE = 0x0100U;
        static constexpr uint16_t ADS1X15_OS_NOT_BUSY = 0x8000U;

        //  REGISTERS
        static constexpr uint8_t ADS1X15_REG_CONVERT = 0x00U;
        static constexpr uint8_t ADS1X15_REG_CONFIG = 0x01U;
        static constexpr uint8_t ADS1X15_REG_LOW_THRESHOLD = 0x02U;
        static constexpr uint8_t ADS1X15_REG_HIGH_THRESHOLD = 0x03U;

        class Ads1115 : public Ads1115Interface {
        public:
            Ads1115(Driver::Twi::TwiInterface &twi, const uint8_t address = ADS1115_I2C_ADDRESS);

            ~Ads1115() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual uint16_t GetVoltage(void) final override;

            virtual uint16_t GetIntensity(void) final override;

        private:
            bool IsReady(void);

            void SetConfig(const Ads1115Pin pin);

            void SetGain(uint8_t gain);

            void SetMode(const uint8_t mode);

            void SetDataRate(const uint8_t dataRate);

            int16_t ReadADC(const Ads1115Pin pin);

        private:
            Driver::Twi::TwiInterface &mTwi;
            uint8_t mAddress;
            uint8_t mConfig;
            uint16_t mGain;
            uint16_t mMode;
            uint8_t mDataRate;
        };
    } // namespace Adc
} // namespace Driver
