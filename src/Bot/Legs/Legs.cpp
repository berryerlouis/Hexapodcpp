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
            LOG_BOT_DEBUG("Leg",
                          "Legs Initialized");
        }

        std::map<ELeg, Leg::Leg> &Legs::GetLegs(void) {
            return this->mLegs;
        }

        Leg::LegInterface *Legs::GetLeg(const ELeg legId) {
            const auto it = this->mLegs.find(legId);
            if (it != this->mLegs.end()) {
                return &it->second;
            }
            return nullptr;
        }

        Core::Status Legs::Update(void) {
            for (auto &leg: this->mLegs) {
                leg.second.Update();
            }
            return Core::Status::CORE_OK;
        }
    }
}
