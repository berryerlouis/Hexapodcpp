#pragma once

#include "../ClusterBase.h"
#include "../../Component/Software/SoftwareInterface.h"

namespace Cluster
{
    namespace General
    {
        using namespace Component::Software;

        class ClusterGeneral : public ClusterBase, StrategyCluster {
        public:
            ClusterGeneral(SoftwareInterface &software)
                : ClusterBase(GENERAL, this)
                  , mSoftware(software) {
                this->AddClusterItem({.commandId = EGeneralCommands::VERSION, .expectedSize = 0U});
                this->AddClusterItem({.commandId = EGeneralCommands::MIN_EXECUTION_TIME, .expectedSize = 1U});
                this->AddClusterItem({.commandId = EGeneralCommands::MAX_EXECUTION_TIME, .expectedSize = 1U});
                this->AddClusterItem({.commandId = EGeneralCommands::RESET_EXECUTION_TIME, .expectedSize = 1U});
            }

            ~ClusterGeneral() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (request.commandId == EGeneralCommands::VERSION) {
                    const SoftwareInterface::Version version = this->mSoftware.GetVersion();
                    success = this->BuildFrameGetVersion(version, response);
                } else if (request.commandId == EGeneralCommands::MIN_EXECUTION_TIME) {
                    const uint8_t serviceId = request.params[0U];
                    success = this->BuildFrameGetMinTime(serviceId, 0, response);
                } else if (request.commandId == EGeneralCommands::MAX_EXECUTION_TIME) {
                    const uint8_t serviceId = request.params[0U];
                    success = this->BuildFrameGetMaxTime(serviceId, 0, response);
                } else if (request.commandId == EGeneralCommands::RESET_EXECUTION_TIME) {
                }
                return success;
            }

            inline Core::CoreStatus BuildFrameGetVersion(const SoftwareInterface::Version version,
                                                         Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::VERSION);
                if (success) {
                    response.Set1ByteParam(version.major);
                    response.Set1ByteParam(version.minor);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetMinTime(const uint8_t serviceId, const uint16_t deltaTime,
                                                         Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::MIN_EXECUTION_TIME);
                if (success) {
                    response.Set1ByteParam(serviceId);
                    response.Set2BytesParam(deltaTime);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetMaxTime(const uint8_t serviceId, const uint16_t deltaTime,
                                                         Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::MAX_EXECUTION_TIME);
                if (success) {
                    response.Set1ByteParam(serviceId);
                    response.Set2BytesParam(deltaTime);
                }
                return (success);
            }

        private:
            SoftwareInterface &mSoftware;
        };
    }
}
