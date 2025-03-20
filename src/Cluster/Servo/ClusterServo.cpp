#include "ClusterServo.h"

namespace Cluster
{
    namespace Servo
    {
        using namespace Component::Servos;

        ClusterServo::ClusterServo(ServosInterface &servos) :
            ClusterBase(SERVO, this)
            , ClusterCommand(NB_COMMANDS_SERVO)
            , mServosInterface(servos) {
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_ALL, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_ANGLE, .expectedSize = 1U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::SET_ANGLE, .expectedSize = 2U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_MIN, .expectedSize = 1U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::SET_MIN, .expectedSize = 2U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_MAX, .expectedSize = 1U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::SET_MAX, .expectedSize = 2U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_OFFSET, .expectedSize = 1U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::SET_OFFSET, .expectedSize = 2U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_STATE, .expectedSize = 1U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::SET_STATE, .expectedSize = 2U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_REVERSE, .expectedSize = 1U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::SET_REVERSE, .expectedSize = 2U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_STATE_PCA, .expectedSize = 0U});
            this->AddClusterItem((ClusterItem){.commandId = EServoCommands::GET_STATE_PCA, .expectedSize = 1U});
        }


        Core::Status ClusterServo::ExecuteFrame(const Frame &request, Frame &response) {
            if (request.commandId == EServoCommands::GET_ALL) {
                uint8_t params[NB_SERVOS] = {0U};
                for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++) {
                    params[servoId] = this->mServosInterface.GetServo(servoId).GetAngle();
                }
                return BuildFrameAllAngle(params, response);
            }
            if (request.commandId == EServoCommands::GET_ANGLE) {
                const uint8_t servoId = request.params[0U];
                const uint8_t angle = this->mServosInterface.GetServo(servoId).GetAngle();
                return this->BuildFrameGetAngle(servoId, angle, response);
            }
            if (request.commandId == EServoCommands::SET_ANGLE) {
                const uint8_t servoId = request.params[0U];
                const uint8_t angle = request.params[1U];
                const Core::Status success = this->mServosInterface.GetServo(servoId).SetAngle(angle);
                if (success == Core::Status::CORE_OK) {
                    const uint8_t angleServo = this->mServosInterface.GetServo(servoId).GetAngle();
                    return this->BuildFrameSetAngle(servoId, angleServo, response);
                }
                return this->BuildFrameNack(response, success);
            }
            if (request.commandId == EServoCommands::GET_MIN) {
                const uint8_t servoId = request.params[0U];
                const uint8_t angle = this->mServosInterface.GetServo(servoId).GetMin();
                return this->BuildFrameGetMinAngle(servoId, angle, response);
            }
            if (request.commandId == EServoCommands::SET_MIN) {
                const uint8_t servoId = request.params[0U];
                const uint8_t angle = request.params[1U];
                this->mServosInterface.GetServo(servoId).SetMin(angle);
                return this->BuildFrameSetMinAngle(servoId, angle, response);
            }
            if (request.commandId == EServoCommands::GET_MAX) {
                const uint8_t servoId = request.params[0U];
                const uint8_t angle = this->mServosInterface.GetServo(servoId).GetMax();
                return this->BuildFrameGetMaxAngle(servoId, angle, response);
            }
            if (request.commandId == EServoCommands::SET_MAX) {
                const uint8_t servoId = request.params[0U];
                const uint8_t angle = request.params[1U];
                this->mServosInterface.GetServo(servoId).SetMax(angle);
                return this->BuildFrameSetMaxAngle(servoId, angle, response);
            }
            if (request.commandId == EServoCommands::GET_OFFSET) {
                const uint8_t servoId = request.params[0U];
                const int8_t angle = this->mServosInterface.GetServo(servoId).GetOffset();
                return this->BuildFrameGetOffset(servoId, angle, response);
            }
            if (request.commandId == EServoCommands::SET_OFFSET) {
                const uint8_t servoId = request.params[0U];
                const int8_t angle = request.params[1U];
                this->mServosInterface.GetServo(servoId).SetOffset(angle);
                return this->BuildFrameSetOffset(servoId, angle, response);
            }
            if (request.commandId == EServoCommands::GET_STATE) {
                const uint8_t servoId = request.params[0U];
                const bool state = this->mServosInterface.GetServo(servoId).IsEnable();
                return this->BuildFrameGetState(servoId, state, response);
            }
            if (request.commandId == EServoCommands::SET_STATE) {
                const uint8_t servoId = request.params[0U];
                const bool state = request.params[1U];
                this->mServosInterface.GetServo(servoId).SetEnable(state);
                return this->BuildFrameSetState(servoId, state, response);
            }
            if (request.commandId == EServoCommands::GET_REVERSE) {
                const uint8_t servoId = request.params[0U];
                const bool reversed = this->mServosInterface.GetServo(servoId).GetReverse();
                return this->BuildFrameGetReverse(servoId, reversed, response);
            }
            if (request.commandId == EServoCommands::SET_REVERSE) {
                const uint8_t servoId = request.params[0U];
                const bool reversed = request.params[1U];
                this->mServosInterface.GetServo(servoId).SetReverse(reversed);
                return this->BuildFrameSetReverse(servoId, reversed, response);
            }
            if (request.commandId == EServoCommands::GET_STATE_PCA) {
                const bool state = this->mServosInterface.GetState();
                return this->BuildFrameGetStatePca(state, response);
            }
            if (request.commandId == EServoCommands::SET_STATE_PCA) {
                const bool state = request.params[0U];
                if (state == true) {
                    this->mServosInterface.Enable();
                } else {
                    this->mServosInterface.Disable();
                }
                return this->BuildFrameSetStatePca(state, response);
            }
            return Core::Status::CORE_ERROR;
        }

        Core::Status ClusterServo::BuildFrameAllAngle(const uint8_t angles[NB_SERVOS], Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_ALL);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(NB_SERVOS, angles);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetAngle(const uint8_t servoId, const uint8_t angle,
                                                      Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_ANGLE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetAngle(const uint8_t servoId, const uint8_t angle,
                                                      Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_ANGLE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetMinAngle(const uint8_t servoId, const uint8_t angle,
                                                         Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_MIN);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetMinAngle(const uint8_t servoId, const uint8_t angle,
                                                         Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_MIN);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetMaxAngle(const uint8_t servoId, const uint8_t angle,
                                                         Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_MAX);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetMaxAngle(const uint8_t servoId, const uint8_t angle,
                                                         Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_MAX);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetOffset(const uint8_t servoId, const uint8_t angle,
                                                       Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_OFFSET);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetOffset(const uint8_t servoId, const uint8_t angle,
                                                       Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_OFFSET);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetState(const uint8_t servoId, const bool state,
                                                      Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_STATE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(state);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetState(const uint8_t servoId, const bool state,
                                                      Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_STATE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(state);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetReverse(const uint8_t servoId, const bool reversed,
                                                        Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_REVERSE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(reversed);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetReverse(const uint8_t servoId, const bool reversed,
                                                        Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_REVERSE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(reversed);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetStatePca(const bool state, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_STATE_PCA);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(state);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetStatePca(const bool state, Frame &response) {
            const Core::Status success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_STATE_PCA);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(state);
            }
            return (success);
        }

    };
}

