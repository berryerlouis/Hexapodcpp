#include "ClusterGeneral.h"
#include "../../Service/Constants.h"

namespace Cluster
{
    namespace General
    {
        using namespace Component::Software;

        ClusterGeneral::ClusterGeneral(SoftwareInterface &software)
            : ClusterBase(GENERAL, *this)
            , ClusterCommand(NB_COMMANDS_GENERAL)
            , mSoftware(software) {
            this->AddClusterItem(ClusterItem(EGeneralCommands::RESET, 0U));
            this->AddClusterItem(ClusterItem(EGeneralCommands::VERSION, 0U));
            this->AddClusterItem(ClusterItem(EGeneralCommands::MIN_EXECUTION_TIME, 0U));
            this->AddClusterItem(ClusterItem(EGeneralCommands::MAX_EXECUTION_TIME, 0U));
            this->AddClusterItem(ClusterItem(EGeneralCommands::RESET_EXECUTION_TIME, 0U));
            LOG_CLUSTER_DEBUG("General", "(%d) Initialized.", GENERAL);
        }


        auto ClusterGeneral::ExecuteFrame(const Frame &request, Frame &response) -> Core::Status {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.GetCommandId() == EGeneralCommands::RESET) {
                success = BuildFrameReset(response, Core::Status::CORE_OK);
            } else if (request.GetCommandId() == EGeneralCommands::VERSION) {
                const SoftwareInterface::Version version = this->mSoftware.GetVersion();
                success = BuildFrameGetVersion(version, response);
            } else if (request.GetCommandId() == EGeneralCommands::MIN_EXECUTION_TIME) {
                success = BuildFrameGetMinTime(
                        Service::EServices::GENERAL, this->mSoftware.GetMinTime(), response);
            } else if (request.GetCommandId() == EGeneralCommands::MAX_EXECUTION_TIME) {
                success = BuildFrameGetMaxTime(
                        Service::EServices::GENERAL, this->mSoftware.GetMaxTime(), response);
            } else if (request.GetCommandId() == EGeneralCommands::RESET_EXECUTION_TIME) {
                success = BuildFrameResetTime(Service::EServices::GENERAL, response);
            }
            return success;
        }

        Core::Status ClusterGeneral::BuildFrameReset(Frame             &response,
                                                     const Core::Status successReset) {
            const Core::Status success =
                    response.Build(EClusters::GENERAL, EGeneralCommands::RESET);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(successReset);
            }
            return (success);
        }

        Core::Status ClusterGeneral::BuildFrameGetVersion(const SoftwareInterface::Version version,
                                                          Frame &response) {
            const Core::Status success =
                    response.Build(EClusters::GENERAL, EGeneralCommands::VERSION);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(version.major);
                response.Set1ByteParam(version.minor);
            }
            return (success);
        }

        Core::Status ClusterGeneral::BuildFrameGetMinTime(const uint8_t  serviceId,
                                                          const uint64_t deltaTime,
                                                          Frame         &response) {
            const Core::Status success =
                    response.Build(EClusters::GENERAL, EGeneralCommands::MIN_EXECUTION_TIME);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(serviceId);
                response.Set8BytesParam(deltaTime);
            }
            return (success);
        }

        Core::Status ClusterGeneral::BuildFrameGetMaxTime(const uint8_t  serviceId,
                                                          const uint64_t deltaTime,
                                                          Frame         &response) {
            const Core::Status success =
                    response.Build(EClusters::GENERAL, EGeneralCommands::MAX_EXECUTION_TIME);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(serviceId);
                response.Set8BytesParam(deltaTime);
            }
            return (success);
        }

        Core::Status ClusterGeneral::BuildFrameResetTime(const uint8_t serviceId, Frame &response) {
            const Core::Status success =
                    response.Build(EClusters::GENERAL, EGeneralCommands::RESET_EXECUTION_TIME);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(serviceId);
            }
            return (success);
        }
    } // namespace General
} // namespace Cluster
