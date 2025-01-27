#pragma once

#include "../ClusterBase.h"
#include "../../Bot/Body/BodyInterface.h"

namespace Cluster
{
    namespace Body
    {
        using namespace Bot::Body;

        class ClusterBody : public ClusterBase, ClusterCommand {
        public:
            ClusterBody(BodyInterface &body);

            ~ClusterBody() = default;

            virtual Core::Status ExecuteFrame(const Frame &request, Frame &response) override;

            static Core::Status BuildFrameSetBodyPosition(Frame &response, const uint32_t successMove);

            static Core::Status BuildFrameSetLegPosition(Frame &response, const uint32_t successMove);

            static Core::Status BuildFrameUpdateWalkStatus(Frame &response);

        private:
            BodyInterface &mBody;
        };
    }
}
