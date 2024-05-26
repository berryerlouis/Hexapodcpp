#include "SensorProximity.h"
#include "../drv/Communication.h"

SensorProximity::SensorProximity(Srf05 &srf05Left, Srf05 &srf05Right, Vl53l0x &Vl53l0x) : mSrf05Left(srf05Left),
                                                                                          mSrf05Right(srf05Right),
                                                                                          mVl53l0x(Vl53l0x),
                                                                                          mStepSrf(0),
                                                                                          mStepVlx(0)
{
}

void SensorProximity::Initialize(void)
{
  this->mSrf05Left.Initialize();
  this->mSrf05Right.Initialize();
  this->mVl53l0x.Initialize();
  this->mStepSrf = 0;
  this->mStepVlx = 0;
}

void SensorProximity::Update(const uint32_t currentTime)
{
  if (this->mStepSrf == 0)
  {
    this->mSrf05Left.Update(currentTime);
    this->mStepSrf = SensorProximity::UPDATE_STEP_SRF;
  }
  else if (this->mStepSrf == SensorProximity::UPDATE_STEP_SRF)
  {
    this->mSrf05Right.Update(currentTime);
    this->mStepSrf = 0;
  }

  if (++this->mStepVlx == SensorProximity::UPDATE_STEP_VLX)
  {
    this->mVl53l0x.Update(currentTime);
    this->mStepVlx = 0;
  }
}

bool SensorProximity::IsDetecting(const SensorsId sensorId)
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

  return success;
}

uint16_t SensorProximity::GetDistance(const SensorsId sensorId)
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
  return distance;
}

bool SensorProximity::SetThreshold(const SensorsId sensorId, const uint16_t threshold)
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
  return success;
}

bool SensorProximity::BuildFrameDistance(EProximityCommands side, Frame &response)
{
  uint16_t distance = this->GetDistance((SensorsId)side);
  uint8_t params[] = {(uint8_t)(distance >> 8U), (uint8_t)(distance & 0xFFU)};
  response.Build(EClusters::PROXIMITY, side, params, 2U);
  return true;
}
