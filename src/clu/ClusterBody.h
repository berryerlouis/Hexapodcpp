#pragma once

#include "Cluster.h"
#include "../bot/BodyInterface.h"

namespace Clusters {
using namespace Component;

class ClusterBody : public Cluster <BodyInterface> {
public:
	ClusterBody( BodyInterface &body )
		: Cluster <BodyInterface>( BODY, body )
	{
	}

	~ClusterBody() = default;

	inline virtual Core::CoreStatus Execute ( Frame &request, Frame &response ) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		if ( request.clusterId != this->GetId() )
		{
			return ( success );
		}

		switch ( (EBodyCommands) request.commandId )
		{
		case EBodyCommands::SET_LEG_X_Y_Z:

			if ( request.nbParams == 14U )
			{
				Leg::Position3d position =
				{
					position.x = (uint16_t) ( request.Get2BytesParam( 0U ) ) / 10.0f,
					position.y = (uint16_t) ( request.Get2BytesParam( 2U ) ) / 10.0f,
					position.z = (uint16_t) ( request.Get2BytesParam( 4U ) ) / 10.0f
				};

				Leg::Rotation3d rotation =
				{
					.angleX = (uint16_t) ( request.Get2BytesParam( 6U ) ) / 10.0f,
					.angleY = (uint16_t) ( request.Get2BytesParam( 8U ) ) / 10.0f,
					.angleZ = (uint16_t) ( request.Get2BytesParam( 10U ) ) / 10.0f
				};

				uint16_t travelTime = (uint16_t) ( request.Get2BytesParam( 12U ) );
				this->GetComponent().SetPositionRotation( position, rotation, travelTime );
				success = this->BuildFrameSetPosition( response );
			}
			break;

		default:
			break;
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameSetPosition ( Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::BODY,
			EBodyCommands::SET_LEG_X_Y_Z );
		if ( success )
		{
			response.Set1ByteParam( true );
		}
		return ( success );
	}
};
}
