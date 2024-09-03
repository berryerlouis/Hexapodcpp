#pragma once

#include "../ComponentInterface.h"
#include "../Servo/Servo.h"

namespace Component {
namespace Servos {
class ServosInterface : public ComponentInterface {
public:

    static const uint8_t NB_SERVOS = 18U;
    ServosInterface()  = default;
    ~ServosInterface() = default;

    virtual Core::CoreStatus Initialize( void )       = 0;
    virtual void Update( const uint64_t currentTime ) = 0;

    virtual Servo::Servo &GetServo( const uint8_t servoId ) = 0;
    virtual ServosController::Pca9685Interface &GetServosController( const uint8_t pca9685Id ) = 0;
};
}
}
