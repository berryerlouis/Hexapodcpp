#pragma once

#include "../ComponentInterface.h"
#include "../Servo/Servo.h"
#include "Constants.h"

namespace Component
{
    namespace Servos
    {
        class ServosInterface : public ComponentInterface<0U, std::nullptr_t> {
        public:
            ServosInterface() = default;

            ~ServosInterface() = default;

            virtual void                   Enable(void) = 0;

            virtual void                   Disable(void) = 0;

            virtual bool                   GetState(void) = 0;

            virtual Servo::ServoInterface *GetServo(const EServos servoId) = 0;

            virtual ServosController::Pca9685Interface &
            GetServosController(const uint8_t pca9685Id) = 0;
        };
    } // namespace Servos
} // namespace Component
