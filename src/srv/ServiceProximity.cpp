#include "../drv/Communication.h"
#include "ServiceProximity.h"

ServiceProximity::ServiceProximity(SensorProximity &sensorProximity)
	: Service(25)
	, mSensorProximity(sensorProximity)
{
}

void ServiceProximity::Initialize (void)
{
	this->mSensorProximity.Initialize();
}

void ServiceProximity::Update (const uint32_t currentTime)
{
	this->mSensorProximity.Update(currentTime);

	for ( size_t sensorId = 0U; sensorId < SensorProximity::NB_SENSORS; sensorId++ )
	{
		this->mSensorProximity.GetDistance( (SensorProximity::SensorsId) sensorId);

		if (this->mSensorProximity.IsDetecting( (SensorProximity::SensorsId) sensorId) )
		{
			Frame response;
			this->mSensorProximity.BuildFrameDistance( (EProximityCommands) sensorId, response);
			Communication::Send(response);
		}
	}
}
