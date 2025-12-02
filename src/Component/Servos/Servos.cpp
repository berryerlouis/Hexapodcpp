#include "Servos.h"

namespace Component
{
    namespace Servos
    {
        Servos::Servos(ServosController::Pca9685Interface &pca9685_0, ServosController::Pca9685Interface &pca9685_1,
                       Gpio::GpioInterface &enablePwmPin) :
                                                          mServos{
                                                              {
                                                                  SERVO_0,
                                                                  Servo::Servo(pca9685_0, 0, 90U, 15, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_1,
                                                                  Servo::Servo(pca9685_0, 1, 90U, -4, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_2,
                                                                  Servo::Servo(pca9685_0, 2, 90U, 12, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_3,
                                                                  Servo::Servo(pca9685_0, 3, 90U, 2, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_4,
                                                                  Servo::Servo(pca9685_0, 4, 90U, 0, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_5,
                                                                  Servo::Servo(pca9685_0, 5, 90U, 18, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_6,
                                                                  Servo::Servo(pca9685_0, 6, 90U, 2, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_7,
                                                                  Servo::Servo(pca9685_0, 7, 90U, 0, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_8,
                                                                  Servo::Servo(pca9685_0, 8, 90U, 10, 60U, 120U, false)
                                                              },
                                                              {
                                                                  SERVO_9,
                                                                  Servo::Servo(pca9685_1, 0, 90U, -20, 60U, 120U, true)
                                                              },
                                                              {
                                                                  SERVO_10,
                                                                  Servo::Servo(pca9685_1, 1, 90U, -2, 60U, 120U, true)
                                                              },
                                                              {
                                                                  SERVO_11,
                                                                  Servo::Servo(pca9685_1, 2, 90U, -15, 60U, 120U, true)
                                                              },
                                                              {
                                                                  SERVO_12,
                                                                  Servo::Servo(pca9685_1, 3, 90U, 0, 60U, 120U, true)
                                                              },
                                                              {
                                                                  SERVO_13,
                                                                  Servo::Servo(pca9685_1, 4, 90U, 0, 60U, 120U, true)
                                                              },
                                                              {
                                                                  SERVO_14,
                                                                  Servo::Servo(pca9685_1, 5, 90U, -20, 60U, 120U, true)
                                                              },
                                                              {
                                                                  SERVO_15,
                                                                  Servo::Servo(pca9685_1, 6, 90U, 10, 60U, 120U, true)
                                                              },
                                                              {
                                                                  SERVO_16,
                                                                  Servo::Servo(pca9685_1, 7, 90U, 0, 60U, 120U, true)
                                                              },
                                                              {
                                                                  SERVO_17,
                                                                  Servo::Servo(pca9685_1, 8, 90U, -18, 60U, 120U, true)
                                                              }
                                                          }
                                                          , mPca9685Left(pca9685_0)
                                                          , mPca9685Right(pca9685_1)
                                                          , mEnablePwmPin(enablePwmPin) {
        }

        Core::Status Servos::Initialize(void) {
            uint8_t success = 0UL;
            this->mPca9685Left.Initialize();
            this->mPca9685Right.Initialize();
            this->Disable();
            for (auto &pair: this->mServos) {
                if (pair.second.Initialize() == Core::Status::CORE_OK) {
                    success++;
                } else {
                    break;
                }
            }

            if (success == NB_SERVOS) {
                LOG_COMPONENT_DEBUG("Servos", "Initialized.");
            }
            return ((success == NB_SERVOS) ? Core::Status::CORE_OK : Core::Status::CORE_ERROR);
        }

        void Servos::Update(const uint64_t currentTime) {
            for (auto &pair: this->mServos) {
                pair.second.Update(currentTime);
            }
        }

        void Servos::Enable(void) {
            this->mEnablePwmPin.Reset();
        }

        void Servos::Disable(void) {
            this->mEnablePwmPin.Set();
        }

        bool Servos::GetState(void) {
            return this->mEnablePwmPin.Get() == false;
        }


        Servo::Servo *Servos::GetServo(const EServos servoId) {
            const auto it = mServos.find(servoId);
            if (it != mServos.end()) {
                return &(it->second);
            }
            return nullptr;
        }


        ServosController::Pca9685Interface &Servos::GetServosController(const uint8_t pca9685Id) {
            if (pca9685Id == 0U) {
                return (this->mPca9685Left);
            }
            return (this->mPca9685Right);
        }
    }
}