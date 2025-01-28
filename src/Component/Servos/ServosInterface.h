#pragma once

#include "../ComponentInterface.h"
#include "../Servo/Servo.h"
#include "Constants.h"

namespace Component
{
    namespace Servos
    {
        class ServosInterface : public ComponentInterface {
        public:
            ServosInterface() = default;

            ~ServosInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual void Enable(void) = 0;

            virtual void Disable(void) = 0;

            virtual bool GetState(void) = 0;

            virtual Servo::Servo &GetServo(const uint8_t servoId) = 0;

            virtual ServosController::Pca9685Interface &GetServosController(const uint8_t pca9685Id) = 0;
        };
    }
}
