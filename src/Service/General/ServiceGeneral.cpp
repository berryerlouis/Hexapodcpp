#include "ServiceGeneral.h"
#include "../../Misc/Logger/Logger.h"

namespace Service {
namespace General {
ServiceGeneral::ServiceGeneral( SoftwareInterface &software )
    : Service( 1U )
    , mSoftware( software )
{
}

Core::CoreStatus ServiceGeneral::Initialize ( void )
{
    return ( this->mSoftware.Initialize() );;
}

void ServiceGeneral::Update ( const uint64_t currentTime )
{
    const uint64_t previousTime = this->GetPreviousTime();
    uint64_t       delta        = currentTime - previousTime;

    if ( delta < this->mSoftware.GetMinTime() )
    {
        this->mSoftware.SetMinTime( delta );
        this->mComMediator->SendMessage( { id: Cluster::EClusters::GENERAL, value: (uint8_t) Cluster::EGeneralCommands::MIN_EXECUTION_TIME } );
    }
    else if ( delta > this->mSoftware.GetMaxTime() )
    {
        this->mSoftware.SetMaxTime( delta );
        this->mComMediator->SendMessage( { id: Cluster::EClusters::GENERAL, value: (uint8_t) Cluster::EGeneralCommands::MAX_EXECUTION_TIME } );
    }
}
}
}
