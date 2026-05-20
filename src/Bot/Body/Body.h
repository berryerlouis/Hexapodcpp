#pragma once

#include "../../Move/Walk/Walk.h"
#include "BodyInterface.h"

namespace Bot
{
    namespace Body
    {
        class Body : public BodyInterface {
        public:
            struct SBodyIk {
                float      totalY;
                float      totalX;
                float      distBodyCenterFeet;
                float      angleBodyCenterX;
                float      rollZ;
                float      pitchZ;
                Position3d bodyIk;
            };

            explicit Body(Legs::LegsInterface &legs);

            ~Body() = default;

            virtual Core::Status            Initialize(void) final override;

            virtual void                    Update(const uint64_t currentTime) final override;

            virtual void                    UpdateWalkStatus(const Move::Walk::EWalkStatus status,
                                                             const uint16_t                duration) final override;

            virtual Move::Walk::EWalkStatus GetWalkStatus(void) final override;

            virtual uint32_t                SetBodyPositionRotation(const Position3d &position,
                                                                    const Rotation3d &rotation,
                                                                    const uint16_t    travelTime) final override;

            virtual uint32_t                SetLegPositionRotation(const uint8_t    &legId,
                                                                   const Position3d &position,
                                                                   const uint16_t    travelTime) final override;

            virtual bool                    SetDirection(const float directionAngle) final override;

            virtual bool                    SetRotation(const float rotationAngle,
                                                        const bool  clockWize) final override;

            virtual bool                    SetAmplitude(const float amplitude) final override;

            virtual bool                    SetElevation(const float elevation) final override;

            virtual bool                    SetDuration(const uint16_t duration) final override;

            virtual bool                    SetGait(const Move::Gait::GaitType gait) final override;

            virtual float                   GetDirection(void) final override;

            virtual float                   GetRotation(void) final override;

            virtual bool                    GetRotationClockWize(void) final override;

            virtual float                   GetAmplitude(void) final override;

            virtual float                   GetElevation(void) final override;

            virtual uint16_t                GetDuration(void) final override;

            virtual Move::Gait::GaitType    GetGait(void) final override;

        private:
            SBodyIk              mBodyIk;
            Legs::LegsInterface &mLegs;
            Move::Walk::Walk     mWalk;
            Position3d           mPosition;
            Rotation3d           mRotation;

            void                 SetComputeIk(const Leg::LegInterface &leg,
                                              const Position3d        &position,
                                              const Rotation3d        &rotation);
        };
    } // namespace Body
} // namespace Bot
