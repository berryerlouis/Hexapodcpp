#include "Srf05.h"

namespace Component
{
    namespace Proximity
    {
        namespace Ultrasound
        {
            Srf05::Srf05(const Cluster::EProximityCommands side, Driver::Gpio::GpioInterface &gpioTrigger,
                         Driver::InputCapture::InputCaptureInterface &gpioEcho, Led::LedInterface &led,
                         Driver::Tick::TickInterface &tick)
                : mSide(side)
                  , mGpioTrigger(gpioTrigger)
                  , mGpioEcho(gpioEcho)
                  , mTick(tick)
                  , mLed(led)
                  , mThreshold(DISTANCE_THRESHOLD) {
            }

            Core::CoreStatus Srf05::Initialize(void) {
                this->mGpioEcho.Initialize();
                this->mLed.Initialize();
                return (Core::CoreStatus::CORE_OK);
            }

            void Srf05::Update(const uint64_t currentTime) {
                (void) currentTime;
                this->SendPulse();
                const uint16_t distance = this->GetDistance();
                const bool detection = (distance != 0U && distance <= this->mThreshold);
                if (true == detection) {
                    this->mLed.On();
                    this->Notify(static_cast<SensorsId>(this->mSide), distance);
                } else {
                    this->mLed.Off();
                }
            }

            uint16_t Srf05::GetThreshold(void) {
                return (this->mThreshold);
            }

            Core::CoreStatus Srf05::SetThreshold(const uint16_t threshold) {
                this->mThreshold = threshold;
                return (Core::CoreStatus::CORE_OK);
            }

            void Srf05::SendPulse(void) const {
                this->mGpioTrigger.Set();
                this->mTick.DelayUs(10U);
                this->mGpioTrigger.Reset();
            }

            uint16_t Srf05::GetDistance(void) {
                return static_cast<uint16_t>((this->mGpioEcho.GetInputCaptureTime() / 58.0F));
            }
        }
    }
}
