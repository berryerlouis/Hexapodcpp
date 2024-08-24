#pragma once

#include <stdint.h>

namespace Cluster {
enum EClusters
{
	GENERAL   = 0U,
	IMU       = 1U,
	PROXIMITY = 2U,
	SERVO     = 3U,
	BATTERY   = 4U,
	BODY      = 5U
};

static const uint8_t NB_CLUSTERS = 6U;

enum EClusterCommandGeneric
{
	GENERIC = 0xFFU
};

enum EGeneralCommands
{
	VERSION,
	MIN_EXECUTION_TIME,
	MAX_EXECUTION_TIME,
	INSTANT_EXECUTION_TIME,
	RESET_EXECUTION_TIME
};

enum EBatteryCommands
{
	GET_VOLTAGE = 0x00U,
	GET_BAT_STATUS,
};

enum EBodyCommands
{
	SET_LEG_X_Y_Z = 0,
};

enum EImuCommands
{
	ALL,
	ACC,
	GYR,
	MAG,
	TMP
};
enum EServoCommands
{
	GET_ALL = 0,
	GET_ANGLE,
	SET_ANGLE,
	GET_MIN,
	SET_MIN,
	GET_MAX,
	SET_MAX,
	GET_OFFSET,
	SET_OFFSET,
	GET_STATE,
	SET_STATE,
	GET_REVERSE,
	SET_REVERSE,
	SAVE,
};

enum EProximityCommands
{
	US_LEFT,
	US_RIGHT,
	LASER,
	SET_THRESHOLD,
};
}
