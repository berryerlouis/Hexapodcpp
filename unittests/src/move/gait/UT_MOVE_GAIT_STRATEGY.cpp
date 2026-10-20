#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <map>

#include "../../../../src/Move/Gait/GaitParams.h"
#include "../../../../src/Move/Gait/GaitStrategy.h"
#include "../../../mock/cmp/MockServo.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

namespace Move
{
    namespace Gait
    {
        class UT_MOVE_GAIT_STRATEGY : public ::testing::Test {
        protected:
            UT_MOVE_GAIT_STRATEGY() {
                for (uint8_t id = 0U; id < Bot::Legs::NB_LEGS; id++) {
                    for (uint8_t servo = 0U; servo < 3U; servo++) {
                        ON_CALL(mServos[(id * 3U) + servo], SetAngle(_, _))
                                .WillByDefault(Return(Core::Status::CORE_OK));
                    }
                    mLegs.emplace(static_cast<Bot::Legs::ELeg>(id),
                                  Bot::Leg::Leg(static_cast<Bot::Legs::ELeg>(id),
                                                mServos[(id * 3U)],
                                                mServos[(id * 3U) + 1U],
                                                mServos[(id * 3U) + 2U]));
                }
            }

            // A leg is in swing when it is lifted (z > 0 in the leg target convention).
            uint8_t CountSwingLegs() {
                uint8_t count = 0U;
                for (auto &leg: mLegs) {
                    if (leg.second.GetCurrentPosition().z > 1e-4F) {
                        count++;
                    }
                }
                return count;
            }

            void ExpectSwingCountOverCycle(GaitStrategy &strategy, const uint8_t expected) {
                for (uint8_t i = 0U; i < 40U; i++) {
                    const float phase = 0.0317F + (0.05F * static_cast<float>(i));
                    strategy.doGaitStrategy(mLegs, mParams, mTrajectory, phase, 1.0F);
                    EXPECT_EQ(CountSwingLegs(), expected) << "phase " << phase;
                }
            }

            NiceMock<Component::Servo::MockServo>    mServos[Bot::Legs::NB_LEGS * 3U];
            std::map<Bot::Legs::ELeg, Bot::Leg::Leg> mLegs;
            GaitParams                               mParams;
            const GaitTrajectory                     mTrajectory = {{{{{0.0F, 1.0F, 0.0F},
                                                                       {0.0F, 0.0F, -2.0F},
                                                                       {0.0F, -1.0F, 0.0F}}},
                                                                     {{{0.0F, -1.0F, 0.0F},
                                                                       {0.0F, 0.0F, 0.1F},
                                                                       {0.0F, 1.0F, 0.0F}}}}};
        };

        TEST_F(UT_MOVE_GAIT_STRATEGY, Tripod_ThreeLegsInSwingAtAllTimes) {
            GaitTripod tripod;
            ExpectSwingCountOverCycle(tripod, 3U);
        }

        TEST_F(UT_MOVE_GAIT_STRATEGY, Wave_OneLegInSwingAtAllTimes) {
            GaitWave wave;
            ExpectSwingCountOverCycle(wave, 1U);
        }

        TEST_F(UT_MOVE_GAIT_STRATEGY, Ripple_TwoLegsInSwingAtAllTimes) {
            GaitRipple ripple;
            ExpectSwingCountOverCycle(ripple, 2U);
        }

        TEST_F(UT_MOVE_GAIT_STRATEGY, Tripod_TripodsAreOppositeSides) {
            GaitTripod tripod;
            tripod.doGaitStrategy(mLegs, mParams, mTrajectory, 0.5F, 1.0F);
            const bool frontLeftSwing =
                    mLegs.at(Bot::Legs::ELeg::FRONT_LEFT).GetCurrentPosition().z > 0.0F;
            EXPECT_EQ(mLegs.at(Bot::Legs::ELeg::REAR_LEFT).GetCurrentPosition().z > 0.0F,
                      frontLeftSwing);
            EXPECT_EQ(mLegs.at(Bot::Legs::ELeg::MIDDLE_RIGHT).GetCurrentPosition().z > 0.0F,
                      frontLeftSwing);
            EXPECT_NE(mLegs.at(Bot::Legs::ELeg::MIDDLE_LEFT).GetCurrentPosition().z > 0.0F,
                      frontLeftSwing);
        }

        TEST_F(UT_MOVE_GAIT_STRATEGY, ZeroScaleKeepsEveryLegAtNeutral) {
            GaitTripod tripod;
            tripod.doGaitStrategy(mLegs, mParams, mTrajectory, 0.7F, 0.0F);
            for (auto &leg: mLegs) {
                const Misc::Maths::Position3d position = leg.second.GetCurrentPosition();
                EXPECT_FLOAT_EQ(position.x, 0.0F);
                EXPECT_FLOAT_EQ(position.y, 0.0F);
                EXPECT_FLOAT_EQ(position.z, 0.0F);
            }
        }

        TEST_F(UT_MOVE_GAIT_STRATEGY, SwingPeakReachesTheRequestedElevation) {
            GaitTripod tripod;
            // Phase 0.5 of the first segment is the middle of the swing for the legs starting at 0.
            tripod.doGaitStrategy(mLegs, mParams, mTrajectory, 0.5F, 1.0F);
            EXPECT_NEAR(mLegs.at(Bot::Legs::ELeg::FRONT_LEFT).GetCurrentPosition().z,
                        mParams.GetElevation(),
                        1e-3F);
        }
    } // namespace Gait
} // namespace Move
