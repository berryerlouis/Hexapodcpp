#pragma once

#include "ServosInterface.h"
#include "../../Driver/Tick/TickInterface.h"

namespace Component
{
    namespace Servos
    {
        using namespace Driver;

        class Servos : public ServosInterface {
        public:
            Servos(ServosController::Pca9685Interface &pca9685_0, ServosController::Pca9685Interface &pca9685_1,
                   Tick::TickInterface &tick);

            ~Servos() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual Servo::Servo &GetServo(const uint8_t servoId) final override;

            virtual ServosController::Pca9685Interface &GetServosController(const uint8_t pca9685Id) final override;

        private:
            Servo::Servo mServo0;
            Servo::Servo mServo1;
            Servo::Servo mServo2;
            Servo::Servo mServo3;
            Servo::Servo mServo4;
            Servo::Servo mServo5;
            Servo::Servo mServo6;
            Servo::Servo mServo7;
            Servo::Servo mServo8;
            Servo::Servo mServo9;
            Servo::Servo mServo10;
            Servo::Servo mServo11;
            Servo::Servo mServo12;
            Servo::Servo mServo13;
            Servo::Servo mServo14;
            Servo::Servo mServo15;
            Servo::Servo mServo16;
            Servo::Servo mServo17;
            Servo::Servo *mServos[NB_SERVOS];
            ServosController::Pca9685Interface &mPca9685Left;
            ServosController::Pca9685Interface &mPca9685Right;
        };
    }
}
