#include "Body.h"

namespace Bot
{
    namespace Body
    {
        Body::Body(Legs::Legs &legs)
            : mBodyIk{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, {0.0, 0.0, 0.0}}
              , mLegs(legs)
              , mWalk(mLegs) {
        }

        Core::CoreStatus Body::Initialize(void) {
            constexpr Position3d position = {0.0, 0.0, 0.0};
            constexpr Rotation3d rotation = {0.0, 0.0, 0.0};

            this->SetBodyPositionRotation(position, rotation, 1000);

            return (Core::CoreStatus::CORE_OK);
        }

        void Body::Update(const uint64_t currentTime) {
            this->mWalk.Update(currentTime, this->mBodyIk.bodyIk);
        }

        void Body::UpdateWalkStatus(const EWalkStatus status) {
            this->mWalk.UpdateStatus(status);
        }

        uint32_t Body::SetBodyPositionRotation(const Position3d &position,
                                               const Rotation3d &rotation,
                                               const uint16_t travelTime) {
            return this->SetBodyIk(position, rotation, travelTime);
        }

        uint32_t Body::SetLegPositionRotation(const uint8_t &legId,
                                              const Position3d &position,
                                              const uint16_t travelTime) {
            if (legId < NB_LEGS) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
                const uint8_t successServos = leg.SetLegIk(position, this->mBodyIk.bodyIk, travelTime);
                return successServos;
            }
            return 255;
        }

        uint32_t Body::SetBodyIk(const Position3d &position, const Rotation3d &rotation,
                                 const uint16_t travelTime) {
            uint32_t success = 0U;
            for (size_t legId = 0U; legId < NB_LEGS; legId++) {
                Leg::Leg leg = this->mLegs.GetLeg(legId);
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
                const uint8_t successServos = leg.SetLegIk(position, this->mBodyIk.bodyIk, travelTime);
                success |= successServos << (legId * 3U);
            }
            return success;
        }
    }
}
