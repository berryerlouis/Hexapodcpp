#pragma once

#include "../cmp/Servo.h"
#include "Constants.h"
#include <math.h>
#include <stddef.h>

class Leg {
public:
	static constexpr float BODY_SIDE_X_LENGTH        = 80.0;
	static constexpr float BODY_SIDE_X_MIDDLE_LENGTH = 130.0;
	static constexpr float BODY_SIDE_Y_LENGTH        = 150.0;

	static constexpr float BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH     = BODY_SIDE_X_MIDDLE_LENGTH / 2.0;
	static constexpr float BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH     = 0.0;
	static constexpr float BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH = BODY_SIDE_X_LENGTH / 2.0;
	static constexpr float BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH = BODY_SIDE_Y_LENGTH / 2.0;

	static constexpr float COXA_LENGTH  = 2.54;
	static constexpr float FEMUR_LENGTH = 8.50;
	static constexpr float TIBIA_LENGTH = 12.70;


	struct Position3d {
		float x;
		float y;
		float z;
	};
	struct Rotation3d {
		float angleX;
		float angleY;
		float angleZ;
	};

	struct SLegIk {
		Position3d newFootPos;
		float      coxaFootDist;
		float      iksw;
		float      ika1;
		float      ika2;
		float      tangle;
		float      coxaIk;
		float      femurIk;
		float      tibiaIk;
	};


	Leg(ELeg legId, Servo&coxa, Servo&femur, Servo&tibia);
	~Leg() = default;


	void SetLegIk(const Leg::Position3d&position, const Leg::Position3d&bodyIk, const uint16_t travelTime);

public:
	float mBodyCenterOffsetX;
	float mBodyCenterOffsetY;
	Position3d mFootPosition;
	SLegIk mLegIk;

private:
	ELeg mLegId;
	Servo&mCoxa;
	Servo&mFemur;
	Servo&mTibia;
};
