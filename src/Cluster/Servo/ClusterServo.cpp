#include "ClusterServo.h"

namespace Cluster
{
    namespace Servo
    {
        using namespace Component::Servos;

        ClusterServo::ClusterServo(ServosInterface &servos)
            : ClusterBase(SERVO, *this)
            , ClusterCommand(NB_COMMANDS_SERVO)
            , mServosInterface(servos) {
            this->AddClusterItem(ClusterItem(EServoCommands::GET_ALL, 0U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_ANGLE, 1U));
            this->AddClusterItem(ClusterItem(EServoCommands::SET_ANGLE, 2U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_MIN, 1U));
            this->AddClusterItem(ClusterItem(EServoCommands::SET_MIN, 2U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_MAX, 1U));
            this->AddClusterItem(ClusterItem(EServoCommands::SET_MAX, 2U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_OFFSET, 1U));
            this->AddClusterItem(ClusterItem(EServoCommands::SET_OFFSET, 2U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_STATE, 1U));
            this->AddClusterItem(ClusterItem(EServoCommands::SET_STATE, 2U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_REVERSE, 1U));
            this->AddClusterItem(ClusterItem(EServoCommands::SET_REVERSE, 2U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_STATE_PCA, 0U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_STATE_PCA, 1U));
            this->AddClusterItem(ClusterItem(EServoCommands::GET_SERVO_ALL, 1U));
            LOG_CLUSTER_DEBUG("Servo", "(%d) Initialized.", SERVO);
        }


        Core::Status ClusterServo::ExecuteFrame(const Frame &request, Frame &response) {
            if (request.GetCommandId() == EServoCommands::GET_ALL) {
                uint8_t params[NB_SERVOS] = {0U};
                for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++) {
                    params[servoId] =
                            this->mServosInterface.GetServo(static_cast<EServos>(servoId))
                                    ->GetAngle();
                }
                return BuildFrameAllAngle(params, response);
            }
            if (request.GetCommandId() == EServoCommands::GET_STATE_PCA) {
                const bool state = this->mServosInterface.GetState();
                return BuildFrameGetStatePca(state, response);
            }
            if (request.GetCommandId() == EServoCommands::SET_STATE_PCA) {
                const bool state = static_cast<bool>(request.Get1ByteParam(0U));
                if (state) {
                    this->mServosInterface.Enable();
                } else {
                    this->mServosInterface.Disable();
                }
                return BuildFrameSetStatePca(state, response);
            }

            const uint8_t                     servoId = request.Get1ByteParam(0U);
            Component::Servo::ServoInterface *servo =
                    this->mServosInterface.GetServo(static_cast<EServos>(servoId));
            Core::Status success = Core::Status::CORE_ERROR;
            if (servo == nullptr) {
                BuildFrameSetError(success, servoId, 0U, response);
                return Core::CORE_ERROR;
            }

            if (request.GetCommandId() == EServoCommands::GET_ANGLE) {
                const uint8_t angle = servo->GetAngle();
                return BuildFrameGetAngle(servoId, angle, response);
            }
            if (request.GetCommandId() == EServoCommands::SET_ANGLE) {
                const uint8_t angle = request.Get1ByteParam(1U);
                success = servo->SetAngle(angle, 0U);
                if (success == Core::Status::CORE_OK) {
                    const uint8_t angleServo =
                            this->mServosInterface.GetServo(static_cast<EServos>(servoId))
                                    ->GetAngle();
                    return BuildFrameSetAngle(servoId, angleServo, response);
                }
            }
            if (request.GetCommandId() == EServoCommands::GET_MIN) {
                const uint8_t angle = servo->GetMin();
                return BuildFrameGetMinAngle(servoId, angle, response);
            }
            if (request.GetCommandId() == EServoCommands::SET_MIN) {
                uint8_t angle = request.Get1ByteParam(1U);
                success = servo->SetMin(angle) ? Core::CORE_OK : Core::CORE_ERROR_MIN;
                if (success == Core::Status::CORE_OK) {
                    return BuildFrameSetMinAngle(servoId, angle, response);
                }
                angle = servo->GetAngle();
                return BuildFrameSetError(success, servoId, angle, response);
            }
            if (request.GetCommandId() == EServoCommands::GET_MAX) {
                const uint8_t angle = servo->GetMax();
                return BuildFrameGetMaxAngle(servoId, angle, response);
            }
            if (request.GetCommandId() == EServoCommands::SET_MAX) {
                uint8_t angle = request.Get1ByteParam(1U);
                success = servo->SetMax(angle) ? Core::CORE_OK : Core::CORE_ERROR_MAX;
                if (success == Core::Status::CORE_OK) {
                    return BuildFrameSetMaxAngle(servoId, angle, response);
                }
                angle = servo->GetAngle();
                return BuildFrameSetError(success, servoId, angle, response);
            }
            if (request.GetCommandId() == EServoCommands::GET_OFFSET) {
                const int8_t angle = servo->GetOffset();
                return BuildFrameGetOffset(servoId, angle, response);
            }
            if (request.GetCommandId() == EServoCommands::SET_OFFSET) {
                int8_t angle = request.Get1ByteParam(1U);
                success = servo->SetOffset(angle) ? Core::CORE_OK : Core::CORE_ERROR_MAX;
                if (success == Core::Status::CORE_OK) {
                    return BuildFrameSetOffset(servoId, angle, response);
                }
                angle = servo->GetOffset();
                return BuildFrameSetError(success, servoId, angle, response);
            }
            if (request.GetCommandId() == EServoCommands::GET_STATE) {
                const bool state = servo->IsEnable();
                return BuildFrameGetState(servoId, state, response);
            }
            if (request.GetCommandId() == EServoCommands::SET_STATE) {
                const bool state = static_cast<bool>(request.Get1ByteParam(1U));
                servo->SetEnable(state);
                return BuildFrameSetState(servoId, state, response);
            }
            if (request.GetCommandId() == EServoCommands::GET_REVERSE) {
                const bool reversed = servo->GetReverse();
                return BuildFrameGetReverse(servoId, reversed, response);
            }
            if (request.GetCommandId() == EServoCommands::SET_REVERSE) {
                const bool reversed = static_cast<bool>(request.Get1ByteParam(1U));
                servo->SetReverse(reversed);
                return BuildFrameSetReverse(servoId, reversed, response);
            }
            if (request.GetCommandId() == EServoCommands::GET_SERVO_ALL) {
                const bool state = servo->IsEnable();
                const uint8_t angle = servo->GetAngle();
                const uint8_t offset = servo->GetOffset();
                const uint8_t min = servo->GetMin();
                const uint8_t max = servo->GetMax();
                const bool reversed = servo->GetReverse();
                return BuildFrameGetServoAllParams(servoId, state, angle, offset, min, max, reversed, response);
            }
            return Core::Status::CORE_ERROR;
        }

        Core::Status ClusterServo::BuildFrameAllAngle(const uint8_t angles[NB_SERVOS],
                                                      Frame        &response) {
            const Core::Status success = response.Build(EClusters::SERVO, EServoCommands::GET_ALL);
            if (success == Core::Status::CORE_OK) {
                response.SetxBytesParam(NB_SERVOS, angles);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetAngle(const uint8_t servoId,
                                                      const uint8_t angle,
                                                      Frame        &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::GET_ANGLE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetAngle(const uint8_t servoId,
                                                      const uint8_t angle,
                                                      Frame        &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::SET_ANGLE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetMinAngle(const uint8_t servoId,
                                                         const uint8_t angle,
                                                         Frame        &response) {
            const Core::Status success = response.Build(EClusters::SERVO, EServoCommands::GET_MIN);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetMinAngle(const uint8_t servoId,
                                                         const uint8_t angle,
                                                         Frame        &response) {
            const Core::Status success = response.Build(EClusters::SERVO, EServoCommands::SET_MIN);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetMaxAngle(const uint8_t servoId,
                                                         const uint8_t angle,
                                                         Frame        &response) {
            const Core::Status success = response.Build(EClusters::SERVO, EServoCommands::GET_MAX);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetMaxAngle(const uint8_t servoId,
                                                         const uint8_t angle,
                                                         Frame        &response) {
            const Core::Status success = response.Build(EClusters::SERVO, EServoCommands::SET_MAX);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetOffset(const uint8_t servoId,
                                                       const uint8_t angle,
                                                       Frame        &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::GET_OFFSET);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetOffset(const uint8_t servoId,
                                                       const uint8_t angle,
                                                       Frame        &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::SET_OFFSET);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status
        ClusterServo::BuildFrameGetState(const uint8_t servoId, const bool state, Frame &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::GET_STATE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(static_cast<uint8_t>(state));
            }
            return (success);
        }

        Core::Status
        ClusterServo::BuildFrameSetState(const uint8_t servoId, const bool state, Frame &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::SET_STATE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(static_cast<uint8_t>(state));
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetReverse(const uint8_t servoId,
                                                        const bool    reversed,
                                                        Frame        &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::GET_REVERSE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(static_cast<uint8_t>(reversed));
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetReverse(const uint8_t servoId,
                                                        const bool    reversed,
                                                        Frame        &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::SET_REVERSE);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(static_cast<uint8_t>(reversed));
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetStatePca(const bool state, Frame &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::GET_STATE_PCA);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(static_cast<uint8_t>(state));
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetStatePca(const bool state, Frame &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::GET_STATE_PCA);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(static_cast<uint8_t>(state));
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameSetError(const Core::Status error,
                                                      const uint8_t      servoId,
                                                      const uint8_t      angle,
                                                      Frame             &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EClusterCommandGeneric::GENERIC);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(static_cast<uint8_t>(error));
                response.Set1ByteParam(angle);
            }
            return (success);
        }

        Core::Status ClusterServo::BuildFrameGetServoAllParams( const uint8_t      servoId,
                                                                const bool         state,
                                                                const uint8_t      angle,
                                                                const uint8_t      offset,
                                                                const uint8_t      min,
                                                                const uint8_t      max,
                                                                const bool         reversed,
                                                                Frame             &response) {
            const Core::Status success =
                    response.Build(EClusters::SERVO, EServoCommands::GET_SERVO_ALL);
            if (success == Core::Status::CORE_OK) {
                response.Set1ByteParam(servoId);
                response.Set1ByteParam(static_cast<uint8_t>(state));
                response.Set1ByteParam(angle);
                response.Set1ByteParam(offset);
                response.Set1ByteParam(min);
                response.Set1ByteParam(max);
                response.Set1ByteParam(static_cast<uint8_t>(reversed));
            }
            return (success);

        }
    }; // namespace Servo
} // namespace Cluster
