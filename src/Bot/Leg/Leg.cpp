#include "Leg.h"

#include <cstdio>
#include <iostream>

#include "../../Misc/Maths/Utils.h"

namespace Bot
{
    namespace Leg
    {
        Leg::Leg(const ELeg legId, ServoInterface &coxa, ServoInterface &femur, ServoInterface &tibia) :
            mBodyCenterOffsetX{0}
            , mBodyCenterOffsetY{0}
            , mFootPosition{0, 0, 0}
            , mLegIk{{0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0}
            , mCurrentPos{0, 0, 0}
            , mTargetPos{0, 0, 0}
            , mLegId(legId)
            , mCoxa(coxa)
            , mFemur(femur)
            , mTibia(tibia) {
            switch (mLegId) {
                case ELeg::FRONT_LEFT:
                    this->mBodyCenterOffsetX = -BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = -cos(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = sin(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case ELeg::MIDDLE_LEFT:
                    this->mBodyCenterOffsetX = -BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = 0U;
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case ELeg::REAR_LEFT:
                    this->mBodyCenterOffsetX = -BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = -BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = -cos(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = -sin(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case ELeg::FRONT_RIGHT:
                    this->mBodyCenterOffsetX = BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = cos(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = sin(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case ELeg::MIDDLE_RIGHT:
                    this->mBodyCenterOffsetX = BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = 0U;
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case ELeg::REAR_RIGHT:
                    this->mBodyCenterOffsetX = BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = -BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = cos(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = -sin(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;
            }
            LOG_BOT_DEBUG("Leg", "Leg %s(%d) Initialized",
                          ElegToString(this->mLegId).c_str(),
                          this->mLegId);
        }

        ELeg Leg::GetId(void) const {
            return this->mLegId;
        }

        void Leg::ComputeDirection(Position3d &position, const float angle, const bool clockWize) {
            Position3d rotatePosition = position;
            const float clockwize = clockWize ? 1.0F : -1.0F;
            const float newAngle = clockWize ? angle : -angle - M_PI;
            rotatePosition.x = (position.x * cos(newAngle)) + (position.y * clockwize * sin(newAngle));
            rotatePosition.y = (position.y * cos(newAngle)) - (position.x * clockwize * sin(newAngle));
            position = rotatePosition;
        }

        void Leg::ComputeAmplitude(Position3d &position, const uint8_t amplitude) {
            position.x *= amplitude;
            position.y *= amplitude;
        }

        void Leg::ComputeElevation(Position3d &position, const uint8_t elevation) {
            position.z *= elevation * -1.0F;
        }

        Core::Status Leg::SetTarget(const Position3d &target) {
            this->mTargetPos = target;
            return Core::CORE_OK;
        }

        Core::Status Leg::UpdatePosition(const float deltaTime) {
            this->mCurrentPos = Misc::Utils::LerpF3d(this->mCurrentPos, this->mTargetPos, deltaTime);
            return this->SetLegIk(this->mCurrentPos);
        }

        Core::Status Leg::SetLegIk(const Position3d &position, const uint16_t travelTime) {
            this->mLegIk.newFootPos.x = position.x + (COXA_LENGTH + FEMUR_LENGTH);
            this->mLegIk.newFootPos.y = position.y + 0;
            this->mLegIk.newFootPos.z = position.z + TIBIA_LENGTH;

            //compute angles from distances
            this->mLegIk.coxaFootDist = sqrt(
                    this->mLegIk.newFootPos.x * this->mLegIk.newFootPos.x +
                    this->mLegIk.newFootPos.y * this->mLegIk.newFootPos.y);
            this->mLegIk.iksw = sqrt(
                    ((this->mLegIk.coxaFootDist - COXA_LENGTH) * (this->mLegIk.coxaFootDist - COXA_LENGTH)) +
                    (this->mLegIk.newFootPos.z * this->mLegIk.newFootPos.z));
            this->mLegIk.ika1 = atan((this->mLegIk.coxaFootDist - COXA_LENGTH) / this->mLegIk.newFootPos.z);
            this->mLegIk.ika2 = acos(((TIBIA_LENGTH * TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH) -
                                      (this->mLegIk.iksw * this->mLegIk.iksw)) / (
                                         -2 * this->mLegIk.iksw * FEMUR_LENGTH));
            this->mLegIk.tangle = acos(((this->mLegIk.iksw * this->mLegIk.iksw) - (TIBIA_LENGTH * TIBIA_LENGTH) - (
                                            FEMUR_LENGTH * FEMUR_LENGTH)) / (-2 * FEMUR_LENGTH * TIBIA_LENGTH));
            this->mLegIk.tibiaIk = 90 + (90 - this->mLegIk.tangle * 180 / M_PI);
            this->mLegIk.femurIk = 90 + (90 - (this->mLegIk.ika1 + this->mLegIk.ika2) * 180 / M_PI);
            this->mLegIk.coxaIk = 90 + atan2(this->mLegIk.newFootPos.y, this->mLegIk.newFootPos.x) * 180 / M_PI;

            uint8_t success = 0U;
            this->mLegIk.coxaIk = static_cast<uint16_t>(this->mLegIk.coxaIk) % 360;
            success = this->mCoxa.SetAngle(static_cast<uint8_t>(this->mLegIk.coxaIk), travelTime) << 0U;
            success |= this->mFemur.SetAngle(static_cast<uint8_t>(this->mLegIk.femurIk), travelTime) << 3U;
            success |= this->mTibia.SetAngle(static_cast<uint8_t>(this->mLegIk.tibiaIk), travelTime) << 6U;

            if (success != 0U)
                LOG_BOT_ERROR("Gaits", "leg %s(%d) Set IK error (coxaIk:%d, femurIk:%d, tibiaIk:%d)",
                          ElegToString(this->mLegId).c_str(),
                          this->mLegId,
                          static_cast<uint8_t>(this->mLegIk.coxaIk),
                          static_cast<uint8_t>(this->mLegIk.femurIk),
                          static_cast<uint8_t>(this->mLegIk.tibiaIk));
            return success == 0U ? Core::Status::CORE_OK : Core::Status::CORE_ERROR;
        }

        Core::Status Leg::SetLegBodyIk(const Position3d &position, const Position3d &bodyIk,
                                       const uint16_t travelTime) {
            this->mLegIk.newFootPos.x = this->mFootPosition.x + position.x + bodyIk.x;
            this->mLegIk.newFootPos.y = this->mFootPosition.y + position.y + bodyIk.y;
            this->mLegIk.newFootPos.z = this->mFootPosition.z + position.z + bodyIk.z;
            this->mLegIk.coxaFootDist = sqrt(
                    this->mLegIk.newFootPos.x * this->mLegIk.newFootPos.x +
                    this->mLegIk.newFootPos.y * this->mLegIk.newFootPos.y);
            this->mLegIk.iksw = sqrt(
                    ((this->mLegIk.coxaFootDist - COXA_LENGTH) * (this->mLegIk.coxaFootDist - COXA_LENGTH)) +
                    (this->mLegIk.newFootPos.z * this->mLegIk.newFootPos.z));
            this->mLegIk.ika1 = atan((this->mLegIk.coxaFootDist - COXA_LENGTH) / this->mLegIk.newFootPos.z);
            this->mLegIk.ika2 = acos(((TIBIA_LENGTH * TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH) -
                                      (this->mLegIk.iksw * this->mLegIk.iksw)) / (
                                         -2 * this->mLegIk.iksw * FEMUR_LENGTH));
            this->mLegIk.tangle = acos(((this->mLegIk.iksw * this->mLegIk.iksw) - (TIBIA_LENGTH * TIBIA_LENGTH) - (
                                            FEMUR_LENGTH * FEMUR_LENGTH)) / (-2 * FEMUR_LENGTH * TIBIA_LENGTH));
            this->mLegIk.tibiaIk = 90 + (90 - this->mLegIk.tangle * 180 / M_PI);
            this->mLegIk.femurIk = 90 + (90 - (this->mLegIk.ika1 + this->mLegIk.ika2) * 180 / M_PI);
            this->mLegIk.coxaIk = atan2(this->mLegIk.newFootPos.y, this->mLegIk.newFootPos.x) * 180 / M_PI;

            switch (this->mLegId) {
                case ELeg::FRONT_LEFT:
                    this->mLegIk.coxaIk -= 30;
                    break;

                case ELeg::MIDDLE_LEFT:
                    this->mLegIk.coxaIk += 90;
                    this->mLegIk.coxaIk = (((this->mLegIk.coxaIk) * -1) + 180);
                    break;

                case ELeg::REAR_LEFT:
                    this->mLegIk.coxaIk += 210;
                    break;

                case ELeg::FRONT_RIGHT:
                    this->mLegIk.coxaIk += 30;
                    break;

                case ELeg::MIDDLE_RIGHT:
                    this->mLegIk.coxaIk += 90;
                    break;

                case ELeg::REAR_RIGHT:
                    this->mLegIk.coxaIk += 150;
                    break;
            }
            uint8_t success = 0U;
            this->mLegIk.coxaIk = static_cast<uint16_t>(this->mLegIk.coxaIk) % 360;
            success = this->mCoxa.SetAngle(static_cast<uint8_t>(this->mLegIk.coxaIk), travelTime) << 0U;
            success |= this->mFemur.SetAngle(static_cast<uint8_t>(this->mLegIk.femurIk), travelTime) << 3U;
            success |= this->mTibia.SetAngle(static_cast<uint8_t>(this->mLegIk.tibiaIk), travelTime) << 6U;

            if (success != 0U)
                LOG_BOT_ERROR("Gaits", "leg Id:%d Set Body IK error", this->mLegId);
            return success == 0U ? Core::Status::CORE_OK : Core::Status::CORE_ERROR;
        }
    }
}
