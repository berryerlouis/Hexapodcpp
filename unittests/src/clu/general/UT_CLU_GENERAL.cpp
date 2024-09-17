#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSoftware.h"
#include "../../../../src/Cluster/General/ClusterGeneral.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
	namespace General
	{
		class UT_CLU_GENERAL : public ::testing::Test {
		protected:
			UT_CLU_GENERAL() : mMockSoftware(),
			                   mClusterGeneral(mMockSoftware) {
			}

			virtual void SetUp() {
			}

			virtual void TearDown() {
			}

			virtual ~UT_CLU_GENERAL() = default;

			/* Mocks */
			StrictMock<Component::Software::MockSoftware> mMockSoftware;

			/* Test class */
			ClusterGeneral mClusterGeneral;
		};

		TEST_F(UT_CLU_GENERAL, Execute_WrongCommand_Ko) {
			Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
			Frame request;
			Frame response;

			request.Build(GENERAL, 0x5FU);
			success = mClusterGeneral.ExecuteFrame(request, response);

			EXPECT_EQ(response.clusterId, 0U);
			EXPECT_EQ(response.commandId, 0U);
			EXPECT_EQ(response.nbParams, 0U);
			EXPECT_FALSE(success);
		}

		TEST_F(UT_CLU_GENERAL, Execute_VERSION_Ok) {
			Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
			Frame request;
			Frame response;

			EXPECT_CALL(mMockSoftware, GetVersion()).WillOnce(Return(SoftwareInterface::Version{1, 0}));
			request.Build(GENERAL, EGeneralCommands::VERSION);
			success = mClusterGeneral.ExecuteFrame(request, response);

			EXPECT_EQ(response.clusterId, GENERAL);
			EXPECT_EQ(response.commandId, EGeneralCommands::VERSION);
			EXPECT_EQ(response.nbParams, 2U);
			EXPECT_TRUE(success);
		}
	}
}
