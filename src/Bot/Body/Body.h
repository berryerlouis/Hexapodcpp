#pragma once

#include "BodyInterface.h"
#include "../Walk/Walk.h"

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

            Body(Legs::Legs &legs);

            ~Body() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void UpdateWalkStatus(const EWalkStatus status) final override;

            virtual uint32_t SetBodyPositionRotation(const Position3d &position,
                                                     const Rotation3d &rotation,
                                                     const uint16_t travelTime) final override;

            virtual uint32_t SetLegPositionRotation(const uint8_t &legId,
                                                    const Position3d &position,
                                                    const uint16_t travelTime) final override;

        private:
            SBodyIk mBodyIk;
            Legs::Legs &mLegs;
            Walk::Walk mWalk;

            uint32_t SetBodyIk(const Position3d &position, const Rotation3d &rotation,
                               const uint16_t travelTime);
        };
    }
}
