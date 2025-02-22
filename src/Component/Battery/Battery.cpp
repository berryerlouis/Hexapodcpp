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
            return (this->mAdc.Initialize());
        }

        void Battery::Update(const uint64_t currentTime) {
            (void) currentTime;
            this->mVoltage = this->mAdc.GetVoltage();
            this->mIntensity = this->mAdc.GetIntensity();
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
    }
}
