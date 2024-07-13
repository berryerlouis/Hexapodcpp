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
		Core::CoreStatus success = response.Build(
			EClusters::IMU,
			EImuCommands::ALL );
		if ( success )
		{
			Mpu9150Interface::Vector3 params = this->GetComponent().ReadAcc();
			response.SetnBytesParam( 6U, (uint8_t *) &params );
			params = this->GetComponent().ReadGyr();
			response.SetnBytesParam( 6U, (uint8_t *) &params );
			params = this->GetComponent().ReadMag();
			response.SetnBytesParam( 6U, (uint8_t *) &params );
			response.Set2BytesParam( this->GetComponent().ReadTemp() );
		}
		return ( success );
	}

	Core::CoreStatus BuildFrameAcc ( Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::IMU,
			EImuCommands::ACC );
		if ( success )
		{
			Mpu9150Interface::Vector3 params = this->GetComponent().ReadAcc();
			response.SetnBytesParam( 6U, (uint8_t *) &params );
		}
		return ( success );
	}

	Core::CoreStatus BuildFrameGyr ( Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::IMU,
			EImuCommands::GYR );
		if ( success )
		{
			Mpu9150Interface::Vector3 params = this->GetComponent().ReadGyr();
			response.SetnBytesParam( 6U, (uint8_t *) &params );
		}
		return ( success );
	}

	Core::CoreStatus BuildFrameMag ( Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::IMU,
			EImuCommands::MAG );
		if ( success )
		{
			Mpu9150Interface::Vector3 params = this->GetComponent().ReadMag();
			response.SetnBytesParam( 6U, (uint8_t *) &params );
		}
		return ( success );
	}

	Core::CoreStatus BuildFrameTmp ( Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::IMU,
			EImuCommands::TMP );
		if ( success )
		{
			response.Set2BytesParam( this->GetComponent().ReadTemp() );
		}
		return ( success );
	}
};
}
