#include "ClusterButton.h"


namespace Cluster
{
    namespace Button
    {
        using namespace Component::Button;

        ClusterButton::ClusterButton(ButtonInterface &button)
            : ClusterBase(BUTTON, *this)
            , ClusterCommand(NB_COMMANDS_BUTTON)
            , mButton(button) {
            this->AddClusterItem(ClusterItem(EButtonCommands::GET_BP_STATUS, 0U));
            LOG_CLUSTER_DEBUG("Button", "(%d) Initialized.", BUTTON);
        }

        auto ClusterButton::ExecuteFrame(const Frame &request, Frame &response) -> Core::Status {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.GetCommandId() == EButtonCommands::GET_BP_STATUS) {
                const ButtonState state = this->mButton.Get();
                success = BuildFrameGetButtonState(state, response);
            }
            return success;
        }

        auto ClusterButton::BuildFrameGetButtonState(const ButtonState state,
                                                     Frame            &response) -> Core::Status {
            const Core::Status success =
                    response.Build(EClusters::BUTTON, EButtonCommands::GET_BP_STATUS);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(state);
            }
            return (success);
        }
    }; // namespace Button
} // namespace Cluster
