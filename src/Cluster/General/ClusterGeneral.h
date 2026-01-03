#pragma once

#include "../../Component/Software/SoftwareInterface.h"
#include "../ClusterBase.h"

namespace Cluster
{
    namespace General
    {
        using namespace Component::Software;

        class ClusterGeneral : public ClusterBase, ClusterCommand {
        public:
            explicit ClusterGeneral(SoftwareInterface &software);

            ~ClusterGeneral() = default;

            virtual Core::Status ExecuteFrame(const Frame &request,
                                              Frame &response) final override;

            static Core::Status
            BuildFrameReset(Frame &response, const Core::Status successReset);

            static Core::Status
            BuildFrameGetVersion(const SoftwareInterface::Version version,
                                 Frame                           &response);

            static Core::Status BuildFrameGetMinTime(const uint8_t  serviceId,
                                                     const uint64_t deltaTime,
                                                     Frame         &response);

            static Core::Status BuildFrameGetMaxTime(const uint8_t  serviceId,
                                                     const uint64_t deltaTime,
                                                     Frame         &response);

            static Core::Status BuildFrameResetTime(const uint8_t serviceId,
                                                    Frame        &response);

        private:
            SoftwareInterface &mSoftware;
        };
    } // namespace General
} // namespace Cluster
