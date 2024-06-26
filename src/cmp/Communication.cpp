#include "Communication.h"

namespace Component {
Communication::Communication( UartInterface &uart, ClustersInterface &clusters, LedInterface &ledStatus )
	: mUart( uart )
	, mClusters( clusters )
	, mLedStatus( ledStatus )
	, mBufferRx{ 0U }
	, mIndexBufferRx( 0U )
{
}

Core::CoreStatus Communication::Initialize ( void )
{
	return ( this->mLedStatus.Initialize() );
}

void Communication::Update ( const uint32_t currentTime )
{
	(void) currentTime;
	if ( true == this->ReceivedStringFrame() )
	{
		this->mLedStatus.On();
		Frame request;
		Frame response;
		Protocol::ProtocolStatus parsedStatus = Protocol::Decode( this->mBufferRx, request );
		if ( parsedStatus == Protocol::ProtocolStatus::NO_ERROR )
		{
			uint8_t frameClusterID = request.clusterId;
			auto    cluster        = this->mClusters.GetCluster( frameClusterID );

			if ( cluster != nullptr )
			{
				if ( Core::CoreStatus::CORE_OK == cluster->Execute( request, response ) )
				{
					this->Send( response );
				}
				else
				{
					response.clusterId = request.clusterId;
					response.commandId = ( (uint8_t) EClusterCommandGeneric::GENERIC );
					response.nbParams  = 1U;
					response.params[0] = false;
					this->Send( response );
				}
				response.Reset();
			}
			else
			{
				response.clusterId = request.clusterId;
				response.commandId = ( (uint8_t) EClusterCommandGeneric::GENERIC );
				response.nbParams  = 1U;
				response.params[0] = false;
				this->Send( response );
			}
		}
		else
		{
			response.clusterId = 0xFFU;
			response.commandId = ( (uint8_t) EClusterCommandGeneric::GENERIC );
			response.nbParams  = 1U;
			response.params[0] = parsedStatus;
			this->Send( response );
		}
		this->mLedStatus.Off();
	}
}

Core::CoreStatus Communication::Send ( Clusters::Frame &message )
{
	uint8_t buffer[100U];

	memset( buffer, 0U, 100U );
	const size_t size = Protocol::Encode( message, buffer );

	if ( size != 0 )
	{
		this->mUart.Send( buffer, size );
		return ( Core::CoreStatus::CORE_OK );
	}
	return ( Core::CoreStatus::CORE_ERROR );
}

bool Communication::ReceivedStringFrame ( void )
{
	if ( this->mUart.DataAvailable() > 0 )
	{
		const char rc = this->mUart.Read();
		if ( rc == '<' )
		{
			this->mIndexBufferRx = 0U;
		}
		else if ( rc != '>' )
		{
			this->mBufferRx[this->mIndexBufferRx] = rc;
			this->mIndexBufferRx++;
		}
		else
		{
			this->mBufferRx[this->mIndexBufferRx] = '\0';
			this->mIndexBufferRx++;
			return ( true );
		}
	}
	return ( false );
}
}
