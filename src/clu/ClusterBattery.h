#pragma once

#include "../cmp/Battery.h"
#include "Cluster.h"
#include "Constants.h"

class ClusterBattery : public Cluster {
public:
	ClusterBattery(Battery &battery)
		: Cluster(BATTERY)
		, mBattery(battery)
	{
	}

	~ClusterBattery() = default;


	virtual bool Execute (Frame &request, Frame &response) final override
	{
		bool success = false;

		switch ( (EBatteryCommands) request.commandId)
		{
		case EBatteryCommands::GET_VOLTAGE:
			success = mBattery.BuildFrameVoltage(response);
			break;

		case EBatteryCommands::GET_BAT_STATUS:
			success = mBattery.BuildFrameState(response);
			break;

		default:
			break;
		}
		return (success);
	}

private:
	Battery &mBattery;
};
