#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockMpu9150.h"

#include "../../../../src/srv/ServiceOrientation.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST(ServiceOrientation, Initialize_Ok)
{
	bool success = false;
	StrictMock <MockMpu9150> mpu9150;

	ServiceOrientation serviceOrientation(mpu9150);

	EXPECT_CALL(mpu9150, Initialize() ).WillOnce(Return(true) );

	success = serviceOrientation.Initialize();

	EXPECT_TRUE(success);
}


TEST(ServiceOrientation, Update_Ok)
{
	bool success = false;
	StrictMock <MockMpu9150> mpu9150;

	ServiceOrientation serviceOrientation(mpu9150);

	EXPECT_CALL(mpu9150, Initialize() ).WillOnce(Return(true) );

	success = serviceOrientation.Initialize();

	EXPECT_CALL(mpu9150, Update(_) ).Times(1U);

	serviceOrientation.Update(0UL);

	EXPECT_TRUE(success);
}