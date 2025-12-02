#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/bot/MockLeg.h"
#include "../../../mock/bot/MockLegs.h"
#include "../../../../src/Bot/Body/Body.h"
#include "../../../../src/Cluster/Battery/ClusterBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Bot
{
    namespace Body
    {
        class UT_BOT_BODY : public ::testing::Test {
        protected:
            UT_BOT_BODY() :
                          mMockLegs()
                          , mBody(mMockLegs) {
            }

            virtual void
            SetUp() {
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_BOT_BODY() = default;

            /* Mocks */
            StrictMock<Legs::MockLegs> mMockLegs;
            /* Test class */
            Body mBody;
        };


        TEST_F(UT_BOT_BODY, Initialize_Ok) {
            //const Core::Status success = mBody.Initialize();

            //EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    }
}