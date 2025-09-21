#pragma once

#include <unordered_map>

#include "ServosInterface.h"
#include "../../Driver/Gpio/GpioInterface.h"

namespace Component
{
    namespace Servos
    {
        using namespace Driver;

        class Servos : public ServosInterface {
        public:
            Servos(ServosController::Pca9685Interface &pca9685_0
                   , ServosController::Pca9685Interface &pca9685_1
                   , Gpio::GpioInterface &enablePwmPin);


            ~Servos() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Enable(void) final override;

            virtual void Disable(void) final override;

            virtual bool GetState(void) final override;

            virtual Servo::Servo *GetServo(const EServos servoId) final override;

            virtual ServosController::Pca9685Interface &GetServosController(const uint8_t pca9685Id) final override;

        private:
            std::unordered_map<EServos, Servo::Servo> mServos;
            ServosController::Pca9685Interface &mPca9685Left;
            ServosController::Pca9685Interface &mPca9685Right;
            Gpio::GpioInterface &mEnablePwmPin;
        };
    }
}
