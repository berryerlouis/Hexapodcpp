#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <map>

#include "../../../../src/Bot/Legs/LegsInterface.h"
#include "../../../../src/Move/Gait/GaitCycle.h"
#include "../../../mock/cmp/MockServo.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;

namespace Move
{
    namespace Gait
    {
        namespace
        {
            using Bot::Leg::Leg;
            using Vec = std::array<float, 3U>;

            constexpr float DEG_TO_RAD = 0.0174532925F;
            // Two legs are about 4 cm wide, so centerlines closer than this touch.
            constexpr float MIN_CLEARANCE_CM = 5.0F;

            struct SLegShape {
                Vec femurAxis;
                Vec knee;
                Vec foot;
            };

            Vec Sub(const Vec &a, const Vec &b) {
                return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
            }

            float Dot(const Vec &a, const Vec &b) {
                return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
            }

            float Clamp01(const float v) {
                return std::min(1.0F, std::max(0.0F, v));
            }

            float SegmentDistance(const Vec &p1, const Vec &q1, const Vec &p2, const Vec &q2) {
                const Vec   d1 = Sub(q1, p1);
                const Vec   d2 = Sub(q2, p2);
                const Vec   r = Sub(p1, p2);
                const float a = Dot(d1, d1);
                const float e = Dot(d2, d2);
                const float f = Dot(d2, r);
                const float c = Dot(d1, r);
                const float b = Dot(d1, d2);
                const float den = (a * e) - (b * b);
                float       s = (den > 1e-9F) ? Clamp01(((b * f) - (c * e)) / den) : 0.0F;
                float       t = ((b * s) + f) / e;
                if (t < 0.0F) {
                    t = 0.0F;
                    s = Clamp01(-c / a);
                } else if (t > 1.0F) {
                    t = 1.0F;
                    s = Clamp01((b - c) / a);
                }
                const Vec n = {(p1[0] + (d1[0] * s)) - (p2[0] + (d2[0] * t)),
                               (p1[1] + (d1[1] * s)) - (p2[1] + (d2[1] * t)),
                               (p1[2] + (d1[2] * s)) - (p2[2] + (d2[2] * t))};
                return std::sqrt(Dot(n, n));
            }

            float LegDistance(const SLegShape &a, const SLegShape &b) {
                float distance = 1e9F;
                for (const auto &sa: {std::pair<Vec, Vec>{a.femurAxis, a.knee},
                                      std::pair<Vec, Vec>{a.knee, a.foot}}) {
                    for (const auto &sb: {std::pair<Vec, Vec>{b.femurAxis, b.knee},
                                          std::pair<Vec, Vec>{b.knee, b.foot}}) {
                        distance = std::min(
                                distance, SegmentDistance(sa.first, sa.second, sb.first, sb.second));
                    }
                }
                return distance;
            }

            class FakeLegs : public Bot::Legs::LegsInterface {
            public:
                explicit FakeLegs(std::map<Bot::Legs::ELeg, Leg> &legs)
                    : mLegs(legs) {
                }

                std::map<Bot::Legs::ELeg, Leg> &GetLegs() override {
                    return this->mLegs;
                }

                Bot::Leg::LegInterface *GetLeg(const Bot::Legs::ELeg legId) override {
                    return &this->mLegs.at(legId);
                }

                Core::Status Update() override {
                    for (auto &leg: this->mLegs) {
                        leg.second.Update();
                    }
                    return Core::Status::CORE_OK;
                }

            private:
                std::map<Bot::Legs::ELeg, Leg> &mLegs;
            };
        } // namespace

        class UT_MOVE_GAIT_COLLISION : public ::testing::Test {
        protected:
            UT_MOVE_GAIT_COLLISION() {
                for (uint8_t id = 0U; id < Bot::Legs::NB_LEGS; id++) {
                    for (uint8_t joint = 0U; joint < 3U; joint++) {
                        const size_t servo = (id * 3U) + joint;
                        ON_CALL(mServos[servo], SetAngle(_, _))
                                .WillByDefault(Invoke([this, servo](const uint8_t angle, const uint16_t) {
                                    mAngles[servo] = angle;
                                    return Core::Status::CORE_OK;
                                }));
                    }
                    mLegs.emplace(static_cast<Bot::Legs::ELeg>(id),
                                  Leg(static_cast<Bot::Legs::ELeg>(id),
                                      mServos[(id * 3U)],
                                      mServos[(id * 3U) + 1U],
                                      mServos[(id * 3U) + 2U]));
                }
            }

            SLegShape Shape(const uint8_t id) {
                const Leg  &leg = mLegs.at(static_cast<Bot::Legs::ELeg>(id));
                const float coxa = (mAngles[id * 3U] - 90.0F) * DEG_TO_RAD;
                const float femurFromVertical = (180.0F - mAngles[(id * 3U) + 1U]) * DEG_TO_RAD;
                const float kneeInterior = (180.0F - mAngles[(id * 3U) + 2U]) * DEG_TO_RAD;
                const float tibiaFromVertical = femurFromVertical + kneeInterior - M_PI;
                const float heading = std::atan2(leg.GetFootPosition().y, leg.GetFootPosition().x) + coxa;

                const auto point = [&](const float radial, const float z) {
                    return Vec{leg.GetBodyCenterOffsetX() + (radial * std::cos(heading)),
                               leg.GetBodyCenterOffsetY() + (radial * std::sin(heading)),
                               z};
                };
                const float kneeRadial = Leg::COXA_LENGTH + (Leg::FEMUR_LENGTH * std::sin(femurFromVertical));
                const float kneeZ = Leg::FEMUR_LENGTH * std::cos(femurFromVertical);
                return {point(Leg::COXA_LENGTH, 0.0F),
                        point(kneeRadial, kneeZ),
                        point(kneeRadial + (Leg::TIBIA_LENGTH * std::sin(tibiaFromVertical)),
                              kneeZ + (Leg::TIBIA_LENGTH * std::cos(tibiaFromVertical)))};
            }

            // Smallest centerline distance between two legs, over a full walk.
            float WalkMinClearance(const GaitType gait,
                                   const float    direction,
                                   const float    rotation,
                                   const bool     clockWise) {
                GaitParams params;
                FakeLegs   legs(mLegs);
                GaitCycle  cycle(legs, params);
                params.SetDirection(direction);
                params.SetRotation(rotation, clockWise);
                cycle.SetGaitType(gait);
                cycle.Start();

                float minimum = 1e9F;
                for (uint64_t time = 0U; time < 8000U; time += 20U) {
                    cycle.Update(time);
                    std::array<SLegShape, Bot::Legs::NB_LEGS> shapes;
                    for (uint8_t id = 0U; id < Bot::Legs::NB_LEGS; id++) {
                        shapes[id] = this->Shape(id);
                    }
                    for (uint8_t a = 0U; a < Bot::Legs::NB_LEGS; a++) {
                        for (uint8_t b = a + 1U; b < Bot::Legs::NB_LEGS; b++) {
                            minimum = std::min(minimum, LegDistance(shapes[a], shapes[b]));
                        }
                    }
                }
                return minimum;
            }

            NiceMock<Component::Servo::MockServo>     mServos[Bot::Legs::NB_LEGS * 3U];
            std::array<uint8_t, Bot::Legs::NB_LEGS * 3U> mAngles{};
            std::map<Bot::Legs::ELeg, Leg>            mLegs;
        };

        TEST_F(UT_MOVE_GAIT_COLLISION, Walk_LegsNeverCollide) {
            const GaitType gaits[] = {GaitType::TRIPOD, GaitType::WAVE, GaitType::RIPPLE};
            for (const GaitType gait: gaits) {
                for (int degrees = 0; degrees < 360; degrees += 15) {
                    EXPECT_GE(WalkMinClearance(gait, degrees * DEG_TO_RAD, 0.0F, false),
                              MIN_CLEARANCE_CM)
                            << GaitTypeToString(gait) << " direction " << degrees;
                }
            }
        }

        TEST_F(UT_MOVE_GAIT_COLLISION, Rotate_LegsNeverCollide) {
            const GaitType gaits[] = {GaitType::TRIPOD, GaitType::WAVE, GaitType::RIPPLE};
            for (const GaitType gait: gaits) {
                EXPECT_GE(WalkMinClearance(gait, 0.0F, 1.0F, true), MIN_CLEARANCE_CM)
                        << GaitTypeToString(gait) << " clockwise";
                EXPECT_GE(WalkMinClearance(gait, 0.0F, 1.0F, false), MIN_CLEARANCE_CM)
                        << GaitTypeToString(gait) << " counter clockwise";
            }
        }
    } // namespace Gait
} // namespace Move
