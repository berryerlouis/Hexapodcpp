#pragma once

#include "Cluster.h"
#include "../cmp/BatteryInterface.h"

namespace Clusters {
using namespace Component;

class ClusterBattery : public Cluster <BatteryInterface> {
public:
	ClusterBattery( BatteryInterface &battery )
		: Cluster <BatteryInterface>( BATTERY, battery )
	{
	}

	~ClusterBattery() = default;

	inline virtual Core::CoreStatus Execute ( Frame &request, Frame &response ) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		if ( request.clusterId != this->GetId() )
		{
			return ( success );
		}

		switch ( (EBatteryCommands) request.commandId )
		{
		case EBatteryCommands::GET_VOLTAGE:
			success = this->BuildFrameVoltage( response );
			break;

		case EBatteryCommands::GET_BAT_STATUS:
			success = this->BuildFrameState( response );
			break;

		default:
			break;
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameVoltage ( Frame &response )
	{
		uint16_t volt     = this->GetComponent().GetVoltage();
		uint8_t  params[] = { (uint8_t) ( volt >> 8U ), (uint8_t) volt };

		return ( response.Build(
						EClusters::BATTERY,
						EBatteryCommands::GET_VOLTAGE,
						params,
						2U ) );
	}

	inline Core::CoreStatus BuildFrameState ( Frame &response )
	{
		uint16_t volt     = this->GetComponent().GetVoltage();
		uint8_t  params[] = { this->GetComponent().GetState(), (uint8_t) ( volt >> 8U ), (uint8_t) volt };

		return ( response.Build(
						EClusters::BATTERY,
						EBatteryCommands::GET_BAT_STATUS,
						params,
						3U ) );
	}
};
}
