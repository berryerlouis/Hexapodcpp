#include "Body.h"

namespace Bot
{
    namespace Body
    {
        Body::Body(Legs::LegsInterface &legs) :
            mBodyIk{0.0,
                    0.0,
                    0.0,
                    0.0,
                    0.0,
                    0.0,
                    {0.0,
                     0.0,
                     0.0}},
            mLegs(legs),
            mWalk(mLegs),
            mPosition{0.0,
                      0.0,
                      0.0},
            mRotation{0.0,
                      0.0,
                      0.0} {
        }

        Core::Status Body::Initialize(void) {
            this->SetBodyPositionRotation(this->mPosition, this->mRotation, 1000);

            return (Core::Status::CORE_OK);
        }

        void Body::Update(const uint64_t currentTime) {
            this->mWalk.Update(currentTime);
        }

        void Body::UpdateWalkStatus(const Move::Walk::EWalkStatus status,
                                    const uint16_t                duration) {
            this->SetDuration(duration);
            this->mWalk.UpdateStatus(status);
        }

        Move::Walk::EWalkStatus Body::GetWalkStatus() {
            return this->mWalk.GetStatus();
        }

        bool Body::SetDirection(const float directionAngle) {
            return this->mWalk.SetDirection(directionAngle);
        }

        bool Body::SetRotation(const float rotationAngle,
                               const bool  clockWize) {
            return this->mWalk.SetRotation(rotationAngle, clockWize);
        }

        bool Body::SetAmplitude(const float amplitude) {
            return this->mWalk.SetAmplitude(amplitude);
        }

        bool Body::SetElevation(const float elevation) {
            return this->mWalk.SetElevation(elevation);
        }

        bool Body::SetDuration(const uint16_t duration) {
            return this->mWalk.SetCycleDuration(duration);
        }

        bool Body::SetGait(const Move::Gait::GaitType gait) {
            return this->mWalk.SetGait(gait);
        }

        float Body::GetDirection(void) {
            return this->mWalk.GetDirection();
        }

        float Body::GetRotation() {
            return this->mWalk.GetRotation();
        }

        bool Body::GetRotationClockWize() {
            return this->mWalk.GetRotationClockWize();
        }

        float Body::GetAmplitude(void) {
            return this->mWalk.GetAmplitude();
        }

        float Body::GetElevation(void) {
            return this->mWalk.GetElevation();
        }

        uint16_t Body::GetDuration() {
            return this->mWalk.GetCycleDuration();
        }

        Move::Gait::GaitType Body::GetGait() {
            return this->mWalk.GetGait();
        }

        uint32_t Body::SetBodyPositionRotation(const Position3d &position,
                                               const Rotation3d &rotation,
                                               const uint16_t    travelTime) {
            this->mPosition = position;
            this->mRotation = rotation;
            uint32_t success = 0U;
            for (size_t legId = 0U; legId < Legs::NB_LEGS; legId++) {
                Leg::LegInterface *leg = this->mLegs.GetLeg(static_cast<Legs::ELeg>(legId));
                this->SetComputeIk(*leg, position, rotation);
                const uint8_t successServos = leg->SetLegBodyIk(position, this->mBodyIk.bodyIk, travelTime);
                success |= successServos << (legId * 3U);
            }
            return success;
        }

        uint32_t Body::SetLegPositionRotation(const uint8_t    &legId,
                                              const Position3d &position,
                                              const uint16_t    travelTime) {
            if (legId < Legs::NB_LEGS) {
                Leg::LegInterface *leg = this->mLegs.GetLeg(static_cast<Legs::ELeg>(legId));
                return leg->SetLegIk(position, travelTime);
            }
            return 255UL;
        }

        void Body::SetComputeIk(const Leg::LegInterface &leg,
                                const Position3d        &position,
                                const Rotation3d        &rotation) {
            this->mBodyIk.totalX = leg.GetFootPosition().x + leg.GetBodyCenterOffsetX() + position.x;
            this->mBodyIk.totalY = leg.GetFootPosition().y + leg.GetBodyCenterOffsetY() + position.y;
            this->mBodyIk.distBodyCenterFeet =
                    sqrt(this->mBodyIk.totalX * this->mBodyIk.totalX + this->mBodyIk.totalY * this->mBodyIk.totalY);
            this->mBodyIk.angleBodyCenterX = atan2(this->mBodyIk.totalY, this->mBodyIk.totalX);
            this->mBodyIk.rollZ = tan(rotation.angleZ * M_PI / 180.0) * this->mBodyIk.totalX;
            this->mBodyIk.pitchZ = tan(rotation.angleX * M_PI / 180.0) * this->mBodyIk.totalY;
            this->mBodyIk.bodyIk.x = (cos(this->mBodyIk.angleBodyCenterX + (rotation.angleY * M_PI / 180.0)) *
                                      this->mBodyIk.distBodyCenterFeet) -
                                     this->mBodyIk.totalX;
            this->mBodyIk.bodyIk.y = (sin(this->mBodyIk.angleBodyCenterX + (rotation.angleY * M_PI / 180.0)) *
                                      this->mBodyIk.distBodyCenterFeet) -
                                     this->mBodyIk.totalY;
            this->mBodyIk.bodyIk.z = this->mBodyIk.rollZ + this->mBodyIk.pitchZ;
        }
    } // namespace Body
} // namespace Bot
