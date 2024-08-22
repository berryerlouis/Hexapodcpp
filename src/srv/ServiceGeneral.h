#pragma once

#include "../cmp/SoftwareInterface.h"
#include "../clu/ClusterGeneral.h"
#include "Service.h"


using namespace Component;
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
