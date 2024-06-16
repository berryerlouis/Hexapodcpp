#include "Servos.h"

namespace Component {
Servos::Servos(Pca9685Interface &pca9685_0, Pca9685Interface &pca9685_1, TickInterface &tick)
	: mServo0(pca9685_0, tick, 0, 90U, 15, 60U, 120U, false)
	, mServo1(pca9685_0, tick, 1, 90U, -4, 60U, 120U, false)
	, mServo2(pca9685_0, tick, 2, 90U, 12, 60U, 120U, false)
	, mServo3(pca9685_0, tick, 3, 90U, 2, 60U, 120U, false)
	, mServo4(pca9685_0, tick, 4, 90U, 0, 60U, 120U, false)
	, mServo5(pca9685_0, tick, 5, 90U, 18, 60U, 120U, false)
	, mServo6(pca9685_0, tick, 6, 90U, 2, 60U, 120U, false)
	, mServo7(pca9685_0, tick, 7, 90U, 0, 60U, 120U, false)
	, mServo8(pca9685_0, tick, 8, 90U, 10, 60U, 120U, false)
	, mServo9(pca9685_1, tick, 0, 90U, -20, 60U, 120U, true)
	, mServo10(pca9685_1, tick, 1, 90U, -2, 60U, 120U, true)
	, mServo11(pca9685_1, tick, 2, 90U, -15, 60U, 120U, true)
	, mServo12(pca9685_1, tick, 3, 90U, 0, 60U, 120U, true)
	, mServo13(pca9685_1, tick, 4, 90U, 0, 60U, 120U, true)
	, mServo14(pca9685_1, tick, 5, 90U, -20, 60U, 120U, true)
	, mServo15(pca9685_1, tick, 6, 90U, 10, 60U, 120U, true)
	, mServo16(pca9685_1, tick, 7, 90U, 0, 60U, 120U, true)
	, mServo17(pca9685_1, tick, 8, 90U, -18, 60U, 120U, true)
	, mServos{&mServo0, &mServo1, &mServo2, &mServo3, &mServo4, &mServo5, &mServo6, &mServo7, &mServo8,
				 &mServo9, &mServo10, &mServo11, &mServo12, &mServo13, &mServo14, &mServo15, &mServo16, &mServo17}
	, mPca9685Left(pca9685_0)
	, mPca9685Right(pca9685_1)
{
}

bool Servos::Initialize (void)
{
	this->mPca9685Left.Initialize();
	this->mPca9685Right.Initialize();

	for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++)
	{
		this->mServos[servoId]->Initialize();
	}

	return (true);
}

void Servos::Update (const uint32_t currentTime)
{
	for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++)
	{
		this->mServos[servoId]->Update(currentTime);
	}
}

Servo &Servos::GetServo (const uint8_t servoId)
{
	return (*this->mServos[servoId]);
}

Pca9685Interface &Servos::GetPca9685 (const uint8_t pca9685Id)
{
	if (pca9685Id == 0U)
	{
		return (this->mPca9685Left);
	}
	return (this->mPca9685Right);
}

bool Servos::BuildFrameAllAngle (Frame &response)
{
	uint8_t params[NB_SERVOS] = { 0U };

	for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++)
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
}
