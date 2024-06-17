#pragma once

#include "../cmp/ServosInterface.h"
#include "Cluster.h"
#include "Constants.h"

namespace Cluster {
using namespace Component;
class ClusterServo : public Cluster {
public:
	ClusterServo(ServosInterface &servos)
		: Cluster(SERVO)
		, mServos(servos)
	{
	}

	~ClusterServo() = default;


	Core::CoreStatus Execute (Frame &request, Frame &response) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
		uint8_t          servoId = 0xFFU;
		EServoCommands   command = (EServoCommands) request.commandId;

		if (command != EServoCommands::GET_ALL)
		{
			if (request.nbParams == 0U)
			{
				return (Core::CoreStatus::CORE_ERROR);
			}
			servoId = request.params[0U];

			if (servoId > NB_SERVOS)
			{
				return (Core::CoreStatus::CORE_ERROR);
			}
		}

		switch (command)
		{
		case EServoCommands::GET_ALL:

			if (request.nbParams == 0U)
			{
				return (this->BuildFrameAllAngle(response) );
			}
			break;

		case EServoCommands::GET_ANGLE:

			if (request.nbParams == 1U)
			{
				return (this->BuildFrameAngle(servoId, response) );
			}
			break;

		case EServoCommands::SET_ANGLE:

			if (request.nbParams == 2U)
			{
				uint8_t angle = request.params[1U];
				this->mServos.GetServo(servoId).SetAngle(angle);
				return (this->BuildFrameAngle(servoId, response) );
			}
			break;

		case EServoCommands::GET_MIN:

			if (request.nbParams == 1U)
			{
				return (this->BuildFrameMinAngle(servoId, response) );
			}
			break;

		case EServoCommands::SET_MIN:

			if (request.nbParams == 2U)
			{
				uint8_t angle = request.params[1U];
				this->mServos.GetServo(servoId).SetMin(angle);
				return (this->BuildFrameMinAngle(servoId, response) );
			}
			break;

		case EServoCommands::GET_MAX:

			if (request.nbParams == 1U)
			{
				return (this->BuildFrameMaxAngle(servoId, response) );
			}
			break;

		case EServoCommands::SET_MAX:

			if (request.nbParams == 2U)
			{
				uint8_t angle = request.params[1U];
				this->mServos.GetServo(servoId).SetMax(angle);
				return (this->BuildFrameMaxAngle(servoId, response) );
			}
			break;

		case EServoCommands::GET_OFFSET:

			if (request.nbParams == 1U)
			{
				return (this->BuildFrameOffset(servoId, response) );
			}
			break;

		case EServoCommands::SET_OFFSET:

			if (request.nbParams == 2U)
			{
				int8_t angle = (int8_t) request.params[1U];
				this->mServos.GetServo(servoId).SetOffset(angle);
				return (this->BuildFrameOffset(servoId, response) );
			}
			break;

		case EServoCommands::GET_STATE:

			if (request.nbParams == 1U)
			{
				return (this->BuildFrameState(servoId, response) );
			}
			break;

		case EServoCommands::SET_STATE:

			if (request.nbParams == 2U)
			{
				if (request.params[1U] == 0x00U || request.params[1U] == 0x01U)
				{
					this->mServos.GetServo(servoId).SetEnable(request.params[1U]);
					return (this->BuildFrameState(servoId, response) );
				}
			}
			break;

		case EServoCommands::GET_REVERSE:

			if (request.nbParams == 1U)
			{
				return (this->BuildFrameReverse(servoId, response) );
			}
			break;

		case EServoCommands::SET_REVERSE:

			if (request.nbParams == 2U)
			{
				if (request.params[1U] == 0x00U || request.params[1U] == 0x01U)
				{
					this->mServos.GetServo(servoId).SetReverse( (bool) request.params[1U]);
					return (this->BuildFrameReverse(servoId, response) );
				}
			}
			break;

		default:
			break;
		}
		return (success);
	}

private:
	ServosInterface &mServos;

	Core::CoreStatus BuildFrameAllAngle (Frame &response)
	{
		uint8_t params[NB_SERVOS] = { 0U };

		for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++)
		{
			params[servoId] = this->mServos.GetServo(servoId).GetAngle();
		}

		return (response.Build(
					  EClusters::SERVO,
					  EServoCommands::GET_ALL,
					  params,
					  NB_SERVOS) );
	}

	Core::CoreStatus BuildFrameAngle (uint8_t servoId, Frame &response)
	{
		uint8_t angle    = this->mServos.GetServo(servoId).GetAngle();
		uint8_t params[] = { servoId, angle };

		return (response.Build(
					  EClusters::SERVO,
					  EServoCommands::GET_ANGLE,
					  params,
					  2U) );
	}

	Core::CoreStatus BuildFrameMinAngle (uint8_t servoId, Frame &response)
	{
		uint8_t angle    = this->mServos.GetServo(servoId).GetMin();
		uint8_t params[] = { servoId, angle };

		return (response.Build(
					  EClusters::SERVO,
					  EServoCommands::GET_MIN,
					  params,
					  2U) );
	}

	Core::CoreStatus BuildFrameMaxAngle (uint8_t servoId, Frame &response)
	{
		uint8_t angle    = this->mServos.GetServo(servoId).GetMax();
		uint8_t params[] = { servoId, angle };

		return (response.Build(
					  EClusters::SERVO,
					  EServoCommands::GET_MAX,
					  params,
					  2U) );
	}

	Core::CoreStatus BuildFrameOffset (uint8_t servoId, Frame &response)
	{
		uint8_t angle    = this->mServos.GetServo(servoId).GetOffset();
		uint8_t params[] = { servoId, angle };

		return (response.Build(
					  EClusters::SERVO,
					  EServoCommands::GET_OFFSET,
					  params,
					  2U) );
	}

	Core::CoreStatus BuildFrameState (uint8_t servoId, Frame &response)
	{
		bool    state    = this->mServos.GetServo(servoId).IsEnable();
		uint8_t params[] = { servoId, state };

		return (response.Build(
					  EClusters::SERVO,
					  EServoCommands::GET_STATE,
					  params,
					  2U) );
	}

	Core::CoreStatus BuildFrameReverse (uint8_t servoId, Frame &response)
	{
		bool    reverse  = this->mServos.GetServo(servoId).GetReverse();
		uint8_t params[] = { servoId, reverse };

		return (response.Build(
					  EClusters::SERVO,
					  EServoCommands::GET_REVERSE,
					  params,
					  2U) );
	}
};
}
