#pragma once

#include "../../Component/Servo/ServoInterface.h"
#include "../Constants.h"
#include "../../Misc/Geometry/Geometry.h"
#include <math.h>
#include <stddef.h>

namespace Bot
{
    namespace Leg
    {
        using namespace Component::Servo;
        using namespace Misc::Geometry;

        class Leg {
        public:
            static constexpr float BODY_SIDE_X_LENGTH = 80.0;
            static constexpr float BODY_SIDE_X_MIDDLE_LENGTH = 130.0;
            static constexpr float BODY_SIDE_Y_LENGTH = 150.0;

            static constexpr float BODY_LEG_MIDDLE_FROM_CENTER_X_LENGTH = BODY_SIDE_X_MIDDLE_LENGTH / 2.0;
            static constexpr float BODY_LEG_MIDDLE_FROM_CENTER_Y_LENGTH = 0.0;
            static constexpr float BODY_LEG_FRONT_REAR_FROM_CENTER_X_LENGTH = BODY_SIDE_X_LENGTH / 2.0;
            static constexpr float BODY_LEG_FRONT_REAR_FROM_CENTER_Y_LENGTH = BODY_SIDE_Y_LENGTH / 2.0;

            static constexpr float COXA_LENGTH = 2.54;
            static constexpr float FEMUR_LENGTH = 8.50;
            static constexpr float TIBIA_LENGTH = 12.70;

            struct SLegIk {
                Position3d newFootPos;
                float coxaFootDist;
                float iksw;
                float ika1;
                float ika2;
                float tangle;
                float coxaIk;
                float femurIk;
                float tibiaIk;
            };


            Leg(ELeg legId, ServoInterface &coxa, ServoInterface &femur, ServoInterface &tibia);

            ~Leg() = default;


            void SetLegIk(const Position3d &position, const Position3d &bodyIk, const uint16_t travelTime);

        public:
            float mBodyCenterOffsetX;
            float mBodyCenterOffsetY;
            Position3d mFootPosition;
            SLegIk mLegIk;

        private:
            ELeg mLegId;
            ServoInterface &mCoxa;
            ServoInterface &mFemur;
            ServoInterface &mTibia;
        };
    }
}
