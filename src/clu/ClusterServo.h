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
			uint8_t servoId = request.params[0U];

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
				uint8_t angle = request.params[1U];
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
				uint8_t angle = request.params[1U];
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
				uint8_t angle = request.params[1U];
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
				int8_t angle = (int8_t) request.params[1U];
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
				if ( request.params[1U] == 0x00U || request.params[1U] == 0x01U )
				{
					this->GetComponent().GetServo( servoId ).SetEnable( request.params[1U] );
					return ( this->BuildFrameState( servoId, response ) );
				}
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
				if ( request.params[1U] == 0x00U || request.params[1U] == 0x01U )
				{
					this->GetComponent().GetServo( servoId ).SetReverse( (bool) request.params[1U] );
					return ( this->BuildFrameReverse( servoId, response ) );
				}
			}
			break;

		default:
			break;
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameAllAngle ( Frame &response )
	{
		uint8_t params[ServosInterface::NB_SERVOS] = { 0U };

		for ( size_t servoId = 0U; servoId < ServosInterface::NB_SERVOS; servoId++ )
		{
			params[servoId] = this->GetComponent().GetServo( servoId ).GetAngle();
		}

		return ( response.Build(
						EClusters::SERVO,
						EServoCommands::GET_ALL,
						params,
						ServosInterface::NB_SERVOS ) );
	}

	inline Core::CoreStatus BuildFrameAngle ( uint8_t servoId, Frame &response )
	{
		uint8_t angle    = this->GetComponent().GetServo( servoId ).GetAngle();
		uint8_t params[] = { servoId, angle };

		return ( response.Build(
						EClusters::SERVO,
						EServoCommands::GET_ANGLE,
						params,
						2U ) );
	}

	inline Core::CoreStatus BuildFrameMinAngle ( uint8_t servoId, Frame &response )
	{
		uint8_t angle    = this->GetComponent().GetServo( servoId ).GetMin();
		uint8_t params[] = { servoId, angle };

		return ( response.Build(
						EClusters::SERVO,
						EServoCommands::GET_MIN,
						params,
						2U ) );
	}

	inline Core::CoreStatus BuildFrameMaxAngle ( uint8_t servoId, Frame &response )
	{
		uint8_t angle    = this->GetComponent().GetServo( servoId ).GetMax();
		uint8_t params[] = { servoId, angle };

		return ( response.Build(
						EClusters::SERVO,
						EServoCommands::GET_MAX,
						params,
						2U ) );
	}

	inline Core::CoreStatus BuildFrameOffset ( uint8_t servoId, Frame &response )
	{
		uint8_t angle    = this->GetComponent().GetServo( servoId ).GetOffset();
		uint8_t params[] = { servoId, angle };

		return ( response.Build(
						EClusters::SERVO,
						EServoCommands::GET_OFFSET,
						params,
						2U ) );
	}

	inline Core::CoreStatus BuildFrameState ( uint8_t servoId, Frame &response )
	{
		bool    state    = this->GetComponent().GetServo( servoId ).IsEnable();
		uint8_t params[] = { servoId, state };

		return ( response.Build(
						EClusters::SERVO,
						EServoCommands::GET_STATE,
						params,
						2U ) );
	}

	inline Core::CoreStatus BuildFrameReverse ( uint8_t servoId, Frame &response )
	{
		bool    reverse  = this->GetComponent().GetServo( servoId ).GetReverse();
		uint8_t params[] = { servoId, reverse };

		return ( response.Build(
						EClusters::SERVO,
						EServoCommands::GET_REVERSE,
						params,
						2U ) );
	}
};
}
