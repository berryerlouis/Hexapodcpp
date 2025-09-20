#include "Srf05.h"

namespace Component
{
    namespace Proximity
    {
        namespace Ultrasound
        {

            Srf05::Srf05(const Cluster::EProximityCommands side, Driver::Gpio::GpioInterface &gpioTrigger,
                         Driver::InputCapture::InputCaptureInterface &gpioEcho, Led::LedInterface &led) :
                mSide(side)
                , mGpioTrigger(gpioTrigger)
                , mGpioEcho(gpioEcho)
                , mLed(led)
                , mThreshold(DISTANCE_THRESHOLD) {
            }

            Core::Status Srf05::Initialize(void) {
                this->mGpioEcho.Initialize();
                this->mLed.Initialize();
                LOG_COMPONENT_DEBUG("Ultrasound", "pin trigger %d Initialized.", this->mGpioTrigger.GetPin().pin);
                return (Core::Status::CORE_OK);
            }

            void Srf05::Update(const uint64_t currentTime) {
                (void) currentTime;
                this->SendPulse();
                const uint16_t distance = this->GetDistance();
                const bool detection = (distance != 0U && distance <= this->mThreshold);
                if (true == detection) {
                    this->mLed.On();
                    this->Notify({static_cast<SensorsId>(this->mSide), distance});
                } else {
                    this->mLed.Off();
                }
            }

            uint16_t Srf05::GetThreshold(void) {
                return (this->mThreshold);
            }

            Core::Status Srf05::SetThreshold(const uint16_t threshold) {
                this->mThreshold = threshold;
                return (Core::Status::CORE_OK);
            }

            void Srf05::SendPulse(void) const {
                this->mGpioTrigger.Set();
                Driver::Tick::Tick::GetInstance().DelayUs(10U);
                this->mGpioTrigger.Reset();
            }

            uint16_t Srf05::GetDistance(void) {
                const uint64_t delay = this->mGpioEcho.GetInputCaptureTime();
                return static_cast<uint16_t>(delay / 58.0F);
            }
        }
    }
}
