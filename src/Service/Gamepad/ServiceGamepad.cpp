#include "ServiceGamepad.h"
#include "../../Cluster/Body/ClusterBody.h"

#include <cmath>

namespace Service
{
    namespace Gamepad
    {
        using Move::Gait::GaitType;
        using Move::Walk::EWalkStatus;

        // Defaults applied while a stick is at rest.
        static constexpr float    DEFAULT_AMPLITUDE      = 0.0F;
        static constexpr float    DEFAULT_ROTATION_ANGLE = 0.0F;
        // Step duration is bumped by L1 / R1 between these bounds.
        static constexpr uint16_t MIN_DURATION_MS        = 600U;
        static constexpr uint16_t MAX_DURATION_MS        = 3000U;
        static constexpr uint16_t DURATION_STEP_MS       = 200U;
        // Body elevation bump on D-pad up / down.
        static constexpr float    ELEVATION_STEP         = 5.0F;
        // Stop-to-start transition time.
        static constexpr uint16_t WALK_TRANSITION_MS     = 1000U;

        ServiceGamepad::ServiceGamepad(GamepadInterface                &gamepad,
                                       Bot::Body::BodyInterface        &body,
                                       Message::MessageInterface       &messageListener,
                                       Event::EventDispatcherInterface &eventDispatcher)
            : Service(GAMEPAD, 20U, messageListener, eventDispatcher)
            , mGamepad(gamepad)
            , mBody(body)
            , mPrevButtons{} {
        }

        Core::Status ServiceGamepad::Initialize() {
            const Core::Status success = this->mGamepad.Initialize();
            if (Core::IsSuccess(success)) {
                this->GetEventDispatcher().AddListener(this);
                static_cast<Core::Observable<3U, GamepadButtonState> &>(this->mGamepad)
                        .Attach(static_cast<Core::ObserverInterface<GamepadButtonState> *>(this));
                static_cast<Core::Observable<3U, GamepadConnectedState> &>(this->mGamepad)
                        .Attach(static_cast<Core::ObserverInterface<GamepadConnectedState> *>(this));
                this->mInitialized = true;
            }
            return success;
        }

        void ServiceGamepad::Update(const uint64_t currentTime) {
            this->mGamepad.Update(currentTime);
        }

        void ServiceGamepad::Notified(const GamepadButtonState &state) {
            this->HandleButtonEdges(state);
            this->ApplyMotion(state);
            
            Frame asyncStatus;
            const Move::Walk::EWalkStatus walkStatus = this->mBody.GetWalkStatus();
            const Move::Gait::GaitType gait = this->mBody.GetGait();
            const uint8_t  amplitude = this->mBody.GetAmplitude() * 10U;
            const uint8_t  elevation = this->mBody.GetElevation() * 10U;
            const uint16_t direction = Misc::Utils::ToDeg(this->mBody.GetDirection());
            const uint16_t rotation = Misc::Utils::ToDeg(this->mBody.GetRotation());
            const bool     clockWize = this->mBody.GetRotationClockWize();
            const uint16_t duration = this->mBody.GetDuration();
            Cluster::Body::ClusterBody::BuildFrameUpdateAll(asyncStatus, walkStatus, gait, amplitude, elevation, direction, rotation, clockWize, duration);
            this->SendMessage(asyncStatus);
        }

        void ServiceGamepad::Notified(const GamepadConnectedState &state) {
            this->DispatchEvent<GamepadConnectedState>(EventType::EVENT_GAMEPAD_UPDATE, state);
        }

        void ServiceGamepad::OnEvent(const Event::Event &event) {
            (void) event;
        }

        void ServiceGamepad::ApplyMotion(const GamepadButtonState &state) {
            // Joystick Y is positive when pulled down. Flip so "up" means forward.
            const float lx = state.axes[AXIS_LEFT_X];
            const float ly = -state.axes[AXIS_LEFT_Y];
            const float rx = state.axes[AXIS_RIGHT_X];

            const float magnitude = std::sqrt((lx * lx) + (ly * ly));
            if (magnitude > 0.0F) {
                // atan2 -> radians; convert to degrees (0..360) so it matches the rest of the API.
                float angleDeg = std::atan2(ly, lx) * 180.0F / static_cast<float>(M_PI);
                this->mBody.SetDirection(angleDeg);
                this->mBody.SetAmplitude(magnitude > 1.0F ? 1.0F : magnitude);
            } else {
                this->mBody.SetAmplitude(DEFAULT_AMPLITUDE);
            }

            if (rx != 0.0F) {
                const bool  clockwise = rx > 0.0F;
                const float absRx     = clockwise ? rx : -rx;
                // Map [0..1] to a sensible angular step in degrees per cycle.
                this->mBody.SetRotation(Misc::Utils::ToRad(absRx * 30.0F), clockwise);
            } else {
                this->mBody.SetRotation(DEFAULT_ROTATION_ANGLE, true);
            }
        }

        void ServiceGamepad::HandleButtonEdges(const GamepadButtonState &state) {
            // Helper: detect press (rising edge).
            auto pressed = [&](const uint8_t btn) {
                const bool now  = state.buttons[btn];
                const bool prev = this->mPrevButtons[btn];
                this->mPrevButtons[btn] = now;
                return now && !prev;
            };

            // We still want to update prev for every button so make a pass instead of mixing.
            bool edgeCross    = pressed(BTN_CROSS);
            bool edgeTriangle = pressed(BTN_TRIANGLE);
            bool edgeCircle   = pressed(BTN_CIRCLE);
            bool edgeL1       = pressed(BTN_L1);
            bool edgeR1       = pressed(BTN_R1);
            bool edgeDUp      = pressed(BTN_DPAD_UP);
            bool edgeDDown    = pressed(BTN_DPAD_DOWN);
            // Keep prev state up to date for the remaining (unused for now) buttons.
            for (uint8_t i = 0U; i < NB_BUTTONS; i++) {
                this->mPrevButtons[i] = state.buttons[i];
            }

            if (edgeCross) {
                const EWalkStatus status = (this->mBody.GetWalkStatus() == EWalkStatus::WALKING)
                                                   ? EWalkStatus::STOPPED
                                                   : EWalkStatus::WALKING;
                this->mBody.UpdateWalkStatus(status, WALK_TRANSITION_MS);
                LOG_SERVICE_INFO("Gamepad walk -> %s.", Move::Walk::WalkStatusToString(status));
            }

            if (edgeCircle) {
                this->mBody.UpdateWalkStatus(EWalkStatus::STOPPED, WALK_TRANSITION_MS);
                LOG_SERVICE_INFO("Gamepad emergency stop.");
            }

            if (edgeTriangle) {
                const uint8_t current = static_cast<uint8_t>(this->mBody.GetGait());
                const uint8_t next =
                        static_cast<uint8_t>((current + 1U) %
                                              static_cast<uint8_t>(GaitType::NB_GAITS));
                this->mBody.SetGait(static_cast<GaitType>(next));
                LOG_SERVICE_INFO("Gamepad gait -> %s.",
                                 Move::Gait::GaitTypeToString(static_cast<GaitType>(next)));
            }

            if (edgeL1) {
                const uint16_t d = this->mBody.GetDuration();
                const uint16_t newD =
                        (d + DURATION_STEP_MS > MAX_DURATION_MS) ? MAX_DURATION_MS
                                                                  : (d + DURATION_STEP_MS);
                this->mBody.SetDuration(newD);
            }
            if (edgeR1) {
                const uint16_t d = this->mBody.GetDuration();
                const uint16_t newD =
                        (d < MIN_DURATION_MS + DURATION_STEP_MS) ? MIN_DURATION_MS
                                                                  : (d - DURATION_STEP_MS);
                this->mBody.SetDuration(newD);
            }

            if (edgeDUp) {
                this->mBody.SetElevation(this->mBody.GetElevation() + ELEVATION_STEP);
            }
            if (edgeDDown) {
                this->mBody.SetElevation(this->mBody.GetElevation() - ELEVATION_STEP);
            }
        }
    } // namespace Gamepad
} // namespace Service
