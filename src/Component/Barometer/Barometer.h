#pragma once

#include "../../Driver/Twi/TwiInterface.h"
#include "BarometerInterface.h"
#include "math.h"

namespace Component
{
    namespace Barometer
    {
        static constexpr uint8_t MS5611_I2C_ADDRESS = 0x77U;

        static constexpr uint8_t MS5611_CMD_ADC_READ = 0x00U;
        static constexpr uint8_t MS5611_CMD_RESET = 0x1EU;
        static constexpr uint8_t MS5611_CMD_CONV_D1 = 0x40U;
        static constexpr uint8_t MS5611_CMD_CONV_D2 = 0x50U;
        static constexpr uint8_t MS5611_CMD_READ_PROM = 0xA2U;


        class Barometer : public BarometerInterface {
        public:
            Barometer(Driver::Twi::TwiInterface &i2c, const uint8_t address = MS5611_I2C_ADDRESS,
                      const BarometerResolution resolution = MS5611_STANDARD);

            ~Barometer() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            inline virtual int16_t GetTemp(void) final override {
                return (this->mTemp);
            }

            inline virtual int32_t GetPressure(void) final override {
                return (this->mPressure);
            }

            inline virtual uint16_t GetAltitude(void) final override {
                return (44330.0F * (1.0F - pow(static_cast<float>(this->mPressure) / 101325.0F, 0.1902949F)));
            }

        private:
            void Reset(void) const;

            void ReadProm(void);

            uint32_t ReadRawTemp(void) const;

            uint32_t ReadRawPressure(void) const;

            Driver::Twi::TwiInterface &mI2c;
            uint8_t mAddress;
            BarometerResolution mResolution;
            uint16_t mProm[6U];
            int32_t mRawTemp;
            int32_t mRawPressure;
            int16_t mTemp;
            int32_t mPressure;
            uint16_t mAltitude;
        };
    }
}
