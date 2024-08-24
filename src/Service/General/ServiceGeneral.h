#pragma once

#include "../../Component/Software/SoftwareInterface.h"
#include "../../Cluster/General/ClusterGeneral.h"
#include "../Service.h"

namespace Service {
namespace General {
using namespace Component::Software;
using namespace Cluster::General;

class ServiceGeneral : public Service {
public:
	ServiceGeneral( ClusterGeneral &clusterGeneral, SoftwareInterface &software );
	~ServiceGeneral() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

protected:
	ClusterGeneral &mClusterGeneral;
	SoftwareInterface &mSoftware;
};
}
}
