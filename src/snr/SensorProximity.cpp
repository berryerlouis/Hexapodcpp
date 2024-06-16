#include "SensorProximity.h"

SensorProximity::SensorProximity(ProximityInterface &srf05Left, ProximityInterface &srf05Right, ProximityInterface &Vl53l0x)
	: mSrf05Left(srf05Left)
	, mSrf05Right(srf05Right)
	, mVl53l0x(Vl53l0x)
	, mStepSrf(0U)
	, mStepVlx(0U)
{
}

bool SensorProximity::Initialize (void)
{
	uint8_t success = 0U;

	this->mStepSrf = 0U;
	this->mStepVlx = 0U;
	success        = this->mSrf05Left.Initialize() << 0U;
	success       |= this->mSrf05Right.Initialize() << 1U;
	success       |= this->mVl53l0x.Initialize() << 2U;

	return (success == 7U);
}

void SensorProximity::Update (const uint32_t currentTime)
{
	if (this->mStepSrf == 0)
	{
		this->mSrf05Left.Update(currentTime);
		this->mStepSrf = SensorProximity::UPDATE_STEP_SRF_MS;
	}
	else if (this->mStepSrf == SensorProximity::UPDATE_STEP_SRF_MS)
	{
		this->mSrf05Right.Update(currentTime);
		this->mStepSrf = 0U;
	}

	if (++this->mStepVlx == SensorProximity::UPDATE_STEP_VLX_MS)
	{
		this->mVl53l0x.Update(currentTime);
		this->mStepVlx = 0U;
	}
}

bool SensorProximity::IsDetecting (const SensorsId sensorId)
{
	bool success = false;

	switch (sensorId)
	{
	case SensorsId::SRF_LEFT:
		success = this->mSrf05Left.IsDetecting();
		break;

	case SensorsId::SRF_RIGHT:
		success = this->mSrf05Right.IsDetecting();
		break;

	case SensorsId::VLX:
		success = this->mVl53l0x.IsDetecting();
		break;
	}
	return (success);
}

uint16_t SensorProximity::GetDistance (const SensorsId sensorId)
{
	uint16_t distance = 0U;

	switch (sensorId)
	{
	case SensorsId::SRF_LEFT:
		distance = this->mSrf05Left.GetDistance();
		break;

	case SensorsId::SRF_RIGHT:
		distance = this->mSrf05Right.GetDistance();
		break;

	case SensorsId::VLX:
		distance = this->mVl53l0x.GetDistance();
		break;
	}
	return (distance);
}

bool SensorProximity::SetThreshold (const SensorsId sensorId, const uint16_t threshold)
{
	bool success = false;

	switch (sensorId)
	{
	case SensorsId::SRF_LEFT:
		success = this->mSrf05Left.SetThreshold(threshold);
		break;

	case SensorsId::SRF_RIGHT:
		success = this->mSrf05Right.SetThreshold(threshold);
		break;

	case SensorsId::VLX:
		success = this->mVl53l0x.SetThreshold(threshold);
		break;
	}
	return (success);
}

bool SensorProximity::BuildFrameDistance (Cluster::EProximityCommands side, Cluster::Frame &response)
{
	uint16_t distance = this->GetDistance( (SensorsId) side);
	uint8_t  params[] = { (uint8_t) (distance >> 8U), (uint8_t) (distance & 0xFFU) };

	response.Build(Cluster::EClusters::PROXIMITY, side, params, 2U);
	return (true);
}
