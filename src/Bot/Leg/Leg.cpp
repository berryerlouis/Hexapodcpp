#include "Leg.h"

#include <cmath>
#include <cstdio>

namespace
{
    constexpr float RAD_TO_DEG = 57.2957795F;
    constexpr float DEG_TO_RAD = 0.0174532925F;
    constexpr float MM_TO_CM = 0.1F;

    // Keeps the law-of-cosines solution away from the fully folded / fully stretched singularities.
    constexpr float REACH_MARGIN = 0.01F;

    inline float Clamp(const float value, const float min, const float max) {
        if (value < min) {
            return min;
        }
        if (value > max) {
            return max;
        }
        return value;
    }

    struct SLegMount {
        float offsetX;
        float offsetY;
        float angleDeg;
    };

    SLegMount GetLegMount(const Bot::Legs::ELeg legId) {
        using Bot::Leg::Leg;
        switch (legId) {
            case Bot::Legs::ELeg::FRONT_LEFT:
                return {-Leg::BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH,
                        Leg::BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH,
                        120.0F};
            case Bot::Legs::ELeg::MIDDLE_LEFT:
                return {-Leg::BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH,
                        Leg::BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH,
                        180.0F};
            case Bot::Legs::ELeg::REAR_LEFT:
                return {-Leg::BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH,
                        -Leg::BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH,
                        -120.0F};
            case Bot::Legs::ELeg::FRONT_RIGHT:
                return {Leg::BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH,
                        Leg::BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH,
                        60.0F};
            case Bot::Legs::ELeg::MIDDLE_RIGHT:
                return {Leg::BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH,
                        Leg::BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH,
                        0.0F};
            case Bot::Legs::ELeg::REAR_RIGHT:
            default:
                return {Leg::BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH,
                        -Leg::BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH,
                        -60.0F};
        }
    }
} // namespace

namespace Bot
{
    namespace Leg
    {
        Leg::Leg(const Legs::ELeg legId,
                 ServoInterface  &coxa,
                 ServoInterface  &femur,
                 ServoInterface  &tibia)
            : mBodyCenterOffsetX{0.0F}
            , mBodyCenterOffsetY{0.0F}
            , mMountAngle{0.0F}
            , mFootPosition{0.0F, 0.0F, 0.0F}
            , mCurrentPos{0.0F, 0.0F, 0.0F}
            , mLegId(legId)
            , mCoxa(coxa)
            , mFemur(femur)
            , mTibia(tibia) {
            const SLegMount mount = GetLegMount(legId);
            this->mBodyCenterOffsetX = mount.offsetX * MM_TO_CM;
            this->mBodyCenterOffsetY = mount.offsetY * MM_TO_CM;
            this->mMountAngle = mount.angleDeg * DEG_TO_RAD;
            this->mFootPosition.x = std::cos(this->mMountAngle) * (COXA_LENGTH + FEMUR_LENGTH);
            this->mFootPosition.y = std::sin(this->mMountAngle) * (COXA_LENGTH + FEMUR_LENGTH);
            this->mFootPosition.z = TIBIA_LENGTH;

            LOG_BOT_DEBUG("Leg",
                          "Leg %s(%d) Initialized",
                          ElegToString(this->mLegId).c_str(),
                          this->mLegId);
        }

        bool Leg::SolveIk(const Position3d &foot, SLegAngles &angles) {
            if (!std::isfinite(foot.x) || !std::isfinite(foot.y) || !std::isfinite(foot.z)) {
                return false;
            }

            const float radial = std::hypot(foot.x, foot.y) - COXA_LENGTH;
            const float reach = Clamp(std::hypot(radial, foot.z),
                                      std::fabs(FEMUR_LENGTH - TIBIA_LENGTH) + REACH_MARGIN,
                                      FEMUR_LENGTH + TIBIA_LENGTH - REACH_MARGIN);

            // Angle of the femur-to-foot line from the vertical, direction kept even when clamped.
            const float ika1 = std::atan2(radial, foot.z);
            const float ika2 = std::acos(Clamp(((FEMUR_LENGTH * FEMUR_LENGTH) + (reach * reach) -
                                                (TIBIA_LENGTH * TIBIA_LENGTH)) /
                                                       (2.0F * reach * FEMUR_LENGTH),
                                               -1.0F,
                                               1.0F));
            const float kneeAngle = std::acos(Clamp(((FEMUR_LENGTH * FEMUR_LENGTH) +
                                                     (TIBIA_LENGTH * TIBIA_LENGTH) -
                                                     (reach * reach)) /
                                                            (2.0F * FEMUR_LENGTH * TIBIA_LENGTH),
                                                    -1.0F,
                                                    1.0F));

            angles.coxa = 90.0F + (std::atan2(foot.y, foot.x) * RAD_TO_DEG);
            angles.femur = 180.0F - ((ika1 + ika2) * RAD_TO_DEG);
            angles.tibia = 180.0F - (kneeAngle * RAD_TO_DEG);
            return true;
        }

        Legs::ELeg Leg::GetId() const {
            return this->mLegId;
        }

        void Leg::ComputeDirection(Position3d &position, const float angleDirection) const {
            // Input is in the travel frame: x lateral, -y along the travel direction.
            // The travel direction is rotated by angleDirection (counter-clockwise, 0 = forward)
            // in the body frame, then expressed in the leg frame.
            const float alpha = angleDirection - this->mMountAngle;
            const float lateral = position.x;
            const float along = -position.y;

            position.x = (lateral * std::cos(alpha)) - (along * std::sin(alpha));
            position.y = (lateral * std::sin(alpha)) + (along * std::cos(alpha));
        }

        void Leg::ComputeRotation(Position3d &position,
                                  const float angleRotation,
                                  const bool  clockwize) const {
            // The leg frame y axis is tangent to the body, so the same sign turns every leg
            // the same way. The rotation magnitude is driven by the amplitude only.
            (void) angleRotation;
            if (!clockwize) {
                position.y *= -1.0F;
            }
        }

        void Leg::ComputeAmplitude(Position3d &position, const float amplitude) const {
            position.x *= amplitude;
            position.y *= amplitude;
        }

        void Leg::ComputeElevation(Position3d &position, const float elevation) const {
            position.z *= elevation * -1.0F;
        }

        void Leg::ComputeTarget(Position3d &position,
                                const float amplitude,
                                const float elevation,
                                const float direction,
                                const bool  isRotated,
                                const float rotation,
                                const bool  clockWize) const {
            if (isRotated) {
                this->ComputeRotation(position, rotation, clockWize);
            } else {
                this->ComputeDirection(position, direction);
            }
            this->ComputeAmplitude(position, amplitude);
            this->ComputeElevation(position, elevation);
        }

        Position3d Leg::GetFootPosition() const {
            return this->mFootPosition;
        }

        float Leg::GetBodyCenterOffsetX() const {
            return this->mBodyCenterOffsetX;
        }

        float Leg::GetBodyCenterOffsetY() const {
            return this->mBodyCenterOffsetY;
        }

        void Leg::SetTarget(const Position3d &target) {
            this->mCurrentPos = target;
        }

        Position3d &Leg::GetCurrentPosition() {
            return this->mCurrentPos;
        }

        Core::Status Leg::Update() {
            return this->SetLegIk(this->mCurrentPos, 0U);
        }

        Core::Status Leg::ApplyAngles(const SLegAngles &angles, const uint16_t travelTime) {
            const float coxaCmd = Clamp(angles.coxa, COXA_ANGLE_MIN, COXA_ANGLE_MAX);
            const float femurCmd = Clamp(angles.femur, 0.0F, 180.0F);
            const float tibiaCmd = Clamp(angles.tibia, 0.0F, 180.0F);

            const uint8_t coxaAngle = static_cast<uint8_t>(std::lround(coxaCmd));
            const uint8_t femurAngle = static_cast<uint8_t>(std::lround(femurCmd));
            const uint8_t tibiaAngle = static_cast<uint8_t>(std::lround(tibiaCmd));

            Core::Status success = Core::Status::CORE_OK;
            success |= this->mCoxa.SetAngle(coxaAngle, travelTime);
            success |= this->mFemur.SetAngle(femurAngle, travelTime);
            success |= this->mTibia.SetAngle(tibiaAngle, travelTime);

            if (success != Core::Status::CORE_OK) {
                LOG_BOT_ERROR("Leg",
                              "leg %s(%d) Set IK (coxaIk:%d, femurIk:%d, tibiaIk:%d)",
                              ElegToString(this->mLegId).c_str(),
                              this->mLegId,
                              coxaAngle,
                              femurAngle,
                              tibiaAngle);
            }
            return success;
        }

        Core::Status Leg::SetLegIk(const Position3d &position, const uint16_t travelTime) {
            const Position3d foot = {position.x + (COXA_LENGTH + FEMUR_LENGTH),
                                     position.y,
                                     position.z + TIBIA_LENGTH};

            SLegAngles angles{};
            if (!Leg::SolveIk(foot, angles)) {
                LOG_BOT_ERROR("Leg",
                              "leg %s(%d) IK produced invalid angle (target=%.2f,%.2f,%.2f)",
                              ElegToString(this->mLegId).c_str(),
                              this->mLegId,
                              position.x,
                              position.y,
                              position.z);
                return Core::Status::CORE_ERROR;
            }
            return this->ApplyAngles(angles, travelTime);
        }

        Core::Status Leg::SetLegBodyIk(const Position3d &position,
                                       const Position3d &bodyIk,
                                       const uint16_t    travelTime) {
            // Foot relative to the coxa axis in the body frame, rotated into the leg frame.
            const float bodyX = this->mFootPosition.x + position.x + bodyIk.x;
            const float bodyY = this->mFootPosition.y + position.y + bodyIk.y;
            const float cosMount = std::cos(this->mMountAngle);
            const float sinMount = std::sin(this->mMountAngle);
            const Position3d foot = {(bodyX * cosMount) + (bodyY * sinMount),
                                     (bodyY * cosMount) - (bodyX * sinMount),
                                     this->mFootPosition.z + position.z + bodyIk.z};

            SLegAngles angles{};
            if (!Leg::SolveIk(foot, angles)) {
                LOG_BOT_ERROR("Leg", "leg Id:%d Body IK produced invalid angle", this->mLegId);
                return Core::Status::CORE_ERROR;
            }
            return this->ApplyAngles(angles, travelTime);
        }
    } // namespace Leg
} // namespace Bot
