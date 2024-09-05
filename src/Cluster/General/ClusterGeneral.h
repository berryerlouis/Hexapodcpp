#pragma once

#include "../Cluster.h"
#include "../../Component/Software/SoftwareInterface.h"

namespace Cluster
{
    namespace General
    {
        using namespace Component::Software;

        class ClusterGeneral : public Cluster {
        public:
            ClusterGeneral(SoftwareInterface &software)
                : Cluster(GENERAL)
                  , mSoftware(software) {
            }

            ~ClusterGeneral() = default;

            inline virtual Core::CoreStatus Execute(Frame &request, Frame &response) final override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

                if (request.clusterId != this->GetId()) {
                    return (success);
                }

                switch ((EGeneralCommands) request.commandId) {
                    case EGeneralCommands::VERSION:
                        success = this->BuildFrameGetVersion(response);
                        break;

                    case EGeneralCommands::MIN_EXECUTION_TIME:
                        success = this->BuildFrameGetMinTime(response);
                        break;

                    case EGeneralCommands::MAX_EXECUTION_TIME:
                        success = this->BuildFrameGetMaxTime(response);
                        break;

                    case EGeneralCommands::RESET_EXECUTION_TIME:
                        this->mSoftware.ResetTiming();
                        success = response.Build(EClusters::GENERAL, EGeneralCommands::RESET_EXECUTION_TIME);
                        break;

                    default:
                        break;
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetVersion(Frame &response) {
                Core::CoreStatus success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::VERSION);
                if (success) {
                    SoftwareInterface::Version version = this->mSoftware.GetVersion();
                    response.SetnBytesParam(2U, (uint8_t *) &version);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetMinTime(Frame &response) {
                Core::CoreStatus success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::MIN_EXECUTION_TIME);
                if (success) {
                    response.Set8BytesParam(this->mSoftware.GetMinTime());
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetMaxTime(Frame &response) {
                Core::CoreStatus success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::MAX_EXECUTION_TIME);
                if (success) {
                    response.Set8BytesParam(this->mSoftware.GetMaxTime());
                }
                return (success);
            }

        private:
            SoftwareInterface &mSoftware;
        };
    }
}
