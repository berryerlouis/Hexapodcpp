#pragma once

#include "Cluster.h"
#include "../cmp/SoftwareInterface.h"

namespace Clusters {
using namespace Component;

class ClusterGeneral : public Cluster <SoftwareInterface> {
public:
	ClusterGeneral( SoftwareInterface &software )
		: Cluster <SoftwareInterface>( GENERAL, software )
	{
	}

	~ClusterGeneral() = default;

	inline virtual Core::CoreStatus  Execute ( Frame &request, Frame &response ) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		if ( request.clusterId != this->GetId() )
		{
			return ( success );
		}

		switch ( (EGeneralCommands) request.commandId )
		{
		case EGeneralCommands::VERSION:
			success = this->BuildFrameGetVersion( response );
			break;

		case EGeneralCommands::MIN_EXECUTION_TIME:
			success = this->BuildFrameGetMinTime( response );
			success = Core::CoreStatus::CORE_OK;
			break;

		case EGeneralCommands::MAX_EXECUTION_TIME:
			success = this->BuildFrameGetMaxTime( response );
			success = Core::CoreStatus::CORE_OK;
			break;

		case EGeneralCommands::RESET_EXECUTION_TIME:
			this->GetComponent().ResetTiming();
			return ( response.Build( GENERAL, RESET_EXECUTION_TIME ) );

			break;

		default:
			break;
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameGetVersion ( Frame &response )
	{
		SoftwareInterface::Version version = this->GetComponent().GetVersion();
		uint8_t params[] = { version.major, version.minor };
		return ( response.Build( GENERAL, VERSION, params, 2U ) );
	}

	inline Core::CoreStatus BuildFrameGetMinTime ( Frame &response )
	{
		uint64_t time = this->GetComponent().GetMinTime();
		return ( response.Build( GENERAL, MIN_EXECUTION_TIME, (uint8_t *) &time, 8U ) );
	}

	inline Core::CoreStatus BuildFrameGetMaxTime ( Frame &response )
	{
		uint64_t time = this->GetComponent().GetMaxTime();
		return ( response.Build( GENERAL, MAX_EXECUTION_TIME, (uint8_t *) &time, 8U ) );
	}
};
}
