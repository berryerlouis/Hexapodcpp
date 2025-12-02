#include "Legs.h"
#include "../../Misc/Maths/Utils.h"
#include <cmath>

namespace Bot
{
    namespace Legs
    {
        Legs::Legs(ServosInterface &servos) :
            mLegs{
                    {
                            FRONT_LEFT,
                            Leg::Leg(FRONT_LEFT, *servos.GetServo(SERVO_0),
                                     *servos.GetServo(SERVO_1),
                                     *servos.GetServo(SERVO_2))
                    },
                    {
                            MIDDLE_LEFT,
                            Leg::Leg(MIDDLE_LEFT, *servos.GetServo(SERVO_3),
                                     *servos.GetServo(SERVO_4),
                                     *servos.GetServo(SERVO_5))
                    },
                    {
                            REAR_LEFT,
                            Leg::Leg(REAR_LEFT, *servos.GetServo(SERVO_6),
                                     *servos.GetServo(SERVO_7),
                                     *servos.GetServo(SERVO_8))
                    },
                    {
                            REAR_RIGHT,
                            Leg::Leg(REAR_RIGHT, *servos.GetServo(SERVO_9),
                                     *servos.GetServo(SERVO_10),
                                     *servos.GetServo(SERVO_11))
                    },
                    {
                            MIDDLE_RIGHT,
                            Leg::Leg(MIDDLE_RIGHT, *servos.GetServo(SERVO_12),
                                     *servos.GetServo(SERVO_13),
                                     *servos.GetServo(SERVO_14))
                    },
                    {
                            FRONT_RIGHT,
                            Leg::Leg(FRONT_RIGHT, *servos.GetServo(SERVO_15),
                                     *servos.GetServo(SERVO_16),
                                     *servos.GetServo(SERVO_17))
                    }
            } {
            LOG_BOT_DEBUG("Leg", "Legs Initialized");
        }


        Leg::LegInterface *Legs::GetLeg(const ELeg legId) {
            const auto it = this->mLegs.find(legId);
            if (it != this->mLegs.end()) {
                return &it->second;
            }
            return nullptr;
        }

        void Legs::ResetLegs(const uint16_t cycleDuration) {
            for (auto &leg: this->mLegs) {
                leg.second.ResetTarget();
                leg.second.SetLegIk({0.0F, 0.0F, 0.0F}, cycleDuration);
            }
        }

        Core::Status Legs::Update(void) {
            for (auto &leg: this->mLegs) {
                leg.second.Update();
            }
            return Core::Status::CORE_OK;
        }

        void Legs::ComputeTarget(const uint64_t currentTime,
                             const Move::Gait::GaitParams &gaitParams,
                             const std::vector<std::vector<Misc::Maths::Position3d> > &positions,
                             const uint8_t stepPositionIndex,
                             const float normalizedTime) {
            uint8_t stepPositionIndexAlt = stepPositionIndex;
            for (auto &leg: this->mLegs) {
                uint8_t legId = static_cast<uint8_t>(leg.second.GetId());
                stepPositionIndexAlt = this->GetStepIndexPosition(legId,
                                     static_cast<uint8_t>(positions.size()),
                                     stepPositionIndex,
                                     gaitParams.GetGaitType());

                Misc::Maths::Position3d position = Misc::Utils::QuadraticLerp(
                        positions[stepPositionIndexAlt][0U],
                        positions[stepPositionIndexAlt][1U],
                        positions[stepPositionIndexAlt][2U],
                        normalizedTime
                        );
                leg.second.ComputeLerpTarget(currentTime, position, gaitParams);
                leg.second.SetTarget(position);
            }
        }

        uint8_t Legs::GetStepIndexPosition(const uint8_t legId,
                                           const uint8_t nbPositions,
                                           const uint8_t stepPositionIndex, 
                                           const Move::Gait::GaitType gaitType) {
            uint8_t stepPositionIndexAlt = stepPositionIndex;
            if (gaitType == Move::Gait::GaitType::TRIPOD) {
                // TRIPOD: 2 groups alternating
                // Group 0: FRONT_LEFT(0), REAR_LEFT(2), MIDDLE_RIGHT(4)
                // Group 1: MIDDLE_LEFT(1), REAR_RIGHT(3), FRONT_RIGHT(5)
                if (legId % 2U == 0U) {
                    stepPositionIndexAlt = stepPositionIndex;
                } else {
                    stepPositionIndexAlt = (stepPositionIndex + 1U) % nbPositions;
                }
            } else if (gaitType == Move::Gait::GaitType::WAVE) {
                // WAVE: 6 sequential phases, one leg at a time
                // Each leg offset by 1 position in sequence
                mLegIdInWaveGait++;
            } else if (gaitType == Move::Gait::GaitType::RIPPLE) {
                // RIPPLE: 3 pairs of legs (2 legs per phase)
                // Pair 0: legs 0-1, Pair 1: legs 2-3, Pair 2: legs 4-5
                stepPositionIndexAlt = (stepPositionIndex + (legId / 2U)) % nbPositions;
            }
            return stepPositionIndexAlt;
        }
    }
}