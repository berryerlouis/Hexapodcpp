#pragma once

#include "../ClusterBase.h"
#include "../../Bot/Body/BodyInterface.h"

#include "../../Move/Gait/Constants.h"

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

            static Core::Status BuildFrameUpdateWalkStatus(Frame &response, uint8_t walkStatus);

            static Core::Status BuildFrameUpdateDirection(Frame &response, const uint16_t direction);

            static Core::Status BuildFrameUpdateRotation(Frame &response, uint16_t rotation, bool clockWize);

            static Core::Status BuildFrameUpdateAmplitude(Frame &response, const uint8_t successAmplitude);

            static Core::Status BuildFrameUpdateElevation(Frame &response, const uint8_t successElevation);

            static Core::Status BuildFrameUpdateAll(Frame &response,
                                                    const uint8_t amplitude,
                                                    const uint8_t elevation,
                                                    const uint16_t direction,
                                                    const uint16_t rotation,
                                                    const bool clockWize,
                                                    const uint16_t duration);

            static Core::Status BuildFrameUpdateDuration(Frame &response, const uint16_t duration);

            Core::Status BuildFrameUpdateGait(Frame &response, Move::Gait::GaitType gait);

        private:
            BodyInterface &mBody;
        };
    }
}
