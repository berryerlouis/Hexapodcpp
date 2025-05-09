#include "Body.h"

namespace Bot
{
    namespace Body
    {
        Body::Body(Legs::Legs &legs) :
            mBodyIk{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, {0.0, 0.0, 0.0}}
            , mLegs(legs)
            , mWalk(mLegs)
            , mPosition{0.0, 0.0, 0.0}
            , mRotation{0.0, 0.0, 0.0} {
        }

        Core::Status Body::Initialize(void) {
            this->SetBodyPositionRotation(this->mPosition, this->mRotation, 1000);

            return (Core::Status::CORE_OK);
        }

        void Body::Update(const uint64_t currentTime) {
            this->mWalk.Update(currentTime);
        }

        void Body::UpdateWalkStatus(const EWalkStatus status) {
            this->mWalk.UpdateStatus(status);
        }

        bool Body::SetDirection(const uint16_t directionAngle) {
            return this->mWalk.SetDirection(directionAngle);
        }

        bool Body::SetAmplitudeElevation(const uint16_t amplitude, const uint16_t elevation) {
            return this->mWalk.SetAmplitudeElevation(amplitude, elevation);
        }

        uint16_t Body::GetDirection(void) const {
            return this->mWalk.GetDirection();
        }

        uint8_t Body::GetAmplitude(void) const {
            return this->mWalk.GetAmplitude();
        }

        uint8_t Body::GetElevation(void) const {
            return this->mWalk.GetElevation();
        }

        uint32_t Body::SetBodyPositionRotation(const Position3d &position,
                                               const Rotation3d &rotation,
                                               const uint16_t travelTime) {
            this->mPosition = position;
            this->mRotation = rotation;
            uint32_t success = 0U;
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                this->SetComputeIk(leg, position, rotation);
                const uint8_t successServos = leg.SetLegBodyIk(position, this->mBodyIk.bodyIk, travelTime);
                success |= successServos << (legId * 3U);
            }
            return success;
        }

        uint32_t Body::SetLegPositionRotation(const uint8_t &legId,
                                              const Position3d &position,
                                              const uint16_t travelTime) {
            if (legId < NB_LEGS) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                return leg.SetLegIk(position, travelTime);
            }
            return 255;
        }

        void Body::SetComputeIk(const Leg::Leg &leg, const Position3d &position, const Rotation3d &rotation) {
            this->mBodyIk.totalX = leg.mFootPosition.x + leg.mBodyCenterOffsetX + position.x;
            this->mBodyIk.totalY = leg.mFootPosition.y + leg.mBodyCenterOffsetY + position.y;
            this->mBodyIk.distBodyCenterFeet = sqrt(
                    this->mBodyIk.totalX * this->mBodyIk.totalX +
                    this->mBodyIk.totalY * this->mBodyIk.totalY);
            this->mBodyIk.angleBodyCenterX = atan2(this->mBodyIk.totalY, this->mBodyIk.totalX);
            this->mBodyIk.rollZ = tan(rotation.angleZ * M_PI / 180.0) * this->mBodyIk.totalX;
            this->mBodyIk.pitchZ = tan(rotation.angleX * M_PI / 180.0) * this->mBodyIk.totalY;
            this->mBodyIk.bodyIk.x = (cos(this->mBodyIk.angleBodyCenterX + (rotation.angleY * M_PI / 180.0)) *
                                      this->mBodyIk.distBodyCenterFeet) - this->mBodyIk.totalX;
            this->mBodyIk.bodyIk.y = (sin(this->mBodyIk.angleBodyCenterX + (rotation.angleY * M_PI / 180.0)) *
                                      this->mBodyIk.distBodyCenterFeet) - this->mBodyIk.totalY;
            this->mBodyIk.bodyIk.z = this->mBodyIk.rollZ + this->mBodyIk.pitchZ;
        }
    }
}
