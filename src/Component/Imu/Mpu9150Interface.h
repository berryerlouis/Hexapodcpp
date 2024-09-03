#pragma once

#include "../ComponentInterface.h"
#include "../../Misc/Geometry/Geometry.h"

namespace Component {
namespace Imu {
using namespace Misc::Geometry;

class Mpu9150Interface : public ComponentInterface
{
public:
    Mpu9150Interface( void ) = default;
    ~Mpu9150Interface()      = default;

    virtual Core::CoreStatus Initialize( void )       = 0;
    virtual void Update( const uint64_t currentTime ) = 0;

    virtual Vector3 ReadAcc( void )  = 0;
    virtual Vector3 ReadGyr( void )  = 0;
    virtual Vector3 ReadMag( void )  = 0;
    virtual int16_t ReadTemp( void ) = 0;
};
}
}
