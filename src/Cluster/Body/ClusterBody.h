#pragma once

#include "../ClusterBase.h"
#include "../../Bot/Body/BodyInterface.h"

namespace Cluster
{
    namespace Body
    {
        using namespace Bot::Body;

        class ClusterBody : public ClusterBase, StrategyCluster {
        public:
            ClusterBody(BodyInterface &body)
                : ClusterBase(BODY, this)
                  , StrategyCluster(NB_COMMANDS_BODY)
                  , mBody(body) {
                this->AddClusterItem((ClusterItem){.commandId = EBodyCommands::SET_LEG_X_Y_Z, .expectedSize = 14U});
            }

            ~ClusterBody() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (request.commandId == EBodyCommands::SET_LEG_X_Y_Z) {
                    const Position3d position =
                    {
                        .x = static_cast<int16_t>(request.Get2BytesParam(0U)) / 10.0f,
                        .y = static_cast<int16_t>(request.Get2BytesParam(2U)) / 10.0f,
                        .z = static_cast<int16_t>(request.Get2BytesParam(4U)) / 10.0f
                    };
                    const Rotation3d rotation =
                    {
                        .angleX = static_cast<int16_t>(request.Get2BytesParam(6U)) / 10.0f,
                        .angleY = static_cast<int16_t>(request.Get2BytesParam(8U)) / 10.0f,
                        .angleZ = static_cast<int16_t>(request.Get2BytesParam(10U)) / 10.0f
                    };
                    const uint16_t travelTime = request.Get2BytesParam(12U);
                    const uint32_t successMove = this->mBody.SetPositionRotation(position, rotation, travelTime);
                    success = this->BuildFrameSetPosition(response, successMove);
                }
                return success;
            }

            inline Core::CoreStatus BuildFrameSetPosition(Frame &response, const uint32_t successMove) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::BODY,
                    EBodyCommands::SET_LEG_X_Y_Z);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set4BytesParam(successMove);
                }
                return (success);
            }

        private:
            BodyInterface &mBody;
        };
    }
}
