#pragma once

#include "../cmp/BatteryInterface.h"
#include "Cluster.h"
#include "Constants.h"

namespace Cluster {
using namespace Component;

class ClusterBattery : public Cluster {
public:
	ClusterBattery(BatteryInterface &battery)
		: Cluster(BATTERY)
		, mBattery(battery)
	{
	}

	~ClusterBattery() = default;


	virtual Core::CoreStatus Execute (Frame &request, Frame &response) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		switch ( (EBatteryCommands) request.commandId)
		{
		case EBatteryCommands::GET_VOLTAGE:
			success = this->BuildFrameVoltage(response);
			break;

		case EBatteryCommands::GET_BAT_STATUS:
			success = this->BuildFrameState(response);
			break;

		default:
			break;
		}
		return (success);
	}

private:
	BatteryInterface &mBattery;

	Core::CoreStatus BuildFrameVoltage (Frame &response)
	{
		uint16_t volt     = this->mBattery.GetVoltage();
		uint8_t  params[] = { (uint8_t) (volt >> 8U), (uint8_t) volt };

		return (response.Build(
					  EClusters::BATTERY,
					  EBatteryCommands::GET_VOLTAGE,
					  params,
					  2U) );
	}

	Core::CoreStatus BuildFrameState (Frame &response)
	{
		uint16_t volt     = this->mBattery.GetVoltage();
		uint8_t  params[] = { this->mBattery.GetState(), (uint8_t) (volt >> 8U), (uint8_t) volt };

		return (response.Build(
					  EClusters::BATTERY,
					  EBatteryCommands::GET_BAT_STATUS,
					  params,
					  3U) );
	}
};
}
