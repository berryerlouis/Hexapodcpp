#pragma once

#include "ClusterInterface.h"

class Cluster : public ClusterInterface {
public:
	Cluster(EClusters clusterId)
		: mClusterId(clusterId) {
	}

	EClusters GetId (void) {
		return(mClusterId);
	}

	virtual bool Execute(Frame&request, Frame&response) = 0;

private:
	const EClusters mClusterId;
};
