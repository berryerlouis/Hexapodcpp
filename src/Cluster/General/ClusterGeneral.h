#pragma once

#include "../ClusterBase.h"
#include "../../Component/Software/SoftwareInterface.h"

namespace Cluster
{
    namespace General
    {
        using namespace Component::Software;

        class ClusterGeneral : public ClusterBase {
        public:
            ClusterGeneral(SoftwareInterface &software)
                : ClusterBase(GENERAL)
                  , mSoftware(software) {
            }

            ~ClusterGeneral() = default;

            inline virtual Core::CoreStatus Execute(Frame &request, Frame &response) final override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

                if (request.clusterId != this->GetId()) {
                    return (success);
                }

                switch (static_cast<EGeneralCommands>(request.commandId)) {
                    case EGeneralCommands::VERSION:
                        success = this->BuildFrameGetVersion(response);
                        break;

                    case EGeneralCommands::MIN_EXECUTION_TIME:
                        success = this->BuildFrameGetMinTime(response, 0, 0);
                        break;

                    case EGeneralCommands::MAX_EXECUTION_TIME:
                        success = this->BuildFrameGetMaxTime(response, 0, 0);
                        break;

                    case EGeneralCommands::RESET_EXECUTION_TIME:
                        success = response.Build(EClusters::GENERAL, EGeneralCommands::RESET_EXECUTION_TIME);
                        break;

                    default:
                        break;
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetVersion(Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::VERSION);
                if (success) {
                    SoftwareInterface::Version version = this->mSoftware.GetVersion();
                    response.SetnBytesParam(2U, reinterpret_cast<uint8_t *>(&version));
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetMinTime(Frame &response, const uint8_t serviceId,
                                                         const uint16_t deltaTime) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::MIN_EXECUTION_TIME);
                if (success) {
                    response.Set1ByteParam(serviceId);
                    response.Set2BytesParam(deltaTime);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetMaxTime(Frame &response, const uint8_t serviceId,
                                                         const uint16_t deltaTime) const {
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
