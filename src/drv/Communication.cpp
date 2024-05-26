#include "Communication.h"

String bufferRx;

Communication::Communication(const Clusters &clusters, Led &ledStatus) : mClusters(clusters),
																																				 mLedStatus(ledStatus)
{
}

bool Communication::ReceivedStringFrame(void)
{
	if (Serial.available() > 0)
	{
		const char rc = Serial.read();
		// Serial.print(rc);
		if (rc == '<')
		{
			bufferRx = "";
		}
		else if (rc != '>')
		{
			bufferRx += rc;
		}
		else
		{
			return true;
		}
	}
	return false;
}

void Communication::Update()
{
	if (true == this->ReceivedStringFrame())
	{
		String frameBuffer(bufferRx.c_str());
		bufferRx = "";
		Frame request;
		Frame response;
		// Serial.println(frameBuffer);
		Protocol::ProtocolStatus state = this->mProtocol.Decode(frameBuffer.c_str(), request);
		if (state == Protocol::ProtocolStatus::NO_ERROR)
		{
			uint8_t frameClusterID = request.clusterId;
			Cluster *cluster = this->mClusters.GetCluster(frameClusterID);
			if (cluster != nullptr)
			{
				if (true == (uint8_t)cluster->Execute(request, response))
				{
					this->Send(response);
				}
				response.Reset();
			}
			else
			{
				response.clusterId = request.clusterId;
				response.commandId = ((uint8_t)EClusterCommandGeneric::GENERIC);
				response.nbParams = 1U;
				response.params[0] = false;
				this->Send(response);
			}
		}
		else
		{
			response.clusterId = 0xFFU;
			response.commandId = ((uint8_t)EClusterCommandGeneric::GENERIC);
			response.nbParams = 1U;
			response.params[0] = state;
			this->Send(response);
		}
		this->mLedStatus.Off();
	}
}

bool Communication::Send(Frame &message)
{
	uint8_t buffer[100U];
	memset(buffer, 0U, 100U);
	const size_t size = Protocol::Encode(message, buffer);
	if (size != 0)
	{
		Serial.write(buffer, size);
		return true;
	}
	return false;
}
