#pragma once

#include "../SensorProximityInterface.h"

namespace Component
{
    namespace Proximity
    {
        namespace Laser
        {
            enum LaserModel {
                VL53L0X = 0x00U,
                VL53L5X,
            };

            class Vl53lxxInterface : public SensorProximityInterface {
            public:
                Vl53lxxInterface(void) = default;

                ~Vl53lxxInterface() = default;

                virtual auto GetDistanceMatrix(void) const -> const uint16_t (*)[8U] {
                    return nullptr; // Default: no matrix support
                }

                virtual uint16_t GetDistanceBufferSize(void) const {
                    return 2U; // Default: single distance (2 bytes)
                }

                virtual LaserModel GetLaserModelName(void) const = 0;
            };
        } // namespace Laser
    } // namespace Proximity
} // namespace Component
