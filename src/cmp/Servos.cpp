#include "Servos.h"
#include <stddef.h>

#define UPDATE_TIME    20U

Servos::Servos(Pca9685 &pca9685_0, Pca9685 &pca9685_1)
	: mServo0(pca9685_0, 0, 90, 15, 60, 120, false)
	, mServo1(pca9685_0, 1, 90, -4, 60, 120, false)
	, mServo2(pca9685_0, 2, 90, 12, 60, 120, false)
	, mServo3(pca9685_0, 3, 90, 2, 60, 120, false)
	, mServo4(pca9685_0, 4, 90, 0, 60, 120, false)
	, mServo5(pca9685_0, 5, 90, 18, 60, 120, false)
	, mServo6(pca9685_0, 6, 90, 2, 60, 120, false)
	, mServo7(pca9685_0, 7, 90, 0, 60, 120, false)
	, mServo8(pca9685_0, 8, 90, 10, 60, 120, false)
	, mServo9(pca9685_1, 0, 90, -20, 60, 120, true)
	, mServo10(pca9685_1, 1, 90, -2, 60, 120, true)
	, mServo11(pca9685_1, 2, 90, -15, 60, 120, true)
	, mServo12(pca9685_1, 3, 90, 0, 60, 120, true)
	, mServo13(pca9685_1, 4, 90, 0, 60, 120, true)
	, mServo14(pca9685_1, 5, 90, -20, 60, 120, true)
	, mServo15(pca9685_1, 6, 90, 10, 60, 120, true)
	, mServo16(pca9685_1, 7, 90, 0, 60, 120, true)
	, mServo17(pca9685_1, 8, 90, -18, 60, 120, true)
	, mServos{&mServo0, &mServo1, &mServo2, &mServo3, &mServo4, &mServo5, &mServo6, &mServo7, &mServo8,
				 &mServo9, &mServo10, &mServo11, &mServo12, &mServo13, &mServo14, &mServo15, &mServo16, &mServo17}
	, mPca9685Left(pca9685_0)
	, mPca9685Right(pca9685_1)
{
}

void Servos::Initialize (void)
{
	this->mPca9685Left.Initialize();
	this->mPca9685Right.Initialize();

	for ( size_t servoId = 0; servoId < NB_SERVOS; servoId++ )
	{
		this->mServos[servoId]->Initialize();
	}
}

void Servos::Update (const uint32_t currentTime)
{
	for ( size_t servoId = 0; servoId < NB_SERVOS; servoId++ )
	{
		this->mServos[servoId]->Update(currentTime);
	}
}

Servo&Servos::GetServo (const uint8_t servoId)
{
	return (*this->mServos[servoId]);
}

Pca9685&Servos::GetPca9685 (const uint8_t pca9685Id)
{
	if (pca9685Id == 0)
	{
		return (this->mPca9685Left);
	}
	return (this->mPca9685Right);
}

bool Servos::BuildFrameAllAngle (Frame &response)
{
	uint8_t params[NB_SERVOS] = { 0 };

	for ( size_t servoId = 0; servoId < NB_SERVOS; servoId++ )
	{
		params[servoId] = GetServo(servoId).GetAngle();
	}

	return (response.Build(
				  EClusters::SERVO,
				  EServoCommands::GET_ALL,
				  params,
				  NB_SERVOS) );
}

bool Servos::BuildFrameAngle (uint8_t servoId, Frame &response)
{
	uint8_t angle    = GetServo(servoId).GetAngle();
	uint8_t params[] = { servoId, angle };

	return (response.Build(
				  EClusters::SERVO,
				  EServoCommands::GET_ANGLE,
				  params,
				  2U) );
}

bool Servos::BuildFrameMinAngle (uint8_t servoId, Frame &response)
{
	uint8_t angle    = GetServo(servoId).GetMin();
	uint8_t params[] = { servoId, angle };

	return (response.Build(
				  EClusters::SERVO,
				  EServoCommands::GET_MIN,
				  params,
				  2U) );
}

bool Servos::BuildFrameMaxAngle (uint8_t servoId, Frame &response)
{
	uint8_t angle    = GetServo(servoId).GetMax();
	uint8_t params[] = { servoId, angle };

	return (response.Build(
				  EClusters::SERVO,
				  EServoCommands::GET_MAX,
				  params,
				  2U) );
}

bool Servos::BuildFrameOffset (uint8_t servoId, Frame &response)
{
	uint8_t angle    = GetServo(servoId).GetOffset();
	uint8_t params[] = { servoId, angle };

	return (response.Build(
				  EClusters::SERVO,
				  EServoCommands::GET_OFFSET,
				  params,
				  2U) );
}

bool Servos::BuildFrameState (uint8_t servoId, Frame &response)
{
	bool    state    = GetServo(servoId).IsEnable();
	uint8_t params[] = { servoId, state };

	return (response.Build(
				  EClusters::SERVO,
				  EServoCommands::GET_STATE,
				  params,
				  2U) );
}

bool Servos::BuildFrameReverse (uint8_t servoId, Frame &response)
{
	bool    reverse  = GetServo(servoId).GetReverse();
	uint8_t params[] = { servoId, reverse };

	return (response.Build(
				  EClusters::SERVO,
				  EServoCommands::GET_REVERSE,
				  params,
				  2U) );
}
