#include "Battery.h"


namespace Component
{
    namespace Battery
    {
#define NOMINAL_LEVEL    800U
#define WARNING_LEVEL    750U

        Battery::Battery(Adc::Ads1115Interface &adc) :
            mVoltage(0U)
            , mIntensity(0U)
            , mState(BatteryState::UNKNOWN)
            , mAdc(adc) {
        }

        Core::Status Battery::Initialize(void) {
            LOG_COMPONENT_DEBUG("Battery", "Initialized.");
            return (this->mAdc.Initialize());
        }

        void Battery::Update(const uint64_t currentTime) {
            (void) currentTime;
            this->mVoltage = static_cast<uint16_t>(this->mAdc.ReadADC(Adc::PIN_1) * 0.46F);
            this->mIntensity = static_cast<uint16_t>((this->mAdc.ReadADC(Adc::PIN_0) - 250.0F) * 0.066F);
            const BatteryState prevState = this->mState;
            if (this->mVoltage >= NOMINAL_LEVEL) {
                this->mState = NOMINAL;
            } else if (this->mVoltage >= WARNING_LEVEL) {
                this->mState = WARNING;
            } else {
                this->mState = CRITICAL;
            }
            if (prevState != this->mState) {
                this->Notify({this->mState, this->mVoltage, this->mIntensity});
            }
        }

        BatteryState Battery::GetState(void) {
            return (this->mState);
        }

        uint16_t Battery::GetVoltage(void) {
            return (this->mVoltage);
        }

        uint16_t Battery::GetCurrent(void) {
            return (this->mIntensity);
        }

    }
}
