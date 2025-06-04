#pragma once

#include "BodyInterface.h"
#include "../Walk/Walk.h"
#include "../../Driver/Tick/TickInterface.h"

namespace Bot
{
    namespace Body
    {
        class Body : public BodyInterface {
        public:
            struct SBodyIk {
                float totalY;
                float totalX;
                float distBodyCenterFeet;
                float angleBodyCenterX;
                float rollZ;
                float pitchZ;
                Position3d bodyIk;
            };

            Body(Legs::Legs &legs, Driver::Tick::TickInterface &tick);

            ~Body() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void UpdateWalkStatus(const EWalkStatus status) final override;

            virtual uint32_t SetBodyPositionRotation(const Position3d &position,
                                                     const Rotation3d &rotation,
                                                     const uint16_t travelTime) final override;

            virtual uint32_t SetLegPositionRotation(const uint8_t &legId,
                                                    const Position3d &position,
                                                    const uint16_t travelTime) final override;

            virtual bool SetDirection(const uint16_t directionAngle) final override;

            virtual bool SetAmplitude(const uint8_t amplitude) final override;

            virtual bool SetElevation(const uint8_t elevation) final override;

            virtual uint16_t GetDirection(void) const final override;

            virtual uint8_t GetAmplitude(void) const final override;

            virtual uint8_t GetElevation(void) const final override;

        private:
            SBodyIk mBodyIk;
            Legs::Legs &mLegs;
            Walk::Walk mWalk;
            Position3d mPosition;
            Rotation3d mRotation;

            void SetComputeIk(const Leg::Leg &leg, const Position3d &position, const Rotation3d &rotation);
        };
    }
}
