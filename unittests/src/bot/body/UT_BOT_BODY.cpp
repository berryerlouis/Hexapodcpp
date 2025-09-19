#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockTick.h"
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
                mMockTick(),
                mMockLegs(),
                mBody(mMockLegs, mMockTick) {
            }

            virtual void
            SetUp() {
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_BOT_BODY() = default;

            /* Mocks */
            StrictMock<Driver::Tick::MockTick> mMockTick;
            StrictMock<Legs::MockLegs> mMockLegs;
            /* Test class */
            Body mBody;
        };


        TEST_F(UT_BOT_BODY, Initialize_Ok) {
            StrictMock<Leg::MockLeg> leg0, leg1, leg2, leg3, leg4, leg5;
            constexpr Position3d pos = {};
            EXPECT_CALL(leg0, GetFootPosition()).Times(2U).WillRepeatedly(Return(pos));
            EXPECT_CALL(leg0, GetBodyCenterOffsetX()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg0, GetBodyCenterOffsetY()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg0, SetLegBodyIk(pos,_,_)).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockLegs, GetLeg(Legs::FRONT_LEFT)).WillRepeatedly(Return(&leg0));

            EXPECT_CALL(leg1, GetFootPosition()).Times(2U).WillRepeatedly(Return(pos));
            EXPECT_CALL(leg1, GetBodyCenterOffsetX()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg1, GetBodyCenterOffsetY()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg1, SetLegBodyIk(pos,_,_)).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockLegs, GetLeg(Legs::MIDDLE_LEFT)).WillRepeatedly(Return(&leg1));

            EXPECT_CALL(leg2, GetFootPosition()).Times(2U).WillRepeatedly(Return(pos));
            EXPECT_CALL(leg2, GetBodyCenterOffsetX()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg2, GetBodyCenterOffsetY()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg2, SetLegBodyIk(pos,_,_)).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockLegs, GetLeg(Legs::REAR_LEFT)).WillRepeatedly(Return(&leg2));

            EXPECT_CALL(leg3, GetFootPosition()).Times(2U).WillRepeatedly(Return(pos));
            EXPECT_CALL(leg3, GetBodyCenterOffsetX()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg3, GetBodyCenterOffsetY()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg3, SetLegBodyIk(pos,_,_)).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockLegs, GetLeg(Legs::FRONT_RIGHT)).WillRepeatedly(Return(&leg3));

            EXPECT_CALL(leg4, GetFootPosition()).Times(2U).WillRepeatedly(Return(pos));
            EXPECT_CALL(leg4, GetBodyCenterOffsetX()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg4, GetBodyCenterOffsetY()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg4, SetLegBodyIk(pos,_,_)).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockLegs, GetLeg(Legs::MIDDLE_RIGHT)).WillRepeatedly(Return(&leg4));

            EXPECT_CALL(leg5, GetFootPosition()).Times(2U).WillRepeatedly(Return(pos));
            EXPECT_CALL(leg5, GetBodyCenterOffsetX()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg5, GetBodyCenterOffsetY()).WillOnce(Return(0.0F));
            EXPECT_CALL(leg5, SetLegBodyIk(pos,_,_)).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockLegs, GetLeg(Legs::REAR_RIGHT)).WillRepeatedly(Return(&leg5));

            const Core::Status success = mBody.Initialize();

            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    }
}
