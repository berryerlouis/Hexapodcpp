#pragma once

#include "../../Component/Button/ButtonInterface.h"
#include "../ClusterBase.h"

namespace Cluster
{
    namespace Button
    {
        using namespace Component::Button;

        class ClusterButton : public ClusterBase, ClusterCommand {
        public:
            explicit ClusterButton(ButtonInterface &button);

            ~ClusterButton() = default;

            virtual Core::Status ExecuteFrame(const Frame &request, Frame &response) override;

            static Core::Status  BuildFrameGetButtonState(const ButtonState state, Frame &response);

        private:
            ButtonInterface &mButton;
        };
    } // namespace Button
} // namespace Cluster
