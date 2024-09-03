#include "ServiceOrientation.h"

namespace Service {
namespace Orientation {
ServiceOrientation::ServiceOrientation( Mpu9150Interface &imu )
    : Service( 50U )
    , mImu( imu )
{
}

Core::CoreStatus ServiceOrientation::Initialize ( void )
{
    return ( this->mImu.Initialize() );
}

void ServiceOrientation::Update ( const uint64_t currentTime )
{
    this->mImu.Update( currentTime );
}
}
}
