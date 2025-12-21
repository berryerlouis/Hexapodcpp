#pragma once

#include "../../Component/Battery/BatteryInterface.h"
#include "../ClusterBase.h"

namespace Cluster
{
    namespace Battery
    {
        using namespace Component::Battery;

        class ClusterBattery : public ClusterBase, ClusterCommand {
        public:
            ClusterBattery(BatteryInterface &battery);

            ~ClusterBattery() = default;

            virtual Core::Status ExecuteFrame(const Frame &request,
                                              Frame       &response) override;

            static Core::Status  BuildFrameVoltage(const uint16_t voltage,
                                                   Frame         &response);

            static Core::Status  BuildFrameIntensity(const uint16_t intensity,
                                                     Frame         &response);

            static Core::Status  BuildFrameState(const uint16_t state,
                                                 const uint16_t voltage,
                                                 const uint16_t intensity,
                                                 Frame         &response);

        private:
            BatteryInterface &mBattery;
        };
    } // namespace Battery
} // namespace Cluster
