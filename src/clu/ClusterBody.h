#pragma once

#include "../bot/Body.h"
#include "Cluster.h"
#include "Constants.h"

class ClusterBody : public Cluster {
public:
	ClusterBody(Body &body)
		: Cluster(BODY)
		, mBody(body)
	{
	}

	~ClusterBody() = default;


	virtual bool Execute (Frame &request, Frame &response) final override
	{
		bool success = false;

		switch ( (EBodyCommands) request.commandId)
		{
		case EBodyCommands::SET_LEG_X_Y_Z:

			if (request.nbParams == 14U)
			{
				Leg::Position3d position =
				{
					position.x = this->GetSignedParam( ( (uint16_t) (request.params[1U] << 8U) + request.params[0U]) ) / 10.0,
					position.y = this->GetSignedParam( ( (uint16_t) (request.params[3U] << 8U) + request.params[2U]) ) / 10.0,
					position.z = this->GetSignedParam( ( (uint16_t) (request.params[5U] << 8U) + request.params[4U]) ) / 10.0
				};

				Leg::Rotation3d rotation =
				{
					.angleX = this->GetSignedParam( (uint16_t) (request.params[7U] << 8U) + request.params[6U]) / 10.0,
					.angleY = this->GetSignedParam( (uint16_t) (request.params[9U] << 8U) + request.params[8U]) / 10.0,
					.angleZ = this->GetSignedParam( (uint16_t) (request.params[11U] << 8U) + request.params[10U]) / 10.0
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
