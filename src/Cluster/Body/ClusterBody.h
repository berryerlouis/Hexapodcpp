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
                this->AddClusterItem((ClusterItem){.commandId = EBodyCommands::SET_BODY_POS_ROT, .expectedSize = 14U});
                this->AddClusterItem((ClusterItem){.commandId = EBodyCommands::SET_LEG_POS_ROT, .expectedSize = 9U});
                this->AddClusterItem((ClusterItem){.commandId = EBodyCommands::SET_WALK_STATUS, .expectedSize = 1U});
            }

            ~ClusterBody() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (request.commandId == EBodyCommands::SET_BODY_POS_ROT) {
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
                    const uint32_t successMove = this->mBody.SetBodyPositionRotation(position, rotation, travelTime);
                    success = this->BuildFrameSetBodyPosition(response, successMove);
                } else if (request.commandId == EBodyCommands::SET_LEG_POS_ROT) {
                    const uint8_t legId = request.Get1ByteParam(0U);
                    const Position3d position =
                    {
                        .x = static_cast<int16_t>(request.Get2BytesParam(1U)) / 10.0f,
                        .y = static_cast<int16_t>(request.Get2BytesParam(3U)) / 10.0f,
                        .z = static_cast<int16_t>(request.Get2BytesParam(5U)) / 10.0f
                    };
                    const uint16_t travelTime = request.Get2BytesParam(7U);
                    const uint32_t successMove = this->mBody.SetLegPositionRotation(legId, position, travelTime);
                    success = this->BuildFrameSetLegPosition(response, successMove);
                } else if (request.commandId == EBodyCommands::SET_WALK_STATUS) {
                    const Bot::EWalkStatus status = static_cast<Bot::EWalkStatus>(request.Get1ByteParam(0U));
                    this->mBody.UpdateWalkStatus(status);
                    success = this->BuildFrameUpdateWalkStatus(response);
                }
                return success;
            }

            inline Core::CoreStatus BuildFrameSetBodyPosition(Frame &response, const uint32_t successMove) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::BODY,
                    EBodyCommands::SET_BODY_POS_ROT);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set4BytesParam(successMove);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameSetLegPosition(Frame &response, const uint32_t successMove) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::BODY,
                    EBodyCommands::SET_LEG_POS_ROT);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set4BytesParam(successMove);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameUpdateWalkStatus(Frame &response) const {
                return response.Build(EClusters::BODY, EBodyCommands::SET_WALK_STATUS);
            }

        private:
            BodyInterface &mBody;
        };
    }
}
