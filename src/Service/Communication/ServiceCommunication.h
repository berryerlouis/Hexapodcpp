#pragma once

#include "../../Component/Communication/CommunicationInterface.h"
#include "../../Cluster/Clusters/ClustersInterface.h"
#include "../../Core/Event/EventMediatorInterface.h"
#include "../Service.h"

namespace Service {
namespace Communication {
using namespace Component::Communication;

class ServiceCommunication : public Service, public Core::EventMediatorInterface
{
public:
	ServiceCommunication( CommunicationInterface &communication, Clusters::ClustersInterface &clusters );
	~ServiceCommunication() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void SendMessage( Core::Event event ) const final override;

private:
	Clusters::ClustersInterface &mClusters;
protected:
	CommunicationInterface & mCommunication;
};
}
}
