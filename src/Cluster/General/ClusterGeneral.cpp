#include "ClusterGeneral.h"
#include "../../Service/Constants.h"

namespace Cluster
{
    namespace General
    {
        using namespace Component::Software;

        ClusterGeneral::ClusterGeneral(SoftwareInterface &software) :
            ClusterBase(GENERAL, this)
            , ClusterCommand(NB_COMMANDS_GENERAL)
            , mSoftware(software) {
            this->AddClusterItem({.commandId = EGeneralCommands::RESET, .expectedSize = 0U});
            this->AddClusterItem({.commandId = EGeneralCommands::VERSION, .expectedSize = 0U});
            this->AddClusterItem({.commandId = EGeneralCommands::MIN_EXECUTION_TIME, .expectedSize = 0U});
            this->AddClusterItem({.commandId = EGeneralCommands::MAX_EXECUTION_TIME, .expectedSize = 0U});
            this->AddClusterItem({.commandId = EGeneralCommands::RESET_EXECUTION_TIME, .expectedSize = 0U});
        }


        Core::Status ClusterGeneral::ExecuteFrame(const Frame &request, Frame &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.GetCommandId() == EGeneralCommands::RESET) {
                success = this->BuildFrameReset(response, Core::Status::CORE_OK);
            } else if (request.GetCommandId() == EGeneralCommands::VERSION) {
                const SoftwareInterface::Version version = this->mSoftware.GetVersion();
                success = this->BuildFrameGetVersion(version, response);
            } else if (request.GetCommandId() == EGeneralCommands::MIN_EXECUTION_TIME) {
                success = this->BuildFrameGetMinTime(Service::EServices::GENERAL, this->mSoftware.GetMinTime(),
                                                     response);
            } else if (request.GetCommandId() == EGeneralCommands::MAX_EXECUTION_TIME) {
                success = this->BuildFrameGetMaxTime(Service::EServices::GENERAL, this->mSoftware.GetMaxTime(),
                                                     response);
            } else if (request.GetCommandId() == EGeneralCommands::RESET_EXECUTION_TIME) {
                success = this->BuildFrameResetTime(Service::EServices::GENERAL, response);
            }
            return success;
        }

        Core::Status ClusterGeneral::BuildFrameReset(Frame &response, const Core::Status successReset) {
            const Core::Status success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::RESET);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(successReset);
            }
            return (success);
        }

        Core::Status ClusterGeneral::BuildFrameGetVersion(const SoftwareInterface::Version version,
                                                          Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::VERSION);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(version.major);
                response.Set1ByteParam(version.minor);
            }
            return (success);
        }

        Core::Status ClusterGeneral::BuildFrameGetMinTime(const uint8_t serviceId, const uint64_t deltaTime,
                                                          Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::MIN_EXECUTION_TIME);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(serviceId);
                response.Set8BytesParam(deltaTime);
            }
            return (success);
        }

        Core::Status ClusterGeneral::BuildFrameGetMaxTime(const uint8_t serviceId, const uint64_t deltaTime,
                                                          Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::MAX_EXECUTION_TIME);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(serviceId);
                response.Set8BytesParam(deltaTime);
            }
            return (success);
        }

        Core::Status ClusterGeneral::BuildFrameResetTime(const uint8_t serviceId, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::GENERAL,
                    EGeneralCommands::RESET_EXECUTION_TIME);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(serviceId);
            }
            return (success);
        }

    }
}
