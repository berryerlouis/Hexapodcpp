#include "Leg.h"

namespace Bot
{
    namespace Leg
    {
        Leg::Leg(ELeg legId, ServoInterface &coxa, ServoInterface &femur, ServoInterface &tibia)
            : mBodyCenterOffsetX{0}
              , mBodyCenterOffsetY{0}
              , mFootPosition{0, 0, 0}
              , mLegIk{{0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0}
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

                    this->mFootPosition.x = -(COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = 0U;
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;

                case ELeg::REAR_LEFT:
                    this->mBodyCenterOffsetX = -BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH;
                    this->mBodyCenterOffsetY = -BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH;

                    this->mFootPosition.x = -cos(60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.y = sin(-60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
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
                    this->mFootPosition.y = sin(-60 / 180.0 * M_PI) * (COXA_LENGTH + FEMUR_LENGTH);
                    this->mFootPosition.z = TIBIA_LENGTH;
                    break;
            }
        }

        uint8_t Leg::SetLegIk(const Position3d &position, const Position3d &bodyIk,
                              const uint16_t travelTime) {
            this->mLegIk.newFootPos.x = this->mFootPosition.x + position.x + bodyIk.x;
            this->mLegIk.newFootPos.y = this->mFootPosition.y + position.y + bodyIk.y;
            this->mLegIk.newFootPos.z = this->mFootPosition.z + position.z + bodyIk.z;
            this->mLegIk.coxaFootDist = sqrt(
                this->mLegIk.newFootPos.x * this->mLegIk.newFootPos.x + this->mLegIk.newFootPos.y * this->mLegIk.
                newFootPos.y);
            this->mLegIk.iksw = sqrt(
                ((this->mLegIk.coxaFootDist - COXA_LENGTH) * (this->mLegIk.coxaFootDist - COXA_LENGTH)) + (
                    this->mLegIk.newFootPos.z * this->mLegIk.newFootPos.z));
            this->mLegIk.ika1 = atan((this->mLegIk.coxaFootDist - COXA_LENGTH) / this->mLegIk.newFootPos.z);
            this->mLegIk.ika2 = acos(
                ((TIBIA_LENGTH * TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH) - (
                     this->mLegIk.iksw * this->mLegIk.iksw)) / (-2 * this->mLegIk.iksw * FEMUR_LENGTH));
            this->mLegIk.tangle = acos(
                ((this->mLegIk.iksw * this->mLegIk.iksw) - (TIBIA_LENGTH * TIBIA_LENGTH) - (
                     FEMUR_LENGTH * FEMUR_LENGTH)) / (-2 * FEMUR_LENGTH * TIBIA_LENGTH));
            this->mLegIk.tibiaIk = this->mLegIk.tangle * 180 / M_PI;
            this->mLegIk.femurIk = 90 + (90 - (this->mLegIk.ika1 + this->mLegIk.ika2) * 180 / M_PI);
            this->mLegIk.coxaIk = atan2(this->mLegIk.newFootPos.y, this->mLegIk.newFootPos.x) * 180 / M_PI;

            switch (this->mLegId) {
                case ELeg::FRONT_LEFT:
                    this->mLegIk.coxaIk += -120;
                    break;

                case ELeg::MIDDLE_LEFT:
                    this->mLegIk.coxaIk += 180;
                    break;

                case ELeg::REAR_LEFT:
                    this->mLegIk.coxaIk += 120;
                    break;

                case ELeg::FRONT_RIGHT:
                    this->mLegIk.coxaIk += -60;
                    break;

                case ELeg::MIDDLE_RIGHT:
                    this->mLegIk.coxaIk += 0;
                    break;

                case ELeg::REAR_RIGHT:
                    this->mLegIk.coxaIk += 60;
                    break;
            }
            this->mLegIk.coxaIk += 90;
            this->mLegIk.coxaIk = static_cast<uint16_t>(this->mLegIk.coxaIk) % 360;
            uint8_t success = 0U;
            success = this->mCoxa.SetAngle(static_cast<uint8_t>(this->mLegIk.coxaIk), travelTime) << 0U;
            success |= this->mFemur.SetAngle(static_cast<uint8_t>(this->mLegIk.femurIk), travelTime) << 3U;
            success |= this->mTibia.SetAngle(static_cast<uint8_t>(this->mLegIk.tibiaIk), travelTime) << 6U;

            return success;
        }
    }
}
