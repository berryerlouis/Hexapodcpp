#pragma once

#include "Cluster.h"
#include "../cmp/Mpu9150Interface.h"

namespace Clusters {
using namespace Component;

class ClusterImu : public Cluster <Mpu9150Interface> {
public:
	ClusterImu( Mpu9150Interface &imu )
		: Cluster <Mpu9150Interface>( IMU, imu )
	{
	}

	~ClusterImu() = default;


	inline virtual Core::CoreStatus Execute ( Frame &request, Frame &response ) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		if ( request.clusterId != this->GetId() )
		{
			return ( success );
		}

		switch ( (EImuCommands) request.commandId )
		{
		case EImuCommands::ALL:
			success = this->BuildFrameAll( response );
			break;

		case EImuCommands::ACC:
			success = this->BuildFrameAcc( response );
			break;

		case EImuCommands::GYR:
			success = this->BuildFrameGyr( response );
			break;

		case EImuCommands::MAG:
			success = this->BuildFrameMag( response );
			break;

		case EImuCommands::TMP:
			success = this->BuildFrameTmp( response );
			break;

		default:
			break;
		}
		return ( success );
	}

	Core::CoreStatus BuildFrameAll ( Frame &response )
	{
		uint8_t params[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		/*Mpu9150Interface::Vector3 paramsAcc = this->GetComponent().ReadAcc();
		 * Mpu9150Interface::Vector3 paramsGyr = this->GetComponent().ReadGyr();
		 * Mpu9150Interface::Vector3 paramsMag = this->GetComponent().ReadMag();
		 * int16_t paramsTemp = this->GetComponent().ReadTemp();*/
		return ( response.Build( EClusters::IMU, EImuCommands::ALL, (uint8_t *) &params, 11U ) );
	}

	Core::CoreStatus BuildFrameAcc ( Frame &response )
	{
		Mpu9150Interface::Vector3 params = this->GetComponent().ReadAcc();
		return ( response.Build( EClusters::IMU, EImuCommands::ACC, (uint8_t *) &params, 6U ) );
	}

	Core::CoreStatus BuildFrameGyr ( Frame &response )
	{
		Mpu9150Interface::Vector3 params = this->GetComponent().ReadGyr();
		return ( response.Build( EClusters::IMU, EImuCommands::GYR, (uint8_t *) &params, 6U ) );
	}

	Core::CoreStatus BuildFrameMag ( Frame &response )
	{
		Mpu9150Interface::Vector3 params = this->GetComponent().ReadMag();
		return ( response.Build( EClusters::IMU, EImuCommands::MAG, (uint8_t *) &params, 6U ) );
	}

	Core::CoreStatus BuildFrameTmp ( Frame &response )
	{
		int16_t params = this->GetComponent().ReadTemp();
		return ( response.Build( EClusters::IMU, EImuCommands::TMP, (uint8_t *) &params, 2U ) );
	}
};
}
