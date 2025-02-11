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
#define ADS_CONF_CHAN_4  0x01U
#define ADS_CONF_RES_16  0x04U
#define ADS_CONF_NOCOMP  0x00U
#define ADS_CONF_COMP    0x20U

        Ads1115::Ads1115(Driver::Twi::TwiInterface &twi, const uint8_t address) :
            mTwi(twi)
            , mAddress(address)
            , mConfig(ADS_CONF_COMP | ADS_CONF_GAIN | ADS_CONF_RES_16 | ADS_CONF_CHAN_4)
            , mGain(0U)
            , mMode(0U)
            , mDataRate(0U) {
#ifdef RPI
#ifndef GTEST
            this->mAddress = wiringPiI2CSetup(address);
#endif
#endif
        }

        Core::Status Ads1115::Initialize(void) {
            this->SetGain(0U);
            this->SetMode(ADS1X15_MODE_CONTINUE);
            this->SetDataRate(7U);
            return (Core::Status::CORE_OK);
        }

        void Ads1115::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        uint16_t Ads1115::GetVoltage(void) {
            return this->ReadADC(PIN_0);
        }

        uint16_t Ads1115::GetIntensity(void) {
            return this->ReadADC(PIN_1);
        }

        bool Ads1115::IsReady(void) const {
            uint16_t value = 0U;
            uint8_t timeout = 0U;

            while ((timeout < 100U) && (value & ADS1X15_OS_NOT_BUSY) == 0U) {
                usleep(10U);
                timeout++;
                this->mTwi.ReadRegister16Bits(this->mAddress, ADS1X15_REG_CONVERT, value);
            }
            return ((value & ADS1X15_OS_NOT_BUSY) > 0U);
        }

        void Ads1115::SetConfig(const Ads1115Pin pin) const {
            const uint16_t mode = (4U + pin) << 12U;
            uint16_t config = 0x8000U; //  bit 15     force wake up if needed
            config |= mode; //  bit 12-14
            config |= this->mGain; //  bit 9-11
            config |= this->mMode; //  bit 8
            config |= this->mDataRate; //  bit 5-7
            config |= 0x0008; //  bit 3      ALERT active value
            config |= 0x0000; //  bit 2      ALERT latching
            config |= 3U; //  bit 0..1   ALERT mode

            this->mTwi.WriteRegister16Bits(this->mAddress, ADS1X15_REG_CONFIG, config);
        }

        int16_t Ads1115::ReadADC(const Ads1115Pin pin) const {
            this->SetConfig(pin);
            //delay 8
            const bool ready = this->IsReady();

            uint16_t raw = 0U;
            if (true == ready) {
                this->mTwi.ReadRegister16Bits(this->mAddress, ADS1X15_REG_CONVERT, raw);
            }
            return raw;
        }

        void Ads1115::SetGain(uint8_t gain) {
            if (!(this->mConfig & ADS_CONF_GAIN)) {
                gain = 0U;
            }
            switch (gain) {
                default: //  catch invalid values and go for the safest gain.
                case 0U:
                    this->mGain = ADS1X15_PGA_6_144V;
                    break;
                case 1U:
                    this->mGain = ADS1X15_PGA_4_096V;
                    break;
                case 2U:
                    this->mGain = ADS1X15_PGA_2_048V;
                    break;
                case 4U:
                    this->mGain = ADS1X15_PGA_1_024V;
                    break;
                case 8U:
                    this->mGain = ADS1X15_PGA_0_512V;
                    break;
                case 16U:
                    this->mGain = ADS1X15_PGA_0_256V;
                    break;
            }
        }

        void Ads1115::SetMode(const uint8_t mode) {
            switch (mode) {
                case 0U:
                    this->mMode = ADS1X15_MODE_CONTINUE;
                    break;
                default: //  catch invalid modi
                case 1U:
                    this->mMode = ADS1X15_MODE_SINGLE;
                    break;
            }
        }

        void Ads1115::SetDataRate(const uint8_t dataRate) {
            this->mDataRate = dataRate;
            if (this->mDataRate > 7U)
                this->mDataRate = 4U;
            this->mDataRate <<= 5U;
        }
    }
}
