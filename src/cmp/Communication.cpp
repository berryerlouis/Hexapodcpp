#include "Communication.h"

namespace Component {
char    bufferRx[100U];
uint8_t indexBufferRx = 0U;

Communication::Communication(UartInterface &uart, const Clusters &clusters, Led &ledStatus)
	: mUart(uart)
	, mClusters(clusters)
	, mLedStatus(ledStatus)
{
}

bool Communication::Initialize (void)
{
	return (this->mLedStatus.Initialize() );
}

void Communication::Update (const uint32_t currentTime)
{
	(void) currentTime;
	if (true == this->ReceivedStringFrame() )
	{
		Frame request;
		Frame response;
		char  frameBuffer[100U];

		memcpy(frameBuffer, bufferRx, indexBufferRx);
		Protocol::ProtocolStatus parsedStatus = Protocol::Decode(frameBuffer, request);
		if (parsedStatus == Protocol::ProtocolStatus::NO_ERROR)
		{
			uint8_t           frameClusterID = request.clusterId;
			Cluster::Cluster *cluster        = this->mClusters.GetCluster(frameClusterID);

			if (cluster != nullptr)
			{
				if (true == (uint8_t) cluster->Execute(request, response) )
				{
					this->Send(response);
				}
				response.Reset();
			}
			else
			{
				response.clusterId = request.clusterId;
				response.commandId = ( (uint8_t) EClusterCommandGeneric::GENERIC);
				response.nbParams  = 1U;
				response.params[0] = false;
				this->Send(response);
			}
		}
		else
		{
			response.clusterId = 0xFFU;
			response.commandId = ( (uint8_t) EClusterCommandGeneric::GENERIC);
			response.nbParams  = 1U;
			response.params[0] = parsedStatus;
			this->Send(response);
		}
		this->mLedStatus.Off();
	}
}

bool Communication::Send (Frame &message)
{
	uint8_t buffer[100U];

	memset(buffer, 0U, 100U);
	const size_t size = Protocol::Encode(message, buffer);

	if (size != 0)
	{
		this->mUart.Send(buffer, size);
		return (true);
	}
	return (false);
}

bool Communication::ReceivedStringFrame (void)
{
	if (this->mUart.DataAvailable() > 0)
	{
		const char rc = this->mUart.Read();
		if (rc == '<')
		{
			indexBufferRx = 0U;
		}
		else if (rc != '>')
		{
			bufferRx[indexBufferRx] = rc;
			indexBufferRx++;
		}
		else
		{
			bufferRx[indexBufferRx] = '\0';
			indexBufferRx++;
			return (true);
		}
	}
	return (false);
}
}
