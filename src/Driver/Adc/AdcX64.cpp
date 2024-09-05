#include "Adc.h"

namespace Driver
{
    namespace Adc
    {
        volatile uint16_t Adc::sAdcValue = 0U;

        Adc::Adc(Gpio::GpioInterface &gpio)
            : mGpio(gpio) {
        }

        Core::CoreStatus Adc::Initialize(void) {
            return (Core::CoreStatus::CORE_OK);
        }

        void Adc::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Adc::StartConversion(void) {
        }

        uint16_t Adc::Read() {
            this->StartConversion();
            return 0U;
        }
    }
}
