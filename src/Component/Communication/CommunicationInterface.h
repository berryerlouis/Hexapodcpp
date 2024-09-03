#pragma once

#include "../ComponentInterface.h"
#include "../../Cluster/Frame/Frame.h"

namespace Component {
namespace Communication {
using namespace Cluster;

class CommunicationInterface : public ComponentInterface
{
public:
    CommunicationInterface( void ) = default;
    ~CommunicationInterface()      = default;

    virtual Core::CoreStatus Initialize( void )       = 0;
    virtual void Update( const uint64_t currentTime ) = 0;

    virtual Core::CoreStatus SendMessage( Frame &message ) = 0;
};
}
}
