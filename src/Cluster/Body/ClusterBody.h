#pragma once

#include "../../Bot/Body/BodyInterface.h"
#include "../ClusterBase.h"

#include "../../Move/Gait/Constants.h"
#include "../../Move/Walk/Constants.h"

namespace Cluster
{
    namespace Body
    {
        using namespace Bot::Body;
        using Move::Walk::EWalkStatus;

        class ClusterBody : public ClusterBase, ClusterCommand {
        public:
            explicit ClusterBody(BodyInterface &body);

            ~ClusterBody() = default;

            virtual Core::Status ExecuteFrame(const Frame &request, Frame &response) override;

            static Core::Status  BuildFrameSetBodyPosition(Frame         &response,
                                                           const uint32_t successMove);

            static Core::Status  BuildFrameSetLegPosition(Frame         &response,
                                                          const uint32_t successMove);

            static Core::Status  BuildFrameUpdateWalkStatus(Frame &response, EWalkStatus walkStatus);

            static Core::Status  BuildFrameUpdateDirection(Frame         &response,
                                                           const uint16_t direction);

            static Core::Status
            BuildFrameUpdateRotation(Frame &response, uint16_t rotation, bool clockWize);

            static Core::Status BuildFrameUpdateAmplitude(Frame &response, const uint8_t amplitude);

            static Core::Status BuildFrameUpdateElevation(Frame &response, const uint8_t elevation);

            static Core::Status BuildFrameUpdateAll(Frame         &response,
                                                    const Move::Walk::EWalkStatus walkStatus,
                                                    const Move::Gait::GaitType gait,
                                                    const uint8_t  amplitude,
                                                    const uint8_t  elevation,
                                                    const uint16_t direction,
                                                    const uint16_t rotation,
                                                    const bool     clockWize,
                                                    const uint16_t duration);

            static Core::Status BuildFrameUpdateDuration(Frame &response, const uint16_t duration);

            static Core::Status BuildFrameUpdateGait(Frame &response, Move::Gait::GaitType gait);

        private:
            BodyInterface &mBody;
        };
    } // namespace Body
} // namespace Cluster
