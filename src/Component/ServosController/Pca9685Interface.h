#pragma once

#include "../ComponentInterface.h"

namespace Component
{
    namespace ServosController
    {
        class Pca9685Interface : public ComponentInterface<0U, std::nullptr_t> {
        public:
            Pca9685Interface(void) = default;

            ~Pca9685Interface() = default;

            virtual void Reset(void) = 0;

            virtual void Sleep(void) = 0;

            virtual void WakeUp(void) = 0;

            virtual void setOscillatorFrequency(const uint32_t frequency) = 0;

            virtual void SetFrequency(const uint16_t frequency) = 0;

            virtual void SetPwm(const uint8_t num, const uint16_t off) = 0;

            virtual uint8_t GetAddress(void) = 0;
        };
    } // namespace ServosController
} // namespace Component
