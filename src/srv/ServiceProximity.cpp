#include "ServiceProximity.h"
#include "../drv/Communication.h"

ServiceProximity::ServiceProximity(SensorProximity &sensorProximity) : Service(50),
                                                                       mSensorProximity(sensorProximity)
{
}

void ServiceProximity::Initialize(void)
{
    this->mSensorProximity.Initialize();
}

void ServiceProximity::Update(const uint32_t currentTime)
{
    this->mSensorProximity.Update(currentTime);
    for (uint8_t sensorId = 0; sensorId < SensorProximity::NB_SENSORS; sensorId++)
    {
        this->mSensorProximity.GetDistance((SensorProximity::SensorsId)sensorId);
        if (this->mSensorProximity.IsDetecting((SensorProximity::SensorsId)sensorId))
        {
            Frame response;
            this->mSensorProximity.BuildFrameDistance((EProximityCommands)sensorId, response);
            Communication::Send(response);
        }
    }
}