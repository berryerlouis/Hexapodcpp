#pragma once

#include "Cluster.h"
#include "../cmp/ServosInterface.h"

namespace Clusters {
using namespace Component;

class ClusterServo : public Cluster <ServosInterface> {
public:
	ClusterServo( ServosInterface &servos )
		: Cluster <ServosInterface>( SERVO, servos )
	{
	}

	~ClusterServo() = default;

	inline virtual Core::CoreStatus Execute ( Frame &request, Frame &response ) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		if ( request.clusterId != this->GetId() )
		{
			return ( success );
		}

		uint8_t        servoId = 0xFFU;
		EServoCommands command = (EServoCommands) request.commandId;

		if ( command != EServoCommands::GET_ALL )
		{
			if ( request.nbParams == 0U )
			{
				return ( Core::CoreStatus::CORE_ERROR );
			}
			uint8_t servoId = request.Get1ByteParam( 0U );

			if ( servoId > ServosInterface::NB_SERVOS )
			{
				return ( Core::CoreStatus::CORE_ERROR );
			}
		}

		switch ( command )
		{
		case EServoCommands::GET_ALL:

			if ( request.nbParams == 0U )
			{
				return ( this->BuildFrameAllAngle( response ) );
			}
			break;

		case EServoCommands::GET_ANGLE:

			if ( request.nbParams == 1U )
			{
				return ( this->BuildFrameAngle( servoId, response ) );
			}
			break;

		case EServoCommands::SET_ANGLE:

			if ( request.nbParams == 2U )
			{
				uint8_t angle = request.Get1ByteParam( 1U );
				this->GetComponent().GetServo( servoId ).SetAngle( angle );
				return ( this->BuildFrameAngle( servoId, response ) );
			}
			break;

		case EServoCommands::GET_MIN:

			if ( request.nbParams == 1U )
			{
				return ( this->BuildFrameMinAngle( servoId, response ) );
			}
			break;

		case EServoCommands::SET_MIN:

			if ( request.nbParams == 2U )
			{
				uint8_t angle = request.Get1ByteParam( 1U );
				this->GetComponent().GetServo( servoId ).SetMin( angle );
				return ( this->BuildFrameMinAngle( servoId, response ) );
			}
			break;

		case EServoCommands::GET_MAX:

			if ( request.nbParams == 1U )
			{
				return ( this->BuildFrameMaxAngle( servoId, response ) );
			}
			break;

		case EServoCommands::SET_MAX:

			if ( request.nbParams == 2U )
			{
				uint8_t angle = request.Get1ByteParam( 1U );
				this->GetComponent().GetServo( servoId ).SetMax( angle );
				return ( this->BuildFrameMaxAngle( servoId, response ) );
			}
			break;

		case EServoCommands::GET_OFFSET:

			if ( request.nbParams == 1U )
			{
				return ( this->BuildFrameOffset( servoId, response ) );
			}
			break;

		case EServoCommands::SET_OFFSET:

			if ( request.nbParams == 2U )
			{
				int8_t angle = request.Get1ByteParam( 1U );
				this->GetComponent().GetServo( servoId ).SetOffset( angle );
				return ( this->BuildFrameOffset( servoId, response ) );
			}
			break;

		case EServoCommands::GET_STATE:

			if ( request.nbParams == 1U )
			{
				return ( this->BuildFrameState( servoId, response ) );
			}
			break;

		case EServoCommands::SET_STATE:

			if ( request.nbParams == 2U )
			{
				bool state = request.Get1ByteParam( 1U );
				this->GetComponent().GetServo( servoId ).SetEnable( state );
				return ( this->BuildFrameState( servoId, response ) );
			}
			break;

		case EServoCommands::GET_REVERSE:

			if ( request.nbParams == 1U )
			{
				return ( this->BuildFrameReverse( servoId, response ) );
			}
			break;

		case EServoCommands::SET_REVERSE:

			if ( request.nbParams == 2U )
			{
				bool reverse = request.Get1ByteParam( 1U );
				this->GetComponent().GetServo( servoId ).SetReverse( reverse );
				return ( this->BuildFrameReverse( servoId, response ) );
			}
			break;

		default:
			break;
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameAllAngle ( Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::SERVO,
			EServoCommands::GET_ALL );
		if ( success )
		{
			uint8_t params[ServosInterface::NB_SERVOS] = { 0U };
			for ( size_t servoId = 0U; servoId < ServosInterface::NB_SERVOS; servoId++ )
			{
				params[servoId] = this->GetComponent().GetServo( servoId ).GetAngle();
			}
			response.SetnBytesParam( ServosInterface::NB_SERVOS, (uint8_t *) &params );
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameAngle ( uint8_t servoId, Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::SERVO,
			EServoCommands::GET_ANGLE );
		if ( success )
		{
			response.Set1ByteParam( servoId );
			response.Set1ByteParam( this->GetComponent().GetServo( servoId ).GetAngle() );
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameMinAngle ( uint8_t servoId, Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::SERVO,
			EServoCommands::GET_MIN );
		if ( success )
		{
			response.Set1ByteParam( servoId );
			response.Set1ByteParam( this->GetComponent().GetServo( servoId ).GetMin() );
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameMaxAngle ( uint8_t servoId, Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::SERVO,
			EServoCommands::GET_MAX );
		if ( success )
		{
			response.Set1ByteParam( servoId );
			response.Set1ByteParam( this->GetComponent().GetServo( servoId ).GetMax() );
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameOffset ( uint8_t servoId, Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::SERVO,
			EServoCommands::GET_OFFSET );
		if ( success )
		{
			response.Set1ByteParam( servoId );
			response.Set1ByteParam( this->GetComponent().GetServo( servoId ).GetOffset() );
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameState ( uint8_t servoId, Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::SERVO,
			EServoCommands::GET_STATE );
		if ( success )
		{
			response.Set1ByteParam( servoId );
			response.Set1ByteParam( this->GetComponent().GetServo( servoId ).IsEnable() );
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameReverse ( uint8_t servoId, Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::SERVO,
			EServoCommands::GET_REVERSE );
		if ( success )
		{
			response.Set1ByteParam( servoId );
			response.Set1ByteParam( this->GetComponent().GetServo( servoId ).GetReverse() );
		}
		return ( success );
	}
};
}
