#include "Leg.h"

#include <cstdio>

#include "../../Misc/Maths/Utils.h"

namespace Bot
{
    namespace Leg
    {
        Leg::Leg(const Legs::ELeg legId,
                 ServoInterface  &coxa,
                 ServoInterface  &femur,
                 ServoInterface  &tibia) :
            mBodyCenterOffsetX{0.0F},
            mBodyCenterOffsetY{0.0F},
            mFootPosition{0.0F,
                          0.0F,
                          0.0F},
            mLegIk{{0.0F,
                    0.0F,
                    0.0F},
                   0.0F,
                   0.0F,
                   0.0F,
                   0.0F,
                   0.0F,
                   0.0F,
                   0.0F,
                   0.0F},
            mStartPos{0.0F,
                      0.0F,
                      0.0F},
            mCurrentPos{0.0F,
                        0.0F,
                        0.0F},
            mLegId(legId),
            mCoxa(coxa),
            mFemur(femur),
            mTibia(tibia) {
            switch (mLegId) {
                case Legs::ELeg::FRONT_LEFT:
                    this->mBodyCenterOffsetX = -BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = -cos(Misc::Utils::ToRad(60.0F)) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = sin(Misc::Utils::ToRad(60.0F)) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case Legs::ELeg::MIDDLE_LEFT:
                    this->mBodyCenterOffsetX = -BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = -(COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = 0.0F;
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case Legs::ELeg::REAR_LEFT:
                    this->mBodyCenterOffsetX = -BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = -BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = -cos(Misc::Utils::ToRad(60.0F)) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = -sin(Misc::Utils::ToRad(60.0F)) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case Legs::ELeg::FRONT_RIGHT:
                    this->mBodyCenterOffsetX = BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = cos(Misc::Utils::ToRad(60.0F)) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = sin(Misc::Utils::ToRad(60.0F)) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case Legs::ELeg::MIDDLE_RIGHT:
                    this->mBodyCenterOffsetX = BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = 0.0F;
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case Legs::ELeg::REAR_RIGHT:
                    this->mBodyCenterOffsetX = BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = -BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = cos(Misc::Utils::ToRad(60.0F)) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = -sin(Misc::Utils::ToRad(60.0F)) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;
            }
            LOG_BOT_DEBUG("Leg", "Leg %s(%d) Initialized", ElegToString(this->mLegId).c_str(), this->mLegId);
        }

        Legs::ELeg Leg::GetId(void) const {
            return this->mLegId;
        }

        void Leg::ComputeDirection(Position3d &position,
                                   const float angleDirection) const {
            Position3d  rotatePosition = position;
            const float clockwize = (this->GetId() < Legs::NB_LEGS / 2U) ? 1.0F : -1.0F;
            const float newAngle = (this->GetId() < Legs::NB_LEGS / 2U) ? angleDirection : -angleDirection - M_PI;

            rotatePosition.x = (position.x * cos(newAngle)) + (position.y * clockwize * sin(newAngle));
            rotatePosition.y = (position.y * cos(newAngle)) - (position.x * clockwize * sin(newAngle));
            position = rotatePosition;
        }

        void Leg::ComputeRotation(Position3d &position,
                                  const float angleRotation,
                                  const bool  clockwize) const {
            (void) angleRotation;
            if (clockwize == false) {
                if (this->GetId() <= Legs::ELeg::REAR_LEFT) {
                    position.y *= -1.0F;
                }
            } else {
                if (this->GetId() > Legs::ELeg::REAR_LEFT) {
                    position.y *= -1.0F;
                }
            }
            this->ComputeDirection(position, 0U);
        }

        void Leg::ComputeAmplitude(Position3d   &position,
                                   const uint8_t amplitude) {
            position.x *= amplitude;
            position.y *= amplitude;
        }

        void Leg::ComputeElevation(Position3d   &position,
                                   const uint8_t elevation) {
            position.z *= elevation * -1.0F;
        }

        Position3d Leg::GetFootPosition() const {
            return this->mFootPosition;
        }

        float Leg::GetBodyCenterOffsetX(void) const {
            return this->mBodyCenterOffsetX;
        }

        float Leg::GetBodyCenterOffsetY(void) const {
            return this->mBodyCenterOffsetY;
        }

        void Leg::SetTarget(const Position3d &target) {
            this->mCurrentPos = target;
        }

        Position3d &Leg::GetCurrentPosition(void) {
            return this->mCurrentPos;
        }

        Core::Status Leg::Update(void) {
            return this->SetLegIk(this->mCurrentPos);
        }

        void Leg::ComputeLerpTarget(const uint64_t           currentTime,
                                    Misc::Maths::Position3d &position,
                                    const float              amplitude,
                                    const float              elevation,
                                    const float              direction,
                                    const bool               isRotated,
                                    const float              rotation,
                                    const bool               clockWize,
                                    const uint64_t           timeStamp) {
            if (false == isRotated) {
                this->ComputeDirection(position, direction);
            } else {
                this->ComputeRotation(position, rotation, clockWize);
            }
            this->ComputeAmplitude(position, amplitude);
            this->ComputeElevation(position, elevation);

            uint64_t timeDiff = currentTime - timeStamp;
            if (timeDiff < 500U) {
                if (this->mStartPos.x == INFINITY && this->mStartPos.y == INFINITY && this->mStartPos.z == INFINITY) {
                    this->mStartPos = this->mCurrentPos;
                }
                position = Misc::Utils::LerpPosition(this->mStartPos, position, static_cast<float>(timeDiff) / 500.0F);
                this->mCurrentPos = position;
            } else {
                this->mStartPos = {INFINITY, INFINITY, INFINITY};
            }
        }

        Core::Status Leg::SetLegIk(const Position3d &position,
                                   const uint16_t    travelTime) {
            this->mLegIk.newFootPos.x = position.x + (COXA_LENGTH + FEMUR_LENGTH);
            this->mLegIk.newFootPos.y = position.y + 0;
            this->mLegIk.newFootPos.z = position.z + TIBIA_LENGTH;

            // compute angles from distances
            this->mLegIk.coxaFootDist = sqrt(this->mLegIk.newFootPos.x * this->mLegIk.newFootPos.x +
                                             this->mLegIk.newFootPos.y * this->mLegIk.newFootPos.y);
            this->mLegIk.iksw =
                    sqrt(((this->mLegIk.coxaFootDist - COXA_LENGTH) * (this->mLegIk.coxaFootDist - COXA_LENGTH)) +
                         (this->mLegIk.newFootPos.z * this->mLegIk.newFootPos.z));
            this->mLegIk.ika1 = atan((this->mLegIk.coxaFootDist - COXA_LENGTH) / this->mLegIk.newFootPos.z);
            this->mLegIk.ika2 = acos(((TIBIA_LENGTH * TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH) -
                                      (this->mLegIk.iksw * this->mLegIk.iksw)) /
                                     (-2.0F * this->mLegIk.iksw * FEMUR_LENGTH));
            this->mLegIk.tangle = acos(((this->mLegIk.iksw * this->mLegIk.iksw) - (TIBIA_LENGTH * TIBIA_LENGTH) -
                                        (FEMUR_LENGTH * FEMUR_LENGTH)) /
                                       (-2.0F * FEMUR_LENGTH * TIBIA_LENGTH));
            this->mLegIk.tibiaIk = 90.0F + (90.0F - this->mLegIk.tangle * 180.0F / M_PI);
            this->mLegIk.femurIk = 90.0F + (90.0F - (this->mLegIk.ika1 + this->mLegIk.ika2) * 180.0F / M_PI);
            this->mLegIk.coxaIk = 90.0F + atan2(this->mLegIk.newFootPos.y, this->mLegIk.newFootPos.x) * 180.0F / M_PI;

            // Normalize angle: first wrap to -180 to +180 range
            this->mLegIk.coxaIk = fmod(this->mLegIk.coxaIk + 180.0F, 360.0F) - 180.0F;
            if (this->mLegIk.coxaIk < 0.0F) {
                this->mLegIk.coxaIk += 360.0F;
            }

            Core::Status success = Core::Status::CORE_OK;
            success |= this->mCoxa.SetAngle(static_cast<uint8_t>(this->mLegIk.coxaIk), travelTime);
            success |= this->mFemur.SetAngle(static_cast<uint8_t>(this->mLegIk.femurIk), travelTime);
            success |= this->mTibia.SetAngle(static_cast<uint8_t>(this->mLegIk.tibiaIk), travelTime);

            if (success != Core::Status::CORE_OK) {
                LOG_BOT_ERROR("Leg",
                              "leg %s(%d) Set IK (coxaIk:%d, femurIk:%d, tibiaIk:%d)",
                              ElegToString(this->mLegId).c_str(),
                              this->mLegId,
                              static_cast<uint8_t>(this->mLegIk.coxaIk),
                              static_cast<uint8_t>(this->mLegIk.femurIk),
                              static_cast<uint8_t>(this->mLegIk.tibiaIk));
            }
            return success;
        }

        Core::Status Leg::SetLegBodyIk(const Position3d &position,
                                       const Position3d &bodyIk,
                                       const uint16_t    travelTime) {
            this->mLegIk.newFootPos.x = this->mFootPosition.x + position.x + bodyIk.x;
            this->mLegIk.newFootPos.y = this->mFootPosition.y + position.y + bodyIk.y;
            this->mLegIk.newFootPos.z = this->mFootPosition.z + position.z + bodyIk.z;
            this->mLegIk.coxaFootDist = sqrt(this->mLegIk.newFootPos.x * this->mLegIk.newFootPos.x +
                                             this->mLegIk.newFootPos.y * this->mLegIk.newFootPos.y);
            this->mLegIk.iksw =
                    sqrt(((this->mLegIk.coxaFootDist - COXA_LENGTH) * (this->mLegIk.coxaFootDist - COXA_LENGTH)) +
                         (this->mLegIk.newFootPos.z * this->mLegIk.newFootPos.z));
            this->mLegIk.ika1 = atan((this->mLegIk.coxaFootDist - COXA_LENGTH) / this->mLegIk.newFootPos.z);
            this->mLegIk.ika2 = acos(((TIBIA_LENGTH * TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH) -
                                      (this->mLegIk.iksw * this->mLegIk.iksw)) /
                                     (-2.0F * this->mLegIk.iksw * FEMUR_LENGTH));
            this->mLegIk.tangle = acos(((this->mLegIk.iksw * this->mLegIk.iksw) - (TIBIA_LENGTH * TIBIA_LENGTH) -
                                        (FEMUR_LENGTH * FEMUR_LENGTH)) /
                                       (-2.0F * FEMUR_LENGTH * TIBIA_LENGTH));
            this->mLegIk.tibiaIk = 90.0F + (90.0F - this->mLegIk.tangle * 180.0F / M_PI);
            this->mLegIk.femurIk = 90.0F + (90.0F - (this->mLegIk.ika1 + this->mLegIk.ika2) * 180.0F / M_PI);
            this->mLegIk.coxaIk = atan2(this->mLegIk.newFootPos.y, this->mLegIk.newFootPos.x) * 180.0F / M_PI;

            switch (this->mLegId) {
                case Legs::ELeg::FRONT_LEFT: this->mLegIk.coxaIk -= 30.0F; break;

                case Legs::ELeg::MIDDLE_LEFT: this->mLegIk.coxaIk -= 90.0F; break;

                case Legs::ELeg::REAR_LEFT: this->mLegIk.coxaIk += 210.0F; break;

                case Legs::ELeg::FRONT_RIGHT: this->mLegIk.coxaIk += 30.0F; break;

                case Legs::ELeg::MIDDLE_RIGHT: this->mLegIk.coxaIk += 90.0F; break;

                case Legs::ELeg::REAR_RIGHT: this->mLegIk.coxaIk += 150.0F; break;
            }

            // Normalize angle: first wrap to -180 to +180 range
            this->mLegIk.coxaIk = fmod(this->mLegIk.coxaIk + 180.0F, 360.0F) - 180.0F;
            if (this->mLegIk.coxaIk < 0.0F) {
                this->mLegIk.coxaIk += 360.0F;
            }

            // Clamp to valid servo range (60-120 degrees)
            if (this->mLegIk.coxaIk < 60.0F) {
                this->mLegIk.coxaIk = 60.0F;
            } else if (this->mLegIk.coxaIk > 120.0F && this->mLegIk.coxaIk < 240.0F) {
                this->mLegIk.coxaIk = 120.0F;
            } else if (this->mLegIk.coxaIk >= 240.0F) {
                // Angle is closer to 0-60 when wrapped
                this->mLegIk.coxaIk = 60.0F;
            }

            Core::Status success = Core::Status::CORE_OK;
            success |= this->mCoxa.SetAngle(static_cast<uint8_t>(this->mLegIk.coxaIk), travelTime);
            success |= this->mFemur.SetAngle(static_cast<uint8_t>(this->mLegIk.femurIk), travelTime);
            success |= this->mTibia.SetAngle(static_cast<uint8_t>(this->mLegIk.tibiaIk), travelTime);

            if (success != Core::Status::CORE_OK) {
                LOG_BOT_ERROR("Leg", "leg Id:%d Set Body IK error", this->mLegId);
            }
            return success;
        }
    } // namespace Leg
} // namespace Bot
