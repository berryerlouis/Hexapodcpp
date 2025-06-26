#include "Ads1115.h"

#include <unistd.h>
#ifdef RPI
#ifndef GTEST
#include "wiringPi/wiringPiI2C.h"
#endif
#endif

namespace Component
{
    namespace Adc
    {
        Ads1115::Ads1115(Driver::Twi::TwiInterface &twi, const uint8_t address) :
            mTwi(twi)
            , mAddress(address)
            , mGain(GAIN_TWOTHIRDS)
            , mSps(SPS_128) {
#ifdef RPI
#ifndef GTEST
            this->mAddress = wiringPiI2CSetup(address);
#endif
#endif
        }

        Core::Status Ads1115::Initialize(void) {
            LOG_COMPONENT_DEBUG("Ads1115", "address 0x%02X Initialized.", this->mAddress);
            return (Core::Status::CORE_OK);
        }

        void Ads1115::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        bool Ads1115::IsReady(void) const {
            uint8_t buffer[2U];
            uint16_t value = 0U;
            uint8_t timeout = 0U;

            while ((timeout < 100U) && ((value & ADS1115_REG_CONFIG_OS_NOTBUSY) == 0U)) {
                usleep(1U);
                timeout++;
                this->mTwi.ReadRegisters(this->mAddress, ADS1115_REG_POINTER_CONFIG, buffer, 2U);
                value = (static_cast<uint16_t>(buffer[0U]) << 8U) + static_cast<uint16_t>(buffer[1U]);
            }
            return ((value & ADS1115_REG_CONFIG_OS_NOTBUSY) != 0U);
        }

        float Ads1115::ReadADC(const Ads1115Pin pin) const {
            // Start with default values
            uint16_t config = ADS1115_REG_CONFIG_CQUE_1CONV |
                              ADS1115_REG_CONFIG_CLAT_NONLAT |
                              ADS1115_REG_CONFIG_CPOL_ACTVLOW |
                              ADS1115_REG_CONFIG_CMODE_TRAD |
                              ADS1115_REG_CONFIG_MODE_SINGLE |
                              ADS1115_REG_CONFIG_OS_SINGLE;
            config |= mGain;
            config |= mSps;

            switch (pin) {
                case (PIN_0):
                    config |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
                    break;
                case (PIN_1):
                    config |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
                    break;
                case (PIN_2):
                    config |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
                    break;
                case (PIN_3):
                    config |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
                    break;
            }
            uint8_t buffer[2U];

            buffer[0] = config >> 8U;
            buffer[1] = config & 0xFFU;
            this->mTwi.WriteRegisters(this->mAddress, ADS1115_REG_POINTER_CONFIG, buffer, 2U);

            if (this->IsReady() == true) {
                this->mTwi.ReadRegisters(this->mAddress, ADS1115_REG_POINTER_CONVERT, buffer, 2U);
                return ((static_cast<uint16_t>(buffer[0U]) << 8U) + static_cast<uint16_t>(buffer[1U]))
                       * (6144.0F / 32768.0F);
            }
            return 0.0F;
        }

        void Ads1115::SetGain(const Ads1115Gain gain) {
            this->mGain = gain;
        }
    }
}
