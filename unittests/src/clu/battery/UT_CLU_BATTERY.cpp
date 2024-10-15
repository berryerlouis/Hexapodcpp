#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockBattery.h"
#include "../../../../src/Cluster/Constants.h"
#include "../../../../src/Cluster/Battery/ClusterBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
	namespace Battery
	{
		class UT_CLU_BATTERY : public ::testing::Test {
		protected:
			UT_CLU_BATTERY() : mMockBattery(),
			                   mClusterBattery(mMockBattery) {
			}

			virtual void SetUp() {
			}

			virtual void TearDown() {
			}

			virtual ~UT_CLU_BATTERY() = default;

			/* Mocks */
			StrictMock<Component::Battery::MockBattery> mMockBattery;

			/* Test class */
			ClusterBattery mClusterBattery;
		};


		TEST_F(UT_CLU_BATTERY, Execute_Command_Voltage_Ok) {
			Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
			const Frame request(BATTERY, GET_VOLTAGE);
			Frame response;

			EXPECT_CALL(mMockBattery, GetVoltage()).WillOnce(Return(10U));

			success = mClusterBattery.ExecuteFrame(request, response);

			EXPECT_EQ(response.clusterId, BATTERY);
			EXPECT_EQ(response.commandId, GET_VOLTAGE);
			EXPECT_EQ(response.nbParams, 2U);
			EXPECT_EQ(response.params[0U], 10U);
			EXPECT_EQ(response.params[1U], 0U);
			EXPECT_EQ(success, Core::CoreStatus::CORE_OK);
		}

		TEST_F(UT_CLU_BATTERY, Execute_Command_status_Ok) {
			Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
			const Frame request(BATTERY, GET_BAT_STATUS);
			Frame response;

			EXPECT_CALL(mMockBattery, GetVoltage()).WillOnce(Return(10U));
			EXPECT_CALL(mMockBattery, GetState()).WillOnce(Return(NOMINAL));

			success = mClusterBattery.ExecuteFrame(request, response);

			EXPECT_EQ(response.clusterId, BATTERY);
			EXPECT_EQ(response.commandId, GET_BAT_STATUS);
			EXPECT_EQ(response.nbParams, 3U);
			EXPECT_EQ(response.params[0U], NOMINAL);
			EXPECT_EQ(response.params[1U], 10U);
			EXPECT_EQ(response.params[2U], 0U);
			EXPECT_EQ(success, Core::CoreStatus::CORE_OK);
		}

		TEST_F(UT_CLU_BATTERY, Execute_WrongCommand_Ko) {
			Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
			Frame request;
			Frame response;

			request.Build(BATTERY, 0x5FU);
			success = mClusterBattery.ExecuteFrame(request, response);

			EXPECT_EQ(response.clusterId, 0U);
			EXPECT_EQ(response.commandId, 0U);
			EXPECT_EQ(response.nbParams, 0U);
			EXPECT_EQ(success, Core::CoreStatus::CORE_ERROR);
		}
	}
}
