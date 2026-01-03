#pragma once

#include "../../Component/Servos/ServosInterface.h"
#include "../ClusterBase.h"

namespace Cluster
{
    namespace Servo
    {
        using namespace Component::Servos;

        class ClusterServo : public ClusterBase, ClusterCommand {
        public:
            explicit ClusterServo(ServosInterface &servos);

            ~ClusterServo() = default;

            virtual Core::Status ExecuteFrame(const Frame &request,
                                              Frame       &response) override;

            static Core::Status
            BuildFrameAllAngle(const uint8_t angles[NB_SERVOS],
                               Frame        &response);

            static Core::Status BuildFrameGetAngle(const uint8_t servoId,
                                                   const uint8_t angle,
                                                   Frame        &response);

            static Core::Status BuildFrameSetAngle(const uint8_t servoId,
                                                   const uint8_t angle,
                                                   Frame        &response);

            static Core::Status BuildFrameGetMinAngle(const uint8_t servoId,
                                                      const uint8_t angle,
                                                      Frame        &response);

            static Core::Status BuildFrameSetMinAngle(const uint8_t servoId,
                                                      const uint8_t angle,
                                                      Frame        &response);

            static Core::Status BuildFrameGetMaxAngle(const uint8_t servoId,
                                                      const uint8_t angle,
                                                      Frame        &response);

            static Core::Status BuildFrameSetMaxAngle(const uint8_t servoId,
                                                      const uint8_t angle,
                                                      Frame        &response);

            static Core::Status BuildFrameGetOffset(const uint8_t servoId,
                                                    const uint8_t angle,
                                                    Frame        &response);

            static Core::Status BuildFrameSetOffset(const uint8_t servoId,
                                                    const uint8_t angle,
                                                    Frame        &response);

            static Core::Status BuildFrameGetState(const uint8_t servoId,
                                                   const bool    state,
                                                   Frame        &response);

            static Core::Status BuildFrameSetState(const uint8_t servoId,
                                                   const bool    state,
                                                   Frame        &response);

            static Core::Status BuildFrameGetReverse(const uint8_t servoId,
                                                     const bool    reversed,
                                                     Frame        &response);

            static Core::Status BuildFrameSetReverse(const uint8_t servoId,
                                                     const bool    reversed,
                                                     Frame        &response);

            static Core::Status BuildFrameGetStatePca(const bool state,
                                                      Frame     &response);

            static Core::Status BuildFrameSetStatePca(const bool state,
                                                      Frame     &response);

            static Core::Status BuildFrameSetError(const Core::Status error,
                                                   const uint8_t      servoId,
                                                   const uint8_t      angle,
                                                   Frame             &response);

        private:
            ServosInterface &mServosInterface;
        };
    } // namespace Servo
} // namespace Cluster
