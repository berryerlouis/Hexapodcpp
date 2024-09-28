#pragma once

#include "../ClusterBase.h"
#include "../../Component/Servos/ServosInterface.h"

namespace Cluster
{
    namespace Servo
    {
        using namespace Component::Servos;

        class ClusterServo : public ClusterBase, StrategyCluster {
        public:
            ClusterServo(ServosInterface &servos)
                : ClusterBase(SERVO, this)
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
            }

            ~ClusterServo() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (request.commandId == EServoCommands::GET_ALL) {
                    return BuildFrameAllAngle(response);
                }
                if (request.commandId == EServoCommands::GET_ANGLE) {
                    const uint8_t servoId = request.params[0U];
                    const uint8_t angle = this->mServosInterface.GetServo(servoId).GetAngle();
                    return this->BuildFrameGetAngle(servoId, angle, response);
                }
                if (request.commandId == EServoCommands::SET_ANGLE) {
                    const uint8_t servoId = request.params[0U];
                    const uint8_t angle = request.params[1U];
                    if (this->mServosInterface.GetServo(servoId).SetAngle(angle) == true) {
                        const uint8_t angleServo = this->mServosInterface.GetServo(servoId).GetAngle();
                        return this->BuildFrameSetAngle(servoId, angleServo, response);
                    }
                    return this->BuildFrameNack(response);
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
                return success;
            }

            inline Core::CoreStatus BuildFrameAllAngle(Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_ALL);
                if (success) {
                    uint8_t params[NB_SERVOS] = {0U};
                    for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++) {
                        params[servoId] = this->mServosInterface.GetServo(servoId).GetAngle();
                    }
                    response.SetxBytesParam(NB_SERVOS, reinterpret_cast<uint8_t *>(&params));
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetAngle(const uint8_t servoId, const uint8_t angle,
                                                       Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_ANGLE);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(angle);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameSetAngle(const uint8_t servoId, const uint8_t angle,
                                                       Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_ANGLE);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(angle);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetMinAngle(const uint8_t servoId, const uint8_t angle,
                                                          Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_MIN);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(angle);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameSetMinAngle(const uint8_t servoId, const uint8_t angle,
                                                          Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_MIN);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(angle);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetMaxAngle(const uint8_t servoId, const uint8_t angle,
                                                          Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_MAX);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(angle);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameSetMaxAngle(const uint8_t servoId, const uint8_t angle,
                                                          Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_MAX);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(angle);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetOffset(const uint8_t servoId, const uint8_t angle,
                                                        Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_OFFSET);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(angle);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameSetOffset(const uint8_t servoId, const uint8_t angle,
                                                        Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_OFFSET);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(angle);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetState(const uint8_t servoId, const bool state,
                                                       Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_STATE);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(state);
                }
                return (success);
            }


            inline Core::CoreStatus BuildFrameSetState(const uint8_t servoId, const bool state,
                                                       Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_STATE);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(state);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGetReverse(const uint8_t servoId, const bool reversed,
                                                         Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::GET_REVERSE);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(reversed);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameSetReverse(const uint8_t servoId, const bool reversed,
                                                         Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::SERVO,
                    EServoCommands::SET_REVERSE);
                if (success) {
                    response.Set1ByteParam(servoId);
                    response.Set1ByteParam(reversed);
                }
                return (success);
            }

        private:
            ServosInterface &mServosInterface;
        };
    }
}
