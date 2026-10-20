#pragma once

#include "../../Bot/Body/BodyInterface.h"
#include "../../Component/Gamepad/Gamepad.h"
#include "../../Core/ObserverInterface.h"
#include "../Service.h"

namespace Service
{
    namespace Gamepad
    {
        using namespace Component::Gamepad;

        /// Translates PS3 controller input into hexapod motion commands.
        ///
        /// Mapping (PS3 / Sixaxis):
        ///   - Left stick           : walk direction (angle) and amplitude (magnitude)
        ///   - Right stick X        : in-place rotation (left/right)
        ///   - Cross (X)            : toggle WALKING / STOPPED
        ///   - Triangle             : cycle gait (TRIPOD -> WAVE -> RIPPLE)
        ///   - Circle               : emergency stop
        ///   - D-Pad Up / Down      : raise / lower body
        ///   - L1 / R1              : slower / faster cycle (step duration)
        class ServiceGamepad : public Service,
                               Core::ObserverInterface<GamepadButtonState>,
                               Core::ObserverInterface<GamepadConnectedState>,
                               Event::EventListenerInterface {
        public:
            ServiceGamepad(GamepadInterface                &gamepad,
                           Bot::Body::BodyInterface        &body,
                           Message::MessageInterface       &messageListener,
                           Event::EventDispatcherInterface &eventDispatcher);

            ~ServiceGamepad() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         Notified(const GamepadButtonState &state) final override;

            virtual void         Notified(const GamepadConnectedState &state) final override;

            virtual void         OnEvent(const Event::Event &event) final override;

        private:
            void ApplyMotion(const GamepadButtonState &state);
            void HandleButtonEdges(const GamepadButtonState &state);

            GamepadInterface         &mGamepad;
            Bot::Body::BodyInterface &mBody;
            bool                      mPrevButtons[NB_BUTTONS];
        };
    } // namespace Gamepad
} // namespace Service
