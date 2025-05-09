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

            static Core::Status BuildFrameUpdateDirection(Frame &response, const bool successDirection);

            static Core::Status BuildFrameUpdateAmplitudeElevation(
                    Frame &response, const bool successAmplitudeElevation);

            static Core::Status BuildFrameUpdateDirectionAmplitudeElevation(
                    Frame &response, const uint8_t amplitude, const uint8_t elevation, const uint16_t direction);

        private:
            BodyInterface &mBody;
        };
    }
}
