#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/Bot/Body/Body.h"
#include "../../../../src/Cluster/Battery/ClusterBattery.h"
#include "../../../mock/bot/MockLeg.h"
#include "../../../mock/bot/MockLegs.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Bot
{
    namespace Body
    {
        class UT_BOT_BODY : public ::testing::Test {
        protected:
            UT_BOT_BODY()
                : mMockLeg0()
                , mMockLeg1()
                , mMockLeg2()
                , mMockLeg3()
                , mMockLeg4()
                , mMockLeg5()
                , mMockLegs()
                , mBody(mMockLegs) {
            }

            virtual void SetUp() {
            }

            virtual void TearDown() {
            }

            virtual ~UT_BOT_BODY() = default;

            /* Mocks */
            StrictMock<Leg::MockLeg>   mMockLeg0;
            StrictMock<Leg::MockLeg>   mMockLeg1;
            StrictMock<Leg::MockLeg>   mMockLeg2;
            StrictMock<Leg::MockLeg>   mMockLeg3;
            StrictMock<Leg::MockLeg>   mMockLeg4;
            StrictMock<Leg::MockLeg>   mMockLeg5;
            StrictMock<Legs::MockLegs> mMockLegs;
            /* Test class */
            Body mBody;
        };


        TEST_F(UT_BOT_BODY, Initialize_Ok) {
            // Setup default position return value
            Misc::Maths::Position3d defaultPos = {0.0F, 0.0F, 0.0F};

            // Setup expectations for all 6 legs
            EXPECT_CALL(mMockLegs, GetLeg(Legs::ELeg::FRONT_LEFT))
                    .WillOnce(Return(&mMockLeg0));
            EXPECT_CALL(mMockLeg0, GetFootPosition())
                    .WillRepeatedly(Return(defaultPos));
            EXPECT_CALL(mMockLeg0, GetBodyCenterOffsetX())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg0, GetBodyCenterOffsetY())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg0, SetLegBodyIk(_, _, 1000))
                    .WillOnce(Return(Core::Status::CORE_OK));

            EXPECT_CALL(mMockLegs, GetLeg(Legs::ELeg::MIDDLE_LEFT))
                    .WillOnce(Return(&mMockLeg1));
            EXPECT_CALL(mMockLeg1, GetFootPosition())
                    .WillRepeatedly(Return(defaultPos));
            EXPECT_CALL(mMockLeg1, GetBodyCenterOffsetX())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg1, GetBodyCenterOffsetY())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg1, SetLegBodyIk(_, _, 1000))
                    .WillOnce(Return(Core::Status::CORE_OK));

            EXPECT_CALL(mMockLegs, GetLeg(Legs::ELeg::REAR_LEFT))
                    .WillOnce(Return(&mMockLeg2));
            EXPECT_CALL(mMockLeg2, GetFootPosition())
                    .WillRepeatedly(Return(defaultPos));
            EXPECT_CALL(mMockLeg2, GetBodyCenterOffsetX())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg2, GetBodyCenterOffsetY())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg2, SetLegBodyIk(_, _, 1000))
                    .WillOnce(Return(Core::Status::CORE_OK));

            EXPECT_CALL(mMockLegs, GetLeg(Legs::ELeg::FRONT_RIGHT))
                    .WillOnce(Return(&mMockLeg3));
            EXPECT_CALL(mMockLeg3, GetFootPosition())
                    .WillRepeatedly(Return(defaultPos));
            EXPECT_CALL(mMockLeg3, GetBodyCenterOffsetX())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg3, GetBodyCenterOffsetY())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg3, SetLegBodyIk(_, _, 1000))
                    .WillOnce(Return(Core::Status::CORE_OK));

            EXPECT_CALL(mMockLegs, GetLeg(Legs::ELeg::MIDDLE_RIGHT))
                    .WillOnce(Return(&mMockLeg4));
            EXPECT_CALL(mMockLeg4, GetFootPosition())
                    .WillRepeatedly(Return(defaultPos));
            EXPECT_CALL(mMockLeg4, GetBodyCenterOffsetX())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg4, GetBodyCenterOffsetY())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg4, SetLegBodyIk(_, _, 1000))
                    .WillOnce(Return(Core::Status::CORE_OK));

            EXPECT_CALL(mMockLegs, GetLeg(Legs::ELeg::REAR_RIGHT))
                    .WillOnce(Return(&mMockLeg5));
            EXPECT_CALL(mMockLeg5, GetFootPosition())
                    .WillRepeatedly(Return(defaultPos));
            EXPECT_CALL(mMockLeg5, GetBodyCenterOffsetX())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg5, GetBodyCenterOffsetY())
                    .WillRepeatedly(Return(0.0F));
            EXPECT_CALL(mMockLeg5, SetLegBodyIk(_, _, 1000))
                    .WillOnce(Return(Core::Status::CORE_OK));

            const Core::Status success = mBody.Initialize();

            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    } // namespace Body
} // namespace Bot
