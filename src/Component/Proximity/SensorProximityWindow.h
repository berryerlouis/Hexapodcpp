#pragma once

#include <stdint.h>

namespace Component
{
    namespace Proximity
    {
        class SensorProximityWindow {
        public:
            SensorProximityWindow()
                : mDetectionStatus(false) {
            }

            ~SensorProximityWindow() = default;

            bool Detect() {
                bool detection = false;
                if (this->mDetectionStatus == false) {
                    this->mDetectionStatus = true;
                    detection = true;
                }
                return (detection);
            }

            bool UnDetect() {
                bool detection = false;
                if (this->mDetectionStatus == true) {
                    this->mDetectionStatus = false;
                    detection = true;
                }
                return (detection);
            }

        private:
            bool mDetectionStatus;
        };
    }
}
