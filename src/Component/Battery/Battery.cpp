#include "Battery.h"

namespace Component
{
    namespace Battery
    {
#define NOMINAL_LEVEL    800U
#define WARNING_LEVEL    750U

        Battery::Battery(Adc::AdcInterface &adc)
            : mVoltage(0U)
              , mState(BatteryState::UNKNOWN)
              , mAdc(adc)
              , mObservable() {
        }

        Core::CoreStatus Battery::Initialize(void) {
            return (this->mAdc.Initialize());
        }

        void Battery::Update(const uint64_t currentTime) {
            (void) currentTime;
            this->mVoltage = this->mAdc.Read();
            const BatteryState state = this->mState;
            if (this->mVoltage >= NOMINAL_LEVEL) {
                this->mState = NOMINAL;
            } else if (this->mVoltage >= WARNING_LEVEL) {
                this->mState = WARNING;
            } else {
                this->mState = CRITICAL;
            }
            if (state != this->mState) {
                this->Notify(this->mState, this->mVoltage);
            }
        }

        BatteryState Battery::GetState(void) {
            return (this->mState);
        }

        uint16_t Battery::GetVoltage(void) {
            return (this->mVoltage);
        }

        Core::CoreStatus Battery::Attach(BatteryObserverInterface *observer) {
            return (this->mObservable.Attach(observer));
        }

        void Battery::Notify(const BatteryState &state, const uint16_t voltage) {
            this->mObservable.Notify(state, voltage);
        }
    }
}
