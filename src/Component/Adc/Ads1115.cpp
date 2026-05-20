#include "Ads1115.h"

#include <unistd.h>

namespace Component
{
    namespace Adc
    {
        static uint32_t GetConversionDelayUs(const Ads1115SPS sps) {
            switch (sps) {
                case SPS_8:
                    return 128000U;
                case SPS_16:
                    return ADS1115_CONVERSION_DELAY_16;
                case SPS_32:
                    return ADS1115_CONVERSION_DELAY_32;
                case SPS_64:
                    return ADS1115_CONVERSION_DELAY_64;
                case SPS_128:
                    return ADS1115_CONVERSION_DELAY_128;
                case SPS_250:
                    return ADS1115_CONVERSION_DELAY_250;
                case SPS_475:
                    return ADS1115_CONVERSION_DELAY_475;
                case SPS_860:
                    return ADS1115_CONVERSION_DELAY_860;
                default:
                    return ADS1115_CONVERSION_DELAY_128;
            }
        }

        Ads1115::Ads1115(Driver::Twi::TwiInterface &twi, const uint8_t address)
            : mTwi(twi)
            , mAddress(address)
            , mGain(GAIN_TWOTHIRDS)
            , mSps(SPS_128) {
        }

        Core::Status Ads1115::Initialize() {
            LOG_COMPONENT_DEBUG("Ads1115", "address 0x%02X Initialized.", this->mAddress);
            return (Core::Status::CORE_OK);
        }

        void Ads1115::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        bool Ads1115::IsReady() const {
            uint8_t                  buffer[2U];

            static constexpr uint8_t kPollCount = 12U;
            const uint32_t           conversionDelayUs = GetConversionDelayUs(this->mSps);
            uint32_t                 pollDelayUs = conversionDelayUs / kPollCount;

            if (pollDelayUs < 200U) {
                pollDelayUs = 200U;
            }

            for (uint8_t timeout = 0U; timeout < kPollCount; timeout++) {
                uint16_t value = 0U;
                this->mTwi.ReadRegisters(this->mAddress, ADS1115_REG_POINTER_CONFIG, buffer, 2U);
                value = (static_cast<uint16_t>(buffer[0U]) << 8U) +
                        static_cast<uint16_t>(buffer[1U]);

                if ((value & ADS1115_REG_CONFIG_OS_NOTBUSY) != 0U) {
                    return true;
                }

                usleep(pollDelayUs);
            }
            return false;
        }

        float Ads1115::ReadADC(const Ads1115Pin pin) const {
            // Start with default values
            uint16_t config = ADS1115_REG_CONFIG_CQUE_1CONV | ADS1115_REG_CONFIG_CLAT_NONLAT |
                              ADS1115_REG_CONFIG_CPOL_ACTVLOW | ADS1115_REG_CONFIG_CMODE_TRAD |
                              ADS1115_REG_CONFIG_MODE_SINGLE | ADS1115_REG_CONFIG_OS_SINGLE;
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

            buffer[0U] = config >> 8U;
            buffer[1U] = config & 0xFFU;
            this->mTwi.WriteRegisters(this->mAddress, ADS1115_REG_POINTER_CONFIG, buffer, 2U);

            if (this->IsReady()) {
                this->mTwi.ReadRegisters(this->mAddress, ADS1115_REG_POINTER_CONVERT, buffer, 2U);
                return ((static_cast<uint16_t>(buffer[0U]) << 8U) +
                        static_cast<uint16_t>(buffer[1U])) *
                       (6144.0F / 32768.0F);
            }
            return 0.0F;
        }
    } // namespace Adc
} // namespace Component
