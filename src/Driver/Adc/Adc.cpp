#include "Adc.h"
#include "../Isr.h"

namespace Driver
{
    namespace Adc
    {
        volatile uint16_t Adc::sAdcValue = 0U;

        Adc::Adc(Gpio::GpioInterface &gpio)
            : mGpio(gpio) {
        }

        Core::CoreStatus Adc::Initialize(void) {
            ADMUX = _BV(REFS0) | _BV(REFS1);
            ADCSRA = _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADIE);
            ADMUX = ((ADMUX & 0xE0U) | static_cast<uint8_t>(this->mGpio.GetPin()));

            this->StartConversion();
            return (Core::CoreStatus::CORE_OK);
        }

        void Adc::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Adc::StartConversion(void) {
            ADCSRA |= _BV(ADSC);
        }

        uint16_t Adc::Read() {
            this->StartConversion();
            return static_cast<uint16_t>(100.0 * ADC_VOLT(Adc::sAdcValue / BRIDGE_DIVIDER));
        }

        ISR(ADC_vect) {
            ISR_EMBEDDED_CODE(Adc::sAdcValue = ADC;);
        }
    }
}
