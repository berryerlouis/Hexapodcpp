#pragma once

#include "../bot/Body.h"
#include "Cluster.h"
#include "Constants.h"

namespace Cluster {
using namespace Component;
class ClusterBody : public Cluster {
public:
	ClusterBody(Body &body)
		: Cluster(BODY)
		, mBody(body)
	{
	}

	~ClusterBody() = default;


	virtual Core::CoreStatus Execute (Frame &request, Frame &response) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		switch ( (EBodyCommands) request.commandId)
		{
		case EBodyCommands::SET_LEG_X_Y_Z:

			if (request.nbParams == 14U)
			{
				Leg::Position3d position =
				{
					position.x = this->GetSignedParam( ( (uint16_t) (request.params[1U] << 8U) + request.params[0U]) ) / 10.0f,
					position.y = this->GetSignedParam( ( (uint16_t) (request.params[3U] << 8U) + request.params[2U]) ) / 10.0f,
					position.z = this->GetSignedParam( ( (uint16_t) (request.params[5U] << 8U) + request.params[4U]) ) / 10.0f
				};

				Leg::Rotation3d rotation =
				{
					.angleX = this->GetSignedParam( (uint16_t) (request.params[7U] << 8U) + request.params[6U]) / 10.0f,
					.angleY = this->GetSignedParam( (uint16_t) (request.params[9U] << 8U) + request.params[8U]) / 10.0f,
					.angleZ = this->GetSignedParam( (uint16_t) (request.params[11U] << 8U) + request.params[10U]) / 10.0f
				};

				uint16_t travelTime = (uint16_t) (request.params[13U] << 8U) + request.params[12U];
				this->mBody.SetPositionRotation(position, rotation, travelTime);
				success = mBody.BuildFrameSetPosition(response);
			}
			break;

		default:
			break;
		}
		return (success);
	}

private:
	Body &mBody;


	float GetSignedParam (const uint16_t param)
	{
		float value = param;

		if (value > 0x8000)
		{
			value = 32768 - param;
		}
		return (value);
	}
};
}
