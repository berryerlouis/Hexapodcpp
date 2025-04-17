#include "ClusterBody.h"

namespace Cluster
{
    namespace Body
    {
        using namespace Bot::Body;

        ClusterBody::ClusterBody(BodyInterface &body) :
            ClusterBase(BODY, this)
            , ClusterCommand(NB_COMMANDS_BODY)
            , mBody(body) {
            this->AddClusterItem((ClusterItem){.commandId = EBodyCommands::SET_BODY_POS_ROT, .expectedSize = 14U});
            this->AddClusterItem((ClusterItem){.commandId = EBodyCommands::SET_LEG_POS_ROT, .expectedSize = 9U});
            this->AddClusterItem((ClusterItem){.commandId = EBodyCommands::SET_WALK_STATUS, .expectedSize = 1U});
        }


        Core::Status ClusterBody::ExecuteFrame(const Frame &request, Frame &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.commandId == EBodyCommands::SET_BODY_POS_ROT) {
                const Position3d position =
                {
                        .x = static_cast<int16_t>(
                                 static_cast<uint16_t>(request.Get1ByteParam(1U)) |
                                 static_cast<uint16_t>(request.Get1ByteParam(0U)) << 8U) / 10.0f,
                        .y = static_cast<int16_t>(
                                 static_cast<uint16_t>(request.Get1ByteParam(3U)) |
                                 static_cast<uint16_t>(request.Get1ByteParam(2U)) << 8U) / 10.0f,
                        .z = static_cast<int16_t>(
                                 static_cast<uint16_t>(request.Get1ByteParam(5U)) |
                                 static_cast<uint16_t>(request.Get1ByteParam(4U)) << 8U) / 10.0f
                };
                const Rotation3d rotation =
                {
                        .angleX = static_cast<int16_t>(
                                      static_cast<uint16_t>(request.Get1ByteParam(7U)) |
                                      static_cast<uint16_t>(request.Get1ByteParam(6U)) << 8U) / 10.0f,
                        .angleY = static_cast<int16_t>(
                                      static_cast<uint16_t>(request.Get1ByteParam(9U)) |
                                      static_cast<uint16_t>(request.Get1ByteParam(8U)) << 8U) / 10.0f,
                        .angleZ = static_cast<int16_t>(
                                      static_cast<uint16_t>(request.Get1ByteParam(11U)) |
                                      static_cast<uint16_t>(request.Get1ByteParam(10U)) << 8U) / 10.0f
                };
                const uint16_t travelTime = static_cast<int16_t>(
                    static_cast<uint16_t>(request.Get1ByteParam(13U)) |
                    static_cast<uint16_t>(request.Get1ByteParam(12U)) << 8U);
                const uint32_t successMove = this->mBody.SetBodyPositionRotation(position, rotation, travelTime);
                success = this->BuildFrameSetBodyPosition(response, successMove);
            } else if (request.commandId == EBodyCommands::SET_LEG_POS_ROT) {
                const uint8_t legId = request.Get1ByteParam(0U);
                const Position3d position =
                {
                        .x = static_cast<int16_t>(
                                 static_cast<uint16_t>(request.Get1ByteParam(2U)) |
                                 static_cast<uint16_t>(request.Get1ByteParam(1U)) << 8U) / 10.0f,
                        .y = static_cast<int16_t>(
                                 static_cast<uint16_t>(request.Get1ByteParam(4U)) |
                                 static_cast<uint16_t>(request.Get1ByteParam(3U)) << 8U) / 10.0f,
                        .z = static_cast<int16_t>(
                                 static_cast<uint16_t>(request.Get1ByteParam(6U)) |
                                 static_cast<uint16_t>(request.Get1ByteParam(5U)) << 8U) / 10.0f
                };
                const uint16_t travelTime = static_cast<int16_t>(
                    static_cast<uint16_t>(request.Get1ByteParam(8U)) |
                    static_cast<uint16_t>(request.Get1ByteParam(7U)) << 8U);
                const uint32_t successMove = this->mBody.SetLegPositionRotation(legId, position, travelTime);
                success = this->BuildFrameSetLegPosition(response, successMove);
            } else if (request.commandId == EBodyCommands::SET_WALK_STATUS) {
                const Bot::EWalkStatus status = static_cast<Bot::EWalkStatus>(request.Get1ByteParam(0U));
                this->mBody.UpdateWalkStatus(status);
                success = this->BuildFrameUpdateWalkStatus(response);
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameSetBodyPosition(Frame &response, const uint32_t successMove) {
            const Core::Status success = response.Build(
                    EClusters::BODY,
                    EBodyCommands::SET_BODY_POS_ROT);
            if (success == Core::Status::CORE_OK) {
                response.Set4BytesParam(successMove);
            }
            return (success);
        }

        Core::Status ClusterBody::BuildFrameSetLegPosition(Frame &response, const uint32_t successMove) {
            const Core::Status success = response.Build(
                    EClusters::BODY,
                    EBodyCommands::SET_LEG_POS_ROT);
            if (success == Core::Status::CORE_OK) {
                response.Set4BytesParam(successMove);
            }
            return (success);
        }

        Core::Status ClusterBody::BuildFrameUpdateWalkStatus(Frame &response) {
            return response.Build(EClusters::BODY, EBodyCommands::SET_WALK_STATUS);
        }

    };
}
