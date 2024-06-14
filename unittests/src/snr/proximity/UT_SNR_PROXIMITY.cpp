#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSrf05.h"
#include "../../../mock/cmp/MockVl53l0x.h"

#include "../../../../src/snr/SensorProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST(SensorProximity, Initialize_Ok)
{
	bool success = false;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity(srf05Left, srf05Right, vl53l0x);

	EXPECT_CALL(srf05Left, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(srf05Right, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(vl53l0x, Initialize() ).WillOnce(Return(true) );

	success = sensorProximity.Initialize();

	EXPECT_TRUE(success);
}

TEST(SensorProximity, Update_Ok)
{
	bool success = false;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity(srf05Left, srf05Right, vl53l0x);

	EXPECT_CALL(srf05Left, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(srf05Right, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(vl53l0x, Initialize() ).WillOnce(Return(true) );

	EXPECT_CALL(srf05Left, Update(_) ).Times(1U);
	EXPECT_CALL(srf05Right, Update(_) ).Times(0U);
	EXPECT_CALL(vl53l0x, Update(_) ).Times(1U);

	success = sensorProximity.Initialize();

	sensorProximity.Update(0UL);
	EXPECT_TRUE(success);
}

TEST(SensorProximity, Update2Times_Ok)
{
	bool success = false;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity(srf05Left, srf05Right, vl53l0x);

	EXPECT_CALL(srf05Left, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(srf05Right, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(vl53l0x, Initialize() ).WillOnce(Return(true) );

	EXPECT_CALL(srf05Left, Update(_) ).Times(1U);
	EXPECT_CALL(srf05Right, Update(_) ).Times(1U);
	EXPECT_CALL(vl53l0x, Update(_) ).Times(2U);

	success = sensorProximity.Initialize();

	sensorProximity.Update(0UL);
	sensorProximity.Update(0UL);
	EXPECT_TRUE(success);
}

TEST(SensorProximity, GetDistance_Ok)
{
	bool success = false;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity(srf05Left, srf05Right, vl53l0x);

	EXPECT_CALL(srf05Left, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(srf05Right, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(vl53l0x, Initialize() ).WillOnce(Return(true) );

	EXPECT_CALL(srf05Left, GetDistance() ).Times(1U);
	EXPECT_CALL(srf05Right, GetDistance() ).Times(1U);
	EXPECT_CALL(vl53l0x, GetDistance() ).Times(1U);

	success = sensorProximity.Initialize();

	for (size_t sensorId = 0U; sensorId < SensorProximity::NB_SENSORS; sensorId++)
	{
		sensorProximity.GetDistance( (SensorProximity::SensorsId) sensorId);
	}
	EXPECT_TRUE(success);
}

TEST(SensorProximity, SetThreshold_Ok)
{
	bool                     success   = false;
	const uint16_t           threshold = 10U;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity(srf05Left, srf05Right, vl53l0x);

	EXPECT_CALL(srf05Left, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(srf05Right, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(vl53l0x, Initialize() ).WillOnce(Return(true) );

	EXPECT_CALL(srf05Left, SetThreshold(threshold) ).Times(1U);
	EXPECT_CALL(srf05Right, SetThreshold(threshold) ).Times(1U);
	EXPECT_CALL(vl53l0x, SetThreshold(threshold) ).Times(1U);

	success = sensorProximity.Initialize();

	for (size_t sensorId = 0U; sensorId < SensorProximity::NB_SENSORS; sensorId++)
	{
		sensorProximity.SetThreshold( (SensorProximity::SensorsId) sensorId, threshold);
	}
	EXPECT_TRUE(success);
}

TEST(SensorProximity, BuildFrameDistance_Ok)
{
	bool                     success = false;
	Cluster::Frame           response;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity(srf05Left, srf05Right, vl53l0x);

	EXPECT_CALL(srf05Left, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(srf05Right, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(vl53l0x, Initialize() ).WillOnce(Return(true) );

	EXPECT_CALL(srf05Left, GetDistance() ).WillOnce(Return(10U) );
	EXPECT_CALL(srf05Right, GetDistance() ).WillOnce(Return(10U) );
	EXPECT_CALL(vl53l0x, GetDistance() ).WillOnce(Return(10U) );

	success = sensorProximity.Initialize();

	for (size_t sensorId = 0U; sensorId < SensorProximity::NB_SENSORS; sensorId++)
	{
		sensorProximity.BuildFrameDistance( (Cluster::EProximityCommands) sensorId, response);
		EXPECT_EQ(response.clusterId, Cluster::EClusters::PROXIMITY);
		EXPECT_EQ(response.commandId, sensorId);
		EXPECT_EQ(response.nbParams, 2U);
		EXPECT_EQ(response.params[0U], 0U);
		EXPECT_EQ(response.params[1U], 10U);
	}

	EXPECT_TRUE(success);
}
