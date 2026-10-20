#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cmath>
#include <map>

#include "../../../../src/Bot/Leg/Leg.h"
#include "../../../mock/cmp/MockServo.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

namespace Bot
{
    namespace Leg
    {
        namespace
        {
            constexpr float RAD_TO_DEG = 57.2957795F;

            // Forward kinematics matching the IK angle convention, used to check round trips.
            Position3d ForwardKinematics(const Leg::SLegAngles &angles) {
                const float femurFromVertical = (180.0F - angles.femur) / RAD_TO_DEG;
                const float kneeInterior = (180.0F - angles.tibia) / RAD_TO_DEG;
                const float tibiaFromVertical = femurFromVertical + kneeInterior - M_PI;
                const float radial = Leg::COXA_LENGTH + (Leg::FEMUR_LENGTH * sin(femurFromVertical)) +
                                     (Leg::TIBIA_LENGTH * sin(tibiaFromVertical));
                const float z = (Leg::FEMUR_LENGTH * cos(femurFromVertical)) +
                                (Leg::TIBIA_LENGTH * cos(tibiaFromVertical));
                const float coxa = (angles.coxa - 90.0F) / RAD_TO_DEG;
                return {radial * cos(coxa), radial * sin(coxa), z};
            }
        } // namespace

        class UT_BOT_LEG : public ::testing::Test {
        protected:
            UT_BOT_LEG() {
                for (uint8_t id = 0U; id < Legs::NB_LEGS; id++) {
                    ON_CALL(mServos[(id * 3U)], SetAngle(_, _))
                            .WillByDefault(Return(Core::Status::CORE_OK));
                    ON_CALL(mServos[(id * 3U) + 1U], SetAngle(_, _))
                            .WillByDefault(Return(Core::Status::CORE_OK));
                    ON_CALL(mServos[(id * 3U) + 2U], SetAngle(_, _))
                            .WillByDefault(Return(Core::Status::CORE_OK));
                    mLegs.emplace(static_cast<Legs::ELeg>(id),
                                  Leg(static_cast<Legs::ELeg>(id),
                                      mServos[(id * 3U)],
                                      mServos[(id * 3U) + 1U],
                                      mServos[(id * 3U) + 2U]));
                }
            }

            NiceMock<Component::Servo::MockServo> mServos[Legs::NB_LEGS * 3U];
            std::map<Legs::ELeg, Leg>             mLegs;
        };

        TEST_F(UT_BOT_LEG, SolveIk_NeutralPoseIsNinety) {
            Leg::SLegAngles angles{};
            ASSERT_TRUE(Leg::SolveIk({Leg::COXA_LENGTH + Leg::FEMUR_LENGTH, 0.0F, Leg::TIBIA_LENGTH},
                                     angles));
            EXPECT_NEAR(angles.coxa, 90.0F, 0.01F);
            EXPECT_NEAR(angles.femur, 90.0F, 0.01F);
            EXPECT_NEAR(angles.tibia, 90.0F, 0.01F);
        }

        TEST_F(UT_BOT_LEG, SolveIk_RoundTripWithForwardKinematics) {
            const Position3d targets[] = {{11.2F, 0.0F, 12.7F},
                                          {13.0F, 2.0F, 10.0F},
                                          {9.0F, -3.0F, 14.0F},
                                          {12.0F, 4.0F, 9.5F},
                                          {10.0F, 0.0F, 15.0F}};
            for (const Position3d &target: targets) {
                Leg::SLegAngles angles{};
                ASSERT_TRUE(Leg::SolveIk(target, angles));
                const Position3d result = ForwardKinematics(angles);
                EXPECT_NEAR(result.x, target.x, 0.01F);
                EXPECT_NEAR(result.y, target.y, 0.01F);
                EXPECT_NEAR(result.z, target.z, 0.01F);
            }
        }

        TEST_F(UT_BOT_LEG, SolveIk_UnreachableTargetStaysFinite) {
            const Position3d targets[] = {{1000.0F, 0.0F, 0.0F},
                                          {0.0F, 0.0F, 0.0F},
                                          {Leg::COXA_LENGTH, 0.0F, 0.0F},
                                          {5.0F, 5.0F, -50.0F}};
            for (const Position3d &target: targets) {
                Leg::SLegAngles angles{};
                ASSERT_TRUE(Leg::SolveIk(target, angles));
                EXPECT_TRUE(std::isfinite(angles.coxa));
                EXPECT_TRUE(std::isfinite(angles.femur));
                EXPECT_TRUE(std::isfinite(angles.tibia));
            }
        }

        TEST_F(UT_BOT_LEG, SolveIk_RejectsNonFiniteTarget) {
            Leg::SLegAngles angles{};
            EXPECT_FALSE(Leg::SolveIk({NAN, 0.0F, 0.0F}, angles));
            EXPECT_FALSE(Leg::SolveIk({0.0F, INFINITY, 0.0F}, angles));
        }

        TEST_F(UT_BOT_LEG, SetLegIk_NeutralSendsNinetyOnEveryLeg) {
            for (auto &leg: mLegs) {
                const uint8_t id = static_cast<uint8_t>(leg.first);
                EXPECT_CALL(mServos[id * 3U], SetAngle(90U, 0U));
                EXPECT_CALL(mServos[(id * 3U) + 1U], SetAngle(90U, 0U));
                EXPECT_CALL(mServos[(id * 3U) + 2U], SetAngle(90U, 0U));
                EXPECT_EQ(leg.second.SetLegIk({0.0F, 0.0F, 0.0F}, 0U), Core::Status::CORE_OK);
            }
        }

        TEST_F(UT_BOT_LEG, SetLegBodyIk_NeutralSendsNinetyOnEveryLeg) {
            for (auto &leg: mLegs) {
                const uint8_t id = static_cast<uint8_t>(leg.first);
                EXPECT_CALL(mServos[id * 3U], SetAngle(90U, 1000U));
                EXPECT_CALL(mServos[(id * 3U) + 1U], SetAngle(90U, 1000U));
                EXPECT_CALL(mServos[(id * 3U) + 2U], SetAngle(90U, 1000U));
                EXPECT_EQ(leg.second.SetLegBodyIk({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F}, 1000U),
                          Core::Status::CORE_OK);
            }
        }

        TEST_F(UT_BOT_LEG, SetLegIk_CoxaStaysInSafeRange) {
            for (auto &leg: mLegs) {
                const uint8_t id = static_cast<uint8_t>(leg.first);
                EXPECT_CALL(mServos[id * 3U], SetAngle(96U, 0U));
                EXPECT_EQ(leg.second.SetLegIk({0.0F, 20.0F, 0.0F}, 0U), Core::Status::CORE_OK);
            }
            for (auto &leg: mLegs) {
                const uint8_t id = static_cast<uint8_t>(leg.first);
                EXPECT_CALL(mServos[id * 3U], SetAngle(84U, 0U));
                EXPECT_EQ(leg.second.SetLegIk({0.0F, -20.0F, 0.0F}, 0U), Core::Status::CORE_OK);
            }
        }

        TEST_F(UT_BOT_LEG, SetLegBodyIk_CoxaStaysInSafeRange) {
            const Position3d bodyShifts[] = {{30.0F, 0.0F, 0.0F},
                                             {-30.0F, 0.0F, 0.0F},
                                             {0.0F, 30.0F, 0.0F},
                                             {0.0F, -30.0F, 0.0F}};
            for (auto &leg: mLegs) {
                const uint8_t id = static_cast<uint8_t>(leg.first);
                EXPECT_CALL(mServos[id * 3U], SetAngle(::testing::AllOf(::testing::Ge(84), ::testing::Le(96)), 0U))
                        .Times(4);
                for (const Position3d &shift: bodyShifts) {
                    EXPECT_EQ(leg.second.SetLegBodyIk({0.0F, 0.0F, 0.0F}, shift, 0U),
                              Core::Status::CORE_OK);
                }
            }
        }

        TEST_F(UT_BOT_LEG, ComputeDirection_AllLegsTravelInTheSameBodyDirection) {
            // Swing end of the trajectory: the foot moves forward by one unit.
            const float directions[] = {0.0F, M_PI / 2.0F, M_PI, 3.0F * M_PI / 2.0F, 0.7F};
            for (const float direction: directions) {
                // Counter-clockwise from forward (90 = left), body frame x right, y forward.
                const float expectedX = -std::sin(direction);
                const float expectedY = std::cos(direction);

                for (auto &leg: mLegs) {
                    const Position3d foot = leg.second.GetFootPosition();
                    const float      mount = std::atan2(foot.y, foot.x);

                    Position3d       local = {0.0F, -1.0F, 0.0F};
                    leg.second.ComputeDirection(local, direction);

                    // The leg frame mirrors the body frame, so the lateral axis is flipped back.
                    const float bodyX = -((local.x * std::cos(mount)) - (local.y * std::sin(mount)));
                    const float bodyY = (local.x * std::sin(mount)) + (local.y * std::cos(mount));
                    EXPECT_NEAR(bodyX, expectedX, 1e-4F)
                            << "leg " << leg.first << " direction " << direction;
                    EXPECT_NEAR(bodyY, expectedY, 1e-4F)
                            << "leg " << leg.first << " direction " << direction;
                }
            }
        }

        TEST_F(UT_BOT_LEG, ComputeRotation_AllLegsTurnTheSameWay) {
            for (auto &leg: mLegs) {
                Position3d clockwise = {0.0F, 1.0F, 0.0F};
                Position3d counterClockwise = {0.0F, 1.0F, 0.0F};
                leg.second.ComputeRotation(clockwise, 1.0F, true);
                leg.second.ComputeRotation(counterClockwise, 1.0F, false);
                EXPECT_FLOAT_EQ(clockwise.y, 1.0F);
                EXPECT_FLOAT_EQ(counterClockwise.y, -1.0F);
            }
        }

        TEST_F(UT_BOT_LEG, ComputeAmplitudeAndElevation_KeepFractionalValues) {
            Leg        &leg = mLegs.at(Legs::ELeg::MIDDLE_LEFT);
            Position3d position = {1.0F, 1.0F, 1.0F};
            leg.ComputeAmplitude(position, 0.5F);
            leg.ComputeElevation(position, 0.5F);
            EXPECT_FLOAT_EQ(position.x, 0.5F);
            EXPECT_FLOAT_EQ(position.y, 0.5F);
            EXPECT_FLOAT_EQ(position.z, -0.5F);
        }
    } // namespace Leg
} // namespace Bot
