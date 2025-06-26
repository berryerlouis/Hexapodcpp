#include "Adc.h"

namespace Driver
{
    namespace Adc
    {
        Adc::Adc(const Gpio::GpioInterface &gpio) {
            (void) gpio;
        }

        Core::Status Adc::Initialize(void) {
            LOG_DRIVER_DEBUG("Adc", "Initialized.");
            return (Core::Status::CORE_OK);
        }

        void Adc::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Adc::StartConversion(void) {
        }

        uint16_t Adc::Read() {
            return 0U;
        }
    }
}
