#include "Legs.h"
#include "../../Misc/Maths/Utils.h"

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

        void Legs::SetTarget(const Move::Gait::GaitParams &gaitParams,
                             const std::vector<std::vector<Misc::Maths::Position3d> > &positions,
                             const uint8_t stepPositionIndex,
                             const float normalizedTime) {
            uint8_t stepPositionIndexAlt = stepPositionIndex;
            for (auto &leg: this->mLegs) {
                if (leg.second.GetId() % 2U == 0U) {
                    stepPositionIndexAlt = (stepPositionIndex + 1U) % positions.size();
                } else {
                    stepPositionIndexAlt = stepPositionIndex;
                }
                Misc::Maths::Position3d position = Misc::Utils::QuadraticLerp(
                        positions[stepPositionIndexAlt][0U],
                        positions[stepPositionIndexAlt][1U],
                        positions[stepPositionIndexAlt][2U],
                        normalizedTime
                        );
                if (false == gaitParams.IsRotated()) {
                    leg.second.ComputeDirection(position, gaitParams.GetDirection());
                } else {
                    leg.second.ComputeRotation(position, gaitParams.GetRotation(),
                                               gaitParams.GetRotationClockWize());
                }
                leg.second.ComputeAmplitude(position, gaitParams.GetAmplitude());
                leg.second.ComputeElevation(position, gaitParams.GetElevation());
                leg.second.SetTarget(position);
            }
        }
    }
}