#include "ClusterBody.h"

#include "../../Move/Gait/Constants.h"

namespace Cluster
{
    namespace Body
    {
        using namespace Bot::Body;

        ClusterBody::ClusterBody(BodyInterface &body)
            : ClusterBase(BODY, *this)
            , ClusterCommand(NB_COMMANDS_BODY)
            , mBody(body) {
            this->AddClusterItem(ClusterItem(EBodyCommands::GET_ALL_PARAMS, 0U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_BODY_POS_ROT, 14U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_LEG_POS_ROT, 9U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_WALK_STATUS, 3U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_DIRECTION, 2U));
            this->AddClusterItem(ClusterItem(EBodyCommands::GET_DIRECTION, 0U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_AMPLITUDE, 1U));
            this->AddClusterItem(ClusterItem(EBodyCommands::GET_AMPLITUDE, 0U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_ELEVATION, 1U));
            this->AddClusterItem(ClusterItem(EBodyCommands::GET_ELEVATION, 0U));
            this->AddClusterItem(ClusterItem(EBodyCommands::GET_DURATION, 0U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_DURATION, 2U));
            this->AddClusterItem(ClusterItem(EBodyCommands::GET_GAIT, 0U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_GAIT, 1U));
            this->AddClusterItem(ClusterItem(EBodyCommands::SET_ROTATION, 3U));
            this->AddClusterItem(ClusterItem(EBodyCommands::GET_ROTATION, 0U));
            LOG_CLUSTER_DEBUG("Body", "(%d) Initialized.", BODY);
        }

        Core::Status ClusterBody::ExecuteFrame(const Frame &request, Frame &response) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (request.GetCommandId() == EBodyCommands::GET_ALL_PARAMS) {
                const Move::Walk::EWalkStatus walkStatus = this->mBody.GetWalkStatus();
                const Move::Gait::GaitType gait = this->mBody.GetGait();
                const uint8_t  amplitude = this->mBody.GetAmplitude() * 10U;
                const uint8_t  elevation = this->mBody.GetElevation() * 10U;
                const uint16_t direction = Misc::Utils::ToDeg(this->mBody.GetDirection());
                const uint16_t rotation = Misc::Utils::ToDeg(this->mBody.GetRotation());
                const bool     clockWize = this->mBody.GetRotationClockWize();
                const uint16_t duration = this->mBody.GetDuration();
                success = BuildFrameUpdateAll(
                        response,walkStatus, gait, amplitude, elevation, direction, rotation, clockWize, duration);
            } else if (request.GetCommandId() == EBodyCommands::SET_BODY_POS_ROT) {
                const Position3d position = {.x = request.Get2BytesParam(0U) / 10.0F,
                                             .y = request.Get2BytesParam(2U) / 10.0F,
                                             .z = request.Get2BytesParam(4U) / 10.0F};
                const Rotation3d rotation = {.angleX = request.Get2BytesParam(6U) / 10.0F,
                                             .angleY = request.Get2BytesParam(8U) / 10.0F,
                                             .angleZ = request.Get2BytesParam(10U) / 10.0F};
                const uint16_t   travelTime = request.Get2BytesParam(12U);
                const uint32_t   successMove =
                        this->mBody.SetBodyPositionRotation(position, rotation, travelTime);
                success = BuildFrameSetBodyPosition(response, successMove);
            } else if (request.GetCommandId() == EBodyCommands::SET_LEG_POS_ROT) {
                const uint8_t    legId = request.Get1ByteParam(0U);
                const Position3d position = {.x = request.Get2BytesParam(1U) / 10.0F,
                                             .y = request.Get2BytesParam(3U) / 10.0F,
                                             .z = request.Get2BytesParam(5U) / 10.0F};
                const uint16_t   travelTime = request.Get2BytesParam(7U);
                const uint32_t   successMove =
                        this->mBody.SetLegPositionRotation(legId, position, travelTime);
                success = BuildFrameSetLegPosition(response, successMove);
            } else if (request.GetCommandId() == EBodyCommands::SET_WALK_STATUS) {
                const Move::Walk::EWalkStatus status =
                        static_cast<Move::Walk::EWalkStatus>(request.Get1ByteParam(0U));
                const uint16_t travelTime = request.Get2BytesParam(1U);
                this->mBody.UpdateWalkStatus(status, travelTime);
                success = BuildFrameUpdateWalkStatus(
                        response, this->mBody.GetWalkStatus());
            } else if (request.GetCommandId() == EBodyCommands::GET_DIRECTION) {
                success = BuildFrameUpdateDirection(response,
                                                    Misc::Utils::ToDeg(this->mBody.GetDirection()));
            } else if (request.GetCommandId() == EBodyCommands::SET_DIRECTION) {
                const uint16_t direction = request.Get2BytesParam(0U);
                this->mBody.SetDirection(Misc::Utils::ToRad(direction));
                success = BuildFrameUpdateDirection(response,
                                                    Misc::Utils::ToDeg(this->mBody.GetDirection()));
            } else if (request.GetCommandId() == EBodyCommands::GET_ROTATION) {
                success = BuildFrameUpdateRotation(response,
                                                   Misc::Utils::ToDeg(this->mBody.GetRotation()),
                                                   this->mBody.GetRotationClockWize());
            } else if (request.GetCommandId() == EBodyCommands::SET_ROTATION) {
                const uint16_t rotation = request.Get2BytesParam(0U);
                const bool     clockWize = static_cast<bool>(request.Get1ByteParam(2U));
                this->mBody.SetRotation(Misc::Utils::ToRad(rotation), clockWize);
                success = BuildFrameUpdateRotation(response,
                                                   Misc::Utils::ToDeg(this->mBody.GetRotation()),
                                                   this->mBody.GetRotationClockWize());
            } else if (request.GetCommandId() == EBodyCommands::GET_AMPLITUDE) {
                success = BuildFrameUpdateAmplitude(response, this->mBody.GetAmplitude() * 10U);
            } else if (request.GetCommandId() == EBodyCommands::SET_AMPLITUDE) {
                const uint8_t amplitude = request.Get1ByteParam(0U);
                this->mBody.SetAmplitude(amplitude / 10.0F);
                success = BuildFrameUpdateAmplitude(response, this->mBody.GetAmplitude() * 10U);
            } else if (request.GetCommandId() == EBodyCommands::GET_ELEVATION) {
                success = BuildFrameUpdateElevation(response, this->mBody.GetElevation() * 10U);
            } else if (request.GetCommandId() == EBodyCommands::SET_ELEVATION) {
                const uint8_t elevation = request.Get1ByteParam(0U);
                this->mBody.SetElevation(elevation / 10.0F);
                success = BuildFrameUpdateElevation(response, this->mBody.GetElevation() * 10U);
            } else if (request.GetCommandId() == EBodyCommands::GET_DURATION) {
                success = BuildFrameUpdateDuration(response, this->mBody.GetDuration());
            } else if (request.GetCommandId() == EBodyCommands::SET_DURATION) {
                const uint16_t duration = request.Get2BytesParam(0U);
                this->mBody.SetDuration(duration);
                success = BuildFrameUpdateDuration(response, this->mBody.GetDuration());
            } else if (request.GetCommandId() == EBodyCommands::GET_GAIT) {
                success = BuildFrameUpdateGait(response, this->mBody.GetGait());
            } else if (request.GetCommandId() == EBodyCommands::SET_GAIT) {
                const Move::Gait::GaitType gait =
                        static_cast<Move::Gait::GaitType>(request.Get1ByteParam(0U));
                this->mBody.SetGait(gait);
                success = BuildFrameUpdateGait(response, this->mBody.GetGait());
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameSetBodyPosition(Frame         &response,
                                                            const uint32_t successMove) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::SET_BODY_POS_ROT);
            if (success == Core::Status::CORE_OK) {
                response.Set4BytesParam(successMove);
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameSetLegPosition(Frame         &response,
                                                           const uint32_t successMove) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::SET_LEG_POS_ROT);
            if (success == Core::Status::CORE_OK) {
                response.Set4BytesParam(successMove);
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameUpdateWalkStatus(Frame        &response,
                                                             const EWalkStatus walkStatus) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::SET_WALK_STATUS);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(static_cast<uint8_t>(walkStatus));
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameUpdateDirection(Frame         &response,
                                                            const uint16_t direction) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::GET_DIRECTION);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(direction);
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameUpdateRotation(Frame         &response,
                                                           const uint16_t rotation,
                                                           const bool     clockWize) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::GET_ROTATION);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(rotation);
                response.Set1ByteParam(static_cast<uint8_t>(clockWize));
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameUpdateAmplitude(Frame        &response,
                                                            const uint8_t amplitude) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::GET_AMPLITUDE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(amplitude);
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameUpdateElevation(Frame        &response,
                                                            const uint8_t elevation) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::GET_ELEVATION);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(elevation);
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameUpdateAll(Frame         &response,
                                                      const Move::Walk::EWalkStatus walkStatus,
                                                      const Move::Gait::GaitType gait,
                                                      const uint8_t  amplitude,
                                                      const uint8_t  elevation,
                                                      const uint16_t direction,
                                                      const uint16_t rotation,
                                                      const bool     clockWize,
                                                      const uint16_t duration) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::GET_ALL_PARAMS);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(static_cast<uint8_t>(walkStatus));
                response.Set1ByteParam(static_cast<uint8_t>(gait));
                response.Set1ByteParam(amplitude);
                response.Set1ByteParam(elevation);
                response.Set2BytesParam(direction);
                response.Set2BytesParam(rotation);
                response.Set1ByteParam(static_cast<uint8_t>(clockWize));
                response.Set2BytesParam(duration);
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameUpdateDuration(Frame         &response,
                                                           const uint16_t duration) {
            const Core::Status success =
                    response.Build(EClusters::BODY, EBodyCommands::GET_DURATION);
            if (success == Core::Status::CORE_OK) {
                response.Set2BytesParam(duration);
            }
            return success;
        }

        Core::Status ClusterBody::BuildFrameUpdateGait(Frame                     &response,
                                                       const Move::Gait::GaitType gait) {
            const Core::Status success = response.Build(EClusters::BODY, EBodyCommands::GET_GAIT);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(static_cast<uint8_t>(gait));
            }
            return success;
        }
    }; // namespace Body
} // namespace Cluster
