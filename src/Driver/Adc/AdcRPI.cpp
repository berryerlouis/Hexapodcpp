#include "Adc.h"

namespace Driver
{
    namespace Adc
    {
        Adc::Adc(Gpio::GpioInterface &gpio) {
            (void) gpio;
            LOG_DRIVER_DEBUG("Adc", "Initialized.");
        }

        Core::Status Adc::Initialize(void) {
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
