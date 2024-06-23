#pragma once

#include "../clu/ClusterGeneral.h"
#include "Service.h"


using namespace Component;
class ServiceGeneral : public Service {
public:
	ServiceGeneral( ClusterGeneral &clusterGeneral );
	~ServiceGeneral() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint32_t currentTime ) final override;

protected:
	ClusterGeneral &mClusterGeneral;
};
